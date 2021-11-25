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

// Perceptron functions

double activation(double x){
    //Sigmoid
    return 1 / (1 + exp(-1 * x));
}

double percThink(double * weights, int weights_size, double bias, int * inputs){
    //inputs = {x,y,category} 
    double res = bias;
    
    int i;
    for (i = 0; i < weights_size; i++){
        res += inputs[i] * weights[i];
    }

    return activation(res);    
}

void percTrain(double * weights, int weights_size, double *bias, double l_rate, int * inputs, int inputs_size){
    int i, j;
    int single_input[3];
    double output;
    for(i = 0; i < inputs_size; i++){
        single_input[0] = inputs[3 * i + 0];
        single_input[1] = inputs[3 * i + 1];
        single_input[2] = inputs[3 * i + 2];

        output = percThink(weights, weights_size, *bias, single_input);

        for (j = 0; j < weights_size; j++){
            weights[j] += (single_input[2] - output) * l_rate * single_input[j];
        }

        *bias += (single_input[2] - output) * l_rate;
    }
}

double function(int x, double * weights, double bias){
    double m = -1 * (bias / weights[1]) / (bias / weights[0]);
    double c = -1 * bias / weights[1];
    
    return m * x + c;
}

void drawFunction(char * grid, double * weights, double bias){
    int x, y;
    for (x = 0; x < WIDTH; x++){
        y = function(x, weights, bias);
        y = floor(y);

        if (y < HEIGHT && y >= 0) grid[WIDTH * y + x] = '#';
    }
}

//Accuracy function

int main(){
char grid[HEIGHT * WIDTH];
    
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    int x, y, category, i;

    srand(time(NULL));

    const int weight_size = 2;
    double weights[weight_size] = {0.6 , 0.4};
    double bias = 0.5;
    double epochs = 50;
    double l_rate = 1;
    
    const int input_size = 4;
    int inputs[input_size * 3] = {10, 10, 0, 20, 20, 1, 5, 5, 0, 30 ,30, 1}; 

    for (i = 0; i < input_size; i++){
        grid[WIDTH * inputs[3 * i + 0] + inputs[3 * i + 1]] = '#';
    }

    clearGrid(grid, '.');

    /*
    printf("Enter input coordinates [x y category(0 or 1)]: ");
    for (i = 0; i < input_size; i++){
        scanf("%d %d %d", &x, &y, &category);
        inputs[3 * i + 0] = x;
        inputs[3 * i + 1] = y;
        inputs[3 * i + 2] = category;

        printf("x:%d, y:%d => %d\n\n", x, y, category);

        grid[WIDTH * y + x] = category + '0';
    }*/


    printf("Weights before training: \n>%f >%lf\n", weights[0], weights[1]);
    printf("Bias: \n>%lf\n", bias);

    printf("\nTraining starts...\n");
    for (i = 1; i <= epochs; i++){
        percTrain(weights, weight_size, &bias, l_rate, inputs, input_size);
    }
    printf("%d Epochs done\n\n", epochs);

    printf("Training completed\n");

    printf("\nTrained weights: \n>%lf >%lf\n", weights[0], weights[1]);
    printf("Bias: \n>%lf\n", bias);

    drawFunction(grid, weights, bias);
    drawInputs(grid, inputs, input_size);

    renderGrid(grid, console);

    setColor(console, 0);
    return 0;
}