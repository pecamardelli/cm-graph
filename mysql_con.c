#include "header.h"

MYSQL *mysql_con(void){

	MYSQL *conexion;
	MYSQL_RES *res;

	char *servidor = "localhost";
	char *usuario = "cm_data";
	char *clave = "cm_data";
	char *db = "cm_data";

	if(!(conexion = mysql_init(0)))
	{
		logger(cfg.log_file, "No se pudo iniciar la conexion con la base de datos.");
		fprintf(stderr, "No se pudo iniciar la conexion con la base de datos.\n");
		return NULL;
	}

	if(!mysql_real_connect(conexion, servidor, usuario, clave, db, 2585, NULL, 0))
	{
		logger(cfg.log_file, "Imposible conectar con servidor mysql en el puerto.");
		fprintf(stderr, "Imposible conectar con servidor mysql en el puerto.\n");
	    mysql_close(conexion);
		return NULL;
	}

	return conexion;
}
