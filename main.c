#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <vdpau/vdpau.h>

#include <linux/fb.h>
#include <sys/ioctl.h>

#include "context.h"
#include "h264decoder.h"

#define SCREENWIDTH 800
#define SCREENHEIGHT 480

#define SURFACEWIDTH 800
#define SURFACEHEIGHT 480

#define BLOCKSIZE 4096

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("Usage:\n");
		printf("%s stream.h264\n", argv[0]);
		return -1;
	}

	int fb = open("/dev/fb0", O_RDWR);

	VdpOutputSurface display_surface;
	VdpVideoMixer mixer;
	VdpPresentationQueueTarget queue_target;
	VdpPresentationQueue queue;

	vdp_context *context = vdp_context_create();
	if (vdp_context_initialize(context) != VDP_STATUS_OK) {
		vdp_context_free(context);
		fprintf(stderr, "Context not initialized\n");
		return -1;
	}

	if (context->vdp_output_surface_create(context->vdp_device, VDP_RGBA_FORMAT_B8G8R8A8, SCREENWIDTH, SCREENHEIGHT, &display_surface) != VDP_STATUS_OK) {
		fprintf(stderr, "Surface not created\n");
		vdp_context_free(context);
		return -1;
	}

	VdpVideoMixerParameter params[] = {
		VDP_VIDEO_MIXER_PARAMETER_VIDEO_SURFACE_WIDTH,
		VDP_VIDEO_MIXER_PARAMETER_VIDEO_SURFACE_HEIGHT,
		VDP_VIDEO_MIXER_PARAMETER_CHROMA_TYPE,
		VDP_VIDEO_MIXER_PARAMETER_LAYERS
	};
	int num_layers = 3;
	int w = SURFACEWIDTH;
	int h = SURFACEHEIGHT;
	VdpChromaType chroma = VDP_CHROMA_TYPE_420;
	void const *param_values[] = { &w, &h, &chroma, &num_layers };
   VdpVideoMixerFeature mixer_features[0];
	if (context->vdp_video_mixer_create(context->vdp_device, 0, mixer_features, 4, params, param_values, &mixer) != VDP_STATUS_OK) {
		fprintf(stderr, "Mixer not created\n");
		vdp_context_free(context);
		return -1;
	}

	VdpStatus status = context->vdp_presentation_queue_target_create_x11(context->vdp_device, 1, &queue_target);
	if (status != VDP_STATUS_OK) {
		fprintf(stderr, "can't create queue target\n");
		vdp_context_free(context);
		return -1;
	}

	status = context->vdp_presentation_queue_create(context->vdp_device, queue_target, &queue);
	if (status != VDP_STATUS_OK) {
		fprintf(stderr, "can't create display queue\n");
		vdp_context_free(context);
		return -1;
	}

	h264decoder_ctx *decoder_ctx = h264decoder_create(context, argv[1]);
	if (decoder_ctx == NULL) {
		vdp_context_free(context);
		return -1;
	}
	h264decoder_init(decoder_ctx);

	for (;;) {
		VdpVideoSurface surface = h264decoder_get_next_frame(decoder_ctx);
		if (surface == VDP_INVALID_HANDLE) {
			break;
		}

		VdpRect vid_source = { 0, 0, decoder_ctx->width, decoder_ctx->height };
		VdpRect out_dest = { 0, 0, SCREENWIDTH, SCREENHEIGHT };
		VdpStatus status = context->vdp_video_mixer_render(mixer, VDP_INVALID_HANDLE, 0, VDP_VIDEO_MIXER_PICTURE_STRUCTURE_FRAME, 0, 0, surface, 0, 0, &vid_source, display_surface, &out_dest, &out_dest, 0, NULL);
		if (status != VDP_STATUS_OK) {
			fprintf(stderr, "Error while video mixer render\n");
		}

		if (argc == 2) {
			int arg = 0;
			ioctl(fb, FBIO_WAITFORVSYNC, &arg);
			ioctl(fb, FBIO_WAITFORVSYNC, &arg);
		}

		status = context->vdp_presentation_queue_display(queue, display_surface, 0, 0, 0);
		if (status != VDP_STATUS_OK) {
			fprintf(stderr, "Error while displaying\n");
		}
	}

	h264decoder_free(decoder_ctx);
	vdp_context_free(context);
	return 0;
}
