/*
 * File:   blinky.c
 * Author: kolel
 *
 * Created on 10 January 2023, 4:26 PM
 */


#include <xc.h>
#include "FreeRTOS.h"
#include "peripheral/gpio/plib_gpio.h"
#include "task.h"
#include "semphr.h"

extern size_t UART1_Write(uint8_t* pWrBuffer, const size_t size ); 
SemaphoreHandle_t xUartMutex;
void Blinky_Task(void *pvParameters)
{
    while(1)
    {
        xSemaphoreTake(xUartMutex, portMAX_DELAY); 
        UART1_Write((uint8_t *) "Hello World from PIC32\r\n", 24);
        xSemaphoreGive(xUartMutex); 
        LED_Toggle();
        vTaskDelay(100 / portTICK_PERIOD_MS); 
    }
}