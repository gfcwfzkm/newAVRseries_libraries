# Driver / Library Interface Functions
In order to interface with the various peripheral drivers for different microcontrollers and the many different IC-drivers, I had to come up with a way of connecting different libraries to different controllers and peripherals with as little code-changes as possible.

The solution to this is a set of I/O functions that are as generic as possible, in order to interface with many different kind of peripheral libraries - while also making it easy enough to add these I/O functions to HAL-drivers with just a few, basic glue-functions.

## Basic Overview
The I/O functions are kept as simple as possible, consisting only of two functions to initialise or close the interface and three functions to send the actual data through. 

While the peripheral drivers provides the functions themself, the IC drivers need a way to hold the function pointer and the potential structure pointer to the peripheral driver.
<table>
<tr>
<td> Peripheral Driver Side </td> <td> IC Driver Side </td>
</tr>
<tr>
<td>

```c
typedef struct {...} Driver_t;
uint8_t perDrv_IntPrepare(void *intv);
uint8_t perDrv_IntSendBytes(void *intv,
                        uint8_t addr,
                        uint8_t *buf_ptr,
                        uint16_t buf_len);
uint8_t perDrv_IntTransceiveBytes(void *intv,
                        uint8_t addr,
                        uint8_t *buf_ptr,
                        uint16_t buf_len);
uint8_t perDrv_IntGetBytes(void *intv,
                        uint8_t addr,
                        uint8_t *buf_ptr,
                        uint16_t buf_len);
uint8_t perDrv_IntFinish(void *intv);
```
</td><td>

```c
void *ioInterface;
uint8_t (*startTransaction)(void*);
uint8_t (*sendBytes)(void*,
                    uint8_t,
                    uint8_t*,
                    uint16_t);
uint8_t (*transceiveBytes)(void*,
                    uint8_t,
                    uint8_t*,
                    uint16_t);
uint8_t (*getBytes)(void*,
                    uint8_t,
                    uint8_t*,
                    uint16_t);
uint8_t (*endTransaction)(void*);
```
</tr>
</table>

## Function explanation
___
### Interface Prepare
```c
uint8_t perDrv_IntPrepare(void *intv);  // Generic function name / example
uint8_t usartSPI_InterfacePrepare(void *intUSPI);  // Example for USART-SPI
```
Todo: Explain stuff
___
### Interface Send Bytes
```c
uint8_t perDrv_IntSendBytes(void *intv,             // Generic function name / example
                        uint8_t addr,
                        uint8_t *buf_ptr,
                        uint16_t buf_len);
uint8_t usartSPI_InterfaceSendBytes(void *intUSPI   // Example for USART-SPI
                        uint8_t addr,
                        uint8_t *buf_ptr,
                        uint16_t buf_len); 
```
Todo: Explain stuff
___
### Interface Transceive Bytes
```c
uint8_t perDrv_IntTransceiveBytes(void *intv,       // Generic function name / example
                        uint8_t addr,
                        uint8_t *buf_ptr,
                        uint16_t buf_len);
uint8_t usartSPI_InterfaceTransceiveBytes(void *intUSPI, // Example for USART-SPI
                        uint8_t addr,
                        uint8_t *buf_ptr,
                        uint16_t buf_len);
```
Todo: Explain stuff
___
### Interface Get Bytes
```c
uint8_t perDrv_IntGetBytes(void *intv,              // Generic function name / example
                        uint8_t addr,
                        uint8_t *buf_ptr,
                        uint16_t buf_len);
uint8_t usartSPI_InterfaceGetBytes(void *intUSPI,   // Example for USART-SPI
                        uint8_t addr,
                        uint8_t *buf_ptr,
                        uint16_t buf_len);
```
Todo: Explain stuff
___
### Interface Finish
```c
uint8_t perDrv_IntFinish(void *intv);               // Generic function name / example
uint8_t usartSPI_InterfaceFinish(void *intUSPI);    // Example for USART-SPI
```
Todo: Explain stuff
