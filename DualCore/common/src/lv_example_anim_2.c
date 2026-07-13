#include "../../lvgl/examples/lv_examples.h"

static void anim1_x_cb(void * var, int32_t v)
{
    lv_obj_set_x((lv_obj_t *) var, v);
}

static void anim1_size_cb(void * var, int32_t v)
{
    lv_obj_set_size((lv_obj_t *) var, v, v);
}

/**
 * Create a playback animation
 */
void lv_example_anim_2(void)
{

    lv_obj_t * obj = lv_obj_create(lv_screen_active());
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(obj,  lv_color_hex(0xFF0000), 0);
    lv_obj_set_style_radius(obj, LV_RADIUS_CIRCLE, 0);

    lv_obj_align(obj, LV_ALIGN_LEFT_MID, 10, 0);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_values(&a, 10, 50);
    lv_anim_set_duration(&a, 1000);
    lv_anim_set_reverse_delay(&a, 100);
    lv_anim_set_reverse_duration(&a, 1000);
    lv_anim_set_repeat_delay(&a, 500);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);

    lv_anim_set_exec_cb(&a, anim1_size_cb);
    lv_anim_start(&a);
    lv_anim_set_exec_cb(&a, anim1_x_cb);
    lv_anim_set_values(&a, 10, 180);
    lv_anim_start(&a);
}

LV_IMAGE_DECLARE(img_needle00);

static lv_obj_t * needle_line;
static lv_obj_t * needle_img;

static void set_needle_line_value(void * obj, int32_t v)
{
    lv_scale_set_line_needle_value((lv_obj_t *)obj, needle_line, 60, v);
}

static void set_needle_img_value(void * obj, int32_t v)
{
    lv_scale_set_image_needle_value((lv_obj_t *)obj, needle_img, v);
}

void lv_example_scale_3(void)
{
    lv_obj_t * scale_img = lv_scale_create(lv_screen_active());
    lv_obj_set_size(scale_img, 240, 240);
    lv_scale_set_mode(scale_img, LV_SCALE_MODE_ROUND_INNER);
    lv_obj_set_style_bg_opa(scale_img, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(scale_img, lv_palette_lighten(LV_PALETTE_RED, 5), 0);
    lv_obj_set_style_radius(scale_img, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_clip_corner(scale_img, true, 0);
    lv_obj_align(scale_img, LV_ALIGN_CENTER, 0, 0);

    lv_scale_set_label_show(scale_img, true);

    lv_scale_set_total_tick_count(scale_img, 31);
    lv_scale_set_major_tick_every(scale_img, 5);

    lv_obj_set_style_length(scale_img, 5, LV_PART_ITEMS);
    lv_obj_set_style_length(scale_img, 10, LV_PART_INDICATOR);
    lv_scale_set_range(scale_img, 10, 40);

    lv_scale_set_angle_range(scale_img, 270);
    lv_scale_set_rotation(scale_img, 135);

    /* image must point to the right. E.g. -O------>*/
    needle_img = lv_image_create(scale_img);
    lv_image_set_src(needle_img, &img_needle00);
    lv_obj_align(needle_img, LV_ALIGN_CENTER, 47, 0);
    lv_image_set_pivot(needle_img, 3, 4);

    lv_anim_t anim_scale_img;
    lv_anim_init(&anim_scale_img);
    lv_anim_set_var(&anim_scale_img, scale_img);
    lv_anim_set_exec_cb(&anim_scale_img, set_needle_img_value);
    lv_anim_set_duration(&anim_scale_img, 4000);
    lv_anim_set_repeat_count(&anim_scale_img, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_reverse_duration(&anim_scale_img, 4000);
    lv_anim_set_values(&anim_scale_img, 10, 40);
    lv_anim_start(&anim_scale_img);
}

