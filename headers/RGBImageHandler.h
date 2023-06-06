#ifndef RGB_IMAGE_HANDLER_H
#define RGB_IMAGE_HANDLER_H

#include "ImageHandler.h"



// R/W methods
RGB_ImageHandler_t RGB_Image_Handler_Read_Image(char* ImageName, char* ImagePath);
void RGB_Image_Handler_Write_Image(RGB_ImageHandler_t anImage, char* ImageName, char* ImagePath);

// Blurring Functions
RGB_ImageHandler_t RGB_Image_Handler_Box_Blur(RGB_ImageHandler_t img);
RGB_ImageHandler_t RGB_Image_Handler_Gaussian_Blur(RGB_ImageHandler_t img);

// Special filters
RGB_ImageHandler_t RGB_Image_Handler_Sepia_Filter(RGB_ImageHandler_t img);


#endif      // RGB_IMAGE_HANDLER_H