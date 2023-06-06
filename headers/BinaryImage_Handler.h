#ifndef BINARY_IMAGE_HANDLER_H
#define BINARY_IMAGE_HANDLER_H

#include "ImageHandler.h"


Binary_ImageHandler_t Binary_Image_Handler_Read_Image_From_Greyscale(char* ImageName, char* ImagePath, int thres);
Binary_ImageHandler_t Binary_Image_Handler_Read_Image_From_RGB(char* ImageName, char* ImagePath, int thres);
void Binary_Image_Handler_Write_Image(Binary_ImageHandler_t anImage, char* ImageName, char* ImagePath);
Binary_ImageHandler_t Image_Convt_RGB_to_Binary(RGB_ImageHandler_t rgb_image, int thres);

#endif          //  BINARY_IMAGE_HANDLER_H
