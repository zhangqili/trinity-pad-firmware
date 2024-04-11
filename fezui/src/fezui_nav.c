
#include "fezui.h"
#include "fezui_var.h"

void fezui_frame_init(fezui_frame_t *frame, fezui_page_t *page, fezui_animation_base_t *animation)
{
    frame->pages[frame->current_page_index] = page;
    frame->animation = animation;
    frame->old_page_index = -1;
    if (frame->animation)
        fezui_animation_begin(frame->animation);
}

void fezui_frame_go_home(fezui_frame_t *frame)
{
    frame->old_page_index = frame->current_page_index;
    frame->current_page_index=0;
}

void fezui_frame_go_forward(fezui_frame_t *frame)
{
    frame->old_page_index = frame->current_page_index;
    if (frame->pages[frame->current_page_index+1])
    {
        frame->current_page_index++;
    }
    if (frame->pages[frame->current_page_index]->page_load_cb)
        frame->pages[frame->current_page_index]->page_load_cb(frame->pages[frame->current_page_index]);
    if (frame->animation)
        fezui_animation_begin(frame->animation);
}

void fezui_frame_go_back(fezui_frame_t *frame)
{
    frame->old_page_index = frame->current_page_index;
    if(frame->current_page_index)
    {
        frame->current_page_index--;
    }
    if (frame->pages[frame->current_page_index]->page_load_cb)
        frame->pages[frame->current_page_index]->page_load_cb(frame->pages[frame->current_page_index]);
    if (frame->animation)
        fezui_animation_begin(frame->animation);
}

void fezui_frame_show_dialog(fezui_frame_t *frame, fezui_page_t *dialog)
{
    if (!frame->dialog)
    {
        frame->dialog = dialog;
    }
    frame->dialog_state = true;
}

void fezui_frame_close_dialog(fezui_frame_t *frame)
{
    frame->dialog_state = false;
}

void fezui_frame_navigate(fezui_frame_t *frame, fezui_page_t *page)
{
    frame->old_page_index = frame->current_page_index;
    if(frame->current_page_index<FEZUI_FRAME_MAX_PAGE-1)
    {
        frame->current_page_index++;
    }
    frame->pages[frame->current_page_index]=page;
    if (frame->pages[frame->current_page_index]->page_load_cb)
        frame->pages[frame->current_page_index]->page_load_cb(frame->pages[frame->current_page_index]);
    if (frame->animation)
        fezui_animation_begin(frame->animation);
}

void fezui_frame_tick(fezui_frame_t *frame)
{
    if (frame->animation)
    {
        fezui_animation_calculate(frame->animation);
        if (frame->animation->value < 0.5)
        {
            // if(frame->old_page!=NULL)
            //     frame->old_page->page_tick_cb(frame->old_page);
        }
        else
        {
            frame->pages[frame->current_page_index]->page_tick_cb(frame->pages[frame->current_page_index]);
        }
    }
    else
    {
        frame->pages[frame->current_page_index]->page_tick_cb(frame->pages[frame->current_page_index]);
    }
    if (frame->dialog)
    {
        frame->dialog->page_tick_cb(frame->dialog);
    }
}

void fezui_frame_draw(fezui_frame_t *frame)
{
    if (frame->animation)
    {
        fezui_animation_calculate(frame->animation);
        if (frame->animation->value < 0.5)
        {
            if (frame->pages[frame->old_page_index]&&frame->old_page_index >= 0)
                frame->pages[frame->old_page_index]->page_draw_cb(frame->pages[frame->old_page_index]);
            fezui_veil_full_screen(&fezui, frame->animation->value * 14);
        }
        else
        {
            frame->pages[frame->current_page_index]->page_draw_cb(frame->pages[frame->current_page_index]);
            fezui_veil_full_screen(&fezui, (1 - frame->animation->value) * 14);
        }
    }
    else
    {
        frame->pages[frame->current_page_index]->page_draw_cb(frame->pages[frame->current_page_index]);
    }
    if (frame->dialog)
    {
        frame->dialog->page_draw_cb(frame->dialog);
    }
}

void fezui_frame_input(fezui_frame_t *frame, void *sender)
{
    if (frame->dialog_state)
    {
        frame->dialog->event_handler(sender);
    }
    else if (frame->pages[frame->current_page_index]->event_handler)
    {
        frame->pages[frame->current_page_index]->event_handler(sender);
    }
}