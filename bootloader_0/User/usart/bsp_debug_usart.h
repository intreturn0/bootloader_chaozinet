#ifndef __DEBUG_USART_H
#define	__DEBUG_USART_H

#include "stm32f1xx.h"
#include <stdio.h>
#include <stdlib.h>

//串口波特率
#define DEBUG_USART_BAUDRATE                    115200

#define USART_RX_DATA_SIZE                      2048
#define USART_RX_DATA_SINGAL_MAX                256
#define NUM                                     10



typedef struct _USART_RxBuffPtr_
{
    uint8_t *start;
    uint8_t *end;
}USART_RxBuffPtr;

typedef struct _USART_RxControl_
{
    uint16_t rx_count;
    USART_RxBuffPtr rx_dataBuff[NUM];
    USART_RxBuffPtr *IN;
    USART_RxBuffPtr *OUT;
    USART_RxBuffPtr *END;
}USART_RxControl;




//引脚定义    串口
/*******************************************************/
#define DEBUG_USART                             USART1
#define DEBUG_USART_CLK_ENABLE()                __HAL_RCC_USART1_CLK_ENABLE();

#define DEBUG_USART_RX_GPIO_PORT                GPIOA
#define DEBUG_USART_RX_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()
#define DEBUG_USART_RX_PIN                      GPIO_PIN_10

#define DEBUG_USART_TX_GPIO_PORT                GPIOA
#define DEBUG_USART_TX_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define DEBUG_USART_TX_PIN                      GPIO_PIN_9

#define DEBUG_USART_IRQHandler                  USART1_IRQHandler
#define DEBUG_USART_IRQ                 		    USART1_IRQn
/************************************************************/

// DMA定义  dma1 channel5
#define DMA_x                                  DMA1
#define DMA_CHANNEL                            DMA1_Channel5
#define DMA_CLK_FUNC_ENABLE()                    __HAL_RCC_DMA1_CLK_ENABLE()





void Usart_SendString(uint8_t *str);
void DEBUG_USART_Config(void);
int fputc(int ch, FILE *f);
int fgetc(FILE *f);
extern UART_HandleTypeDef UartHandle;
extern DMA_HandleTypeDef DMA_Handle;
extern uint8_t DMA_Rx_databuffer[USART_RX_DATA_SINGAL_MAX]; 
extern USART_RxControl *USART_RxCU; 
HAL_StatusTypeDef DMA_init(void);
//USART_RxControl *USART_Rxdata_control_init(void);
#endif /* __USART1_H */
