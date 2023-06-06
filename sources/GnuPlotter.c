#include "../headers/GnuPlotter.h"

void Gnu_plot_from_file(char* fileName, char* xLabel, char* yLabel, char* title, char* output, uint8_t include_lines){
    FILE* gnu = popen("gnuplot -persistent", "w");

    if(!gnu){
        printf("GNU plotter failed to open\n");
        exit(1);
    }

    if(yLabel)
        fprintf(gnu, "set ylabel \"%s\"\n", yLabel);
    if(xLabel)
        fprintf(gnu, "set xlabel \"%s\"\n", xLabel);

    if(title)
        fprintf(gnu, "set title \"%s\"\n", title);

    if(output)
        fprintf(gnu, "set output \"%s\"\n", output);
    // Include lines
    if(include_lines)
        fprintf(gnu, "plot \"%s\" with impulse\n", fileName);
    else{
        fprintf(gnu, "plot \"%s\"\n", fileName);
    }

    fprintf(gnu, "exit\n");

    // Close the file
    pclose(gnu);
}


void Gnu_plot_from_function(char* function, char* xLabel, char* yLabel, char* title, char* output, uint8_t include_lines){
    FILE* gnu = popen("gnuplot -persistent", "w");

    if(!gnu){
        printf("GNU plotter failed to open\n");
        exit(1);
    }

    if(yLabel)
        fprintf(gnu, "set ylabel \"%s\"\n", yLabel);
    if(xLabel)
        fprintf(gnu, "set xlabel \"%s\"\n", xLabel);
    
    if(title)
        fprintf(gnu, "set title \"%s\"\n", title);
    

    if(output)
        fprintf(gnu, "set output \"%s\"\n", output);
    // Include lines
    if(include_lines)
        fprintf(gnu, "plot  %s with impulse\n", function);
    else{
        fprintf(gnu, "plot %s\n", function);
    }

    fprintf(gnu, "exit\n");

    // Close the file
    pclose(gnu);
}