/**
************************************************************
* @file         hal_key.c
* @brief        按键驱动

* 按键模块采用定时器 + GPIO状态读取机制，GPIO的配置要根据STM32的相关手册来配置

* 本驱动支持 0 ~ 12 个GPIO按键扩展，支持跨平台移植。
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
#include "Hal_key/hal_key.h"
#include "stm32f4xx.h"
#include <math.h>

extern keysTypedef_t keys;

uint8_t                 keyCountTime;
static uint8_t          keyTotolNum = 0;

/**
* @brief TIM2 interrupt handler

* Detect key state
* @param none
* @return none
*/
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        keyHandle((keysTypedef_t *)&keys);
    }
}

/**
* @brief Timer init function

* 
* @param none
* @return none
*/
void timer2Init(void)
{
    u16 arr = 9; //re-load the initial value
    u16 psc = 8399; //Prescaler value
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //Clock enable

    //Timer TIM2 init
    TIM_TimeBaseStructure.TIM_Period = arr; //Set the value of the auto-reload register cycle in the next update event
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //Set the prescaler value used as the TIMx clock frequency
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //Set the clock split:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM count up mode
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //Initializes the time base according to the specified parameter

    TIM_ITConfig(TIM2, TIM_IT_Update,ENABLE ); //Enable the specified TIM3 interrupt, allowing update interrupts

    //Set NVIC interrupt priority
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2 interrupt
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //Excellent priority level 0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //Slave priority level 3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ IRQ channel is enabled
    NVIC_Init(&NVIC_InitStructure);  //Initialize the NVIC register
    TIM_Cmd(TIM2, ENABLE);  //TIMx enable
}

/**
* @brief Read the GPIO state
* @param [in] keys :Key global structure pointer
* @return GPIO status 
*/
uint16_t getKey(keysTypedef_t *keyS)
{
    uint8_t i = 0; 
    uint16_t readKey = 0;              //Essential init
    
    //GPIO Cyclic scan
    for(i = 0; i < keys.keyTotolNum; i++)
    {
        if(!GPIO_ReadInputDataBit((GPIO_TypeDef*)keyS->singleKey[i].keyPort,keyS->singleKey[i].keyGpio))
        {
            G_SET_BIT(readKey, keyS->singleKey[i].keyNum);
        }
    }
    
    return readKey;
}


/**
* @brief Read the KEY value
* @param [in] keys :Key global structure pointer
* @return GPIO status
*/
uint16_t readKeyValue(keysTypedef_t *keyS)
{
    static uint8_t keyCheck = 0;
    static uint8_t keyState = 0;
    static uint16_t keyLongCheck = 0;
    static uint16_t keyPrev = 0;      //last key

    uint16_t keyPress = 0;
    uint16_t keyReturn = 0;
    
    keyCountTime ++;
    
    if(keyCountTime >= (DEBOUNCE_TIME / KEY_TIMER_MS))          //keyCountTime 1MS+1  key eliminate jitter 10MS
    {
        keyCountTime = 0;
        keyCheck = 1;
    }
    if(1 == keyCheck)
    {
        keyCheck = 0;
        keyPress = getKey(keyS);
        switch (keyState)
        {
            case 0:
                if(keyPress != 0)
                {
                    keyPrev = keyPress;
                    keyState = 1;
                }
                break;
                
            case 1:
                if(keyPress == keyPrev)
                {
                    keyState = 2;
                    keyReturn= keyPrev | KEY_DOWN;
                }
                else                //Button lift, jitter, no response button
                {
                    keyState = 0;
                }
                break;
                
            case 2:

                if(keyPress != keyPrev)
                {
                    keyState = 0;
                    keyLongCheck = 0;
                    keyReturn = keyPrev | KEY_UP;
                    return keyReturn;
                }
                if(keyPress == keyPrev)
                {
                    keyLongCheck++;
                    if(keyLongCheck >= (PRESS_LONG_TIME / DEBOUNCE_TIME))    //Long press 3S effective
                    {
                        keyLongCheck = 0;
                        keyState = 3;
                        keyReturn= keyPress | KEY_LONG;
                        return keyReturn;
                    }
                }
                break;

            case 3:
                if(keyPress != keyPrev)
                {
                    keyState = 0;
                }
                break;
        }
    }
    return  NO_KEY;
}

/**
* @brief Key call-back function

* Detects the keys state and call the corresponding callback function
* @param [in] keys :Key global structure pointer
* @return none
*/
void keyHandle(keysTypedef_t *keyS)
{
    uint8_t i = 0;
    uint16_t key_value = 0;

    key_value = readKeyValue(keyS);

    if(!key_value) return;
    
    //Check short press button
    if(key_value & KEY_UP)
    {
        //Valid key is detected
        for(i = 0; i < keyS->keyTotolNum; i++)
        {
            if(G_IS_BIT_SET(key_value, keyS->singleKey[i].keyNum)) 
            {
                //key callback function of short press
                if(keyS->singleKey[i].shortPress) 
                {
                    keyS->singleKey[i].shortPress(); 

                }
            }
        }
    }

    //Check short long button
    if(key_value & KEY_LONG)
    {
        //Valid key is detected
        for(i = 0; i < keyS->keyTotolNum; i++)
        {
            if(G_IS_BIT_SET(key_value, keyS->singleKey[i].keyNum))
            {
                //key callback function of long press
                if(keyS->singleKey[i].longPress) 
                {
                    keyS->singleKey[i].longPress(); 

                }
            }
        }
    }
} 

/**
* @brief key init function

* 
* @param [in] keyRccPeriph APB2_peripheral
* @param [in] keyPort Peripheral_declaration
* @param [in] keyGpio GPIO_pins_define 
* @param [in] short_press :Short press state callback function address
* @param [in] long_press :Long press state callback function address
* @return key structure pointer
*/
keyTypedef_t keyInitOne(uint32_t keyRccPeriph, GPIO_TypeDef * keyPort, uint32_t keyGpio, gokitKeyFunction shortPress, gokitKeyFunction longPress)
{
    static int8_t key_total = -1;

    keyTypedef_t singleKey;
    
    //Platform-defined GPIO
    singleKey.keyRccPeriph = keyRccPeriph;
    singleKey.keyPort = keyPort;
    singleKey.keyGpio = keyGpio;
    singleKey.keyNum = ++key_total;
    
    //Button trigger callback type
    singleKey.longPress = longPress;
    singleKey.shortPress = shortPress;
    
    keyTotolNum++;
    
    return singleKey;
}

/**
* @brief key parameter init function

* Keys GPIO init,start timer detect keys state
* @param [in] keys ::Key global structure pointer
* @return none
*/
void keyParaInit(keysTypedef_t *keys)
{
    uint8_t temI = 0; 
    
    if(NULL == keys)
    {
        return ;
    }
    
    keys->keyTotolNum = keyTotolNum;
    
    //Limit on the number keys (Allowable number: 0~12)
    if(KEY_MAX_NUMBER < keys->keyTotolNum) 
    {
        keys->keyTotolNum = KEY_MAX_NUMBER; 
    }
    
    for(temI = 0; temI < keys->keyTotolNum; temI++) 
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        RCC_AHB1PeriphClockCmd(keys->singleKey[temI].keyRccPeriph, ENABLE);

        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

        GPIO_InitStructure.GPIO_Pin = keys->singleKey[temI].keyGpio;
        GPIO_Init(keys->singleKey[temI].keyPort, &GPIO_InitStructure);
    }
    
    //Init 1ms timer 
    timer2Init();
}
