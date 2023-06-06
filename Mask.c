#include "Mask.h"
#include <stdlib.h>
#include <stdio.h>


/*==============================================================*/
/*======================== Mask Filters ========================*/
/*==============================================================*/




Mask_t Mask_Init(Mask_Types_t mask_type){
    Mask_t mask;
    switch(mask_type){
        case Laplacian:
            // Set numbuer of rows & cols
            mask.Rows = MASK_DATA_LAPLACIAN_ROWS;
            mask.Cols = MASK_DATA_LAPLACIAN_COLS;
            mask.Data = (float*) calloc((MASK_DATA_LAPLACIAN_COLS * MASK_DATA_LAPLACIAN_ROWS), sizeof(float));
            for(int i = 0; i < MASK_DATA_LAPLACIAN_ROWS; i++){
                for (int j = 0; j < MASK_DATA_LAPLACIAN_COLS; j++){
                    mask.Data[j + (i * MASK_DATA_LAPLACIAN_COLS)] = Laplacian_Mask[i][j];
                }
            }
            break;

        case Edge_Vertical:
            // Set numbuer of rows & cols
            mask.Rows = MASK_DATA_EDGE_DETECT_ROWS;
            mask.Cols = MASK_DATA_EDGE_DETECT_COLS;
            mask.Data = (float*) calloc((MASK_DATA_EDGE_DETECT_ROWS * MASK_DATA_EDGE_DETECT_COLS), sizeof(float));
            for(int i = 0; i < MASK_DATA_EDGE_DETECT_ROWS; i++){
                for (int j = 0; j < MASK_DATA_EDGE_DETECT_COLS; j++){
                    mask.Data[j + (i * MASK_DATA_EDGE_DETECT_COLS)] = Edge_Detect_Mask_Gen[i][j];
                }
            }
            mask.Data[1 + (0 * MASK_DATA_EDGE_DETECT_COLS)] = 2;
            mask.Data[1 + (1 * MASK_DATA_EDGE_DETECT_COLS)] = 2;
            mask.Data[1 + (2 * MASK_DATA_EDGE_DETECT_COLS)] = 2;
            break;

        case Edge_Horizontal:
            // Set numbuer of rows & cols
            mask.Rows = MASK_DATA_EDGE_DETECT_ROWS;
            mask.Cols = MASK_DATA_EDGE_DETECT_COLS;
            mask.Data = (float*) calloc((MASK_DATA_EDGE_DETECT_ROWS * MASK_DATA_EDGE_DETECT_COLS), sizeof(float));
            for(int i = 0; i < MASK_DATA_EDGE_DETECT_ROWS; i++){
                for (int j = 0; j < MASK_DATA_EDGE_DETECT_COLS; j++){
                    mask.Data[j + (i * MASK_DATA_EDGE_DETECT_COLS)] = Edge_Detect_Mask_Gen[i][j];
                }
            }
            mask.Data[0 + (1 * MASK_DATA_EDGE_DETECT_COLS)] = 2;
            mask.Data[1 + (1 * MASK_DATA_EDGE_DETECT_COLS)] = 2;
            mask.Data[2 + (1 * MASK_DATA_EDGE_DETECT_COLS)] = 2;
            break;

        case Edge_Left_Diag:
            // Set numbuer of rows & cols
            mask.Rows = MASK_DATA_EDGE_DETECT_ROWS;
            mask.Cols = MASK_DATA_EDGE_DETECT_COLS;
            mask.Data = (float*) calloc((MASK_DATA_EDGE_DETECT_ROWS * MASK_DATA_EDGE_DETECT_COLS), sizeof(float));
            for(int i = 0; i < MASK_DATA_EDGE_DETECT_ROWS; i++){
                for (int j = 0; j < MASK_DATA_EDGE_DETECT_COLS; j++){
                    mask.Data[j + (i * MASK_DATA_EDGE_DETECT_COLS)] = Edge_Detect_Mask_Gen[i][j];
                }
            }
            mask.Data[0] = 2;
            mask.Data[1 + (1 * MASK_DATA_EDGE_DETECT_COLS)] = 2;
            mask.Data[2 + (2 * MASK_DATA_EDGE_DETECT_COLS)] = 2;
            break;

        case Edge_Right_Diag:
            // Set numbuer of rows & cols
            mask.Rows = MASK_DATA_EDGE_DETECT_ROWS;
            mask.Cols = MASK_DATA_EDGE_DETECT_COLS;
            mask.Data = (float*) calloc((MASK_DATA_EDGE_DETECT_ROWS * MASK_DATA_EDGE_DETECT_COLS), sizeof(float));
            for(int i = 0; i < MASK_DATA_EDGE_DETECT_ROWS; i++){
                for (int j = 0; j < MASK_DATA_EDGE_DETECT_COLS; j++){
                    mask.Data[j + (i * MASK_DATA_EDGE_DETECT_COLS)] = Edge_Detect_Mask_Gen[i][j];
                }
            }
            mask.Data[2] = 2;
            mask.Data[1 + (1 * MASK_DATA_EDGE_DETECT_COLS)] = 2;
            mask.Data[0 + (2 * MASK_DATA_EDGE_DETECT_COLS)] = 2;
            break;
    }
    return mask;
}



void Mask_Convolve(uint8_t* in_buffer, int rows, int cols, Mask_t mask, uint8_t* out_buffer){
    int x_idx = 0,
        y_idx = 0;
    float sum = 0;
    int temp = 0;
    // Loop over the image values
    for (int x = 0; x < rows; x++){
        for (int y = 0; y < cols; y++){
            sum = 0;
            // Loop over the mask
            for (int i = 0; i < mask.Rows; i++){
                for (int j = 0; j < mask.Cols; j++){
                    x_idx = x - i;
                    y_idx = y - j;
                    // Check if at valid offset
                    if( ( x_idx >= 0) && ( y_idx >= 0) ){
                        sum += mask.Data[j + (i * mask.Cols)] * (float) in_buffer[y_idx + (x_idx * cols)];
                    }
                }
            }
            // Set the value in the output buffer
            temp = (int) sum;
            if(temp > MAX_PIXEL_VAL)
                temp = MAX_PIXEL_VAL;
            if(temp < MIN_PIXEL_VAL)
                temp = MIN_PIXEL_VAL;

            out_buffer[y + (x * cols)] = (uint8_t) temp;
        }
    }
}
