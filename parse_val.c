#include "header.h"

char *parse_val(netsnmp_vardata val, u_char tipo)
{
	char *valor, mac[16];
	int length;
	struct sockaddr_in adr_inet;

	valor = (char *) malloc (32);

	switch(tipo)
	{
		case 2: 
				sprintf(valor, "%ld", *val.integer);
				break;
		case 4: 

				sprintf(mac, "%012lX", *val.integer);
				length = strlen(mac);
				sprintf(valor, "%c%c%c%c%c%c%c%c%c%c%c%c", mac[length-2], mac[length-1], mac[length-4], mac[length-3], mac[length-6], mac[length-5], mac[length-8], mac[length-7], mac[length-10], mac[length-9], mac[length-12], mac[length-11]);
				break;
		case 64: 
				adr_inet.sin_addr.s_addr = *val.integer;
				strcpy(valor, inet_ntoa(adr_inet.sin_addr));
				break;
		case 65:
				sprintf(valor, "%ld", *val.integer);
				break;
		default:
				strcpy(valor, "0");
	}

	return valor;
}
