/***********************************************************************
 * Project      :     tm1638_led
 * Description  :     Test LED on tm1638
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
#include <tiny32_tm1638.h>


/**************************************/
/*        define object variable      */
/**************************************/


/**************************************/
/*       Constand define value        */
/**************************************/
#define WDT_TIMEOUT 10 // set Watchdog timeout 10 second

/**************************************/
/*       eeprom address define        */
/**************************************/


/**************************************/
/*           define function          */
/**************************************/
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
    // LED 1  Test
    tm1638_var.led_tm1638(1, 1);
    vTaskDelay(1000);
    tm1638_var.led_tm1638(1, 0);
    vTaskDelay(1000);

    // LED 2  Test
    tm1638_var.led_tm1638(2, 1);
    vTaskDelay(1000);
    tm1638_var.led_tm1638(2, 0);
    vTaskDelay(1000);

    // LED 3  Test
    tm1638_var.led_tm1638(3, 1);
    vTaskDelay(1000);
    tm1638_var.led_tm1638(3, 0);
    vTaskDelay(1000);

    // LED 4  Test
    tm1638_var.led_tm1638(4, 1);
    vTaskDelay(1000);
    tm1638_var.led_tm1638(4, 0);
    vTaskDelay(1000);

    // LED 5  Test
    tm1638_var.led_tm1638(5, 1);
    vTaskDelay(1000);
    tm1638_var.led_tm1638(5, 0);
    vTaskDelay(1000);

    // LED 6  Test
    tm1638_var.led_tm1638(6, 1);
    vTaskDelay(1000);
    tm1638_var.led_tm1638(6, 0);
    vTaskDelay(1000);

    // LED 7  Test
    tm1638_var.led_tm1638(7, 1);
    vTaskDelay(1000);
    tm1638_var.led_tm1638(7, 0);
    vTaskDelay(1000);

    // LED 8  Test
    tm1638_var.led_tm1638(8, 1);
    vTaskDelay(1000);
    tm1638_var.led_tm1638(8, 0);
    vTaskDelay(1000);
}