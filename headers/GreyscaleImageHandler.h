#ifndef GREYSCALE_IMAGE_HANDLER_H
#define GREYSCALE_IMAGE_HANDLER_H

#include "ImageHandler.h"


/*===============================================*/
/**             GreyScale Image Handlers           **/
/*===============================================*/


// R/W methods
GreyScale_ImageHandler_t GreyScale_Image_Handler_Read_Image(char* ImageName, char* ImagePath);
void GreyScale_Image_Handler_Write_Image(GreyScale_ImageHandler_t anImage, char* ImageName, char* ImagePath);

// Conversion
GreyScale_ImageHandler_t Greyscale_Image_Handler_Convt_RGB_to_GreyScale(RGB_ImageHandler_t rgb_image);

// Rotating Images
GreyScale_ImageHandler_t Greyscale_Image_Handler_Rotate_Image_Right(GreyScale_ImageHandler_t img);
GreyScale_ImageHandler_t Greyscale_Image_Handler_Rotate_Image_Left(GreyScale_ImageHandler_t img);
GreyScale_ImageHandler_t Greyscale_Image_Handler_Rotate_Image_180(GreyScale_ImageHandler_t img);

// Negative Transformation
GreyScale_ImageHandler_t  Greyscale_Image_Handler_Negative_Transform(GreyScale_ImageHandler_t img);

// Box blur filter function
GreyScale_ImageHandler_t Greyscale_Image_Handler_Box_Blur(GreyScale_ImageHandler_t img);
GreyScale_ImageHandler_t Greyscale_Image_Handler_Gaussian_Blur(GreyScale_ImageHandler_t img);

// Convolution based Filters
GreyScale_ImageHandler_t Greyscale_Image_Handler_Laplacian_Convolution(GreyScale_ImageHandler_t img);
GreyScale_ImageHandler_t Greyscale_Image_Handler_Edge_Detect_Single(GreyScale_ImageHandler_t img, Mask_Types_t edge_flow);
GreyScale_ImageHandler_t Greyscale_Image_Handler_Edge_Detect_Complete(GreyScale_ImageHandler_t img);

// Brighten and Dim
GreyScale_ImageHandler_t Greyscale_Brighten_Image_Saturation(GreyScale_ImageHandler_t image, uint8_t offset);
GreyScale_ImageHandler_t Greyscale_Darken_Image_Saturation(GreyScale_ImageHandler_t image, uint8_t offset);



// Computing Histogram
float* Image_Handler_Compute_Histogram(GreyScale_ImageHandler_t img);
float* Image_Handler_Compute_Equalized_Histogram(float* hist, int size);
GreyScale_ImageHandler_t Image_Handler_Equalize_Image_Pixels(GreyScale_ImageHandler_Ptr img_p);

#endif  //GREYSCALE_IMAGE_HANDLER_H