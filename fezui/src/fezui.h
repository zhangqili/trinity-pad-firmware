/*
 * fez_ui.h
 *
 *  Created on: 2023年6月18日
 *      Author: xq123
 */

#ifndef FEZUI_H_
#define FEZUI_H_

#include "u8g2.h"
#include "lefl.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "string.h"
#include "fezui_util.h"
#include "fezui_config.h"
#include "fezui_interface.h"
#include "usb_hid_keys.h"

#ifndef DEFAULT_ANIMATION_EASE_FUNCTION
#define DEFAULT_ANIMATION_EASE_FUNCTION fezui_animation_cubic_ease
#endif 

#ifndef DEFAULT_ANIMATION_MODE
#define DEFAULT_ANIMATION_MODE EASE_OUT
#endif 

#ifndef DEFAULT_ANIMATION_DURATION
#define DEFAULT_ANIMATION_DURATION (500)
#endif 

typedef u8g2_uint_t fezui_uint_t;
typedef u8g2_int_t fezui_int_t;

extern char g_fezui_printf_buffer[PRINTF_BUFFER_LENGTH];
//#define g_fezui_printf_buffer (fezui_get_buffer())

typedef struct __fezui_t
{
    u8g2_t u8g2;
    bool invert;
    uint8_t contrast;
    float speed;
    uint16_t screensaver_timeout;
    uint16_t screensaver_countdown;
} fezui_t;

// #define FEZUI_CREATE(name) fezui_t name;
// #define FEZUI_DECLARE(name) fezui_t name;

u8g2_uint_t fezui_printf(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, const char *fmt,...);
u8g2_uint_t fezui_printf_right_aligned(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, const char *fmt,...);
char* fezui_get_buffer();
void fezui_apply(fezui_t *fezui_ptr);
void fezui_timer_handler();
void fezui_render_handler();
void fezui_init();

typedef struct __fezui_point_t
{
    fezui_uint_t x;
    fezui_uint_t y;
} fezui_point_t;

typedef struct __fezui_size_t
{
    fezui_uint_t w;
    fezui_uint_t h;
} fezui_size_t;

typedef struct __fezui_base_t
{
    fezui_point_t location;
    fezui_size_t size;
} fezui_base_t;

typedef enum
{
    FEZUI_ALIGN_LEFT,
    FEZUI_ALIGN_CENTER,
    FEZUI_ALIGN_RIGHT
} fezui_align_t;

typedef enum
{
    FEZUI_TYPE_UINT8 = '8',
    FEZUI_TYPE_UINT16 = 'u',
    FEZUI_TYPE_UINT32 = 'U',
    FEZUI_TYPE_INT16 = 'd',
    FEZUI_TYPE_INT32 = 'D',
    FEZUI_TYPE_STRING = 's',
    FEZUI_TYPE_BOOL = 'b',
    FEZUI_TYPE_FLOAT = 'f',
    FEZUI_TYPE_DOUBLE = 'F',
} fezui_numberic_type_t;
/*
 * fezui_interface.c
 */
/*
 uint8_t ui_init();
 void fezui_clear_buf();
 void fezui_send_buf();
 void fezui_draw_rectangle(uint16_t x,uint16_t y,uint16_t w,uint16_t h,bool fill);
 void fezui_draw_pixel(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
 void fezui_draw_line(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
 */

/*
 * fezui_clock.c
 */
extern uint32_t fezui_tick;
/*
 * fezui_animation.c
 */

typedef float fezui_animation_float_t;

typedef enum
{
    EASE_IN,
    EASE_OUT,
    EASE_INOUT,
} fezui_animation_mode_t;

typedef struct __fezui_animation_base_t
{
    uint32_t begin_time;
    uint16_t duration;
    fezui_animation_float_t value;
    fezui_animation_mode_t mode;
    fezui_animation_float_t parameter1;
    fezui_animation_float_t parameter2;
    fezui_animation_float_t (*easing_func)(struct __fezui_animation_base_t *a, fezui_animation_float_t f);
} fezui_animation_base_t;

typedef struct __fezui_animation_PID_t
{
	/* Controller gains */
	float Kp;
	float Ki;
	float Kd;

	/* Derivative low-pass filter time constant */
	float tau;

	/* Output limits */
	float limMin;
	float limMax;
	
	/* Integrator limits */
	float limMinInt;
	float limMaxInt;

	/* Sample time (in seconds) */
	float T;

	/* Controller "memory" */
	float integrator;
	float prevError;			/* Required for integrator */
	float differentiator;
	float prevMeasurement;		/* Required for differentiator */

	/* Controller output */
	float out;
} fezui_animation_PID_t;

void fezui_animation_init(fezui_animation_base_t *a,
                          fezui_animation_float_t (*easing_func)(fezui_animation_base_t *a, fezui_animation_float_t f),
                          fezui_animation_mode_t mode);
void fezui_animation_bind(fezui_animation_base_t *a, fezui_animation_float_t *f);
void fezui_animation_begin(fezui_animation_base_t *a);
fezui_animation_float_t fezui_animation_calculate(fezui_animation_base_t *a);
#define FEZUI_ANIMATION_GET_VALUE(animation,from,to) ((animation)->value*(((fezui_animation_float_t)(to))-((fezui_animation_float_t)(from)))+((fezui_animation_float_t)(from)))

fezui_animation_float_t fezui_animation_sine_ease(fezui_animation_base_t *a, fezui_animation_float_t f);
fezui_animation_float_t fezui_animation_pow_ease(fezui_animation_base_t *a, fezui_animation_float_t f);
fezui_animation_float_t fezui_animation_linear_ease(fezui_animation_base_t*a, fezui_animation_float_t f);
fezui_animation_float_t fezui_animation_quad_ease(fezui_animation_base_t *a, fezui_animation_float_t f);
fezui_animation_float_t fezui_animation_cubic_ease(fezui_animation_base_t *a, fezui_animation_float_t f);
fezui_animation_float_t fezui_animation_quart_ease(fezui_animation_base_t *a, fezui_animation_float_t f);
fezui_animation_float_t fezui_animation_quint_ease(fezui_animation_base_t *a, fezui_animation_float_t f);
fezui_animation_float_t fezui_animation_expo_ease(fezui_animation_base_t *a, fezui_animation_float_t f);
fezui_animation_float_t fezui_animation_circ_ease(fezui_animation_base_t *a, fezui_animation_float_t f);
fezui_animation_float_t fezui_animation_back_ease(fezui_animation_base_t *a, fezui_animation_float_t f);
fezui_animation_float_t fezui_animation_elastic_ease(fezui_animation_base_t *a, fezui_animation_float_t f);
fezui_animation_float_t fezui_animation_bounce_ease(fezui_animation_base_t *a, fezui_animation_float_t f);

void fezui_animation_PID_init(fezui_animation_PID_t *pid);
float fezui_animation_PID_update(fezui_animation_PID_t *pid, float setpoint, float measurement);
/*
 * fezui_cursor.c
 */

typedef struct __fezui_cursor_t
{
    float x;
    float y;
    float w;
    float h;
} fezui_cursor_t;

typedef struct __fezui_animated_cursor_t
{
    fezui_cursor_t cursor;
    fezui_cursor_t target_cursor;
    fezui_animation_PID_t PID_x;
    fezui_animation_PID_t PID_y;
    fezui_animation_PID_t PID_w;
    fezui_animation_PID_t PID_h;
} fezui_animated_cursor_t;

extern fezui_cursor_t g_fezui_cursor;
//extern fezui_animated_cursor_t g_fezui_cursor;
extern fezui_cursor_t g_target_cursor;
extern fezui_animated_cursor_t animated_cursor;

void fezui_animated_cursor_update(fezui_animated_cursor_t* cursor);
void fezui_animated_cursor_goto(fezui_animated_cursor_t*cursor, float x, float y, float w, float h);
void fezui_animated_cursor_set(fezui_animated_cursor_t*cursor, float x, float y, float w, float h);
void fezui_draw_animated_cursor(fezui_t *fezui_ptr, fezui_animated_cursor_t *c);
void fezui_cursor_move(fezui_t *fezui_ptr, fezui_cursor_t *c, fezui_cursor_t *tc);
void fezui_cursor_set(fezui_cursor_t *c, float x, float y, float w, float h);
void fezui_draw_cursor(fezui_t *fezui_ptr, fezui_cursor_t *c);


/*
 * fezui_controls.c
 */
typedef enum
{
    ORIENTATION_HORIZAIONTAL,
    ORIENTATION_VERTICAL
} fezui_orientation_t;
void fezui_draw_flowingwater(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, uint8_t *bits);
//void fezui_draw_wave(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, lefl_loop_array_t *arr, uint8_t *bits);
//void fezui_draw_detailed_wave(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, AdvancedKey *key, lefl_loop_array_t *arr, uint8_t *bits);
void fezui_draw_chart(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, lefl_loop_array_t *arr, uint8_t max);
void fezui_veil(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, uint8_t level, uint8_t color);
void fezui_veil_full_screen(fezui_t *fezui_ptr, uint8_t level);

void fezui_draw_scrollbar(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, float size, float value, fezui_orientation_t o);

typedef struct __fezui_rangebase_t
{
    float max;
    float min;
    float interval;
    uint8_t type;
    void *target;
} fezui_rangebase_t;
void fezui_rangebase_init(fezui_rangebase_t *r, void *target, uint8_t type, float min, float max, float interval);
void fezui_rangebase_increase(fezui_rangebase_t *r, int8_t n);

void fezui_draw_slider(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, fezui_rangebase_t *r, fezui_orientation_t orientation);

typedef struct __fezui_tile_t
{
    char *header;
    char *icon;
    void (*tile_cb)(struct __fezui_tile_t *tile);
} fezui_tile_t;

#define ROLLING_NUMBER_OFFSET_LENGTH 8

typedef struct __fezui_rolling_number_t
{
    u8g2_long_t number;
    uint8_t digit;
    float offsets[8];
    const uint8_t *font;
} fezui_rolling_number_t;

void fezui_draw_rolling_number(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, fezui_rolling_number_t *rolling_number);
// void fezui_rolling_number_set(fezui_rolling_number_t* rolling_number, u8g2_long_t num);
#define fezui_rolling_number_set(rolling_number, num) (rolling_number)->number = (num);
void fezui_rolling_number_update(fezui_t *fezui_ptr, fezui_rolling_number_t *rolling_number);

typedef struct __fezui_progressbar_t
{
    float value;
    float max;
    float min;
    fezui_orientation_t orientation;
    bool is_indeterminate;
} fezui_progressbar_t;

void fezui_draw_progressbar(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, fezui_progressbar_t *bar);
void fezui_progressbar_update(fezui_t *fezui_ptr, fezui_progressbar_t *bar);

typedef enum
{
    SCROLLING_TEXT_PAUSE,
    SCROLLING_TEXT_PLAYING,
    SCROLLING_TEXT_PLAYING_ONCE,
} fezui_scrolling_status_t;

typedef struct __fezui_scrolling_text_t
{
    const char *text;
    float offset;
    float speed;
    u8g2_int_t width;
    u8g2_int_t content_width;
    const uint8_t *font;
    fezui_scrolling_status_t status;
} fezui_scrolling_text_t;

void fezui_draw_scrolling_text(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, fezui_scrolling_text_t *text);
void fezui_scrolling_text_update(fezui_scrolling_text_t *text);
void fezui_scrolling_text_init(fezui_t *fezui_ptr, fezui_scrolling_text_t *text, u8g2_int_t w, float speed, const uint8_t *font, const char *str);
void fezui_scrolling_text_begin(fezui_scrolling_text_t *text);
void fezui_scrolling_text_begin_once(fezui_scrolling_text_t *text);
void fezui_scrolling_text_get_status(fezui_t *fezui_ptr, fezui_scrolling_text_t *text);
typedef struct __fezui_list_base_t
{
    const char **items;
    int16_t selected_index;
    uint16_t len;
    void (*list_cb)(void *list);
} fezui_list_base_t;
void fezui_list_base_init(fezui_list_base_t* list, const char* *items,uint8_t len,void (*cb)(void* list));
void fezui_list_base_index_increase(fezui_list_base_t* list, int8_t delta);
void fezui_list_base_click(fezui_list_base_t* list);
typedef struct __fezui_animated_listbox_t
{
    fezui_list_base_t list;
    u8g2_int_t content_height;
    fezui_animation_base_t scroll_animation;
    fezui_animation_base_t start_animation;
    float targetoffset;
    float offset;
    bool show_scrollbar;
} fezui_animated_listbox_t;
void fezui_animated_listbox_init(fezui_animated_listbox_t *listbox, const char **items, uint8_t len, void (*cb)(void *listbox));
void fezui_animated_listbox_index_increase(fezui_animated_listbox_t *listbox, int8_t delta);
void fezui_animated_listbox_click(fezui_animated_listbox_t *listbox);
void fezui_animated_listbox_update(fezui_t *fezui_ptr, fezui_animated_listbox_t *listbox);
void fezui_animated_listbox_get_cursor(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, fezui_animated_listbox_t *listbox, u8g2_uint_t item_height, fezui_cursor_t *c);
void fezui_animated_listbox_begin(fezui_animated_listbox_t *listbox);
void fezui_draw_animated_listbox(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, fezui_animated_listbox_t *listbox, u8g2_uint_t item_height, u8g2_uint_t adjust);

typedef struct __fezui_menuitem_t
{
    const char *header;
    void *target;
} fezui_menuitem_t;

typedef struct __fezui_animated_menu_t
{
    const fezui_menuitem_t *items;
    int16_t selected_index;
    uint16_t len;
    u8g2_int_t content_height;
    float targetoffset;
    float offset;
    float animation;
    bool show_scrollbar;
    void (*menu_cb)(void *menu);
} fezui_animated_menu_t;
void fezui_animated_menu_init(fezui_animated_menu_t *menu,const fezui_menuitem_t *items, uint8_t len, void (*cb)(void *menu));
void fezui_animated_menu_index_increase(fezui_animated_menu_t *menu, int8_t delta);
void fezui_animated_menu_click(fezui_animated_menu_t *menu);
void fezui_animated_menu_update(fezui_t *fezui_ptr, fezui_animated_menu_t *menu);
void fezui_animated_menu_get_cursor(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, fezui_animated_menu_t *menu, u8g2_uint_t item_height, fezui_cursor_t *c);
void fezui_animated_menu_begin(fezui_animated_menu_t *menu);
void fezui_draw_animated_menu(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, fezui_animated_menu_t *menu, u8g2_uint_t item_height, u8g2_uint_t adjust);

/*
 * fezui_scrollviewer.c
 */

typedef struct __fezui_scrollview_t
{
    u8g2_int_t content_height;
    u8g2_int_t content_width;
    float abscissa;
    float ordinate;
} fezui_scrollview_t;

void fezui_draw_scrollview(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, fezui_scrollview_t *scrollview);

/*
 * fezui_flyout.c
 */

typedef struct __fezui_notification_t
{
    u8g2_int_t content_height;
    u8g2_int_t time;
    fezui_scrolling_text_t text;
    float offset;
} fezui_notification_t;

void fezui_draw_notification(fezui_t *fezui_ptr, fezui_notification_t *n);
void fezui_notification_begin(fezui_t *fezui_ptr, fezui_notification_t *n, const char *str, u8g2_int_t time, float speed);
void fezui_notification_update(fezui_t *fezui_ptr, fezui_notification_t *n);

typedef enum
{
    MESSAGEBOX_NORMAL,
    MESSAGEBOX_WARNING,
    MESSAGEBOX_ERROR,
} fezui_messagebox_type_t;

typedef struct __fezui_messagebox_t
{
    char *title;
    float *text;
    fezui_messagebox_type_t type;
} fezui_messagebox_t;

void fezui_draw_messagebox(fezui_t *fezui_ptr, uint8_t x, uint8_t y, uint8_t w, uint8_t h, fezui_messagebox_t *messagebox);

typedef struct __fezui_numberic_dialog_t
{
    char *title;
    fezui_rangebase_t rangebase;
} fezui_numberic_dialog_t;

void fezui_numberic_dialog_init(fezui_numberic_dialog_t *dialog, void *target, uint8_t type, float min, float max, float interval, char *title);
void fezui_numberic_dialog_increase(fezui_numberic_dialog_t *dialog, int8_t n);
void fezui_draw_numberic_dialog(fezui_t *fezui_ptr, u8g2_int_t x, u8g2_int_t y, u8g2_int_t w, u8g2_int_t h, fezui_numberic_dialog_t *dialog);

typedef struct __fezui_flyout_numberic_dialog_t
{
    fezui_numberic_dialog_t dialog;
    float animation;
    float target_animation;
    bool show;
} fezui_flyout_numberic_dialog_t;
void fezui_flyout_numberic_dialog_init(fezui_flyout_numberic_dialog_t *dialog, void *target, uint8_t type, float min, float max, float interval, char *title);
void fezui_flyout_numberic_dialog_update(fezui_t *fezui_ptr, fezui_flyout_numberic_dialog_t *dialog);
#define fezui_flyout_numberic_dialog_close(dialog) (dialog)->show = false
#define fezui_flyout_numberic_dialog_show(dialog) (dialog)->show = true
void fezui_draw_flyout_numberic_dialog(fezui_t *fezui_ptr, fezui_flyout_numberic_dialog_t *dialog);

/*
 * fezui_nav.c
 */

typedef struct __fezui_link_page_t
{
    void (*page_tick_cb)(void *page);
    void (*page_draw_cb)(void *page);
    void (*page_load_cb)(void *page);
    void (*event_handler)(void *e);
    struct __fezui_link_page_t *forward;
    struct __fezui_link_page_t *back;
} fezui_link_page_t;

typedef struct __fezui_link_frame_t
{
    fezui_link_page_t *old_page;
    fezui_link_page_t *current_page;
    void (*link_frame_cb)(void *frame);
    fezui_animation_base_t *animation;
} fezui_link_frame_t;

void fezui_link_frame_go_home(fezui_link_frame_t *frame);
void fezui_link_frame_go_forward(fezui_link_frame_t *frame);
void fezui_link_frame_go_back(fezui_link_frame_t *frame);
void fezui_link_frame_navigate(fezui_link_frame_t *frame, fezui_link_page_t *page);
void fezui_link_frame_logic(fezui_link_frame_t *frame);
void fezui_link_frame_draw(fezui_link_frame_t *frame);
void fezui_link_frame_input(fezui_link_frame_t* frame, void* sender);

/*
 * fezui_util.c
 */
float fezui_generics_convert_to_float(void *p, fezui_numberic_type_t t);
void fezui_generics_convertback(void *p, fezui_numberic_type_t t, float x);

#endif /* FEZUI_H_ */
