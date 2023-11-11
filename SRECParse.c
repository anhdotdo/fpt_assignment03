#include"SRECParse.h"

static FILE* fPtr = NULL;
static SREC_Type srec_line;
static uint8_t srec_address_bytes;
static uint8_t srec_data_bytes;

// function to convert 1 character -> decimal
/*
input: hex digit (1, 2, ..., A, F)
output: decimal number
*/
uint8_t Char2Hex(uint8_t c){
    uint8_t result;
    if(c >= '0' && c <= '9'){
        result = c - '0';
    }
    else if(c >= 'A' && c <= 'F'){
        result = c - 'A' + 0x0A;
    }
    else{
        result = HEX_INVALID;
    }
    return result;
}

// function to convert 1 string -> decimal
SREC_Status_Type Str2Hex(uint8_t *pStr, uint8_t len, uint32_t *result){
    uint8_t idx;
    uint8_t c;
    for(idx = 0; idx < len; idx++){
        c = Char2Hex(pStr[idx]);
        if(HEX_INVALID == c){
            return SREC_ERROR_FORMAT;
        }
        *result += c * pow(16, len-1-idx);
    }
    return SREC_LINE_TRUE;
}

// function to get file pointer
FILE* getFilePtr(void){
    return fPtr;
}

// function to open file
SREC_Status_Type SREC_OpenFile(const uint8_t* fileName){
    fPtr = fopen(fileName, "r");
    if(fPtr == NULL){
        return SREC_FILE_NOT_EXIST;
    }
    return SREC_FILE_EXIST;
}

// function to close file
void SREC_CloseFile(void){
    free(fPtr);
    fclose(fPtr);
}

// function to init an SREC_Type variable
void SREC_InitLine(SREC_Type *srec_line){
    srec_line->ByteCount = 0x00u;
    srec_line->Address = 0x00u;
    srec_line->Data = NULL;
    srec_line->Checksum = 0x00u;
}

// function to get srec type
SREC_Status_Type SREC_StartSrecFile(void){
    SREC_Status_Type status = SREC_FILE_FALSE;
    uint8_t buffer[3];
    fgets(buffer, 3, fPtr);
    if(buffer[0] == 'S' && buffer[1] == '0'){
        status = SREC_FILE_START;
    }
    while(fgetc(fPtr) != '\0' && feof(fPtr) == 0);
    return status;
}

// function to read one line
SREC_Status_Type SREC_ReadLine(void){
    SREC_Status_Type status = SREC_LINE_TRUE;
    uint8_t buffer[9];
    uint8_t idx;

    SREC_InitLine(&srec_line);

    if(fPtr != NULL){
        // buffer[3] read srec type
        fgets(srec_line.SrecType, 3, fPtr);
        if(srec_line.SrecType[0] == 'S' && srec_line.SrecType[1] >= '0' && srec_line.SrecType[1] <= '9' && srec_line.SrecType[1] != '4'){
            if(srec_line.SrecType[1] == '1' || srec_line.SrecType[1] == '2' || srec_line.SrecType[1] == '3'){
                // srec line have address, data
                // buf[3] read byte count
                fgets(buffer, 3, fPtr);
                status = Str2Hex(buffer, 2, (uint32_t*)&srec_line.ByteCount);

                if(status == SREC_LINE_TRUE){
                    // buf[9] read address, just for S1/S2/S3
                    srec_address_bytes = Char2Hex(srec_line.SrecType[1]) + 1;
                    fgets(buffer, srec_address_bytes * 2 + 1, fPtr);
                    status = Str2Hex(buffer, srec_address_bytes * 2, &srec_line.Address);

                    if(status == SREC_LINE_TRUE){
                        // uint8_t *data read data, using dynamic allocation
                        srec_data_bytes = srec_line.ByteCount - srec_address_bytes - 1;
                        free(srec_line.Data);
                        srec_line.Data = (uint8_t*)malloc(srec_data_bytes);
                        for(idx = 0; idx < srec_data_bytes; idx++){
                            fgets(buffer, 3, fPtr);
                            status = Str2Hex(buffer, 2, (uint32_t*)&srec_line.Data[idx]);
                            if(status != SREC_LINE_TRUE){
                                break;
                            }
                        }
                        
                        if(status == SREC_LINE_TRUE){
                            // buf[3] read checksum
                            fgets(buffer, 3, fPtr);
                            status = Str2Hex(buffer, 2, (uint32_t*)&srec_line.Checksum);
                            // printf("Checksum: %x\n", srec_line.Checksum);

                            // =>calcular & check sum
                            status = SREC_CheckSum();
                            // =>read out of one line
                            if(status == SREC_LINE_TRUE){
                                if(fgetc(fPtr) != '\n' && feof(fPtr) == 0){
                                    status = SREC_ERROR_BYTECOUNT;      // ...
                                }
                                while(fgetc(fPtr) != '\n' && feof(fPtr) == 0);
                            }
                        }
                    }
                }
            }else if(srec_line.SrecType[1] == '7' || srec_line.SrecType[1] == '8' || srec_line.SrecType[1] == '9'){
                // end srec file
                status = SREC_FILE_END;
            }else{
                // '5', '6'
                while(fgetc(fPtr) != '\0' && feof(fPtr) == 0);              // read till end of line or file
            }
        }else{
            status = SREC_ERROR_FORMAT;
        }
    }else{
        status = SREC_FILE_NOT_EXIST;
    }
    return status;
}

// function to check sum
SREC_Status_Type SREC_CheckSum(void){
    SREC_Status_Type status = SREC_ERROR_CHECKSUM;
    uint8_t idx;
    uint8_t sum = 0u;
    uint8_t* ptr = (uint8_t*)&srec_line.Address;

    sum += srec_line.ByteCount;

    for(idx = 0; idx < srec_address_bytes; idx++){
        sum += ptr[idx];
    }

    for(idx = 0; idx < srec_data_bytes; idx++){
        sum += srec_line.Data[idx];
    }

    sum += srec_line.Checksum;

    if(0xFFu == (sum & 0xFFu)){
        status = SREC_LINE_TRUE;
    }

    return status;
}

Bool_Type SREC_isEOF(){
    return feof(fPtr);
}