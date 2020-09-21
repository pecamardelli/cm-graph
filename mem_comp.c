#include "header.h"

cm *mem_comp(key_t key, size_t size, mode_t mode){

	cm *ptr;

	mem_id = shmget(key, (sizeof(cm)*256), mode | IPC_CREAT);

	if(mem_id < 0){
		return NULL;
	}

	ptr = shmat(mem_id, NULL, 0);	//se  obtiene un puntero que apunte la zona de memoria. char * shmat (int, char *, int).

	return ptr;
}
