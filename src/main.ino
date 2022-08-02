#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUDP.h>
#include <RF24.h>
#include <SPI.h>
#include <GyverHacks.h>
#include <ESP8266Ping.h>
#include <sendemail.h>
#include <NTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <FS.h>                 //Содержится в пакете. Видео с уроком http://esp8266-arduinoide.ru/step4-fswebserver

//#define DEBUG_GETADMIN
#define LEDPIN                   2
#define LED_ON                   0
#define LED_OFF                  1
#define NTP_REFRESH_TIME         300000
#define DEBUG                    1

byte wakeup_cmd;
bool srv1_state;
bool mypc_state;
byte mac_MYPC[6]={0x00,0x19,0x66,0xC7,0x3F,0x70};
byte mac_SRV1[6]={0x00,0x1D,0x7D,0xD2,0xE7,0x2E};
byte ip_broadcast[4]={192,168,0,255};
byte ip_MYPC[4]={192,168,0,101};
byte ip_SRV1[4]={192,168,0,107};

// Определяем переменные wifi
String _ssid     = "tenda"; // Для хранения SSID
String _password = "89174600028"; // Для хранения пароля сети
String _ssidAP = "WiFi";   // SSID AP точки доступа
String _passwordAP = ""; // пароль точки доступа
String SSDP_Name="FSWebServer"; // Имя SSDP

struct      struct_data{
  byte state;
  int8_t temp;
  int8_t ftemp;
  int8_t bat;
  int8_t fbat;
};

unsigned long timer;
const char* www_username = "admin";
const char* www_password = "453116";
const char* mail_host = "smtp.yandex.ru";
const char* mail_login = "S-tehnikum@yandex.ru";
const char* mail_pass = "453116PooH";
const char* mail_dst = "S-tehnikum@rambler.ru";
String      strNTPtime = "";
byte        rx_addr[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"};
float       nrftemp;
float       bat;
const int   TCPport = 8585;
struct_data recive_data;

IPAddress                   apIP(192, 168, 4, 1);
ESP8266WebServer            HTTP(80);
ESP8266HTTPUpdateServer     httpUpdater;
WiFiUDP                     udp;
RF24                        radio(4, 15); // Set up nRF24L01 radio on SPI bus plus pins 4 for CE and 15 for CSN
NTPClient                   timeClient(udp);
SendEmail                   myemail(mail_host, 465, mail_login, mail_pass, 5000, true);
WiFiClient                  client;
GTimer                      myTimer1(100);
GTimer                      myTimer2(NTP_REFRESH_TIME);
File                        fsUploadFile;

ADC_MODE(ADC_VCC);

void setup() {
pinMode(LEDPIN, OUTPUT);
digitalWrite(LEDPIN, 1);
Serial.begin(115200);
Serial.println("");
//Запускаем файловую систему

Serial.println("Start 1-WIFI");
WIFIinit();
//Настраиваем и запускаем HTTP интерфейс
Serial.println("Start 2-WebServer");
HTTP_init();
HTTPcmd();
//Настраиваем и запускаем SSDP интерфейс
Serial.println("Start 3-SSDP");
Serial.println("Start 4-FS");
httpUpdater.setup(&HTTP);
timeClient.begin();
timeClient.setTimeOffset(18000);
ArduinoOTA.begin();
//nrf_init();
}

void internet_services_upd(){

  if (myTimer2.isReady()){
    myTimer2.reset();
    timeClient.update();
    ping();
    if (ESP.getFreeHeap() < 5000)
    {
      myemail.send(mail_login, mail_dst, "RESET ESP", strNTPtime + "Reason: HEAP overload!");
      ESP.reset();
    }
  }

  if(myTimer1.isReady()){
    myTimer1.reset();
    if(digitalRead(LEDPIN))
      digitalWrite(LEDPIN, LED_ON);
    else
      digitalWrite(LEDPIN, LED_OFF);
  }
}

void sendWOL(const IPAddress ip, const byte mac[]) {

  byte preamble[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  udp.beginPacket(ip, 7);
  udp.write(preamble, 6);
  for (uint8 i = 0; i < 16; i++) {
    udp.write(mac, 6);
  }
  udp.endPacket();
}

void pc_wakeup_func(){

  switch(wakeup_cmd){

      case 1:
        Serial.println("Sending WOL");
        sendWOL(ip_broadcast, mac_MYPC);
      break;

      case 2:
        mypc_state = Ping.ping(ip_MYPC);
        srv1_state = Ping.ping(ip_SRV1);
      break;

      case 3:
        httpget("/?cmd=asleep");
         break;

      case 4:
        httpget("/?cmd=alock");
         break;

      default:break;
  }
    wakeup_cmd=0;
}

void ping(){
  digitalWrite(LEDPIN, LED_ON);
  srv1_state = Ping.ping(ip_SRV1);
  mypc_state = Ping.ping(ip_MYPC);

  static bool email_sended = false;

  if(!srv1_state && !email_sended){
    strNTPtime = timeClient.getFormattedTime();
    email_sended = myemail.send(mail_login, mail_dst, "SRV1 is down", strNTPtime);
  }

  if(srv1_state)
    email_sended = 0;
}

void loop() {
  ArduinoOTA.handle();
  HTTP.handleClient();
  pc_wakeup_func();
  internet_services_upd();
}
