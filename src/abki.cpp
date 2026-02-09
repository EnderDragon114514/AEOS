/*
AEOS v5.11.2 Boot and kernel initilazer
Release date:26/2/7
*/
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>
int main()
{
    printf("Probing driver...\n");
    system("ctmouse.exe");
    printf("Initilazing kernel...\n");
    FILE* f=fopen("51kern.exe","r");
    if(!f)
    {
        printf("[CRITICAL ERROR]Operating system kernel not found!\n");
        printf("System halted\n");
        while(1)
        {
            getch();
        }
    }
    fclose(f);
    while(1)
    {
        int ret=system("51kern.exe");
        printf("%d",ret);
        if(ret==23)
        {
            printf("Kernel brodcasted SIGACST,Shutting down...\n");
            system("shutdown s");
        }
        else if(ret==27)
        {
            printf("Kernel brodcasted SIGACRB,Rebooting...\n");
            system("shutdown r");
        }
        else if(ret!=1)
        {
            printf("[FATAL ERROR]Kernel panic!\n");
            printf("Press Ctrl-Alt-Delete to reboot\n");
            system("shutdown r");
        }
    }
}
