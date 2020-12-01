#ifndef _STRUTILS_H_
#define _STRUTILS_H_


/*
 * cria_vetor_comandos
 * IN:	 comando - String de comandos lida do usuário via stdin
 *      commands - Vetor de strings para salvar os comandos separaods
 *                 por '<3'

 * OUT:	 Retorna a quantidade de comandos.(1 + quantidade de '<3')
 * POST: Inutiliza a string comando(efeito colateral da strtok()) e, copia e aloca, para cada
 *       posição de commands, um espaço de memória o suficiente para comportar a string
 *       do comando.
 */
int cria_vetor_comandos(char *comando, char**commands);


/*
 * get_tokenized_command
 * IN:	 commands - Vetor de strings para salvar os comandos separaods
 *                 por '<3'

 * OUT:	 Retorna o número de posições utilizadas do vetor commands ou -1 caso
 *       haja problema na leitura do input do usuário.
 * POST: Aloca, para cada posição de commands, um espaço de memória
 *       o suficiente para comportar a string do comando.
 */
int get_tokenized_command(char **commands);


/*
 * cria_argv
 * IN:	 comando - String referente a um comando a ser executado.
 *       argv - Vetor de strings onde cada palavra separada por " " em comando
 *       será salvo num elemento.
 * OUT:	 Retorna argc - um inteiro que diz quantas palavras havia o comando do usuário.
 * POST: Inutiliza a string comando (efeito colateral de strtok) e, copie e aloca, para cada elemento
 *       de argv, uma área de memória o suficiente para comportar a string do argumento passado.
 */
int cria_argv(char *comando, char** argv);


#endif  