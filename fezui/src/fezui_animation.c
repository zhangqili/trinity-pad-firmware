/*
 * fezui_animation.c
 */

/*
 * The MIT License (MIT)
 *
 * Copyright (c) .NET Foundation and Contributors
 *
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include "fezui.h"

#define PI M_PI
#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)

#define FLOAT_EPS 1e-6
#define DOUBLE_EPS 1e-15
#define iszero(x) (fabs(x) < 10.0f * FLOAT_EPS)
#define isone(x) (fabs(x - 1.0f) < 10.0f * FLOAT_EPS)

void fezui_animation_init(fezui_animation_base_t *a,
                          fezui_animation_float_t (*easing_func)(fezui_animation_base_t*a, fezui_animation_float_t f),
                          fezui_animation_mode_t mode)
{
    a->easing_func = easing_func;
    a->mode = mode;
}

void fezui_animation_bind(fezui_animation_base_t *a, fezui_animation_float_t *f)
{
    //a->target = f;
}
void fezui_animation_begin(fezui_animation_base_t *a, int32_t offset)
{
    a->begin_time = fezui_tick + offset;
}
fezui_animation_float_t fezui_animation_calculate(fezui_animation_base_t *a)
{
    if (fezui_tick >= a->begin_time + a->duration)
    {
        return a->value = 1.0;
    }
    if (fezui_tick < a->begin_time)
    {
        return  a->value = 0.0;
    }
    
    fezui_animation_float_t normalized_time = (fezui_animation_float_t)(fezui_tick - a->begin_time) / (fezui_animation_float_t)a->duration;
    if (a->easing_func == NULL)
    {
        a->easing_func = fezui_animation_linear_ease;
    }
    switch (a->mode)
    {
    case EASE_IN:
        return a->value = a->easing_func(a,normalized_time);
        break;
    case EASE_OUT:
        // EaseOut is the same as EaseIn, except time is reversed & the result is flipped.
        return a->value = (1.0f - a->easing_func(a,1.0f - normalized_time));
        break;
    case EASE_INOUT:
    default:
        // EaseInOut is a combination of EaseIn & EaseOut fit to the 0-1, 0-1 range.
        return a->value = ((normalized_time < 0.5f) ? a->easing_func(a,normalized_time * 2.0f) * 0.5f : (1.0f - a->easing_func(a,(1.0f - normalized_time) * 2.0f)) * 0.5f + 0.5f);
        break;
    }
}


fezui_animation_float_t fezui_animation_sine_ease(fezui_animation_base_t*a, fezui_animation_float_t f)
{
    return 1.0f - sinf(PI * 0.5f * (1.0f - f));
}
fezui_animation_float_t fezui_animation_pow_ease(fezui_animation_base_t*a, fezui_animation_float_t f)
{
    fezui_animation_float_t power = a->parameter1;
    power = max(0.0f, power);
    return powf(f, power);
}
fezui_animation_float_t fezui_animation_linear_ease(fezui_animation_base_t*a, fezui_animation_float_t f)
{
    return f;
}
fezui_animation_float_t fezui_animation_quad_ease(fezui_animation_base_t*a, fezui_animation_float_t f)
{
    return f * f;
}
fezui_animation_float_t fezui_animation_cubic_ease(fezui_animation_base_t*a, fezui_animation_float_t f)
{
    return f * f * f;
}
fezui_animation_float_t fezui_animation_quart_ease(fezui_animation_base_t*a, fezui_animation_float_t f)
{
    return f * f * f * f;
}
fezui_animation_float_t fezui_animation_quint_ease(fezui_animation_base_t*a, fezui_animation_float_t f)
{
    return f * f * f * f * f;
}
fezui_animation_float_t fezui_animation_expo_ease(fezui_animation_base_t*a, fezui_animation_float_t f)
{
    fezui_animation_float_t expo = a->parameter1;
    if (iszero(expo))
        return f;
    else
        return (expf(expo * f) - 1.0f) / (expf(expo) - 1.0f);
}
fezui_animation_float_t fezui_animation_circ_ease(fezui_animation_base_t*a, fezui_animation_float_t f)
{
    f = max(0.0f, min(1.0f, f));
    return 1.0f - sqrt(1.0f - f * f);
}
fezui_animation_float_t fezui_animation_back_ease(fezui_animation_base_t*a, fezui_animation_float_t f)
{
    fezui_animation_float_t amp = a->parameter1;
    return pow(f, 3.0f) - f * amp * sinf(PI * f);
}
fezui_animation_float_t fezui_animation_elastic_ease(fezui_animation_base_t*a, fezui_animation_float_t f)
{
    fezui_animation_float_t oscillations = a->parameter1;
    fezui_animation_float_t springiness = a->parameter2;
    fezui_animation_float_t oscillations1 = max(0.0f, (fezui_animation_float_t)oscillations);
    fezui_animation_float_t expo;
    springiness = max(0.0, springiness);
    if (iszero(springiness))
        expo = f;
    else
        expo = (expf(springiness * f) - 1.0f) / (expf(springiness) - 1.0f);

    return expo * (sinf((PI * 2.0f * oscillations1 + PI * 0.5f) * f));
}
fezui_animation_float_t fezui_animation_bounce_ease(fezui_animation_base_t*a, fezui_animation_float_t f)
{
    // The math below is complicated because we have a few requirements to get the correct look for bounce:
    //  1) The bounces should be symetrical
    //  2) Bounciness should control both the amplitude and the period of the bounces
    //  3) Bounces should control the number of bounces without including the final half bounce to get you back to 1.0
    //
    //  Note: Simply modulating a expo or power curve with a abs(sin(...)) wont work because it violates 1) above.
    //

    // Constants
    fezui_animation_float_t bounces = a->parameter1;
    fezui_animation_float_t bounciness = a->parameter2;
    bounces = max(0.0f, bounces);

    // Clamp the bounciness so we dont hit a divide by zero
    if (bounciness < 1.0f || isone(bounciness))
    {
        // Make it just over one.  In practice, this will look like 1.0 but avoid divide by zeros.
        bounciness = 1.001f;
    }

    fezui_animation_float_t power = powf(bounciness, bounces);
    fezui_animation_float_t oneMinusBounciness = 1.0f - bounciness;

    // 'unit' space calculations.
    // Our bounces grow in the x axis exponentially.  we define the first bounce as having a 'unit' width of 1.0 and compute
    // the total number of 'units' using a geometric series.
    // We then compute which 'unit' the current time is in.
    fezui_animation_float_t sumOfUnits = (1.0f - power) / oneMinusBounciness + power * 0.5f; // geometric series with only half the last sum
    fezui_animation_float_t unitAtT = f * sumOfUnits;

    // 'bounce' space calculations.
    // Now that we know which 'unit' the current time is in, we can determine which bounce we're in by solving the geometric equation:
    // unitAtT = (1 - bounciness^bounce) / (1 - bounciness), for bounce.
    fezui_animation_float_t bounceAtT = logf(-unitAtT * (1.0f - bounciness) + 1.0) / logf(bounciness);
    fezui_animation_float_t start = floorf(bounceAtT);
    fezui_animation_float_t end = start + 1.0;

    // 'time' space calculations.
    // We then project the start and end of the bounce into 'time' space
    fezui_animation_float_t startTime = (1.0 - powf(bounciness, start)) / (oneMinusBounciness * sumOfUnits);
    fezui_animation_float_t endTime = (1.0 - powf(bounciness, end)) / (oneMinusBounciness * sumOfUnits);

    // Curve fitting for bounce.
    fezui_animation_float_t midTime = (startTime + endTime) * 0.5;
    fezui_animation_float_t timeRelativeToPeak = f - midTime;
    fezui_animation_float_t radius = midTime - startTime;
    fezui_animation_float_t amplitude = powf(1.0 / bounciness, (bounces - start));

    // Evaluate a quadratic that hits (startTime,0), (endTime, 0), and peaks at amplitude.
    return (-amplitude / (radius * radius)) * (timeRelativeToPeak - radius) * (timeRelativeToPeak + radius);
}

void fezui_animation_PID_init(fezui_animation_PID_t *pid) {

	/* Clear controller variables */
	pid->integrator = 0.0f;
	pid->prevError  = 0.0f;

	pid->differentiator  = 0.0f;
	pid->prevMeasurement = 0.0f;

	pid->out = 0.0f;

}

float fezui_animation_PID_update(fezui_animation_PID_t *pid, float setpoint, float measurement) {

	/*
	* Error signal
	*/
    float error = setpoint - measurement;


	/*
	* Proportional
	*/
    float proportional = pid->Kp * error;


	/*
	* Integral
	*/
    pid->integrator = pid->integrator + 0.5f * pid->Ki * pid->T * (error + pid->prevError);

	/* Anti-wind-up via integrator clamping */
    if (pid->integrator > pid->limMaxInt) {

        pid->integrator = pid->limMaxInt;

    } else if (pid->integrator < pid->limMinInt) {

        pid->integrator = pid->limMinInt;

    }


	/*
	* Derivative (band-limited differentiator)
	*/
		
    pid->differentiator = -(2.0f * pid->Kd * (measurement - pid->prevMeasurement)	/* Note: derivative on measurement, therefore minus sign in front of equation! */
                        + (2.0f * pid->tau - pid->T) * pid->differentiator)
                        / (2.0f * pid->tau + pid->T);


	/*
	* Compute output and apply limits
	*/
    pid->out = proportional + pid->integrator + pid->differentiator;

    if (pid->out > pid->limMax) {

        pid->out = pid->limMax;

    } else if (pid->out < pid->limMin) {

        pid->out = pid->limMin;

    }

	/* Store error and measurement for later use */
    pid->prevError       = error;
    pid->prevMeasurement = measurement;

	/* Return controller output */
    return pid->out;

}
