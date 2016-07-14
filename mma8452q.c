#include<stdio.h>
#include "mma8452q.h"
#include "i2c.c"
uint8_t readRegister(uint8_t registerAddress)
	{
		uint8_t data = I2CRead(MMA8452Q_Address, registerAddress);
		return data;
	}
void writeRegister(uint8_t registerAddress, uint8_t data)
	{
		I2CWriteRegister(MMA8452Q_Address, registerAddress, data);
	}  
void setupAccelerometer()
	{
		uint8_t data;
//Registers can only be modified in standby mode.
//Set the accelerometer in standby.
		data = readRegister(CTRL_REG1);
		writeRegister(CTRL_REG1, data & ~(0x01));
//Set the scale of the accelerometer - 2g, 4g, or 8g
//Setting to 2g for better accuracy
		writeRegister(XYZ_DATA_CFG, 0x00);
//Setting the output data rate.
//Setting as 200Hz - double of the magnetometer output data rate.
		writeRegister(CTRL_REG1, 0X10);
//Setting all options of CTRL_REG2 - self test, reset, oversampling etc off
		writeRegister(CTRl_REG2, 0x00);
//Configuring the interrupt generation for active low with an open drain output
		writeRegister(CTRL_REG3, 0x03);
//Enabling the data ready interrupt
		writeRegister(CTRL_REG4, 0x01);
//Routing the interrupt to pin INT1 on the accelerometer
		writeRegister(CTRL_REG5, 0x01);
//Now that the device is configured, setting the device into active mode.
		data = readRegister(CTRL_REG1);
		writeRegister(CTRL_REG1, data|0x01);
//Set up is done
	}
void readAccelerometer()
	{
		I2CReadN(MMA8452Q_Address, OUT_X_MSB, rawData, 6);
		x = ((short)(rawData[0]<<8 | rawData[1])) >> 4;
		y = ((short)(rawData[2]<<8 | rawData[3])) >> 4;
		z = ((short)(rawData[4]<<8 | rawData[5])) >> 4;
		cx = (float) x / (float)(1<<11) * (float)(scale);
		cy = (float) y / (float)(1<<11) * (float)(scale);
		cz = (float) z / (float)(1<<11) * (float)(scale);
	}