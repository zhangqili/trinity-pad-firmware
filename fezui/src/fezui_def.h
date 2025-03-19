/*
 * fezui_def.h
 *
 *  Created on: Jul 20, 2023
 *      Author: xq123
 */

#ifndef FEZUI_DEF_H_
#define FEZUI_DEF_H_

#define ROUND(x) ((int)((x)+0.5f))

#ifdef BIT
#undef BIT
#define BIT(n) (1UL << (n))
#else
#define BIT(n) (1UL << (n))
#endif

#define VAR_LOOP_INCREMENT(x,min,max,delta) if((x)<(max)) (x)+=(delta);else (x)=(min);

#define VAR_LOOP_DECREMENT(x,min,max,delta) if((x)>(min)) (x)-=(delta);else (x)=(max);

#define VAR_INCREMENT(x,min,max,delta) if((x)<(max)) (x)+=(delta);

#define VAR_DECREMENT(x,min,max,delta) if((x)>(min)) (x)-=(delta);

#define CONVERGE_TO_ROUNDED(x,y,m) (x)=(x)+(m)*((y)+0.5f-(x))

#define CONVERGE_TO(x,y,m) (x)=(x)+(m)*((y)-(x))

#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050) /* ARM Compiler V6 */
  #ifndef __WEAK
    #define __WEAK  __attribute__((weak))
  #endif
  #ifndef __packed
    #define __packed  __attribute__((packed))
  #endif
#elif defined ( __GNUC__ ) && !defined (__CC_ARM) /* GNU Compiler */
  #ifndef __WEAK
    #define __WEAK   __attribute__((weak))
  #endif /* __WEAK */
  #ifndef __packed
    #define __packed __attribute__((__packed__))
  #endif /* __packed */
#endif /* __GNUC__ */

#endif /* FEZUI_DEF_H_ */
