/*----------------------------------------------------------------
|  Autor: Dos Ramos, Erick  07-40845                              |
|         Gallardo, Alfredo 06-39563                              |
|  Fecha: 08/02/2013                                              |
|-----------------------------------------------------------------|
|  Descripcion del Programa:									  |
|			Cabecera del manejador de archivo					  |
| ----------------------------------------------------------------*/

#ifndef ESMANEJADOR_H
#define ESMANEJADOR_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*Retorna un apuntador a fichero el cual tiene estado de abierto.*/
FILE* abrirArchEsc(char* dir);

/*Escribe la cadena de caracteres dada en el fichero. */
void aecArch(char* nombre, char* info);

#endif
