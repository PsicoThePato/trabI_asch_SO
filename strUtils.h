#ifndef _STRUTILS_H_
#define _STRUTILS_H_


int cria_vetor_comandos(char *comando, char**commands);


int get_tokenized_command(char **commands);


int cria_argv(char *comando, char** argv);


#endif  