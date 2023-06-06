#ifndef MASK_H
#define MASK_H

#include <stdint.h>




typedef enum {
    Laplacian,
    Edge_Vertical,
    Edge_Horizontal,
    Edge_Left_Diag,
    Edge_Right_Diag
    /* {#TODO} other mask types */
} Mask_Types_t;


typedef struct{
    int Rows;
    int Cols;
    float* Data;
} Mask_t;


Mask_t Mask_Init(Mask_Types_t mask_type);
void Mask_Convolve(uint8_t* in_buffer, int rows, int cols, Mask_t mask, uint8_t* out_buffer);

#endif