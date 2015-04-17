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

void ProcessPacket(unsigned char *, int);
void print_ip_header(unsigned char *, int);
void print_tcp_packet(unsigned char *, int);
void print_udp_packet(unsigned char *, int);
void print_icmp_packet(unsigned char *, int);
void PrintData(unsigned char *, int);

int sock_raw;
FILE *logfile;
int tcp = 0, udp = 0, icmp = 0, others = 0, igmp = 0, total = 0, i, j;
struct sockaddr_in source, dest;


#endif // MAIN_H
