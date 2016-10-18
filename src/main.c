#include "arp.h"
#include "ethernet.h"
#include "stdheaders.h"
#include "tuntap.h"
#include "utils.h"
#include "netdev.h"

void handle_frame(struct netdev* netdev, struct eth_hdr* ehdr)
{
    switch (ehdr->ethertype) {
    case ETH_P_ARP:
        arp_incoming(netdev, ehdr);
        break;
    case ETH_P_IP:
        printf("Found IPv4\n");
        break;
    case ETH_P_IPV6:
        printf("Found IPv6\n");
        break;
    default:
        printf("Unrecognised ethertype\n");
        break;
    }
}

int main(int argc, char const* argv[])
{
    char buf[BUFLEN];
    char* dev = calloc(10, 1);
    struct netdev netdev;

    CLEAR(buf);
    tun_init(dev);
    arp_init();

    for (;;) {
        struct eth_hdr* ehdr = to_eth_hdr(buf);
        if (tun_read(buf, BUFLEN) < 0)
            print_err("ERR: Read from tun_fd: %s\n", strerror(errno));

        print_hex(buf, BUFLEN);
        print_eth_hdr(ehdr);
        handle_frame(&netdev, ehdr);
    }

    return 0;
}
