/*
 * ctrl-bus.cpp
 *
 *  Created on: Apr 14, 2016
 *      Author: zhang
 */

#include "ctrl-bus.h"

HYSTERESIS_AND_LOGIC hysteresis_and_logic;

CTRL_BUS_BASE :: CTRL_BUS_BASE   (  RET (*tx_callback) (char* data, unsigned int length),
									RET (*rx_callback) (char* data, unsigned int*length) )
{
	this->tx_callback = tx_callback;
	this->rx_callback = rx_callback;
}

void CTRL_BUS_BASE :: delayMs (unsigned int count)
{
	while(count--);
}


CTRL_BUS_MASTER :: CTRL_BUS_MASTER (RET (*tx_callback) (char* data, unsigned int length),
									RET (*rx_callback) (char* data, unsigned int*length))
				 : CTRL_BUS_BASE   (tx_callback,
									rx_callback)
{
}

RET CTRL_BUS_MASTER :: tx		   (char* data, unsigned int length)
{
	if (!this->isEventDeclaim())
	{
		this->delayMs(CTRL_BUS_WAIT_TIME);
	}
	else
	{
		this->timingClose();
		this->delayMs(CTRL_BUS_GUARD_TIME);
		this->tx_callback(data, length);
		this->timingOpen();
	}

	return RET_OK;
}

RET CTRL_BUS_MASTER :: rx		   (char* data, unsigned int* length)
{
	if (this->isEventDeclaim())
	{
		this->delayMs(CTRL_BUS_WAIT_TIME);
	}
	else
	{
		this->rx_callback(data, length);
		this->timingOpen();
	}

	return RET_OK;
}

bool CTRL_BUS_MASTER :: isEventDeclaim ( void )
{
	if (hysteresis_and_logic.get() == 1)
		return true;
	else
		return false;
}

void CTRL_BUS_MASTER :: timingOpen	    ( void )
{
	hysteresis_and_logic.set(0);
}

void CTRL_BUS_MASTER :: timingClose	( void )
{
	hysteresis_and_logic.reset(0);
}


CTRL_BUS_SLAVE  :: CTRL_BUS_SLAVE  (RET (*tx_callback) (char* data, unsigned int length),
									RET (*rx_callback) (char* data, unsigned int*length))
				 : CTRL_BUS_BASE   (tx_callback,
									rx_callback)
{
}

RET CTRL_BUS_SLAVE :: tx		   (char* data, unsigned int length)
{
	if (!this->isTimingOpen())
	{
		this->delayMs(CTRL_BUS_WAIT_TIME);
	}
	else
	{
		this->eventClaim();
		this->delayMs(CTRL_BUS_GUARD_TIME);
		this->tx_callback(data, length);
		this->eventDeclaim();
	}

	return RET_OK;
}

RET CTRL_BUS_SLAVE :: rx		   (char* data, unsigned int* length)
{
	if (this->isTimingOpen())
	{
		this->delayMs(CTRL_BUS_WAIT_TIME);
	}
	else
	{
		this->rx_callback(data, length);
		this->eventDeclaim();
	}

	return RET_OK;
}

bool CTRL_BUS_SLAVE :: isTimingOpen   ( void )
{
	if (hysteresis_and_logic.get() == 1)
		return true;
	else
		return false;
}

void CTRL_BUS_SLAVE :: eventDeclaim   ( void )
{
	hysteresis_and_logic.set(1);
}

void CTRL_BUS_SLAVE :: eventClaim	  ( void )
{
	hysteresis_and_logic.reset(1);
}
