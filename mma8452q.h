#define MMA8452Q_Address 0x1D
#define STATUS 0x00
#define OUT_X_MSB 0x01
#define OUT_X_LSB 0x02
#define OUT_Y_MSB 0x03
#define OUT_Y_LSB 0x04
#define OUT_Z_MSB 0x05
#define OUT_Z_LSB 0x06
#define XYZ_DATA_CFG 0x0E
#define SYSMOD 0x08
#define CTRL_REG1 0x2A
#define CTRL_REG2 0x2B
#define CTRL_REG3 0x2C
#define CTRL_REG4 0x2D
#define CTRL_REG5 0x2E
uint16_t x, y, z;
float cx, cy, cz;
uint8_t rawData[6];
void readAccelerometer();
void setupAccelerometer();
uint8_t readRegister(uint8_t RegisterAddress);
uint8_t writeRegister(uint8_t RegisterAddress, uint8_t data);
