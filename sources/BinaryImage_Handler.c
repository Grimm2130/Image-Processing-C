#include "../headers/BinaryImage_Handler.h"


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

