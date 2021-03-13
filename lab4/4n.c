#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <string.h>

int strlenght(char* s){
    int i = 0;
    while(s[i] != '\0'){
        i = i + 1;
    }
    return i;
}

int main()
{

    char *firstchildname = malloc(FILENAME_MAX * sizeof(char));
    char *secondchildname = malloc(FILENAME_MAX * sizeof(char));

    int id2 = 7;
    int statch = 0;
    int long0 = sizeof(FILENAME_MAX);

    int fd = open("mmapfile", O_RDWR | O_TRUNC | O_CREAT, 0777);
    if (fd < 0)
    {
        printf("open error\n");
        return 0;
    }

    int pip[2];
    pipe(pip);

    int id1 = fork();
    if (id1 == -1)
    { //error
        perror("Bad fork1\n");
    }

    if (id1 > 0)
    { //parent only
        id2 = fork();
        if (id2 == -1)
        { //error
            perror("Bad fork2\n");
        }
    }

    if ((id2 == 0) || (id1 == 0))
    { //child 1 and 2 only
        printf("У аппарата %d id1 = %d  id2 = %d \n", getpid(), id1, id2);
        char c;



            read(pip[0], &c, sizeof(char));
           printf("Went %d id1 = %d  id2 = %d \n", getpid(), id1, id2);
           if (execl("child", "mmapfile", NULL) == -1)
                {
                    printf("execl1 error %d\n", errno);
                    return 1;
                }
       
    }

    if ((id1 > 0) && (id2 > 0))
    {
        //printf("Parent office entered mypid:%d id1 %d id2 %d \n", getpid(), id1, id2);

        char c = '1', n = '\n';
       
        printf("Enter the first filename\n");
        scanf("%s", firstchildname);
        printf("Entered %s\n", firstchildname);
        write(fd, firstchildname, strlenght(firstchildname));
        
        printf("\n size == %d\n", strlenght(firstchildname));
        write(fd, &n, sizeof(char));
       
        close(fd);
        write(pip[1], &c, sizeof(char));
        if(waitpid(-1, &statch, 0) < 0){
            printf("Wait1\n");
            return 0;        
        }
        fd = open("mmapfile", O_WRONLY | O_TRUNC| O_CREAT, 0777);
        printf("Enter the second filename\n");

       

        scanf("%s", secondchildname);
        printf("Entered %s, size s == %ld\n", secondchildname, strlenght(secondchildname));
        write(fd, secondchildname, strlenght(secondchildname));
        write(fd, "\n", sizeof(char));
        //write(1, &secondchildname, sizeof(secondchildname));
        
        write(pip[1], &c, sizeof(char));
        if(waitpid(-1, &statch, 0) < 0){
            printf("Wait2\n");
            return 0;        
        }

        close(pip[0]);
        close(pip[1]);

        close(fd);
    }

    free(secondchildname);
    free(firstchildname);
    printf("PARENT:mypid = %d ch1_id = %d ch2_id = %d ENDED\n", getpid(), id1, id2);

    return 0;
}