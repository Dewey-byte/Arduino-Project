#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 2);  // I2C LCD setup

#include <Keypad.h>

const byte ROWS = 4;  // Mga rows para sa keypad
const byte COLS = 4;  // Mga columns para sa keypad

// Layout sa keypad
char keys[ROWS][COLS] = {
  {'1', '2', '3', '+'},
  {'4', '5', '6', '-'},
  {'7', '8', '9', '*'},
  {'C', '0', '=', '/'}
};

// Mga pin assignments para sa rows ug columns sa keypad
byte rowPins[ROWS] = {10, 9, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

// Initialization sa keypad
Keypad myKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

boolean presentValue = false;  // Flag para malaman kung ang second number gisulod na
String num1 = "", num2 = "";  // Mga variable para sa una ug ikaduhang numero
long answer = 0;  // Variable para sa resulta, long type para mas dagko nga value
char op;  // Variable para sa operator

void setup() {
  lcd.init();  // Initialize ang LCD
  lcd.backlight();  // I-on ang backlight sa LCD
  lcd.setCursor(2, 0);
  lcd.print("Pretty Jane");  // I-display ang message
  lcd.setCursor(7, 1);
  lcd.print("^_^");  // I-display ang smiley
  delay(6000);  // Paabota og 6 ka segundo
  lcd.clear();  // I-clear ang screen
}

void loop() {
  char key = myKeypad.getKey();  // Kuhaa ang key nga gi-press sa keypad

  if (key != NO_KEY && (key >= '0' && key <= '9')) {  // Kung ang gi-press kay numero
    if (!presentValue) {  // Kung wala pa ta mag-input sa ikaduhang numero
      num1 += key;  // I-add ang key sa num1
      lcd.setCursor(0, 0);  // Ibutang ang cursor sa top-left
      lcd.print(num1);  // I-display ang una nga numero
    } else {  // Kung mag-input ta sa ikaduhang numero (human sa operator)
      num2 += key;  // I-add ang key sa num2
      lcd.setCursor(num1.length() + 1, 0);  // Ibutang ang cursor pagkahuman sa num1
      lcd.print(num2);  // I-display ang ikaduhang numero
    }
  }

  else if (!presentValue && (key == '/' || key == '*' || key == '+' || key == '-')) {  // Kung operator ang gi-press
    op = key;  // I-store ang operator
    presentValue = true;  // Flag nga nagsugod na ta sa pag-input sa ikaduhang numero
    lcd.setCursor(num1.length(), 0);  // Ibutang ang cursor pagkahuman sa num1
    lcd.print(op);  // I-display ang operator
  }

  else if (key == '=' && presentValue) {  // Kung gi-press ang '=' human sa operator
    // I-perform ang calculation base sa operator
    if (op == '+') answer = num1.toInt() + num2.toInt();
    else if (op == '-') answer = num1.toInt() - num2.toInt();
    else if (op == '*') answer = num1.toInt() * num2.toInt();
    else if (op == '/') {
      if (num2.toInt() == 0) {  // Kung mag-divide by zero, ipakita ang error
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Err: Div by 0");
        return;
      }
      answer = num1.toInt() / num2.toInt();  // Perform division
    }

    // Convert ang answer to string ug tan-awa kung magkasya ba sa screen
    String answerStr = String(answer);
    if (answerStr.length() > 5) {  // Kung ang answer lapas na sa screen capacity, ipakita ang error
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Syntax Error");
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);  // I-clear ang screen ug ipakita ang result
      lcd.print(answer);
    }
    
    presentValue = false;  // I-reset ang state
    num1 = num2 = "";  // I-clear ang mga numero
  }

  else if (key == 'C') {  // Kung gi-press ang 'C', i-clear ang calculator
    lcd.clear();  // I-clear ang screen
    presentValue = false;  // I-reset ang state
    num1 = num2 = "";  // I-clear ang mga numero
    answer = 0;  // I-reset ang answer
    op = ' ';  // I-clear ang operator
  }
}
