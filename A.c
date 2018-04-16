#include <stdio.h>
#include <unistd.h>



unsigned int snooze(unsigned int secs){
unsigned int rc = sleep(secs);

printf("SLEEP FOR %u of %u secs.\n", secs - rc, secs);
    return rc;


}




int main (int argc, char** argv){ 
printf("AAA\n");
for(int i = 1; i < argc; i++){

 printf("args %d: %s\n",i, argv[i]);
}

unsigned int s = 5;

snooze(s);
printf("BBB\n"); 
return 0;

}

