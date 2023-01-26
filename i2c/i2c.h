 /**
 * @mainpage TWI-Library for the new Microchip AVR MCU
 *
 * @brief Two-Wire Interface Master (I2C) Library for the new AVR microcontroller series with hardware TWI
 * and timeouts, to prevent loop-lockups.
 *
 * @version 1.0
 * - Initial release of the library
 *
 * @author gfcwfzkm
 * @date 20.02.2020
 */

/**
 * @file i2c.h
 * @brief Headerfile of the twi-library
 *
*/


#ifndef TWI_LIB_H_
#define TWI_LIB_H_

#ifndef F_CPU
/**
 * @brief CPU Clock of the microcontroller
 */
#error "F_CPU hasn't been defined yet!"
#endif

#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>

#ifndef TWI_TIMEOUT
/**
 * @brief Timeout-Time in microseconds
 *
 * The time in in microseconds until \a TWI_start() & \a TWI_write() function
 * cancel the loop and return an error message.
 */
#define TWI_TIMEOUT 5000 
#endif

/**
 * @brief Starting TWI in read mode, is used by \a TWI_start()
 */
#define TWI_READ	1
/**
 * @brief Starting TWI in write mode, is used by \a TWI_start()
 */
#define TWI_WRITE	0

typedef enum {
	NO_ERROR					= 0x00,
	SLAVE_RESPONDED_NACK		= 0x01,
	SLAVE_RESPONDED_ACK			= 0x02,
	ARBITRATION_LOST			= 0x04,
	BUS_ERROR					= 0x08,
	MASTER_NOT_CONTROLLING_BUS	= 0x10,
	TIMEOUT_ERROR				= 0x20
}TWI_ERROR;

enum TWI_ACKFLAG {
	TWI_ACK	=0,
	TWI_NACK=1
};

enum i2c_useMUX {
	NORMAL_I2C_PINPOS	= 0,	/**< I2C pin mux position unchanged */
	MUXED_I2C_PINPOS	= 1		/**< I2C pin mux position changed */
};

enum i2c_usePullup {
	NO_PULLUP			= 0,	/**< I2C pin mux position unchanged */
	USE_INTERNAL_PULLUP	= 1		/**< I2C pin mux position changed */
};

/**
 * @brief TWI repeated start
 */
#define TWI_rstart(adr) (TWI_start(adr))

/**
 * @brief Initialise the TWI hardware & setup TWI clock
 *
 * Initialise the twi-hardware of the microcontroller. Requires the twi clock, mux - and
 * ullup-configuration.\n
 * Example: \n \code{.c}
 * TWI_init(100000UL, MUXED_I2C_PINPOS, USE_INTERNAL_PULLUP);
 * \endcode Im Beispiel wird die TWI Schnittstelle mit 100kHz Geschwindigkeit, den alternativen I2C-Pins & 
 * den internen Pull-Ups initialisiert.
 * @param twi_clk	Taktfrequenz für den TWI-BUS
 * @param _mux		Normale oder alternative Pinposition
 * @param _pullEN	Interne Pullups verwenden oder nicht
 */
void TWI_init(uint32_t twi_clk, enum i2c_useMUX _mux, enum i2c_usePullup _pullEN);

/**
 * @brief Liest ein Byte vom TWI-BUS
 *
 * Liest ein Byte von der TWI-Peripherie und teilt der Peripherie mit, ob dieser
 * weitere Daten senden soll oder mit der Übertragung aufhören soll.
 * \n Beispiel: \n \code{.c}
 * int myData;
 * TWI_read(&myData, TWI_NACK);
 * \endcode Dies würde 1 Byte vom TWI-BUS lesen und die Übertragung weiterer Daten 
 * stoppen
 * @param data Pointer zum zu lesendem Byte
 * @param _ack I2C-Mastersignal, \a TWI_ACK können weitere Daten folgen, 
 * bei \a TWI_NACK wird danach die Übertragung gestoppt
 * @return \a TWI_ERROR Rück/Fehlermeldung
 */
TWI_ERROR TWI_read(uint8_t *data, enum TWI_ACKFLAG _ack);

/**
 * @brief Ein Byte in den TWI-BUS senden
 *
 * Schreibt ein Byte in den BUS. 
 * \n Beispiel: \n \code{.c}
 * if(TWI_write(MyByte))
 * {
 * 	//FEHLER
 * }
 * \endcode Im Beispiel wird ein Byte an die TWI_Peripherie gesendet.
 * @param data Zu sendende Byte
 * @return \a TWI_ERROR Rück/Fehlermeldung
 */
TWI_ERROR TWI_write(uint8_t data);

/**
 * @brief TWI BUS starten und Peripherie ansprechen
 *
 * Spricht die Peripherie mit der Adresse und dem Modus an.
 * \n Beispiel: \n \code{.c}
 * if(TWI_start(0x55 | TWI_WRITE))
 * {
 *		//FEHLER
 * }
 * \endcode Im Beispiel wird die TWI_Busübertragung gestartet und der Sensor in der Adresse 0x55 wird im Schreibmodus aufgerufen.
 * @param address Adresse des ICs und Modus (\a TWI_READ / \a TWI_WRITE)
 * @return Fehlermeldung bei Timeout
 */
TWI_ERROR TWI_start(uint8_t addr);


/**
 * @brief TWI-BUS schliessen und freigeben
 */
void TWI_stop();


TWI_ERROR TWI_wait_ACK();

#endif /* TWI-LIB_H_ */
