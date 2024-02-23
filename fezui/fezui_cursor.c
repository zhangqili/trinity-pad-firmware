#include "fezui.h"
void fezui_animated_cursor_goto(fezui_animated_cursor_t*cursor, float x, float y, float w, float h)
{
    fezui_animated_cursor_begin(cursor);
    cursor->target_cursor.x = x;
    cursor->target_cursor.y = y;
    cursor->target_cursor.w = w;
    cursor->target_cursor.h = h;
}

void fezui_animated_cursor_update(fezui_animated_cursor_t *c)
{
    c->cursor.x+=fezui_animation_PID_update(&c->PID_x,c->target_cursor.x,c->cursor.x);
    c->cursor.y+=fezui_animation_PID_update(&c->PID_y,c->target_cursor.y,c->cursor.y);
    c->cursor.w+=fezui_animation_PID_update(&c->PID_w,c->target_cursor.w,c->cursor.w);
    c->cursor.h+=fezui_animation_PID_update(&c->PID_h,c->target_cursor.h,c->cursor.h);
}
void fezui_draw_animated_cursor(fezui_t *fezui_ptr, fezui_animated_cursor_t *cursor)
{
    fezui_draw_cursor(fezui_ptr,&cursor->cursor);
}
void fezui_animated_cursor_set(fezui_animated_cursor_t*cursor, float x, float y, float w, float h)
{
    cursor->target_cursor.x = x;
    cursor->target_cursor.y = y;
    cursor->target_cursor.w = w;
    cursor->target_cursor.h = h;
    cursor->cursor.x = x;
    cursor->cursor.y = y;
    cursor->cursor.w = w;
    cursor->cursor.h = h;
}
inline void fezui_cursor_set(fezui_cursor_t *c, float x, float y, float w, float h)
{
    c->x = x;
    c->y = y;
    c->w = w;
    c->h = h;
}

void fezui_cursor_move(fezui_t *fezui_ptr, fezui_cursor_t *c, fezui_cursor_t *tc)
{
    CONVERGE_TO((c->x), tc->x, fezui_ptr->speed);
    CONVERGE_TO((c->y), tc->y, fezui_ptr->speed);
    CONVERGE_TO((c->w), tc->w, fezui_ptr->speed);
    CONVERGE_TO((c->h), tc->h, fezui_ptr->speed);
}

void fezui_draw_cursor(fezui_t *fezui_ptr, fezui_cursor_t *c)
{
    uint8_t color_backup = u8g2_GetDrawColor(&(fezui_ptr->u8g2));
    u8g2_SetDrawColor(&(fezui_ptr->u8g2), 2);
    u8g2_DrawBox(&(fezui_ptr->u8g2), (u8g2_int_t)(c->x+0.5f), (u8g2_int_t)(c->y+0.5f), c->w+0.5f, c->h+0.5f);
    u8g2_SetDrawColor(&(fezui_ptr->u8g2), color_backup);
    // u8g2_SetDrawColor(&(fezui_ptr->u8g2), !fezui_ptr->invert);
}