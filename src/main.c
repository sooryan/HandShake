#include "arp.h"
#include "ethernet.h"
#include "stdheaders.h"
#include "tuntap.h"
#include "utils.h"

void handle_frame(int tun_fd, struct eth_hdr* ehdr)
{
    switch (ehdr->ethertype) {
    case ETH_P_ARP:
        arp_incoming(tun_fd, ehdr);
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
    int tun_fd;
    char buf[BUFLEN];
    char* dev = calloc(10, 1);
    CLEAR(buf);
    tun_fd = tun_alloc(dev);

    if (set_if_up(dev) != 0)
        print_err("ERROR when setting up if\n");

    // if (set_if_address(dev, "10.0.0.5/24") != 0)
    //     print_err("ERROR when setting address for if\n");

    if (set_if_route(dev, "10.0.0.0/24") != 0)
        print_err("ERROR when setting route for if\n");

    for (;;) {
        struct eth_hdr* ehdr = to_eth_hdr(buf);
        read(tun_fd, buf, BUFLEN);
        print_hex(buf, BUFLEN);
        print_eth_hdr(ehdr);
        handle_frame(tun_fd, ehdr);
    }

    free(dev);

    return 0;
}
