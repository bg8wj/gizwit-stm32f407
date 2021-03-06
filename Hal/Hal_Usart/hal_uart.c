/**
************************************************************
* @file         hal_uart.c
* @brief        串口驱动
* @author       Gizwits
* @date         2016-09-05
* @version      V03010101
* @copyright    Gizwits
* 
* @note         机智云.只为智能硬件而生
*               Gizwits Smart Cloud  for Smart Products
*               链接|增值ֵ|开放|中立|安全|自有|自由|生态
*               www.gizwits.com
*
***********************************************************/
#include "stm32f4xx.h"
#include "Hal_Usart/hal_uart.h"

#ifdef __GNUC__
// With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf set to 'Yes') calls __io_putchar()
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


/**
* @brief Serial port GPIO init

* @param none
* @return none
*/
void uartGpioInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

#if defined (Open_UART1)
    USART1_GPIO_Cmd(USART1_GPIO_CLK, ENABLE);
	  USART1_CLK_Cmd(USART1_CLK, ENABLE);
	
    USART1_AFIO_Cfg(USART1_GPIO_PORT,USART1_AFIO_Sour1,USART1_AFIO_Per);
	USART1_AFIO_Cfg(USART1_GPIO_PORT,USART1_AFIO_Sour2,USART1_AFIO_Per);
	
    GPIO_InitStructure.GPIO_Pin = USART1_TxPin | USART1_RxPin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(USART1_GPIO_PORT, &GPIO_InitStructure);


#endif

#if defined (Open_UART2)

    USART2_GPIO_Cmd(USART2_GPIO_CLK, ENABLE);
    USART2_CLK_Cmd(USART2_CLK, ENABLE);
	
	USART2_AFIO_Cfg(USART2_GPIO_PORT,USART2_AFIO_Sour1,USART2_AFIO_Per);
	USART2_AFIO_Cfg(USART2_GPIO_PORT,USART2_AFIO_Sour2,USART2_AFIO_Per);

    GPIO_InitStructure.GPIO_Pin = USART2_TxPin | USART2_RxPin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(USART2_GPIO_PORT, &GPIO_InitStructure);

#endif

#if defined (Open_UART3)

    USART3_GPIO_Cmd(USART3_GPIO_CLK, ENABLE);
    USART3_CLK_Cmd(USART3_CLK, ENABLE);

    USART3_AFIO_Cfg(USART3_GPIO_PORT,USART3_AFIO_Sour1,USART3_AFIO_Per);
	USART3_AFIO_Cfg(USART3_GPIO_PORT,USART3_AFIO_Sour2,USART3_AFIO_Per);

    GPIO_InitStructure.GPIO_Pin = USART3_TxPin | USART3_RxPin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(USART3_GPIO_PORT, &GPIO_InitStructure);

#endif
}

/**
* @brief Serial port configure init

* @param none
* @return none
*/
void uartConfig(void)
{
    USART_InitTypeDef USART_InitStructure;
#if defined (Open_UART1)
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
    USART_Cmd(USART1, ENABLE);
    USART_ClearFlag(USART1, USART_FLAG_TC); //clear transmission complete flag
#endif

#if defined (Open_UART2)
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);

    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
    USART_Cmd(USART2, ENABLE);
    USART_ClearFlag(USART2, USART_FLAG_TC); //clear transmission complete flag
#endif

#if defined (Open_UART3)
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);

    USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
    USART_Cmd(USART3, ENABLE);
    USART_ClearFlag(USART3, USART_FLAG_TC); //clear transmission complete flag
#endif

}

/**
* @brief Serial port NVIC init

* @param none
* @return none
*/
void nvicConfiguration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
#if defined (Open_UART1)
    /*Enable serial port interrupt, and set the priority*/
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif

#if defined (Open_UART2)
    /*Enable serial port interrupt, and set the priority*/
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif

#if defined (Open_UART3)
    /*Enable serial port interrupt, and set the priority*/
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif
}

/**
* @brief UART1 write operation
*
* @param data     : 
*
* @return : none
*/
void uart1SendData(uint8_t data)
{
    USART_SendData(USART1,data);
    //Loop until the end of transmission
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

/**
* @brief Serial port init function
* @param none
* @return none
*/
void uartxInit(void)
{
    uartGpioInit();
    uartConfig();
    nvicConfiguration();
}

/**
* @brief printf redirection
* @param none
* @return none
*/
PUTCHAR_PROTOTYPE
{
    //Place your implementation of fputc here , e.g. write a character to the USART
    USART_SendData(USART1,(u8)ch);
    //Loop until the end of transmission
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    return ch;
}

/**
* @brief USART1 interrupt handle
* @param none
* @return none
*/
void USART1_IRQHandler(void)
{
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  { 		
		
		USART_SendData(USART1,USART_ReceiveData(USART1));
		//Loop until the end of transmission
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	
	}
}
