/*
 * fezui.c
 *
 *  Created on: 2023年6月19日
 *      Author: xq123
 */
#include "fezui.h"
#include "stdarg.h"
#include "tgprintf.h"

char g_fezui_printf_buffer[PRINTF_BUFFER_LENGTH];

char* fezui_get_printf_buffer()
{
    return g_fezui_printf_buffer;
}
u8g2_uint_t fezui_printf(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, const char *fmt,...)
{
	va_list ap;
	va_start(ap, fmt);
	vsprintf(g_fezui_printf_buffer, fmt, ap);
	va_end(ap);
    return u8g2_DrawUTF8(&(fezui_ptr->u8g2), x, y, g_fezui_printf_buffer);
}
u8g2_uint_t fezui_tgprintf(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, const char *fmt,...)
{
	va_list ap;
	va_start(ap, fmt);
	tgvsprintf(g_fezui_printf_buffer, fmt, ap);
	va_end(ap);
    return u8g2_DrawStr(&(fezui_ptr->u8g2), x, y, g_fezui_printf_buffer);
}

u8g2_uint_t fezui_printf_right_aligned(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, const char *fmt,...)
{
	va_list ap;
	va_start(ap, fmt);
	vsprintf(g_fezui_printf_buffer, fmt, ap);
	va_end(ap);
    return u8g2_DrawUTF8(&(fezui_ptr->u8g2), x-u8g2_GetStrWidth(&(fezui_ptr->u8g2), g_fezui_printf_buffer), y, g_fezui_printf_buffer);
}
void fezui_apply(fezui_t* fezui_ptr)
{
    //u8g2_SendF(&(fezui_ptr->u8g2), "c", 0xA6+fezui_ptr->invert);
	if (fezui_ptr->speed<=1e-6)
	{
		fezui_ptr->speed = 0.1;
	}
	if (fezui_ptr->lang>=LANG_NUM)
	{
		fezui_ptr->lang = 0;
	}
	
    u8g2_SetContrast(&(fezui_ptr->u8g2),fezui_ptr->contrast);
    fezui_ptr->screensaver_countdown=fezui_ptr->screensaver_timeout;
}

__WEAK void fezui_timer_handler()
{
}

__WEAK void fezui_render_handler()
{
}