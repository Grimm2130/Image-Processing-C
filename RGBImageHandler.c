#include "RGBImageHandler..h"


RGB_ImageHandler_t RGB_Image_Handler_Read_Image(char* ImageName, char* ImagePath){
    RGB_ImageHandler_t anImage = {0};

    char* path = ImageName;
    int got_path = 0;

    if(ImagePath){
        path = Set_File_Path(ImagePath, ImageName);
        got_path = 1;
    }

    FILE* fp = fopen(path, "rb");

    // Read in image header
    fread(anImage.ImageHeader, sizeof(uint8_t), IMAGE_HEADER_SIZE, fp);

    // Set image params
    anImage.ImageWidth = *(int*)&anImage.ImageHeader[HEADER_IMAGE_HEIGHT_OFFSET];
    anImage.ImageHeight = *(int*)&anImage.ImageHeader[HEADER_IMAGE_HEIGHT_OFFSET];
    anImage.BitDepth = *(int*)&anImage.ImageHeader[HEADER_BIT_DEPTH_OFFSET];

    if(anImage.BitDepth <= 8){
        fread(anImage.ColorTable, sizeof(uint8_t), IMAGE_COLOR_TABLE_SIZE, fp);
    }

    const int size =  anImage.ImageWidth*anImage.ImageHeight;
    // Instantiate the image buffer
    anImage.ImageBuffer = Set_RGB_Buffer_Mem(size);
    for(int i = 0; i < size; i++){
        anImage.ImageBuffer[RED][i] = getc(fp);
        anImage.ImageBuffer[GREEN][i] = getc(fp);
        anImage.ImageBuffer[BLUE][i] = getc(fp);
    }

    fclose(fp);
    if(got_path){
        free(path);
    }

    // Check for color table
    return anImage;
}



void RGB_Image_Handler_Write_Image(RGB_ImageHandler_t anImage, char* ImageName, char* ImagePath){
    char* path = ImageName;
    int got_path = 0;

    if(ImagePath){
        path = Set_File_Path(ImagePath, ImageName);
        got_path = 1;
    }
    // File pointer
    FILE* fp = fopen(path, "wb");

    // Write header
    fwrite(anImage.ImageHeader, sizeof(uint8_t), IMAGE_HEADER_SIZE, fp);

    // Write color table if available
    if(anImage.BitDepth <= 8){
        fwrite(anImage.ColorTable, sizeof(uint8_t), IMAGE_COLOR_TABLE_SIZE, fp);
    }

    int size = anImage.ImageHeight*anImage.ImageWidth;

    for(int i = 0; i < size; i++){
        putc(anImage.ImageBuffer[RED][i], fp);
        putc(anImage.ImageBuffer[GREEN][i], fp);
        putc(anImage.ImageBuffer[BLUE][i], fp);
    }

    if(got_path){
        free(path);
    }

    fclose(fp);
}


/**
 * @brief               Function to perform a Box Blurr on an RGB image 
 * 
 * @param img 
 * @return RGB_ImageHandler_t 
 */
RGB_ImageHandler_t RGB_Image_Handler_Box_Blur(RGB_ImageHandler_t img){
    RGB_ImageHandler_t new_img = {0};

    new_img.BitDepth = img.BitDepth;
    new_img.ImageWidth = img.ImageWidth;
    new_img.ImageHeight = img.ImageHeight;

    // Copy the image header
    memcpy(new_img.ImageHeader, img.ImageHeader, IMAGE_HEADER_SIZE * sizeof(uint8_t));

    // Copy color table if available
    if(new_img.BitDepth <= 8)
        memcpy(new_img.ColorTable, img.ColorTable, IMAGE_COLOR_TABLE_SIZE * sizeof(uint8_t));

    // Copy the buffers
    int height = new_img.ImageHeight,
        width = new_img.ImageWidth,
        size = height * width;

    new_img.ImageBuffer = Set_RGB_Buffer_Mem(size);

    for(int channel = 0; channel < RGB_CHANNEL_SIZE; channel++){
        memcpy(new_img.ImageBuffer[channel], img.ImageBuffer[channel], size * sizeof(uint8_t));
    }

    // perform the Gaussian Blurr on all the channels
    for(int x = 1; x < height-1; x++){
        for(int y = 1; y < width-1; y++){

            float   red = 0,
                    green = 0,
                    blue = 0 ;
            for(int i = -1; i <= 1; i++){
                for(int j = -1; j <= 1; j++){
                    red += (float)new_img.ImageBuffer[RED][(y+j) + ((x+i)*width)] * Box_Blurr_Kernel[i+1][j+1];
                    green += (float)new_img.ImageBuffer[GREEN][(y+j) + ((x+i)*width)] * Box_Blurr_Kernel[i+1][j+1];
                    blue += (float)new_img.ImageBuffer[BLUE][(y+j) + ((x+i)*width)] * Box_Blurr_Kernel[i+1][j+1];
                }
            }

            new_img.ImageBuffer[RED][y + (x * width)] = red;
            new_img.ImageBuffer[GREEN][y + (x * width)] = green;
            new_img.ImageBuffer[BLUE][y + (x * width)] = blue;
        }
    }

    return new_img;
}
RGB_ImageHandler_t RGB_Image_Handler_Gaussian_Blur(RGB_ImageHandler_t img){
    RGB_ImageHandler_t new_img = {0};

    new_img.BitDepth = img.BitDepth;
    new_img.ImageWidth = img.ImageWidth;
    new_img.ImageHeight = img.ImageHeight;

    // Copy the image header
    memcpy(new_img.ImageHeader, img.ImageHeader, IMAGE_HEADER_SIZE * sizeof(uint8_t));

    // Copy color table if available
    if(new_img.BitDepth <= 8)
        memcpy(new_img.ColorTable, img.ColorTable, IMAGE_COLOR_TABLE_SIZE * sizeof(uint8_t));

    // Copy the buffers
    int height = new_img.ImageHeight,
        width = new_img.ImageWidth,
        size = height * width;

    new_img.ImageBuffer = Set_RGB_Buffer_Mem(size);

    for(int channel = 0; channel < RGB_CHANNEL_SIZE; channel++){
        memcpy(new_img.ImageBuffer[channel], img.ImageBuffer[channel], size * sizeof(uint8_t));
    }

    // perform the Gaussian Blurr on all the channels
    for(int x = 1; x < height-1; x++){
        for(int y = 1; y < width-1; y++){

            float   red = 0,
                    green = 0,
                    blue = 0 ;
            for(int i = -1; i <= 1; i++){
                for(int j = -1; j <= 1; j++){
                    red += (float)new_img.ImageBuffer[RED][(y+j) + ((x+i)*width)] * Gaussian_Blurr_Kernel[i+1][j+1];
                    green += (float)new_img.ImageBuffer[GREEN][(y+j) + ((x+i)*width)] * Gaussian_Blurr_Kernel[i+1][j+1];
                    blue += (float)new_img.ImageBuffer[BLUE][(y+j) + ((x+i)*width)] * Gaussian_Blurr_Kernel[i+1][j+1];
                }
            }

            new_img.ImageBuffer[RED][y + (x * width)] = red;
            new_img.ImageBuffer[GREEN][y + (x * width)] = green;
            new_img.ImageBuffer[BLUE][y + (x * width)] = blue;
        }
    }

    return new_img;
}


/**
 * @brief           Function to Apply a sepia filter on an RGB image
 * 
 * @param img 
 * @return RGB_ImageHandler_t 
 */
RGB_ImageHandler_t RGB_Image_Handler_Sepia_Filter(RGB_ImageHandler_t img){
    RGB_ImageHandler_t new_img = {0};

    new_img.BitDepth = img.BitDepth;
    new_img.ImageWidth = img.ImageWidth;
    new_img.ImageHeight = img.ImageHeight;

    // Copy the image header
    memcpy(new_img.ImageHeader, img.ImageHeader, IMAGE_HEADER_SIZE * sizeof(uint8_t));

    // Copy color table if available
    if(new_img.BitDepth <= 8)
        memcpy(new_img.ColorTable, img.ColorTable, IMAGE_COLOR_TABLE_SIZE * sizeof(uint8_t));

    // Copy the channel buffers
    int height = new_img.ImageHeight,
        width = new_img.ImageWidth,
        size = height * width;

    new_img.ImageBuffer = Set_RGB_Buffer_Mem(size);

    for(int channel = 0; channel < RGB_CHANNEL_SIZE; channel++){
        memcpy(new_img.ImageBuffer[channel], img.ImageBuffer[channel], size * sizeof(uint8_t));
    }

    // Apply a sepia filter on
    int red = 0, green = 0, blue = 0;
    for(int i = 0; i < size; i++){
        red = (new_img.ImageBuffer[RED][i] * Sepia_kernel[0][0]) + (new_img.ImageBuffer[GREEN][i] * Sepia_kernel[0][1]) + (new_img.ImageBuffer[BLUE][i] * Sepia_kernel[0][2]);
        green = (new_img.ImageBuffer[RED][i] * Sepia_kernel[1][0]) + (new_img.ImageBuffer[GREEN][i] * Sepia_kernel[1][1]) + (new_img.ImageBuffer[BLUE][i] * Sepia_kernel[1][2]);
        blue = (new_img.ImageBuffer[RED][i] * Sepia_kernel[2][0]) + (new_img.ImageBuffer[GREEN][i] * Sepia_kernel[2][1]) + (new_img.ImageBuffer[BLUE][i] * Sepia_kernel[2][2]);
    
        red = red > MAX_PIXEL_VAL ? MAX_PIXEL_VAL : red;
        green = green > MAX_PIXEL_VAL ? MAX_PIXEL_VAL : green;
        blue = blue > MAX_PIXEL_VAL ? MAX_PIXEL_VAL : blue;

        new_img.ImageBuffer[RED][i] = red;
        new_img.ImageBuffer[GREEN][i] = green;
        new_img.ImageBuffer[BLUE][i] = blue;
    }

    return new_img;
}