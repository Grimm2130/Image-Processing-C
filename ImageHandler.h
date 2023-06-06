#ifndef IMAGE_HANDLER_H
#define IMAGE_HANDLER_H

#define IMAGE_HEADER_SIZE           54
#define IMAGE_COLOR_TABLE_SIZE      1024
#define HEADER_IMAGE_WIDTH_OFFSET   18
#define HEADER_IMAGE_HEIGHT_OFFSET  22
#define HEADER_BIT_DEPTH_OFFSET     28
#define RGB_CHANNEL_SIZE            3

#define MAX_PIXEL_VAL               255
#define MIN_PIXEL_VAL               10

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "Mask.h"

/*===============================================================*/
/*===================== Channel Enumeration =====================*/
/*===============================================================*/
typedef enum {
    RED,
    GREEN,
    BLUE
} RGB_channels;


/*===============================================================*/
/*===================== Image Handler Types =====================*/
/*===============================================================*/
typedef struct{
    int image_thres;
    int ImageWidth;
    int ImageHeight;
    int BitDepth;
    int FromRGB;
    uint8_t ImageHeader[IMAGE_HEADER_SIZE];
    uint8_t ColorTable[IMAGE_COLOR_TABLE_SIZE];
    uint8_t * ImageBuffer;
} Binary_ImageHandler_t;
typedef Binary_ImageHandler_t* Binary_ImageHandler_Ptr;

typedef struct{
    int ImageWidth;
    int ImageHeight;
    int BitDepth;
    int FromRGB;
    uint8_t ImageHeader[IMAGE_HEADER_SIZE];
    uint8_t ColorTable[IMAGE_COLOR_TABLE_SIZE];
    uint8_t* ImageBuffer;
} GreyScale_ImageHandler_t;
typedef GreyScale_ImageHandler_t* GreyScale_ImageHandler_Ptr;


typedef struct{
    int ImageWidth;
    int ImageHeight;
    int BitDepth;
    uint8_t ImageHeader[IMAGE_HEADER_SIZE];
    uint8_t ColorTable[IMAGE_COLOR_TABLE_SIZE];
    uint8_t ** ImageBuffer;
} RGB_ImageHandler_t;
typedef RGB_ImageHandler_t* RGB_ImageHandler_Ptr;



/*===============================================================*/
/*===================== Inline Helper Functions =====================*/
/*===============================================================*/
inline static char* Set_File_Path(char* a, char* b){
    int size = strlen(a)+strlen(b)+2;
    char* ret = (char*) calloc(size, sizeof(char));
    int idx = 0;
    while(*a != '\0'){
        ret[idx++] = *a++;
    }
    ret[idx++] = '/';
    while(*b != '\0'){
        ret[idx++] = *b++;
    }
    ret[idx] = '\0';
    return ret;
}


inline static uint8_t** Set_RGB_Buffer_Mem(const int size){
    uint8_t **ret = (uint8_t**) calloc(RGB_CHANNEL_SIZE, sizeof(uint8_t*));
    for(int i = 0; i < RGB_CHANNEL_SIZE; i++){
        ret[i] = (uint8_t*) calloc(size, sizeof(uint8_t));
    }
    return ret;
}


inline static uint8_t* Init_Buffer(const int rows, const int cols){
    uint8_t* buff = (uint8_t*)calloc(rows * cols, sizeof(u_int8_t));
    return buff;
}

inline static void Buffer_And(uint8_t* mask, uint8_t* target, const int rows, const int cols){
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            target[j + (i * cols)] |= mask[j + (i * cols)];
        }
    }
}


/*===============================================*/
/**              Binary Image Handlers          **/
/*===============================================*/




/*===============================================*/
/**           Image Conversion Methods          **/
/*===============================================*/






/*===============================================*/
/**     Image Arithmetic Operation Methods      **/
/*===============================================*/











#endif      //IMAGE_HANDLER_H