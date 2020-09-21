#ifndef HEADER_H
   #define HEADER_H

	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	#include <mysql/mysql.h>
	#include <sys/select.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <sys/fcntl.h>
	#include <sys/unistd.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netdb.h>
	#include <arpa/inet.h>
	#include <time.h>
	#include <pthread.h>
	#include <sys/ipc.h>
	#include <sys/sem.h>
	#include <sys/shm.h>
	#include <sys/wait.h>
	#include <math.h>
	#include <errno.h>
	#include <rrd.h>
	#include <stdarg.h>

	#include <net-snmp/net-snmp-config.h>
	#include <net-snmp/net-snmp-includes.h>
	#include <net-snmp/mib_api.h>

	#define NETSNMP_DS_WALK_INCLUDE_REQUESTED	1
	#define NETSNMP_DS_WALK_PRINT_STATISTICS	2
	#define NETSNMP_DS_WALK_DONT_CHECK_LEXICOGRAPHIC	3

	typedef struct oid_data
	{
		char oid_str[128];
		char desc[32];
		oid objid[MAX_OID_LEN];
		size_t objid_len;
	}oids;

	oids cmts_oids[10], cm_oids[20];

	union semun
	{
		int val;
		struct semid_ds *buf;
		unsigned short int *array;
		struct seminfo *_buf;
	};

	key_t clave;
	int idsem;
	struct sembuf operacion;
	union semun arg;

	/*---------------------------------------*/

	typedef struct CMTS
	{
		char cmts_id[4];
		char desc[64];
		char cmts_ip[16];
		char comm[32];
	}cmts;

	cmts cmts_list[10];
	int q_eq;

	typedef struct CM
	{
		char cm_mac[16];
		char cm_ip[16];
		char us_snr[4];
		char us_pwr_cmts[4];
		char ds1_pwr[4], ds2_pwr[4], ds3_pwr[4], ds4_pwr[4];
		char ds1_snr[4], ds2_snr[4], ds3_snr[4], ds4_snr[4];
		char us1_pwr[4];
		char mic[4];
		char ds1_tf[16], ds2_tf[16], ds3_tf[16], ds4_tf[16];
		char us1_tf[16], us2_tf[16], us3_tf[16];
	}cablemodem;

	typedef struct CONFIG
	{
		int procesos;	
		char log_file[64];
		char rrd_path[192];
		char cm_comm[64];
	}config;

	config cfg;
	int p, pipes[2], cmts_q, cm_q, pids[16], token;

	/* ----------------- FUNCIONES ------------------- */

	void configuracion();
	void lib_rec();
	MYSQL *mysql_con(void);
	char *snmp_get(netsnmp_session *, oids);
	int snmp_walk(cmts);
	void get_cm_data(cablemodem);
	void logger(char *, char *);
	char *parse_val(netsnmp_vardata, u_char);
	void update(cablemodem);
	cm *mem_comp(key_t, size_t, mode_t);

	/* ------------------------------------------------- */

#endif
