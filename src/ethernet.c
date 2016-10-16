#include "ethernet.h"
#include "stdheaders.h"

struct eth_hdr* to_eth_hdr(char* buf)
{
    // Simply typecasts to eth header format
    struct eth_hdr* hdr = (struct eth_hdr*)buf;
    // host to network order conversion
    hdr->ethertype = htons(hdr->ethertype);

    return hdr;
}

void print_eth_hdr(struct eth_hdr* ehdr)
{
    printf("\nPrinting Ethernet Header:\n");

    printf("Source MAC:");
    for (int i = 0; i < 6; ++i)
        printf("%x ", ehdr->src_mac[i]);

    printf("\nDestination MAC:");
    for (int i = 0; i < 6; ++i)
        printf("%x ", ehdr->dst_mac[i]);

    printf("\nEthertype: %x\n", ehdr->ethertype);
}
