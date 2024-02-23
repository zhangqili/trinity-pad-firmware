#include "fezui.h"

float fezui_generics_convert_to_float(void*p,fezui_numberic_type_t t)
{
    switch (t)
    {
    case FEZUI_TYPE_FLOAT:
        return*(float*)p;
        break;
    case FEZUI_TYPE_DOUBLE:
        return*(double*)p;
        break;
    case FEZUI_TYPE_INT16:
        return*(int16_t*)p;
        break;
    case FEZUI_TYPE_INT32:
        return*(int32_t*)p;
        break;
    case FEZUI_TYPE_UINT16:
        return*(uint16_t*)p;
        break;
    case FEZUI_TYPE_UINT32:
        return*(uint32_t*)p;
        break;
    case FEZUI_TYPE_UINT8:
        return*(uint8_t*)p;
        break;
    case FEZUI_TYPE_BOOL:
        return*(bool*)p;
        break;
    default:
        return (uint32_t)p;
    }
}

void fezui_generics_convertback(void*p,fezui_numberic_type_t t,float x)
{
    switch (t)
    {
    case FEZUI_TYPE_FLOAT:
        *(float*)p=(float)x;
        break;
    case FEZUI_TYPE_DOUBLE:
        *(double*)p=(double)x;
        break;
    case FEZUI_TYPE_INT16:
        *(int16_t*)p=(int16_t)x;
        break;
    case FEZUI_TYPE_INT32:
        *(int32_t*)p=(int32_t)x;
        break;
    case FEZUI_TYPE_UINT16:
        *(uint16_t*)p=(uint16_t)x;
        break;
    case FEZUI_TYPE_UINT32:
        *(uint32_t*)p=(uint32_t)x;
        break;
    case FEZUI_TYPE_UINT8:
        *(uint8_t*)p=(uint8_t)x;
        break;
    case FEZUI_TYPE_BOOL:
        *(bool*)p=(bool)x;
        break;
    default:
        *(uint32_t*)p=x;
    }
}

void fezui_clearbuffer(fezui_t*fezui_ptr,u8g2_int_t x,u8g2_int_t y,u8g2_int_t w,u8g2_int_t h)
{
    //uint8_t*p = u8g2_GetBufferPtr(&fezui_ptr->u8g2);
}