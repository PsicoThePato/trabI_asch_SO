#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strUtils.h"


int cria_vetor_comandos(char *comando, char**commands)
{
    char* end_str;
    int n_process = 0;
    char* command_token = strtok_r(comando, "<3", &end_str);
    while(command_token != NULL)
    {   
        commands[n_process] = strdup(command_token);
        printf("cria_vetor_comandos(): [%s]\n", commands[n_process]);
        command_token = strtok_r(NULL, "<3", &end_str);
        n_process++;
    }
    return n_process;
}


int get_tokenized_command(char **commands)
{
    char comando[1000] = {'\0'};
    fgets(comando, 1000, stdin);
    if(strlen(comando) == 0)
    {
        printf("Problema na leitura do comando, tente de novo!\n");
        return -1;
    }
    for (char* p = comando; *p != '\0'; p++)
    {
        if (*p == '\n') 
        {
            *p = '\0';
            break;
        }
        if(*p == '\0')
        {
            printf("Problema na leitura do comando, tente de novo!\n");
            return -1;
        }
    }
    int n_comandos = cria_vetor_comandos(comando, commands);
    return n_comandos;
}


int cria_argv(char *comando, char** argv)
{
    char* token = strtok(comando, " ");
    int argc = 0;
    while(token != NULL)
    {
        argv[argc] = strdup(token);
        printf("cria_argv() : argv: [%s]\n", argv[argc]);
        argc++;
        token = strtok(NULL, " ");
    }
    return argc;
}