#include <stdio.h>
#include "lib.h"

//#define logir
int * qs(int* arr, int first, int last)
{
    #ifdef logir
    printf("qs from lib2.c\n");
    #endif
    int i = first, j = last, x = arr[(first + last) / 2], buf;
    do {
        while (arr[i] < x) i++;
        while (arr[j] > x) j--;
  
        if(i <= j) {
            if (arr[i] > arr[j]){
                buf = arr[i];
                arr[i] = arr[j];
                arr[j] = buf;
            }
            i++;
            j--;
        }
    } while (i <= j);
  
    if (i < last)
        qs(arr, i, last);
    if (first < j)
        qs(arr, first, j);
        return arr;
}
////////////////////////////////////////////////////////////////////////////////////////////
int * sort(int* arr, int size){//hoar
#ifdef logir
    printf("Hoarosort from lib2.c\n");
    #endif
    int first = 0;//arr[0];
    int last = size-1;//arr[size-1];
    #ifdef logir
    printf("%d %d f l\n", first, last);
    #endif
    return qs(arr, first, last);
}

float pi(int n){ //valis
#ifdef logir
    printf("Libvalis\n");
    #endif
    float res = 1;
    for(int i = 1; i < n; i++){
    res *= (float)(4.0*i*i)/((2.0*i-1)*(2.0*i+1));
    }
    res *= 2.0;
    #ifdef logir
        printf("%f\n", res);
        #endif
    return res;
}