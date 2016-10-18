#include "arp.h"
#include "netdev.h"
#include "utils.h"

static struct arp_cache_entry arp_cache[ARP_CACHE_LEN];
void arp_init()
{
    memset(arp_cache, 0, sizeof(arp_cache));
}

static int insert_arp_table(struct arp_hdr* hdr, struct arp_ipv4* data)
{
    for (int i = 0; i < ARP_CACHE_LEN; ++i) {
        struct arp_cache_entry* entry = arp_cache + i;

        if (entry->state == ARP_FREE) {
            entry->state = ARP_RESOLVED;
            memcpy(&entry->hw_type, &hdr->hw_type, sizeof(hdr->hw_type));
            memcpy(entry->src_addr, data->src_addr, sizeof(data->src_addr));
            memcpy(entry->src_mac, data->src_mac, sizeof(data->src_mac));
            return 0;
        }
    }
    return -1;
}

static int update_arp_table(struct arp_hdr* hdr, struct arp_ipv4* data)
{

    for (int i = 0; i < ARP_CACHE_LEN; ++i) {
        struct arp_cache_entry* entry = arp_cache + i;

        if (entry->state == ARP_FREE)
            continue;

        if (entry->hw_type == hdr->hw_type && entry->src_addr == data->src_addr) {
            memcpy(entry->src_mac, data->src_mac, 6);
            return 1;
        }
    }
    return 0;
}

void arp_incoming(struct netdev* netdev, struct eth_hdr* hdr)
{
    struct arp_hdr* ahdr;
    struct arp_ipv4* arp_payload;
    int merge_flag = 0;

    ahdr = (struct arp_hdr*)hdr->payload;
    ahdr->hw_type = htons(ahdr->hw_type);
    ahdr->pro_type = htons(ahdr->pro_type);
    ahdr->opcode = htons(ahdr->opcode);

    if (ahdr->hw_type != ARP_ETHERNET) {
        printf("Unsupported HW Type\n");
        return;
    }

    if (ahdr->pro_type != ARP_IPV4) {
        printf("Unsupported Protocol\n");
        return;
    }

    arp_payload = (struct arp_ipv4*)ahdr->payload;

    merge_flag = update_arp_table(ahdr, arp_payload);

    if(!memcmp(&netdev->addr, arp_payload->dst_addr, 4))
        printf("ARP not for this machine\n");

    if(!merge_flag && insert_arp_table(ahdr, arp_payload))
        print_err("ERR: No free space in ARP translation table\n");

    switch (ahdr->opcode) {
    case ARP_REQUEST:
        break;
    default:
        printf("Opcode not supported\n");
        break;
    }
}
