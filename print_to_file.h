#ifndef PRINT_TO_FILE_H
#define PRINT_TO_FILE_H

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


void print_ip_header(unsigned char *, int);
void print_tcp_packet(unsigned char *, int);
void print_udp_packet(unsigned char *, int);
void print_icmp_packet(unsigned char *, int);
void print_data(unsigned char *, int);
void find_pid_in_ss(int port, char *buffer);


extern FILE *logfile;
// int i, j;
const int STRING_LENGTH = 256;
const int SIZE_OF_BUFFER = 65536;




#endif // PRINT_TO_FILE_H
