#include "header.h"

void get_cm_data(cablemodem cm)
{
	netsnmp_session cm_session, *cm_ss;
	netsnmp_pdu    *cm_pdu, *cm_response;
	netsnmp_variable_list *cm_vars;
	char *valor, log[256];
	struct oid_data *aux;
	int i, k;

    SOCK_STARTUP;

	snmp_sess_init(&cm_session);
	cm_session.version = SNMP_VERSION_1;
	cm_session.community = cfg.cm_comm;
	cm_session.community_len = strlen(cm_session.community);
	cm_session.peername = cm.cm_ip;//printf("%s %s\n", cm.cm_ip, cfg.cm_comm);
	cm_session.retries = 3;
	cm_session.timeout = 1000000;
    cm_ss = snmp_open(&cm_session);

    if(cm_ss == NULL)
	{
		SOCK_CLEANUP;
		sprintf(log, "Error de snmp_open contra el cm: %s", cm.cm_mac);
		logger(cfg.log_file, log);
    }
	else
	{
		for(i=0; i<cm_q; i++)
		{
			valor = snmp_get(cm_ss, cm_oids[i]);

			if(strcmp(cm_oids[i].desc, "DS1_PWR") == 0)
			{
				strcpy(cm.ds1_pwr, valor);
			}
			else if(strcmp(cm_oids[i].desc, "DS2_PWR") == 0)
			{
				strcpy(cm.ds2_pwr, valor);
			}
			else if(strcmp(cm_oids[i].desc, "DS3_PWR") == 0)
			{
				strcpy(cm.ds3_pwr, valor);
			}
			else if(strcmp(cm_oids[i].desc, "DS4_PWR") == 0)
			{
				strcpy(cm.ds4_pwr, valor);
			}
			else if(strcmp(cm_oids[i].desc, "DS1_SNR") == 0)
			{
				strcpy(cm.ds1_snr, valor);
			}
			else if(strcmp(cm_oids[i].desc, "DS2_SNR") == 0)
			{
				strcpy(cm.ds2_snr, valor);
			}
			else if(strcmp(cm_oids[i].desc, "DS3_SNR") == 0)
			{
				strcpy(cm.ds3_snr, valor);
			}
			else if(strcmp(cm_oids[i].desc, "DS4_SNR") == 0)
			{
				strcpy(cm.ds4_snr, valor);
			}
			else if(strcmp(cm_oids[i].desc, "US1_PWR") == 0)
			{
				strcpy(cm.us1_pwr, valor);
			}
			else if(strcmp(cm_oids[i].desc, "MIC") == 0)
			{
				strcpy(cm.mic, valor);
			}
			else if(strcmp(cm_oids[i].desc, "DS1_TF") == 0)
			{
				strcpy(cm.ds1_tf, valor);
			}
			else if(strcmp(cm_oids[i].desc, "DS2_TF") == 0)
			{
				strcpy(cm.ds2_tf, valor);
			}
			else if(strcmp(cm_oids[i].desc, "DS3_TF") == 0)
			{
				strcpy(cm.ds3_tf, valor);
			}
			else if(strcmp(cm_oids[i].desc, "DS4_TF") == 0)
			{
				strcpy(cm.ds4_tf, valor);
			}
			else if(strcmp(cm_oids[i].desc, "US1_TF") == 0)
			{
				strcpy(cm.us1_tf, valor);
			}
			else if(strcmp(cm_oids[i].desc, "US2_TF") == 0)
			{
				strcpy(cm.us2_tf, valor);
			}
			else if(strcmp(cm_oids[i].desc, "US3_TF") == 0)
			{
				strcpy(cm.us3_tf, valor);
			}
			free(valor);
		}
	}

	snmp_close(cm_ss);
	SOCK_CLEANUP;
	//printf("%s %s %s %s %s %s %s %s %s %s\nUpdateando...\n", cm.cm_mac, cm.cm_ip, cm.us_snr, cm.us_pwr_cmts, cm.ds1_pwr, cm.ds1_snr, cm.us1_pwr, cm.mic, cm.ds1_tf, cm.us1_tf);
	update(cm);
}
