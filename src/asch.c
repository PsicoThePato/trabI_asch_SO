#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include "../headers/linkedList.h"
#include "../headers/strUtils.h"
#include "../headers/procUtils.h"


List* GPID_LIST = NULL;     // binding da variável global
int FG_PROCESS_FLAG = 0;    // binding da variável global
pid_t FG_PROCESS_PID = 0;   // binding da variável global


int main()
{
    ///------------------- Define tratamento de sinais -------------------------///
    struct sigaction tratador_sinais = {.sa_handler = trata_sinal};
    sigemptyset(&tratador_sinais.sa_mask);
    sigaddset(&tratador_sinais.sa_mask, SIGINT);
    sigaddset(&tratador_sinais.sa_mask, SIGQUIT);
    sigaddset(&tratador_sinais.sa_mask, SIGTSTP);
    sigaction(SIGINT, &tratador_sinais, NULL);
    sigaction(SIGQUIT, &tratador_sinais, NULL);
    sigaction(SIGTSTP, &tratador_sinais, NULL);
    // ------------------------------------------------------------- ///
    pid_t pid_gerenciador = 1;
    GPID_LIST = malloc(sizeof(List)); // Aloca espaço da lista
    newList(GPID_LIST, sizeof(pid_t), NULL); // Inicializa lista que guardará pid de sessões
    ////
    while(1)
    {
        char cwd[1000];
        getcwd(cwd, sizeof(cwd));
        printf("%s >>> ", cwd);
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
                //printf("id da sessão: %d\n", getpid());
                trata_comando_background(commands, n_comandos);
                break; // para filho não executar fluxo da main
            }

            else if (pid_gerenciador < 0)
            {
                perror("Erro no fork do gerenciador!");
            }
            
            else if (pid_gerenciador > 0)
            {
                waitpid(pid_gerenciador, NULL, WNOHANG);
                addTail(GPID_LIST, &pid_gerenciador); // Adiciona id da sessão à lista encadeada.
                for(int i = 0; i < n_comandos; i++)
                {
                    free(commands[i]);
                }
                    
            }
        }

    }

}
