///*
// * lcd_hardware.h
// *
// *  Created on: Dec 2, 2017
// *      Author: joe
// */
//
#ifndef LCD_HARDWARE_H_
#define LCD_HARDWARE_H_
//
#include "stm32f4xx_hal.h"
#include "Global.h"

#ifdef __cplusplus
extern "C" {
#endif

extern SPI_HandleTypeDef  h_lcd_spi;
//
//
// Physical display size
//
//need for the gui toolkit config
#define XSIZE_PHYS  240 // To be adapted to x-screen size
#define YSIZE_PHYS  320 // To be adapted to y-screen size
// better names here, and rotate them. The LCD screen natively in portrait orientation
// but we want to use it in landscape. So the GUI toolkit we give it the native size
// but in our app we'll use our logical size
#define LCD_XSIZE YSIZE_PHYS
#define LCD_YSIZE XSIZE_PHYS


#define LCD_SPIx						 SPI1
#define LCD_SPI_SCK_PIN					 GPIO_PIN_3
#define LCD_SPI_SCK_PORT				 GPIOB
#define LCD_SPI_MISO_PIN				 GPIO_PIN_4
#define LCD_SPI_MISO_PORT				 GPIOB
#define LCD_SPI_MOSI_PIN				 GPIO_PIN_5
#define LCD_SPI_MOSI_PORT				 GPIOB
#define LCD_SPI_NSS_PIN					 GPIO_PIN_10
#define LCD_SPI_NSS_PORT				 GPIOB
#define LCD_SPI_DC_PIN					 GPIO_PIN_8
#define LCD_SPI_DC_PORT                  GPIOA
#define LCD_RESET_PIN					 GPIO_PIN_6
#define LCD_RESET_PORT					 GPIOB
#define LCD_SPI_CLK_ENABLE()			 __HAL_RCC_SPI1_CLK_ENABLE()

#define LCD_CS_LOW() 	HAL_GPIO_WritePin(LCD_SPI_NSS_PORT,LCD_SPI_NSS_PIN,GPIO_PIN_RESET)
#define LCD_CS_HIGH() 	HAL_GPIO_WritePin(LCD_SPI_NSS_PORT,LCD_SPI_NSS_PIN,GPIO_PIN_SET)
#define LCD_DC_LOW()	HAL_GPIO_WritePin(LCD_SPI_DC_PORT, LCD_SPI_DC_PIN, GPIO_PIN_RESET) /* command */
#define LCD_DC_HIGH()	HAL_GPIO_WritePin(LCD_SPI_DC_PORT, LCD_SPI_DC_PIN, GPIO_PIN_SET)   /* data */

#define LCD_SET_DATA()   LCD_DC_HIGH()
#define LCD_SET_CMD()    LCD_DC_LOW()
//
void LCD_WriteByte(uint8_t data);
void LCD_WriteWord(uint16_t data);
void LCD_WriteReg(U16 Data);
void LCD_WriteData(U16 Data);
void LCD_WriteDataMultiple(U16 * pData, int NumItems);
void LCD_ReadDataMultiple(U16 * pData, int NumItems);

void LCD_Set8Bit(void);
void LCD_Set16Bit(void);
void LCD_HardwareInit(void);
void LCD_ModuleInit();

//uint16_t LCD_ReadPixel();
//void LCD_SetColumn(uint16_t col_left, uint16_t col_right);
//void LCD_SetRow(uint16_t row_top, uint16_t row_bottom);
void LCD_FillScreen2(uint16_t color);

#ifdef __cplusplus
}
#endif

#endif /* LCD_HARDWARE_H_ */
