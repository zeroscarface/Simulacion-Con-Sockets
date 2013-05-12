/*----------------------------------------------------------------
|  Autor: Dos Ramos, Erick  07-40845                              |
|         Gallardo, Alfredo 06-39563                              |
|  Fecha: 08/02/2013                                              |
|-----------------------------------------------------------------|
|  Descripcion del Programa:					  				  |
|		Funciones para creacion de archivo de salida (log)		  |
| ----------------------------------------------------------------*/

#include "esManejador.h"

/* Retorna un apuntador a fichero el cual tiene estado de abierto para
	escritura.
 * dir: nombre del fichero a abrirse para escritura.
 */
FILE* abrirArchEsc(char* dir){
  FILE* fp;
  if ((fp = fopen(dir, "a+")) == NULL) {
    perror("Error al abrir archivo para escritura\n");
    exit(1);
  }
  return fp;
}

/* Escribe la cadena de caracteres dada en el fichero.
 * nombre: nombre del fichero a escribir la informacion.
 * info: cadena de caracteres a escribir en el fichero.
 */
void aecArch(char* nombre, char* info){
	FILE* fp = abrirArchEsc(nombre);
	fprintf(fp,"%s\n",info);
	fclose(fp);
}
