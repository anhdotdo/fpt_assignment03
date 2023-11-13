#include<stdio.h>
#include<stdint.h>
#include"SRECParse.h"
#include"Display.h"

uint8_t main(void){
    SREC_Status_Type status = SREC_FILE_NOT_EXIST;
    uint32_t line_number = 0u;
    // uint8_t idx;
    status = SREC_OpenFile("blinkLed.srec");

    // => read file
    if(status == SREC_FILE_EXIST){
        status = SREC_StartSrecFile();

        if(status == SREC_FILE_START){
            line_number = 1;
            do{
                status = SREC_ReadLine();
                line_number++;
                DisplayLine(line_number, status, *SREC_GetVariable(), SREC_GetDataBytes());
            }while(status != SREC_FILE_END);
        }
    }
    // printf("status: %d\n", status);

    SREC_CloseFile();    

    return 0;
}