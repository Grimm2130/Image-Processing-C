#ifndef MASK_H
#define MASK_H

#include <stdint.h>


#define     MASK_DATA_LAPLACIAN_ROWS             5
#define     MASK_DATA_LAPLACIAN_COLS             5

#define     MASK_DATA_EDGE_DETECT_ROWS           3
#define     MASK_DATA_EDGE_DETECT_COLS           3

#define     MAX_PIXEL_VAL               255
#define     MIN_PIXEL_VAL               0


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


const float Rgb_to_Bin_conv_floats[3] = {0.3, 0.59, 0.11};


const float Box_Blurr_Kernel[3][3]= {   {1.0/9.0, 1.0/9.0, 1.0/9.0},
                                        {1.0/9.0, 1.0/9.0, 1.0/9.0},
                                        {1.0/9.0, 1.0/9.0, 1.0/9.0}    };

const float Gaussian_Blurr_Kernel[3][3]= {  {1.0/16.0, 1.0/8.0, 1.0/16.0},
                                            {1.0/8.0,  1.0/4.0,  1.0/8.0},
                                            {1.0/16.0, 1.0/8.0, 1.0/16.0}  };

const float Sharpen_Kernel[3][3]= { { 0,  -1,   0},
                                    {-1,   5,  -1},
                                    { 0,  -1,   0}  };

const float Sepia_kernel[3][3] = {  {0.272, 0.534, 0.131},
                                    {0.349, 0.686, 0.168},
                                    {0.393, 0.769, 0.189}   };


const float Laplacian_Mask[MASK_DATA_LAPLACIAN_ROWS][MASK_DATA_LAPLACIAN_COLS] =
                        {   {-1,-1,-1,-1,-1} ,
                            {-1,-1,-1,-1,-1} ,
                            {-1,-1,25.0,-1,-1} ,
                            {-1,-1,-1,-1,-1} ,
                            {-1,-1,-1,-1,-1}    };


const float Edge_Detect_Mask_Gen[MASK_DATA_EDGE_DETECT_ROWS][MASK_DATA_EDGE_DETECT_COLS] =
                        {   {-1,-1,-1},
                            {-1,-1,-1},
                            {-1,-1,-1}
                        };


#endif