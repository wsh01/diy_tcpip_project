/**
 * @file main.c
 * @author wang
 * @brief 测试主程序，完成一些简单的测试主程序
 * @version 0.1
 * @date 2022-10-23
 *
 * @copyright Copyright (c) 2022
 * @note 
 */
#include <stdio.h>
#include "sys_plat.h"

int main (void) {
	pcap_t * pcap = pcap_device_open(netdev0_phy_ip, netdev0_hwaddr);//打开网络设备，IP地址，Mac地址
	while (pcap)
	{
		static uint8_t buffer[1024];
		static int counter = 0;
		struct pcap_pkthdr * pkthdr;
		const uint8_t * pkt_data;
/*为什么虚拟机上ping 192.168.74.1时，能ping通，但程序没反应。但当ping 192.168.74.255 广播时，ping不通，但程序正常工作,
wireshark抓取的包符合程序功能。还有就是为什么没有buffer[0] = 1;buffer[1] = 2;这俩修改代码时，会进入死循环*/
/*查阅发现，Windows默认不对广播包进行回应*/
		plat_printf("begin test: %d\n", counter++);
		for (int  i = 0; i < sizeof(buffer); i++)
		{
			buffer[i] = i;
		}
		
		if (pcap_next_ex(pcap, &pkthdr, &pkt_data) != 1)//从网口读取数据，pkthdr:数据包信息，pkt_data:数据内容
		{
			continue;
		}

		int len = pkthdr->len > sizeof(buffer) ? sizeof(buffer) : pkthdr->len;
		plat_memcpy(buffer, pkt_data, len);//将接收到的数据复制到缓冲区
		buffer[0] = 1;
		buffer[1] = 2;						//将包修改一下

		if (pcap_inject(pcap, buffer, len) == -1)//发送数据
		{
			plat_printf("pcap send: send packet failed %s\n ", pcap_geterr(pcap));
			break;
		}

	}

	return 0;
}