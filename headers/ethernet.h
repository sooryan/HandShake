#ifndef ETHER_H
#define ETHER_H
#include <linux/if_ether.h>

// Ethernet header declaration
struct eth_hdr{
    unsigned char dst_mac[6];
    unsigned char src_mac[6];
    unsigned short ethertype;
    unsigned char payload[];
};

struct eth_hdr* to_eth_hdr(char* buf);
void print_eth_hdr(struct eth_hdr *ehdr);

#endif
