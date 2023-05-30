#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ImageHandler.h"
#include "errno.h"


char* grey_image = "TestImages/cameraman.bmp";
char* grey_image_copy = "TestImages/cameraman_copy.bmp";

char* rgb_image = "TestImages/lena_color.bmp";
char* rgb_image_copy = "TestImages/lena_color_copy.bmp";


char* grey_to_bin_imageCopy = "TestImages/cameraman_bin_copy.bmp";
char* rgb_to_bin_imageCopy = "TestImages/lena_binary_copy.bmp";
char* rgb_to_bin_imageCopy_2 = "TestImages/lena_binary2_copy.bmp";


char* rgb_to_grey = "TestImages/lena_grey_copy.bmp";



static void testGreyImage_R_W(){

    GreyScale_ImageHandler_t grey = GreyScale_Image_Handler_Read_Image(grey_image, NULL);

    if(grey.FromRGB == 'Y'){
        printf("Copied from RGB image\n");
    }
    printf("\nGrey Image Features:\nImage Height: %d\nImage Width: %d\nImage Bit Depth: %d\n", grey.ImageHeight, grey.ImageWidth, grey.BitDepth);

    GreyScale_Image_Handler_Write_Image(grey, grey_image_copy, NULL);
}

static void testRGB_Image_R_W(){
    RGB_ImageHandler_t rgb = RGB_Image_Handler_Read_Image(rgb_image, NULL);

    printf("\nRGB Image features:\nImage Height: %d\nImage Width: %d\nImage Bit Depth: %d\n", rgb.ImageHeight, rgb.ImageWidth, rgb.BitDepth);

    RGB_Image_Handler_Write_Image(rgb, rgb_image_copy, NULL);
}


static void testBinary_Image_R_RGB_W_Binary(){
    Binary_ImageHandler_t bin_image_1 = Binary_Image_Handler_Read_Image_From_RGB(rgb_image, NULL, 125);
   
    printf("\nBinary Image @Thres: (%d)\nImage Height: %d\nImage Width: %d\nImage Bit Depth: %d\n", bin_image_1.image_thres, bin_image_1.ImageHeight, bin_image_1.ImageWidth, bin_image_1.BitDepth);

    Binary_Image_Handler_Write_Image(bin_image_1, rgb_to_bin_imageCopy, NULL);
}

static void testBinary_Image_R_RGB_W_Binary_2(){

    RGB_ImageHandler_t  rgb = RGB_Image_Handler_Read_Image(rgb_image, NULL);

    Binary_ImageHandler_t bin_image = Image_Convt_RGB_to_Binary(rgb, 125);

    printf("\nBinary Image @Thres: (%d)\nImage Height: %d\nImage Width: %d\nImage Bit Depth: %d\n", bin_image.image_thres, bin_image.ImageHeight, bin_image.ImageWidth, bin_image.BitDepth);

    Binary_Image_Handler_Write_Image(bin_image, rgb_to_bin_imageCopy_2, NULL);
}

static void testBinary_Image_R_Grey_W_Binary(){
    Binary_ImageHandler_t bin_image = Binary_Image_Handler_Read_Image_From_Greyscale(grey_image, NULL, 125);

    printf("\nBinary Image @Thres: (%d)\nImage Height: %d\nImage Width: %d\nImage Bit Depth: %d\n", bin_image.image_thres, bin_image.ImageHeight, bin_image.ImageWidth, bin_image.BitDepth);

    Binary_Image_Handler_Write_Image(bin_image, grey_to_bin_imageCopy, NULL);
}


static void testConvert_RGB_to_Grey(){
    RGB_ImageHandler_t rgb = RGB_Image_Handler_Read_Image(rgb_image, NULL);

    GreyScale_ImageHandler_t grey = Image_Convt_RGB_to_GreyScale(rgb);

    GreyScale_Image_Handler_Write_Image(grey, rgb_to_grey, NULL);
}

int main(){

    testGreyImage_R_W();

    testRGB_Image_R_W();

    testBinary_Image_R_Grey_W_Binary();

    testBinary_Image_R_RGB_W_Binary();

    testBinary_Image_R_RGB_W_Binary_2();

    testConvert_RGB_to_Grey();

    printf("Success!\n");

    return 0;
}