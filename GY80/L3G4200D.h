/* Copyright (c) 2011 Pololu Corporation.  For more information, see
 * 
 * http://www.pololu.com/
 * http://forum.pololu.com/
 * 
 * Modified by Rebeca Nunes Rodrigues
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __L3G4200D_H
#define __L3G4200D_H

#include "mbed.h"

// register addresses

#define L3G4200D_WHO_AM_I      0x0F

#define L3G4200D_CTRL_REG1     0x20
#define L3G4200D_CTRL_REG2     0x21
#define L3G4200D_CTRL_REG3     0x22
#define L3G4200D_CTRL_REG4     0x23
#define L3G4200D_CTRL_REG5     0x24
#define L3G4200D_REFERENCE     0x25
#define L3G4200D_OUT_TEMP      0x26
#define L3G4200D_STATUS_REG    0x27

#define L3G4200D_OUT_X_L       0x28
#define L3G4200D_OUT_X_H       0x29
#define L3G4200D_OUT_Y_L       0x2A
#define L3G4200D_OUT_Y_H       0x2B
#define L3G4200D_OUT_Z_L       0x2C
#define L3G4200D_OUT_Z_H       0x2D

#define L3G4200D_FIFO_CTRL_REG 0x2E
#define L3G4200D_FIFO_SRC_REG  0x2F

#define L3G4200D_INT1_CFG      0x30
#define L3G4200D_INT1_SRC      0x31
#define L3G4200D_INT1_THS_XH   0x32
#define L3G4200D_INT1_THS_XL   0x33
#define L3G4200D_INT1_THS_YH   0x34
#define L3G4200D_INT1_THS_YL   0x35
#define L3G4200D_INT1_THS_ZH   0x36
#define L3G4200D_INT1_THS_ZL   0x37
#define L3G4200D_INT1_DURATION 0x38

//Few parameters
typedef char byte;

#define SENSITIVITY_2000DPS 0.07

/** Interface library for the ST L3G4200D 3-axis gyro
 *
 * Ported from Pololu L3G4200D library for Arduino by
 * Michael Shimniok http://bot-thoughts.com
 *
 * @code
 * #include "mbed.h"
 * #include "L3G4200D.h"
 * L3G4200D gyro(p28, p27);
 * ...
 * int g[3];
 * gyro.read(g);
 * @endcode
 */
class L3G4200D
{
    public:
        /** Create a new L3G4200D I2C interface
         * @param sda is the pin for the I2C SDA line
         * @param scl is the pin for the I2C SCL line
         */
        L3G4200D(PinName sda, PinName scl);
        
        /** Read gyro raw values
         * @param g Array containing x, y, and z gyro raw values
         * @return g Array containing x, y, and z gyro raw values
         */
        void read_raw(int g[3]);
        
        /** Read gyro values considering offset
        * @param g Array containing x, y, and z gyro values
        * @return g Array containing x, y, and z gyro values
        */
        void read_dps(float g[3]);
        
        /** Get offset values to calibrate gyro readings
        * @param offset Array containing x, y, and z means to 100 readings
        */
        void get_offset(int offset[3]);

        /* Get offset values and record it to be used by read_dps
        */
        void calibrate();
        
        /** Read yaw angle by integration of z gyro values
        * @param yaw_angle containing the yaw angle value
        */
        void read_yaw_angle(float yaw_angle);
    private:
        int gyro_offset[3];
        Timer timerAngle;
        int current_time, previous_time;
        float gx, gy, gz;
        float gyroAngle, deltat;
        int previous_gz;
        byte data[6];
        int _rates[3];
        I2C _device;
        void writeReg(byte reg, byte value);
        byte readReg(byte reg);
        void enableDefault(void);
};

#endif
