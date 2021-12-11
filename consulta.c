void LiderActual(char respuesta[512])	
{
	// Estructura especial para almacenar resultados de consultas 
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta[80];
	char respuesta2[512];
	int err;
	sprintf(consulta, "SELECT Ganador FROM partidas GROUP BY Ganador ORDER BY COUNT(*) DESC LIMIT 1");
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
		strcpy(respuesta2, row[0]);
		sprintf(respuesta, "30/%s/", respuesta2);
}