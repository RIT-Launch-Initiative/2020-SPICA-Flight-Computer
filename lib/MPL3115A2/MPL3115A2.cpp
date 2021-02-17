/*!
 *  @file MPL3115A2.cpp
 *
 *  @mainpage Adafruit MPL3115A2 alitmeter
 *
 *  @section intro_sec Introduction
 *
 *  This is the documentation for Adafruit's MPL3115A2 driver for the
 *  Arduino platform.  It is designed specifically to work with the
 *  Adafruit MPL3115A2 breakout: https://www.adafruit.com/products/1893
 *
 *  These sensors use I2C to communicate, 2 pins (SCL+SDA) are required
 *  to interface with the breakout.
 *
 *  Adafruit invests time and resources providing this open source code,
 *  please support Adafruit and open-source hardware by purchasing
 *  products from Adafruit!
 *
 *  @section dependencies Dependencies
 *
 *  @section author Author
 *
 *  Written by Kevin "KTOWN" Townsend for Adafruit Industries.
 *
 *  @section license License
 *
 *  BSD license, all text here must be included in any redistribution.
 *
 *  Modified to work with HAL
 */

#include <stdio.h>
#include "lib/MPL3115A2/MPL3115A2.h"

/*!
    @brief  Instantiates a new MPL3115A2 class
*/
MPL3115A2::MPL3115A2() {}

/*!
 *   @brief  Setups the HW (reads coefficients values, etc.)
 *   @param  twoWire
 *           Optional TwoWire I2C object
 *   @return true on successful startup, false otherwise
 */
bool MPL3115A2::begin(I2C_HandleTypeDef* i2c) {
  this->i2c = i2c;
  //_i2c->begin();
  uint8_t whoami = read8(MPL3115A2_WHOAMI);
  if (whoami != 0xC4) {
    return false;
  }

  write8(MPL3115A2_CTRL_REG1, MPL3115A2_CTRL_REG1_RST);
  HAL_Delay(10);

  while (read8(MPL3115A2_CTRL_REG1) & MPL3115A2_CTRL_REG1_RST)
    HAL_Delay(10);

  _ctrl_reg1.reg = MPL3115A2_CTRL_REG1_OS128 | MPL3115A2_CTRL_REG1_ALT;

  write8(MPL3115A2_CTRL_REG1, _ctrl_reg1.reg);

  write8(MPL3115A2_PT_DATA_CFG, MPL3115A2_PT_DATA_CFG_TDEFE |
                                    MPL3115A2_PT_DATA_CFG_PDEFE |
                                    MPL3115A2_PT_DATA_CFG_DREM);
  return true;
}

/*!
 *  @brief  Gets the floating-point pressure level in kPa
 *  @return altitude reading as a floating point value
 */
float MPL3115A2::getPressure() {
  uint32_t pressure;

  while (read8(MPL3115A2_CTRL_REG1) & MPL3115A2_CTRL_REG1_OST)
    HAL_Delay(10);

  _ctrl_reg1.bit.ALT = 0;
  write8(MPL3115A2_CTRL_REG1, _ctrl_reg1.reg);

  _ctrl_reg1.bit.OST = 1;
  write8(MPL3115A2_CTRL_REG1, _ctrl_reg1.reg);

  uint8_t sta = 0;
  while (!(sta & MPL3115A2_REGISTER_STATUS_PDR)) {
    sta = read8(MPL3115A2_REGISTER_STATUS);
    HAL_Delay(10);
  }
  //_i2c->beginTransmission(MPL3115A2_ADDRESS); // start transmission to device
  //_i2c->write(MPL3115A2_REGISTER_PRESSURE_MSB);
  //_i2c->endTransmission(false); // end transmission

  //uint8_t data = (uint8_t)MPL3115A2_REGISTER_PRESSURE_MSB;
  //HAL_I2C_Master_Transmit(i2c, MPL3115A2_ADDRESS<<1, &data, 1, MPL3115A2_TIMEOUT);

  //_i2c->requestFrom((uint8_t)MPL3115A2_ADDRESS,
  //                  (uint8_t)3); // send data n-bytes read
  uint8_t received[3];
  //HAL_I2C_Master_Receive(i2c, MPL3115A2_ADDRESS<<1, received, 3, MPL3115A2_TIMEOUT);
  HAL_I2C_Mem_Read(i2c, MPL3115A2_ADDRESS, MPL3115A2_REGISTER_PRESSURE_MSB, 1, received, 3, MPL3115A2_TIMEOUT);

  //pressure = _i2c->read();       // receive DATA
  pressure = received[0];
  pressure <<= 8;
  //pressure |= _i2c->read(); // receive DATA
  pressure |= received[1];
  pressure <<= 8;
  //pressure |= _i2c->read(); // receive DATA
  pressure |= received[2];
  pressure >>= 4;

  float baro = pressure;
  baro /= 4.0;
  return baro;
}

/*!
 *  @brief  Try to read the pressure in kPa and set 'p' to the value on RET_OK
 *  @return current status of the read, RET_OK when done, RET_BLOCKED if blocking, RET_ERROR on error
 */
RetType MPL3115A2::getPressure(float* p) {
  if(read8(MPL3115A2_CTRL_REG1) & MPL3115A2_CTRL_REG1_OST) {
      return RET_BLOCKED; // block
  }

  _ctrl_reg1.bit.ALT = 0;
  write8(MPL3115A2_CTRL_REG1, _ctrl_reg1.reg);

  _ctrl_reg1.bit.OST = 1;
  write8(MPL3115A2_CTRL_REG1, _ctrl_reg1.reg);

  if(!(read8(MPL3115A2_REGISTER_STATUS) & MPL3115A2_REGISTER_STATUS_PDR)) {
      return RET_BLOCKED;
  }

  uint8_t received[3];
  HAL_I2C_Mem_Read(i2c, MPL3115A2_ADDRESS, MPL3115A2_REGISTER_PRESSURE_MSB, 1, received, 3, MPL3115A2_TIMEOUT);

  uint32_t pressure;
  pressure = received[0];
  pressure <<= 8;
  pressure |= received[1];
  pressure <<= 8;
  pressure |= received[2];
  pressure >>= 4;

  *p = pressure;
  *p /= 4.0;
  return RET_OK;
}

/*!
 *  @brief  Gets the floating-point altitude value
 *  @return altitude reading as a floating-point value
 */
float MPL3115A2::getAltitude() {
  int32_t alt;

  while (read8(MPL3115A2_CTRL_REG1) & MPL3115A2_CTRL_REG1_OST)
    HAL_Delay(10);

  _ctrl_reg1.bit.ALT = 1;
  write8(MPL3115A2_CTRL_REG1, _ctrl_reg1.reg);

  _ctrl_reg1.bit.OST = 1;
  write8(MPL3115A2_CTRL_REG1, _ctrl_reg1.reg);

  uint8_t sta = 0;
  while (!(sta & MPL3115A2_REGISTER_STATUS_PDR)) {
    sta = read8(MPL3115A2_REGISTER_STATUS);
    HAL_Delay(10);
  }
  //_i2c->beginTransmission(MPL3115A2_ADDRESS); // start transmission to device
  //_i2c->write(MPL3115A2_REGISTER_PRESSURE_MSB);
  //_i2c->endTransmission(false); // end transmission

  //uint8_t data = (uint8_t)MPL3115A2_REGISTER_PRESSURE_MSB;
  //HAL_I2C_Master_Transmit(i2c, MPL3115A2_ADDRESS<<1, &data, 1, MPL3115A2_TIMEOUT);

  //_i2c->requestFrom((uint8_t)MPL3115A2_ADDRESS,
  //                  (uint8_t)3);         // send data n-bytes read
  uint8_t received[3];
  //HAL_I2C_Master_Receive(i2c, MPL3115A2_ADDRESS<<1, received, 3, MPL3115A2_TIMEOUT);
  HAL_I2C_Mem_Read(i2c, MPL3115A2_ADDRESS, MPL3115A2_REGISTER_PRESSURE_MSB, 1, received, 3, MPL3115A2_TIMEOUT);

  //alt = ((uint32_t)_i2c->read()) << 24;  // receive DATA
  //alt |= ((uint32_t)_i2c->read()) << 16; // receive DATA
  //alt |= ((uint32_t)_i2c->read()) << 8;  // receive DATA
  alt = ((uint32_t)received[0]) << 24;  // receive DATA
  alt |= ((uint32_t)received[1]) << 16; // receive DATA
  alt |= ((uint32_t)received[2]) << 8;  // receive DATA

  float altitude = alt;
  altitude /= 65536.0;
  return altitude;
}

/*!
 *  @brief  Try to read the altitude and set 'a' to the value on RET_OK
 *  @return RET_OK on read, RET_BLOCKING if blocking, RET_ERROR on error
 */
RetType MPL3115A2::getAltitude(float* a) {
  if(read8(MPL3115A2_CTRL_REG1) & MPL3115A2_CTRL_REG1_OST) {
      return RET_BLOCKED;
  }

  _ctrl_reg1.bit.ALT = 1;
  write8(MPL3115A2_CTRL_REG1, _ctrl_reg1.reg);

  _ctrl_reg1.bit.OST = 1;
  write8(MPL3115A2_CTRL_REG1, _ctrl_reg1.reg);

  if(!(read8(MPL3115A2_REGISTER_STATUS) &  MPL3115A2_REGISTER_STATUS_PDR)) {
      return RET_BLOCKED;
  }

  uint8_t received[3];
  HAL_I2C_Mem_Read(i2c, MPL3115A2_ADDRESS, MPL3115A2_REGISTER_PRESSURE_MSB, 1, received, 3, MPL3115A2_TIMEOUT);

  int32_t alt;
  alt = ((uint32_t)received[0]) << 24;  // receive DATA
  alt |= ((uint32_t)received[1]) << 16; // receive DATA
  alt |= ((uint32_t)received[2]) << 8;  // receive DATA

  *a = alt;
  *a /= 65536.0;
  return RET_OK;
}

/*!
 *  @brief  Set the local sea level barometric pressure
 *  @param pascal the pressure to use as the baseline
 */
void MPL3115A2::setSeaPressure(float pascal) {
  uint16_t bar = pascal / 2;
  //_i2c->beginTransmission(MPL3115A2_ADDRESS);
  //_i2c->write((uint8_t)MPL3115A2_BAR_IN_MSB);
  //_i2c->write((uint8_t)(bar >> 8));
  //_i2c->write((uint8_t)bar);
  //_i2c->endTransmission(false);
  uint8_t data[2];
  //data[0] = (uint8_t)MPL3115A2_BAR_IN_MSB;
  data[0] = (uint8_t)(bar >> 8);
  data[1] = (uint8_t)bar;
  //HAL_I2C_Master_Transmit(i2c, MPL3115A2_ADDRESS<<1, data, 3, MPL3115A2_TIMEOUT);
  HAL_I2C_Mem_Write(i2c, MPL3115A2_ADDRESS, MPL3115A2_BAR_IN_MSB, 1, data, 2, MPL3115A2_TIMEOUT);
}

/*!
 *  @brief  Gets the floating-point temperature in Centigrade
 *  @return temperature reading in Centigrade as a floating-point value
 */
float MPL3115A2::getTemperature() {
  int16_t t;

  _ctrl_reg1.bit.OST = 1;
  write8(MPL3115A2_CTRL_REG1, _ctrl_reg1.reg);

  uint8_t sta = 0;
  while (!(sta & MPL3115A2_REGISTER_STATUS_TDR)) {
    sta = read8(MPL3115A2_REGISTER_STATUS);
    HAL_Delay(10);
  }
  //_i2c->beginTransmission(MPL3115A2_ADDRESS); // start transmission to device
  //_i2c->write(MPL3115A2_REGISTER_TEMP_MSB);
  //_i2c->endTransmission(false); // end transmission

  //uint8_t data = (uint8_t)MPL3115A2_REGISTER_TEMP_MSB;
  //HAL_I2C_Master_Transmit(i2c, MPL3115A2_ADDRESS<<1, &data, 1, MPL3115A2_TIMEOUT);

  //_i2c->requestFrom((uint8_t)MPL3115A2_ADDRESS,
  //                  (uint8_t)2); // send data n-bytes read
  uint8_t received[2];
  //HAL_I2C_Master_Receive(i2c, MPL3115A2_ADDRESS<<1, received, 2, MPL3115A2_TIMEOUT);
  HAL_I2C_Mem_Read(i2c, MPL3115A2_ADDRESS, MPL3115A2_REGISTER_TEMP_MSB, 1, received, 2, MPL3115A2_TIMEOUT);

  //t = _i2c->read();              // receive DATA
  t = received[0];
  t <<= 8;
  //t |= _i2c->read(); // receive DATA
  t |= received[1];
  t >>= 4;

  if (t & 0x800) {
    t |= 0xF000;
  }

  float temp = t;
  temp /= 16.0;
  return temp;
}

/*!
 *  @brief  Read the temperature and set 't' on RET_OK
 *  @return RET_OK on read, RET_BLOCKED when blocking, RET_ERROR on error
 */
RetType MPL3115A2::getTemperature(float* t) {
  _ctrl_reg1.bit.OST = 1;
  write8(MPL3115A2_CTRL_REG1, _ctrl_reg1.reg);

  if(!(read8(MPL3115A2_REGISTER_STATUS) & MPL3115A2_REGISTER_STATUS_TDR)) {
      return RET_BLOCKED;
  }

  uint8_t received[2];
  HAL_I2C_Mem_Read(i2c, MPL3115A2_ADDRESS, MPL3115A2_REGISTER_TEMP_MSB, 1, received, 2, MPL3115A2_TIMEOUT);

  int16_t temp;
  temp = received[0];
  temp <<= 8;
  temp |= received[1];
  temp >>= 4;

  if (temp & 0x800) {
    temp |= 0xF000;
  }

  *t = temp;
  *t /= 16.0;
  return RET_OK;
}

/*!
 *  @brief  read 1 byte of data at the specified address
 *  @param  a
 *          the address to read
 *  @return the read data byte
 */
uint8_t MPL3115A2::read8(uint16_t a) {
  //_i2c->beginTransmission(MPL3115A2_ADDRESS); // start transmission to device
  //_i2c->write(a);               // sends register address to read from
  //_i2c->endTransmission(false); // end transmission

  //HAL_I2C_Master_Transmit(i2c, MPL3115A2_ADDRESS<<1, &a, 1, MPL3115A2_TIMEOUT);

  //_i2c->requestFrom((uint8_t)MPL3115A2_ADDRESS,
  //              (uint8_t)1); // send data n-bytes read
  //return _i2c->read();           // receive DATA
  uint8_t received;
  //HAL_I2C_Master_Receive(i2c, MPL3115A2_ADDRESS<<1, &received, 1, MPL3115A2_TIMEOUT);
  HAL_StatusTypeDef status = HAL_I2C_Mem_Read(i2c, MPL3115A2_ADDRESS, a, 1, &received, 1, MPL3115A2_TIMEOUT);
  if(HAL_ERROR == status) {
      return 0;
  } else {
      return received;
  }
}

/*!
 *  @brief  write a byte of data to the specified address
 *  @param  a
 *          the address to write to
 *  @param  d
 *          the byte to write
 */
void MPL3115A2::write8(uint16_t a, uint8_t d) {
  //_i2c->beginTransmission(MPL3115A2_ADDRESS); // start transmission to device
  //_i2c->write(a);               // sends register address to write to
  //_i2c->write(d);               // sends register data
  //_i2c->endTransmission(false); // end transmission

  //uint8_t send[2];
  //send[0] = a;
  //send[1] = d;
  //HAL_I2C_Master_Transmit(i2c, MPL3115A2_ADDRESS<<1, send, 2, MPL3115A2_TIMEOUT);
  uint8_t send = d;
  HAL_I2C_Mem_Write(i2c, MPL3115A2_ADDRESS, (uint16_t)a, 1, &send, 1, MPL3115A2_TIMEOUT);
}
