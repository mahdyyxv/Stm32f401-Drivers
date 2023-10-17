/**
 * @file USART_program.c
 * @author Ahmed Mahdy (mahdyyxv@yahoo.com)
 * @brief 
 * @version 0.1
 * @date 2023-09-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "BIT_MATH.h"
#include "STD_TYPES.h"
#include <string.h>

#include "GPIO_interface.h"
#include "NVIC_interface.h"
#include "DET.h"

#include "USART_interface.h"
#include "USART_config.h"
#include "USART_private.h"
static volatile uint8 u8StringBuffer1[USART_MAX_BUFFER_LENGTH] = {'\0'};
static volatile uint8 u8StringBuffer2[USART_MAX_BUFFER_LENGTH] = {'\0'};
static volatile uint8 u8StringBuffer6[USART_MAX_BUFFER_LENGTH] = {'\0'};
static uint8 u8Is_Uart_Init[3] = {0};
void (*USART1_pvCallBack) (void);
void (*USART2_pvCallBack) (void);
void (*USART6_pvCallBack) (void);
UART_Buffer_t udtUsart1Buffer = 
{
    .u8Head = 0,
    .u8Tail = 0,
    .u8BufferStatus[0] = USART_BUFFER_AVAILABLE
};

UART_Buffer_t udtUsart2Buffer = 
{
    .u8Head = 0,
    .u8Tail = 0,
    .u8BufferStatus[0] = USART_BUFFER_AVAILABLE
};

UART_Buffer_t udtUsart6Buffer = 
{
    .u8Head = 0,
    .u8Tail = 0,
    .u8BufferStatus[0] = USART_BUFFER_AVAILABLE
};
/**
 * @brief Init Uart From passed Configurations
 * 
 * @param ptrConfig 
 */

void USART_vInit(const USART_CONFIG_t *ptrConfig)
{
    if(ptrConfig == NULL)
    {
#if USART_DET_USED == STD_ON
        Det_ReportError(USART_MODULE_ID, USART_V_INIT_ID, USART_PASSING_NULL_ARG);
#endif
    }
    
    else 
    {
        GPIO_vInit(&(ptrConfig -> udtUSART_Pins[0]));
        GPIO_vInit(&(ptrConfig -> udtUSART_Pins[1]));
        switch ((ptrConfig -> u8UsartId))
        {
            case USART_1:
                        USART1->CR2.B.STOP = (ptrConfig -> udtStopBit);
                        if(ptrConfig->udtParitySelect == DISABLED)
                        {
                            /*DO Nothing*/
                        }
                        else
                        {   
                            USART1 -> CR1.B.PCE = 1;
                            USART1 -> CR1.B.PS  = ptrConfig -> udtParityType;
                        }
                        USART1 -> BRR.R = (uint32)(25000000 / (ptrConfig -> u32BaudRate));

                        USART1 -> CR1.B.TE = 1;
                        USART1 -> CR1.B.RE = 1;
                        USART1 -> CR1.B.UE = 1;
                        u8Is_Uart_Init[0] = 1;
                        break;
                        
            case USART_2:
                        USART2->CR2.B.STOP = (ptrConfig -> udtStopBit);
                        if(ptrConfig->udtParitySelect == DISABLED)
                        {
                            /*DO Nothing*/
                        }
                        else
                        {   
                            USART2 -> CR1.B.PCE = 1;
                            USART2 -> CR1.B.PS  = ptrConfig -> udtParityType;
                        }
                        USART2 -> BRR.R = (25000000 / (ptrConfig -> u32BaudRate));

                        USART2 -> CR1.B.TE = 1;
                        USART2 -> CR1.B.RE = 1;
                        u8Is_Uart_Init[1] = 1;
                        break;
            case USART_6:
                        USART6->CR2.B.STOP = (ptrConfig -> udtStopBit);
                        if(ptrConfig->udtParitySelect == DISABLED)
                        {
                            /*DO Nothing*/
                        }
                        else
                        {   
                            USART6 -> CR1.B.PCE = 1;
                            USART6 -> CR1.B.PS  = ptrConfig -> udtParityType;
                        }
                        USART6 -> BRR.R = (25000000 / (ptrConfig -> u32BaudRate));

                        USART6 -> CR1.B.TE = 1;
                        USART6 -> CR1.B.RE = 1;
                        u8Is_Uart_Init[2] = 1;
                        break;
            default:
#if USART_DET_USED == STD_ON
                    Det_ReportError(USART_MODULE_ID, USART_V_INIT_ID, USART_MODULE_ERR_ID);
#endif        
                    break;
            }
                        
    }
}



/**
 * @brief 
 * 
 * @param u8UsartId
 * @param u8Data 
 * @return Std_ReturnType 
 */
Std_ReturnType  USART_udtTransmitCharachter(uint8 u8UsartId, uint8 u8Data)
{
    Std_ReturnType udt_ReturnValue = E_NOT_OK;
    if(!(IS_Initialized(u8UsartId)))
    {
#if USART_DET_USED == STD_ON
        Det_ReportError(USART_MODULE_ID, USART_UDT_TRANSMIT_CHARACHTER, USART_NOT_INIT);
#endif        
    }
    else
    {
        switch (u8UsartId)
        {
            case USART_1:
                        USART1 -> DR.B.DR = u8Data;
                        while(USART1 -> SR.B.TXE == 0);/* Wait till flag be rised To know that data transferred into
                                                        the shift register*/
                        udt_ReturnValue = E_OK;
                        break;
            case USART_2:
                        USART2 -> DR.B.DR = u8Data;
                        while(USART2 -> SR.B.TXE == 0);/* Wait till flag be rised To know that data transferred into
                                                        the shift register*/
                        udt_ReturnValue = E_OK;
                        break;
            
            case USART_6:
                        USART6 -> DR.B.DR = u8Data;
                        while(USART6 -> SR.B.TXE == 0);/* Wait till flag be rised To know that data transferred into
                                                        the shift register*/
                        udt_ReturnValue = E_OK;
                        break;
            default:
#if USART_DET_USED == STD_ON
                    Det_ReportError(USART_MODULE_ID, USART_UDT_TRANSMIT_CHARACHTER, USART_MODULE_ERR_ID);
#endif 
                    break;
        }
    }
    return udt_ReturnValue;
}
/**
 * @brief 
 * 
 * @param u8UsartId
 * @param pu8Data 
 * @return Std_ReturnType 
 */
Std_ReturnType  USART_udtReceiveCharachter (uint8 u8UsartId, uint8* pu8Data)
{
    Std_ReturnType udt_ReturnValue = E_NOT_OK;
    uint32 u32LocalCounter = 0;
    if(!(IS_Initialized(u8UsartId)))
    {
#if USART_DET_USED == STD_ON
        Det_ReportError(USART_MODULE_ID, USART_UDT_RECEIVE_CHARACHTER, USART_NOT_INIT);
#endif        
    }

    else
    {
        *pu8Data = 0;
        switch (u8UsartId)
        {
            case USART_1:
                        while(USART1 -> SR.B.RXNE == 0)
                        {
                            u32LocalCounter++;
                            if(u32LocalCounter == USART_USER_TIMOUT_VALUE)
                            {
                                *pu8Data = 255;
                                break;
                            }
                        }
                        if(*pu8Data == 0)
                        {
                            *pu8Data = USART1 -> DR.R;
                        }
                        udt_ReturnValue = E_OK;
                        break;
            case USART_2:
                        while(USART2 -> SR.B.RXNE == 0)
                        {
                            u32LocalCounter++;
                            if(u32LocalCounter == USART_USER_TIMOUT_VALUE)
                            {
                                *pu8Data = 255;
                                break;
                            }
                        }
                        if(*pu8Data == 0)
                        {
                            *pu8Data = USART2 -> DR.R;
                        }
                        udt_ReturnValue = E_OK;
                        break;
            
            case USART_6:
                        while(USART6 -> SR.B.RXNE == 0)
                        {
                            u32LocalCounter++;
                            if(u32LocalCounter == USART_USER_TIMOUT_VALUE)
                            {
                                *pu8Data = 255;
                                break;
                            }
                        }
                        if(*pu8Data == 0)
                        {
                            *pu8Data = USART6 -> DR.R;
                        }
                        udt_ReturnValue = E_OK;
                        break;
            default:
#if USART_DET_USED == STD_ON
                    Det_ReportError(USART_MODULE_ID, USART_UDT_RECEIVE_CHARACHTER, USART_MODULE_ERR_ID);
#endif
                    break;
        }
    }
    return udt_ReturnValue;
}
/**
 * @brief
 *
 * @param u8UsartId
 * @param pu8DataString
 * @return Std_ReturnType
 */
Std_ReturnType  USART_udtAsyncTransmitString(uint8 u8UsartId, uint8 *pu8DataString)
{
    Std_ReturnType udt_ReturnValue = E_NOT_OK;
    if(!(IS_Initialized(u8UsartId)))
    {
#if USART_DET_USED == STD_ON
        Det_ReportError(USART_MODULE_ID, USART_UDT_RECEIVE_CHARACHTER, USART_NOT_INIT);
#endif
    }
    else if( pu8DataString == NULL)
    {

    }
    else
    {
        switch (u8UsartId)
        {
            case USART_1:
                        if(EN_Queue(&udtUsart1Buffer, pu8DataString) == 1)
                        {

                            if(USART1 -> CR1.B.TXEIE == 0)
                            {
                            	USART1 -> CR1.B.TXEIE = 1;
                            	NVIC_udtEnableInterrupt(37);
                            }

                            udt_ReturnValue = E_OK;
                        }
                        else
                        {
                        }
                        break;
            case USART_2:
                        if(EN_Queue(&udtUsart2Buffer, pu8DataString) == 1)
                        {

                            if(USART2 -> CR1.B.TXEIE == 0)
                            {
                            	USART2 -> CR1.B.TXEIE = 1;
                            	NVIC_udtEnableInterrupt(38);
                            }

                            udt_ReturnValue = E_OK;
                        }
                        else
                        {
                        }
                        break;
            case USART_6:
                        if(EN_Queue(&udtUsart6Buffer, pu8DataString) == 1)
                        {

                            if(USART6 -> CR1.B.TXEIE == 0)
                            {
                            	USART6 -> CR1.B.TXEIE = 1;
                            	NVIC_udtEnableInterrupt(71);
                            }

                            udt_ReturnValue = E_OK;
                        }
                        else
                        {
                        }
                        break;
            default:
#if USART_DET_USED == STD_ON
                    Det_ReportError(USART_MODULE_ID, USART_UDT_ASYNC_TRANSMIT_STRING, USART_MODULE_ERR_ID);
#endif
                    break;
        }
    }

    return udt_ReturnValue;
}
/**
 * @brief
 *
 * @param u8UsartId
 * @param pu8DataString
 * @return Std_ReturnType
 */
Std_ReturnType  USART_udtTransmitString(uint8 u8UsartId, uint8* pu8DataString)
{
    Std_ReturnType udt_ReturnValue=E_NOT_OK;
    uint8 counter = 0;
    if(!(IS_Initialized(u8UsartId)))
    {
#if USART_DET_USED == STD_ON
        Det_ReportError(USART_MODULE_ID, USART_UDT_TRANSMIT_STRING, USART_NOT_INIT);
#endif
    }
    else
    {
        switch (u8UsartId)
        {
            case USART_1:
                        for(counter = 0; counter < strlen(pu8DataString); counter++)
                        {
                            USART1 -> DR.B.DR = pu8DataString[counter];
                            while(USART1 -> SR.B.TXE == 0);
                        }
                        USART_udtTransmitCharachter(USART_1, '\n');
                        USART_udtTransmitCharachter(USART_1, '\r');
                        break;
            case USART_2:
                        for(counter = 0; counter < strlen(pu8DataString); counter++)
                        {
                            USART2 -> DR.B.DR = pu8DataString[counter];
                            while(USART2 -> SR.B.TXE == 0);
                        }
                        USART_udtTransmitCharachter(USART_2, '\n');
                        USART_udtTransmitCharachter(USART_2, '\r');
                        break;
            case USART_6:
                        for(counter = 0; counter < strlen(pu8DataString); counter++)
                        {
                            USART6 -> DR.B.DR = pu8DataString[counter];
                            while(USART6 -> SR.B.TXE == 0);
                        }
                        USART_udtTransmitCharachter(USART_6, '\n');
                        USART_udtTransmitCharachter(USART_6, '\r');
                        break;
            default:
#if USART_DET_USED == STD_ON
                    Det_ReportError(USART_MODULE_ID, USART_UDT_TRANSMIT_STRING, USART_MODULE_ERR_ID);
#endif
                    break;
        }
    }
    return udt_ReturnValue;
}
/**
 * @brief 
 * 
 * @param u8UsartId
 * @param pu8RetData
 * @return Std_ReturnType
 */
Std_ReturnType  USART_udtTransmitBufferStatus(uint8 u8UsartId, uint8* pu8RetData)
{
    Std_ReturnType udt_ReturnValue=E_NOT_OK;
    uint8 u8LocalCounter = 0;
    if((IsQueueEmpty(&udtUsart1Buffer) == 0) && 
       (IsQueueEmpty(&udtUsart2Buffer) == 0) && 
       (IsQueueEmpty(&udtUsart6Buffer) == 0))
    {

    }
    else
    {
        switch (u8UsartId)
        {
            case USART_1:
                        for(u8LocalCounter = 0; u8LocalCounter<50; u8LocalCounter++)
                        {
                            pu8RetData[u8LocalCounter] = udtUsart1Buffer.u8BufferStatus[u8LocalCounter];
                        }
                        udt_ReturnValue = E_OK;
                        break;
            case USART_2:
                        for(u8LocalCounter = 0; u8LocalCounter<50; u8LocalCounter++)
                        {
                            pu8RetData[u8LocalCounter] = udtUsart2Buffer.u8BufferStatus[u8LocalCounter];
                        }
                        udt_ReturnValue = E_OK;
                        break;
            case USART_6:
                        for(u8LocalCounter = 0; u8LocalCounter<50; u8LocalCounter++)
                        {
                            pu8RetData[u8LocalCounter] = udtUsart6Buffer.u8BufferStatus[u8LocalCounter];
                        }
                        udt_ReturnValue = E_OK;
                        break;
            default:
                break;
        }
    }
    return udt_ReturnValue;
}
/**
 * @brief Receive function only used inside rx interrupt there is no waiting for rx flag here 
 *        if you want to use polling try USART_udtReceiveCharachter Function
 * 
 * @param u8UsartId 
 * @param pu8Data 
 * @return Std_ReturnType 
 */
Std_ReturnType  USART_udtRec(uint8 u8UsartId, uint8* pu8Data)
{
    Std_ReturnType udtReturnValue = E_NOT_OK;
    if(!(IS_Initialized(u8UsartId)))
    {
#if USART_DET_USED == STD_ON
        Det_ReportError(USART_MODULE_ID, USART_UDT_REC, USART_NOT_INIT);
#endif
    }
    else
    {
        switch (u8UsartId)
        {
        case USART_1:
                    *pu8Data = USART1 -> DR.B.DR;
                    udtReturnValue = E_OK;
                    break;
        case USART_2:
                    *pu8Data = USART2 -> DR.B.DR;
                    udtReturnValue = E_OK;
                    break;
        case USART_6:
                    *pu8Data = USART6 -> DR.B.DR;
                    udtReturnValue = E_OK;
                    break;
        default:
#if USART_DET_USED == STD_ON
                Det_ReportError(USART_MODULE_ID, USART_UDT_REC, USART_MODULE_ERR_ID);
#endif
                break;
        }
    }
    return udtReturnValue;
}
/**
 * @brief 
 * 
 * @param u8UsartId 
 * @param pvCallBack 
 * @return Std_ReturnType 
 */
Std_ReturnType USART_vSetRxCallBack(uint8 u8UsartId, void (*pvCallBack) (void))
{
    Std_ReturnType udtReturnValue = E_NOT_OK;
    if(!(IS_Initialized(u8UsartId)))
    {
#if USART_DET_USED == STD_ON
        Det_ReportError(USART_MODULE_ID, USART_V_SET_RX_CALLBACK, USART_NOT_INIT);
#endif
    }
    else
    {
        switch (u8UsartId)
        {
        case USART_1:
                    USART1_pvCallBack = pvCallBack;
                    udtReturnValue = E_OK;
                    if(USART1 -> CR1.B.RXNEIE == 0)
                    {
                        USART1 -> CR1.B.RXNEIE = 1;
                        NVIC_udtEnableInterrupt(37);
                    }
                    else
                    {

                    }
                    break;
        case USART_2:
                    USART2_pvCallBack = pvCallBack;
                    udtReturnValue = E_OK;
                    if(USART2 -> CR1.B.RXNEIE == 0)
                    {
                        USART2 -> CR1.B.RXNEIE = 1;
                        NVIC_udtEnableInterrupt(38);
                    }
                    else
                    {
                        
                    }
                    break;
        case USART_6:
                    USART6_pvCallBack = pvCallBack;
                    udtReturnValue = E_OK;
                    if(USART6 -> CR1.B.RXNEIE == 0)
                    {
                        USART6 -> CR1.B.RXNEIE = 1;
                        NVIC_udtEnableInterrupt(71);
                    }
                    else
                    {
                        
                    }
                    break;
        default:
#if USART_DET_USED == STD_ON
                Det_ReportError(USART_MODULE_ID, USART_V_SET_RX_CALLBACK, USART_MODULE_ERR_ID);
#endif
                break;
        }
    }
    return udtReturnValue;

}
/**
 * @brief
 *
 * @param u8UartId 
 * @return uint8 
 */
static inline uint8 IS_Initialized(uint8 u8UartId)
{
   return u8Is_Uart_Init[(u8UartId-1)];
}

void USART1_IRQHandler()
{
    static uint8 u8Counter = 0;

    if(USART1->SR.B.TXE == 1)
    {
        if(IsQueueEmpty(&udtUsart1Buffer) != 0 && u8StringBuffer1[0] == '\0')
        {
            DE_Queue(&udtUsart1Buffer, u8StringBuffer1);
        }
        if( u8StringBuffer1 [u8Counter] != '\0')
        {
            USART1 -> DR.B.DR = u8StringBuffer1[u8Counter++];
        }
        else
        {
            u8Counter = 0;
            memset(u8StringBuffer1, '\0', USART_MAX_BUFFER_LENGTH);
            if(IsQueueEmpty(&udtUsart1Buffer) == 0)
            {
                USART1 -> CR1.B.TXEIE = 0;
                memset(udtUsart1Buffer.u8BufferStatus, USART_BUFFER_AVAILABLE, 50);
            }
            else
            {

            }
        }
    }
    if(USART1->SR.B.RXNE == 1)
    {
        USART1_pvCallBack();
    }    
}

void USART2_IRQHandler()
{
    static uint8 u8Counter = 0;

    if(USART2->SR.B.TXE == 1)
    {
        if(IsQueueEmpty(&udtUsart2Buffer) != 0 && u8StringBuffer2[0] == '\0')
        {
            DE_Queue(&udtUsart2Buffer, u8StringBuffer2);
        }
        if( u8StringBuffer2 [u8Counter] != '\0')
        {
            USART2 -> DR.B.DR = u8StringBuffer2[u8Counter++];
        }
        else
        {
            u8Counter = 0;
            memset(u8StringBuffer2, '\0', USART_MAX_BUFFER_LENGTH);
            if(IsQueueEmpty(&udtUsart2Buffer) == 0)
            {
                USART2 -> CR1.B.TXEIE = 0;
                memset(udtUsart2Buffer.u8BufferStatus, USART_BUFFER_AVAILABLE, 50);
            }
            else
            {

            }
        }
    }
    if(USART2->SR.B.RXNE == 1)
    {
        USART2_pvCallBack();
    }    
}

void USART6_IRQHandler()
{
    static uint8 u8Counter = 0;

    if(USART6->SR.B.TXE == 1)
    {
        if(IsQueueEmpty(&udtUsart6Buffer) != 0 && u8StringBuffer6[0] == '\0')
        {
            DE_Queue(&udtUsart6Buffer, u8StringBuffer6);
        }
        if( u8StringBuffer6 [u8Counter] != '\0')
        {
            USART6 -> DR.B.DR = u8StringBuffer6[u8Counter++];
        }
        else
        {
            u8Counter = 0;
            memset(u8StringBuffer6, '\0', USART_MAX_BUFFER_LENGTH);
            if(IsQueueEmpty(&udtUsart6Buffer) == 0)
            {
                USART6 -> CR1.B.TXEIE = 0;
                memset(udtUsart6Buffer.u8BufferStatus, USART_BUFFER_AVAILABLE, 50);
            }
            else
            {

            }
        }
    }
    if(USART6->SR.B.RXNE == 1)
    {
        USART6_pvCallBack();
    }    
}


static uint8 EN_Queue(UART_Buffer_t *ptr, uint8* data)
{
    uint8 u8RetVal = 0;
    if( ptr -> u8Tail == 50 )
    {
    }
    else
    {
        strcpy(ptr -> u8String[ptr -> u8Tail] , data);
        ptr -> u8BufferStatus[ptr->u8Tail] = USART_BUFFER_WAIT;
        ptr -> u8Tail++;
        ptr -> u8BufferStatus[ptr->u8Tail] = USART_BUFFER_AVAILABLE;
        u8RetVal = 1;
    }
    return u8RetVal;
}
static uint8 DE_Queue(UART_Buffer_t *ptr, uint8* retdata)
{
    uint8 i=0, retVal = 0;

    if( ptr -> u8Tail == ptr -> u8Head )
    {
    }
    else
    {
        strcpy(retdata, ptr -> u8String[0]);

        for(i = 0; i<= ptr -> u8Tail - 1; i++)
        {
            memset(ptr -> u8String[i], '\0', 50);
            strcpy(ptr -> u8String[i], ptr -> u8String[i+1]);
        }
        ptr -> u8Tail--;
        ptr -> u8BufferStatus[ptr->u8Tail] = USART_BUFFER_BUSY;
        retVal = 1;
    }
    return retVal;
}

static inline uint8 IsQueueEmpty(UART_Buffer_t *ptr)
{
    return (ptr->u8Tail);
}
