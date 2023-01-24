/*
 * File:   enc28j60.c
 * Author: kolel
 *
 * Created on 10 January 2023, 6:30 PM
 */


#include <xc.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "enc28j60defines.h"
#include "enc28j60Driver.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

extern size_t UART1_Write(uint8_t* pWrBuffer, const size_t size ); 
extern SemaphoreHandle_t xUartMutex;
uint8_t offset = 0;
xSemaphoreHandle xSpiMutex; 
uint8_t buffer[100];
void enc28j60_Task(void *pvParameters)
{
    //Create the SPI Mutex... 
    xUartMutex = xSemaphoreCreateMutex();
    xSpiMutex = xSemaphoreCreateMutex();
    
    
    uint8_t returnValue = 0; 
    UART1_Write((uint8_t *) "Hello World\r\n", 13);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    
    xSemaphoreTake(xSpiMutex, portMAX_DELAY); 
    ENC28J60_Init(); 
    xSemaphoreGive(xSpiMutex); 
    while(1)
    {
        
        xSemaphoreTake(xSpiMutex, portMAX_DELAY); 
        returnValue = Read_Control_Register(EIR, false);  
        
        if(returnValue & (1 << 7))
        {
            //Proper Packet was received... 
            
        }
        
        
        xSemaphoreGive(xSpiMutex); 
        

        
        
        

#if 0 
        sprintf((char *) buffer, "The size is %u\r\n", returnValue); 
        uint16_t length = strlen((char *) buffer); 
        
        
        xSemaphoreTake(xUartMutex, portMAX_DELAY); 
        UART1_Write(buffer, length);
        xSemaphoreGive(xUartMutex); 
#endif
        
        vTaskDelay(100 / portTICK_PERIOD_MS); 
    }
}
