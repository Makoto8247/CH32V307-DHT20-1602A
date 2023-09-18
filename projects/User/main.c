/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/06/06
* Description        : Main program body.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

/*
 *@Note
 task1 and task2 alternate printing
*/

#include "debug.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <math.h>

/* Global define */
#define TASK1_TASK_PRIO     5
#define TASK1_STK_SIZE      125 * 2
#define TASK2_TASK_PRIO     4
#define TASK2_STK_SIZE      64

#define DHT20_ADDRESS       0x38
#define DHT20_CHECK_CMD     0x71    // Check status
#define DHT20_MEA_CMD       0xAC

#define LCD_ADDRESS         0x27
// commands
#define LCD_CMD             0x00
#define LCD_CLEARDISPLAY    0x01
#define LCD_ENTRYMODESET    0x04
#define LCD_FUNCTIONSET     0x20
#define LCD_DISPLAYCONTROL  0x08
// flags for display entry mode
#define LCD_ENTRYLEFT       0x02
// flags for function set
#define LCD_2LINE           0x08
// flags for display and cursor control
#define LCD_DISPLAYON       0x04
// flag for backlight control
#define LCD_BACKLIGHT       0x08
#define LCD_ENABLE_BIT      0x04

#define LCD_CHARACTER       0x01
#define MAX_LINES           2
#define MAX_CHARS           16

#define I2C_CLK             100000
#define DEVICE_ADDR         0x02

/* Global Variable */
TaskHandle_t Task1Task_Handler;
TaskHandle_t Task2Task_Handler;
QueueHandle_t xQueue;


/*
 *@Note
 7-bit address mode, EEPROM, transceiver routine:
 I2C2_SCL(PB10)\I2C2_SDA(PB11).

*/

void GPIO_Toggle_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void IIC_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    I2C_InitTypeDef  I2C_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);

    I2C_InitStructure.I2C_ClockSpeed = I2C_CLK;
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = DEVICE_ADDR;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C2, &I2C_InitStructure);

    I2C_Cmd(I2C2, ENABLE);
    I2C_AcknowledgeConfig(I2C2, ENABLE);
}

void DHT20_Init()
{
    u8 recieve_data = 0;
    TickType_t xLastWakeTime = xTaskGetTickCount();

    I2C_AcknowledgeConfig(I2C2, ENABLE);
    while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY) != RESET);

    do{
        vTaskDelayUntil(&xLastWakeTime, 500/portTICK_PERIOD_MS);
        I2C_GenerateSTART(I2C2, ENABLE);
        while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));
        // I2C transmitter mode
        I2C_Send7bitAddress(I2C2, DHT20_ADDRESS << 1, I2C_Direction_Transmitter);
        while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
        // send state cmd
        I2C_SendData(I2C2, DHT20_CHECK_CMD);
        while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
        I2C_GenerateSTOP(I2C2, ENABLE);

        I2C_GenerateSTART(I2C2, ENABLE);
        while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));
        // I2C receive
        I2C_Send7bitAddress(I2C2, DHT20_ADDRESS << 1, I2C_Direction_Receiver);
        while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
        while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));
        recieve_data = I2C_ReceiveData(I2C2);
        I2C_GenerateSTOP(I2C2, ENABLE);
    }while(recieve_data != 0x18 && recieve_data != 0x1c);
}

void DHT20_Data_Recieve(u8* data)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();

    I2C_AcknowledgeConfig(I2C2, ENABLE);
    vTaskDelayUntil(&xLastWakeTime, 100/portTICK_PERIOD_MS);
    while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY) != RESET);
    // cmd send
    I2C_GenerateSTART(I2C2, ENABLE);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));

    // I2C transmitter mode
    I2C_Send7bitAddress(I2C2, DHT20_ADDRESS << 1, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    I2C_SendData(I2C2, 0xAC);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_SendData(I2C2, 0x33);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_SendData(I2C2, 0x00);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_GenerateSTOP(I2C2, ENABLE);
    vTaskDelayUntil(&xLastWakeTime, 2000/portTICK_PERIOD_MS);
    // I2C receive
    I2C_GenerateSTART(I2C2, ENABLE);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C2, DHT20_ADDRESS << 1, I2C_Direction_Receiver);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    for(int i=0; i<7; i++){
        while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));
        data[i] = I2C_ReceiveData(I2C2);
    }
    I2C_GenerateSTOP(I2C2, ENABLE);
}

void Lcd_Write(u8, u8);
void Lcd_Clear();

void Lcd_Init()
{

    TickType_t xLastWakeTime = xTaskGetTickCount();
    Lcd_Write(0x03, LCD_CMD);
    vTaskDelayUntil(&xLastWakeTime, 5/portTICK_PERIOD_MS);
    Lcd_Write(0x03, LCD_CMD);
    Lcd_Write(0x03, LCD_CMD);
    Lcd_Write(0x02, LCD_CMD);
    Lcd_Write(LCD_ENTRYMODESET | LCD_ENTRYLEFT, LCD_CMD);

    Lcd_Write(LCD_FUNCTIONSET | LCD_2LINE, LCD_CMD);
    Lcd_Write(LCD_DISPLAYCONTROL | LCD_DISPLAYON, LCD_CMD);
    Lcd_Clear();
}

void Lcd_Write_Byte(u8 val)
{
    while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY) != RESET);
    I2C_GenerateSTART(I2C2, ENABLE);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C2, LCD_ADDRESS << 1, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    I2C_SendData(I2C2, val);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_GenerateSTOP(I2C2, ENABLE);
}

void Lcd_Toggle(u8 val)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    vTaskDelayUntil(&xLastWakeTime, 5/portTICK_PERIOD_MS);
    Lcd_Write_Byte(val | LCD_ENABLE_BIT);
    vTaskDelayUntil(&xLastWakeTime, 5/portTICK_PERIOD_MS);
    Lcd_Write_Byte(val & ~LCD_ENABLE_BIT);
    vTaskDelayUntil(&xLastWakeTime, 5/portTICK_PERIOD_MS);
}

void Lcd_Write(u8 val, u8 mode)
{
    u8 high = mode | (val & 0xf0) | LCD_BACKLIGHT;
    u8 low = mode | ((val << 4) & 0xf0) | LCD_BACKLIGHT;
    Lcd_Write_Byte(high);
    Lcd_Toggle(high);
    Lcd_Write_Byte(low);
    Lcd_Toggle(low);
}

void Lcd_Set_Cursor(int line, int position) {
    int val = (line == 0) ? 0x80 + position : 0xC0 + position;
    Lcd_Write(val, LCD_CMD);
}

static void inline Lcd_Char(char val) {
    Lcd_Write(val, LCD_CHARACTER);
}

void Lcd_Clear()
{
    Lcd_Write(LCD_CLEARDISPLAY, LCD_CMD);
}

void Lcd_String(const char *s) {
    while (*s) {
        Lcd_Char(*s++);
    }
}


/*********************************************************************
 * @fn      task1_task
 *
 * @brief   task1 program.
 *
 * @param  *pvParameters - Parameters point of task1
 *
 * @return  none
 */
void task1_task(void *pvParameters)
{
    u8 dht_recieve[7];
    u32 humidity_data = 0;
    u32 temperature_data = 0;
    float_t humidity;
    float_t temperature;
    char lcd_str_buf[16];

    DHT20_Init();
    Lcd_Init();

    while(1)
    {
        DHT20_Data_Recieve(dht_recieve);
        if((dht_recieve[0] & 0x80) == 0x00) {
            humidity_data =
                    (dht_recieve[1] << 12) |
                    (dht_recieve[2] << 4) |
                    ((dht_recieve[3] & 0xf0) >> 4);

            temperature_data =
                    ((dht_recieve[3] & 0x0f) << 16) |
                    (dht_recieve[4] << 8) |
                    (dht_recieve[5]);
        }
        // relative humidity conversion
        humidity = humidity_data / 10485.76;
        // temperature conversion
        temperature = temperature_data / 5242.88 - 50;

        printf("humidity = %d%% ", (u8)humidity);
        printf("temperature = %dC\r\n", (u8)temperature);
        sniprintf(lcd_str_buf, sizeof(lcd_str_buf), "HUMIDITY: %2.1f%%", humidity);
        Lcd_Set_Cursor(0,0);Lcd_String(lcd_str_buf);
        sniprintf(lcd_str_buf, sizeof(lcd_str_buf), "TEMPERAT: %2.1fC", temperature);
        Lcd_Set_Cursor(1,0);Lcd_String(lcd_str_buf);
    }
}


/*********************************************************************
 * @fn      task2_task
 *
 * @brief   task2 program.
 *
 * @param  *pvParameters - Parameters point of task2
 *
 * @return  none
 */
void task2_task(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_0);
        xLastWakeTime = xTaskGetTickCount();
        vTaskDelayUntil(&xLastWakeTime, 1000/portTICK_PERIOD_MS);
        GPIO_ResetBits(GPIOA, GPIO_Pin_0);
        xLastWakeTime = xTaskGetTickCount();
        vTaskDelayUntil(&xLastWakeTime, 1000/portTICK_PERIOD_MS);

    }
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{


	SystemCoreClockUpdate();
	USART_Printf_Init(115200);

	xQueue = xQueueCreate(2, sizeof(float_t));
	IIC_Init();
	GPIO_Toggle_Init();
		
	printf("SystemClk:%d\r\n",SystemCoreClock);
	printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
	printf("FreeRTOS Kernel Version:%s\r\n",tskKERNEL_VERSION_NUMBER);

	/* create two task */
    xTaskCreate((TaskFunction_t )task1_task,
                    (const char*    )"task1",
                    (uint16_t       )TASK1_STK_SIZE,
                    (void*          )NULL,
                    (UBaseType_t    )TASK1_TASK_PRIO,
                    (TaskHandle_t*  )&Task1Task_Handler);
    printf("Task1 Created\r\n");
    xTaskCreate((TaskFunction_t )task2_task,
                    (const char*    )"task2",
                    (uint16_t       )TASK2_STK_SIZE,
                    (void*          )NULL,
                    (UBaseType_t    )TASK2_TASK_PRIO,
                    (TaskHandle_t*  )&Task2Task_Handler);
    printf("Task2 Created\r\n");
    vTaskStartScheduler();

	while(1)
	{
	    printf("shouldn't run at here!!\n");
	}
}
