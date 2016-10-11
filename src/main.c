#include "stdheaders.h"
#include "utils.h"
#include "tuntap.h"

int main(int argc, char const *argv[]){
    int tun_fd;
    char buf[BUFLEN];
    char *dev = calloc(10, 1);
    CLEAR(buf);
    tun_fd = tun_alloc(dev);

    if (set_if_up(dev) != 0)
        print_err("ERROR when setting up if\n");

    // if (set_if_address(dev, "10.0.0.5/24") != 0)
    //     print_err("ERROR when setting address for if\n");

    if (set_if_route(dev, "10.0.0.0/24") != 0)
        print_err("ERROR when setting route for if\n");

    for(;;){
        read(tun_fd, buf, BUFLEN);
        print_hex(buf, BUFLEN);
    }

    free(dev);

    return 0;
}
