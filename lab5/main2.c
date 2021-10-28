#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <stdint.h>

//#include "lib.h"

void *lib_handle = NULL;
int lib_mumb = 1;

void *switchlib(void *handle)
{
    if(handle)
    dlclose(handle);
    if (lib_mumb == 2)
    {
        //printf("./libfirst.so\n");
        lib_handle = dlopen("./libfirst.so", RTLD_LAZY);
        lib_mumb = 1;
    }
    else
    {
        //printf("./libsecond.so\n");
        lib_handle = dlopen("./libsecond.so", RTLD_LAZY);
        lib_mumb = 2;
    }
    /* if (lib_handle != 0)
    {
        printf("dlopenerror\n");
        exit(0);
    } */
    //printf("%p\n",lib_handle);
    char* error;
    if ((error = dlerror()) != NULL)  {
        fprintf (stderr, "%smm\n", error);
        exit(1);
    }
    
    return lib_handle;
}

int main()
{
    lib_handle = dlopen("./libfirst.so", RTLD_LAZY);
    //printf("%p\n",lib_handle);
    int type = 0;

    float (*pi)(int n);
    int *(*sort)(int *arr, int size);

    while (printf("Enter functype:\n") && scanf("%d", &type) > 0)
    {
        switch (type)
        {
        case 0:
        {
            lib_handle = switchlib(lib_handle);
            break;
        }
        case 1:
        {
            pi = dlsym(lib_handle, "pi");
            int k = 0;
            printf("Enter arg for pifunc:\n");
            scanf("%d", &k);
            printf("pi(%d):\tpi = %f\n", k, pi(k));
            break;
        }
        case 2:
        {
            sort = dlsym(lib_handle, "sort");
            int k = 0;
            printf("Enter args for sort:\tsize, arg1,...,argn\n");
            scanf("%d", &k);
            int *arr = malloc(sizeof(int) * k);
            for (int i = 0; i < k; i++)
            {
                scanf("%d", &arr[i]);
            }
            
            arr = sort(arr, k);
            printf("sorted array = \n{ ");
            for (int i = 0; i < k; i++)
            {
                printf("%d, ", arr[i]);
            }
            printf("}\n");

            free(arr);
            break;
        }
        default:
            return 0;
        }
    }
}