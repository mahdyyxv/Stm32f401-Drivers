/**
 * @file USART_private.h
 * @author Ahmed Mahdy (mahdyyxv@yahoo.com)
 * @brief Usart private file includes registers addresses and private declerations
 * @version 0.1
 * @date 2023-09-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef _USART_PRIVATE_H_
#define	_USART_PRIVATE_H_
/*************************_DEFINITIONS_OF_ADDRESSES_*************************/
#define UART_MODULE1_BASE_ADDRESS   0x40011000U
#define UART_MODULE2_BASE_ADDRESS   0x40004400U
#define UART_MODULE6_BASE_ADDRESS   0x40011400U
/**************************_END_OF_ADDRESSES_DEFINITIONS_**************************/

/**************************DIFINE_UART_TX_BUFFER**************************/
typedef struct
{
    uint8 u8String[50][USART_MAX_BUFFER_LENGTH];
    uint8 u8Head;
    uint8 u8Tail;
    uint8 u8BufferStatus[50];
}UART_Buffer_t;
/******************************************************************************/

/**
 * @brief Registers of uart module
 * 
 */
typedef struct 
{
    union
    {
        uint32 R;
        struct 
        {
            uint32 PE   :1;
            uint32 FE   :1;
            uint32 NF   :1;
            uint32 ORE  :1;
            uint32 IDLE :1;
            uint32 RXNE :1;
            uint32 TC   :1;
            uint32 TXE  :1;
            uint32 LBD  :1;
            uint32 CTS  :1;
            uint32 _RES :22;
        }B;
    }SR;

    union
    {
        uint32 R;
        struct 
        {
            uint32 DR   :9;
            uint32 _RES :23;
        }B;
    }DR;

    union
    {
        uint32 R;
        struct 
        {
            uint32 DIV_FRACTION     :4;
            uint32 DIV_MANTISSA     :12;
            uint32 _RES             :16;
        }B;
    }BRR;
    
    union
    {
        uint32 R;
        struct 
        {
            uint32 SBK      :1;
            uint32 RWU      :1;
            uint32 RE       :1;
            uint32 TE       :1;
            uint32 IDLEIE   :1;
            uint32 RXNEIE   :1;
            uint32 TCIE     :1;
            uint32 TXEIE    :1;
            uint32 PEIE     :1;
            uint32 PS       :1;
            uint32 PCE      :1;
            uint32 WAKE     :1;
            uint32 M        :1;
            uint32 UE       :1;
            uint32          :1;
            uint32 OVER8    :1;
            uint32          :16;
        }B;
    }CR1;
    
    
    union
    {
        uint32 R;
        struct 
        {
            uint32 ADD      :4;
            uint32          :1;
            uint32 LBDL     :1;
            uint32 LBDIE    :1;
            uint32          :1;
            uint32 LBCL     :1;
            uint32 CPHA     :1;
            uint32 CPOL     :1;
            uint32 CLKEN    :1;
            uint32 STOP     :2;
            uint32 LINEN    :1;
            uint32          :17;
        }B;
    }CR2;
    
    union
    {
        uint32 R;
        struct 
        {
            uint32 EIE      :1;
            uint32 IREN     :1;
            uint32 IRLP     :1;
            uint32 HDSEL    :1;
            uint32 NACK     :1;
            uint32 SCEN     :1;
            uint32 DMAR     :1;
            uint32 DMAT     :1;
            uint32 RTSE     :1;
            uint32 CTSE     :1;
            uint32 CTSIE    :1;
            uint32 ONEBIT   :1;
            uint32          :20
        }B;
    }CR3;

    union
    {
        uint32 R;
        struct 
        {
            uint32 PSC      :8;
            uint32 GT       :8;
            uint32          :16
        }B;
    }GTPR;

}USART_Type;
/* !Comment: USART Module Definitions*/
#define USART1       ((volatile USART_Type*) UART_MODULE1_BASE_ADDRESS)
#define USART2       ((volatile USART_Type*) UART_MODULE2_BASE_ADDRESS)
#define USART6       ((volatile USART_Type*) UART_MODULE6_BASE_ADDRESS)
/* !Comment: Helper Functions APIs*/
static inline uint8 IS_Initialized(uint8 u8UartId);
static inline uint8 IsQueueEmpty(UART_Buffer_t *ptr);
static uint8 EN_Queue(UART_Buffer_t *ptr, uint8* data);
static uint8 DE_Queue(UART_Buffer_t *ptr, uint8* retdata);
#endif/*_USART_PRIVATE_H_*/
