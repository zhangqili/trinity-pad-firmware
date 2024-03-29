#ifndef RECORD_H_
#define RECORD_H_
#include "keyboard.h"
#include "keyboard_conf.h"

#define KPS_HISTORY_LENGTH    65
#define BIT_STREAM_LENGTH    128
#define ANALOG_HISTORY_LENGTH 129
#define RECORD_MAX_KEY_NUM    8
#define KPS_REFRESH_RATE      144

#define BIT_DATA_LENGTH ((BIT_STREAM_LENGTH - 1) / (sizeof(size_t) * 8) + 1)

typedef uint16_t LoopArrayElement;

typedef struct __LoopArray
{
    LoopArrayElement *data;
    int16_t index;
    int16_t len;
} LoopArray;

#ifdef ENABLE_COUNTER
extern uint32_t g_key_init_counts[ADVANCED_KEY_NUM];
extern uint32_t g_key_counts[ADVANCED_KEY_NUM];
#endif

#ifdef ENABLE_KPS
extern uint16_t g_kps;
extern LoopArray g_kps_data;
#endif

#ifdef ENABLE_KPS_HISTORY
extern LoopArray g_kps_history;
extern uint16_t g_kps_max_since_last_timer;
#endif

#ifdef ENABLE_BIT_STREAM
extern size_t g_bit_stream_datas[ADVANCED_KEY_NUM][BIT_DATA_LENGTH];
#endif

#ifdef ENABLE_ANALOG_HISTORY
extern LoopArray g_analog_historys[ADVANCED_KEY_NUM];
#endif
void loop_array_push_back(LoopArray *arr,LoopArrayElement t);
uint16_t loop_array_get(LoopArray *arr,LoopArrayElement j);
LoopArrayElement loop_array_max(LoopArray *arr);
//void record_kps_register(Key*k);
//void record_bit_stream_register(Key*k);
//void record_analog_register(AdvancedKey*k);
void record_init();
void record_bit_stream_timer();
void record_analog_timer();
void record_kps_timer();
uint16_t record_get_kps();
void record_kps_tick();
void record_kps_timer();
void record_kps_history_timer();

#endif /* RECORD_H_ */
