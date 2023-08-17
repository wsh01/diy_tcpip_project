/**
 * @file main.c
 * @author wang
 * @brief 测试主程序，完成一些简单的测试主程序
 * @version 0.1
 * @date 2023-8-15
 *
 * @copyright Copyright (c) 2023
 * @note 
 */
#include <stdio.h>
#include "sys_plat.h"
#include "net.h"
#include "netif_pcap.h"

net_err_t netdev_init(void)
{
	netif_pcap_open();

	return NET_ERR_OK;
}

int main (void) 
{
	net_init();

	net_start();

	netdev_init();

	while(1)
	{
		sys_sleep(10);
	}
	return 0;
}