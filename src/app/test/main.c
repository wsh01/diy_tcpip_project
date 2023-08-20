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
#include "dbg.h"

net_err_t netdev_init(void)
{
	netif_pcap_open();

	return NET_ERR_OK;
}

#define DBG_TEST 	 DBG_LEVEL_INFO

int main (void) 
{
	dbg_info(DBG_TEST, "info");
	dbg_warning(DBG_TEST, "warning");
	dbg_error(DBG_TEST, "error");
	
	net_init();

	net_start();

	netdev_init();

	while(1)
	{
		sys_sleep(10);
	}
	return 0;
}