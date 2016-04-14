/*
 * test.cpp
 *
 *  Created on: Apr 13, 2016
 *      Author: zhang
 */

#include <stdio.h>
#include "return-value.h"
#include "memory-share/memory-share.h"
#include "../Sources/ctrl-bus.h"


CTRL_BUS_MASTER ctrl_bus_master(memory_share_write,
								memory_share_read);

CTRL_BUS_SLAVE  ctrl_bus_slave (memory_share_write,
								memory_share_read);

int main (int argc, char* argv[])
{
	char data[20];
	unsigned int length;
	setvbuf (stdout, NULL, _IONBF, 0);

	ctrl_bus_master.tx("hello world!", 13);
	ctrl_bus_slave.rx(data, &length);
	printf("data is %s, length is %d\r\n", data, length);

	return 0;
}
