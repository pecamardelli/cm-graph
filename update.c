#include "header.h"

void update(cablemodem cm)
{
	char data[512], cm_db[256], cm_db_bw[256];
	char log[128];
	char comando[128];

	sprintf(cm_db, "%s%s.rrd", cfg.rrd_path, cm.cm_mac);
	sprintf(cm_db_bw, "%s%s_bw.rrd", cfg.rrd_path, cm.cm_mac);

	if(access(cm_db, F_OK) == -1)
	{
		char *create_rrd[] = {
			"rrdcreate",
			cm_db,
			"DS:DS1_PWR:GAUGE:900:-200:600",
			"DS:DS2_PWR:GAUGE:900:-200:600",
			"DS:DS3_PWR:GAUGE:900:-200:600",
			"DS:DS4_PWR:GAUGE:900:-200:600",
			"DS:DS1_SNR:GAUGE:900:0:600",
			"DS:DS2_SNR:GAUGE:900:0:600",
			"DS:DS3_SNR:GAUGE:900:0:600",
			"DS:DS4_SNR:GAUGE:900:0:600",
			"DS:US_PWR:GAUGE:900:0:600",
			"DS:US_SNR:GAUGE:900:0:600",
			"DS:US_CMTS_PWR:GAUGE:900:-200:200",
			"DS:MIC:GAUGE:900:0:600",
			"RRA:MIN:0.5:6:720",
			"RRA:MAX:0.5:6:720",
			"RRA:AVERAGE:0.5:1:4320",
			NULL
	    };

		rrd_clear_error();
		if(rrd_create(17, create_rrd) != 0)
		{
			sprintf(log, "No se pudo crear la base de datos %s.rrd", cm.cm_mac);
			logger(cfg.log_file, log);
		}


		sprintf(comando, "chmod 777 %s", cm_db);
		system(comando);
	}

	if(access(cm_db_bw, F_OK) == -1)
	{
		char *create_rrd_bw[] = {
			"rrdcreate",
			cm_db_bw,
			"DS:DS1_TF:DERIVE:1800:0:U",
			"DS:DS2_TF:DERIVE:1800:0:U",
			"DS:DS3_TF:DERIVE:1800:0:U",
			"DS:DS4_TF:DERIVE:1800:0:U",
			"DS:US1_TF:DERIVE:1800:0:U",
			"DS:US2_TF:DERIVE:1800:0:U",
			"DS:US3_TF:DERIVE:1800:0:U",
			"RRA:MIN:0.5:6:720",
			"RRA:MAX:0.5:6:720",
			"RRA:AVERAGE:0.5:1:4320",
			NULL
	    };

		rrd_clear_error();
		if(rrd_create(12, create_rrd_bw) != 0)
		{
			sprintf(log, "No se pudo crear la base de datos %s_bw.rrd", cm.cm_mac);
			logger(cfg.log_file, log);
		}


		sprintf(comando, "chmod 777 %s", cm_db);
		system(comando);
	}

	sprintf(data, "N:%s:%s:%s:%s:%s:%s:%s:%s:%s:%s:%s:%s", cm.ds1_pwr, cm.ds2_pwr, cm.ds3_pwr, cm.ds4_pwr, cm.ds1_snr, cm.ds2_snr, cm.ds3_snr, cm.ds4_snr, cm.us1_pwr, cm.us_snr, cm.us_pwr_cmts, cm.mic);

	char *update_rrd[] = { 
		"rrdupdate", 
		cm_db, 
		data, 
		NULL 
	};

	rrd_clear_error();
	//printf("%s %s %s\n", update_rrd[0], update_rrd[1], update_rrd[2]);
	if(rrd_update(3, update_rrd) != 0)
	{
		sprintf(log, "No se pudo actualizar la base de datos %s.rrd", cm.cm_mac);
		logger(cfg.log_file, log);
	}

	sprintf(data, "N:%s:%s:%s:%s:%s:%s:0", cm.ds1_tf, cm.ds2_tf, cm.ds3_tf, cm.ds4_tf, cm.us1_tf, cm.us2_tf);

	char *update_rrd_bw[] = {
		"rrdupdate", 
		cm_db_bw, 
		data, 
		NULL 
	};

	rrd_clear_error();
	//printf("%s %s %s\n", update_rrd[0], update_rrd[1], update_rrd[2]);
	if(rrd_update(3, update_rrd_bw) != 0)
	{
		sprintf(log, "No se pudo actualizar la base de datos %s_bw.rrd", cm.cm_mac);
		logger(cfg.log_file, log);
	}
}
