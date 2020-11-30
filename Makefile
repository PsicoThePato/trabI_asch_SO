# INTEGRANTES: JAIRO MARCOS OLIVEIRA MOUTINHO

all:
	gcc *.c -o asch

run: all
	valgrind ./asch
