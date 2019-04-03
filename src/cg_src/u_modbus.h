/*
 * u_modbus.h
 *
 *  Created on: 2 เม.ย. 2562
 *      Author: Elect
 */

#ifndef CG_SRC_U_MODBUS_H_
#define CG_SRC_U_MODBUS_H_


/**
@def lowWord(ww) ((uint16_t) ((ww) & 0xFFFF))
Macro to return low word of a 32-bit integer.
*/
#define lowWord(ww) 					((uint16_t) ((ww) & 0xFFFF))

/**
@def highWord(ww) ((uint16_t) ((ww) >> 16))
Macro to return high word of a 32-bit integer.
*/
#define highWord(ww) 					((uint16_t) ((ww) >> 16))

/**
@def LONG(hi, lo) ((uint32_t) ((hi) << 16 | (lo)))
Macro to generate 32-bit integer from (2) 16-bit words.
*/
#define LONG(hi, lo) 					((uint32_t) ((hi) << 16 | (lo)))

#define lowByte(w) 						((uint8_t) ((w) & 0xff))
#define highByte(w) 					((uint8_t) ((w) >> 8))

#define bitRead(value, bit) 			(((value) >> (bit)) & 0x01)
#define bitSet(value, bit) 				((value) |= (1UL << (bit)))
#define bitClear(value, bit) 			((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) 	(bitvalue ? bitSet(value, bit) : bitClear(value, bit))


//Modbus Function
#define fReadHoldingRegisters				0x03
#define	fWriteSingleRegister				0x06
#define fWriteMultipleRegisters				0x10



//Modbus Standard Exception
#define IllegalFunction			0x01
#define IllegalDataAddress		0x02
#define IllegalDataValue		0x03
#define SlaveDeviceFailure		0x04

//Modbus Master Exception
#define InvalidSlaveID			0xA0
#define InvalidFunction			0xA1
#define ResponseTimeout			0xA2
#define InvalidCRC				0xA3

//Other
#define WaitingResponse			0xB0
#define ResponseSuccess			0xB1
#define Ready					0x00

///=========================================================================///
/// Global Function

void initialModbus(void);				//Call one time at r_cg_main.c
void ReceiveData(uint8_t rxData);		//RX Data @ r_uart1_callback_softwareoverrun() at r_cg_sau_user.c

void WriteSingleRegister(uint16_t address, uint16_t u16Value);	//Function #6
void WriteMultipleRegisters(uint16_t startAddress, uint16_t length, uint16_t const *u16Value); //Function #16
void ReadHoldingRegisters(uint16_t startAddress, uint16_t length);	 //Function #3

uint16_t getResponseBuffer(uint8_t u8Index);	//Get slave response data after ReadHoldingRegisters success
void clearResponseBuffer(void);					//Clear slave response data


uint8_t CheckModbusStatus(void);				//Interval check at loop 1ms
void setModbusStatus(uint8_t u8Status);

/*
 * This function use for response of slave after sending request
 * It should be created on the .c file that call getModbusStatus() function
 *
 * */
void ModbusResponseCallback(uint8_t exception, uint8_t u8MBFunction);



#endif /* CG_SRC_U_MODBUS_H_ */
