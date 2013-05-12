#*----------------------------------------------------------------
#  Autor: Dos Ramos, Erick  07-40845                              |
#         Gallardo, Alfredo 06-39563                              |
#  Fecha: 08/02/2013                                              |
#-----------------------------------------------------------------|
#  Descripcion del Programa:									  |
#		Archivo Makefile, compilar con make all, limpiar		  |
#		con makeclean.										      |
# ----------------------------------------------------------------*/

all: centro bomba

clean:
	rm centro bomba colaP.o esManejador.o

centro: centro.c centro.h esManejador.o
	gcc -o centro centro.c esManejador.o -w -pthread
bomba: bomba.c bomba.h colaP.o esManejador.o
	gcc -o bomba bomba.c colaP.o esManejador.o -w -pthread
colaP.o:
	gcc -c colaP.c -w
esManejador.o:
	gcc -c esManejador.c -w
