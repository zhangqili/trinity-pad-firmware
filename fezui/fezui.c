/*
 * fezui.c
 *
 *  Created on: 2023年6月19日
 *      Author: xq123
 */
#include "fezui.h"

char fezui_buffer[BUFFER_LENGTH] = {0};


void fezui_apply(fezui_t* fezui_ptr)
{
    //u8g2_SendF(&(fezui_ptr->u8g2), "c", 0xA6+fezui_ptr->invert);
    u8g2_SetContrast(&(fezui_ptr->u8g2),fezui_ptr->contrast);
    fezui_ptr->screensaver_countdown=fezui_ptr->screensaver_timeout;
}
