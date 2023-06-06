#include "../headers/Utils.h"
#include <stdlib.h>
#include <stdio.h>


void CopyFloatArrayToFile(float* arr, int size, char* fileName){
    FILE* fp = fopen(fileName, "w+");
    if(!fp){
        printf("File failed to open\n");
        exit(1);
    }

    // fprintf(fp, "%d\n", size);

    for(int i = 0; i < size; i++){
        fprintf(fp, "%f\n", arr[i]);
    }

    fclose(fp);
}


void NormalizeFloatArr(float** arr, int size, float trunc_val){
    for(int i = 0; i < size; i++){
       (*arr)[i] /= trunc_val;
    }
}