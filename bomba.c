/*----------------------------------------------------------------
|  Autor: Dos Ramos, Erick  07-40845                              |
|         Gallardo, Alfredo 06-39563                              |
|  Fecha: 08/02/2013                                              |
|-----------------------------------------------------------------|
|  Descripcion del Programa:									  |
|	Es el programa que representa la entidad de bomba en la       |
|	simulacion.	  												  |
| ----------------------------------------------------------------*/

#include "bomba.h"

/* Indicará el tiempo total de la simulación */ 
	int tiempoBomba;
	int tiempoArchivo;
	int flag1;
	int flag2;
	int inv, cons;

/* Hilo que mantiene el conteo del timepo de la simulacion*/
void* hilofunctiempo(void* arg){
	char* asd = (char*) arg;
	char concat_str[256];
	while (tiempoBomba > 0){
		usleep(100000);
		tiempoBomba -= 1;
		tiempoArchivo = 480 - tiempoBomba;
		if (inv >= cons){
			while(flag1 == 1){;}
			inv -= cons;
			if(inv <= 0){				
				while(flag1 == 1){;}
					inv = 0;
				}
			}
		else {
			while(flag1 == 1){;}
			inv = 0;
			if (flag2 == 1){	
				bzero(concat_str,256);
				sprintf(concat_str, "Tanque vacio: %i minutos\n",tiempoArchivo);
				aecArch(asd, concat_str);
				flag2 = 0;
			}
		}
	} 
	exit(1);
}

//Funcion principal:
int main(int argc, char** argv) {
	int i;

	/*Informacion del cliente.*/
	char* nombreBomba;
	int maxCap;
	char* nombreFichero;
	FILE* fp;
	char ofNombre[256];
	char concat_str[256];


	/*Validacion de los argumentos de entrada*/
	char* sintaxis = "Sintaxis: bomba [-n nombreBomba] [-cp capacidadMaxima] [-i inventario] [-c consumo] [-fc FicheroCentros]\n";

	//Validando la cantidad de parámetros
	if(argc != 11){
		printf("%s", sintaxis);
		exit(1);
	}
	//Validacion de cada campo
	for( i=1; i<argc; i++){
		
		//Validando nombre de la bomba
		if (strcmp(argv[i], "-n") == 0){
			i++;
			if(argv[i]== NULL ){
				printf("%s", sintaxis);
				exit(1);
			}
			nombreBomba = argv[i];
			
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
			if(maxCap < 38000 || maxCap > 380000){
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

		//Validando número de consumo promedio
		else if (strcmp(argv[i], "-c") == 0){
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

			cons = atoi(argv[i]);
			if ( cons < 0 || cons > 1000){
				printf("Número de consumo promedio Invalido\n");
				exit(1);
			}

		}

		//Validando nombre del fichero de entrada
		else if (strcmp(argv[i], "-fc") == 0){
			i++;
			if(argv[i] == NULL){
				printf("%s", sintaxis);
				exit(1);
			}
			nombreFichero = argv[i];
			
		}
	
		
	}
	/* Indicará el tiempo total de la simulación */ 
	tiempoBomba = 480;
	tiempoArchivo = 0;
	
	/* creacion del archivo de salida log */
	bzero(ofNombre,256);
	sprintf(ofNombre,"log_%s.txt",nombreBomba);

	aecArch(ofNombre, "--------Estado Incial de la Bomba:--------");
	bzero(concat_str,256);
	sprintf(concat_str, "Nombre: %s",nombreBomba);
	aecArch(ofNombre, concat_str);

	bzero(concat_str,256);
	sprintf(concat_str, "Inventario Incial: %i",inv);
	aecArch(ofNombre, concat_str);	
	
	pthread_t hiloTiempo;
	pthread_create(&hiloTiempo,NULL,hilofunctiempo,(void *) ofNombre);

	/*Informacion sobre el socket y la coneccion*/
	struct sockaddr_in address;
    struct in_addr inaddr;
    struct hostent *host;
    int sock;


	if ((fp = fopen(nombreFichero,"r")) == NULL){
		perror("error al abrir el archivo con los servidores");
		exit(1);
	}
	flag2 = 1;
	char nombre[100];
	char att[100];
	char puertoChar[100];
	flag1 = 0;
	char buffer[256];
	int puerto;
	int n;
	int numerito;
	ColaP* servers = crearColaP();
	/*Verificamos cada servidor en el archivo de servidores para llenar la lista de prioridades*/
	while((numerito = fscanf(fp,"%[^&]&%[^&]&%s\n",nombre,att,puertoChar)) != EOF){
		/* Si el argumento se puede convertir en una direccion IP lo hace, de lo contrario busca en DNS */
		puerto = atoi(puertoChar);
    	if (inet_aton(att, &inaddr))
    		host = gethostbyaddr((char *) &inaddr, sizeof(inaddr), AF_INET);
    	else
    		host = gethostbyname(att);

	    if (!host) {	
	    	perror("error encontrando host");
	    	exit(1);
	    }
	
	    if(( sock = socket (PF_INET, SOCK_STREAM, 0)) < 0){
			perror("error al abrir el socket");
			exit(1);
		}
    	
    	address.sin_family = AF_INET;
    	address.sin_port = htons(puerto);
    	/* Toma la primera direccion IP asociada a este hostname */
    	memcpy(&address.sin_addr, host->h_addr_list[0], sizeof(address.sin_addr));
		/*Se intenta conectar al socket del servidor*/
    	if (connect(sock, (struct sockaddr *)&address, sizeof(address)) < 0){
			perror("error al conectar el socket");
		} else {
			char infoServComp[256];
			bzero(infoServComp,256);
			/*solicita al server el tiempo minimo de respuesta*/
			n = write(sock,"tiempo",6);
			if (n < 0){
    		    perror("error escribiendo solicitud");
				exit(1);
			}
			/*recibe la respuesta con el tiempo*/
			bzero(buffer,256);
    		n = read(sock,buffer,255);
    		if (n < 0){ 
    		    perror("error leyendo datos");
				exit(1);
			}
			/*agrega el server a la lista de prioridades donde el tiempo es
			es la prioridad*/
			sprintf(infoServComp,"%s&%s&%i",nombre,att,puerto);
			agregarElemP(servers,strdup(infoServComp),atoi(buffer));
    		close(sock);
		}
	}

	close(fp);
	
	char inf[256];
	bzero(inf,256);
	aecArch(ofNombre,inf);
	while(tiempoBomba > 0){
		if( inv == 0 ){
			bzero(concat_str,256);
			sprintf(concat_str, "Tanque Vacio: %i minutos \n",tiempoArchivo);
			aecArch(ofNombre, concat_str);
		}

		int predecir;
		if (servers->tam == 0){
			perror("No hay servidores activos\n");
			exit(1);
		}
		// consumo cliente * tiempo de servicio servidor
		predecir = cons*servers->pri->prior;
		
		if (inv <= maxCap - 38000 + predecir){
			int k;
			for(k=0; k < servers->tam && inv <= maxCap - 38000 + predecir; k++){
				bzero(nombre,100);
				bzero(att,100);
				bzero(puertoChar,100);
				sscanf(darElemP(servers,k),"%[^&]&%[^&]&%s",nombre,att,puertoChar);
				printf("%s - %s\n",att,nombre);
				
				bzero(concat_str,256);
				sprintf(concat_str, "Peticion: %i minutos, %s,",tiempoArchivo,nombre);
				aecArch(ofNombre, concat_str);

				puerto = atoi(puertoChar);
				if (inet_aton(att, &inaddr))
    				host = gethostbyaddr((char *) &inaddr, sizeof(inaddr), AF_INET);
    			else
    				host = gethostbyname(att);

	    		if (!host) {	
	    			perror("error encontrando host");
	    		} else if(( sock = socket (PF_INET, SOCK_STREAM, 0)) < 0){
					perror("error al abrir el socket");
				} else {
					address.sin_family = AF_INET;
		    		address.sin_port = htons(puerto);
		    		/* Toma la primera direccion IP asociada a este hostname */
		    		memcpy(&address.sin_addr, host->h_addr_list[0], sizeof(address.sin_addr));
					/*Se intenta conectar al socket del servidor*/
		    		if (connect(sock, (struct sockaddr *)&address, sizeof(address)) < 0){
						perror("No Disponible");
					} else {
						/*solicita la disponibilidad del servidor*/
						char buff2[256];
						bzero(buff2,256);
						sprintf(buff2,"d&%s",nombreBomba);
						if (n = write(sock,buff2,256) < 0)
    		    			perror("error escribiendo datos");
						bzero(buffer,256);   						
    					if (n = read(sock,buffer,255) < 0) 
    		    			perror("error leyendo datos");
						close(sock);
						if (strcmp(buffer,"no") == 0){
							aecArch(ofNombre, "Respuesta: No Disponible \n");					
							perror("No Disponible");
						} else {
							aecArch(ofNombre, "Respuesta: Disponible \n");
							int time;
							if(atoi(buffer) > tiempoBomba)
								time = tiempoBomba;
							else
								time = atoi(buffer);
							
							// Tiempo que pasa al recibir el camion de suministro
							usleep(time*100000);
							flag1 = 1;
							inv += 38000;
							if( inv >= maxCap ){
								inv = maxCap;
								bzero(concat_str,256);
								sprintf(concat_str, "Tanque FULL: %i minutos\n",tiempoArchivo);
								aecArch(ofNombre, concat_str);
							}
							flag1 = 0;
							flag2 = 1;
							bzero(concat_str,256);
							sprintf(concat_str, "Llegada de la gandola: %i min,%i litros\n",tiempoArchivo,inv);
							aecArch(ofNombre, concat_str);
						}
					} 
				}
			}
		}

		
	}

	return 0;
}
