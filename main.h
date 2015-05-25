#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>              //For standard things
#include <stdlib.h>             //malloc
#include <string.h>             //memset
#include <netinet/ip_icmp.h>    //Provides declarations for icmp header
#include <netinet/udp.h>        //Provides declarations for udp header
#include <netinet/tcp.h>        //Provides declarations for tcp header
#include <netinet/ip.h>         //Provides declarations for ip header
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
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
void add_or_create_connection_item(char *from, char *to, char *pid);

FILE *logfile;
int tcp = 0, udp = 0, icmp = 0, others = 0, igmp = 0, total = 0;
connection_item *connections = (connection_item *) malloc(SIZE_OF_BUFFER);
char **connects_list = (char **) malloc(SIZE_OF_BUFFER);
int connections_num = 0;
extern struct sockaddr_in source, dest;


#include <netinet/if_ether.h>

#endif // MAIN_H
