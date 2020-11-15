#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <signal.h>
#include <unistd.h>
#include "asch.h"

void trata_sinal(int sinal)
{
    char caractere_chamada;
    switch (sinal)
    {
    case SIGINT:
        caractere_chamada = 'C';
        break;
    case SIGQUIT:
        caractere_chamada = '\\';
        printf("aaa");
    case SIGTSTP:
        caractere_chamada = 'Z';
    default:
        printf("%d", sinal);
        break;
    }

    printf("\nNão adianta me enviar o sinal por Ctrl-%c. Estou vacinado!!\n", caractere_chamada);
}


int main()
{   
    char signal;
    struct sigaction tratador_sinais = {.sa_handler = trata_sinal};
    sigemptyset(&tratador_sinais.sa_mask);
    sigaddset(&tratador_sinais.sa_mask, SIGINT);
    sigaddset(&tratador_sinais.sa_mask, SIGQUIT);
    sigaddset(&tratador_sinais.sa_mask, SIGTSTP);
    sigaction(SIGINT, &tratador_sinais, NULL);
    sigaction(SIGQUIT, &tratador_sinais, NULL);
    sigaction(SIGTSTP, &tratador_sinais, NULL);
    printf(">>>>  ");
    scanf("%c", &signal);
}