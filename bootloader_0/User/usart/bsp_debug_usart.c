/**
  ******************************************************************************
  * @file    bsp_debug_usart.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   ʹ�ô���1���ض���c��printf������usart�˿ڣ��жϽ���ģʽ
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� STM32 F103 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "./usart/bsp_debug_usart.h"

UART_HandleTypeDef UartHandle;
DMA_HandleTypeDef DMA_Handle;

uint8_t DMA_Rx_databuffer[USART_RX_DATA_SINGAL_MAX]; 
USART_RxControl *USART_RxCU;
//extern uint8_t ucTemp;  

/*������Ϣ�ṹ��*/
static USART_RxControl *USART_Rxdata_control_init(void)
{
  USART_RxControl *self = (USART_RxControl *)malloc(sizeof(USART_RxControl));

  self->rx_count = 0;
//  / self->rx_dataBuff = DMA_Rx_databuffer;
  self->IN = &self->rx_dataBuff[0];
  self->OUT = &self->rx_dataBuff[0];
  self->END =&self->rx_dataBuff[NUM-1];
  self->IN->start = DMA_Rx_databuffer;

  return self;
}

 /**
  * @brief  DEBUG_USART GPIO ����,����ģʽ���á�115200 8-N-1
  * @param  ��
  * @retval ��
  */  
void DEBUG_USART_Config(void)
{ 
  
  UartHandle.Instance          = DEBUG_USART;
  
  UartHandle.Init.BaudRate     = DEBUG_USART_BAUDRATE;
  UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits     = UART_STOPBITS_1;
  UartHandle.Init.Parity       = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode         = UART_MODE_TX_RX;
  
  HAL_UART_Init(&UartHandle);
	//HAL_USART_Receive_DMA();
    
 /*ʹ�ܴ��ڽ��ն� */
 // __HAL_UART_ENABLE_IT(&UartHandle,UART_IT_RXNE);  UART_FLAG_IDLE
	__HAL_UART_ENABLE_IT(&UartHandle,UART_FLAG_IDLE);	//�����ж�
}

/**
  * @brief UART MSP ��ʼ�� 
  * @param huart: UART handle
  * @retval ��
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{  
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  DEBUG_USART_CLK_ENABLE();
	
	DEBUG_USART_RX_GPIO_CLK_ENABLE();
  DEBUG_USART_TX_GPIO_CLK_ENABLE();
  
/**USART1 GPIO Configuration    
  PA9     ------> USART1_TX
  PA10    ------> USART1_RX 
  */
  /* ����Tx����Ϊ���ù���  */
  GPIO_InitStruct.Pin = DEBUG_USART_TX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed =  GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStruct);
  
  /* ����Rx����Ϊ���ù��� */
  GPIO_InitStruct.Pin = DEBUG_USART_RX_PIN;
  GPIO_InitStruct.Mode=GPIO_MODE_AF_INPUT;	//ģʽҪ����Ϊ��������ģʽ��	
  HAL_GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStruct); 
   //��ռ���ȼ�0�������ȼ�1
  HAL_NVIC_SetPriority(DEBUG_USART_IRQ ,0,1);	
	 //ʹ��USART1�ж�ͨ��  
  HAL_NVIC_EnableIRQ(DEBUG_USART_IRQ );
	USART_RxCU = USART_Rxdata_control_init();

}


//DMA��ʼ������
HAL_StatusTypeDef DMA_init(void)
{
  DMA_CLK_FUNC_ENABLE();

  DMA_Handle.Instance = DMA_CHANNEL;
  DMA_Handle.Init.Direction = DMA_PERIPH_TO_MEMORY;
  DMA_Handle.Init.PeriphInc = DMA_PINC_DISABLE;
  DMA_Handle.Init.MemInc = DMA_MINC_ENABLE;
  DMA_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  DMA_Handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  DMA_Handle.Init.Mode = DMA_NORMAL;
  DMA_Handle.Init.Priority = DMA_PRIORITY_HIGH;

  //DMA_Handle.Lock = HAL_UNLOCKED;
  //DMA_Handle.State = HAL_DMA_STATE_READY;
  if ( HAL_DMA_Init(&DMA_Handle) != HAL_OK )
  {
    return HAL_ERROR;
  }
  HAL_DMA_Start(&DMA_Handle,(uint32_t)&(DEBUG_USART->DR),(uint32_t)&(DMA_Rx_databuffer),sizeof(DMA_Rx_databuffer));
  

  return HAL_OK;
}




/********************************************************/
/********************************************************/
/*****************  �����ַ��� **********************/
void Usart_SendString(uint8_t *str)
{
	unsigned int k=0;
  do 
  {
      HAL_UART_Transmit(&UartHandle,(uint8_t *)(str + k) ,1,1000);
      k++;
  } while(*(str + k)!='\0');
  
}
//�ض���c�⺯��printf������DEBUG_USART���ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
	/* ����һ���ֽ����ݵ�����DEBUG_USART */
	HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 1000);	
	
	return (ch);
}

//�ض���c�⺯��scanf������DEBUG_USART����д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{		
	int ch;
	HAL_UART_Receive(&UartHandle, (uint8_t *)&ch, 1, 1000);	
	return (ch);
}

/*********************************************END OF FILE**********************/
