#include "header.h"

void logger(char *logpath, char *cadena)
{
	char *log_data, *t_stamp;
	int logfile;
	time_t fecha;

	logfile = open(logpath, O_CREAT | O_WRONLY | O_APPEND, 0666);

	if(logfile != 0)
	{
		fecha = time(NULL);
		t_stamp = ctime(&fecha);
		t_stamp[strlen(t_stamp)-1] = 0;
		log_data = (char *)malloc(strlen(cadena)+strlen(t_stamp));
		sprintf(log_data, "%s - %s\n", t_stamp, cadena);		
		write(logfile, log_data, strlen(log_data));			
		free(log_data);
		close(logfile);
	}
}
