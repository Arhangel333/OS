/* 
ускорение(100) = 0.078125/9.046875

эффективность = ускорение/100 = 0,000086




 */




#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <pthread.h>
#include <limits.h>





pthread_mutex_t mutex;

int real_thread_number = 0;

/* static */ unsigned long long int Ssum = 0, Snumb = 0;

int random1 = 0;

double pi(int r,  int n){
    double x, y, pi ,ok = 0;
    for(int i = 0; i < n; i++){
        srand(time(NULL));
        x = rand() % (2*r) - r;
        srand(time(NULL));
        y = rand() %(2*r) - r;
        if(((x*x + y*y) <= r*r)){
           ok++; 
        }
    }
    

    pi = (ok)/(n);
    //printf("%f\n", pi);
    return pi;
}


void* Circle_work(void* number){
    srand(random1 += 4735);
    double radius = rand() % 1000; //держим значение в рамках чтобы уменьшить вероятность выхода за MAX
    
    //printf("This is the thread body №%d \n", *((int*)number));
    if(pthread_mutex_lock(&mutex) != 0){
        perror("Cannot lock mutex\n");
        exit(1);
    }
    double p = pi(radius, 10000);
    double S = p * radius*radius*4;
    //printf("This is the thread body №%d\n", *((int*)number));
    

    real_thread_number += 1;
    Snumb += 1;
    Ssum += S;


    if(pthread_mutex_unlock(&mutex) != 0){
        perror("Cannot unlock mutex\n");
        exit(1);
    }
    printf("This is the end of the thread body №%d r = %f Square = %f\n",  real_thread_number, radius, S);
    return number;
}




int main(int argc, char** argv){

time_t start, end;
start = clock();

if(argc != 2){
    printf("Usage: ./myprog <thread limit>\nBut now is: argc = %d\n", argc);
    exit(1);
}

srand(time(NULL));
random1 = rand() % 1000000;

int thread_count = atoi(argv[1]);

pthread_t thread[thread_count];
if(pthread_mutex_init(&mutex, NULL) != 0){
        perror("Cannot init mutex\n");
        exit(1);
    }

for (int i = 0; i < thread_count; i++)
{
    if(pthread_create(&thread[i], NULL, Circle_work, &i) != 0){
        perror("Cannot create thread\n");
        exit(1);
    }
}

for (int i = 0; i < thread_count; i++)
{
    if(pthread_join(thread[i], NULL) != 0){
        perror("Cannot join\n");
        exit(1);
    }
}

pthread_mutex_destroy(&mutex);
end = clock();
printf("Площадь окружности в среднем: %f\n Время: %lf\n", (double)(Ssum)/(double)Snumb, difftime(end,start)/CLOCKS_PER_SEC);



return 0;
}
