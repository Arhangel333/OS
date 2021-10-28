#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>


//'ne ghjuhfvve kexit gjrfxnj yt pfgecrfnm yf ye;yjv rjvgm.ntht

int main()
{

    char firstchildname[256], secondchildname[256];


    int id2 = 7;
    int statch;
    int last = 0;

    int p1[2], p2[2], numb[2];
    pipe(p1);
    pipe(p2);
    pipe(numb);

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
        int r;
        read(numb[0], &r, sizeof(r));
        //printf("cool news\n");
        if (r == 1)
        {
                //printf("PRE EXECUTE 1 mypid:%d id1 %d id2 %d\n", getpid(), id1, id2);
                char file[256];

                    read(p1[0], &file, sizeof(file));
            if(execl("child", file, NULL) == -1){
                printf("execl1 error %d\n", errno);
                return 1;
            }
            //printf("exec exitAAAAAAAAAAAAAAA\n");
        }
        else
        {
                //printf("PRE EXECUTE 2 mypid:%d id1 %d id2 %d\n", getpid(), id1, id2);
            char file[256];
                read(p2[0], &file, sizeof(file));
            if(execl("child", file, NULL) == -1){
                printf("execl2 error %d\n", errno);
                return 1;
            }
            //printf("exec exit\n");
        }
        //printf("What?\n\n");
    }


    if((id1 > 0)&&(id2 > 0)){
        //printf("Parent office entered mypid:%d id1 %d id2 %d \n", getpid(), id1, id2);
        
        printf("Enter the first filename\n");
        scanf("%s", &firstchildname);

        printf("Enter the second filename\n");
        scanf("%s", &secondchildname);

        srand(time(NULL));
        int r = rand() % 100;
        if (r <= 80)
        {
            r = 1;
            //printf("first pipe r = %d\n", r);
            write(numb[1], &r, sizeof(r));
            write(p1[1], &firstchildname, sizeof(firstchildname));
        }else{
            r = 2;
            //printf("second piper = %d\n", r);
            write(numb[1], &r, sizeof(r));
            write(p2[1], &firstchildname, sizeof(firstchildname));
        }


        srand(time(NULL));
        r = rand() % 100;

        if (r <= 80)
        {
            r = 1;
            //printf("first pipe r = %d\n", r);
            write(numb[1], &r, sizeof(r));
            write(p1[1], &secondchildname, sizeof(secondchildname));
        }else{
            
            r = 2;
            //printf("second piper = %d\n", r);
            write(numb[1], &r, sizeof(r));
            write(p2[1], &secondchildname, sizeof(secondchildname));
        }

        waitpid(id1, &statch, 0);
        waitpid(id2, &statch, 0);
        close(p1[0]);
        close(p1[1]);
        close(p2[0]);
        close(p2[1]);
    }
    
    printf("PARENT:mypid = %d ch1_id = %d ch2_id = %d ENDED\n", getpid(), id1, id2);

    return 0;
}
