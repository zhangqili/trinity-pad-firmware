
#include "fezui.h"
#include "fezui_var.h"

void fezui_link_frame_go_home(fezui_link_frame_t* frame)
{
    while(frame->current_page->back!=NULL)
        frame->current_page=frame->current_page->back;
}


void fezui_link_frame_go_forward(fezui_link_frame_t* frame)
{
    frame->old_page=frame->current_page;
    if(frame->current_page->forward!=NULL)
        frame->current_page=frame->current_page->forward;
    if(frame->current_page->page_load_cb!=NULL)
        frame->current_page->page_load_cb(frame->current_page);
    if(frame->animation)
        fezui_animation_begin(frame->animation);
}

void fezui_link_frame_go_back(fezui_link_frame_t* frame)
{
    frame->old_page=frame->current_page;
    if(frame->current_page->back!=NULL)
        frame->current_page=frame->current_page->back;
    if(frame->current_page->page_load_cb!=NULL)
        frame->current_page->page_load_cb(frame->current_page);
    if(frame->animation)
        fezui_animation_begin(frame->animation);
}

void fezui_link_frame_navigate(fezui_link_frame_t* frame, fezui_link_page_t* page)
{
    frame->old_page=frame->current_page;
    page->back=frame->current_page;
    frame->current_page->forward=page;
    frame->current_page=frame->current_page->forward;
    if(frame->current_page->page_load_cb!=NULL)
        frame->current_page->page_load_cb(frame->current_page);
    if(frame->animation)
        fezui_animation_begin(frame->animation);
}

void fezui_link_frame_logic(fezui_link_frame_t* frame)
{
    if(frame->animation)
    {
        fezui_animation_calculate(frame->animation);
        if(frame->animation->value <0.5)
        {
            //if(frame->old_page!=NULL)
            //    frame->old_page->page_tick_cb(frame->old_page);
        }
        else
        {
            frame->current_page->page_tick_cb(frame->current_page);
        }
    }
    else
    {
        frame->current_page->page_tick_cb(frame->current_page);
    }
}

void fezui_link_frame_draw(fezui_link_frame_t* frame)
{
    if(frame->animation)
    {
        fezui_animation_calculate(frame->animation);
        if(frame->animation->value<0.5)
        {
            if(frame->old_page!=NULL)
                frame->old_page->page_draw_cb(frame->old_page);
            fezui_veil_full_screen(&fezui,frame->animation->value*14);
        }
        else
        {
            frame->current_page->page_draw_cb(frame->current_page);
            fezui_veil_full_screen(&fezui,(1-frame->animation->value)*14);
        }
    }
    else
    {
        frame->current_page->page_draw_cb(frame->current_page);
    }
}


void fezui_link_frame_input(fezui_link_frame_t* frame, void* sender)
{
    if(frame->current_page->event_handler)
        frame->current_page->event_handler(sender);
}