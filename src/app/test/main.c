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



int main (void) 
{
	net_init();
	net_start();

	while(1)
	{
		sys_sleep(10);
	}
	return 0;
}