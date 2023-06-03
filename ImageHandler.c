#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "Utils.h"
#include "ImageHandler.h"



const float Rgb_to_Bin_conv_floats[3] = {0.3, 0.59, 0.11};

/*======================================================================================*/
/*============================== Special Helper Functions ==============================*/
/*======================================================================================*/

static char* Set_File_Path(char* a, char* b){
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


static uint8_t** Set_RGB_Buffer_Mem(const int size){
    uint8_t **ret = (uint8_t**) calloc(RGB_CHANNEL_SIZE, sizeof(uint8_t*));
    for(int i = 0; i < RGB_CHANNEL_SIZE; i++){
        ret[i] = (uint8_t*) calloc(size, sizeof(uint8_t));
    }
    return ret;
}

/*======================================================================================*/
/*=============================== GreyScale Image Functions ===============================*/
/*======================================================================================*/

/**
 * @brief           Image Handler, Read image function
 * 
 * @param ImageName Name of image to be read
 * @param ImagePath Path to image to be read. Root dir if null
 * @return GreyScale_ImageHandler_t 
 */
GreyScale_ImageHandler_t GreyScale_Image_Handler_Read_Image(char* ImageName, char* ImagePath){
    // define image handler
    GreyScale_ImageHandler_t anImage = {0};
    char* path = ImageName;
    int str_app = 0;                    // path created notifier
    // Concatenate image name if image path not included
    if(ImagePath != NULL){
        path = Set_File_Path(ImagePath, ImageName);
        str_app = 1;
    }

    // open the image
    FILE* fp = fopen(path, "rb");
    if(!fp){
        printf("File failed to open. Error code: %d\n", errno);
        fclose(fp);
        exit(1);
    }
    // read in the image header
    for(int i = 0; i < IMAGE_HEADER_SIZE; i++){
        anImage.ImageHeader[i] = getc(fp);
    }
    anImage.FromRGB = 0;
    anImage.ImageWidth = *(int*)&anImage.ImageHeader[HEADER_IMAGE_WIDTH_OFFSET];
    anImage.ImageHeight = *(int*)&anImage.ImageHeader[HEADER_IMAGE_HEIGHT_OFFSET];
    anImage.BitDepth = *(int*)&anImage.ImageHeader[HEADER_BIT_DEPTH_OFFSET];

    // Set image buffer size
    const int buffer_size = anImage.ImageHeight*anImage.ImageHeight;
    anImage.ImageBuffer = (uint8_t*) calloc(buffer_size, sizeof(uint8_t));
    // Check for a color table
    if(anImage.BitDepth <= 8){
        fread(anImage.ColorTable, sizeof(uint8_t), IMAGE_COLOR_TABLE_SIZE, fp);
    }
    else{
        memset(anImage.ColorTable, '\0', IMAGE_COLOR_TABLE_SIZE);
        anImage.ColorTable[0] = 'x';
    }

    // Read in the image pixels to the buffer
    fread(anImage.ImageBuffer, sizeof(uint8_t), buffer_size, fp);

    // Close files and return image
    fclose(fp);
    if(str_app){
        free(path);
    }
    return anImage;
}


/**
 * @brief               Function to write a GreyScale image to a file
 * 
 * @param anImage       GreyScale Image Object
 * @param ImageName     name of image
 * @param ImagePath     Path to image
 */
void GreyScale_Image_Handler_Write_Image(GreyScale_ImageHandler_t anImage, char* ImageName, char* ImagePath){
    // Define a pointer to point to the imagefile
    char* path = ImageName;
    int str_app = 0;
    if(ImagePath){
        path = Set_File_Path(ImagePath, ImageName);
        str_app = 1;
    }

    FILE* fp = fopen(path, "wb+");

    // Write the header file
    fwrite(anImage.ImageHeader, sizeof(uint8_t), IMAGE_HEADER_SIZE, fp);

    // Write the Color table if it exists
    if(anImage.BitDepth <= 8){
        fwrite(anImage.ColorTable, sizeof(uint8_t), IMAGE_COLOR_TABLE_SIZE, fp);
    }

    // Write the buffer contents
    int size =anImage.ImageWidth*anImage.ImageHeight;
    
    if(anImage.FromRGB){
        size *= 3;
    }

    fwrite(anImage.ImageBuffer, sizeof(uint8_t), size, fp);

    fclose(fp);

    if(str_app){
        free(path);
    }
}


/*======================================================================================*/
/*================================= RGB Image Functions ================================*/
/*======================================================================================*/

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


/*======================================================================================*/
/*=============================== Binary Image Functions ===============================*/
/*======================================================================================*/



Binary_ImageHandler_t Binary_Image_Handler_Read_Image_From_Greyscale(char* ImageName, char* ImagePath, int thres){
 
    Binary_ImageHandler_t bin_image;

    // Set threshold value
    bin_image.image_thres = thres;

    char* path = ImageName;
    int got_path = 0;

    if(ImagePath){
        path = Set_File_Path(ImagePath, ImageName);
        got_path = 1;
    }

    FILE* fp = fopen(path, "rb");

    if(!fp){
        printf("Failed to open file: %s\n", path);
        exit(1);
    }

    // Read in the header
    fread(bin_image.ImageHeader, sizeof(uint8_t), IMAGE_HEADER_SIZE, fp);
    bin_image.FromRGB = 0;
    bin_image.ImageWidth = *(int*)&bin_image.ImageHeader[HEADER_IMAGE_WIDTH_OFFSET];
    bin_image.ImageHeight = *(int*)&bin_image.ImageHeader[HEADER_IMAGE_HEIGHT_OFFSET];
    bin_image.BitDepth = *(int*)&bin_image.ImageHeader[HEADER_BIT_DEPTH_OFFSET];

    // Set color table if exists
    if(bin_image.BitDepth <= 8){
        fread(bin_image.ColorTable, sizeof(uint8_t), IMAGE_COLOR_TABLE_SIZE, fp);
    }

    // Read in the buffer
    int size = bin_image.ImageHeight * bin_image.ImageWidth;
    bin_image.ImageBuffer = (uint8_t*) calloc(size, sizeof(uint8_t));

    for(int i = 0; i < size; i++){
        int pix_val = getc(fp);
        bin_image.ImageBuffer[i] = pix_val < bin_image.image_thres ? 0 : 255;
    }
    if(got_path){
        free(path);
    }
    fclose(fp);

    return bin_image;
}


Binary_ImageHandler_t Binary_Image_Handler_Read_Image_From_RGB(char* ImageName, char* ImagePath, int thres){
    Binary_ImageHandler_t bin_image;

    // Set threshold value
    bin_image.image_thres = thres;

    char* path = ImageName;
    int got_path = 0;

    if(ImagePath){
        path = Set_File_Path(ImagePath, ImageName);
        got_path = 1;
    }

    FILE* fp = fopen(path, "rb");

    if(!fp){
        printf("Failed to open file: %s\n", path);
        exit(1);
    }

    // Read in the header
    fread(bin_image.ImageHeader, sizeof(uint8_t), IMAGE_HEADER_SIZE, fp);
    bin_image.FromRGB = 1;
    bin_image.ImageWidth = *(int*)&bin_image.ImageHeader[HEADER_IMAGE_WIDTH_OFFSET];
    bin_image.ImageHeight = *(int*)&bin_image.ImageHeader[HEADER_IMAGE_HEIGHT_OFFSET];
    bin_image.BitDepth = *(int*)&bin_image.ImageHeader[HEADER_BIT_DEPTH_OFFSET];

    // Set color table if exists
    if(bin_image.BitDepth <= 8){
        fread(bin_image.ColorTable, sizeof(uint8_t), IMAGE_COLOR_TABLE_SIZE, fp);
    }

    // Read in the buffer
    int size = bin_image.ImageHeight * bin_image.ImageWidth*3;
    bin_image.ImageBuffer = (uint8_t*) calloc(size, sizeof(uint8_t));

    uint16_t pix_val = 0;

    for(int i = 0; i < size; i+=3){
        pix_val =  getc(fp) + getc(fp) + getc(fp);       
        pix_val/=3;
        pix_val = pix_val < bin_image.image_thres ? 0 : 255;
        bin_image.ImageBuffer[i] = pix_val;
        bin_image.ImageBuffer[i+1] = pix_val;
        bin_image.ImageBuffer[i+2] = pix_val;
    }

    if(got_path){
        free(path);
    }
    fclose(fp);

    return bin_image;
}

void Binary_Image_Handler_Write_Image(Binary_ImageHandler_t anImage, char* ImageName, char* ImagePath){
    char* path = ImageName;
    int got_path = 0;

    if(ImagePath){
        path = Set_File_Path(ImagePath, ImageName);
        got_path = 1;
    }

    FILE* fp = fopen(path, "wb+");

    // Write in the header
    fwrite(anImage.ImageHeader, sizeof(uint8_t), IMAGE_HEADER_SIZE, fp);

    // Set color table if exists
    if(anImage.BitDepth <= 8){
        fwrite(anImage.ColorTable, sizeof(uint8_t), IMAGE_COLOR_TABLE_SIZE, fp);
    }

    // Write in the buffer
    int size = anImage.ImageHeight * anImage.ImageWidth;

    if(anImage.FromRGB){
        size*=3;
    }
    
    fwrite(anImage.ImageBuffer, sizeof(uint8_t), size, fp);

    if(got_path){
        free(path);
    }

    fclose(fp);
    
}

/*======================================================================================*/
/*================================= Conversion Functions ===============================*/
/*======================================================================================*/


/**
 * @brief               Function to convert an RGB image to a GreyScale Image
 * 
 * @param rgb_image     Image handler for RGB format image
 * @return GreyScale_ImageHandler_t 
 */
GreyScale_ImageHandler_t Image_Convt_RGB_to_GreyScale(RGB_ImageHandler_t rgb_image){
  
    GreyScale_ImageHandler_t anImage = {0};

    // Copy the image header
    memcpy(anImage.ImageHeader, rgb_image.ImageHeader, IMAGE_HEADER_SIZE*sizeof(uint8_t));

    // Set the image features
    anImage.FromRGB = 1;
    anImage.ImageHeight = rgb_image.ImageHeight;
    anImage.ImageWidth = rgb_image.ImageWidth;
    anImage.BitDepth = rgb_image.BitDepth;

    if(anImage.BitDepth <= 8)
        memcpy(anImage.ColorTable, rgb_image.ColorTable, IMAGE_COLOR_TABLE_SIZE*sizeof(uint8_t));


    int size = anImage.ImageWidth*anImage.ImageHeight*3;

    anImage.ImageBuffer = (uint8_t*) calloc(size, sizeof(uint8_t));

    // #TODO: Make the copying process actually work
    uint8_t temp = '\0';
    int true_size = rgb_image.ImageHeight*rgb_image.ImageWidth;

    for(int i = 0, j = 0; i < true_size && j < size; i++, j+=3){

        temp = (uint8_t) ((rgb_image.ImageBuffer[RED][i] * Rgb_to_Bin_conv_floats[RED]) +
                                (rgb_image.ImageBuffer[GREEN][i] * Rgb_to_Bin_conv_floats[GREEN]) +
                                (rgb_image.ImageBuffer[BLUE][i] * Rgb_to_Bin_conv_floats[BLUE]));
        
        anImage.ImageBuffer[j] = temp;
        anImage.ImageBuffer[j+1] = temp;
        anImage.ImageBuffer[j+2] = temp;
    }

    return anImage;
}


Binary_ImageHandler_t Image_Convt_RGB_to_Binary(RGB_ImageHandler_t rgb_image, int thres){
    Binary_ImageHandler_t anImage = {0};

    // Copy the image header
    memcpy(anImage.ImageHeader, rgb_image.ImageHeader, IMAGE_HEADER_SIZE*sizeof(uint8_t));

    // Set the image features
    anImage.image_thres = thres;
    anImage.FromRGB = 1;
    anImage.ImageHeight = rgb_image.ImageHeight;
    anImage.ImageWidth = rgb_image.ImageWidth;
    anImage.BitDepth = rgb_image.BitDepth;

    if(anImage.BitDepth <= 8)
        memcpy(anImage.ColorTable, rgb_image.ColorTable, IMAGE_COLOR_TABLE_SIZE*sizeof(uint8_t));


    int size = anImage.ImageWidth*anImage.ImageHeight*3;
    anImage.ImageBuffer = (uint8_t*) calloc(size, sizeof(uint8_t));

    uint16_t temp = 0;
    // #TODO: Make the copying process actually work
    int true_size = rgb_image.ImageHeight*rgb_image.ImageWidth;

    for(int i = 0, j = 0; i < true_size && j < size; i++, j+=3){
        temp =  rgb_image.ImageBuffer[RED][i] +
                rgb_image.ImageBuffer[GREEN][i] +
                rgb_image.ImageBuffer[BLUE][i];
        temp/=3;
        // threshold picture
        temp = temp < thres ? 0 : 255;

        anImage.ImageBuffer[j] = temp;
        anImage.ImageBuffer[j+1] = temp;
        anImage.ImageBuffer[j+2] = temp;
    }

    return anImage;
}


/*======================================================================================*/
/*================================= Arithmetic Functions ===============================*/
/*======================================================================================*/

GreyScale_ImageHandler_t Greyscale_Brighten_Image_Saturation(GreyScale_ImageHandler_t image, uint8_t offset){
     GreyScale_ImageHandler_t bright_image = {0};
     
     // Copy feature parameters
     bright_image.BitDepth = image.BitDepth;
     bright_image.FromRGB = image.FromRGB;
     bright_image.ImageHeight = image.ImageHeight;
     bright_image.ImageWidth = image.ImageWidth;
     
     // Copy image header
     for(int i = 0; i < IMAGE_HEADER_SIZE; i++){
        bright_image.ImageHeader[i] = image.ImageHeader[i];
     }
    
    // Copy color table
    if(bright_image.BitDepth <= 8){
        for(int i = 0; i < IMAGE_COLOR_TABLE_SIZE; i++){
            bright_image.ColorTable[i] = image.ColorTable[i];
        }
    }

    // Copy buffer

    int size = bright_image.ImageHeight*bright_image.ImageWidth;

    if(bright_image.FromRGB){
        size*=3;
    }

    bright_image.ImageBuffer = (uint8_t*) calloc(size, sizeof(uint8_t));

    uint16_t temp = 0;
    for(int i = 0; i < size; i++){
        temp = image.ImageBuffer[i] + offset;
        temp = temp > MAX_PIXEL_VAL ? MAX_PIXEL_VAL : temp;
         bright_image.ImageBuffer[i] = 0xFF & temp;
    }

    return bright_image;
}


GreyScale_ImageHandler_t Greyscale_Darken_Image_Saturation(GreyScale_ImageHandler_t image, uint8_t offset){
    GreyScale_ImageHandler_t dark_image;
    
    // Copy feature parameters
     dark_image.BitDepth = image.BitDepth;
     dark_image.FromRGB = image.FromRGB;
     dark_image.ImageHeight = image.ImageHeight;
     dark_image.ImageWidth = image.ImageWidth;
     
     // Copy image header
     for(int i = 0; i < IMAGE_HEADER_SIZE; i++){
        dark_image.ImageHeader[i] = image.ImageHeader[i];
     }
    
    // Copy color table
    if(dark_image.BitDepth <= 8){
        for(int i = 0; i < IMAGE_COLOR_TABLE_SIZE; i++){
            dark_image.ColorTable[i] = image.ColorTable[i];
        }
    }

    // Copy buffer

    int size = dark_image.ImageHeight*dark_image.ImageWidth;

    if(dark_image.FromRGB){
        size*=3;
    }

    dark_image.ImageBuffer = (uint8_t*) calloc(size, sizeof(uint8_t));

    int16_t temp = 0;
    for(int i = 0; i < size; i++){
        temp = image.ImageBuffer[i] - offset;
        temp = temp < MIN_PIXEL_VAL ? MIN_PIXEL_VAL : temp;
        dark_image.ImageBuffer[i] = 0xFF & temp;
    }

    return dark_image;
}


/*======================================================================================*/
/*======================= Image Histogram Operation Functions ==========================*/
/*======================================================================================*/

/**
 * @brief           Function to compute the histogram of an image
 * 
 * @param img       Grey Scale Image handler
 * @return float* 
 */
float* Image_Handler_Compute_Histogram(GreyScale_ImageHandler_t img){
    int size = img.ImageHeight*img.ImageWidth;

    if(img.FromRGB)
        size *= 3;

    float* Hist = (float*) calloc(MAX_PIXEL_VAL+1, sizeof(float));

    // Compute the frequency of the images
    for(int i = 0; i < size; i++){
        Hist[img.ImageBuffer[i]]++;
    }

    return Hist;
}


float* Image_Handler_Compute_Equalized_Histogram(float* hist, int size){
    float *Equalized_Hist = (float*) calloc(size, sizeof(float));
    float sum = 0.0;
    for(int i = 0; i < size; i++){
        sum = 0.0;
        // Compute sum of preceding pixels inclusive of current
        for(int j = 0; j <= i; j++){
            sum += hist[j];
        }
        Equalized_Hist[i] = (255.0*sum)+0.5;
    }
    return Equalized_Hist;
}


GreyScale_ImageHandler_t  Image_Handler_Equalize_Image_Pixels(GreyScale_ImageHandler_Ptr img_p){

    GreyScale_ImageHandler_t equalized_img = {0};
    equalized_img.BitDepth = img_p->BitDepth;
    equalized_img.FromRGB = img_p->FromRGB;
    equalized_img.ImageHeight = img_p->ImageHeight;
    equalized_img.ImageWidth = img_p->ImageWidth;

    // Copy header
    for(int i = 0; i < IMAGE_HEADER_SIZE; i++){
        equalized_img.ImageHeader[i] = img_p->ImageHeader[i];
    }

    // Copy Color table
    if(equalized_img.BitDepth <= 8){
        for(int i = 0; i < IMAGE_COLOR_TABLE_SIZE; i++){
            equalized_img.ColorTable[i] = img_p->ColorTable[i];
        }
    }

    // Compute image histogram
    float* hist = Image_Handler_Compute_Histogram(*img_p);
    
    // get Size of buffer/Num. of pixels
    int size = equalized_img.ImageHeight*equalized_img.ImageWidth;

    if(equalized_img.FromRGB){
        size*=3;
    }

    // Normalize Histogram
    NormalizeFloatArr(&hist, MAX_PIXEL_VAL+1, size);

    // Compute Equlized values of histogram
    float* hist_eq = Image_Handler_Compute_Equalized_Histogram(hist, MAX_PIXEL_VAL+1);

    // Set the values in the image buffer
    equalized_img.ImageBuffer = (uint8_t*) calloc(size, sizeof(uint8_t));
    int temp = 0,
        pix_val = 0;
    for(int i = 0; i < size; i++){
        pix_val = img_p->ImageBuffer[i];
        // Get normalized pixel value
        temp = (int) hist_eq[pix_val];
        // Extract and set in equalized Image buffer
        equalized_img.ImageBuffer[i] = 0xFF & temp;
    }

    return equalized_img;
}



/*======================================================================================*/
/*======================= Image Rotation Operation Functions ==========================*/
/*======================================================================================*/

GreyScale_ImageHandler_t Image_Handler_Rotate_Image_Right(GreyScale_ImageHandler_t img){
    GreyScale_ImageHandler_t new_img = {0};

    new_img.BitDepth = img.BitDepth;
    new_img.FromRGB = img.FromRGB;

    // Flip the image height and width values
    new_img.ImageHeight = img.ImageWidth;
    new_img.ImageWidth = img.ImageHeight;

     // Copy header
    for(int i = 0; i < IMAGE_HEADER_SIZE; i++){
        new_img.ImageHeader[i] = img.ImageHeader[i];
    }

    // Copy Color table
    if(new_img.BitDepth <= 8){
        for(int i = 0; i < IMAGE_COLOR_TABLE_SIZE; i++){
            new_img.ColorTable[i] = img.ColorTable[i];
        }
    }

    // set buffer size
    int height = img.ImageHeight,
        width = img.ImageWidth;
    
    


    // Copy back into the image buffer
    
    new_img.ImageBuffer = (uint8_t*) calloc((height * width), sizeof(uint8_t));

    //  for(int i = 0; i < height; i++){
    //     for(int j = 0; j < width; j++){
    //         new_img.ImageBuffer[j + (i * width)] = copy_buff[i][j];
    //     }
    // }

    // Copy the buffer
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            new_img.ImageBuffer[j + (i * height)] = img.ImageBuffer[(height - 1 - i) + (j * width)] ;
        }
    }

    return new_img;
}

/**
 * @brief           Function to rotate an image left
 * 
 * @param img 
 * @return GreyScale_ImageHandler_t 
 */
GreyScale_ImageHandler_t Image_Handler_Rotate_Image_Left(GreyScale_ImageHandler_t img){
    GreyScale_ImageHandler_t new_img;

    new_img.BitDepth = img.BitDepth;
    new_img.FromRGB = img.FromRGB;

    // Flip the image height and width values
    new_img.ImageHeight = img.ImageWidth;
    new_img.ImageWidth = img.ImageHeight;

     // Copy header
    for(int i = 0; i < IMAGE_HEADER_SIZE; i++){
        new_img.ImageHeader[i] = img.ImageHeader[i];
    }

    // Copy Color table
    if(new_img.BitDepth <= 8){
        for(int i = 0; i < IMAGE_COLOR_TABLE_SIZE; i++){
            new_img.ColorTable[i] = img.ColorTable[i];
        }
    }

    // set buffer size
    int height = img.ImageHeight,
        width = img.ImageWidth;
    
    new_img.ImageBuffer = (uint8_t*) calloc((height * width), sizeof(uint8_t));

    // Copy the buffer
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            new_img.ImageBuffer[j + (i * height)] = img.ImageBuffer[i + ((width-1-j) * width)];
        }
    }


    return new_img;
}


GreyScale_ImageHandler_t Image_Handler_Rotate_Image_180(GreyScale_ImageHandler_t img){
    GreyScale_ImageHandler_t new_img;

    return new_img;
}