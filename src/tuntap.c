#include "stdheaders.h"
#include "utils.h"

static int tun_fd;

static int set_if_up(char* dev)
{
    return run_cmd("ip link set dev %s up", dev);
}

static int set_if_address(char* dev, char* cidr)
{
    return run_cmd("ip address add dev %s local %s", dev, cidr);
}

static int set_if_route(char* dev, char* cidr)
{
    return run_cmd("ip route add dev %s %s", dev, cidr);
}

/*
 * Taken directly from the kernel Documentation
 * (https://www.kernel.org/doc/Documentation/networking/tuntap.txt)
 */
static int tun_alloc(char* dev)
{
    struct ifreq ifr;
    int fd, err;

    if ((fd = open("/dev/net/tap", O_RDWR)) < 0) {
        print_err("Cannot open TUN/TAP dev");
        printf("$ sudo mknod /dev/net/tap c 10 200\n");
        exit(1);
    }

    CLEAR(ifr);

    /* Flags: IFF_TUN   - TUN device (no Ethernet headers)
     *        IFF_TAP   - TAP device
     *
     *        IFF_NO_PI - Do not provide packet information
     */
    ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
    if (*dev) {
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
    }

    if ((err = ioctl(fd, TUNSETIFF, (void*)&ifr)) < 0) {
        print_err("ERR: Could not ioctl tun: %s\n", strerror(errno));
        close(fd);
        return err;
    }

    strcpy(dev, ifr.ifr_name);
    return fd;
}

void tun_init(char* dev)
{
    tun_fd = tun_alloc(dev);

    if (set_if_up(dev) != 0)
        print_err("ERR: can't set up dev\n");

    if (set_if_route(dev, "10.0.0.0/24") != 0)
        print_err("ERR: can't set up route for if\n");
}

int tun_read(char* buf, int len)
{
    return(read(tun_fd, buf, len));
}

int tun_write(char* buf, int len)
{
    return(write(tun_fd, buf, len));
}
