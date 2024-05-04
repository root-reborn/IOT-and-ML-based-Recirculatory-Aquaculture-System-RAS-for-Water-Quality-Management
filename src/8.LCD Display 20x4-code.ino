#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); // I2C address 0x27, 20 column and 4 rows

void setup() {
  lcd.init(); // initialize the lcd
  lcd.backlight();

  lcd.setCursor(0, 0);                       // move cursor the first row
  lcd.print("LCD 20x4");                // print message at the first row
  lcd.setCursor(0, 1);                      // move cursor to the second row
  lcd.print("I2C Address: 0x27");   // print message at the second row
  lcd.setCursor(0, 2);                      // move cursor to the third row
  lcd.print("Hello World");             // print message at the third row
  lcd.setCursor(0, 3);                      // move cursor to the fourth row
  lcd.print("EDP Project");             // print message the fourth row
}

void loop() {
}
