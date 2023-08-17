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

static sys_sem_t sem;
static sys_mutex_t mutex;
static int count;

static unsigned char buffer[100];
static int write_indx, read_indx;
static sys_sem_t read_sem;
static sys_sem_t write_sem;
void thread1_entry(void * arg)
{
	for(int i = 0; i < 2 * sizeof(buffer); i++)
	{
		sys_sem_wait(read_sem, 0);

		unsigned char data = buffer[read_indx++];//char会内存泄漏，只能用unsigned char

		if(read_indx >= sizeof(buffer))
		{
			read_indx = 0;
		}
		sys_sem_notify(write_sem);
		plat_printf("thread 1: read data=%d\n", data);
		
		sys_sleep(100);
	}

	while(1)
	{
		plat_printf("this is thread1: %s \n",(char *)arg);
		sys_sem_notify(sem);
		sys_sleep(1000);
	}
}

void thread2_entry(void * arg)
{

	for(int i = 0; i < 2 * sizeof(buffer); i++)
	{
		sys_sem_wait(write_sem, 0);
		buffer[write_indx++] = i;

		if(write_indx >= sizeof(buffer))
		{
			write_indx = 0;
		}

		plat_printf("thread 2: write data = %d\n", i);

		sys_sem_notify(read_sem);
		
		
	}

	while(1)
	{
		sys_sem_wait(sem, 0);
		plat_printf("this is thread2: %s \n", (char *)arg);
	}
}

int main (void) 
{
	sem = sys_sem_create(0);
	mutex = sys_mutex_create();
	read_sem = sys_sem_create(0);
	write_sem = sys_sem_create(sizeof(buffer));

	sys_thread_create(thread1_entry, "AAAA");//创建一个新线程
	sys_thread_create(thread2_entry, "BBBB");

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