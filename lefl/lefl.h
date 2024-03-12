/*
 * lefl.h
 */

#ifndef LEFL_H_
#define LEFL_H_

#ifdef __cplusplus
extern "C" {
#endif

#define _USE_MATH_DEFINES
#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"
#include "string.h"
#include "math.h"
#include "lefl_macro.h"

    /*
     * lefl_datastruct.c
     */

    typedef uint16_t lefl_array_elm_t;

    typedef struct __lefl_loop_array_t
    {
        lefl_array_elm_t *data;
        int16_t index;
        int16_t len;
    } lefl_loop_array_t;

    typedef struct __lefl_loop_array_iterator_t
    {
        lefl_loop_array_t* lefl_loop_array_t;
        int16_t index;
    } lefl_loop_array_iterator_t;

    void lefl_loop_array_init(lefl_loop_array_t *arr,lefl_array_elm_t *data,uint16_t len);
    lefl_array_elm_t lefl_loop_array_get(lefl_loop_array_t* arr, int16_t j);
    void lefl_loop_array_push_back(lefl_loop_array_t* arr, lefl_array_elm_t t);
    lefl_array_elm_t lefl_loop_array_max(lefl_loop_array_t* arr);

    typedef size_t lefl_bit_array_unit_t;
#define LEFL_BIT_ARRAY_UNIT_WIDTH (sizeof(lefl_bit_array_unit_t)*8)
    typedef struct __lefl_bit_array_t
    {
        lefl_bit_array_unit_t *data;
        uint16_t len;
    } lefl_bit_array_t;

    void lefl_bit_array_init(lefl_bit_array_t* arr, lefl_bit_array_unit_t *data, uint16_t len);
    void lefl_bit_array_set(lefl_bit_array_t* arr, int16_t n,bool b);
    void lefl_bit_array_set_or(lefl_bit_array_t* arr, int16_t n,bool b);
    bool lefl_bit_array_get(lefl_bit_array_t* arr, int16_t n);
    void lefl_bit_array_shift(lefl_bit_array_t* arr, int16_t n);

    typedef uint16_t lefl_stack_elm_t;

    typedef struct __lefl_stack_t
    {
        lefl_stack_elm_t *data;
        int16_t top;
        int16_t len;
    } lefl_stack_t;

    lefl_stack_elm_t lefl_stack_get(lefl_stack_t* stack, lefl_stack_elm_t j);
    void lefl_stack_push(lefl_stack_t* stack, lefl_stack_elm_t t);
    lefl_stack_elm_t lefl_stack_pop(lefl_stack_t* stack, lefl_stack_elm_t *t);


    typedef float lefl_loop_queue_elm_t;

    typedef struct __lefl_loop_queue_t
    {
        lefl_loop_queue_elm_t *data;
        int16_t front;
        int16_t rear;
        int16_t len;
    } lefl_loop_queue_t;

    void lefl_loop_queue_init(lefl_loop_queue_t* q, lefl_loop_queue_elm_t*data, uint16_t len);
    lefl_loop_queue_elm_t lefl_loop_queue_dequeue(lefl_loop_queue_t* q);
    void lefl_loop_queue_enqueue(lefl_loop_queue_t* q, lefl_loop_queue_elm_t t);
#define lefl_loop_queue_foreach(q,i) for(uint16_t (i)=(q)->front;(i)!=(q)->rear;(i)=(i+1)%(q)->len)
#ifdef __cplusplus
}
#endif

#endif /* LEFL_H_ */
