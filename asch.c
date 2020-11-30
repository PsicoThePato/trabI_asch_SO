#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <assert.h>

#include "asch.h"
#include "linkedList.h"


List* GPID_LIST;
int FG_PROCESS_FLAG = 0;
pid_t FG_PROCESS_PID;


void trata_sinal(int sinal)
{
    if(FG_PROCESS_FLAG)
    {
        kill(FG_PROCESS_FLAG, SIGKILL);
    }
    else
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


int trata_comando_foreground(char **comando, int n_commands)
{
    printf("running foreground pid: %d\n", getpid());
    if(n_commands < 0)
    {
        return 0;
    }
    if(n_commands == 1)
    {
        printf("INFO ---- comando unitário\n");
        char command[strlen(comando[0]) + 1];
        strcpy(command, comando[0]);
        char* argv[5];
        int argc = cria_argv(command, argv);
        if (strcmp(argv[0], "cd") == 0)
        {
            printf("sorvetinho\n");
            char cwd[1000];
            int result = muda_diretorio(argv[1]);
            getcwd(cwd, sizeof(cwd));
            printf("foreground : %d PATH: %s\n", result, cwd);
            return 0;
        }
        else if (strcmp(argv[0], "exit") == 0)
        {
            printf("Entrando no bloco de exit.\n");
            Node* node = GPID_LIST->head;
            while(node)
            {
                kill(-1 * (*(int*)(GPID_LIST->head->data)), SIGKILL);
                node = node->next;
            }
            destroyList(GPID_LIST);
            printf("DESTRUÍ A LISTA E TENHO O PID %d\n", getpid());
            kill(getpid(), SIGKILL);
            printf("deu ruim no exit\n");
        }
        else if(strcmp(argv[argc - 1], "%") == 0)
        {

            printf("teste\n");
            argc--;
            argv[argc] = NULL;
            pid_t pid = fork();
            if(pid == 0)
            {
                execvp(argv[0], argv);
                perror("Deu ruim!");
                return 0; // failsafe
            }
            else
            {
                FG_PROCESS_PID = pid;
                FG_PROCESS_FLAG = 1;
                waitpid(pid, NULL, 0);
                FG_PROCESS_FLAG = 0;
                for(int i = 0; i < argc; i++)
                {
                    free(argv[i]);
                }
                printf("POC\n");
                return 0;
            }

        }

        else // comando não termina em '%'
        {
            printf("será tratado em background\n");
            return 1;
        }
    }
    
    else // mais de um comando
    {
        printf("será tratado em background\n");
        return 1;
    }
}


int trata_comando_background(char **commands, int n_comandos)
{
    printf("running backgroud pid: %d\n", getpid());
    
    if(n_comandos == 1)
    {
        struct sigaction ignora = {.sa_handler = SIG_IGN};
        sigemptyset(&ignora.sa_mask);
        sigaction(SIGUSR1, &ignora, NULL);
    }
    kill(getpid(), SIGUSR1);
    printf("sou imorrível\n");
    char* argv[6];
    int argc = 0;
    pid_t pid;
    for(int i = 0; i < n_comandos; i++)
    {

        argc = cria_argv(commands[i], argv);
        for(int k = 0; k < argc; k++)
        {
            printf("background : ARGV: %s\n", argv[k]);
        }
        printf("INFO --- ARGC: %d\n", argc); 
        argv[argc] = "&";
        argv[argc+1] = NULL;
        pid = fork();
        if(pid == 0) execvp(argv[0], argv);
        
        else
        {
            for(int j = 0; j < argc; j++)
            {
                free(argv[j]);
            }
            
        }
        
    }
    //kill(pid, SIGUSR1);
    //Previne corona de se espalhar. Você não tem uma pandemia se todo mundo está morto.
    if(pid > 0)
    {
        int status;
        while(wait(&status) > 0)
        {
            if(status == SIGUSR1)
            {
                kill(-1*getpid(), SIGKILL);
            }
        }
        kill(getpid(), SIGKILL);
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
    pid_t pid_gerenciador = 1;
    GPID_LIST = malloc(sizeof(List));
    newList(GPID_LIST, sizeof(pid_t), NULL);
    ////
    while(1)
    {
        printf("Sou a main e meu pid é %d\n", getpid());
        printf(">>> ");
        char* commands[5];
        int n_comandos = get_tokenized_command(commands);
        int background_flag = 0;
        background_flag = trata_comando_foreground(commands, n_comandos);

        if(background_flag)
        {
            printf("FLAG: %d\n", background_flag);
            pid_gerenciador = fork();
            
            if (pid_gerenciador == 0)
            {   
                setsid();
                printf("id da sessão: %d\n", getpid());
                trata_comando_background(commands, n_comandos);
                break;
            }

            else if (pid_gerenciador > 0)
            {
                waitpid(pid_gerenciador, NULL, WNOHANG);
                addTail(GPID_LIST, &pid_gerenciador);
                for(int i = 0; i < n_comandos; i++)
                {
                    free(commands[i]);
                }
                    
            }
        }

        sleep(1);
    }

}
