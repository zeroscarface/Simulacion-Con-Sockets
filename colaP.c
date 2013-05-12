/*----------------------------------------------------------------
|  Autor: Dos Ramos, Erick  07-40845                              |
|         Gallardo, Alfredo 06-39563                              |
|  Fecha: 08/02/2013                                              |
|-----------------------------------------------------------------|
|  Descripcion del Programa:					  				  |
|		Cola de prioridades para manejo de servidores.			  |
| ----------------------------------------------------------------*/

#include "colaP.h"

/*Crea una cola de prioridades vacia.*/
ColaP* crearColaP(){
	ColaP* cp;
	if((cp = (ColaP*)malloc(sizeof(ColaP))) < 0){
		perror("error al pedir memoria");
		exit(1);
	}
	cp->pri = NULL;
	cp->tam = 0;
	return cp;
}

/*Crea un elemento para una cola de prioridades.
 * info: informacion para guardar en una cola de prioridades.
 * prior: prioridad de la informacion.
 */
Nodo* crearNodo(char* info, int prior){
	Nodo* np;
	if((np = (Nodo*)malloc(sizeof(Nodo))) < 0){
		perror("error al pedir memoria");
		exit(1);
	}
	np->info = info;
	np->prior = prior;
	np->sig = NULL;
	np->ant = NULL;
	return np;
}

/*Agrega un elemento en la posicion correspondiente a su prioridad.
 * cp: cola de prioridades a agregarle el elemento nuevo.
 * info: informacion a agregar en la cola de prioridades.
 * prior: prioridad de la informacion a agregar.
 */
void agregarElemP(ColaP* cp, char* info, int prior){
	Nodo* np = crearNodo(info,prior);
	if (cp->tam == 0){
		cp->pri = np;		
		cp->tam = 1;
		return;
	}
	Nodo* aux = cp->pri;
	int i = 0;
	for (i = 0; i < cp->tam && aux->prior < prior; i++)
		aux = aux->sig;	
	if (aux == NULL){
		aux = cp->pri;
		for (i=0; i < cp->tam && aux->sig != NULL; i++)
			aux = aux->sig;
		aux->sig = np;
		np->ant = aux;
		cp->tam += 1;
		return;
	}
	if (aux->ant == NULL){
		cp->pri = np;
	} else {
		aux->ant->sig = np;
	}
	np->sig = aux;
	np->ant = aux->ant;
	aux->ant = np;
	cp->tam += 1;
}

/*Devuelve el elemento en la posicion dada.
 * cp: cola de prioridades a buscar el elemento.
 * pos: posicion del elemento a buscar en la cola de prioridades.
 */
char* darElemP(ColaP* cp, int pos){
	Nodo* aux = cp->pri;
	int i;
	for (i=0; i < pos && i < cp->tam; i++)
		aux = aux->sig;
	return aux->info;
}
