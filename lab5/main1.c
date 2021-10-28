#include <stdio.h>
#include <stdlib.h>

#include "lib.h"

int main()
{

    char ent = 0;

    while (1)
    {
        printf("Enter the meaning1\n\t1 for pi\n\t2 for sort\n\t0 for exit\n");
    scan:
        scanf("%c", &ent);
        switch (ent)
        {
        case '1':
        {
            int k = 0;
            printf("Enter arg for pifunc:\n");
            scanf("%d", &k);
            printf("pi(%d):\tpi = %f\n", k, pi(k));
            break;
        }
        case 10:
        {
            goto scan;
        }
        case '2':
        {
            int k = 0;
            printf("Enter args for sort:\tsize, arg1,...,argn\n");
            scanf("%d", &k);
            int *arr = malloc(sizeof(int) * k);
            for (int i = 0; i < k; i++)
            {
                scanf("%d", &arr[i]);
            }
            arr = sort(arr, k);
            printf("sorted array = {\n");
            for (int i = 0; i < k; i++)
            {
                printf("%d, ", arr[i]);
            }
            printf("}\n");

            free(arr);
            break;
        }
        default:
        {
            printf("Ending...\n");
            return 0;
        }
        break;
        }

    } //while

    return 0;
}