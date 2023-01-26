/*
 * i2c.c
 *
 * Created: 18.10.2019 20:09:26
 *  Author: gfcwfzkm
 */ 

#include "i2c.h"

#if defined(__AVR_ATmega4809__)
#define TWI0_PORT_NORMAL	PORTA
#define TWI0_PORT_MUX		PORTC
#define TWI0_SDA_PULLUP		PIN2CTRL
#define TWI0_SCL_PULLUP		PIN3CTRL	
#else
#error "Pindefinition missing, add here!"
#endif

uint16_t _timeout = 0;

uint8_t _i2c_processTimeout()
{
	if (_timeout > TWI_TIMEOUT) return 1;
	
	_delay_us(1);
	_timeout++;
	
	return 0;
}

void TWI_init(uint32_t twi_clk, enum i2c_useMUX _mux, enum i2c_usePullup _pullEN)
{
	volatile PORT_t *twi_port;
	
	if (_mux)
	{
		twi_port = &TWI0_PORT_MUX;
		PORTMUX.TWISPIROUTEA |= PORTMUX_TWI0_ALT2_gc;
	}
	else
	{
		twi_port = &TWI0_PORT_NORMAL;
		PORTMUX.TWISPIROUTEA &= ~PORTMUX_TWI0_ALT2_gc;
	}
	if (_pullEN)
	{
		(*twi_port).TWI0_SDA_PULLUP |= PORT_PULLUPEN_bm;
		(*twi_port).TWI0_SCL_PULLUP |= PORT_PULLUPEN_bm;
	}
	else
	{
		(*twi_port).TWI0_SDA_PULLUP = (*twi_port).TWI0_SDA_PULLUP & ~PORT_PULLUPEN_bm;
		(*twi_port).TWI0_SCL_PULLUP = (*twi_port).TWI0_SCL_PULLUP & ~PORT_PULLUPEN_bm;
	}
		
	uint16_t t_rise;
	if(twi_clk < 200000)
	{
		t_rise = 1000;	
	}
	else if (twi_clk < 800000)
	{
		t_rise = 300;
	}
	else
	{
		t_rise = 120;	
	}
	
	uint32_t baud = ((F_CPU/twi_clk) - (((F_CPU*t_rise)/1000)/1000)/1000 - 10)/2;
	
	TWI0.MBAUD = (uint8_t)baud;
	TWI0.MCTRLA = TWI_ENABLE_bm;
	
	TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc;
}

TWI_ERROR TWI_read(uint8_t *data, enum TWI_ACKFLAG _ack)
{
	_timeout = 0;												// reset timeout counter, will be incremented by ms tick interrupt
	if ((TWI0.MSTATUS & TWI_BUSSTATE_gm) != TWI_BUSSTATE_OWNER_gc)	// if doesn't master controls bus
		return MASTER_NOT_CONTROLLING_BUS;
	
	while (!(TWI0.MSTATUS & TWI_RIF_bm))						// wait for RIF set (data byte received)
	{
		if (_i2c_processTimeout()) return TIMEOUT_ERROR;		// return timeout error
	}
	
	if (TWI0.MSTATUS & TWI_BUSERR_bm) return BUS_ERROR;					// Bus Error, abort
	if (TWI0.MSTATUS & TWI_ARBLOST_bm) return ARBITRATION_LOST;				// Arbitration Lost, abort
	if (TWI0.MSTATUS & TWI_RXACK_bm) return SLAVE_RESPONDED_NACK;					// Slave replied with NACK, abort
	
	*data = TWI0.MDATA;
	if (_ack == TWI_ACK)
	{
		TWI0.MCTRLB = TWI_ACKACT_ACK_gc;					// setup ACK
	}
	else
	{
		TWI0.MCTRLB = TWI_ACKACT_NACK_gc;						// setup NACK (last byte read)
	}
	TWI0.MCTRLB |= TWI_MCMD_RECVTRANS_gc;	// send ACK, more bytes to follow
	
	return NO_ERROR;
}

TWI_ERROR TWI_write(uint8_t data)
{
	_timeout = 0;													// reset timeout counter, will be incremented by ms tick interrupt
	if ((TWI0.MSTATUS & TWI_BUSSTATE_gm) != TWI_BUSSTATE_OWNER_gc)	// if doesn't master controls bus
		return MASTER_NOT_CONTROLLING_BUS;
	
	TWI0.MDATA = data;
	while (!(TWI0.MSTATUS & TWI_WIF_bm))							// wait until WIF set, status register contains ACK/NACK bit
	{
		if (_i2c_processTimeout()) return TIMEOUT_ERROR;			// return timeout error
	}
	if (TWI0.MSTATUS & TWI_BUSERR_bm) return BUS_ERROR;				// Bus Error, abort
	if (TWI0.MSTATUS & TWI_RXACK_bm) return SLAVE_RESPONDED_NACK;	// Slave replied with NACK, abort
	return NO_ERROR;												// no error
}

TWI_ERROR TWI_start(uint8_t addr)
{
	if (TWI0.MSTATUS & TWI_BUSERR_bm) return BUS_ERROR;						// Bus Error, abort
	TWI0.MADDR = addr;
	return TWI_wait_ACK();
}

TWI_ERROR TWI_wait_ACK()											// wait for slave response after start of Master Write
{
	_timeout = 0;												// reset timeout counter, will be incremented by ms tick interrupt
	while (!(TWI0.MSTATUS & (TWI_RIF_bm | TWI_WIF_bm)))	// wait for RIF or WIF set
	{
		if (_i2c_processTimeout()) return TIMEOUT_ERROR;			// return timeout error
	}
	
	if (TWI0.MSTATUS & TWI_BUSERR_bm) return BUS_ERROR;						// Bus Error, abort
	if (TWI0.MSTATUS & TWI_ARBLOST_bm) return ARBITRATION_LOST;					// Arbitration Lost, abort
	if (TWI0.MSTATUS & TWI_RXACK_bm) return SLAVE_RESPONDED_NACK;						// Slave replied with NACK, abort
	
	return NO_ERROR;														// no error
}

void TWI_stop ()
{
	TWI0.MCTRLB |= TWI_MCMD_STOP_gc;
}
