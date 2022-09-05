#include "i2c.h"
#include "font.h"

void OLED_SEND_CMD(uint16_t cmd)
{
    I2C_START_Condition();
    I2C_CMD_Transmission(0x78);
    I2C_CMD_Transmission(0x00);
    I2C_CMD_Transmission(cmd);
    I2C_STOP_Condition();
}

void OLED_WRITE_CMD(uint16_t cmd)
{
    I2C_START_Condition();
    I2C_CMD_Transmission(0x78);
    I2C_CMD_Transmission(0x40);
    I2C_CMD_Transmission(cmd);
    I2C_STOP_Condition();
}

void OLED_Initialise()
{
    OLED_SEND_CMD(0xAE);
    OLED_SEND_CMD(0x00);
    OLED_SEND_CMD(0x10);
    OLED_SEND_CMD(0x40);
    OLED_SEND_CMD(0xB0);
    OLED_SEND_CMD(0x81);
    OLED_SEND_CMD(0xFF);
    OLED_SEND_CMD(0xA1);
    OLED_SEND_CMD(0xA6);
    OLED_SEND_CMD(0xA8);
    OLED_SEND_CMD(0x3F);
    OLED_SEND_CMD(0xC8);
    OLED_SEND_CMD(0xD3);
    OLED_SEND_CMD(0x00);
    OLED_SEND_CMD(0xD5);
    OLED_SEND_CMD(0x80);
    OLED_SEND_CMD(0xD9);
    OLED_SEND_CMD(0xF1);
    OLED_SEND_CMD(0xDA);
    OLED_SEND_CMD(0x12);
    OLED_SEND_CMD(0xDB);
    OLED_SEND_CMD(0x30);
    OLED_SEND_CMD(0x8D);
    OLED_SEND_CMD(0x14);
    OLED_SEND_CMD(0xAF);
    OLED_SEND_CMD(0x2e);
}

void OLED_Clear()
{
    size_t i, j;

    for(i = 0; i < 8; i++)
    {
        OLED_SEND_CMD(0xB0+i);
        OLED_SEND_CMD(0x00);         
        OLED_SEND_CMD(0x10);

        for(j = 0; j < 128; j++){
            OLED_WRITE_CMD(0);
        }
    }
}

void OLED_SetCursor(uint8_t Y, uint8_t X)
{
	X+=2;
	OLED_SEND_CMD(0xB0 | Y);					
	OLED_SEND_CMD(0x10 | ((X & 0xF0) >> 4));	
	OLED_SEND_CMD(0x00 | (X & 0x0F));			
}

void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		
	for (i = 0; i < 8; i++)
	{
		OLED_WRITE_CMD(OLED_F8x16[Char - ' '][i]);			
	}
	OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	
	for (i = 0; i < 8; i++)
	{
		OLED_WRITE_CMD(OLED_F8x16[Char - ' '][i + 8]);		
	}
}

void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		OLED_ShowChar(Line, Column + i, String[i]);
	}
}


