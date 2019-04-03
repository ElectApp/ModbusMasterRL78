/*
 * u_modbus.c
 *
 *  Created on: 2 เม.ย. 2562
 *      Author: Elect
 *
 *  MODBUS MASTER V2
 *
 *  Use with R5F1056AGSP (RL78 series)
 *  Basic code for Communication on MODBUS RTU
 *  Support MODBUS function #3, #6, #16
 *
 *  Read at u_modbus.h for getting start
 *
 *
 */


#include "r_cg_macrodriver.h"				///< For Pin name
#include "r_cg_sau.h"						///< For UART, DON't forget Start at r_cg_main.c
#include "u_modbus.h"						///< MODBUS Global Access


//PORT and Serial define
#define DriveMode()							P3_bit.no1 = 1		///< Set the IC RS-485 to Drive mode
#define ReceiveMode()						P3_bit.no1 = 0		///< Set the IC RS-485 to Receive mode
#define DBUG_PORT(x)						P2_bit.no0 = (x)  	///< P20 => 0 = LOW, 1 = HIGH
#define SendData()							R_UART1_Send(u8ModbusADU, u8ModbusADUSize)	///< Must include "r_cg_sau.h"

//Other constant
#define MaxBufferSize						64
#define MaxModbusADU						255		///<Value must in rang variable type of u8ModbusADUSize


//Declare Global Variable and Function
uint8_t  status;											///< Modbus Status
uint8_t  u8MBSlave;											///< Modbus slave (1..255) initialized in constructor

uint16_t u16ResponseBuffer[MaxBufferSize];					///< buffer to store Modbus slave response; read via GetResponseBuffer()
uint16_t u16TransmitBuffer[MaxBufferSize];					///< buffer containing data to transmit to Modbus slave; set via SetTransmitBuffer()

// ModbusADU: MODBUS Application on data unit
uint8_t u8ModbusADU[MaxModbusADU], u8ModbusADUSize, u8BytesLeft, u8MBFunction;
uint16_t timeout, count_up;


unsigned short CRC2W_1(const unsigned  char *nData, unsigned short wLength){

unsigned char nTemp;
unsigned short wCRCWord = 0xFFFF;
const short wCRCTable[] = {
   0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
   0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
   0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
   0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
   0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
   0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
   0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
   0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
   0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
   0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
   0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
   0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
   0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
   0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
   0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
   0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
   0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
   0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
   0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
   0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
   0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
   0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
   0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
   0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
   0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
   0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
   0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
   0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
   0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
   0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
   0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
   0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040 };

   while (wLength--)
   {
      nTemp = (*nData++) ^ (wCRCWord);
      wCRCWord >>= 8;
      wCRCWord  ^= wCRCTable[nTemp];
   }

   return wCRCWord;
} // End: CRC16


void clear_u8ModbusADU(void){
	uint16_t i = 0;
	for(i=0; i<MaxModbusADU; i++){
		u8ModbusADU[i] = 0;
	}

}

uint16_t getResponseBuffer(uint8_t u8Index){
	if (u8Index < MaxBufferSize){
		return u16ResponseBuffer[u8Index];
	}else{
		return 0xFFFF;
	}
}

void clearResponseBuffer(void){
	uint16_t i;
	 for(i=0; i<MaxBufferSize; i++){
		 u16ResponseBuffer[i] = 0;
	 }
}

uint8_t setTransmitBuffer(uint8_t u8Index, uint16_t u16Value){
	if(u8Index < MaxBufferSize){
		u16TransmitBuffer[u8Index] = u16Value;
		return Ready;
	}else{
		return IllegalDataAddress;
	}
}



void clearTransmitBuffer(void){
	uint16_t i;
	for(i=0; i<MaxBufferSize; i++){
		u16TransmitBuffer[i] = 0;
	}
}

void initialModbus(void){
	DriveMode();
	u8MBSlave = 1;
	status = Ready;
	timeout = 500;
	clear_u8ModbusADU();
	clearTransmitBuffer();
	clearResponseBuffer();

}

void initialReceiving(void){
	count_up = 0;
	u8BytesLeft = 8;
	clear_u8ModbusADU();
	u8ModbusADUSize = 0;
	ReceiveMode();
}

void SendRequest(uint8_t MBFunction, uint16_t u16Address, uint16_t u16Qty){

	uint16_t i;
	uint16_t u16CRC = 0xFFFF;

	u8MBFunction = MBFunction;

	u8ModbusADUSize = 0;
	clear_u8ModbusADU();

	// assemble Modbus Request Application Data Unit
	u8ModbusADU[u8ModbusADUSize++] = u8MBSlave;
	u8ModbusADU[u8ModbusADUSize++] = MBFunction;

	switch(MBFunction){
		case fReadHoldingRegisters:
			u8ModbusADU[u8ModbusADUSize++] = highByte(u16Address);
			u8ModbusADU[u8ModbusADUSize++] = lowByte(u16Address);
			u8ModbusADU[u8ModbusADUSize++] = highByte(u16Qty);
			u8ModbusADU[u8ModbusADUSize++] = lowByte(u16Qty);
			break;
		case fWriteSingleRegister:
			u8ModbusADU[u8ModbusADUSize++] = highByte(u16Address);
			u8ModbusADU[u8ModbusADUSize++] = lowByte(u16Address);
			u8ModbusADU[u8ModbusADUSize++] = highByte(u16TransmitBuffer[0]);
			u8ModbusADU[u8ModbusADUSize++] = lowByte(u16TransmitBuffer[0]);
			break;
		case fWriteMultipleRegisters:
			u8ModbusADU[u8ModbusADUSize++] = highByte(u16Address);
			u8ModbusADU[u8ModbusADUSize++] = lowByte(u16Address);
			u8ModbusADU[u8ModbusADUSize++] = highByte(u16Qty);
			u8ModbusADU[u8ModbusADUSize++] = lowByte(u16Qty);
			u8ModbusADU[u8ModbusADUSize++] = lowByte(u16Qty << 1);

			for (i = 0; i < lowByte(u16Qty); i++)
			{
			    u8ModbusADU[u8ModbusADUSize++] = highByte(u16TransmitBuffer[i]);
			    u8ModbusADU[u8ModbusADUSize++] = lowByte(u16TransmitBuffer[i]);
			}
			break;

	}

	//Calculation CRC
	u16CRC = CRC2W_1(u8ModbusADU, u8ModbusADUSize);
	//Set
	u8ModbusADU[u8ModbusADUSize++] = lowByte(u16CRC);
	u8ModbusADU[u8ModbusADUSize++] = highByte(u16CRC);
	u8ModbusADU[u8ModbusADUSize] = 0;

	//P3_bit.no1 = 1;

	DriveMode();	//Change Mode to D
	SendData();		//Send request to Slave

}


void ReceiveData(uint8_t rxData){

	u8ModbusADU[u8ModbusADUSize++] = rxData;
	u8BytesLeft--;

}

void setModbusStatus(uint8_t u8Status){
	status = u8Status;
}


uint8_t CheckModbusStatus(void){

	static uint8_t flag;

	if(status != WaitingResponse){
		flag = 0;
		return status;
	}

	if(!flag){
		initialReceiving();
		flag = 1;
	}

		//Check Response
		count_up++;

		//Check Timeout
		if(count_up > timeout){
			status = ResponseTimeout;
			ModbusResponseCallback(ResponseTimeout, u8MBFunction);
			return ResponseTimeout;
		}

		// evaluate slave ID, function code once enough bytes have been read
		if(u8ModbusADUSize == 5){

			// verify response is for correct Modbus slave
			if(u8ModbusADU[0] != u8MBSlave){
				status = InvalidSlaveID;
				ModbusResponseCallback(InvalidSlaveID, u8MBFunction);
				return InvalidSlaveID;
			}

			// verify response is for correct Modbus function code (mask exception bit 7)
			if((u8ModbusADU[1] & 0x7F) != u8MBFunction){
				status = InvalidFunction;
				ModbusResponseCallback(InvalidFunction, u8MBFunction);
				return InvalidFunction;
			}

			// check whether Modbus exception occurred; return Modbus Exception Code
			if (bitRead(u8ModbusADU[1], 7)){
				status = u8ModbusADU[2];
				ModbusResponseCallback(u8ModbusADU[2], u8MBFunction);
				return u8ModbusADU[2];
			}

			// evaluate returned Modbus function code
			switch(u8ModbusADU[1]){
				case fReadHoldingRegisters:
					u8BytesLeft = u8ModbusADU[2];
					break;
				case fWriteSingleRegister:
			    case fWriteMultipleRegisters:
			    	u8BytesLeft = 3;
			        break;
			}

		}

		// verify response is large enough to inspect further
		if(!u8BytesLeft && u8ModbusADUSize>=5){

			//uint8_t u8ResponseBufferLength;
			uint16_t i = 0;

			//Calculation CRC
			uint16_t u16CRC = CRC2W_1(u8ModbusADU, u8ModbusADUSize-2);

			// verify CRC
			if((lowByte(u16CRC) != u8ModbusADU[u8ModbusADUSize - 2] ||
				      highByte(u16CRC) != u8ModbusADU[u8ModbusADUSize - 1])){
				status = InvalidCRC;
				ModbusResponseCallback(InvalidCRC, u8MBFunction);
				return InvalidCRC;
			}

			// disassemble ADU into words
			// evaluate returned Modbus function code
			switch(u8ModbusADU[1]){
				case fReadHoldingRegisters:
					// load bytes into word; response bytes are ordered H, L, H, L, ...
					for(i=0; i<(u8ModbusADU[2] >> 1); i++){
						if(i<MaxBufferSize){
							u16ResponseBuffer[i] = (u8ModbusADU[2 * i + 3] << 8) | u8ModbusADU[2 * i + 4];
						}

						//u8ResponseBufferLength = i;
					}
					break;
				case fWriteSingleRegister:
				case fWriteMultipleRegisters:
					break;
			}

			status = ResponseSuccess;

			ModbusResponseCallback(ResponseSuccess, u8MBFunction);

			return ResponseSuccess;

		}

		return status;

}


void WriteSingleRegister(uint16_t address, uint16_t u16Value){
	u16TransmitBuffer[0] = u16Value;
	SendRequest(fWriteSingleRegister, address, u16Value);
}

void WriteMultipleRegisters(uint16_t startAddress, uint16_t length, uint16_t const *u16Value){
	uint8_t set = 0, x = 0;
	clearTransmitBuffer();
	status = 0;
	for(x = 0; x<length; x++){
		set = setTransmitBuffer(x, u16Value[x]);
		if(set){
			status = set;	//Update Status
			ModbusResponseCallback(status, fWriteMultipleRegisters);		//Callback
			break;
		}
	}
	if(!status){
		SendRequest(fWriteMultipleRegisters, startAddress, length);
	}
}

void ReadHoldingRegisters(uint16_t startAddress, uint16_t length){
	SendRequest(fReadHoldingRegisters, startAddress, length);
}














