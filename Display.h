#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include"MyLib.h"

void Display(uint32_t line_number, SREC_Status_Type status);
void DisplayLine(uint32_t line_number, SREC_Status_Type status, SREC_Type srec_line, uint8_t data_bytes);

#endif