#include"Display.h"

static uint8_t* SREC_MESSAGE[] = {
    "True: Line is true",
    "False: Error Format",
    "False: Error Byte Count",
    "False: Error Check Sum",
    "False: File is not exist",
    "File is exist",
    "File starts",
    "File ends",
    "False: File is false"
};

void Display(uint32_t line_number, SREC_Status_Type status){
    printf("%4d-%s\n", line_number, SREC_MESSAGE[status]);
}

// format: line number-address-data-result details
void DisplayLine(uint32_t line_number, SREC_Status_Type status, SREC_Type srec_line, uint8_t data_bytes){
    uint8_t idx;
    // line number + address
    printf("%04d-%08x-", line_number, srec_line.Address);
    // data
    // for(idx = 0; idx < data_bytes; idx++){
    //     printf("%02x", srec_line.Data[idx]);
    // }
    // result
    printf("-%s\n", SREC_MESSAGE[status]);
}