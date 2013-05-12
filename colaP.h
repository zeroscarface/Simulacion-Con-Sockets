/*----------------------------------------------------------------
|  Autor: Dos Ramos, Erick  07-40845                              |
|         Gallardo, Alfredo 06-39563                              |
|  Fecha: 08/02/2013                                              |
|-----------------------------------------------------------------|
|  Descripcion del Programa:									  |
|			Cabecera del manejador de cola de prioridades		  |
| ----------------------------------------------------------------*/

#ifndef COLAP_H
#define COLAP_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Estructura para la cola de prioridades que tiene apuntadores a los
	los elementos tanto anterior como siguiente.
 */
typedef struct nodo {
	struct nodo* sig;
	struct nodo* ant;
	int prior;
	char* info;
} Nodo;

/*Estructura de cola de prioridades que tiene el tamano y un apuntador al
	primer elemento de esta.
 */
typedef struct colaP {
	Nodo* pri;
	int tam;
} ColaP;

/*crea una cola de prioridades vacia*/
ColaP* crearColaP();

/*agrega un elemento en la posicion correspondiente a su prioridad*/
void agregarElemP(ColaP*, char*, int);

/*devuelve el elemento en la posicion dada*/
char* darElemP(ColaP*, int);
#endif
