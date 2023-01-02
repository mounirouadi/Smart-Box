
// CONNECTIONS:
// DS1302 CLK/SCLK --> 5
// DS1302 DAT/IO --> 4
// DS1302 RST/CE --> 2
// DS1302 VCC --> 3.3v - 5v
// DS1302 GND --> GND

#include <ThreeWire.h>  
#include <RtcDS1302.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Fingerprint.h>
#include <Servo.h>



LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
ThreeWire myWire(4,5,6); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);
int id,previous = 0;
bool vall = false;
Servo myservo;

int getFingerprintIDez();
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

struct DateTime{
  char Date[20];
  char Time[20];
};

void setup () 
{
    Serial.begin(9600);
    finger.begin(57600);
    pinMode(7,INPUT);
    myservo.attach(9);
    
    if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
    } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1);
    }
    lcd.init();
    
    lcd.backlight();
    myservo.write(0);
    Rtc.Begin();
    RtcDateTime(__DATE__, __TIME__);
    

    if (!Rtc.GetIsRunning())
    {
        Serial.println("RTC was not actively running, starting now");
        Rtc.SetIsRunning(true);
    }
}

void loop () 
{
    RtcDateTime now = Rtc.GetDateTime();
    DateTime Data = getDateTime(now);
    DateTime BriefData = getDateTimeBrief(now);
    Serial.println(Data.Time);
    lcd.setCursor(0,0);
    lcd.print(BriefData.Date);
    if(digitalRead(7)==0){
    lcd.setCursor(0,0);
    lcd.print(BriefData.Date);
    lcd.setCursor(0,1);
    lcd.print("Put your phone");}
    if(digitalRead(7)==1 and val1 == false){
    lcd.setCursor(0,0);
    lcd.print(BriefData.Date);
    lcd.setCursor(0,1);
    lcd.print("Scan your finger");     

    int tmp = getFingerprintIDez();
    if(tmp>-1){
    id = tmp;
    if(vall == false){
    previous = id;
    vall = true;
    Serial.println("Finger "+String(id));
    lcd.setCursor(0,0);
    lcd.print(BriefData.Date);
    lcd.setCursor(0,1);
    lcd.print("Finger "+String(id));
    delay(5000);
    myservo.write(100);
    lcd.print("Box is reserved.");
  }
  }
   }

  int tmp = getFingerprintIDez();
    if(tmp>-1){
    id = tmp;
  if(id == previous and vall and (digitalRead(7) == 1)){
    vall = false;
    Serial.println("Access confirmed");

    lcd.setCursor(0,0);
    lcd.print(BriefData.Date);
    lcd.setCursor(0,1);
    lcd.print("Access confirmed");
    id = 0;
    delay(2000);
    myservo.write(10);

  }
  if(id != previous and vall and (digitalRead(7) == 1)){
    Serial.println("Wrong Finger "+String(id));

    lcd.setCursor(0,0);
    lcd.print(BriefData.Date);
    lcd.setCursor(0,1);
    lcd.print("Access denied");
    id = 0;
    delay(2000);
  }
}
}






#define countof(a) (sizeof(a) / sizeof(a[0]))

DateTime getDateTime(const RtcDateTime& dt)
{
    DateTime str;

    snprintf_P(str.Date, 
            countof(str.Date),
            PSTR("%02u/%02u/%04u"),
            dt.Month(),
            dt.Day(),
            dt.Year());
     
     snprintf_P(str.Time, 
            countof(str.Time),
            PSTR("%02u:%02u:%02u"),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    return str;
}


DateTime getDateTimeBrief(const RtcDateTime& dt)
{
    DateTime str;

    snprintf_P(str.Date, 
            countof(str.Date),
            PSTR("%02u/%02u %02u:%02u"),
            dt.Month(),
            dt.Day(),
            dt.Hour(),
            dt.Minute());
    return str;
}





uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
  case FINGERPRINT_OK:
  Serial.println("Image taken");
  break;
  case FINGERPRINT_NOFINGER:
  Serial.println("No finger detected");
  return p;
  case FINGERPRINT_PACKETRECIEVEERR:
  Serial.println("Communication error");
  return p;
  case FINGERPRINT_IMAGEFAIL:
  Serial.println("Imaging error");
  return p;
  default:
  Serial.println("Unknown error");
  return p;
  }
  // OK success!
  p = finger.image2Tz();
  switch (p) {
  case FINGERPRINT_OK:
  Serial.println("Image converted");
  break;
  case FINGERPRINT_IMAGEMESS:
  Serial.println("Image too messy");
  return p;
  case FINGERPRINT_PACKETRECIEVEERR:
  Serial.println("Communication error");
  return p;
  case FINGERPRINT_FEATUREFAIL:
  Serial.println("Could not find fingerprint features");
  return p;
  case FINGERPRINT_INVALIDIMAGE:
  Serial.println("Could not find fingerprint features");
  return p;
  default:
  Serial.println("Unknown error");
  return p;
  }
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
  Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
  Serial.println("Communication error");
  return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
  Serial.println("Did not find a match");
  return p;
  } else {
  Serial.println("Unknown error");
  return p;
  }
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
}
// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;
  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  // found a match!
  //Serial.print("Found ID #"); Serial.print(finger.fingerID);
  //Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}
