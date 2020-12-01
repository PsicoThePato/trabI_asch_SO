#ifndef _PROCUTILS_H_
#define _PROCUTILS_H_
#include <sys/types.h>
#include <unistd.h>
#include "linkedList.h"

extern int FG_PROCESS_FLAG;

extern List* GPID_LIST;

extern pid_t FG_PROCESS_PID;


/*
 * trata_sinal
 * IN:	 sinal - Sinal para ser tratado

 * OUT:	 
 * POST: Caso a global FG_PROCESS_FLAG seja verdadeira, segue o comportamento
 *       normal do sinal.
 *       caso contrário ignora o sinal e printa uma mensagem engraçadinha.
 */
void trata_sinal(int sinal);


/*
 * muda_diretorio
 * IN:	 path - Sinal para ser tratado

 * OUT:	 0 em caso de sucesso, -1 caso contrário.
 * POST: Muda o working directory para aquele específicado no path.
 */
int muda_diretorio(char *path);



/*
 * trata_comando_foreground
 * IN:	 comando - Um vetor de strings, onde cada posição é um comando (para)
 *                  cada '<3', considera-se que houve um comando extra.
 *      n_commands - Número de comandos separaods por "<3"

 * OUT:	 Retorna 0 caso o comando tenha sido tratado em foreground e 1
 *       caso ele seja designado para um tratamento em background.
 * POST: Executa o comando pedido.
 */
int trata_comando_foreground(char **comando, int n_commands);


/*
 * trata_comando_background
 * IN:	 comando - Um vetor de strings, onde cada posição é um comando (para
 *                  cada '<3', considera-se que houve um comando extra.)
 *      n_commands - Número de comandos separaods por "<3"

 * OUT:	 Retorna 0 caso os comandos tenham sido executados e terminados.
 * POST: Executa os comandos pedidos.
 */
int trata_comando_background(char **commands, int n_comandos);


#endif