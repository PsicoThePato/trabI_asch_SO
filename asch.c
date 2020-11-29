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
    {
        printf("CD sem argumentos, nada será feito\n");
        return -1;
    }
    printf("INFO ---- PATH: [%s]\n", path);
    int result = chdir(path);
    if(result != 0) perror("chdir error");
    return result;
}

int trata_comando_foreground(char *comando)
{
    char *command_token;
    const char delimiter[] = " ";
    command_token = strtok(comando, delimiter);

    if (strcmp(command_token, "cd") == 0)
    {
        char cwd[1000];
        command_token = strtok(NULL, delimiter);
        int result = muda_diretorio(command_token);
        getcwd(cwd, sizeof(cwd));
        printf("INFO ---- %d PATH: %s\n", result, cwd);
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
    char *command_token;
    // List* parsed_commands_list;
    // List* parsed_command;
    // if(newList(parsed_commands_list, sizeof(List*), NULL))
    // if(newList(parsed_command, sizeof(char*), NULL))
    
    char* commands[5];
    char *end_str;
    int n_process = 0;
    command_token = strtok_r(comando, "<3", &end_str);
    while(command_token != NULL)
    {   
        commands[n_process] = strdup(command_token);
        command_token = strtok_r(NULL, "<3", &end_str);
        n_process++;
    }
   //acima funciona
    char* argv[5];
    char *argv_token;
    int argc = 0;
    for(int i = 0; i < n_process; i++)
    {
        argc = 0;
        argv_token = strtok(commands[i], " ");
        while(argv_token != NULL)
        {
            argv[argc] = strdup(argv_token);
            printf("INFO ---- ARGV: %s\n", argv[argc]);
            argv_token = strtok(NULL, " ");
            argc++;
        }
        printf("INFO ---- ARGC: %d\n", argc); 
        argv[argc] = NULL;
        pid_t pid = fork();
        if(pid == 0) execvp(argv[0], argv);
        
        else
        {
            for(int j = 0; j < argc; j++)
            {
                free(argv[j]);
            }
            
        }
        
    }    


}

int main()
{
    /// Define tratamento de sinais ///
    // struct sigaction tratador_sinais = {.sa_handler = trata_sinal};
    // sigemptyset(&tratador_sinais.sa_mask);
    // sigaddset(&tratador_sinais.sa_mask, SIGINT);
    // sigaddset(&tratador_sinais.sa_mask, SIGQUIT);
    // sigaddset(&tratador_sinais.sa_mask, SIGTSTP);
    // sigaction(SIGINT, &tratador_sinais, NULL);
    // sigaction(SIGQUIT, &tratador_sinais, NULL);
    // sigaction(SIGTSTP, &tratador_sinais, NULL);
    pid_t pid_gerenciador = 1;
    ////
    while(1)
    {
        printf(">>>>  ");
        char comando[1000];
        char comando_cpy[1000];
        fgets(comando, 1000, stdin);

        // refatorar essa merda numa função
        for (char* p = comando; *p != '\0'; p++)
        {
            if (*p == '\n') 
            {
                *p = '\0';
            }
        }

        strcpy(comando_cpy, comando);
        int background_flag = 0;
        background_flag = trata_comando_foreground(comando);

        if (background_flag)
        {
            pid_gerenciador = fork();
            
            if (pid_gerenciador == 0)
            {   
                setsid();
                trata_comando_background(comando_cpy);
            }

            else if (pid_gerenciador > 0)
            {
                waitpid(pid_gerenciador, NULL, WNOHANG);
            }
        }

    }
}