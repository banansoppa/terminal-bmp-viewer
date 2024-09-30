#include <stdio.h>
#include <stdbool.h>
int secrethexfunc(char* chararray, int position, int amount) {
    int result = 0;
    for(int i = 0; i < amount; i++) {
        result = result + (0x1<<(i*4))*(chararray[position+i]&0x000000FF);
//        printf("-%x %d, %d-", result, i, 0x1<<(i*4));
    }
    return result;
}
int main(int argc, char** argv) {
    char* filename = argc>1 ? argv[1] : "test3.bmp";
    FILE* file = fopen(filename, "r");
    char data[1000000];
    int amount = fread(data, 1, 1000000, file);
    unsigned int pixelarrayoffset = secrethexfunc(data, 0xA, 4);
    unsigned int width = secrethexfunc(data, 0x12, 4);
    unsigned int height = secrethexfunc(data, 0x16, 4);
    printf("file: %s pixelarray: %x width: %d height: %d amount: %d\n", filename, pixelarrayoffset, width, height, amount);
    int j = 0;
    int output[height][width][3];
    int w_index = 0;
    int h_index = 0;
    for(int i = pixelarrayoffset; i < amount; i+=3) {
        j++;
        //printf("\033[38;2;%d;%d;%d;48;2;%d;%d;%dm  \033[0m", data[i+2]&0x000000FF, data[i+1]&0x000000FF, data[i]&0x000000FF, data[i+2]&0x000000FF, data[i+1]&0x000000FF, data[i]&0x000000FF);
        output[h_index][w_index][0] = data[i+2]&0x000000FF;
        output[h_index][w_index][1] = data[i+1]&0x000000FF;
        output[h_index][w_index][2] = data[i]&0x000000FF;
        if(j%width==0) {
            while((i+1)%4!=0) {
                i++;
            }
        }
        w_index++;
        if(w_index==width) {
            w_index = 0;
            h_index++;
        }
    }
    for(int y = height-1; y >= 0; y--) {
        for(int x = 0; x < width; x++) {
            printf("\033[38;2;%d;%d;%d;48;2;%d;%d;%dm  \033[0m", output[y][x][0], output[y][x][1], output[y][x][2], output[y][x][0], output[y][x][1], output[y][x][2]);
        }
        printf("\n");
    }
    printf("\n");
    fclose(file);
    return 0;
}
