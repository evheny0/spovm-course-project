#include "print_to_file.h"

struct sockaddr_in source, dest;


void print_ip_header(unsigned char *data, int size)
{
    struct iphdr *iph = (struct iphdr *) (data + sizeof(struct ethhdr));

    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;

    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;

    fprintf(logfile, "\n");
    fprintf(logfile, "IP Header\n");
    fprintf(logfile, "   |-IP Version        : %d\n", (unsigned int) iph->version);
    fprintf(logfile, "   |-IP Header Length  : %d DWORDS or %d Bytes\n",
            (unsigned int) iph->ihl, ((unsigned int) (iph->ihl)) * 4);
    fprintf(logfile, "   |-Type Of Service   : %d\n", (unsigned int) iph->tos);
    fprintf(logfile,
            "   |-IP Total Length   : %d  Bytes(Size of Packet)\n",
            ntohs(iph->tot_len));
    fprintf(logfile, "   |-Identification    : %d\n", ntohs(iph->id));
    fprintf(logfile, "   |-TTL      : %d\n", (unsigned int) iph->ttl);
    fprintf(logfile, "   |-Protocol : %d\n", (unsigned int) iph->protocol);
    fprintf(logfile, "   |-Checksum : %d\n", ntohs(iph->check));
    fprintf(logfile, "   |-Source IP        : %s\n", inet_ntoa(source.sin_addr));
    fprintf(logfile, "   |-Destination IP   : %s\n", inet_ntoa(dest.sin_addr));
}

void print_tcp_packet(unsigned char *data, int size)
{
    unsigned short iphdrlen;
    char pid_buffer[STRING_LENGTH];

    struct iphdr *iph = (struct iphdr *) (data + sizeof(struct ethhdr));
    iphdrlen = iph->ihl * 4;

    struct tcphdr *tcph = (struct tcphdr *) (data + iphdrlen + sizeof(struct ethhdr));

    fprintf(logfile, "\n\n***********************TCP Packet*************************\n");

    print_ip_header(data, size);

    fprintf(logfile, "\n");
    fprintf(logfile, "TCP Header\n");
    fprintf(logfile, "   |-Source Port      : %u\n", ntohs(tcph->source));
    find_pid_in_ss(ntohs(tcph->dest), pid_buffer);
    fprintf(logfile, "   |-PID              : %s", pid_buffer + 86);
    fprintf(logfile, "   |-Destination Port : %u\n", ntohs(tcph->dest));
    fprintf(logfile, "   |-Sequence Number    : %u\n", ntohl(tcph->seq));
    fprintf(logfile, "   |-Acknowledge Number : %u\n", ntohl(tcph->ack_seq));
    fprintf(logfile, "   |-Header Length      : %d DWORDS or %d BYTES\n",
            (unsigned int) tcph->doff, (unsigned int) tcph->doff * 4);
    fprintf(logfile, "   |-Urgent Flag          : %d\n", (unsigned int) tcph->urg);
    fprintf(logfile, "   |-Acknowledgement Flag : %d\n", (unsigned int) tcph->ack);
    fprintf(logfile, "   |-Push Flag            : %d\n", (unsigned int) tcph->psh);
    fprintf(logfile, "   |-Reset Flag           : %d\n", (unsigned int) tcph->rst);
    fprintf(logfile, "   |-Synchronise Flag     : %d\n", (unsigned int) tcph->syn);
    fprintf(logfile, "   |-Finish Flag          : %d\n", (unsigned int) tcph->fin);
    fprintf(logfile, "   |-Window         : %d\n", ntohs(tcph->window));
    fprintf(logfile, "   |-Checksum       : %d\n", ntohs(tcph->check));
    fprintf(logfile, "   |-Urgent Pointer : %d\n", tcph->urg_ptr);
    fprintf(logfile, "\n");
    fprintf(logfile, "                        DATA Dump");
    fprintf(logfile, "\n");

    // fprintf(logfile, "IP Header\n");
    // print_data(data, iphdrlen);

    // fprintf(logfile, "TCP Header\n");
    // print_data(data + iphdrlen, tcph->doff * 4);

    fprintf(logfile, "Data Payload\n");
    print_data(data + iphdrlen + tcph->doff * 4,
              (size - tcph->doff * 4 - iph->ihl * 4));

    fprintf(logfile, "\n###########################################################");
}

void print_udp_packet(unsigned char *Buffer, int size)
{

    unsigned short iphdrlen;

    struct iphdr *iph = (struct iphdr *) (Buffer + sizeof(struct ethhdr));
    iphdrlen = iph->ihl * 4;

    struct udphdr *udph = (struct udphdr *) (Buffer + iphdrlen + sizeof(struct ethhdr));

    fprintf(logfile, "\n\n***********************UDP Packet*************************\n");

    print_ip_header(Buffer, size);

    fprintf(logfile, "\nUDP Header\n");
    fprintf(logfile, "   |-Source Port      : %d\n", ntohs(udph->source));
    fprintf(logfile, "   |-Destination Port : %d\n", ntohs(udph->dest));
    fprintf(logfile, "   |-UDP Length       : %d\n", ntohs(udph->len));
    fprintf(logfile, "   |-UDP Checksum     : %d\n", ntohs(udph->check));

    fprintf(logfile, "\n");
    fprintf(logfile, "IP Header\n");
    print_data(Buffer, iphdrlen);

    fprintf(logfile, "UDP Header\n");
    print_data(Buffer + iphdrlen, sizeof udph);

    fprintf(logfile, "Data Payload\n");
    print_data(Buffer + iphdrlen + sizeof udph,
              (size - sizeof udph - iph->ihl * 4));

    fprintf(logfile, "\n###########################################################");
}

void print_icmp_packet(unsigned char *buffer, int size)
{
    unsigned short iphdrlen;

    struct iphdr *iph = (struct iphdr *) (buffer + sizeof(struct ethhdr));
    iphdrlen = iph->ihl * 4;

    struct icmphdr *icmph = (struct icmphdr *) (buffer + iphdrlen + sizeof(struct ethhdr));

    fprintf(logfile, "\n\n***********************ICMP Packet*************************\n");

    print_ip_header(buffer, size);

    fprintf(logfile, "\n");

    fprintf(logfile, "ICMP Header\n");
    fprintf(logfile, "   |-Type : %d", (unsigned int) (icmph->type));

    if ((unsigned int) (icmph->type) == 11) {
        fprintf(logfile, "  (TTL Expired)\n");
    } else if ((unsigned int) (icmph->type) == ICMP_ECHOREPLY){
        fprintf(logfile, "  (ICMP Echo Reply)\n");
    }
    fprintf(logfile, "   |-Code : %d\n", (unsigned int) (icmph->code));
    fprintf(logfile, "   |-Checksum : %d\n", ntohs(icmph->checksum));
    fprintf(logfile, "\n");

    fprintf(logfile, "IP Header\n");
    print_data(buffer, iphdrlen);

    fprintf(logfile, "UDP Header\n");
    print_data(buffer + iphdrlen, sizeof icmph);

    fprintf(logfile, "Data Payload\n");
    print_data(buffer + iphdrlen + sizeof icmph,
              (size - sizeof icmph - iph->ihl * 4));

    fprintf(logfile,
            "\n###########################################################");
}

void print_data(unsigned char *data, int size)
{
    int i, j;
    for (i = 0; i < size; i++) {
        if (i != 0 && i % 16 == 0) {    //if one line of hex printing is complete...
            fprintf(logfile, "         ");
            for (j = i - 16; j < i; j++) {
                if (data[j] >= 32 && data[j] <= 128) {    //if its a number or alphabet
                    fprintf(logfile, "%c", (unsigned char) data[j]);
                } else {
                    fprintf(logfile, ".");
                }
            }
            fprintf(logfile, "\n");
        }

        if (i % 16 == 0) {
            fprintf(logfile, "   ");
        }

        fprintf(logfile, " %02X", (unsigned int) data[i]);

        if (i == size - 1) {    //print the last spaces
            for (j = 0; j < 15 - i % 16; j++) {
                fprintf(logfile, "   ");        //extra spaces
            }

            fprintf(logfile, "         ");

            for (j = i - i % 16; j <= i; j++) {
                if (data[j] >= 32 && data[j] <= 128) {
                    fprintf(logfile, "%c", (unsigned char) data[j]);
                } else {
                    fprintf(logfile, ".");
                }
            }
            fprintf(logfile, "\n");
        }
    }
}
