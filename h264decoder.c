#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include "h264decoder.h"


h264decoder_ctx *h264decoder_create(vdp_context *vdp, const char *filename)
{
	int fd = open(filename, O_RDONLY);
	if (fd == -1) {
		fprintf(stderr, "Error while opening file\n");
		return NULL;
	}

	h264decoder_ctx *ctx = (h264decoder_ctx *)malloc(sizeof(h264decoder_ctx));
	ctx->vdp = vdp;
	ctx->fd = fd;
	ctx->current_frame = 0;
	ctx->current_surface = 0;

	struct stat sb;
	fstat(fd, &sb);
	ctx->bufsize = sb.st_size;
	ctx->buf = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if (ctx->buf == MAP_FAILED) {
		fprintf(stderr, "mmap failed\n");
		ctx->buf = NULL;
	}
	ctx->p = ctx->buf;

	return ctx;
}

void h264decoder_free(h264decoder_ctx *ctx)
{
	if (ctx->buf) {
		munmap(ctx->buf, ctx->bufsize);
	}
	free(ctx);
}

void h264decoder_init(h264decoder_ctx *ctx)
{

	ctx->width = *(int *)(ctx->p);
	ctx->p += 4;
	ctx->height = *(int *)(ctx->p);
	ctx->p += 4;
	ctx->ratio = *(int *)(ctx->p);
	ctx->p += 8;
	ctx->profile = *(int *)(ctx->p);
	ctx->p += 4;

	VdpStatus status = ctx->vdp->vdp_decoder_create(ctx->vdp->vdp_device, ctx->profile, ctx->width, ctx->height, 16, &ctx->decoder);
	if (status != VDP_STATUS_OK) {
		fprintf(stderr, "Failed vdp_decoder_create");
	}

	for (int i = 0; i < NUMSURFACES; ++i) {
		ctx->surfaces[i] = VDP_INVALID_HANDLE;
		status = ctx->vdp->vdp_video_surface_create(ctx->vdp->vdp_device, VDP_CHROMA_TYPE_420, ctx->width, ctx->height, &ctx->surfaces[i]);
		if (status != VDP_STATUS_OK) {
			fprintf(stderr, "Surfaces not created");
			return;
		}
	}
}

VdpVideoSurface h264decoder_get_next_frame(h264decoder_ctx *ctx)
{
	h264_frame frame;
	frame.info = *(VdpPictureInfoH264 *)(ctx->p);
	VdpPictureInfoH264 info = frame.info;
	ctx->p += sizeof(VdpPictureInfoH264);
	frame.data_length = *(int *)(ctx->p);
	ctx->p += 4;
	frame.data = ctx->p;
	ctx->p += frame.data_length;
	ctx->current_frame++;

	for (int i = 0; i < 16; ++i) {
		if (info.referenceFrames[i].surface != VDP_INVALID_HANDLE) {
			info.referenceFrames[i].surface = ctx->refframes[info.referenceFrames[i].surface];
		}
	}

	VdpVideoSurface current = ctx->surfaces[ctx->current_surface];

	VdpBitstreamBuffer vbit;
	vbit.struct_version = VDP_BITSTREAM_BUFFER_VERSION;
	vbit.bitstream = frame.data;
	vbit.bitstream_bytes = frame.data_length;

	VdpStatus status = ctx->vdp->vdp_decoder_render(ctx->decoder, current, (VdpPictureInfo*)&info, 1, &vbit);
	if (status != VDP_STATUS_OK) {
		fprintf(stderr, "Error while decoding frame\n");
	}

	ctx->refframes[ctx->current_frame-1] = current;
	ctx->current_surface++;
	if (ctx->current_surface >= NUMSURFACES) {
		ctx->current_surface = 0;
	}

	return current;
}
