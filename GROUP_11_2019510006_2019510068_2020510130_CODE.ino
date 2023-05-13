#include <Wire.h>
// #include <LiquidCrystal_I2C.h>
#include <LiquidCrystalTr_I2C.h>
LiquidCrystal_I2C lcdekranim(0x3f, 16, 2);
boolean SCROLL_FLAG = true;
int SCROLL_WAIT = 1000;
char SCROLL_DIRECTION = 'L';
char LCD_ROW_1[] = "This text is for row 0 and longer than 16 characters.";
char LCD_ROW_2[] = "ABCD EFGH";
char temp_row_1[17];
char temp_row_2[17];
int row = 0;
int temp;
int buttonState = 0;
String temp_str;
String temp_str_2;
void setup() {
  pinMode(2, INPUT_PULLUP);
  lcdekranim.begin();
  lcdekranim.backlight();
  lcdekranim.clear();
  //  pinMode(2, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), change_direction, RISING);
  Serial.begin(9600);

  if (strlen(LCD_ROW_1) < 16)
    fill_array(LCD_ROW_1, temp_row_1, strlen(LCD_ROW_1));
  if (strlen(LCD_ROW_2) < 16) {
    fill_array(LCD_ROW_2, temp_row_2, strlen(LCD_ROW_2));
  }
}
void scrolling(char temp_arr[], int N) {      // Shifting array depends on scroll direction
  if (SCROLL_DIRECTION == 'L') {
    temp = temp_arr[0];
    for (int i = 0; i < N - 1; i++)
      temp_arr[i] = temp_arr[i + 1];

    temp_arr[N - 1] = temp;
  } else {
    temp = temp_arr[N - 1];
    for (int i = N - 1; i > 0; i--)
      temp_arr[i] = temp_arr[i - 1];

    temp_arr[0] = temp;
  }
}
void fill_array(char temp_arr[], char *new_arr, int size) {   // If any array size (number of characters) is less than 16, it will be transferred to a new array.
  for (int i = 0; i < 16; i++) {
    if (i <= size - 1) {
      new_arr[i] = temp_arr[i];
    } else {
      new_arr[i] = ' ';
    }
  }
}
void print3() {     // For print the first 15 terms of the array
  temp_str = "";
  temp_str_2 = "";
  for (int i = 0; i < 16; i++) {
    if (strlen(temp_row_1) == 0 && LCD_ROW_1[i] != '\0') {
      temp_str += LCD_ROW_1[i];
    } else if (temp_row_1[i] != '\0') {
      temp_str += temp_row_1[i];
    }
    if (strlen(temp_row_2) == 0 && LCD_ROW_2[i] != '\0') {
      temp_str_2 += LCD_ROW_2[i];
    } else if (temp_row_2[i] != '\0') {
      temp_str_2 += temp_row_2[i];
    }
  }
  lcdekranim.print(temp_str);
  lcdekranim.setCursor(0, 1);
  lcdekranim.print(temp_str_2);
}
void change_direction() {       // Change scroll direction
  if (SCROLL_DIRECTION == 'R')
    SCROLL_DIRECTION = 'L';
  else if (SCROLL_DIRECTION == 'L')
    SCROLL_DIRECTION = 'R';
}

void loop() {
  buttonState = digitalRead(2);
  if (SCROLL_FLAG && row != 0) {        // Start Scrolling
    if (strlen(LCD_ROW_1) < 16)
      scrolling(temp_row_1, strlen(temp_row_1));
    else
      scrolling(LCD_ROW_1, strlen(LCD_ROW_1));
    if (strlen(LCD_ROW_2) < 16) {
      scrolling(temp_row_2, strlen(temp_row_2));
    } else
      scrolling(LCD_ROW_2, strlen(LCD_ROW_2));
    lcdekranim.clear();
    print3();
    wait();
  } else if (row == 0) {
    print3();
    delay(SCROLL_WAIT);
    row++;
  }
}
void wait() {   // To capture the button input.
  long start = millis();
  while (millis() < start + SCROLL_WAIT) {}
}