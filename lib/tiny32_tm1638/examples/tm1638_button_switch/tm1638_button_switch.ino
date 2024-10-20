/***********************************************************************
 * Project      :     tm1638_button_switch
 * Description  :     Test Button Switch on tm1638
 * Hardware     :     tiny32_v3 & tm1638
 * Author       :     Tenergy Innovation Co., Ltd.
 * Date         :     155/10/2024
 * Revision     :     1.0
 * Rev1.0       :     Origital
 * website      :     http://www.tenergyinnovation.co.th
 * Email        :     uten.boonliam@tenergyinnovation.co.th
 * TEL          :     +66 89-140-7205
 ***********************************************************************/
#include <Arduino.h>
#include <tiny32_v3.h>
#include <ErriezTM1638.h>

/**************************************/
/*            GPIO define             */
/**************************************/
#define CLK_PIN 19 // Clock pin
#define DIO_PIN 18 // Data I/O pin
#define STB_PIN 23 // Strobe pin

/**************************************/
/*        define object variable      */
/**************************************/
TM1638 tm1638(CLK_PIN, DIO_PIN, STB_PIN);
tiny32_v3 mcu;

/**************************************/
/*       Constand define value        */
/**************************************/
#define WDT_TIMEOUT 10 // set Watchdog timeout 10 second

/**************************************/
/*       eeprom address define        */
/**************************************/

/**************************************/
/*        define global variable      */
/**************************************/
uint8_t _digits[16] = {
    0b00111111, 0b00000110, 0b01011011, 0b01001111,
    0b01100110, 0b01101101, 0b01111101, 0b00000111,
    0b01111111, 0b01101111, 0b01110111, 0b01111100,
    0b00111001, 0b01011110, 0b01111001, 0b01110001};

uint8_t _digits_dot[16] = {
    0b10111111, 0b10000110, 0b11011011, 0b11001111,
    0b11100110, 0b11101101, 0b11111101, 0b10000111,
    0b11111111, 0b11101111, 0b11110111, 0b11111100,
    0b10111001, 0b11011110, 0b11111001, 0b11110001};

/**************************************/
/*           define function          */
/**************************************/

/**************************************/
/*           define class             */
/**************************************/
class tm1638_tiny32
{
private:
    /* data */
public:
    /***********************************************************************
     * FUNCTION:    tm1638_tiny32
     * DESCRIPTION: Construction function
     * PARAMETERS:  nothing
     * RETURNED:    nothing
     ***********************************************************************/
    tm1638_tiny32()
    {
    }

    /***********************************************************************
     * FUNCTION:    led_tm1638
     * DESCRIPTION: ON/OFF LED on TM1638 board
     * PARAMETERS:  digit[1-8],status[true/false]
     * RETURNED:    true/false
     ***********************************************************************/
    bool led_tm1638(int digit, bool status)
    {
        if ((digit < 1) || (digit > 8))
        {
            Serial.printf("Error: digit is %d, its over range it must be 1-8\r\n", digit);
            return false;
        }
        else
        {
            tm1638.writeData(15 - ((digit - 1) * 2), status); // 15-((1-1)*2)
            return true;
        }
        return false;
    }

    /***********************************************************************
     * FUNCTION:    button_sw_tm1638
     * DESCRIPTION: get value from button switch
     * PARAMETERS:  -
     * RETURNED:    0 -> No pressing, 1-8  [S1-S8] as location of switch
     ***********************************************************************/
    u_int8_t button_sw_tm1638(void)
    {
        if (tm1638.getKeys() == 0)
        {
            return 0;
        }
        else
        {
            if (tm1638.getKeys() == 0x00000001)
            {
                return 1;
            }
            else if (tm1638.getKeys() == 0x00000100)
            {
                return 2;
            }
            else if (tm1638.getKeys() == 0x00010000)
            {
                return 3;
            }
            else if (tm1638.getKeys() == 0x01000000)
            {
                return 4;
            }
            else if (tm1638.getKeys() == 0x00000010)
            {
                return 5;
            }
            else if (tm1638.getKeys() == 0x00001000)
            {
                return 6;
            }
            else if (tm1638.getKeys() == 0x00100000)
            {
                return 7;
            }
            else if (tm1638.getKeys() == 0x10000000)
            {
                return 8;
            }
            return 0;
        }
        return 0;
    }

    /***********************************************************************
     * FUNCTION:    number_tm1638
     * DESCRIPTION: display float number
     * PARAMETERS:  float decimal [-99999.99, 99999.99]
     * RETURNED:    true/false
     ***********************************************************************/
    bool number_tm1638(float number)
    {
        bool _negative_flat = false;

        if (number < 0)
        {
            _negative_flat = true;
            number = number * (-1);
        }

        if (number > 100000)
        {
            Serial.printf("Error: %.2f Out of range\r\n", number);
            mcu.buzzer_beep(3);
            return false;
        }
        else
        {
            Serial.printf("Info: Number: %.2f\r\n", number);
        }

        tm1638.clear();

        /* After of point  number */
        int _floatPart = (int)(number * 100) % 100;
        int _after_point_digit1 = (_floatPart / 10) % 10;
        int _after_point_digit2 = _floatPart % 10;
        tm1638.writeData(14, _digits[_after_point_digit2]);
        tm1638.writeData(12, _digits[_after_point_digit1]);

        /* Before of point number number */
        u_int32_t _intPart = floor(number);
        int _before_point_digit1 = _intPart % 10;           // หลักหน่วย
        int _before_point_digit2 = (_intPart / 10) % 10;    // หลักสิบ
        int _before_point_digit3 = (_intPart / 100) % 10;   // หลักสิบ
        int _before_point_digit4 = (_intPart / 1000) % 10;  // หลักสิบ
        int _before_point_digit5 = (_intPart / 10000) % 10; // หลักสิบ

        if (_before_point_digit5 > 0)
        {
            tm1638.writeData(10, _digits_dot[_before_point_digit1]);
            tm1638.writeData(8, _digits[_before_point_digit2]);
            tm1638.writeData(6, _digits[_before_point_digit3]);
            tm1638.writeData(4, _digits[_before_point_digit4]);
            tm1638.writeData(2, _digits[_before_point_digit5]);
            if (_negative_flat)
                tm1638.writeData(0, 64);
        }
        else if (_before_point_digit4 > 0)
        {
            tm1638.writeData(10, _digits_dot[_before_point_digit1]);
            tm1638.writeData(8, _digits[_before_point_digit2]);
            tm1638.writeData(6, _digits[_before_point_digit3]);
            tm1638.writeData(4, _digits[_before_point_digit4]);
            if (_negative_flat)
                tm1638.writeData(2, 64);
        }
        else if (_before_point_digit3 > 0)
        {
            tm1638.writeData(10, _digits_dot[_before_point_digit1]);
            tm1638.writeData(8, _digits[_before_point_digit2]);
            tm1638.writeData(6, _digits[_before_point_digit3]);
            if (_negative_flat)
                tm1638.writeData(4, 64);
        }
        else if (_before_point_digit2 > 0)
        {
            tm1638.writeData(10, _digits_dot[_before_point_digit1]);
            tm1638.writeData(8, _digits[_before_point_digit2]);
            if (_negative_flat)
                tm1638.writeData(6, 64);
        }
        else
        {
            tm1638.writeData(10, _digits_dot[_before_point_digit1]);
            if (_negative_flat)
                tm1638.writeData(8, 64);
        }

        return true;
    }

    /***********************************************************************
     * FUNCTION:    number_tm1638
     * DESCRIPTION: display long number
     * PARAMETERS:  long number [-9999999 to 999999999]
     * RETURNED:    true/false
     ***********************************************************************/
    bool number_tm1638(long number)
    {
        bool _negative_flat = false;

        if (number > 99999999 || number < (-9999999))
        {
            Serial.printf("Error: %ld Out of range\r\n", number);
            mcu.buzzer_beep(3);
            return false;
        }
        else
        {
            Serial.printf("Info: Number: %ld\r\n", number);
        }

        if (number < 0)
        {
            _negative_flat = true;
            number = number * (-1);
        }

        tm1638.clear();

        int _digit1 = number % 10;             // หลักที่ 1
        int _digit2 = (number / 10) % 10;      // หลักที่ 2
        int _digit3 = (number / 100) % 10;     // หลักที่ 3
        int _digit4 = (number / 1000) % 10;    // หลักที่ 4
        int _digit5 = (number / 10000) % 10;   // หลักที่ 5
        int _digit6 = (number / 100000) % 10;  // หลักที่ 6
        int _digit7 = (number / 1000000) % 10; // หลักที่ 7
        int _digit8 = 0;
        if (!_negative_flat)
        {
            _digit8 = (number / 10000000) % 10; // หลักที่ 8
        }

        if (_digit8 > 0)
        {
            tm1638.writeData(14, _digits[_digit1]);
            tm1638.writeData(12, _digits[_digit2]);
            tm1638.writeData(10, _digits[_digit3]);
            tm1638.writeData(8, _digits[_digit4]);
            tm1638.writeData(6, _digits[_digit5]);
            tm1638.writeData(4, _digits[_digit6]);
            tm1638.writeData(2, _digits[_digit7]);
            tm1638.writeData(0, _digits[_digit8]);
        }
        else if (_digit7 > 0)
        {
            tm1638.writeData(14, _digits[_digit1]);
            tm1638.writeData(12, _digits[_digit2]);
            tm1638.writeData(10, _digits[_digit3]);
            tm1638.writeData(8, _digits[_digit4]);
            tm1638.writeData(6, _digits[_digit5]);
            tm1638.writeData(4, _digits[_digit6]);
            tm1638.writeData(2, _digits[_digit7]);
            if (_negative_flat)
                tm1638.writeData(0, 64);
        }
        else if (_digit6 > 0)
        {
            tm1638.writeData(14, _digits[_digit1]);
            tm1638.writeData(12, _digits[_digit2]);
            tm1638.writeData(10, _digits[_digit3]);
            tm1638.writeData(8, _digits[_digit4]);
            tm1638.writeData(6, _digits[_digit5]);
            tm1638.writeData(4, _digits[_digit6]);
            if (_negative_flat)
                tm1638.writeData(2, 64);
        }
        else if (_digit5 > 0)
        {
            tm1638.writeData(14, _digits[_digit1]);
            tm1638.writeData(12, _digits[_digit2]);
            tm1638.writeData(10, _digits[_digit3]);
            tm1638.writeData(8, _digits[_digit4]);
            tm1638.writeData(6, _digits[_digit5]);
            if (_negative_flat)
                tm1638.writeData(4, 64);
        }
        else if (_digit4 > 0)
        {
            tm1638.writeData(14, _digits[_digit1]);
            tm1638.writeData(12, _digits[_digit2]);
            tm1638.writeData(10, _digits[_digit3]);
            tm1638.writeData(8, _digits[_digit4]);
            if (_negative_flat)
                tm1638.writeData(6, 64);
        }
        else if (_digit3 > 0)
        {
            tm1638.writeData(14, _digits[_digit1]);
            tm1638.writeData(12, _digits[_digit2]);
            tm1638.writeData(10, _digits[_digit3]);
            if (_negative_flat)
                tm1638.writeData(8, 64);
        }
        else if (_digit2 > 0)
        {
            tm1638.writeData(14, _digits[_digit1]);
            tm1638.writeData(12, _digits[_digit2]);
            if (_negative_flat)
                tm1638.writeData(10, 64);
        }
        else if (_digit1 > 0)
        {
            tm1638.writeData(14, _digits[_digit1]);
            if (_negative_flat)
                tm1638.writeData(12, 64);
        }

        return true;
    }

    /***********************************************************************
     * FUNCTION:    number_tm1638
     * DESCRIPTION: display int number
     * PARAMETERS:  int number [-32,768 to 32,767]
     * RETURNED:    true/false
     ***********************************************************************/
    bool number_tm1638(int number)
    {
        bool _negative_flat = false;

        if (number > 32767 || number < (-32768))
        {
            Serial.printf("Error: %d Out of range\r\n", number);
            mcu.buzzer_beep(3);
            return false;
        }
        else
        {
            Serial.printf("Info: Number: %d\r\n", number);
        }

        if (number < 0)
        {
            _negative_flat = true;
            number = number * (-1);
        }

        tm1638.clear();

        int _digit1 = number % 10;           // หลักที่ 1
        int _digit2 = (number / 10) % 10;    // หลักที่ 2
        int _digit3 = (number / 100) % 10;   // หลักที่ 3
        int _digit4 = (number / 1000) % 10;  // หลักที่ 4
        int _digit5 = (number / 10000) % 10; // หลักที่ 5

        if (_digit5 > 0)
        {
            tm1638.writeData(14, _digits[_digit1]);
            tm1638.writeData(12, _digits[_digit2]);
            tm1638.writeData(10, _digits[_digit3]);
            tm1638.writeData(8, _digits[_digit4]);
            tm1638.writeData(6, _digits[_digit5]);
            if (_negative_flat)
                tm1638.writeData(4, 64);
        }
        else if (_digit4 > 0)
        {
            tm1638.writeData(14, _digits[_digit1]);
            tm1638.writeData(12, _digits[_digit2]);
            tm1638.writeData(10, _digits[_digit3]);
            tm1638.writeData(8, _digits[_digit4]);
            if (_negative_flat)
                tm1638.writeData(6, 64);
        }
        else if (_digit3 > 0)
        {
            tm1638.writeData(14, _digits[_digit1]);
            tm1638.writeData(12, _digits[_digit2]);
            tm1638.writeData(10, _digits[_digit3]);
            if (_negative_flat)
                tm1638.writeData(8, 64);
        }
        else if (_digit2 > 0)
        {
            tm1638.writeData(14, _digits[_digit1]);
            tm1638.writeData(12, _digits[_digit2]);
            if (_negative_flat)
                tm1638.writeData(10, 64);
        }
        else if (_digit1 > 0)
        {
            tm1638.writeData(14, _digits[_digit1]);
            if (_negative_flat)
                tm1638.writeData(12, 64);
        }
        return true;
    }

    /***********************************************************************
     * FUNCTION:    number_tm1638
     * DESCRIPTION: display int number1, number2
     * PARAMETERS:  int number [-999 to 999]
     * RETURNED:    true/false
     ***********************************************************************/
    bool number_tm1638(int number2, int number1)
    {
        bool _negative_flat1 = false;
        bool _negative_flat2 = false;

        if (number1 > 999 || number1 < (-999))
        {
            Serial.printf("Error: Number1 => %d Out of range\r\n", number1);
            mcu.buzzer_beep(3);
            return false;
        }
        else if (number2 > 999 || number2 < (-999))
        {
            Serial.printf("Error: Number2 => %d Out of range\r\n", number2);
            mcu.buzzer_beep(3);
            return false;
        }
        else
        {
            Serial.printf("Info: Number1: %d\r\n", number1);
            Serial.printf("Info: Number2: %d\r\n", number2);
        }

        if (number1 < 0)
        {
            _negative_flat1 = true;
            number1 = number1 * (-1);
        }

        if (number2 < 0)
        {
            _negative_flat2 = true;
            number2 = number2 * (-1);
        }

        tm1638.clear();

        int _digit1_1 = number1 % 10;         // หลักที่ 1
        int _digit2_1 = (number1 / 10) % 10;  // หลักที่ 2
        int _digit3_1 = (number1 / 100) % 10; // หลักที่ 3

        int _digit1_2 = number2 % 10;         // หลักที่ 1
        int _digit2_2 = (number2 / 10) % 10;  // หลักที่ 2
        int _digit3_2 = (number2 / 100) % 10; // หลักที่ 3

        if (_digit3_1 > 0)
        {
            tm1638.writeData(14, _digits[_digit1_1]);
            tm1638.writeData(12, _digits[_digit2_1]);
            tm1638.writeData(10, _digits[_digit3_1]);

            if (_negative_flat1)
                tm1638.writeData(8, 64);
        }
        else if (_digit2_1 > 0)
        {
            tm1638.writeData(14, _digits[_digit1_1]);
            tm1638.writeData(12, _digits[_digit2_1]);

            if (_negative_flat1)
                tm1638.writeData(10, 64);
        }
        else if (_digit1_1 > 0)
        {
            tm1638.writeData(14, _digits[_digit1_1]);
            if (_negative_flat1)
                tm1638.writeData(12, 64);
        }

        if (_digit3_2 > 0)
        {
            tm1638.writeData(6, _digits[_digit1_2]);
            tm1638.writeData(4, _digits[_digit2_2]);
            tm1638.writeData(2, _digits[_digit3_2]);

            if (_negative_flat1)
                tm1638.writeData(0, 64);
        }
        else if (_digit2_2 > 0)
        {
            tm1638.writeData(6, _digits[_digit1_2]);
            tm1638.writeData(4, _digits[_digit2_2]);

            if (_negative_flat1)
                tm1638.writeData(2, 64);
        }
        else if (_digit1_2 > 0)
        {
            tm1638.writeData(6, _digits[_digit1_2]);
            if (_negative_flat1)
                tm1638.writeData(4, 64);
        }

        return true;
    }

    /***********************************************************************
     * FUNCTION:    number_tm1638
     * DESCRIPTION: display float number
     * PARAMETERS:  float number1, number2 [-9.9, 99.9]
     * RETURNED:    true/false
     ***********************************************************************/
    bool number_tm1638(float number2, float number1)
    {

        bool _negative_flat1 = false;
        bool _negative_flat2 = false;
        if (number1 >= 100 || number1 <= (-100))
        {
            Serial.printf("Error: Number1 => %.1f Out of range\r\n", number1);
            mcu.buzzer_beep(3);
            return false;
        }
        else if (number2 >= 100 || number2 <= (-100))
        {
            Serial.printf("Error: Number2 => %.1f Out of range\r\n", number2);
            mcu.buzzer_beep(3);
            return false;
        }
        else
        {
            Serial.printf("Info: Number1: %.1f\r\n", number1);
            Serial.printf("Info: Number2: %.1f\r\n\r\n", number2);
        }

        if (number1 < 0)
        {
            _negative_flat1 = true;
            number1 = number1 * (-1);
        }
        if (number2 < 0)
        {
            _negative_flat2 = true;
            number2 = number2 * (-1);
        }

        tm1638.clear();

        /* After of point  number */
        int _floatPart2 = (int)(number2 * 10) % 10;
        int _after_point_digit1_2 = _floatPart2 % 10;
        //
        int _floatPart1 = (int)(number1 * 10) % 10;
        int _after_point_digit1_1 = _floatPart1 % 10;

        /* Before of point number number */
        u_int32_t _intPart2 = floor(number2);
        int _before_point_digit1_2 = _intPart2 % 10;        // หลักหน่วย
        int _before_point_digit2_2 = (_intPart2 / 10) % 10; // หลักสิบ
        //
        u_int32_t _intPart1 = floor(number1);
        int _before_point_digit1_1 = _intPart1 % 10;        // หลักหน่วย
        int _before_point_digit2_1 = (_intPart1 / 10) % 10; // หลักสิบ

        // MSB
        if (_before_point_digit2_2 > 0)
        {
            tm1638.writeData(2, _digits[_before_point_digit2_2]);
            tm1638.writeData(4, _digits_dot[_before_point_digit1_2]);
            tm1638.writeData(6, _digits[_after_point_digit1_2]);
            if (_negative_flat2)
                tm1638.writeData(0, 64);
        }
        else if (_before_point_digit1_2 > 0)
        {
            tm1638.writeData(4, _digits_dot[_before_point_digit1_2]);
            tm1638.writeData(6, _digits[_after_point_digit1_2]);
            if (_negative_flat2)
                tm1638.writeData(2, 64);
        }
        else if (_after_point_digit1_2 > 0)
        {
            tm1638.writeData(4, _digits_dot[0]);
            tm1638.writeData(6, _digits[_after_point_digit1_2]);

            if (_negative_flat2)
                tm1638.writeData(2, 64);
        }

        // LSB
        if (_before_point_digit2_1 > 0)
        {
            tm1638.writeData(10, _digits[_before_point_digit2_1]);
            tm1638.writeData(12, _digits_dot[_before_point_digit1_1]);
            tm1638.writeData(14, _digits[_after_point_digit1_1]);
            if (_negative_flat1)
                tm1638.writeData(8, 64);
        }
        else if (_before_point_digit1_1 > 0)
        {
            tm1638.writeData(12, _digits_dot[_before_point_digit1_1]);
            tm1638.writeData(14, _digits[_after_point_digit1_1]);
            if (_negative_flat1)
                tm1638.writeData(10, 64);
        }
        else if (_after_point_digit1_1 > 0)
        {
            tm1638.writeData(12, _digits_dot[0]);
            tm1638.writeData(14, _digits[_after_point_digit1_1]);

            if (_negative_flat1)
                tm1638.writeData(10, 64);
        }
        return true;
    }

    /***********************************************************************
     * FUNCTION:    time_tm1638
     * DESCRIPTION: display clock number
     * PARAMETERS:  int hh, mm, ss
     * RETURNED:    true/false
     ***********************************************************************/
    bool time_tm1638(int hh, int mm, int ss)
    {
        if (hh > 23 || hh < 0)
        {
            Serial.printf("Error: hh => %d Out of range\r\n", hh);
            mcu.buzzer_beep(3);
            return false;
        }
        else if (mm > 59 || mm < 0)
        {
            Serial.printf("Error: mm => %d Out of range\r\n", mm);
            mcu.buzzer_beep(3);
            return false;
        }
        else if (ss > 59 || ss < 0)
        {
            Serial.printf("Error: ss => %d Out of range\r\n", ss);
            mcu.buzzer_beep(3);
            return false;
        }
        else
        {
            Serial.printf("Info:  %02d:%02d:%02d\r\n", hh, mm, ss);
        }

        int _digit1_hh = hh % 10;        // หลักที่ 1
        int _digit2_hh = (hh / 10) % 10; // หลักที่ 2

        int _digit1_mm = mm % 10;        // หลักที่ 1
        int _digit2_mm = (mm / 10) % 10; // หลักที่ 2

        int _digit1_ss = ss % 10;        // หลักที่ 1
        int _digit2_ss = (ss / 10) % 10; // หลักที่ 2

        tm1638.clear();

        // show hour
        if (_digit2_hh > 0)
        {
            tm1638.writeData(0, _digits[_digit2_hh]);
            tm1638.writeData(2, _digits[_digit1_hh]);
        }
        else
        {
            tm1638.writeData(2, _digits[_digit1_hh]);
        }

        // show minute
        tm1638.writeData(6, _digits[_digit2_mm]);
        tm1638.writeData(8, _digits[_digit1_mm]);
        tm1638.writeData(4, 64);

        // show minute
        tm1638.writeData(12, _digits[_digit2_ss]);
        tm1638.writeData(14, _digits[_digit1_ss]);
        tm1638.writeData(10, 64);

        return true;
    }

    /***********************************************************************
     * FUNCTION:    date_tm1638
     * DESCRIPTION: display date
     * PARAMETERS:  int DD, MM, YY
     * RETURNED:    true/false
     ***********************************************************************/
    bool date_tm1638(int DD, int MM, int YY)
    {
        if (DD > 31 || DD < 1)
        {
            Serial.printf("Error: DD => %d Out of range\r\n", DD);
            mcu.buzzer_beep(3);
            return false;
        }
        else if (MM > 12 || MM < 1)
        {
            Serial.printf("Error: MM => %d Out of range\r\n", MM);
            mcu.buzzer_beep(3);
            return false;
        }
        else if (YY > 3000 || YY < 0)
        {
            Serial.printf("Error: YY => %d Out of range\r\n", YY);
            mcu.buzzer_beep(3);
            return false;
        }
        else
        {
            Serial.printf("Info:  %02d:%02d:%d\r\n", DD, MM, YY);
        }

        int _digit1_DD = DD % 10;        // หลักที่ 1
        int _digit2_DD = (DD / 10) % 10; // หลักที่ 2

        int _digit1_MM = MM % 10;        // หลักที่ 1
        int _digit2_MM = (MM / 10) % 10; // หลักที่ 2

        int _digit1_YY = YY % 10;        // หลักที่ 1
        int _digit2_YY = (YY / 10) % 10; // หลักที่ 2

        tm1638.clear();

        // show hour
        if (_digit2_DD > 0)
        {
            tm1638.writeData(0, _digits[_digit2_DD]);
            tm1638.writeData(2, _digits[_digit1_DD]);
        }
        else
        {
            tm1638.writeData(2, _digits[_digit1_DD]);
        }

        // show minute
        tm1638.writeData(6, _digits[_digit2_MM]);
        tm1638.writeData(8, _digits[_digit1_MM]);
        tm1638.writeData(4, 64);

        // show minute
        tm1638.writeData(12, _digits[_digit2_YY]);
        tm1638.writeData(14, _digits[_digit1_YY]);
        tm1638.writeData(10, 64);

        return true;
    }
};

tm1638_tiny32 tm1638_var;
/***********************************************************************
 * FUNCTION:    setup
 * DESCRIPTION: setup process
 * PARAMETERS:  nothing
 * RETURNED:    nothing
 ***********************************************************************/
void setup()
{
    Serial.begin(115200);

    // Initialize TM1638
    tm1638.begin();

    // Turn display off (All LED's off)
    tm1638.displayOff();

    // Clear display
    tm1638.clear();

    // Set brightness 0..7
    tm1638.setBrightness(1);

    // Turn display on
    tm1638.displayOn();

    mcu.buzzer_beep(2);
}

/***********************************************************************
 * FUNCTION:    loop
 * DESCRIPTION: loop process
 * PARAMETERS:  nothing
 * RETURNED:    nothing
 ***********************************************************************/
void loop()
{
    if (tm1638_var.button_sw_tm1638() == 0)
    {
        NULL;
    }
    else
    {
        Serial.printf("SW%d\r\n", tm1638_var.button_sw_tm1638());
    }
    vTaskDelay(100);
}