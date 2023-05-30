#ifndef IMAGE_HANDLER_H
#define IMAGE_HANDLER_H

#define IMAGE_HEADER_SIZE           54
#define IMAGE_COLOR_TABLE_SIZE      1024
#define HEADER_IMAGE_WIDTH_OFFSET   18
#define HEADER_IMAGE_HEIGHT_OFFSET  22
#define HEADER_BIT_DEPTH_OFFSET     28
#define RGB_CHANNEL_SIZE            3



/*===============================================*/
/**             GreyScale Image Handlers           **/
/*===============================================*/
typedef struct{
    int ImageWidth;
    int ImageHeight;
    int BitDepth;
    int FromRGB;
    unsigned char ImageHeader[IMAGE_HEADER_SIZE];
    unsigned char ColorTable[IMAGE_COLOR_TABLE_SIZE];
    unsigned char* ImageBuffer;
} GreyScale_ImageHandler_t;

typedef GreyScale_ImageHandler_t* ImageHandler_Ptr;

// R/W methods
GreyScale_ImageHandler_t GreyScale_Image_Handler_Read_Image(char* ImageName, char* ImagePath);
void GreyScale_Image_Handler_Write_Image(GreyScale_ImageHandler_t anImage, char* ImageName, char* ImagePath);

/*===============================================*/
/*===============================================*/


/*===============================================*/
/**              RGB Image Handlers             **/
/*===============================================*/

typedef enum {
    RED,
    GREEN,
    BLUE
} RGB_channels;


typedef struct{
    int ImageWidth;
    int ImageHeight;
    int BitDepth;
    unsigned char ImageHeader[IMAGE_HEADER_SIZE];
    unsigned char ColorTable[IMAGE_COLOR_TABLE_SIZE];
    unsigned char ** ImageBuffer;
} RGB_ImageHandler_t;

typedef RGB_ImageHandler_t* RGB_ImageHandler_Ptr;

// R/W methods
RGB_ImageHandler_t RGB_Image_Handler_Read_Image(char* ImageName, char* ImagePath);
void RGB_Image_Handler_Write_Image(RGB_ImageHandler_t anImage, char* ImageName, char* ImagePath);

/*===============================================*/
/*===============================================*/


/*===============================================*/
/**              Binary Image Handlers          **/
/*===============================================*/

typedef struct{
    int image_thres;
    int ImageWidth;
    int ImageHeight;
    int BitDepth;
    int FromRGB;
    unsigned char ImageHeader[IMAGE_HEADER_SIZE];
    unsigned char ColorTable[IMAGE_COLOR_TABLE_SIZE];
    unsigned char * ImageBuffer;
} Binary_ImageHandler_t;
typedef Binary_ImageHandler_t* Binary_ImageHandler_Ptr;

Binary_ImageHandler_t Binary_Image_Handler_Read_Image_From_Greyscale(char* ImageName, char* ImagePath, int thres);
Binary_ImageHandler_t Binary_Image_Handler_Read_Image_From_RGB(char* ImageName, char* ImagePath, int thres);
void Binary_Image_Handler_Write_Image(Binary_ImageHandler_t anImage, char* ImageName, char* ImagePath);

/*===============================================*/
/*===============================================*/



/*===============================================*/
/**           Image Conversion Methods          **/
/*===============================================*/


GreyScale_ImageHandler_t Image_Convt_RGB_to_GreyScale(RGB_ImageHandler_t rgb_image);
Binary_ImageHandler_t Image_Convt_RGB_to_Binary(RGB_ImageHandler_t rgb_image, int thres);


#endif      //IMAGE_HANDLER_H