#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
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
        break;
    case SIGTSTP:
        caractere_chamada = 'Z';
        break;  
    default:
        printf("%d", sinal);
        break;
    }

    printf("\nNão adianta me enviar o sinal por Ctrl-%c. Estou vacinado!!\n", caractere_chamada);
}


int muda_diretorio(char *path)
{

    int result = chdir(path);
    return result;
}


int trata_comando()
{
    char comando[1000];
    fgets(comando, 1000, stdin);
    char* tokenized_command[8];
    int cd_flag = 0 ;
    char* token;
    const char delimiter[] = " ";
    
    token = strtok(comando, delimiter);
    int count = 0;
    while(token != NULL)
    {
        tokenized_command[count] = malloc(sizeof(char) * strlen(token));
        strcpy(tokenized_command[count], token);
        count++;
        token = strtok(NULL, delimiter);
    }

    int n_tokens = count; // reference sugar

    for(int i = 0; i < n_tokens; i++)
    {
        printf("%s ", tokenized_command[i]);

    }
    
    int flag;
    if(strcmp(tokenized_command[0], "cd") == 0)
    {
        flag = chdir(tokenized_command[1]);
        printf("%s.", tokenized_command[1]);
        printf("INFO ---- chdir result: %d\n", flag);
        system("ls");
    }
}



int main()
{   
    struct sigaction tratador_sinais = {.sa_handler = trata_sinal};
    sigemptyset(&tratador_sinais.sa_mask);  
    sigaddset(&tratador_sinais.sa_mask, SIGINT);
    sigaddset(&tratador_sinais.sa_mask, SIGQUIT);
    sigaddset(&tratador_sinais.sa_mask, SIGTSTP);
    sigaction(SIGINT, &tratador_sinais, NULL);
    sigaction(SIGQUIT, &tratador_sinais, NULL);
    sigaction(SIGTSTP, &tratador_sinais, NULL);
    printf(">>>>  ");
    trata_comando(); //forkar aqui
}