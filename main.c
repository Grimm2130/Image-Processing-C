#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ImageHandler.h"
#include "GnuPlotter.h"
#include "errno.h"
#include "Utils.h"


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


static void test_image_brighten(){

    char* grey_lena_image = "TestImages/lena512.bmp";

    GreyScale_ImageHandler_t grey = GreyScale_Image_Handler_Read_Image(grey_lena_image, NULL);

    GreyScale_ImageHandler_t bright_grey = Greyscale_Brighten_Image_Saturation(grey, 125);

    char* brightened_grey = "TestImages/lena512_bright.bmp";

    GreyScale_Image_Handler_Write_Image(bright_grey, brightened_grey, NULL);
}

static void test_image_darken(){

    char* grey_lena_image = "TestImages/lena512.bmp";

    GreyScale_ImageHandler_t grey = GreyScale_Image_Handler_Read_Image(grey_lena_image, NULL);

    GreyScale_ImageHandler_t dark_grey = Greyscale_Darken_Image_Saturation(grey, 70);

    char* darkened_grey = "TestImages/lena512_dark.bmp";

    GreyScale_Image_Handler_Write_Image(dark_grey, darkened_grey, NULL);
}

static void Test_Image_Rotation(){

    GreyScale_ImageHandler_t img = GreyScale_Image_Handler_Read_Image(grey_image, NULL);

    GreyScale_ImageHandler_t img_right_flipped = Greyscale_Image_Handler_Rotate_Image_Right(img);

    GreyScale_ImageHandler_t img_left_flipped = Greyscale_Image_Handler_Rotate_Image_Left(img);

    GreyScale_Image_Handler_Write_Image(img_right_flipped, "Right_Flipped_Image.bmp", "TestImages");

    GreyScale_Image_Handler_Write_Image(img_left_flipped, "Left_Flipped_Image.bmp", "TestImages");
}

static void Test_Image_Negation(){
    GreyScale_ImageHandler_t img = GreyScale_Image_Handler_Read_Image(grey_image, NULL);

    GreyScale_ImageHandler_t img_neg = Greyscale_Image_Handler_Negative_Transform(img);

    GreyScale_Image_Handler_Write_Image(img_neg, "Negative_Image.bmp", "TestImages");
}


static void Test_Image_Box_Blur(){
    GreyScale_ImageHandler_t img = GreyScale_Image_Handler_Read_Image(grey_image, NULL);

    GreyScale_ImageHandler_t blurred = Greyscale_Image_Handler_Box_Blur(img);

    GreyScale_Image_Handler_Write_Image(blurred, "Box_Blurred_Image.bmp", "TestImages");
}


static void Test_Image_Gaussian_Blur(){
    GreyScale_ImageHandler_t img = GreyScale_Image_Handler_Read_Image(grey_image, NULL);

    GreyScale_ImageHandler_t blurred = Greyscale_Image_Handler_Gaussian_Blur(img);

    GreyScale_Image_Handler_Write_Image(blurred, "Gaussian_Blurred_Image.bmp", "TestImages");
}

static void Test_Image_Laplacian_Blur(){
    GreyScale_ImageHandler_t img = GreyScale_Image_Handler_Read_Image(grey_image, NULL);

    GreyScale_ImageHandler_t lap = Greyscale_Image_Handler_Laplacian_Convolution(img);

    GreyScale_Image_Handler_Write_Image(lap, "Laplacian_Blurred_Image.bmp", "TestImages");
}

static void Test_Image_Box_Blur_RGB(){
    RGB_ImageHandler_t img = RGB_Image_Handler_Read_Image(rgb_image, NULL);

    RGB_ImageHandler_t blurred = RGB_Image_Handler_Box_Blur(img);

    RGB_Image_Handler_Write_Image(blurred, "Box_Blurred_Image.bmp", "TestImages");
}

static void Test_Image_Gaussian_Blur_RGB(){
    RGB_ImageHandler_t img = RGB_Image_Handler_Read_Image(rgb_image, NULL);

    RGB_ImageHandler_t blurred = RGB_Image_Handler_Gaussian_Blur(img);

    RGB_Image_Handler_Write_Image(blurred, "Gausian_Blurred_Image.bmp", "TestImages");
}

static void Test_Image_Sepia_Filter(){
    RGB_ImageHandler_t img = RGB_Image_Handler_Read_Image(rgb_image, NULL);

    RGB_ImageHandler_t sepiad = RGB_Image_Handler_Sepia_Filter(img);

    RGB_Image_Handler_Write_Image(sepiad, "Sepiad_Image.bmp", "TestImages");
}


int main(){

    // testGreyImage_R_W();

    // testRGB_Image_R_W();

    // testBinary_Image_R_Grey_W_Binary();

    // testBinary_Image_R_RGB_W_Binary();

    // testBinary_Image_R_RGB_W_Binary_2();

    // testConvert_RGB_to_Grey();

    // test_image_brighten(); 

    // test_image_darken();

    // Test_Image_Rotation();

    // Test_Image_Negation();

    // Test_Image_Box_Blur();

    // Test_Image_Gaussian_Blur();

    // Test_Image_Box_Blur_RGB();

    // Test_Image_Gaussian_Blur_RGB();

    // Test_Image_Sepia_Filter();
    
    Test_Image_Laplacian_Blur();


    return 0;
}