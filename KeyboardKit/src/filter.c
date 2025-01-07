/*
 * Copyright (c) 2024 Zhangqi Li (@zhangqili)
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "filter.h"

AnalogValue adaptive_schimidt_filter(AdaptiveSchimidtFilter *filter, AnalogValue value)
{
    if (value - TOLERANCE > filter->state)
        filter->state = value - TOLERANCE;
    if (value + TOLERANCE < filter->state)
        filter->state = value + TOLERANCE;
    return filter->state;
}