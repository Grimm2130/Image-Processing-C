#include "../headers/GreyscaleImageHandler.h"

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


/**
 * @brief           Function to rotate an image Clockwise by 90 degrees
 * 
 * @param img 
 * @return GreyScale_ImageHandler_t 
 */
GreyScale_ImageHandler_t Greyscale_Image_Handler_Rotate_Image_Right(GreyScale_ImageHandler_t img){
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

    // Copy the buffer
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            new_img.ImageBuffer[j + (i * height)] = img.ImageBuffer[(height - 1 - i) + (j * width)] ;
        }
    }

    return new_img;
}

/**
 * @brief           Function to rotate an image counter-clockwise 90 degrees
 * 
 * @param img 
 * @return GreyScale_ImageHandler_t 
 */
GreyScale_ImageHandler_t Greyscale_Image_Handler_Rotate_Image_Left(GreyScale_ImageHandler_t img){
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

/**
 * @brief       Function to Rotate an image by 180 degrees
 * 
 * @param img 
 * @return GreyScale_ImageHandler_t 
 */
GreyScale_ImageHandler_t Greyscale_Image_Handler_Rotate_Image_180(GreyScale_ImageHandler_t img){
    GreyScale_ImageHandler_t new_img;
    // {# TODO: add functionality }
    return new_img;
}


/**
 * @brief           Function to Perform a negative transformation on an image
 * 
 * @param img 
 * @return GreyScale_ImageHandler_t 
 */
GreyScale_ImageHandler_t  Greyscale_Image_Handler_Negative_Transform(GreyScale_ImageHandler_t img){
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

    // Allocate a new buffer
    int size = new_img.ImageHeight*new_img.ImageWidth;

    if(new_img.FromRGB){
        size*=3;
    }
    new_img.ImageBuffer = (uint8_t*) calloc(size, sizeof(uint8_t));

    for(int i = 0; i < size; i++){
        new_img.ImageBuffer[i] = MAX_PIXEL_VAL - img.ImageBuffer[i];
    }

    return new_img;
}

/**
 * @brief       Function to perform a Box Blurring on an image 
 * 
 * @param img 
 * @return GreyScale_ImageHandler_t 
 */
GreyScale_ImageHandler_t Greyscale_Image_Handler_Box_Blur(GreyScale_ImageHandler_t img){
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

    // Define new image buffer
    int height = new_img.ImageHeight,
        width = new_img.ImageWidth;
    
    new_img.ImageBuffer = (uint8_t*) calloc(width*height, sizeof(uint8_t));

    // Copy the contents of the old buffer
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            new_img.ImageBuffer[j + (i * width)] = img.ImageBuffer[j + (i * width)];
        }
    }

    // Perform the filter operation
    for(int x = 1; x < (height-1); x++){
        for(int y = 1; y < (width-1); y++){
            // Accumulate the filter value for the kernel
            float sum = 0;
            for(int i = -1; i <= 1; i++){
                for(int j = -1; j <= 1; j++){
                    sum += (float)new_img.ImageBuffer[(y+j) + ((x+i)*width)] * Box_Blurr_Kernel[i+1][j+1];
                }
            }
            new_img.ImageBuffer[y + (x*width)] = (uint8_t) sum;
        }
    }

    return new_img;
}


/**
 * @brief           Function to perform a Gaussian Blur on a Grey scale image
 * 
 * @param img       Greyscale image
 * @return GreyScale_ImageHandler_t 
 */
GreyScale_ImageHandler_t Greyscale_Image_Handler_Gaussian_Blur(GreyScale_ImageHandler_t img){
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

    // Define new image buffer
    int height = new_img.ImageHeight,
        width = new_img.ImageWidth;
    
    new_img.ImageBuffer = (uint8_t*) calloc(width*height, sizeof(uint8_t));

    // Copy the contents of the old buffer
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            new_img.ImageBuffer[j + (i * width)] = img.ImageBuffer[j + (i * width)];
        }
    }

    // Perform the filter operation
    for(int x = 1; x < (height-1); x++){
        for(int y = 1; y < (width-1); y++){
            // Accumulate the filter value for the kernel
            float sum = 0;
            for(int i = -1; i <= 1; i++){
                for(int j = -1; j <= 1; j++){
                    sum += (float)new_img.ImageBuffer[(y+j) + ((x+i)*width)] * Gaussian_Blurr_Kernel[i+1][j+1];
                }
            }
            new_img.ImageBuffer[y + (x*width)] = (uint8_t) sum;
        }
    }

    return new_img;
}



/**
 * @brief           Function to perform Laplacian convolution on an image
 * 
 * @param img 
 * @return GreyScale_ImageHandler_t 
 */
GreyScale_ImageHandler_t Greyscale_Image_Handler_Laplacian_Convolution(GreyScale_ImageHandler_t img){
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

    // Define new image buffer
    int height = new_img.ImageHeight,
        width = new_img.ImageWidth;
    
    new_img.ImageBuffer = (uint8_t*) calloc(width*height, sizeof(uint8_t));
    
    // Copy the buffers values
    memcpy(new_img.ImageBuffer, img.ImageBuffer, ( height * width) * sizeof(uint8_t));

    // Get a laplacian mask
    Mask_t laplacian = Mask_Init(Laplacian);

    // Perform the convolution operation
    Mask_Convolve(img.ImageBuffer, height, width, laplacian, new_img.ImageBuffer);

    return new_img;
}

GreyScale_ImageHandler_t Greyscale_Image_Handler_Edge_Detect_Single(GreyScale_ImageHandler_t img, Mask_Types_t edge_flow){
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

    // Define new image buffer
    int height = new_img.ImageHeight,
        width = new_img.ImageWidth;

    new_img.ImageBuffer = (uint8_t*) calloc(width*height, sizeof(uint8_t));

    memcpy(new_img.ImageBuffer, img.ImageBuffer, height*width);

    Mask_t mask = Mask_Init(Edge_Vertical);

    Mask_Convolve(img.ImageBuffer, height, width, mask, new_img.ImageBuffer);

    return new_img;
}


GreyScale_ImageHandler_t Greyscale_Image_Handler_Edge_Detect_Complete(GreyScale_ImageHandler_t img){
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

    // Define new image buffer
    int height = new_img.ImageHeight,
        width = new_img.ImageWidth;
    
    new_img.ImageBuffer = (uint8_t*) calloc(width*height, sizeof(uint8_t));
    
    // detect vertical
    uint8_t     *vertical = Init_Buffer(height, width),
                *horizontal = Init_Buffer(height, width),
                *left_diag = Init_Buffer(height, width),
                *right_diag = Init_Buffer(height, width);


    Mask_Convolve(img.ImageBuffer, height, width, Mask_Init(Edge_Vertical), vertical);
    Mask_Convolve(img.ImageBuffer, height, width, Mask_Init(Edge_Horizontal), horizontal);
    Mask_Convolve(img.ImageBuffer, height, width, Mask_Init(Edge_Left_Diag), left_diag);
    Mask_Convolve(img.ImageBuffer, height, width, Mask_Init(Edge_Right_Diag), right_diag);

    // And all Diagonals
    Buffer_And(vertical, new_img.ImageBuffer, height, width);
    Buffer_And(horizontal, new_img.ImageBuffer, height, width);
    Buffer_And(left_diag, new_img.ImageBuffer, height, width);
    Buffer_And(right_diag, new_img.ImageBuffer, height, width);

    free(vertical);
    free(horizontal);
    free(left_diag);
    free(right_diag);

    return new_img;
}


/**
 * @brief               Function to convert an RGB image to a GreyScale Image
 * 
 * @param rgb_image     Image handler for RGB format image
 * @return GreyScale_ImageHandler_t 
 */
GreyScale_ImageHandler_t Greyscale_Image_Handler_Convt_RGB_to_GreyScale(RGB_ImageHandler_t rgb_image){
  
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
