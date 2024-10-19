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

extern uint32_t g_fezui_fps;
extern uint8_t g_kps_history_max;
extern char g_fpsstr[8];
extern uint32_t g_fezui_debug;

extern uint32_t g_fezui_run_time;
extern uint32_t g_usb_report_count;
extern uint32_t g_usb_report_count1;
extern uint32_t g_usb_interval;

extern fezui_notification_t fezui_notification;

extern const char *g_hid_usage_names[138];

extern fezui_frame_t g_mainframe;

extern fezui_page_t homepage;

extern fezui_page_t menupage;
void menupage_init();

extern fezui_page_t settingspage;
void settingspage_init();

extern fezui_page_t advancedsettingspage;
void advancedsettingspage_init();

extern fezui_page_t oscilloscopepage;

extern fezui_page_t keylistpage;
void keylistpage_init();
extern uint16_t* current_target_id;


extern fezui_page_t advancedconfigpage;
void advancedconfigpage_init();
extern AdvancedKey* current_config_advanced_key;

extern fezui_page_t displayconfigpage;
void displayconfigpage_init();

extern fezui_page_t panelpage;
void panelpage_init();

extern fezui_page_t keymappage;
void keymappage_init();

extern fezui_page_t debugpage;
void debugpage_init();

extern fezui_page_t calibrationpage;
void calibrationpage_init();

extern fezui_page_t rgbconfigpage;
void rgbconfigpage_init();

extern fezui_page_t statisticpage;

extern fezui_page_t aboutpage;

extern fezui_page_t calibrationpage;
void calibrationpage_init();


#endif /* FEZUI_VAR_H_ */
