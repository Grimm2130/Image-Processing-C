#ifndef MASK_H
#define MASK_H

#include <stdint.h>


#define     MASK_DATA_LAPLACIAN_ROWS             5
#define     MASK_DATA_LAPLACIAN_COLS             5

#define     MASK_DATA_EDGE_DETECT_ROWS           3
#define     MASK_DATA_EDGE_DETECT_COLS           3

#define MAX_PIXEL_VAL               255
#define MIN_PIXEL_VAL               10

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


const float Rgb_to_Bin_conv_floats[3];
const float Box_Blurr_Kernel[3][3];
const float Gaussian_Blurr_Kernel[3][3];
const float Sharpen_Kernel[3][3];
const float Sepia_kernel[3][3];


const float Laplacian_Mask[MASK_DATA_LAPLACIAN_ROWS][MASK_DATA_LAPLACIAN_COLS];


const float Edge_Detect_Mask_Gen[MASK_DATA_EDGE_DETECT_ROWS][MASK_DATA_EDGE_DETECT_COLS];

#endif