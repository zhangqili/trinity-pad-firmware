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

#define KEYBOARD_CHILD_NUM 4
#define KEYBOARD_CHILD_KEY_MAX_NUM 32
#define KEYBOARD_TREE_BUFFER_MAX_SIZE 256

extern bool eeprom_enable;
extern const uint8_t fez_font_6x10_m[204] U8G2_FONT_SECTION("fez_font_6x10_m");
extern fezui_t fezui;
extern u8log_t u8log;
extern uint8_t u8log_buf[1024];
extern uint8_t Keyriggered_count[4];


extern uint32_t fezui_keytotalcounts[ADVANCED_KEY_NUM];
extern uint32_t fezui_keyinitcounts[ADVANCED_KEY_NUM];
extern uint32_t fezui_fps;
extern uint8_t fezui_kps;
extern uint8_t KPS_history_max;
extern char fpsstr[8];
extern char comstr[8];
extern lefl_loop_array_t KPS_history;
extern lefl_array_elm_t KPS_history_data[KPS_HISTORY_LENGTH];
extern lefl_loop_array_t KPS_queue;
extern lefl_array_elm_t KPS_queue_data[REFRESH_RATE];
extern uint8_t UI_KPSMaximumPerSecond;
extern uint32_t fezui_debug;
extern float fezui_debug_float;
extern uint8_t cmd_buffer;
extern uint8_t current_key_index;
extern uint8_t current_function_key;
extern uint8_t fezui_adc_conversion_count;

extern uint32_t fezui_run_time;
extern uint32_t fezui_temp_raw;
extern uint32_t fezui_report_count;
extern uint32_t fezui_report_count1;

extern fezui_notification_t fezui_notification;

extern lefl_loop_array_t analog_historys[4];
extern lefl_array_elm_t analog_history1_data[HISTORY_LENGTH];
extern lefl_array_elm_t analog_history2_data[HISTORY_LENGTH];
extern lefl_array_elm_t analog_history3_data[HISTORY_LENGTH];
extern lefl_array_elm_t analog_history4_data[HISTORY_LENGTH];

extern lefl_bit_array_t lines[4];
extern lefl_bit_array_unit_t lines1_data[4];
extern lefl_bit_array_unit_t lines2_data[4];
extern lefl_bit_array_unit_t lines3_data[4];
extern lefl_bit_array_unit_t lines4_data[4];
extern lefl_loop_array_t KPS_queue;
extern const char *hid_usage_names[138];



extern fezui_link_frame_t mainframe;

extern fezui_link_page_t homepage;
void homepage_logic(void *page);
void homepage_draw(void *page);
void homepage_load(void *page);

extern fezui_link_page_t menupage;
void menupage_init();
void menupage_logic(void *page);
void menupage_draw(void *page);
void menupage_load(void *page);
void main_menu_cb(void *menu);


extern fezui_link_page_t settingspage;
void settingspage_init();
void settingspage_logic(void *page);
void settingspage_draw(void *page);
void settingspage_load(void *page);
void settings_menu_cb(void *menu);


extern fezui_link_page_t advancedsettingspage;
void advancedsettingspage_init();
void advancedsettingspage_logic(void *page);
void advancedsettingspage_draw(void *page);
void advancedsettingspage_load(void *page);
void advancedsettings_menu_cb(void *menu);

extern fezui_link_page_t oscilloscopepage;
void oscilloscopepage_logic(void *page);
void oscilloscopepage_draw(void *page);
void oscilloscopepage_load(void *page);

extern fezui_link_page_t keylistpage;
void keylistpage_init();
void keylistpage_logic(void *page);
void keylistpage_draw(void *page);
void keylistpage_load(void *page);
extern uint16_t* current_target_id;

extern fezui_link_page_t keyconfigpage;
void keyconfigpage_init();
void keyconfigpage_logic(void *page);
void keyconfigpage_draw(void *page);
extern char current_key_name[32];

extern fezui_link_page_t knobconfigpage;
void knobconfigpage_init();
void knobconfigpage_logic(void *page);
void knobconfigpage_draw(void *page);
extern char current_knob_name[16];
extern uint16_t* current_target_knob_press_id;
extern uint16_t* current_target_knob_clockwise_id;
extern uint16_t* current_target_knob_anticlockwise_id;

extern fezui_link_page_t advancedconfigpage;
void advancedconfigpage_init();
void advancedconfigpage_logic(void *page);
void advancedconfigpage_draw(void *page);
extern AdvancedKey* current_config_advanced_key;

extern fezui_link_page_t displayconfigpage;
void displayconfigpage_init();
void displayconfigpage_logic(void *page);
void displayconfigpage_draw(void *page);

extern fezui_link_page_t panelpage;
void panelpage_init();
void panelpage_logic(void *page);
void panelpage_draw(void *page);
void panelpage_load(void *page);

extern fezui_link_page_t keymappage;
void keymappage_init();
void keymappage_logic(void *page);
void keymappage_draw(void *page);
void keymappage_load(void *page);

extern fezui_link_page_t debugpage;
void debugpage_init();
void debugpage_logic(void *page);
void debugpage_draw(void *page);
void debugpage_load(void *page);

extern fezui_link_page_t calibrationpage;
void calibrationpage_init();
void calibrationpage_logic(void *page);
void calibrationpage_draw(void *page);
void calibrationpage_load(void *page);

extern fezui_link_page_t rgbconfigpage;
void rgbconfigpage_init();
void rgbconfigpage_logic(void *page);
void rgbconfigpage_draw(void *page);
void rgbconfigpage_load(void *page);

extern fezui_link_page_t statisticpage;
void statisticpage_logic(void *page);
void statisticpage_draw(void *page);
void statisticpage_load(void *page);

extern fezui_link_page_t aboutpage;
void aboutpage_logic(void *page);
void aboutpage_draw(void *page);
void aboutpage_load(void *page);


extern fezui_link_page_t calibrationpage;
void calibrationpage_init();
void calibrationpage_logic(void *page);
void calibrationpage_draw(void *page);
void calibrationpage_load(void *page);


#endif /* FEZUI_VAR_H_ */
