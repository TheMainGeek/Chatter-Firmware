#include "PicMenu.h"
#include <Arduino.h>
#include "InputLVGL.h"

void arrowAnim(void* obj, int32_t v){
	lv_obj_set_style_translate_x((lv_obj_t*)obj, v, 0);
	lv_obj_invalidate(lv_obj_get_parent((lv_obj_t*)obj));
}

#define PIC_NUM 15

PicMenu::PicMenu(lv_obj_t* parent, void (* callback)(uint8_t, void*), void* userData) : LVObject(parent), returnCallback(callback),
																						userData(userData){
	lv_obj_set_style_bg_color(obj, lv_color_make(57, 49, 75), 0);
	lv_obj_set_style_bg_opa(obj, LV_OPA_100, 0);
	lv_obj_set_style_border_width(obj, 1, 0);
	lv_obj_set_style_border_color(obj, lv_color_white(), 0);
	lv_obj_set_style_border_opa(obj, LV_OPA_100, 0);
	lv_obj_set_size(obj, 116, 76);
	lv_obj_set_style_pad_ver(obj, 4, 0);
	lv_obj_set_style_pad_hor(obj, 7, 0);
	lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLLABLE);

	lv_obj_t* picLayout = lv_obj_create(obj);
	lv_obj_set_layout(picLayout, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(picLayout, LV_FLEX_FLOW_ROW);
	lv_obj_set_style_pad_gap(picLayout, 32, 0);
	lv_obj_set_flex_align(picLayout, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_align(picLayout, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_size(picLayout, 64, 64);
	lv_obj_set_scrollbar_mode(picLayout, LV_SCROLLBAR_MODE_OFF);


	group = lv_group_create();
	lv_group_set_wrap(group, false);
	group->user_data = this;
	lv_group_set_focus_cb(group, [](lv_group_t* group){
		size_t index = lv_obj_get_index(lv_group_get_focused(group));
		lv_obj_scroll_to_x(lv_obj_get_parent(lv_group_get_focused(group)), index * (64 + 32) , LV_ANIM_ON);
		static_cast<PicMenu*>(group->user_data)->refreshArrows(lv_obj_get_index(lv_group_get_focused(group)));
	});

	char imgPath[50];
	lv_obj_t* pic;
	for(int i = 0; i < PIC_NUM; ++i){
		if(i == 1 || i == 3 || i == 6 || i == 7 || i == 9 || i == 13){
			sprintf(imgPath, "S:/Pics/%d.gif", i);
			pic = lv_gif_create(picLayout);
			lv_gif_set_src(pic, imgPath);
		}else{
			sprintf(imgPath, "S:/Pics/%d.bin", i);
			pic = lv_img_create(picLayout);
			lv_img_set_src(pic, imgPath);
		}
		lv_obj_clear_flag(pic, LV_OBJ_FLAG_SCROLLABLE);
		lv_group_add_obj(group, pic);

		lv_obj_add_event_cb(pic, [](lv_event_t* e){
			static_cast<PicMenu*>(lv_event_get_user_data(e))->exit(lv_obj_get_index(lv_event_get_target(e)));
		}, LV_EVENT_CLICKED, this);

		lv_obj_add_event_cb(pic, [](lv_event_t* e){
			uint32_t key = lv_event_get_key(e);
			if(key == LV_KEY_ESC){
				static_cast<PicMenu*>(lv_event_get_user_data(e))->exit(0xFF);
			}
		}, LV_EVENT_KEY, this);

	}


	arrowLeft = lv_img_create(obj);
	lv_obj_set_style_text_color(arrowLeft, lv_color_white(), 0);
	lv_img_set_src(arrowLeft, LV_SYMBOL_LEFT);
	lv_obj_align(arrowLeft, LV_ALIGN_LEFT_MID, 0, 0);

	arrowRight = lv_img_create(obj);
	lv_obj_set_style_text_color(arrowRight, lv_color_white(), 0);
	lv_img_set_src(arrowRight, LV_SYMBOL_RIGHT);
	lv_obj_align(arrowRight, LV_ALIGN_RIGHT_MID, 0, 0);

	lv_anim_init(&anim);
	lv_anim_set_playback_time(&anim, 100);
	lv_anim_set_repeat_count(&anim, LV_ANIM_REPEAT_INFINITE);
	lv_anim_set_path_cb(&anim, lv_anim_path_step);
	lv_anim_set_exec_cb(&anim, arrowAnim);
}

void PicMenu::refreshArrows(uint8_t index){

	lv_anim_del(arrowLeft, arrowAnim);
	lv_anim_del(arrowRight, arrowAnim);

	if(index > 0){
		lv_anim_set_values(&anim, -3, 0);
		lv_anim_set_var(&anim, arrowLeft);
		lv_anim_start(&anim);
	}
	if(index < PIC_NUM){
		lv_anim_set_values(&anim, 3, 0);
		lv_anim_set_var(&anim, arrowRight);
		lv_anim_start(&anim);
	}
}

void PicMenu::exit(uint8_t picID){
	lv_indev_set_group(InputLVGL::getInstance()->getIndev(), oldGroup);
	lv_anim_del(arrowRight, arrowAnim);
	lv_anim_del(arrowLeft, arrowAnim);
	returnCallback(picID, userData);
}

void PicMenu::enter(lv_group_t* oldGroup){
	this->oldGroup = oldGroup;
	lv_indev_set_group(InputLVGL::getInstance()->getIndev(), group);
	refreshArrows(0);

}
