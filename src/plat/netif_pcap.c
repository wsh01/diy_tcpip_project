#include "netif_pcap.h"
#include "sys_plat.h"

void recv_thread (void * arg)
{
    plat_printf("recv_thread is running\n");
    while(1)
    {
        sys_sleep(10);
    }
}

void xmit_thread (void * arg)
{
    plat_printf("xmit_thread is running\n");
     while(1)
    {
        sys_sleep(10);
    }
}

net_err_t netif_pcap_open(void)
{
    sys_thread_create(recv_thread, (void *)0);
    sys_thread_create(xmit_thread, (void *)0);
}