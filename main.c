#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <time.h>

#define HEIGHT 50 
#define WIDTH  50

void setColor(HANDLE console, int color){
    //Color: 0:RESET 1:RED 2:GREEN 3:BLUE 4:BLUE_FG 5:BLACK

    int color_code = 0;

    if      (color == 0) color_code = 7 + 0*16;//RESET
    else if (color == 1) color_code = 4 + 4*16;//RED 
    else if (color == 2) color_code = 2 + 2*16;//GREEN
    else if (color == 3) color_code = 1 + 1*16;//BLUE
    else if (color == 4) color_code = 9 + 0*16;//BLUE_FG
    else if (color == 5) color_code = 0 + 0*16;//BLACK

    SetConsoleTextAttribute(console, color_code);
}

void clearGrid(char * grid, char c){
    int x, y;
    for (y = 0; y < HEIGHT; y++){
        for (x = 0; x < WIDTH; x++){
            grid[WIDTH * y + x] = c;
        }
    }
}

void renderGrid(char * grid, HANDLE console){
    int x, y, i;
    for (y = HEIGHT - 1; y >= 0; y--){
        if (y % 2 == 0) setColor(console, 0);
        else            setColor(console, 4);

        if (y < 0)  printf("0");
        if (y < 10) printf("0");
        if (y >= 0) printf("%d", y);

        setColor(console, 3);
        printf("##");

        for (x = 0; x < WIDTH; x++){
            for (i = 0; i < 2; i++){
                switch(grid[WIDTH * y + x]){
                    case '#':
                        setColor(console, 3);
                        break;
                    case '.':
                        setColor(console, 5);
                        break;
                    case '0':
                        setColor(console, 1);
                        break;
                    case '1':
                        setColor(console, 2);
                        break;
                }
                printf("%c", grid[WIDTH * y + x]);
                setColor(console, 5);
            }
        }
        printf("\n");
    }

    setColor(console, 5);
    printf("  ");

    setColor(console, 3);
    for (x = 0; x <= WIDTH; x++){
        printf("##");
    }

    setColor(console, 5);
    printf("\n    ");

    for (x = 0; x < WIDTH; x++){
        if (x % 2 == 0) setColor(console, 0);
        else            setColor(console, 4);

        if(x < 10) printf("0");
        printf("%d", x);
    }
    printf("\n");
}

void drawInputs(char * grid, int * inputs, int input_size){
    int i, x, y, c;
    char fill;
    for (i = 0; i < input_size; i++){
        x = inputs[3 * i + 0];
        y = inputs[3 * i + 1];
        c = inputs[3 * i + 2];

        grid[WIDTH * y + x] = c + '0';
    }
}

int main(){

    return 0;
}