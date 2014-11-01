#include "context.h"
#include <stdlib.h>
#include <stdio.h>

static VdpStatus get_func_impl(vdp_context *ctx, int func, char *fname, void **p)
{
	VdpStatus status = ctx->vdp_get_proc_address(ctx->vdp_device, func, p);
	if (status != VDP_STATUS_OK) {
		fprintf(stderr, "Can't get ");
		fprintf(stderr, fname);
		fprintf(stderr, " address\n");
	}
	return status;
}

#define get_func(ctx, func, p) get_func_impl(ctx, func, #func, p)

vdp_context *vdp_context_create(void)
{
	return (vdp_context *)malloc(sizeof(vdp_context));
}

void vdp_context_free(vdp_context *context)
{
	free(context);
}

VdpStatus vdp_context_initialize(vdp_context *context)
{
	context->vdp_get_proc_address = NULL;

	Display display;
	context->vdp_device = VDP_INVALID_HANDLE;
	VdpStatus status = vdp_imp_device_create_x11(&display, 0, &context->vdp_device, &context->vdp_get_proc_address);

	if (status != VDP_STATUS_OK) {
		fprintf(stderr, "Can't create vdp device : ");
		if (status == VDP_STATUS_NO_IMPLEMENTATION) {
			fprintf(stderr, "No vdpau implementation.\n");
		}
		else {
			fprintf(stderr, "Unsupported GPU.\n");
		}
		return VDP_STATUS_ERROR;
	}

	if (context->vdp_device == VDP_INVALID_HANDLE) {
		fprintf(stderr, "Invalid VdpDevice handle\n");
		return VDP_STATUS_ERROR;
	}


	if (!context->vdp_get_proc_address) {
		fprintf(stderr, "vdp_get_proc_address is NULL !!\n");
		return VDP_STATUS_ERROR;
	}

	if (get_func(context, VDP_FUNC_ID_GET_ERROR_STRING , (void**)&context->vdp_get_error_string))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_GET_API_VERSION , (void**)&context->vdp_get_api_version))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_GET_INFORMATION_STRING , (void**)&context->vdp_get_information_string))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_DEVICE_DESTROY , (void**)&context->vdp_device_destroy))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_GENERATE_CSC_MATRIX , (void**)&context->vdp_generate_csc_matrix))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_VIDEO_SURFACE_QUERY_CAPABILITIES , (void**)&context->vdp_video_surface_query_capabilities))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_VIDEO_SURFACE_QUERY_GET_PUT_BITS_Y_CB_CR_CAPABILITIES , (void**)&context->vdp_video_surface_query_get_put_bits_y_cb_cr_capabilities))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_VIDEO_SURFACE_CREATE , (void**)&context->vdp_video_surface_create))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_VIDEO_SURFACE_DESTROY , (void**)&context->vdp_video_surface_destroy))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_VIDEO_SURFACE_GET_PARAMETERS , (void**)&context->vdp_video_surface_get_parameters))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_VIDEO_SURFACE_GET_BITS_Y_CB_CR , (void**)&context->vdp_video_surface_get_bits_y_cb_cr))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_VIDEO_SURFACE_PUT_BITS_Y_CB_CR , (void**)&context->vdp_video_surface_put_bits_y_cb_cr))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_OUTPUT_SURFACE_QUERY_CAPABILITIES , (void**)&context->vdp_output_surface_query_capabilities))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_OUTPUT_SURFACE_QUERY_GET_PUT_BITS_NATIVE_CAPABILITIES , (void**)&context->vdp_output_surface_query_get_put_bits_native_capabilities))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_OUTPUT_SURFACE_QUERY_PUT_BITS_INDEXED_CAPABILITIES , (void**)&context->vdp_output_surface_query_put_bits_indexed_capabilities))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_OUTPUT_SURFACE_QUERY_PUT_BITS_Y_CB_CR_CAPABILITIES , (void**)&context->vdp_output_surface_query_put_bits_y_cb_cr_capabilities))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_OUTPUT_SURFACE_CREATE , (void**)&context->vdp_output_surface_create))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_OUTPUT_SURFACE_DESTROY , (void**)&context->vdp_output_surface_destroy))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_OUTPUT_SURFACE_GET_PARAMETERS , (void**)&context->vdp_output_surface_get_parameters))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_OUTPUT_SURFACE_GET_BITS_NATIVE , (void**)&context->vdp_output_surface_get_bits_native))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_OUTPUT_SURFACE_PUT_BITS_NATIVE , (void**)&context->vdp_output_surface_put_bits_native))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_OUTPUT_SURFACE_PUT_BITS_INDEXED , (void**)&context->vdp_output_surface_put_bits_indexed))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_OUTPUT_SURFACE_PUT_BITS_Y_CB_CR , (void**)&context->vdp_output_surface_put_bits_y_cb_cr))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_BITMAP_SURFACE_QUERY_CAPABILITIES , (void**)&context->vdp_bitmap_surface_query_capabilities))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_BITMAP_SURFACE_CREATE , (void**)&context->vdp_bitmap_surface_create))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_BITMAP_SURFACE_DESTROY , (void**)&context->vdp_bitmap_surface_destroy))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_BITMAP_SURFACE_GET_PARAMETERS , (void**)&context->vdp_bitmap_surface_get_parameters))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_BITMAP_SURFACE_PUT_BITS_NATIVE , (void**)&context->vdp_bitmap_surface_put_bits_native))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_OUTPUT_SURFACE_RENDER_OUTPUT_SURFACE , (void**)&context->vdp_output_surface_render_output_surface))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_OUTPUT_SURFACE_RENDER_BITMAP_SURFACE , (void**)&context->vdp_output_surface_render_bitmap_surface))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_DECODER_QUERY_CAPABILITIES , (void**)&context->vdp_decoder_query_capabilities))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_DECODER_CREATE , (void**)&context->vdp_decoder_create))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_DECODER_DESTROY , (void**)&context->vdp_decoder_destroy))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_DECODER_GET_PARAMETERS , (void**)&context->vdp_decoder_get_parameters))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_DECODER_RENDER , (void**)&context->vdp_decoder_render))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_VIDEO_MIXER_QUERY_FEATURE_SUPPORT , (void**)&context->vdp_video_mixer_query_feature_support))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_VIDEO_MIXER_QUERY_PARAMETER_SUPPORT , (void**)&context->vdp_video_mixer_query_parameter_support))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_VIDEO_MIXER_QUERY_ATTRIBUTE_SUPPORT , (void**)&context->vdp_video_mixer_query_attribute_support))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_VIDEO_MIXER_QUERY_PARAMETER_VALUE_RANGE , (void**)&context->vdp_video_mixer_query_parameter_value_range))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_VIDEO_MIXER_QUERY_ATTRIBUTE_VALUE_RANGE , (void**)&context->vdp_video_mixer_query_attribute_value_range))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_VIDEO_MIXER_CREATE , (void**)&context->vdp_video_mixer_create))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_VIDEO_MIXER_SET_FEATURE_ENABLES , (void**)&context->vdp_video_mixer_set_feature_enables))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_VIDEO_MIXER_SET_ATTRIBUTE_VALUES , (void**)&context->vdp_video_mixer_set_attribute_values))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_VIDEO_MIXER_GET_FEATURE_SUPPORT , (void**)&context->vdp_video_mixer_get_feature_support))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_VIDEO_MIXER_GET_FEATURE_ENABLES , (void**)&context->vdp_video_mixer_get_feature_enables))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_VIDEO_MIXER_GET_PARAMETER_VALUES , (void**)&context->vdp_video_mixer_get_parameter_values))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_VIDEO_MIXER_GET_ATTRIBUTE_VALUES , (void**)&context->vdp_video_mixer_get_attribute_values))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_VIDEO_MIXER_DESTROY , (void**)&context->vdp_video_mixer_destroy))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_VIDEO_MIXER_RENDER , (void**)&context->vdp_video_mixer_render))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_PRESENTATION_QUEUE_TARGET_CREATE_X11 , (void**)&context->vdp_presentation_queue_target_create_x11))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_PRESENTATION_QUEUE_TARGET_DESTROY , (void**)&context->vdp_presentation_queue_target_destroy))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_PRESENTATION_QUEUE_CREATE , (void**)&context->vdp_presentation_queue_create))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_PRESENTATION_QUEUE_DESTROY , (void**)&context->vdp_presentation_queue_destroy))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_PRESENTATION_QUEUE_SET_BACKGROUND_COLOR , (void**)&context->vdp_presentation_queue_set_background_color))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_PRESENTATION_QUEUE_GET_BACKGROUND_COLOR , (void**)&context->vdp_presentation_queue_get_background_color))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_PRESENTATION_QUEUE_GET_TIME , (void**)&context->vdp_presentation_queue_get_time))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_PRESENTATION_QUEUE_DISPLAY , (void**)&context->vdp_presentation_queue_display))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_PRESENTATION_QUEUE_BLOCK_UNTIL_SURFACE_IDLE , (void**)&context->vdp_presentation_queue_block_until_surface_idle))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_PRESENTATION_QUEUE_QUERY_SURFACE_STATUS , (void**)&context->vdp_presentation_queue_query_surface_status))
		return VDP_STATUS_ERROR;
	if (get_func(context, VDP_FUNC_ID_PREEMPTION_CALLBACK_REGISTER , (void**)&context->vdp_preemption_callback_register))
		return VDP_STATUS_ERROR;

	return VDP_STATUS_OK;
}
