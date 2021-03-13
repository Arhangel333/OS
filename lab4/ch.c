#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <ctype.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdint.h>
#include <string.h>


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
    perror("Open error in child\n");
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

void work(char * filename){
    int fd = open(filename, O_RDWR | O_CREAT| O_TRUNC, 0777);
if(fd < 0){
    printf("open error\n");
    exit(0);
}



int n = 4000;
char* msg = malloc(n*sizeof(char));

printf("Enter the messege to print into %s \n", filename);
fgets(msg, n, stdin);

foo(msg, filename);

free(msg);
close(fd);

}

int strsize(char* str){
    int i = 0;
    while(str[i] != '\0'){
        //printf("%c %d\n", str[i], i);
        i = i + 1;
        
    }
    return i;
}

char* mmread(void* mmapfile, char* msgfile){
    char fir = 0, n = 'n';
    int p[2], size = 0;
    pipe(p);
    
    sscanf(mmapfile, "%c ", &n);


    /* sscanf(mmapfile, "%d ", &size);
    for (int i = 0; i < size; i++)
    {
        sscanf(mmapfile, "%c ", &n);
        msgfile[i] = n;
    } */
    
    










    while(1){
        if(n == '\n' || n == ' '){
            mmapfile = mmapfile + 1;
            sscanf(mmapfile, "%c ", &n);
        }else{
            break;
        }
    
    }

while((fir != '\n')&&(fir != ' ')&&(fir != EOF)){
    sscanf(mmapfile, "%c ", &fir);
    mmapfile = mmapfile + 1;
    printf("%c%d  %p\n", fir, fir, mmapfile);
    write(p[1], &fir, sizeof(fir));
    size = size +1;
}
size = size - 1;
printf(">>>>>>>>>>>>>>>>size == %d\n", size);
//printf("endwhile %d\n", size);
char *word = malloc(size * sizeof(char));
int i = 0;
for (i; i < size; i++)
{
    read(p[0], &word[i], sizeof(char));
    printf("%c %d\n", word[i], i);
}
free(msgfile);
word[i+1] = '\0';
//printf("%s - word\n", word);

msgfile = word;
printf("%s\n", msgfile);
word = NULL;
//printf("%s - msgfile\n", msgfile);
mmapfile = mmapfile + size;

return msgfile;
}




int main(int argc, char* argv[]){
printf("It is a child, argc = %d\n", argc);
int stop = 0;
if(argc < 1){
    printf("Too few args\n");
    printf("(%s)\n", argv[0]);
    exit(1);
}

printf("argc = %d argv[1] = %s \n", argc, argv[0]);

char* mmfile = argv[0];
int fd = open(mmfile, O_RDWR, 0777);
if(fd < 0){
    printf("open error\n");
    return 0;
}
struct stat fil;
if(fstat(fd, &fil) < 0){
    printf("Fstat failed\n");
    return 0;
}
void* mmapfile = mmap(0, fil.st_size, PROT_READ, MAP_SHARED, fd, 0);




char *msgfile = malloc(FILENAME_MAX * sizeof(char));
    char *second = malloc(FILENAME_MAX * sizeof(char));

msgfile = mmread(mmapfile, msgfile);
printf("%s\n", msgfile);


/* mmapfile = mmapfile + strsize(msgfile);
second = mmread(mmapfile, second); */

//printf("%s, %s, fsize = %ld\n", msgfile, second, sizeof(msgfile)/sizeof(char));
munmap(mmapfile, fil.st_size);
close(fd);
///////////////////////////


work(msgfile);
//work(second);

/////////////////////////


free(msgfile);
free(second);

printf("CHILD: mypid:%d  I ended.\n", getpid());
}


//считывать название ммапа пока не наткнётся на \0