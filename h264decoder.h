#ifndef H264DECODER_H_ZCW945MJ
#define H264DECODER_H_ZCW945MJ

#include <vdpau/vdpau.h>
#include <stdint.h>
#include <stdlib.h>
#include "context.h"

typedef struct {
	vdp_context *vdp;
	int fd;
	int current_frame;
	int current_surface;
	uint8_t *buf;
	uint8_t *p;
	size_t bufsize;

	uint32_t width, height;
	uint32_t numframes;
	VdpDecoder decoder;
	VdpDecoderProfile profile;
	VdpVideoSurface surfaces[NUMSURFACES];
	int *refframes;
} h264decoder_ctx;

typedef struct {
	VdpPictureInfoH264 info;
	size_t data_length;
	uint8_t *data;
} h264_frame;

h264decoder_ctx *h264decoder_create(vdp_context *vdp, const char *filename);
void h264decoder_free(h264decoder_ctx *ctx);
void h264decoder_init(h264decoder_ctx *ctx);
void h264decoder_rewind(h264decoder_ctx *ctx);
VdpVideoSurface h264decoder_get_next_frame(h264decoder_ctx *ctx);

#endif /* end of include guard: H264DECODER_H_ZCW945MJ */
