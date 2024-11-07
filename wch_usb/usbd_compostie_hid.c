/********************************** (C) COPYRIGHT *******************************
 * File Name          : usbd_composite_km.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2022/08/18
 * Description        : USB keyboard and mouse processing.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/


/*******************************************************************************/
/* Header Files */
#include "ch32v30x_usbhs_device.h"
#include "usbd_composite_hid.h"
#include "usb_hid_keys.h"

/*******************************************************************************/
/* Variable Definition */

uint32_t g_usb_report_count;
uint32_t g_usb_report_count1;
uint32_t g_usb_mouse_report_count;
uint32_t g_usb_mouse_report_count1;
uint32_t g_usb_raw_report_count;
uint32_t g_usb_raw_report_count1;
/* Mouse */
volatile uint8_t  MS_Scan_Done = 0x00;                                          // Mouse Movement Scan Done
volatile uint16_t MS_Scan_Result = 0x00F0;                                      // Mouse Movement Scan Result
uint8_t  MS_Data_Pack[ 4 ] = { 0x00 };                                          // Mouse IN Data Packet

/* Keyboard */
volatile uint8_t  KB_Scan_Done = 0x00;                                          // Keyboard Keys Scan Done
volatile uint16_t KB_Scan_Result = 0xF000;                                      // Keyboard Keys Current Scan Result
volatile uint16_t KB_Scan_Last_Result = 0xF000;                                 // Keyboard Keys Last Scan Result
uint8_t  KB_Data_Pack[ 8 ] = { 0x00 };                                          // Keyboard IN Data Packet
volatile uint8_t  KB_LED_Last_Status = 0x00;                                    // Keyboard LED Last Result
volatile uint8_t  KB_LED_Cur_Status = 0x00;                                     // Keyboard LED Current Result

/* USART */
volatile uint8_t  USART_Recv_Dat = 0x00;
volatile uint8_t  USART_Send_Flag = 0x00;
volatile uint8_t  USART_Send_Cnt = 0x00;


/*********************************************************************
 * @fn      USB_Sleep_Wakeup_CFG
 *
 * @brief   Configure USB wake up mode
 *
 * @return  none
 */
void USB_Sleep_Wakeup_CFG( void )
{
    EXTI_InitTypeDef EXTI_InitStructure = { 0 };

    EXTI_InitStructure.EXTI_Line = EXTI_Line20;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init( &EXTI_InitStructure );
}

/*********************************************************************
 * @fn      MCU_Sleep_Wakeup_Operate
 *
 * @brief   Perform sleep operation
 *
 * @return  none
 */
void MCU_Sleep_Wakeup_Operate( void )
{
    printf( "Sleep\r\n" );
    __disable_irq( );
    USBHSD->HOST_CTRL &= ~USBHS_UH_PHY_SUSPENDM;
    EXTI_ClearFlag( EXTI_Line12 | EXTI_Line13 | EXTI_Line14 | EXTI_Line15 );
    EXTI_ClearFlag( EXTI_Line4 | EXTI_Line5 | EXTI_Line6 | EXTI_Line7 );

    _SEV( );
    _WFE( );
    if( USBHSD->SUSPEND & USBHS_USB_WAKEUP_ST )
    {
        USBHSD->HOST_CTRL |= USBHS_UH_PHY_SUSPENDM;
        __enable_irq( );
        return;
    }
    PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFE);

    SystemInit();
    SystemCoreClockUpdate();
    USBHSD->HOST_CTRL |= USBHS_UH_PHY_SUSPENDM;
    USBHS_RCC_Init();

    if( EXTI_GetFlagStatus( EXTI_Line12 | EXTI_Line13 | EXTI_Line14 | EXTI_Line15 ) != RESET  )
    {
        EXTI_ClearFlag( EXTI_Line12 | EXTI_Line13 | EXTI_Line14 | EXTI_Line15 );
        USBHS_Send_Resume( );
    }
    else if( EXTI_GetFlagStatus( EXTI_Line4 | EXTI_Line5 | EXTI_Line6 | EXTI_Line7 ) != RESET )
    {
        EXTI_ClearFlag( EXTI_Line4 | EXTI_Line5 | EXTI_Line6 | EXTI_Line7 );
        USBHS_Send_Resume( );
    }
    __enable_irq( );
    printf( "Wake\r\n" );
}

void hid_raw_send(uint8_t *buffer, int size)
{

    //USBHS_EP3_Tx_Buf[0] = (uint8_t)pkg_len;
    //if (USBHS_DevSpeed == USBHS_SPEED_HIGH)
    //{
    //    USBHS_EP2_Tx_Buf[1] = (uint8_t)(pkg_len >> 8);
    //}
    memcpy(USBHS_EP3_TX_Buf, &buffer, size);
    USBHS_EP3_TX_Buf[0] = 0x02;
    USBHSD->UEP3_TX_DMA = (uint32_t)(uint8_t *)USBHS_EP3_TX_Buf;
    USBHSD->UEP3_TX_LEN = 64;
    USBHS_Endp_Busy[2] |= DEF_UEP_BUSY;
    USBHSD->UEP3_TX_CTRL = ((USBHSD->UEP3_TX_CTRL) & ~USBHS_UEP_T_RES_MASK) | USBHS_UEP_T_RES_ACK;
    //UART2_Rx_RemainLen -= pkg_len;
    //UART2_Rx_Deal_Ptr += pkg_len;
    //if (UART2_Rx_Deal_Ptr >= DEF_UART2_BUF_SIZE)
    //{
    //    UART2_Rx_Deal_Ptr = 0x00;
    //}
}