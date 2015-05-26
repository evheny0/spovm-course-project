#ifndef MAIN_H
#define MAIN_H

#include <sys/time.h>
#include "main.h"
#include "print_to_file.h"
#include "interface.h"  


typedef struct {
    char *from;
    char *to;
    char *pid;
    int count;
} connection_item;


void process_packet(unsigned char *, int);
void out_tcp_packet(unsigned char *data, int size);
void out_udp_packet(unsigned char *data, int size);
void add_or_create_connection_item(char *from, char *to, char *pid, char *connect_type);
int check_f_params(int argc, char const *argv[]);
void open_log_file();

FILE *logfile;
int tcp = 0, udp = 0, icmp = 0, others = 0, igmp = 0, total = 0;
connection_item *connections = (connection_item *) malloc(SIZE_OF_BUFFER);
char **connects_list = (char **) malloc(SIZE_OF_BUFFER);
int connections_num = 0;
int print_to_file = 0;
extern struct sockaddr_in source, dest;


#include <netinet/if_ether.h>

#endif // MAIN_H
