#include "header.h"

int snmp_walk(cmts peer)
{
    netsnmp_session session, *ss;
    netsnmp_pdu    *pdu, *response;
    netsnmp_variable_list *vars;
    oid             name[MAX_OID_LEN];
	oid				root[MAX_OID_LEN];
    size_t          name_length;
	size_t			root_length;
    int             running;
    int             status;
    int             check;
    int             exitval = 0;
	char			log[256], *valor;
	int				length;
	int				i,z,m = 0;
	int nwrite;

	cablemodem	cm = { "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0" };

    netsnmp_ds_register_config(ASN_BOOLEAN, "snmpwalk", "includeRequested",
			       NETSNMP_DS_APPLICATION_ID, 
			       NETSNMP_DS_WALK_INCLUDE_REQUESTED);

    netsnmp_ds_register_config(ASN_BOOLEAN, "snmpwalk", "printStatistics",
			       NETSNMP_DS_APPLICATION_ID, 
			       NETSNMP_DS_WALK_PRINT_STATISTICS);

    netsnmp_ds_register_config(ASN_BOOLEAN, "snmpwalk", "dontCheckOrdering",
			       NETSNMP_DS_APPLICATION_ID,
			       NETSNMP_DS_WALK_DONT_CHECK_LEXICOGRAPHIC);

    SOCK_STARTUP;

    /*
     * open an SNMP session 
     */

	init_snmp("MODEMS");
	snmp_sess_init( &session );
	session.version = SNMP_VERSION_1;
	session.community = peer.comm;
	session.community_len = strlen(session.community);
	session.peername = peer.cmts_ip;
	session.retries = 3;
	session.timeout = 1000000;
    ss = snmp_open(&session);

    if (ss == NULL) {
        /*
         * diagnose snmp_open errors with the input netsnmp_session pointer 
         */
        snmp_sess_perror("snmpwalk", &session);
        SOCK_CLEANUP;
		sprintf(log, "Error de snmp_open contra el peer: %s", peer);
		logger(cfg.log_file, log);
		return 1;
    }

	memmove(name, cmts_oids[0].objid, cmts_oids[0].objid_len * sizeof(oid));
	name_length = cmts_oids[0].objid_len;

    running = 1;
	z = 0;

    check = !netsnmp_ds_get_boolean(NETSNMP_DS_APPLICATION_ID, NETSNMP_DS_WALK_DONT_CHECK_LEXICOGRAPHIC);

    while (running) {
        /*
         * create PDU for GETNEXT request and add object name to request 
         */
        pdu = snmp_pdu_create(SNMP_MSG_GETNEXT);
        snmp_add_null_var(pdu, name, name_length);

        /*
         * do the request 
         */
        status = snmp_synch_response(ss, pdu, &response);
        if (status == STAT_SUCCESS) {
            if (response->errstat == SNMP_ERR_NOERROR) {
                /*
                 * check resulting variables 
                 */
                for (vars = response->variables; vars;
                     vars = vars->next_variable) {
                    if ((vars->name_length < cmts_oids[0].objid_len)
                        || (memcmp(cmts_oids[0].objid, vars->name, cmts_oids[0].objid_len * sizeof(oid))
                            != 0)) {
                        /*
                         * not part of this subtree 
                         */
                        running = 0;
                        continue;
                    }

                    if ((vars->type != SNMP_ENDOFMIBVIEW) &&
                        (vars->type != SNMP_NOSUCHOBJECT) &&
                        (vars->type != SNMP_NOSUCHINSTANCE)) {
                        /*
                         * not an exception value 
                         */
                        if (check
                            && snmp_oid_compare(name, name_length, vars->name, vars->name_length) >= 0) {
                            sprintf(log, "Error: OID not increasing");
							logger(cfg.log_file, log);
                            running = 0;
                            exitval = 1;
                        }

			            sprintf(cm.cm_mac, "%s", parse_val(vars->val, vars->type));

						for(i=1; i<cmts_q; i++)
						{
							cmts_oids[i].objid[cmts_oids[i].objid_len-1] = vars->name[vars->name_length-1];

							valor = snmp_get(ss, cmts_oids[i]);

							if(strcmp(cmts_oids[i].desc, "CM_IP") == 0)
							{
								strcpy(cm.cm_ip, valor);
							}
							else if(strcmp(cmts_oids[i].desc, "CM_US_SNR") == 0)
							{
								strcpy(cm.us_snr, valor);
							}
							else if(strcmp(cmts_oids[i].desc, "US_PWR_CMTS") == 0)
							{
								strcpy(cm.us_pwr_cmts, valor);
							}
							else
							{
								sprintf(log, "OID con descripcion desconocida: %s", cmts_oids[i].desc);
								logger(cfg.log_file, log);
							}
						}

						token = pids[z];printf("z: %d\tpids[z]: %d\ttoken: %d... \n", z, pids[z], token);

						nwrite = write(pipes[1], &cm, sizeof(cablemodem));
						m++;

						if(z == p)
						{
							z = 0;printf("ciclo cumplido\n");
						}
						else
						{
							z++;
						}

						printf("Modems de %s: %d\n", peer.cmts_ip, m);
						if(nwrite != sizeof(cablemodem))
						{
							sprintf(log, "%.5d: error al escribir en pipe %d (%d: %s)", (int)getpid(), pipes[0], errno, strerror(errno));
							logger(cfg.log_file, log);
							printf("\tFallo al escribir en pipe...\n");
						}

                        memmove((char *) name, (char *) vars->name,
                                vars->name_length * sizeof(oid));
                        name_length = vars->name_length;
                    } else
                        /*
                         * an exception value, so stop 
                         */
                        running = 0;
                	}
            } else {
                /*
                 * error in response, print it 
                 */
                running = 0;
                if (response->errstat == SNMP_ERR_NOSUCHNAME) {
                    logger(cfg.log_file, "End of MIB");
                } else {
					sprintf(log, "Error in packet. Reason: %s", snmp_errstring(response->errstat));
					logger(cfg.log_file, log);
                    exitval = 2;
                }
            }
        } else if (status == STAT_TIMEOUT) {
            sprintf(log, "Timeout: No Response from %s", session.peername);
			logger(cfg.log_file, log);
            //running = 0;
            exitval = 1;
        } else {                /* status == STAT_ERROR */
            snmp_sess_perror("snmpwalk", ss);
			sprintf(log, "Status = STAT_ERROR %s", session.peername);
			logger(cfg.log_file, log);
            running = 0;
            exitval = 1;
        }
        if (response)
            snmp_free_pdu(response);
    }

	snmp_close(ss);
    SOCK_CLEANUP;
    return exitval;
}
