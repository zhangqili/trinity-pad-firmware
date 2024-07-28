/*
 * fezui_var.c
 *
 *  Created on: 2023骞�6鏈�19鏃�
 *      Author: xq123
 */
#include "fezui_var.h"

char g_fpsstr[8] = {0};

fezui_t fezui={.speed=0.1};

uint8_t g_kps_history_max;
uint32_t g_fezui_fps;
uint32_t g_fezui_debug;
uint32_t g_fezui_run_time;
uint32_t g_usb_report_count;
uint32_t g_usb_report_count1;
uint32_t g_int_count;
uint32_t g_int_count1;
uint32_t g_interval;
uint32_t g_max_interval;
uint32_t g_last_max_interval;

fezui_notification_t fezui_notification={.text.font=u8g2_font_5x8_mn};
/*
  Fontname: fez_font_6x10
  Copyright: Created with Fony 1.4.6
  Glyphs: 26/26
  BBX Build Mode: 0x
*/
const uint8_t fez_font_6x10_m[204] U8G2_FONT_SECTION("fez_font_6x10_m") =
  "\32\0\3\3\3\4\4\5\4\6\12\0\0\11\0\12\12\0\0\0\0\0\257 \5\0\324\16!\5\0\324"
  "\16\42\5\0\324\16#\5\0\324\16$\5\0\324\16%\5\0\324\16&\5\0\324\16'\5\0\324\16("
  "\5\0\324\16)\5\0\324\16*\5\0\324\16+\5\0\324\16,\5\0\324\16-\6\16\204\16\3.\5"
  "\0\324\16/\5\0\324\16\60\10M\204\16\233\276\31\61\7I\206\216\203\0\62\12M\204\216b\342\61\261"
  "\0\63\12M\204\216b\242\61\321\0\64\11M\204\216\230\66c\16\65\12M\204\16cb\61\321\0\66\12"
  "M\204\16c\242M\315\0\67\10M\204\216b\376\0\70\12M\204\16\233\332M\315\0\71\12M\204\16\233"
  "\232\61\321\0\0\0\0\4\377\377\0";


fezui_frame_t g_mainframe;

fezui_cursor_t g_fezui_cursor;
fezui_cursor_t g_target_cursor;
/*
fezui_animated_cursor_t animated_cursor = 
{
    .PID_x=
    {
        .Kp=0.3/5,
        .Ki=1.0/5,
        .limMax=100,
        .limMaxInt=100,
        .limMin=-100,
        .limMinInt=-100,
        .tau=0.02f,
        .T=0.01f
    },
    .PID_y=
    {
        .Kp=0.3/5,
        .Ki=1.0/5,
        .limMax=100,
        .limMaxInt=100,
        .limMin=-100,
        .limMinInt=-100,
        .tau=0.02f,
        .T=0.01f
    },
    .PID_w=
    {
        .Kp=0.3/5,
        .Ki=1.0/5,
        .limMax=100,
        .limMaxInt=100,
        .limMin=-100,
        .limMinInt=-100,
        .tau=0.02f,
        .T=0.01f
    },
    .PID_h=
    {
        .Kp=0.3/5,
        .Ki=1.0/5,
        .limMax=100,
        .limMaxInt=100,
        .limMin=-100,
        .limMinInt=-100,
        .tau=0.02f,
        .T=0.01f
    },
};
*/
//fezui_animated_cursor_t cursor=
//{
//    .animation.duration=DEFAULT_ANIMATION_DURATION,
//    .animation.easing_func=DEFAULT_ANIMATION_EASE_FUNCTION,
//    .animation.mode=EASE_OUT,
//    };


const char *g_hid_usage_names[138] =
{
    "Left Control",
    "Left Shift",
    "Left Alt",
    "Left GUI(Win)",
    "Right Control",
    "Right Shift",
    "Right Alt",
    "Right GUI(Win)",
    "No Event",
    "Overrun Error",
    "POST Fail",
    "ErrorUndefined",
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "0",
    "Return",
    "Escape",
    "Backspace",
    "Tab",
    "Space",
    "-",
    "=",
    "[",
    "]",
    "\\",
    "Europe 1 (Note 2)",
    ";",
    "\"",
    "`",
    ",",
    ".",
    "/",
    "Caps Lock",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "F11",
    "F12",
    "Print Screen",
    "Scroll Lock",
    "Break (Pause)",
    "Insert",
    "Home",
    "Page Up",
    "Delete",
    "End",
    "Page Down",
    "Right Arrow",
    "Left Arrow",
    "Down Arrow",
    "Up Arrow",
    "Num Lock",
    "Keypad /",
    "Keypad *",
    "Keypad -",
    "Keypad +",
    "Keypad Enter",
    "Keypad 1",
    "Keypad 2",
    "Keypad 3",
    "Keypad 4",
    "Keypad 5",
    "Keypad 6",
    "Keypad 7",
    "Keypad 8",
    "Keypad 9",
    "Keypad 0",
    "Keypad .",
    "Europe 2 (Note 2)",
    "App",
    "Keyboard Power",
    "Keypad =",
    "F13",
    "F14",
    "F15",
    "F16",
    "F17",
    "F18",
    "F19",
    "F20",
    "F21",
    "F22",
    "F23",
    "F24",
    "Execute",
    "Help",
    "Menu",
    "Select",
    "Stop",
    "Again",
    "Undo",
    "Cut",
    "Copy",
    "Paste",
    "Find",
    "Mute",
    "Volume Up",
    "Volume Down"
};



