// Date, Time and Alarm functions using a DS3231 RTC connected via I2C and Wire lib
/*DS3231
    SDA > A4
    SCL > A5
*/


#include <Wire.h>
#include <SPI.h>  // not used here, but needed to prevent a RTClib compile error
#include <RTClib.h>
#include <LiquidCrystal.h>


RTC_DS3231 RTC;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int screenWidth = 20;
int screenHeight = 4;
String line1 = "Nart Namtal";
int stringStart, stringStop = 0;
int scrollCursor = screenWidth;
byte customChar[] = {
  0x00,
  0x0A,
  0x1F,
  0x1F,
  0x1F,
  0x0E,
  0x04,
  0x00
};

void setup() {
  Serial.begin(115200);
  Wire.begin();
  RTC.begin();
  lcd.begin(screenWidth,screenHeight);
    
  RTC.adjust(DateTime(__DATE__, __TIME__));
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  DateTime now = RTC.now();
  //RTC.setAlarm1Simple(21, 58);
  RTC.turnOnAlarm(0);
  if (RTC.checkAlarmEnabled(1)) {
    Serial.println("Alarm Enabled");
  }
}

void loop () {
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  /*lcd.createChar(0, customChar);
  lcd.setCursor(12, 0);
  lcd.write(byte(0));*/
  lcd.setCursor(0, 1);
  lcd.print("Date:");
  lcd.setCursor(11, 1);
  lcd.print('-');
  lcd.setCursor(15, 1);
  lcd.print('-');
  lcd.setCursor(0, 2);
  lcd.print("Time:");

  DateTime now = RTC.now();
  float temp = RTC.getTemperature();

  lcd.setCursor(6, 0);
  lcd.print(temp);

  const char* dayChar = Day(now.dayOfWeek());
  lcd.setCursor(6, 1);
  lcd.print(dayChar);

  if(now.day() < 10) {
    lcd.setCursor(9, 1);
    lcd.print(0);
    lcd.print(now.day(), DEC);
  } else {
    lcd.setCursor(9, 1);
    lcd.print(now.day(), DEC);
  }
  const char* monthChar = Month(now.month());
  lcd.setCursor(12, 1);
  lcd.print(monthChar);

  lcd.setCursor(16, 1);
  lcd.print(now.year(), DEC);

  if(now.hour() < 10) {
    lcd.setCursor(6, 2);
    lcd.print(0);
    lcd.print(now.hour());
  } else {
    lcd.setCursor(6, 2);
    lcd.print(now.hour());
  }
  lcd.print(':');
  if(now.minute() < 10) {
    lcd.setCursor(9, 2);
    lcd.print(0);
    lcd.print(now.minute());
  } else {
    lcd.print(now.minute());
  }
  lcd.print(':');
  if(now.second() < 10) {
    lcd.setCursor(12, 2);
    lcd.print(0);
    lcd.print(now.second());
  } else {
    lcd.print(now.second());
  }

  if (RTC.checkIfAlarm(1)) {
    Serial.println("Alarm Triggered");
  } 
  lcd.setCursor(scrollCursor, 3);
  lcd.print(line1.substring(stringStart,stringStop));
  if(stringStart == 0 && scrollCursor > 0){
    scrollCursor--;
    if(stringStop < line1.length()) {
      stringStop++;
    }
  } else if(scrollCursor == 0 && stringStart < stringStop) {
    stringStart++;
  } else {
    scrollCursor = screenWidth;
    stringStart = stringStop = 0;
  }
  delay(1000);
  lcd.clear();
}

const char* Day(int _dayOfWeek) {
  const char* dayChar;
  switch(_dayOfWeek) {
    case 0:
      dayChar = "Su";
      break;
    case 1:
      dayChar = "Mo";
      break;
    case 2:
      dayChar = "Tu";
      break;
    case 3:
      dayChar = "We";
      break;
    case 4:
      dayChar = "Th";
      break;
    case 5:
      dayChar = "Fr";
      break;
    case 6:
      dayChar = "Sa";
      break;
    default:
      break;
  }
  return dayChar;
}

const char* Month(int _month) {
  const char* monthChar;
  switch(_month) {
    case 1:
      monthChar = "Jan";
      break;
    case 2:
      monthChar = "Feb";
      break;
    case 3:
      monthChar = "Mar";
      break;
    case 4:
      monthChar = "Apr";
      break;
    case 5:
      monthChar = "May";
      break;
    case 6:
      monthChar = "Jun";
      break;
    case 7:
      monthChar = "Jul";
      break;
    case 8:
      monthChar = "Aug";
      break;
    case 9:
      monthChar = "Sep";
      break;
    case 10:
      monthChar = "Oct";
      break;
    case 11:
      monthChar = "Nov";
      break;
    case 12:
      monthChar = "Dec";
      break;
    default:
      break;
  }
  return monthChar;
}