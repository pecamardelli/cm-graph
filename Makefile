CFLAGS=-I/usr/include/mysql -DBIG_JOINS=1  -fno-strict-aliasing   -g -DNDEBUG
SLIBS=-L/usr/lib/x86_64-linux-gnu -lmysqlclient -lpthread -lz -lm -ldl -lrrd

cmgraph: main.o snmp_get.o configuracion.o logger.o snmp_walk.o get_cm_data.o parse_val.o update.o mysql_con.o
	gcc -o cmgraph $(CFLAGS) `net-snmp-config --cflags --libs --external-libs` main.o snmp_get.o configuracion.o logger.o snmp_walk.o get_cm_data.o parse_val.o update.o  mysql_con.o $(SLIBS)

main.o: main.c header.h
	gcc -c main.c

snmp_get.o: snmp_get.c header.h
	gcc -c snmp_get.c

configuracion.o: configuracion.c header.h
	gcc -c configuracion.c

logger.o: logger.c header.h
	gcc -c logger.c

snmp_walk.o: snmp_walk.c header.h
	gcc -c snmp_walk.c

get_cm_data.o: get_cm_data.c header.h
	gcc -c get_cm_data.c

parse_val.o: parse_val.c header.h
	gcc -c parse_val.c

update.o: update.c header.h
	gcc -c update.c

mysql_con.o: mysql_con.c header.h
	gcc -c mysql_con.c

clean:
	rm *.o
