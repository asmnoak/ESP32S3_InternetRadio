//**********************************************************************************************************
//*    ESP32S3_inet_radio_srv -- Internet radio with control function by web server
//*
//**********************************************************************************************************
//**********************************************************************************************************
//*    audioI2S -- I2S audiodecoder for ESP32,  refer to https://github.com/schreibfaul1/ESP32-audioI2S                                                            *
//**********************************************************************************************************
//
// first release on 11/2018 for ESP32
// Version 2  , Aug.05/2019
// Version 3  , Aug.26/2024 for XIAO ESP32S3
//
//
#include "Arduino.h"
#include "WiFiMulti.h"
#include "WebServer.h"
#include "Audio.h"
#include <Adafruit_GFX.h>       // install using lib tool
#include <Adafruit_SSD1306.h>   // install using lib tool
#include "Wire.h"
#include <esp_sntp.h>           // esp lib
#include <TimeLib.h>            // https://github.com/PaulStoffregen/Time
#include <Preferences.h>        // For permanent data

#define MAXVOL    50          // inet max volume  <= 50 , step is 5
#define LED_BULTIN       21   // XIAO esp32s3, low level on     
#define I2S_DOUT      3       //####esp32:25
#define I2S_BCLK      7       //####esp32:26
#define I2S_LRC       9       //####esp32:22
#define SLEEP         1       // sleep timer control button pin
#define PIN_SDA 5             // i2c
#define PIN_SCL 6             // i2c
#define VOL_PIN1     2        // volume up
#define VOL_PIN2     4        // volume down
#define INT_PIN      8        // station change
#define OLED_I2C_ADDRESS 0x3C // Check the I2C bus of your OLED device
#define SCREEN_WIDTH 128      // OLED display width, in pixels
#define SCREEN_HEIGHT 64      // OLED display height, in pixels
#define OLED_RESET -1         // Reset pin # (or -1 if sharing Arduino reset pin)

Audio audio;
WiFiMulti wifiMulti;
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// WiFi
String ssid =     "xxxxxxxx";      // WiFi 1
String password = "pppppppp";
String ssid2 =     "sssssss2";      // WiFi 2, uncomment wifiMulti.addAP() below
String password2 = "ppppppp2";      // if you want to specify it
// time
struct tm *tm;
int d_mon ;
int d_mday ;
int d_hour ;
int d_min ;
int d_sec ;
int d_wday ;
int last_d_sec = 99;
int last_d_min = 99;
static const char *weekStr[7] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"}; //3文字の英字
const char* ntpServer = "ntp.nict.jp";
const long  gmtOffset_sec = 32400;

//
int volume = 35; // inet volume
int station = 0; // inet station url
int ct2,pt2,event;  // interval time 
bool sleepmode = false;
int s_remin = 99;
// mode
int inet_radio = 1;  // 1: Internet radio on
bool led_onoff = true;
int loop_cnt = 0;

// Preset internet station url and name
const char* station_url[]={
  "http://cast1.torontocast.com:2170/stream",  "http://cast1.torontocast.com:2120/stream",
  "http://jenny.torontocast.com:8062/stream",  "http://51.195.203.179:8002/stream",
  "http://216.235.89.171:80/hitlist",  "string 6"
};
const char* station_name[]={
  "JPopSakura",  "JPop hits",
  "J1GOLD",  "HotHits UK",
  "POWERHIT40",  "string 6"
}; // max 10 char
char stnurl[128];  // current internet station url
char stnname[24];  // current internet station name
int max_station = 5; // valid entry
// web server
WebServer server(80);  // port 80(default)
// Operation by server
int stoken = 0;  // server token, count up 
int s_srv = 1;
int s = 1;
int a_srv = 1;
int b_srv = 1;
char titlebuf[166];

String msg = "none";
bool p_on = false;

Preferences preferences; // Permanent data

void handleRoot(void)
{
    String html;
    String val1;
    String val2;
    String val3;
    String val4;
    String val5;
    String val6;
    String val7;
    String val8;
    String html_p1; 
    char htstr[180];
    char tno[4];
    char stnmsg[12];
    bool responsed = false;
    stnmsg[0]=0;

    Serial.println("web received");
    if (server.method() == HTTP_POST) { // submitted with string
      val2 = server.arg("vup");
      val3 = server.arg("vdown");
      val4 = server.arg("stnup");
      val5 = server.arg("stndown");
      val6 = server.arg("sleep");
      val7 = server.arg("stoken");
      val8 = server.arg("stnset");
      if (val7.length() != 0) { // server token
        Serial.print("stoken:");
        String s_stoken = server.arg("stoken");
        int t_stoken = s_stoken.toInt();
        Serial.println(s_stoken);
        msg = "stoken:" + s_stoken;
        if (stoken > t_stoken) {
          Serial.println("redirect");
          msg = "Post converted to Get";
          responsed = true;
          server.send(307, "text/html", "<!DOCTYPE html><html><head><meta charset=\"utf-8\"><meta http-equiv=\"refresh\" content=\"0;url=/\"></head></html>");
        }
      }
      if (!responsed){
        if (val8.length() != 0) {
          Serial.println("station set");
          if (val8 == "set") {
            Serial.print("Set_staion: ");
            // Get input station setting
            String s_stnurl = server.arg("stnurl");
            String s_stnname = server.arg("stnname");
            String s_stnno = server.arg("stnno");
            Serial.print(s_stnno); Serial.print(" "); Serial.print(s_stnname); Serial.print(" ");
            Serial.println(s_stnurl);
            // Set current
            strcpy(stnurl,s_stnurl.c_str());
            strcpy(stnname,s_stnname.c_str());
            station = s_stnno.toInt() - 1;
            if (station < max_station && station >= 0) {
              bool conn_ok;
              conn_ok=audio.connecttohost(s_stnurl.c_str()); // change station
              if (conn_ok) {
                  Serial.println("Set new staion OK");
                  msg = "Set new staion OK";
              } else {
                  Serial.println("Set new staion failed");
                  strcpy(stnmsg,"Err!");
                  msg = "Set new staion error";
              }
            } else msg = "illegal station number";
          } else if (val8 == "save") {
            String s_stnurl = server.arg("stnurl");
            String s_stnname = server.arg("stnname");
            String s_stnno = server.arg("stnno");
            int stnno = s_stnno.toInt();
            Serial.print(s_stnno); Serial.print(" "); Serial.print(s_stnname); Serial.print(" ");
            Serial.println(s_stnurl);
            if (stnno <= max_station && station >= 0) {
              char tstr[166];
              sprintf(tstr,"%s%d","st", stnno);
              preferences.putString(tstr,stnurl);
              sprintf(tstr,"%s%d","nm", stnno);
              preferences.putString(tstr,stnname);
              msg = "new station saved";
            } else msg = "illegal station number";
          }
          else msg = "ignore it";
        }
        else if (val2.length() != 0) {
          Serial.println("vup");
          b_srv=0; 
          msg = "control vup";
        }
        else if (val3.length() != 0) {
          Serial.println("vdown");
          a_srv=0; 
          msg = "control vdown";
        }
        else if (val4.length() != 0) {
          Serial.println("stnup");
          station_setting();
          msg = "control stnup";
        }
        else if (val5.length() != 0) {
          Serial.println("stndown");
          station_setting();
          msg = "control stndown";
        }
        else if (val6.length() != 0) {
          Serial.println("sleep");
          sleep_setting();
          msg = "control sleep";
        }
        else {
          // nop
        }
      }
    }
    if (!responsed) {
      html = "<!DOCTYPE html><html><head><meta charset=\"utf-8\"><title>Inet Radio</title>";
      html +="</head><body><form action=\"\" method=\"post\">";
      html += "<p><h1>Internet Radio</h1></p>";
      html_p1 = String(stnname);
      html += "<p>Now playing: " + html_p1 + " (" + stnurl +  ") "  + "</p>";
      html_p1 = String(titlebuf);
      html += "<p>" + html_p1 + "</p>";
      html += "<p>Volume: ";
      sprintf(tno,"%d",volume);
      html += tno;
      html += "</p>";
      html += "<p>response: " + msg + "</p>";
      html += "<p>Station List: </p>";
      html += "<p><ol>";
      for (int j=0; j < max_station; j++) {
        html += "<li>";
        sprintf(htstr, "%.10s%s%s", station_name[j],  " - ",  station_url[j]);
        html +=  htstr;
        html += "</li>";
      }
      html += "</ol></p>";
      html += "<style>.lay_i input:first-of-type{margin-right: 20px;}</style>";
      html += "<style>.btn {width: 300px; padding: 10px; box-sizing: border-box; border: 1px solid #68779a; background: #cbe8fa; cursor: pointer;}</style>";
      html += "<style>.btn_y {width: 300px; padding: 10px; box-sizing: border-box; border: 1px solid #68779a; background: #ffff8a; cursor: pointer;}</style>";
      //html += "<script>function stokenupd() {const hdtoken = document.getElementById('stoken'); hdtoken.value += 1;}</script>";
      html += "<p>Control Functions</p>";
      html += "<p><div class=\"lay_i\"><input type=\"submit\" name=\"vup\"  value=\"volume up\" class=\"btn\"><input type=\"submit\" name=\"vdown\" value=\"volume down\" class=\"btn\"></div></p>";
      html += "<p><div class=\"lay_i\"><input type=\"submit\" name=\"stnup\"  value=\"station change\" class=\"btn\"></div></p>";
      html += "<p><div class=\"lay_i\"><input type=\"submit\" name=\"sleep\"  value=\"sleep\" class=\"btn_y\"></div></p>";
      html += "<p></p><p>Preference Settings</p>";
      html += "<table style=\”border:none;\”><tr>";
      html += "<td>Station URL:</td>";
      if (strlen(stnmsg)==0) {
        html += "<td></td>";
      } else {
        html += "<td><font color=\"#ff4500\">Err!</font></td>";
      }
      html += "<td><input type=\"text\" name=\"stnno\" size=\"3\" maxlength=\"2\" value=\"";
      html += station + 1;
      html += "\"></td><td>";
      html += "<td><input type=\"text\" name=\"stnname\" size=\"18\" maxlength=\"23\" value=\"";
      html += stnname;
      html += "\"></td><td>";
      html += "<input type=\"text\" name=\"stnurl\" size=\"64\" maxlength=\"127\" value=\"";
      html += stnurl;
      html += "\">";
      html += "<button type=\"submit\" name=\"stnset\" value=\"set\">TEST SET</button>";
      html += "<button type=\"submit\" name=\"stnset\" value=\"save\">SAVE</button>";
      html += "</td></tr></table>";
      html += "<input type=\"hidden\" name=\"stoken\" value=\"";
      stoken += 1;
      html += stoken;
      html += "\">"; 
      html += "</form></p></body>";
      html += "</html>";
      server.send(200, "text/html", html);
    }
    Serial.println("web send response");
}
void handleNotFound(void)
{
  server.send(404, "text/plain", "Not Found.");
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(115200);
  delay(50);
    pinMode(VOL_PIN1, INPUT_PULLUP);
    pinMode(VOL_PIN2, INPUT_PULLUP);
    pinMode(INT_PIN, INPUT_PULLUP);
    pinMode(SLEEP, INPUT_PULLUP); // sleep after about 60 min when button was pressed
    attachInterrupt(INT_PIN,station_setting, CHANGE); // to change station
    attachInterrupt(VOL_PIN1,volup_setting, CHANGE); // to change volue up
    attachInterrupt(VOL_PIN2,voldown_setting, CHANGE); // to change volume down
    attachInterrupt(SLEEP,sleep_setting, CHANGE); // to change sleep on/off
  Wire.setPins(PIN_SDA, PIN_SCL);  
  Wire.begin(); //
    oled.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS);
    oled.clearDisplay();
    oled.setTextColor(SSD1306_WHITE);
  // restore saved settings
    preferences.begin("inet_p", false);
    for (int i = 0; i < max_station; i++){
       char tstr[166];
       String *tcp;
       sprintf(tstr,"%s%d","st",i+1);
       String val1 = preferences.getString(tstr,"");       
       if (val1 != "") {
         tcp = new String(val1.c_str());
         Serial.print(tcp->c_str()); Serial.print(",");        
         station_url[i] = tcp->c_str();
         sprintf(tstr,"%s%d","nm",i+1);
         val1 = preferences.getString(tstr,"");
         tcp = new String(val1.c_str());
         Serial.println(tcp->c_str());
         station_name[i] = tcp->c_str();
       }
    }
    volume = preferences.getInt("vol", -1);
    if (volume < 0) volume = 35;
    station = preferences.getInt("stn", -1);
    if (station < 0) station = 0;

  event = 0;
  pt2=millis();
  p_on = true;
      oled.setTextSize(2); // Draw 2X-scale text
      oled.setCursor(0, 0);
      oled.print("Inet Radio");
      oled.display();
    WiFi.mode(WIFI_STA);
    wifiMulti.addAP(ssid.c_str(), password.c_str());  
    //wifiMulti.addAP(ssid2.c_str(), password2.c_str());  // uncomment if you want second wifi access point
    wifiMulti.run();   // It may be connected to strong one
    while (true) {
      if(WiFi.status() == WL_CONNECTED){ break; }  // WiFi connect OK then next step
      Serial.println("WiFi Err");
      oled.setCursor(0, 15);
      oled.print("WiFi Err");
      oled.display();
      WiFi.disconnect(true);
      delay(5000);
      wifiMulti.run();
      delay(1000*300);  // Wait for Wifi ready
    }
      oled.setCursor(0, 15);
      oled.print("WiFi OK");
      oled.display();
  // time 
    wifisyncjst(); // refer time and day
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(volume); 
    strcpy(stnurl,station_url[station]); // use preset
    strcpy(stnname,station_name[station]);
    bool conn_ok = false;
    WiFi.setTxPower(WIFI_POWER_17dBm); // TX power strong   #### xiao < 17dbm
    Serial.println("TX power 17dBm");
    conn_ok = audio.connecttohost(stnurl); 
    if (!conn_ok) { // conect failure
      Serial.println("Fail to connect");

    } else { // connect ok
      
      inet_radio = 1;  //1: Internet radio
    }
    server.on("/", handleRoot);
    server.onNotFound(handleNotFound);
    server.begin();
    Serial.print("IP = ");
    Serial.println(WiFi.localIP());
    IPAddress ipadr = WiFi.localIP();
      oled.setCursor(0, 30);
      oled.printf("IP:%d.%d", ipadr[2],ipadr[3]);
      oled.display();
    titlebuf[0] = 0;
    WiFi.setTxPower(WIFI_POWER_17dBm); // TX power 
    digitalWrite(LED_BUILTIN, LOW); // led on
    delay(200);

}
void wifisyncjst() {
  // get jst from NTP server
  int lcnt = 0;
  configTzTime("JST-9", "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");
  delay(500);
  // get sync time
  while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET) {
    delay(500);
    lcnt++;
    if (lcnt > 100) {
      Serial.println("time not sync within 50 sec");
      break;
    }
  }
}
void loop()
{
  // check web server req
  server.handleClient();
  //
  if (inet_radio==1) { // process server request
    if (a_srv==0 || b_srv==0) { // by server operation
      updatevolume(a_srv, b_srv);
      a_srv=1;
      b_srv=1;
    }
    if (s==0 || s_srv==0) { // button or by server operation
      sleepmode = !sleepmode;
      if (sleepmode) {
        Serial.println("Sleep timer ON");
        s_remin = 60;
      } else {
        Serial.println("Sleep timer OFF");
        s_remin = 60;
      }
      s=1;
      s_srv=1;
    }
  }
  if (inet_radio==1) {
    loop_cnt ++;
    if (loop_cnt > 500) { // about 3 sec in normal process
      loop_cnt = 0;
      led_onoff = !led_onoff;
      if (led_onoff) {
        digitalWrite(LED_BUILTIN, LOW); // on
        oled_display(1); // inet info
      }
      else {
        digitalWrite(LED_BUILTIN, HIGH); // off
        oled_display(0); // time
      }
    }
    audio.loop(); // Inter net radio function call
  }
  if ((event==1) && (inet_radio==1)) { // interrupt of change station request ?
      char tstr[166];
      event = 0; // clear 
      audio.connecttohost(station_url[station]); // change station.
      // save change
      sprintf(tstr,"%s%d","st", station + 1);
      preferences.putString(tstr,stnurl);
      sprintf(tstr,"%s%d","nm", station + 1);
      preferences.putString(tstr,stnname);
      preferences.putInt("stn",station);
  }
  if (inet_radio==0) delay(300); // if inet radio off , delay ok here
}
// display
void oled_display(int dmode){
  char ts[80];
  char buf[64];
  char buf1[64];
  char slp[8];
  oled.clearDisplay();
  oled.setTextSize(2); // Draw 2X-scale text
  if (dmode==0) { // time
    // display current time
    time_t t = time(NULL);
    tm = localtime(&t);
    d_mon  = tm->tm_mon+1;
    d_mday = tm->tm_mday;
    d_hour = tm->tm_hour;
    d_min  = tm->tm_min;
    d_sec  = tm->tm_sec;
    d_wday = tm->tm_wday;
    //Serial.print("time ");
    if (last_d_sec != d_sec) { // inet info
      sprintf(ts, "%02d-%02d %s", d_mon, d_mday, weekStr[d_wday]);
      oled.setTextSize(2); // Draw 2X-scale text
      oled.setCursor(0, 0);
      oled.print(ts);
      //Serial.println(ts);
      sprintf(ts,"%02d:%02d:%02d",d_hour,d_min,d_sec);
      oled.setCursor(0, 15);
      oled.print(ts);
      //Serial.println(ts);
      last_d_sec = d_sec; // 
    }
  } else {
    if (sleepmode) {
      sprintf(buf1,"SLP%02d V:%02d", s_remin, volume);
    } else {
      sprintf(buf1,"NET Vol:%02d", volume);
    }
    sprintf(buf,"%.10s",stnname);
    oled.setCursor(0, 0);
    oled.print(buf1);
    oled.setCursor(0, 15);
    oled.print(buf);
  }
  oled.setCursor(0, 35);
  oled.setTextSize(1); // Draw 1X-scale text
  oled.print(titlebuf); // title info
  oled.display();
  if (sleepmode) {
    if (last_d_min != d_min) {
      s_remin --;
      if (s_remin==0) {  // fall asleep
        oled.clearDisplay();  // put off
        oled.display(); 
        digitalWrite(LED_BUILTIN, HIGH); // put off
        esp_deep_sleep_start();  // sleep forever
      }
      last_d_min = d_min;
    }
  }
}
// change volume
void updatevolume(uint8_t a, uint8_t b)
{
  if (b==0) { // push vol up
      volume += 5;
      if (volume > MAXVOL) volume = MAXVOL;
  } else if (a==0) { // push vol down
      volume -= 5;      
      if (volume < 0) volume = 0;
  }
  Serial.print("vl:"); 
  Serial.println(volume);
  audio.setVolume(volume);
  preferences.putInt("vol", volume);
}
// sleep int routine
void sleep_setting(){
  ct2=millis();
  if ((ct2-pt2)>250) {
    s = 0;  // sleep on
  }
  pt2=ct2;
}
// volume int routine
void volup_setting(){
  ct2=millis();
  if ((ct2-pt2)>250) {
    updatevolume(1, 0);
  }
  pt2=ct2;
}
void voldown_setting(){
  ct2=millis();
  if ((ct2-pt2)>250) {
    updatevolume(0, 1);
  }
  pt2=ct2;
}
// station change int routine
void station_setting(){
  ct2=millis();
  //delay(10);  // no effect here
  if ((ct2-pt2)>250) {
    Serial.print("st:");
    Serial.println(station);
    station = station + 1;  
    if (station >= max_station) station=0;
    event=1; 
    strcpy(stnurl,station_url[station]); // use preset
    strcpy(stnname,station_name[station]);
  }
  pt2=ct2;
} 
// optional
void audio_info(const char *info){
    Serial.print("info        "); Serial.println(info);
}
void audio_id3data(const char *info){  //id3 metadata
    Serial.print("id3data     ");Serial.println(info);
}
void audio_eof_mp3(const char *info){  //end of file
    Serial.print("eof_mp3     ");Serial.println(info);
}
void audio_showstation(const char *info){
    Serial.print("station     ");Serial.println(info);
}
void audio_showstreaminfo(const char *info){
    Serial.print("streaminfo  ");Serial.println(info);
}
void audio_showstreamtitle(const char *info){
    sprintf(titlebuf,"%.128s",info);  // server data
    oled_display(1); // display inet info
}
void audio_bitrate(const char *info){
    Serial.print("bitrate     ");Serial.println(info);
}
void audio_commercial(const char *info){  //duration in sec
    Serial.print("commercial  ");Serial.println(info);
}
void audio_icyurl(const char *info){  //homepage
    Serial.print("icyurl      ");Serial.println(info);
}
void audio_lasthost(const char *info){  //stream URL played
    Serial.print("lasthost    ");Serial.println(info);
}
void audio_eof_speech(const char *info){
    Serial.print("eof_speech  ");Serial.println(info);
}