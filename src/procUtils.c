
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include "../headers/procUtils.h"
#include "../headers/strUtils.h"
#include "../headers/linkedList.h"


void trata_sinal(int sinal)
{
    if(FG_PROCESS_FLAG)
    {
        kill(FG_PROCESS_FLAG, sinal);
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
            // caso comando seja de CD
            int result = muda_diretorio(argv[1]);
            return 0;
        }
        else if (strcmp(argv[0], "exit") == 0)
        {
            // caso comando seja de exit.
            printf("Entrando no bloco de exit.\n");
            Node* node = GPID_LIST->head;
            while(node)     // percorre lista dos ids de sessões/grupos.
            {
                waitpid(*(int*)node->data, NULL, WNOHANG);      // colhe possível "zumbi" causado por SIGUSR1
                kill(-1 * (*(int*)(GPID_LIST->head->data)), SIGKILL);       // mata todo o grupo
                node = node->next;
            }
            destroyList(GPID_LIST);
            printf("Saindo da shell\n");
            exit(0);
            printf("deu ruim no exit\n");
        }
        else if(strcmp(argv[argc - 1], "%") == 0)
        {

            argc--;     // Diminui argc para eliminar "argumento" '%'
            argv[argc] = NULL;      //argv precisa terminar em NULL
            pid_t pid = fork();
            if(pid == 0)
            {
                execvp(argv[0], argv);
                perror("Deu ruim!");
                return 0; // failsafe
            }
            else
            {
                FG_PROCESS_PID = pid;   // Torna acessível ao handler o pid do processo em foreground
                FG_PROCESS_FLAG = 1;    // Informa que está executando atualmente um processo em foreground
                waitpid(pid, NULL, 0);  // Espera pelo processo
                FG_PROCESS_FLAG = 0;    // Informa que não está mais executando um processo em foreground
                for(int i = 0; i < argc; i++)
                {
                    free(argv[i]);
                }
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
    if(n_comandos == 1)
    {
        struct sigaction ignora = {.sa_handler = SIG_IGN};
        sigemptyset(&ignora.sa_mask);
        sigaction(SIGUSR1, &ignora, NULL);
    }
    char* argv[6];
    int argc = 0;
    pid_t pid;
    for(int i = 0; i < n_comandos; i++)
    {

        argc = cria_argv(commands[i], argv); 
        argv[argc] = "&"; // seta argumento para fazer comando em background
        argv[argc+1] = NULL; // argv precisa terminar com um ponteiro para NULL
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
    if(pid > 0)
    {
        // Mata a sessão inteira em caso de SIGUSR1
        int status;
        while(wait(&status) > 0)
        {
            if(status == SIGUSR1)
            {
                kill(-1*getpid(), SIGKILL);
            }
        }

    }

}
