/*
    use DS3231 + LCD 1602 to build a RTC
    display format: | 2020/06/20 Wed |
                    |17:38:21 30.50°C|
    using: arduino NANO
*/
#include <Wire.h>
#include <DS3231.h>
#include <LiquidCrystal_I2C.h>

const int button_pin = 12;
volatile bool isBacklightOn = true;

Time time;
String first_line;
String second_line;

DS3231 rtc(SDA, SCL);
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
// args as follows:   addr|en|rw|rs|d4|d5|d6|d7|bl| blpol

void setup() {
    Serial.begin(115200);
    pinMode(button_pin, INPUT_PULLUP);
    rtc.begin();
    lcd.begin(16, 2);
    
    // setup current date & time
    // rtc.setDate(20, 6, 2020); // Set the date to 20/04/2020
    // rtc.setDOW(WEDNESDAY);    // Set Day-of-Week to SUNDAY
    // rtc.setTime(17, 29, 2);  // Set the time to 12:00:00 (24hr format)
}
 
void loop() {
    if (digitalRead(button_pin) == HIGH) { // toggle the LCD backlight
        if (isBacklightOn) lcd.noBacklight();
        else lcd.backlight();
        isBacklightOn = !isBacklightOn;
    }

    time = rtc.getTime();
    first_line = time.year + String("/") + zeroPadding(time.mon) + String("/") + zeroPadding(time.date) + String(" ") + rtc.getDOWStr(FORMAT_SHORT);
    second_line = zeroPadding(time.hour) + String(":") + zeroPadding(time.min) + String(":") + zeroPadding(time.sec) + String(" ") + rtc.getTemp();
    // for debugging
    // Serial.println(first_line);
    // Serial.println(second_line);

    lcd.setCursor(0, 0);
    lcd.print(" " + first_line);
    lcd.setCursor(0, 1);
    lcd.print(second_line + (char)223 + "C");
    delay(1000);
}

String zeroPadding(int num) {
    if (0 <= num && num <= 9) return String(0) + String(num);
    else return String(num);
}
