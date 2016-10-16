#include "arp.h"

void arp_incoming(int tun_fd, struct eth_hdr* hdr)
{
    struct arp_hdr* ahdr;
    struct arp_ipv4* arp_payload;

    ahdr = (struct arp_hdr*)hdr->payload;
    ahdr->hw_type = htons(ahdr->hw_type);
    ahdr->pro_type = htons(ahdr->pro_type);
    ahdr->opcode = htons(ahdr->opcode);

    if (ahdr->hw_type != ARP_ETHERNET)
        printf("Unsupported HW Type\n");

    if (ahdr->pro_type != ARP_IPV4)
        printf("Unsupported Protocol\n");

    arp_payload = (struct arp_ipv4*)ahdr->payload;

    if (ahdr->opcode == ARP_REQUEST)
        printf("Detected ARP Request\n");
}
