#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

void app_main(void)
{
    for(;;){
        printf("Hello from app_main!\n");
        sleep(1);
    }
}
