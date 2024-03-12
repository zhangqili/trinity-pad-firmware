/*
 * fezui_var.h
 *
 *  Created on: Jul 21, 2023
 *      Author: xq123
 */

#ifndef FEZUI_VAR_H_
#define FEZUI_VAR_H_
#include "fezui.h"
#include "keyboard.h"

extern const uint8_t fez_font_6x10_m[204] U8G2_FONT_SECTION("fez_font_6x10_m");
extern fezui_t fezui;
extern u8log_t u8log;
extern uint8_t u8log_buf[1024];

extern uint32_t fezui_fps;
extern uint8_t fezui_kps;
extern uint8_t KPS_history_max;
extern char fpsstr[8];
extern char comstr[8];
extern uint8_t UI_KPSMaximumPerSecond;
extern uint32_t fezui_debug;
extern float fezui_debug_float;
extern uint8_t cmd_buffer;
extern uint8_t fezui_adc_conversion_count;

extern uint32_t fezui_run_time;
extern uint32_t fezui_temp_raw;
extern uint32_t fezui_report_count;
extern uint32_t fezui_report_count1;

extern fezui_notification_t fezui_notification;

extern const char *hid_usage_names[138];



extern fezui_link_frame_t mainframe;

extern fezui_link_page_t homepage;

extern fezui_link_page_t menupage;
void menupage_init();


extern fezui_link_page_t settingspage;
void settingspage_init();


extern fezui_link_page_t advancedsettingspage;
void advancedsettingspage_init();

extern fezui_link_page_t oscilloscopepage;

extern fezui_link_page_t keylistpage;
void keylistpage_init();
extern uint16_t* current_target_id;

extern fezui_link_page_t keyconfigpage;
void keyconfigpage_init();
extern char current_key_name[32];

extern fezui_link_page_t knobconfigpage;
void knobconfigpage_init();
extern char current_knob_name[16];
extern uint16_t* current_target_knob_press_id;
extern uint16_t* current_target_knob_clockwise_id;
extern uint16_t* current_target_knob_anticlockwise_id;

extern fezui_link_page_t advancedconfigpage;
void advancedconfigpage_init();
extern AdvancedKey* current_config_advanced_key;

extern fezui_link_page_t displayconfigpage;
void displayconfigpage_init();

extern fezui_link_page_t panelpage;
void panelpage_init();

extern fezui_link_page_t keymappage;
void keymappage_init();

extern fezui_link_page_t debugpage;
void debugpage_init();

extern fezui_link_page_t calibrationpage;
void calibrationpage_init();

extern fezui_link_page_t rgbconfigpage;
void rgbconfigpage_init();

extern fezui_link_page_t statisticpage;

extern fezui_link_page_t aboutpage;


extern fezui_link_page_t calibrationpage;
void calibrationpage_init();


#endif /* FEZUI_VAR_H_ */
