/*
 * ctrl-bus.h
 *
 *  Created on: Apr 14, 2016
 *      Author: zhang
 */

#ifndef CTRL_BUS_H_
#define CTRL_BUS_H_

#include "return-value.h"
#include "../Libraries/hysteresis-and-logic/hysteresis-and-logic.h"


#define CTRL_BUS_WAIT_TIME              (1000)
#define CTRL_BUS_GUARD_TIME             (10)


class   CTRL_BUS_BASE
{
public:
	RET (*tx_callback)	( char* data, unsigned int length );
	RET (*rx_callback)	( char* data, unsigned int*length );
public:
	CTRL_BUS_BASE		( RET (*tx_callback) (char* data, unsigned int length),
						  RET (*rx_callback) (char* data, unsigned int*length) );
	void  delayMs		( unsigned int count);
};

class   CTRL_BUS_MASTER : public CTRL_BUS_BASE
{
public:
	CTRL_BUS_MASTER		( RET (*tx_callback) (char* data, unsigned int length),
						  RET (*rx_callback) (char* data, unsigned int*length) );

	RET   tx			( char* data, unsigned int length );
	RET   rx			( char* data, unsigned int*length );
	bool  isEventDeclaim( void );
	void  timingOpen	( void );
	void  timingClose	( void );
};

class   CTRL_BUS_SLAVE  : public CTRL_BUS_BASE
{
public:
	CTRL_BUS_SLAVE		( RET (*tx_callback) (char* data, unsigned int length),
						  RET (*rx_callback) (char* data, unsigned int*length) );

	RET   tx			( char* data, unsigned int length );
	RET   rx			( char* data, unsigned int*length );
	bool  isTimingOpen	( void );
	void  eventDeclaim	( void );
	void  eventClaim	( void );
};

#endif /* CTRL_BUS_H_ */
