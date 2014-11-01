#ifndef H264DECODER_H_ZCW945MJ
#define H264DECODER_H_ZCW945MJ

#include <vdpau/vdpau.h>
#include <stdint.h>
#include "context.h"

#define FRAMESINSAMPLE 25

typedef struct {
	vdp_context *vdp;
	int fd;
	int current_frame;
	int current_surface;
	uint8_t *buf;
	uint8_t *p;
	size_t bufsize;

	uint32_t width, height;
	double ratio;
	VdpDecoder decoder;
	VdpDecoderProfile profile;
	VdpVideoSurface surfaces[NUMSURFACES];
	int refframes[FRAMESINSAMPLE];
} h264decoder_ctx;

typedef struct {
	VdpPictureInfoH264 info;
	size_t data_length;
	uint8_t *data;
} h264_frame;

h264decoder_ctx *h264decoder_create(vdp_context *vdp, int fd);
void h264decoder_free(h264decoder_ctx *ctx);
void h264decoder_init(h264decoder_ctx *ctx);
VdpVideoSurface h264decoder_get_next_frame(h264decoder_ctx *ctx);

#endif /* end of include guard: H264DECODER_H_ZCW945MJ */
