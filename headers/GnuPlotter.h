#ifndef GNU_PLOT_H
#define GNU_PLOT_H

#include <stdlib.h>
#include <stdio.h>


void Gnu_plot_from_file(char* fileName, char* xLabel, char* yLabel, char* title, char* output, uint8_t include_lines);
void Gnu_plot_from_function(char* function, char* xLabel, char* yLabel, char* title, char* output, uint8_t include_lines);


#endif      // GNU_PLOT_H