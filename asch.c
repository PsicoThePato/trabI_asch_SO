#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include "asch.h"
#include "linkedList.h"

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
    if (path == NULL)
        printf("CD sem argumentos, nada será feito");
    return -1;
    int result = chdir(path);
    return result;
}

int trata_comando_foreground(char *comando)
{
    char *command_token;
    const char delimiter[] = " ";
    command_token = strtok(comando, delimiter);

    if (strcmp(command_token, "cd") == 0)
    {
        command_token = strtok(NULL, delimiter);
        muda_diretorio(command_token);
        return 0;
    }

    else if (strcmp(command_token, "exit") == 0)
        exit(1);

    else
    {
        return 1;
    }
}

int trata_comando_background(char *comando)
{
    char *argv[8];
    int cd_flag = 0;
    char *command_token;
    char *argv_token;
    const char delimiter[] = "<3";
    // List* parsed_commands_list;
    // List* parsed_command;
    // if(newList(parsed_commands_list, sizeof(List*), NULL))
    // if(newList(parsed_command, sizeof(char*), NULL))
    
    char** commands;
    char *end_str;
    command_token = strtok_r(comando, delimiter, &end_str);
    for(int i = 0; command_token != NULL; i++)
    {   
        commands = realloc(commands, sizeof(char*) * i+1);
        commands[i] = strdup(command_token);
        printf("INFO ---- %s\n", commands[i]);
        command_token = strtok_r(NULL, delimiter, &end_str);
    }
    

}

int main()
{
    /// Define tratamento de sinais ///
    struct sigaction tratador_sinais = {.sa_handler = trata_sinal};
    sigemptyset(&tratador_sinais.sa_mask);
    sigaddset(&tratador_sinais.sa_mask, SIGINT);
    sigaddset(&tratador_sinais.sa_mask, SIGQUIT);
    sigaddset(&tratador_sinais.sa_mask, SIGTSTP);
    sigaction(SIGINT, &tratador_sinais, NULL);
    sigaction(SIGQUIT, &tratador_sinais, NULL);
    sigaction(SIGTSTP, &tratador_sinais, NULL);
    pid_t pid_gerenciador;
    ////
    printf(">>>>  ");
    char comando[1000];
    char comando_cpy[1000];
    fgets(comando, 1000, stdin);
    strcpy(comando_cpy, comando);
    // int background_flag = trata_comando_foreground(comando);

    // if (background_flag)
    //     pid_gerenciador = fork();
    
    // if (pid_gerenciador == 0)
    // {   
    //     //setsid();
    //     trata_comando_background(comando_cpy);
    // }

    // if (pid_gerenciador > 0)
    // {
    //     waitpid(pid_gerenciador, NULL, WNOHANG);
    // }

    trata_comando_background(comando_cpy);
}