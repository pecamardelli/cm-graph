#include "header.h"

void configuracion()
{
	int i, f, k;
	MYSQL *conexion;
	MYSQL_ROW row;
	MYSQL_RES *res;
	char consulta[256], log[256];

	oid prueba[MAX_OID_LEN];
	size_t prueba_len = MAX_OID_LEN;

	//cmts_oids = (oids *) malloc (sizeof(oids)*10);
	//cm_oids = (oids *) malloc (sizeof(oids)*20);

	conexion = mysql_con();

	if(conexion == NULL)
	{
		do
		{
			sleep(5);
			conexion = mysql_con();
		}while(conexion == NULL);
	}

	/* --------------- CARGA DE OIDS --------------- */

	mysql_query(conexion, "SELECT * FROM cm_data.oids WHERE 1");
	res = mysql_store_result(conexion);

	cmts_q = 0;
	cm_q = 0;
	init_snmp("MODEMS");

	if(mysql_num_rows(res))
	{
		for(i=0;i < mysql_num_rows(res); i++)
		{
			row = mysql_fetch_row(res);
			if(strcmp(row[3], "CMTS") == 0)
			{
				strcpy(cmts_oids[cmts_q].oid_str, row[1]);
				strcpy(cmts_oids[cmts_q].desc, row[2]);
				cmts_oids[cmts_q].objid_len = MAX_OID_LEN;
				read_objid(row[1], cmts_oids[cmts_q].objid, &cmts_oids[cmts_q].objid_len);	
				cmts_q++;
			}
			else if(strcmp(row[3], "CM") == 0)
			{
				strcpy(cm_oids[cm_q].oid_str, row[1]);
				strcpy(cm_oids[cm_q].desc, row[2]);
				cm_oids[cm_q].objid_len = MAX_OID_LEN;
				read_objid(row[1], cm_oids[cm_q].objid, &cm_oids[cm_q].objid_len);
				cm_q++;
			}
		}
	}

	cmts_oids[1].objid_len = (int)cmts_oids[1].objid_len + 1;
	cmts_oids[2].objid_len = (int)cmts_oids[2].objid_len + 1;
	cmts_oids[3].objid_len = (int)cmts_oids[3].objid_len + 1;

	/* --------------- CARGA DE CMTS --------------- */

	mysql_query(conexion, "SELECT * FROM cm_data.equipos WHERE 1");
	res = mysql_store_result(conexion);

	q_eq = 0;

	if(mysql_num_rows(res))
	{
		for(i=0;i < mysql_num_rows(res); i++)
		{
			row = mysql_fetch_row(res);
			strcpy(cmts_list[i].cmts_id, row[0]);
			strcpy(cmts_list[i].desc, row[1]);
			strcpy(cmts_list[i].cmts_ip, row[2]);
			strcpy(cmts_list[i].comm, row[3]);
			q_eq++;
		}
	}
//	else
//	{
		

	/* ---------------- CONFIGURACIONES --------------- */

	mysql_query(conexion, "SELECT * FROM cm_data.config WHERE 1");
	res = mysql_store_result(conexion);

	if(mysql_num_rows(res))
	{
		for(i=0;i < mysql_num_rows(res); i++)
		{
			row = mysql_fetch_row(res);
			if(strcmp(row[1], "log_file") == 0)
			{
				strcpy(cfg.log_file, row[2]);
			}
			else if(strcmp(row[1], "procesos") == 0)
			{
				cfg.procesos = atoi(row[2]);
			}
			else if(strcmp(row[1], "rrd_path") == 0)
			{
				strcpy(cfg.rrd_path, row[2]);
			}
			else if(strcmp(row[1], "cm_comm") == 0)
			{
				strcpy(cfg.cm_comm, row[2]);
			}
			else
			{
				sprintf(log, "Parametro de configuracion desconocido: %s - Valor: %s", row[1], row[2]);
				logger(cfg.log_file, log);
			}
		}
	}
	else
	{
		logger("/var/log/cmgraph.log", "No se cargaron configuraciones de la base de datos. Cargando valores por defecto.");
		strcpy(cfg.log_file, "/var/log/cmgraph.log");
		strcpy(cfg.rrd_path, "/var/rrd/modems/");
		cfg.procesos = 8;
	}

	struct stat st = {0};

	if (stat(cfg.rrd_path, &st) == -1)
	{
		mkdir(cfg.rrd_path, 0777);
	}	
}
