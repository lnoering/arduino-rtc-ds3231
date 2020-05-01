#include "Arduino.h"
#include <LiquidCrystal.h>
#include <Wire.h> // must be included here so that Arduino library object file references work
#include <RtcDS3231.h>

RtcDS3231<TwoWire> rtc(Wire);

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

void printDateTime(const RtcDateTime& dt,uint8_t line)
{
    lcd.setCursor(0,line);
    char display[20];
    sprintf(display,
            "%02u/%02u/%02u %02u:%02u:%02u",
            dt.Day(),
            dt.Month(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    lcd.print(display);
}


void setup() {
    lcd.begin(20, 4);
	Wire.begin();
    rtc.Begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    // printDateTime(compiled,0);

    if (!rtc.IsDateTimeValid()) 
    {
        if (rtc.LastError() != 0)
        {
            lcd.setCursor(0,0);
            lcd.println("RTC error =        ");
            char display[20];
            sprintf(display,"%s",rtc.LastError());
            lcd.print(display);
        }
        else
        {
          rtc.SetDateTime(compiled);
        }
    }
  
    if (!rtc.GetIsRunning())
    {
        rtc.SetIsRunning(true);
    }

    RtcDateTime now = rtc.GetDateTime();
    if (now < compiled) 
    {
        rtc.SetDateTime(compiled);
    }

    rtc.Enable32kHzPin(false);
    rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone); 
}

void loop() {
    
    if (!rtc.IsDateTimeValid()) 
    {
        if (rtc.LastError() != 0)
        {
            lcd.setCursor(0,0);
            lcd.println("RTC error =        ");
            char display[20];
            sprintf(display,"%s",rtc.LastError());
            lcd.print(display);
        }
        else
        {
            lcd.setCursor(0,0);
            lcd.print("Battery / Power off");
        }
    }
    
    RtcDateTime now = rtc.GetDateTime();
    printDateTime(now,0);

	RtcTemperature temp = rtc.GetTemperature();
    lcd.setCursor(0,1);
    char display[20],
         strTemp[6];
         
    //sprintf(str, "String value: %d.%02d", (int)f, (int)(f*100)%100);
    dtostrf(temp.AsFloatDegC(), 4, 2, strTemp);
    sprintf(display,"Temp = %sC",strTemp);
    lcd.print(display);

	delay(1000);
}