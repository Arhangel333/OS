#include <stdio.h>
#include "lib.h"

//#define logir

float pi (int n){ //leibnic
#ifdef logir
    printf("Leibnic\n");
    #endif
    float res = 0;
    float one = 1;
    for(int i = 0; i < n; i++){
    res += (one)/(2*i+1);
    one *= -1;
    }
    res = (float)4 * res;
    return res;
}

int* sort (int *arr, int size){ //bouble
    int buf;
    #ifdef logir
    printf("Boublesort from lib1.c\n");
    #endif
    for(int i = 1; i< size; i++){
        for (int j = 0; j < size - i; j++)
        {
            if(arr[j] > arr[j+1]){
                buf = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = buf;
            }
        }
    }
    return arr;
}

