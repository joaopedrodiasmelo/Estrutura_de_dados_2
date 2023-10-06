all:
	gcc -c registro.c -o registro.o
	gcc -c indice_primario.c -o indice_primario.o
	gcc -c indice_secundario.c -o indice_secundario.o 
	gcc trabalho02.c registro.o  indice_primario.o  indice_secundario.o -o trabalho02

run:
	./trabalho02  

clear:	
	rm ./trabalho02 ./registro.o ./indice_primario.o ./indice_secundario
