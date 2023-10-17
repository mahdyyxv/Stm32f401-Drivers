/**
 * @file USART_interface.h
 * @author Ahmed Mahdy (Mahdyyxv@yahoo.com)
 * @brief File contain user interface UDT and Function APIs
 * @version 0.1
 * @date 2023-09-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef _USART_INTERFACE_H_
#define	_USART_INTERFACE_H_

/********************************************_MACROS_********************************************/
#define USART_1                 (1U)
#define USART_2                 (2U)
#define USART_6                 (3U)

#define USART_BUFFER_AVAILABLE  (0U)
#define USART_BUFFER_WAIT       (1U)
#define USART_BUFFER_BUSY       (2U)
/********************************************_END_OF_MACROS********************************************/

typedef enum
{
    EVEN_PARITY,
    ODD_PARITY
}UART_PARITY_E;

typedef enum
{
    DISABLED,
    ENABLED,
}USART_EN_DIS_SELECT_E;

typedef enum
{   
    ONE_STOPBIT,
    HALF_STOPBIT,
    TWO_STOPBITS,
    ONEANDHALF_STOPBITS
}USART_STOPBIT_SELECT_E;


typedef struct 
{
    GPIO_PinState_t         udtUSART_Pins[2];
    
    USART_STOPBIT_SELECT_E  udtStopBit;
    USART_EN_DIS_SELECT_E   udtParitySelect;
    UART_PARITY_E           udtParityType;
    uint32                  u32BaudRate;
    uint8                   u8UsartId; 
}USART_CONFIG_t;


void USART_vInit(const USART_CONFIG_t *ptrConfig);
Std_ReturnType  USART_udtTransmitCharachter(uint8 u8UsartId, uint8 u8Data);
Std_ReturnType  USART_udtReceiveCharachter (uint8 u8UsartId, uint8* pu8Data);
Std_ReturnType  USART_udtAsyncTransmitString(uint8 u8UsartId, uint8 *pu8DataString);
Std_ReturnType  USART_udtTransmitString(uint8 u8UsartId, uint8* pu8DataString);
Std_ReturnType  USART_udtTransmitBufferStatus(uint8 u8UsartId, uint8* pu8RetData);
Std_ReturnType  USART_udtRec(uint8 u8UsartId, uint8* pu8Data);
Std_ReturnType  USART_vSetRxCallBack(uint8 u8UsartId, void (*pvCallBack) (void));

/* !Comment: Functions Id*/
#define USART_V_INIT_ID                 1U
#define USART_UDT_TRANSMIT_CHARACHTER   2U
#define USART_UDT_RECEIVE_CHARACHTER    3U
#define USART_UDT_TRANSMIT_STRING       4U
#define USART_UDT_ASYNC_TRANSMIT_STRING 5U
#define USART_UDT_TRANSMIT_BUFFERSTATUS 6U
#define USART_UDT_REC                   7U
#define USART_V_SET_RX_CALLBACK         8U
/* !Comment: Errors Id*/
#define USART_PASSING_NULL_ARG          1U
#define USART_NOT_INIT                  2U
#define USART_MODULE_ERR_ID             3U
#endif/*_USART_INTERFACE_H_*/
