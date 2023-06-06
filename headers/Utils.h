#ifndef UTILS_H
#define UTILS_H

void CopyFloatArrayToFile(float* arr, int size, char* fileName);
void NormalizeFloatArr(float** arr, int size, float trunc_val);
#endif