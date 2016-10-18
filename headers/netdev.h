#ifndef NETDEV_H
#define NETDEV_H

#include "stdheaders.h"

struct netdev {
    struct in_addr addr;
    unsigned char hw_addr[6];
};

void netdev_init(struct netdev* dev, char* addr, char* hwaddr);
#endif // NETDEV_H
