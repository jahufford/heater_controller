///*
// * lcd_hardware.c
// *
// *  Created on: Dec 2, 2017
// *      Author: joe
// */
#include "lcd_hardware.h"
#include "ili9341.h"
#include "error.h" //TODO handle errors better
SPI_HandleTypeDef  h_lcd_spi;

static U16 last_reg_command=0;
//
///********************************************************************
//*
//*       LCD_WriteReg
//*
//* Function description:
//*   Sets display register
//*/
void LCD_WriteReg(U16 Data)
{
  // ... TBD by user
	last_reg_command = Data;
    uint8_t byte = (uint8_t)Data;
    LCD_DC_LOW();
    LCD_CS_LOW();
    if(HAL_SPI_Transmit(&h_lcd_spi,&byte, 1,0xFFFF) != HAL_OK){
        Error_Handler();
    }
    LCD_CS_HIGH();
}
///********************************************************************
//*
//*       LCD_WriteData
//*
//* Function description:
//*   Writes a value to a display register
//*/
void LCD_WriteData(U16 Data)
{
  // ... TBD by user
	uint8_t *p = (uint8_t*)&Data;
	LCD_CS_LOW();
	LCD_DC_HIGH();
	if(last_reg_command == 0x2C){ // write to graphic ram
        if(HAL_SPI_Transmit(&h_lcd_spi,(uint8_t*)(p+1), 1,0xFFFF) != HAL_OK){
            Error_Handler();
        }
        if(HAL_SPI_Transmit(&h_lcd_spi,(uint8_t*)(p), 1,0xFFFF) != HAL_OK){
            Error_Handler();
        }
	}else{
      if(HAL_SPI_Transmit(&h_lcd_spi,(uint8_t*)&Data, 1,0xFFFF) != HAL_OK){
        Error_Handler();
      }
	}
	LCD_CS_HIGH();
}
//
///********************************************************************
//*
//*       LCD_WriteDataMultiple
//*
//* Function description:
//*   Writes multiple values to a display register.
//*/
void LCD_WriteDataMultiple(U16 * pData, int NumItems)
{
    uint8_t *p = (uint8_t*)pData;
    LCD_CS_LOW();
    LCD_DC_HIGH();
    while (NumItems--) {
      // have to reverse the order of the bytes
      if(HAL_SPI_Transmit(&h_lcd_spi,(uint8_t*)(p+1), 1,0xFFFF) != HAL_OK){
        //Error_Handler();
      }
      if(HAL_SPI_Transmit(&h_lcd_spi,(uint8_t*)(p), 1,0xFFFF) != HAL_OK){
        //Error_Handler();
      }
      pData++;
    }
    LCD_CS_HIGH();
}

void LCD_WriteByte(uint8_t Data) {
  // ... TBD by user
	LCD_CS_LOW();
	LCD_DC_HIGH();
	if(HAL_SPI_Transmit(&h_lcd_spi,(uint8_t*)&Data, 1,0xFFFF) != HAL_OK){
		Error_Handler();
    }
	LCD_CS_HIGH();
}

void LCD_WriteWord(uint16_t data)
{
	uint8_t high_byte = (uint8_t)(data >> 8);
	uint8_t low_byte  = (uint8_t)data;

	if(HAL_SPI_Transmit(&h_lcd_spi,&high_byte, 1,0xFFFF) != HAL_OK){
		Error_Handler();
	}
	if(HAL_SPI_Transmit(&h_lcd_spi,&low_byte, 1,0xFFFF) != HAL_OK){
		Error_Handler();
    }
}

void LCD_ReadDataMultiple(U16 * pData, int NumItems)
{

}
void LCD_HardwareInit(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
    // set up LCD SPI ports and pins
    // SPI SCK
	GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = LCD_SPI_SCK_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(LCD_SPI_SCK_PORT, &GPIO_InitStruct);

    // SPI MISO
    GPIO_InitStruct.Pin = LCD_SPI_MISO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(LCD_SPI_MISO_PORT, &GPIO_InitStruct);

    // SPI MOSI
    GPIO_InitStruct.Pin = LCD_SPI_MOSI_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(LCD_SPI_MOSI_PORT, &GPIO_InitStruct);

    // SPI NSS
    GPIO_InitStruct.Pin = LCD_SPI_NSS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    //GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(LCD_SPI_NSS_PORT, &GPIO_InitStruct);
   	HAL_GPIO_WritePin(LCD_SPI_NSS_PORT,LCD_SPI_NSS_PIN,GPIO_PIN_SET);

   	// SPI D/C
   	GPIO_InitStruct.Pin = LCD_SPI_DC_PIN;
   	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
   	GPIO_InitStruct.Pull = GPIO_NOPULL;
   	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
   	HAL_GPIO_Init(LCD_SPI_DC_PORT, &GPIO_InitStruct);

   	// LCD Reset
   	GPIO_InitStruct.Pin = LCD_RESET_PIN;
   	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
   	GPIO_InitStruct.Pull = GPIO_NOPULL;
   	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
   	HAL_GPIO_Init(LCD_RESET_PORT, &GPIO_InitStruct);
   	HAL_GPIO_WritePin(LCD_RESET_PORT, LCD_RESET_PIN,GPIO_PIN_SET);
    //toggle reset
    HAL_Delay(150);
    HAL_GPIO_WritePin(LCD_RESET_PORT, LCD_RESET_PIN,GPIO_PIN_SET);
    HAL_Delay(15);
    HAL_GPIO_WritePin(LCD_RESET_PORT, LCD_RESET_PIN,GPIO_PIN_RESET);
    HAL_Delay(30);
    HAL_GPIO_WritePin(LCD_RESET_PORT, LCD_RESET_PIN,GPIO_PIN_SET);
    HAL_Delay(150);


    LCD_SPI_CLK_ENABLE();

    h_lcd_spi.Instance = LCD_SPIx;
    h_lcd_spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    //h_lcd_spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
    h_lcd_spi.Init.CLKPhase = SPI_PHASE_1EDGE;
    h_lcd_spi.Init.CLKPolarity = SPI_POLARITY_LOW; // might need to be high with 2nd edge
    h_lcd_spi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    h_lcd_spi.Init.CRCPolynomial;
    h_lcd_spi.Init.DataSize = SPI_DATASIZE_8BIT;
    h_lcd_spi.Init.Direction = SPI_DIRECTION_2LINES;
    h_lcd_spi.Init.FirstBit = SPI_FIRSTBIT_MSB;
    h_lcd_spi.Init.Mode = SPI_MODE_MASTER;
    h_lcd_spi.Init.NSS = SPI_NSS_HARD_OUTPUT;
    //h_lcd_spi.Init.NSS = SPI_NSS_SOFT;
    h_lcd_spi.Init.TIMode = SPI_TIMODE_DISABLE;
    h_lcd_spi.State = HAL_SPI_STATE_RESET;

    if(HAL_SPI_Init(&h_lcd_spi) != HAL_OK){
    	//Error_Handler();
    }
}

void LCD_ModuleInit()
{
	// adapted from Seed Technology graphics library, by Alber. Miao  Loovee, Viswewara
	// who go the initialization code from a TFT vendor
    LCD_WriteReg(LCD_POWERA);
    LCD_WriteByte(0x39); // these values are straight from the datasheet
    LCD_WriteByte(0x2C);
    LCD_WriteByte(0x00);
    LCD_WriteByte(0x34);// 1.6 volts for  Vcore
    LCD_WriteByte(0x02);// 5.6 volts for DDVHD

    LCD_WriteReg(LCD_POWERB);
    LCD_WriteByte(0x00);
    LCD_WriteByte(0XC1);
    LCD_WriteByte(0X30); // discharge path is enabled

    LCD_WriteReg(LCD_DTCA); // these values are different than the datasheet
    LCD_WriteByte(0x85);
    LCD_WriteByte(0x00);
    LCD_WriteByte(0x78);
//    LCD_WriteData(0x84);
//    LCD_WriteData(0x11);
//    LCD_WriteData(0x7A);


    LCD_WriteReg(LCD_DTCB);
    LCD_WriteByte(0x00);
    //LCD_WriteData(0x66);
    LCD_WriteByte(0x00);

    LCD_WriteReg(LCD_POWER_SEQ);
    LCD_WriteByte(0x64);
    LCD_WriteByte(0x03);
    LCD_WriteByte(0X12);
    LCD_WriteByte(0X81);

    LCD_WriteReg(LCD_PRC);
    LCD_WriteByte(0x20);

    LCD_WriteReg(0xC0);    	//Power control
    LCD_WriteByte(0x23);   	//VRH[5:0] 4.6V

    LCD_WriteReg(0xC1);    	//Power control
    LCD_WriteByte(0x10);   	//SAP[2:0];BT[3:0]

    LCD_WriteReg(0xC5);    	//VCM control
    LCD_WriteByte(0x3e);   	//Contrast 5.85
    LCD_WriteByte(0x28);    //-1.5

    LCD_WriteReg(0xC7);    	//VCM control2
    LCD_WriteByte(0x86);  	 //--
    //LCD_WriteData(0xC0);  	 //--

    LCD_WriteReg(0x36);    	// Memory Access Control
    //LCD_WriteData(0x48);  	//C8
    LCD_WriteByte(0x28); // row/column exchange, I'm using the module in a
    					 // horizontal orientation, with the top left corner
    					 // being x=0 y=0
    //LCD_WriteByte(0x20);
    //LCD_WriteData(0x3C); // row/column exchange, I'm using the module in a

    LCD_WriteReg(0xF6); // Interface control
    LCD_WriteByte(0x01);
    LCD_WriteByte(0x00);
    LCD_WriteByte(0x01);

    LCD_WriteReg(0x3A); // COLMOD: Pixel Format Set
    LCD_WriteByte(0x55);// 16 bits/pixel. RGB and MCU interface format

    LCD_WriteReg(0xB0); // RGB interface control
    LCD_WriteByte(0xC0);

    LCD_WriteReg(0xB1); // Frame Rate Control
    //LCD_WriteData(0x00);
    LCD_WriteByte(0x00);
    //LCD_WriteData(0x18);// 79Hz. datasheet default is 70Hz, 0x1B
    LCD_WriteByte(0x1F);

    LCD_WriteReg(0xB6);    	// Display Function Control
    LCD_WriteByte(0x08);
    LCD_WriteByte(0x82);
    LCD_WriteByte(0x27);

    LCD_WriteReg(0xF2);    	// 3Gamma Function Disable
    LCD_WriteByte(0x00);

    LCD_WriteReg(0x26);    	//Gamma curve selected
    LCD_WriteByte(0x01);

    LCD_WriteReg(0xE0);    	//Set Gamma
    LCD_WriteByte(0x0F);
    LCD_WriteByte(0x31);
    LCD_WriteByte(0x2B);
    LCD_WriteByte(0x0C);
    LCD_WriteByte(0x0E);
    LCD_WriteByte(0x08);
    LCD_WriteByte(0x4E);
    LCD_WriteByte(0xF1);
    LCD_WriteByte(0x37);
    LCD_WriteByte(0x07);
    LCD_WriteByte(0x10);
    LCD_WriteByte(0x03);
    LCD_WriteByte(0x0E);
    LCD_WriteByte(0x09);
    LCD_WriteByte(0x00);

    LCD_WriteReg(0XE1);    	//Set Gamma
    LCD_WriteByte(0x00);
    LCD_WriteByte(0x0E);
    LCD_WriteByte(0x14);
    LCD_WriteByte(0x03);
    LCD_WriteByte(0x11);
    LCD_WriteByte(0x07);
    LCD_WriteByte(0x31);
    LCD_WriteByte(0xC1);
    LCD_WriteByte(0x48);
    LCD_WriteByte(0x08);
    LCD_WriteByte(0x0F);
    LCD_WriteByte(0x0C);
    LCD_WriteByte(0x31);
    LCD_WriteByte(0x36);
    LCD_WriteByte(0x0F);

    LCD_WriteReg(0x11);    	//Exit Sleep
    HAL_Delay(120);

    LCD_WriteReg(0x34);
    LCD_WriteByte(0x01);

    LCD_WriteReg(0x29);    //Display on
    LCD_WriteReg(0x2c);
//-------------------------------
	LCD_WriteReg(LCD_COLUMN_ADDR);
	LCD_DC_HIGH();
	LCD_CS_LOW();
	LCD_WriteByte(0);
	LCD_WriteByte(0);
	//LCD_WriteWord(0);
	//LCD_WriteWord(319);
	LCD_WriteByte(319>>8);
	LCD_WriteByte((uint8_t)319);
	LCD_CS_HIGH();

	LCD_WriteReg(LCD_PAGE_ADDR);
	LCD_DC_HIGH();
	LCD_CS_LOW();
	//LCD_WriteWord(0);
	LCD_WriteByte(0);
	LCD_WriteByte(0);
	//LCD_WriteWord(239);
	LCD_WriteByte(239>>8);
	LCD_WriteByte((uint8_t)239);
	LCD_CS_HIGH();

	LCD_WriteReg(LCD_GRAM);

//    LCD_FillScreen2(0x00FF);
//    asm("nop");
//    HAL_Delay(500);
}

//uint16_t LCD_ReadPixel()
//{
//	uint8_t vals[3];
//	uint32_t readvalue = 0;
//	uint16_t red,green,blue,color;
//	uint8_t byte = LCD_RAMRD;
//	LCD_DC_LOW();
//	LCD_CS_LOW();
//	if(HAL_SPI_Transmit(&h_lcd_spi,&byte, 1,0xFFFF) != HAL_OK){
//		Error_Handler();
//    }
//
//	LCD_DC_HIGH();
//	// dummy byte
//	if(HAL_SPI_Receive(&h_lcd_spi,(uint8_t*)vals, 1,0xFFFF) != HAL_OK){
//        Error_Handler();
//    }
//    if(HAL_SPI_Receive(&h_lcd_spi,(uint8_t*)vals, 3,0xFFFF) != HAL_OK){
//      Error_Handler();
//    }
//	LCD_CS_HIGH();
//	red = vals[0];
//	green = vals[1];
//	blue = vals[2];
//	color = ((red&0xF8)<<8)|((green&0xFC)<<3)|(blue>>3);
//	return color;
//}
void LCD_SetColumn(uint16_t col_left, uint16_t col_right)
{
	LCD_WriteReg(LCD_COLUMN_ADDR);
	LCD_DC_HIGH();
	LCD_CS_LOW();
	LCD_WriteWord(col_left);
	LCD_WriteWord(col_right);
	LCD_CS_HIGH();
}

void LCD_SetRow(uint16_t row_top, uint16_t row_bottom)
{
	LCD_WriteReg(LCD_PAGE_ADDR);
	LCD_DC_HIGH();
	LCD_CS_LOW();
	LCD_WriteWord(row_top);
	LCD_WriteWord(row_bottom);
	LCD_CS_HIGH();
}

void LCD_Set8Bit()
{
  	h_lcd_spi.Init.DataSize = SPI_DATASIZE_8BIT;
    if(HAL_SPI_Init(&h_lcd_spi) != HAL_OK){
    	Error_Handler();
    }
}

void LCD_Set16Bit()
{
  	h_lcd_spi.Init.DataSize = SPI_DATASIZE_16BIT;
    if(HAL_SPI_Init(&h_lcd_spi) != HAL_OK){
    	Error_Handler();
    }
}



void LCD_FillScreen2(uint16_t color)
{
	uint32_t varr = 0xFF;
//	// weak-ass sychronization attempt, at least it makes the tearing lines stable
//	while(varr>1){
//      varr = LCD_ReadData(0x45,2);
//      //varr >>= 16;
//      asm("nop");
//	}
	uint8_t burst = 160;
	//uint8_t burst = 2;
	uint32_t color32[burst/2];
	for(uint8_t i=0;i<(burst/2);i++){
      color32[i] = color;
      color32[i] <<=16;
      color32[i] |= color;
	}
	LCD_WriteReg(LCD_COLUMN_ADDR);
	LCD_DC_HIGH();
	LCD_CS_LOW();
	LCD_WriteByte(0);
	LCD_WriteByte(0);
	//LCD_WriteWord(0);
	//LCD_WriteWord(319);
	LCD_WriteByte(319>>8);
	LCD_WriteByte((uint8_t)319);
	LCD_CS_HIGH();

	LCD_WriteReg(LCD_PAGE_ADDR);
	LCD_DC_HIGH();
	LCD_CS_LOW();
	//LCD_WriteWord(0);
	LCD_WriteByte(0);
	LCD_WriteByte(0);
	//LCD_WriteWord(239);
	LCD_WriteByte(239>>8);
	LCD_WriteByte((uint8_t)239);
	LCD_CS_HIGH();

	LCD_WriteReg(LCD_GRAM);

  	h_lcd_spi.Init.DataSize = SPI_DATASIZE_16BIT;
    if(HAL_SPI_Init(&h_lcd_spi) != HAL_OK){
    }
    LCD_DC_HIGH();
    LCD_CS_LOW();
  	uint8_t data = 1;
  	int8_t incr = 1;
    for(uint32_t i=0; i<320*240/burst; i++)
    {
    //	LCD_WriteWord(color);
    	//transmit function can transfer 80 bits at a time, 5 words (5*16)
    	if(HAL_SPI_Transmit(&h_lcd_spi,(uint8_t*)&color32[0], burst,0xFFFF) != HAL_OK){
          Error_Handler();
        }
    	//LCD_WriteByte(data);
    	//LCD_WriteByte(data);
//    	data += incr;
//    	if(data == limit){
//    		incr = -1;
//    	}
//    	if(data == 0){
//    		incr = 1;
//    	}
    }
    LCD_CS_HIGH();
	h_lcd_spi.Init.DataSize = SPI_DATASIZE_8BIT;
    if(HAL_SPI_Init(&h_lcd_spi) != HAL_OK){

    }

}
