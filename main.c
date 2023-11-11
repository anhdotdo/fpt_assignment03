#include<stdio.h>
#include<stdint.h>
#include"SRECParse.h"

/*
cover all over cases: data is not enough, ...
*/

uint8_t main(void){
    SREC_Status_Type status;
    uint8_t buffer[3];

    // SREC_OpenFile("test.srec");
    // // FILE* ptr = getFilePtr();
    // // printf("%c\n", fgetc(ptr));
    // // printf("%c\n", fgetc(ptr));

    SREC_OpenFile("test.srec");
    // =>get S0 of srec file type to start
    status = SREC_StartSrecFile();

    if(status == SREC_FILE_EXIST){

        if(status == SREC_FILE_START){
            do{
                status = SREC_ReadLine();
                if(SREC_isEOF()){
                    status = SREC_FILE_FALSE;           // 
                    break;
                }
                // display result
                
            }while(status != SREC_FILE_END);
        }
    }

    

    // =>get S1/S2/S3 => show result: Line numer-Address-Data(Hex format)-Result(True/False Details)
    // Or get S7/S8/S9 => end srec file


    // status = SREC_ReadLine();
    // printf("%d\n", status);

    SREC_CloseFile();    

    return 0;
}




// function to calcular and check sum