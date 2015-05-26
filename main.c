#include "main.h"

int main(int argc, char const *argv[])
{
    int saddr_size, data_size;
    struct sockaddr saddr;
    if (check_f_params(argc, argv)) {
        print_to_file = 1;
    } else {
        init_window();
    }

    unsigned char *buffer = (unsigned char *) malloc(SIZE_OF_BUFFER);

    logfile = fopen("log.txt", "w");
    if (logfile == NULL) {
        printf("Unable to create file.");
    }

    int sock_raw = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sock_raw < 0) {
        printf("Socket Error\n");
        return 1;
    }
    while (1) {
        if (check_keys()) {
            break;
        }
        saddr_size = sizeof saddr;
        data_size = recvfrom(sock_raw, buffer, SIZE_OF_BUFFER, MSG_DONTWAIT, &saddr, (socklen_t *) &saddr_size);
        if (data_size < 0) {
            continue;
        }
        process_packet(buffer, data_size);
    }
    close(sock_raw);
    printf("Finished");
    return 0;
}

void process_packet(unsigned char *buffer, int size)
{
    struct iphdr *iph = (struct iphdr *) buffer;
    ++total;
    switch (iph->protocol) {
    case IPPROTO_ICMP:
        ++icmp;
        print_icmp_packet(buffer, size);
        break;

    case IPPROTO_IGMP:
        ++igmp;
        break;

    case IPPROTO_TCP:
        ++tcp;
        if (print_to_file) {
            print_tcp_packet(buffer, size);
        } else {
            out_tcp_packet(buffer, size);
        }
        break;

    case IPPROTO_UDP:
        ++udp;
        print_udp_packet(buffer, size);
        break;

    default:
        ++others;
        break;
    }
    // printf("TCP : %d   UDP : %d   ICMP : %d   IGMP : %d   Others : %d   Total : %d\r",
    //        tcp, udp, icmp, igmp, others, total);
}

void out_tcp_packet(unsigned char *data, int size)
{
    unsigned short iphdrlen;
    struct iphdr *iph = (struct iphdr *) data;
    iphdrlen = iph->ihl * 4;
    struct tcphdr *tcph = (struct tcphdr *) (data + iphdrlen);

    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;

    char from[STRING_LENGTH], to[STRING_LENGTH], pid[STRING_LENGTH];
    find_pid_in_ss(ntohs(tcph->dest), pid);
    sprintf(from, "%s:%u", inet_ntoa(source.sin_addr), ntohs(tcph->source));
    sprintf(to, "%s:%u", inet_ntoa(dest.sin_addr), ntohs(tcph->dest));
    add_or_create_connection_item(from, to, pid + 86);
}

void add_or_create_connection_item(char *from, char *to, char *pid)
{
    for (int i = 0; i < connections_num; ++i) {
        if (!strcmp(connections[i].to, to) && !strcmp(connections[i].from, from)) {
            connections[i].count++;
            connects_list[i][70] = 0;
            sprintf(connects_list[i], "%70s %d", connects_list[i], connections[i].count);
            return;
        }
    }
    // printf("%d\n", connections_num);
    char *to_new = (char *) calloc(STRING_LENGTH, sizeof(char));
    char *from_new = (char *) calloc(STRING_LENGTH, sizeof(char));
    char *pid_new = (char *) calloc(STRING_LENGTH, sizeof(char));
    char *connect_string_new = (char *) calloc(STRING_LENGTH, sizeof(char));

    strcpy(to_new, to);
    strcpy(from_new, from);
    strcpy(pid_new, pid);
    sprintf(connect_string_new, "%21s %21s %10s     1", from, to, pid);
    // puts(connect_string_new);
    connections[connections_num].to = to_new;
    connections[connections_num].from = from_new;
    connections[connections_num].pid = pid_new;
    connects_list[connections_num] = connect_string_new;
    connections_num++;
    refresh_screen();
    // printf("%21s  > %21s  %s", from, to, pid);
}

void find_pid_in_ss(int port, char *buffer)
{
    char cmd[256];
    FILE *pipe;
    snprintf(cmd, sizeof(cmd), "ss -tp | grep :%d", port);

    if ((pipe = popen(cmd, "r")) == NULL) {
        puts(" ** ERR");
        exit(1);
    }

    fgets(buffer, STRING_LENGTH, pipe);
    pclose(pipe);
}

int check_f_params(int argc, char const *argv[])
{
    if ((argc > 1) && !strcmp(argv[1], "-f")) {
        return 1;
    }
    return 0;
}
