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
    return system("51kern.exe");
}
