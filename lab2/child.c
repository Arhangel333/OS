#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <ctype.h>


int my_close(int fd){
    if(close(fd) == -1){
        perror("my_Close error \n");
        exit(1);
    }
    return 0;
}

int glassn(char c){
    tolower(c);
    if((c == 'a')|(c == 'e')|(c == 'i')|(c == 'j')|(c == 'o')|(c == 'u')|(c == 'y')/* |(c == 'а')|(c == 'е')|(c == 'ё')|(c == 'и')|(c == 'о')|(c == 'у')|(c == 'ы')|(c == 'э')|(c == 'ю')|(c == 'я') */){
        return 1;
    }else{
        return 0;
    }
}

char* foo(char s[], char* filename){
    int fdd;
    if((fdd = creat(filename, 00777)) == -1){
    perror("Open error\n");
}

    int i = 0;
    while(s[i] != '\0'){
        if (!glassn(s[i]))
        {
            write(fdd, &s[i], sizeof(s[i]));
            printf("%c ", s[i]);
        }
        i++;
    }
    my_close(fdd);
}


int main(int argc, char* argv[]){

//printf("It is a child, argc = %d\n", argc);
int stop = 0;
if(argc < 1){
    printf("Too few args\n");
    exit(1);
}

char* filename = argv[0];
int n = 4000;
char* msg = malloc(n*sizeof(char));

printf("Enter the messege to print into %s \n", argv[0]);
fgets(msg, n, stdin);
fflush(stdin);

foo(msg, filename);

free(msg);
printf("CHILD: mypid:%d  I ended.\n", getpid());

}
