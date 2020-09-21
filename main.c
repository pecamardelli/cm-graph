#include "header.h"

int main(int argc, char ** argv)
{
	int i;

	logger("/var/log/cmgraph.log", "CMGRAPH 2.0 iniciado...");

	configuracion();
	
	/* VARIABLES DEL SEMAFORO */

	clave = ftok(".", 33);
	idsem = semget(clave, 1, 0666 | IPC_CREAT); 
	arg.val = 0;
	semctl(idsem, 0, SETVAL, &arg);
	operacion.sem_num = 0;
	operacion.sem_op = 1;
	operacion.sem_flg = 0;
	semop(idsem, &operacion, 1);

	/* ---------- COMIENZO DE FORK ---------- */

	if(pipe(pipes) < 0)
	{
		logger(cfg.log_file, "Error al crear pipes.");
		exit(1);
	}

	for(p=0;p<cfg.procesos;p++)
	{
		pids[p] = fork();
		if(pids[p] == 0)
		{
			int nread;
			cablemodem cm;
			close(pipes[1]);
			char log[256];

			while(1)
			{
				if(pids[p] == token)
				{
					nread=read(pipes[0], &cm, sizeof(cablemodem));

					if(nread > 0)
					{
						if(strcmp(cm.cm_ip, "0.0.0.0") != 0)
						{
							get_cm_data(cm);
						}
					}
					else
					{
						sprintf(log, "PID: %d: error al leer en pipe %d (%d: %s)", (int)getpid(), pipes[1], errno, strerror(errno));
						logger(cfg.log_file, log);
					}
				}
				else
				{
					usleep(1000);
				}
			}
			exit(0);
		}
	}

	for(i=0;i<q_eq;i++)
	{
		if(fork() == 0)
		{
			//close(pipes[0]);
			char log[256];
			time_t ciclo[2];
			int walk_stat, duracion;

			while(1)
			{
				configuracion();
				ciclo[0] = time(NULL);
				walk_stat = snmp_walk(cmts_list[i]);
				ciclo[1] = time(NULL);
				if(walk_stat == 1)
				{
					sprintf(log, "Error en walk del peer %s. Se retorno el valor 1", cmts_list[i].cmts_ip);
					logger(cfg.log_file, log);
				}
				else
				{
					duracion = ciclo[1] - ciclo[0];
					sprintf(log, "Duracion del walk del %s (%s): %ld segundos", cmts_list[i].cmts_ip, cmts_list[i].desc, duracion);
					logger(cfg.log_file, log);
				}

				if(duracion < 600)
				{
					sleep(600 - duracion);
				}
			}
			exit(0);
		}
	}

	/* ------------------------------------- */

	while (wait(NULL) > 0)
    {
		printf("esperando...\n");
    }
	printf("Listo...\n");
	

	return (0);
}
