/*----------------------------------------------------------------
|  Autor: Dos Ramos, Erick  07-40845                              |
|         Gallardo, Alfredo 06-39563                              |
|  Fecha: 08/02/2013                                              |
|-----------------------------------------------------------------|
|  Descripcion del Programa:									  |
|	Es el programa que representa la entidad de centro en la      |
|	simulacion.	  												  |
| ----------------------------------------------------------------*/

#include "centro.h"


// Variables Globales:
int disp[5] = {0,0,0,0,0};
int inv;
int tiempoCentro;
int tiempoArchivo;
int sumin;
int maxCap;
int flag1;
int flag2;
char ofNombre[256];

// Hijo que maneja el tiempo:
void* hilofunctiempo(void* arg){
	char* asd = (char*) arg;
	char concat_str[256];
	while (tiempoCentro > 0){
		usleep(100000);
		tiempoCentro -= 1;
		tiempoArchivo = 480 - tiempoCentro;
		if (inv <= maxCap - sumin){
			while(flag1 == 1){;}
			inv += sumin;
		} else {
			while(flag1 == 1){;}				
			inv = maxCap;
			if(flag2 == 1){	
				bzero(concat_str,256);
				sprintf(concat_str, "Tanque Full: %i min\n",tiempoArchivo);
				aecArch(ofNombre, concat_str);
				flag2 = 0;
			}
		}
	} 
	exit(1);
}

/* Funcion que genera los hijos que manejan el control de las gandolas
que atienden los suministros. */
void* hilofunc(void* arg){
	int* asd = (int*) arg;
	char concat_str[256];
	flag1 = 1;
	if(inv >= 38000){
		inv -= 38000;
	} else {
		inv = 0;
	}
	bzero(concat_str,256);
	sprintf(concat_str, " Inventario: %i\n",inv);
	aecArch(ofNombre, concat_str);
	flag1 = 0;
	char buff[256];
	bzero(buff,256);
	sprintf(buff,"%d",asd[2]);
	if (write(asd[1], buff , 256) < 0){
		perror("error escribiendo en el socket");
		close(asd[1]);
	}
	usleep(asd[2]*100000);	
	close(asd[1]);
	disp[asd[3]] = 0;
	pthread_exit(1);
}


//Funcion principal:
int main(int argc, char** argv) {
	/* Indicará el tiempo total de la simulación */ 
	tiempoCentro = 480;
	tiempoArchivo = 0;
	flag1 = 0;
	flag2 = 1;
	/* Variables para el nombre del archivo de salida */
	ofNombre[256];
	char concat_str[256];
	
	char* nombreCentro;
	int tiempo, i, puerto;
	/*Validacion de los argumentos de entrada*/ 	
	char* sintaxis = "Sintaxis: centro [-n nombreCentro] [-cp capacidadMaxima] [-i inventario] [-t tiempo] [-s suministro] [-p puerto]\n";

	//Validando la cantidad de parámetros
	if(argc != 13){
		printf("%s", sintaxis);
		exit(1);
	}
	//Validacion de cada campo
	for( i=1; i<argc; i++){
		
		//Validando nombre del centro
		if (strcmp(argv[i], "-n") == 0){
			i++;
			if(argv[i]== NULL ){
				printf("%s", sintaxis);
				exit(1);
			}
			nombreCentro = argv[i];
			
		}

		//Validando capacidad maxima
		else if (strcmp(argv[i], "-cp") == 0){
			i++;
			if(argv[i] == NULL){
				printf("%s", sintaxis);
				exit(1);
			}
			
			char* p = argv[i];
			errno = 0;
			long val = strtol(argv[i], &p, 10);
			if (errno != 0 || argv[i] == p || *p!=0){
				printf("%s",sintaxis);
				exit(1);
			}	
		
			maxCap = atoi(argv[i]);
			if(maxCap < 38000 || maxCap > 3800000){
				printf("%s", "Número de capacidad Invalido\n");
				exit(1);
			}
		}

		//Validando tamano del inventario
		else if (strcmp(argv[i], "-i") == 0){
			i++;
			if(argv[i] == NULL){
				printf("%s", sintaxis);
				exit(1);
			}

			char* p = argv[i];
			errno = 0;
			long val = strtol(argv[i], &p, 10);
			if (errno != 0 || argv[i] == p || *p!=0){
				printf("%s",sintaxis);
				exit(1);
			}

			inv = atoi(argv[i]);
			if( inv > maxCap){
				printf("Número de inventario mayor de la capacidad\n");
				exit(1);
			}
		}

		//Validando número de tiempo
		else if (strcmp(argv[i], "-t") == 0){
			i++;
			if(argv[i] == NULL){
				printf("%s", sintaxis);
				exit(1);
			}
			
			char* p = argv[i];
			errno = 0;
			long val = strtol(argv[i], &p, 10);
			if (errno != 0 || argv[i] == p || *p!=0){
				printf("%s",sintaxis);
				exit(1);
			}

			tiempo = atoi(argv[i]);
			if ( tiempo < 0 || tiempo > 180){
				printf("Número de tiempo Invalido\n");
				exit(1);
			}

		}

		//Validando la cantidad de suministro
		else if (strcmp(argv[i], "-s") == 0){
			i++;
			if(argv[i] == NULL){
				printf("%s", sintaxis);
				exit(1);
			}

			char* p = argv[i];
			errno = 0;
			long val = strtol(argv[i], &p, 10);
			if (errno != 0 || argv[i] == p || *p!=0){
				printf("%s",sintaxis);
				exit(1);
			}

			sumin = atoi(argv[i]);
			if ( sumin < 0 || sumin > 10000){
				printf("Número suministro Invalido\n");
				exit(1);
			}
		}
		//Validando el numero de puerto usado
		else if (strcmp(argv[i], "-p") == 0){
			i++;
			if(argv[i] == NULL){
				printf("%s", sintaxis);
				exit(1);
			}

			char* p = argv[i];
			errno = 0;
			long val = strtol(argv[i], &p, 10);
			if (errno != 0 || argv[i] == p || *p!=0){
				printf("%s",sintaxis);
				exit(1);
			}

			puerto = atoi(argv[i]);
			if ( puerto < 1024 && puerto > 49151){
				printf("Número de puerto Invalido\n");
				exit(1);
			}
		}
	}

	/* creacion del archivo de salida log */
	bzero(ofNombre,256);
	sprintf(ofNombre,"log_%s.txt",nombreCentro);

	aecArch(ofNombre, "--------Estado Incial del Centro:--------");
	bzero(concat_str,256);
	sprintf(concat_str, "Nombre: %s",nombreCentro);
	aecArch(ofNombre, concat_str);

	bzero(concat_str,256);
	sprintf(concat_str, "Inventario Incial: %i litros \n",inv);
	aecArch(ofNombre, concat_str);	

	if(inv >= maxCap){
		bzero(concat_str,256);
		sprintf(concat_str, "Tanque Full: %i min\n",tiempoArchivo);
		aecArch(ofNombre, concat_str);
	}
	
	//Inicializacion de las variables para la conexion
	int indexH = 0;
	struct sockaddr_in address;
	struct in_addr inaddr;
	struct hostent *host;
	int sock, conn, n;
	size_t addrLength = sizeof(struct sockaddr_in);
	pthread_t hilo[5];
	pthread_t hiloTiempo;
	/*Intentamos abrir el socket*/
	if(( sock = socket (PF_INET, SOCK_STREAM, 0)) < 0){
		perror("error al abrir el socket");
		exit(1);
	}

	i = 1;
    setsockopt(sock,SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i));
    address.sin_family = AF_INET;
    address.sin_port = htons(puerto);
    memset(&address.sin_addr, 0, sizeof(address.sin_addr));

	// Creacion de socket
	if (bind(sock, (struct sockaddr *)&address, sizeof(address))){
		perror("error al vincular el socket");
		exit(1);
	}
	
	// Escuchar por el socket creado
    if (listen(sock, 5)) {
		perror("error al escuchar el socket");
		exit(1);
	}
	int pid = 0;
	char buff[256];
	char nombreBomba[256];
	char opt[2];
	int pid2 = pthread_create(&hiloTiempo,NULL,hilofunctiempo,(void *) ofNombre);
	

	// Ciclo de la simulacion:
    while(tiempoCentro > 0) {
		if( inv == 0 ){
			bzero(concat_str,256);
			sprintf(concat_str, "Tanque Vacio: %i minutos\n",tiempoArchivo);
			aecArch(ofNombre, concat_str);
		}	

		// Verificando coneccion para revisar que accion se va a tomar.		
		if((conn = accept(sock, (struct sockaddr *) &address, &addrLength)) >= 0){
			bzero(buff,256);
			n = read(conn,buff,255);
			if (n < 0) {
				perror("error escribiendo en el socket");
		    	close(conn);
			}
			if (strcmp(buff,"tiempo") == 0){
				bzero(buff,255);
				sprintf(buff,"%d",tiempo);		
				n = write(conn, buff, 256);
		 		if (n < 0) {
					perror("error escribiendo en el socket");
		    		close(conn);
				}
			} else {
				bzero(nombreBomba,256);
				bzero(opt,2);
				sscanf(buff,"%[^&]&%s",opt,nombreBomba);
				if (strcmp(opt,"d") == 0){
					for(indexH = 0; indexH < 5 && disp[indexH] != 0 ; indexH++){;}
					if(indexH < 5 && disp[indexH] == 0 && inv >= 38000){
						int time;
						if (tiempoCentro > tiempo)
							time = tiempo;
						else
							time = tiempoCentro;
						bzero(concat_str,256);
						sprintf(concat_str, "Suministro: %i  minutos, %s,",tiempoArchivo,nombreBomba);
						aecArch(ofNombre, concat_str);
						disp[indexH] = 1;
						int arg[4] = {sock,conn,time,indexH};
						pid=pthread_create(&hilo[indexH],NULL,hilofunc,(void *)arg);
						if(pid){
							perror("error al crear el hilo\n");
							aecArch(ofNombre, "Respuesta : No Existoso");
						} else {
							aecArch(ofNombre, "Respuesta: Existoso");
						}
						flag2 = 1;
					} else {
						if (n = write(conn, "no" , 2) < 0){
							perror("error escribiendo en el socket");
							close(conn);
						}
						close(conn);
					}
				}
			}
		bzero(buff,256);
		}
   	}

    if (conn <  0) {
		perror("error al escuchar el socket");
		exit(1);
	}
    close(sock);
    return 0;
}
