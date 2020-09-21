#include "header.h"

char *snmp_get(netsnmp_session *ss, oids index)
{
    netsnmp_pdu    *pdu, *response;
    netsnmp_variable_list *vars;
    int             status, k;
	char *valor;

/*	printf("%s - %s - ", index.oid_str, index.desc);

	for(k=0; k<index.objid_len; k++)
	{
		printf("%d.", index.objid[k]);
	}
	printf("\n");*/

    pdu = snmp_pdu_create(SNMP_MSG_GET);
    snmp_add_null_var(pdu, index.objid, index.objid_len);

    status = snmp_synch_response(ss, pdu, &response);

    if(status == STAT_SUCCESS && response->errstat == SNMP_ERR_NOERROR)
	{
        for(vars = response->variables; vars; vars = vars->next_variable)
		{
			//print_variable(vars->name, vars->name_length, vars);
            valor = parse_val(vars->val, vars->type);
        }
    }
	else
	{
		valor = (char *) malloc (32);
		strcpy(valor, "0");
	}

    if(response)
	{
        snmp_free_pdu(response);
    }

	return valor;
}
