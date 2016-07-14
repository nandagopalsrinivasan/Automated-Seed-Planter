#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
void InitUARTConsole(void)
	{
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
		GPIOPinConfigure(GPIO_PA0_U0RX);
		GPIOPinConfigure(GPIO_PA1_U0TX);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
		UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
		GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
		UARTStdioConfig(0, 115200, 16000000);
	}
void InitI2C0(void)
	{
		SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
		SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
		GPIOPinConfigure(GPIO_PB2_I2C0SCL);
		GPIOPinConfigure(GPIO_PB3_I2C0SDA);
		GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
		GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);
		I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);
		HWREG(I2C0_BASE + I2C_O_FIFOCTL) = 80008000;
	}
void I2CWrite(uint8_t slaveAddress, uint8_t dataByte)
	{
		I2CMasterSlaveAddrSet(I2C0_BASE, slaveAddress, false);
		I2CMasterDataPut(I2C0_BASE, dataByte);
		I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
		while(I2CMasterBusy(I2C0_BASE));
	}
void I2CWriteRegister(uint8_t slaveAddress, uint8_t regAddress, uint8_t data)
	{
		I2CMasterSlaveAddrSet(I2C0_BASE, slaveAddress, false);
		I2CMasterDataPut(I2C0_BASE, regAddress);
		I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
		while(I2CMasterBusy(I2C0_BASE));
		I2CMasterDataPut(I2C0_BASE, regAddress);
		I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
		while(I2CMasterBusy(I2C0_BASE));
	}
uint8_t I2CRead(uint8_t slaveAddress)
	{
		//Have to check if the master is busy with either transfer or read with the I2CMasterBusy(I2C0_BASE) function (boolean)
		I2CMasterSlaveAddrSet(I2C0_BASE, slaveAddress, true);
		I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
		while(I2CMasterBusy(I2C0_BASE));
		return I2CMasterDataGet(I2C0_BASE);
	}
void I2CReadN(uint8_t slaveAddress, uint8_t startRegister, uint8_t *data, int n)
	{
		I2CMasterSlaveAddrSet(I2C0_BASE, slaveAddress, false);
		I2CMasterDataPut(I2C0_BASE, startRegister);
		I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
		while(I2CMasterBusy(I2C0_BASE));
		int i = 1;
		I2CMasterSlaveAddrSet(I2C0_BASE, slaveAddress, true);
		I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
		while(I2CMasterBusy(I2C0_BASE));
		data[0] = I2CMasterDataGet(I2C0_BASE);
		while(i<n)
		{
		I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
		while(I2CMasterBusy(I2C0_BASE));
		data[i] = I2CMasterDataGet(I2C0_BASE);
		}
		I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
		while(I2CMasterBusy(I2C0_BASE));
	}


