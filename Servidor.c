#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <mysql.h>


int main(int argc, char *argv[])
{
	
	int sock_conn, sock_listen, ret;
	struct sockaddr_in serv_adr;
	char peticion[512];
	char respuesta[512];
	// INICIALITZACIONS
	// Obrim el socket
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("Error creant socket");
	// Fem el bind al port
	
	
	memset(&serv_adr, 0, sizeof(serv_adr));// inicialitza a zero serv_addr
	serv_adr.sin_family = AF_INET;
	
	// asocia el socket a cualquiera de las IP de la m?quina. 
	//htonl formatea el numero que recibe al formato necesario
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	// establecemos el puerto de escucha
	serv_adr.sin_port = htons(9200);
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf ("Error al bind");
	
	if (listen(sock_listen, 3) < 0)
		printf("Error en el Listen");
	
	int i;
	// Bucle infinito
	for (;;){
		printf ("Escuchando\n");
		
		sock_conn = accept(sock_listen, NULL, NULL);
		printf ("He recibido conexion\n");
		//sock_conn es el socket que usaremos para este cliente
		
		int terminar =0;
		// Entramos en un bucle para atender todas las peticiones de este cliente
		//hasta que se desconecte
		while (terminar ==0)
		{
			// Ahora recibimos la petici?n
			ret=read(sock_conn,peticion, sizeof(peticion));
			printf ("Recibido\n");
			
			// Tenemos que a?adirle la marca de fin de string 
			// para que no escriba lo que hay despues en el buffer
			peticion[ret]='\0';
			
			
			printf ("Peticion: %s\n",peticion);
			char *p = strtok( peticion, "/");
			int codigo =  atoi (p);
			// Ya tenemos el c?digo de la petici?n
			char nombre[20];
			char contrasenya[20];
			if ((codigo == 1) || (codigo == 2))
			{
				p = strtok( NULL, "/");
				strcpy (nombre, p);
				p = strtok(NULL, "/*/");
				strcpy (contrasenya, p);
				printf("1");
				printf ("Codigo: %d, Nombre: %s, Contrase￱a: %s\n", codigo, nombre, contrasenya);
			}
			printf("2");
			if (codigo ==0){
				terminar=1;
			}
			if (codigo ==1)
			{//piden la longitd del nombre
				MYSQL *conn;
				int err;
				// Estructura especial para almacenar resultados de consultas 
				MYSQL_RES *resultado;
				MYSQL_ROW row;
				char consulta[80];
				char id[20];
				//Creamos una conexion al servidor MYSQL 
				conn = mysql_init(NULL);
				if (conn==NULL) {
					printf ("Error al crear la conexi??n: %u %s\n", 
							mysql_errno(conn), mysql_error(conn));
					exit (1);
				}
				//inicializar la conexion
				conn = mysql_real_connect (conn, "localhost","root", "mysql", "base_de_datos",0, NULL, 0);
				if (conn==NULL) {
					printf ("Error al inicializar la conexi??n: %u %s\n", 
							mysql_errno(conn), mysql_error(conn));
					exit (1);
				}
				sprintf(consulta, "SELECT jugadores.id FROM jugadores WHERE nombre = '%s' AND password = '%s'", nombre, contrasenya);
				err=mysql_query (conn, consulta); 
				if (err!=0) {
					printf ("Error al consultar datos de la base %u %s\n",
							mysql_errno(conn), mysql_error(conn));
					exit (1);
				}
				resultado = mysql_store_result (conn);
				row = mysql_fetch_row (resultado);
				if (row==NULL) {
					printf("ERROR");
				}
				else {
					sprintf(respuesta, row[0]);
				}
				mysql_close (conn);
			}
			if (codigo ==2)
			{
				printf("2");
				MYSQL *conn;
				int err;
				// Estructura especial para almacenar resultados de consultas 
				MYSQL_RES *resultado;
				MYSQL_ROW row;
				char consulta[512];
				int id;
				//Creamos una conexion al servidor MYSQL 
				conn = mysql_init(NULL);
				if (conn==NULL) {
					printf ("Error al crear la conexi??n: %u %s\n", 
							mysql_errno(conn), mysql_error(conn));
					exit (1);
				}
				//inicializar la conexion
				conn = mysql_real_connect (conn, "localhost","root", "mysql", "base_de_datos",0, NULL, 0);
				if (conn==NULL) {
					printf ("Error al inicializar la conexi??n: %u %s\n", 
							mysql_errno(conn), mysql_error(conn));
					exit (1);
				}
				sprintf(consulta, "SELECT MAX(id) FROM jugadores");
				err=mysql_query (conn, consulta); 
				if (err!=0) {
					printf ("Error al consultar datos de la base %u %s\n",
							mysql_errno(conn), mysql_error(conn));
					exit (1);
				}
				resultado = mysql_store_result (conn);
				row = mysql_fetch_row (resultado);
				if (row==NULL)
					printf("ERROR");
				else
					id = atoi(row[0]) + 1;
				
				sprintf(consulta, "INSERT INTO jugadores (id, nombre, password) VALUES (%s,'%s','%s')", id, nombre, contrasenya);
				err=mysql_query (conn, consulta); 
				if (err!=0) {
					printf ("Error al consultar datos de la base %u %s\n",
							mysql_errno(conn), mysql_error(conn));
					exit (1);
				}
				resultado = mysql_store_result (conn);
				row = mysql_fetch_row (resultado);
				if (row==NULL)
					printf("ERROR");
				else
				strcpy (respuesta, "Registrado.");
				mysql_close (conn);
			}
			else if (codigo ==3)
			{
				MYSQL *conn;
				int err;
				// Estructura especial para almacenar resultados de consultas 
				MYSQL_RES *resultado;
				MYSQL_ROW row;
				char consulta[80];
				char id[20];
				//Creamos una conexion al servidor MYSQL 
				conn = mysql_init(NULL);
				if (conn==NULL) {
					printf ("Error al crear la conexi??n: %u %s\n", 
							mysql_errno(conn), mysql_error(conn));
					exit (1);
				}
				//inicializar la conexion
				conn = mysql_real_connect (conn, "localhost","root", "mysql", "base_de_datos",0, NULL, 0);
				if (conn==NULL) {
					printf ("Error al inicializar la conexi??n: %u %s\n", 
							mysql_errno(conn), mysql_error(conn));
					exit (1);
				}
				sprintf(consulta, "SELECT Ganador FROM partidas GROUP BY Ganador ORDER BY COUNT(*) DESC LIMIT 1");
				err=mysql_query (conn, consulta); 
				if (err!=0) {
					printf ("Error al consultar datos de la base %u %s\n",
							mysql_errno(conn), mysql_error(conn));
					exit (1);
				}
				resultado = mysql_store_result (conn);
				row = mysql_fetch_row (resultado);
				if (row==NULL) {
					printf("ERROR");
				}
				else {
					sprintf(respuesta, row[0]);
				}
				mysql_close (conn);
			}
			else if (codigo ==5)
			{
				MYSQL *conn;
				int err;
				// Estructura especial para almacenar resultados de consultas 
				MYSQL_RES *resultado;
				MYSQL_ROW row;
				char consulta[80];
				char id[20];
				//Creamos una conexion al servidor MYSQL 
				conn = mysql_init(NULL);
				if (conn==NULL) {
					printf ("Error al crear la conexi??n: %u %s\n", 
							mysql_errno(conn), mysql_error(conn));
					exit (1);
				}
				//inicializar la conexion
				conn = mysql_real_connect (conn, "localhost","root", "mysql", "base_de_datos",0, NULL, 0);
				if (conn==NULL) {
					printf ("Error al inicializar la conexi??n: %u %s\n", 
							mysql_errno(conn), mysql_error(conn));
					exit (1);
				}
				
				strcpy (consulta,"SELECT idPartida FROM relacion WHERE puntos1 > puntos2 + 30 OR puntos2 > puntos1 + 30");
				err=mysql_query (conn, consulta); 
				resultado = mysql_store_result (conn);
				row = mysql_fetch_row (resultado);
				if (err!=0) {
					printf ("Error al consultar datos de la base %u %s\n",
							mysql_errno(conn), mysql_error(conn));
					exit (1);
				}
				else
					while(row!=NULL){
					sprintf(respuesta, row[0]);
					printf ("Respuesta: %s\n", respuesta);
					// Enviamos respuesta
					write (sock_conn,respuesta, strlen(respuesta));
					row = mysql_fetch_row(resultado);
				}
				mysql_close (conn);
			}
			else if (codigo ==4)
			{
				MYSQL *conn;
				int err;
				// Estructura especial para almacenar resultados de consultas 
				MYSQL_RES *resultado;
				MYSQL_ROW row;
				char consulta[80];
				char id[20];
				//Creamos una conexion al servidor MYSQL 
				conn = mysql_init(NULL);
				if (conn==NULL) {
					printf ("Error al crear la conexi??n: %u %s\n", 
							mysql_errno(conn), mysql_error(conn));
					exit (1);
				}
				//inicializar la conexion
				conn = mysql_real_connect (conn, "localhost","root", "mysql", "base_de_datos",0, NULL, 0);
				if (conn==NULL) {
					printf ("Error al inicializar la conexi??n: %u %s\n", 
							mysql_errno(conn), mysql_error(conn));
					exit (1);
				}
					exit (1);strcpy (consulta,"SELECT COUNT(*) FROM partidas WHERE ganador=''");
					err=mysql_query (conn, consulta); 
					if (err!=0) {
						printf ("Error al consultar datos de la base %u %s\n",
								mysql_errno(conn), mysql_error(conn));
						exit (1);
					}
					resultado = mysql_store_result (conn);
					row = mysql_fetch_row (resultado);
					if (row==NULL)
						printf("ERROR");
					else
						sprintf(respuesta, row[0]);
				
				mysql_close (conn);
			}
			
			if (codigo !=0)
				{
					printf ("Respuesta: %s\n", respuesta);
					// Enviamos respuesta
					write (sock_conn,respuesta, strlen(respuesta));
				}
		}
		// Se acabo el servicio para este cliente
		close(sock_conn); 
	}
}