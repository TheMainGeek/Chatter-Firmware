#include "ContextMenu.h"
#include <lvgl.h>
#include "InputLVGL.h"

ContextMenu::ContextMenu(lv_obj_t* parent, const std::vector<std::string>& options) : LVObject(parent){
	lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLLABLE);

	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
	lv_obj_set_style_bg_color(obj, lv_color_hsv_to_rgb(0, 0, 50), 0);
	lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0);
	lv_obj_set_style_pad_gap(obj, 0, 0);
	lv_obj_set_style_border_width(obj, 1, 0);
	lv_obj_set_style_border_color(obj, lv_color_white(), 0);

	lv_style_init(&styleDef);
	lv_style_set_bg_color(&styleDef,lv_color_hsv_to_rgb(326, 20, 64));
	lv_style_set_bg_opa(&styleDef, LV_OPA_COVER);

	lv_style_init(&styleFocus);
	lv_style_set_bg_color(&styleFocus, lv_color_hsv_to_rgb(325, 91, 55));
	lv_style_set_bg_opa(&styleFocus, LV_OPA_COVER);

	lv_obj_set_width(obj, lv_pct(70));
	lv_obj_set_height(obj, LV_SIZE_CONTENT);

	menuInputGroup = lv_group_create();

	for(int i = 0; i < options.size(); ++i){
		lv_style_set_text_font(&styleFocus, &pixelbasic_7);
		lv_style_set_text_font(&styleDef, &pixelbasic_7);
		labelObj = lv_label_create(obj);
		char contextMessage[25];
		lv_label_set_recolor(labelObj,true);
		sprintf(contextMessage,"#ffffff %s",options[i].c_str());
		lv_label_set_text(labelObj, contextMessage);
		labelVector.push_back(labelObj);

		lv_obj_set_width(labelObj, lv_pct(100));
		lv_obj_set_height(labelObj, LV_SIZE_CONTENT);
		lv_obj_set_style_pad_all(labelObj, 2, 0);

		lv_obj_set_style_border_width(labelObj, 1, 0);
		lv_obj_set_style_border_color(labelObj, lv_color_white(), 0);
		lv_obj_add_style(labelObj, &styleFocus, LV_STATE_FOCUSED);
		lv_obj_add_style(labelObj, &styleDef, LV_STATE_DEFAULT);

		lv_group_add_obj(menuInputGroup, labelObj);

	}

}

void ContextMenu::startContextMenu(){

	lv_indev_set_group(InputLVGL::getInstance()->getIndev(), this->menuInputGroup);
}


