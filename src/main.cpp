/****************************************************************************************************************************
 for  WT32_SC01_PLUS boards only !

  Licensed under MIT license
  *****************************************************************************************************************************/


#include <Arduino.h>


#define LOG_LOCAL_LEVEL  ESP_LOG_VERBOSE // this overrides CONFIG_LOG_MAXIMUM_LEVEL setting in menuconfig
                                        // and must be defined before including esp_log.h
#define MY_GLOBAL_DEBUG_LEVEL  ESP_LOG_VERBOSE
#include "esp_log.h"

#include <ESPmDNS.h>
#include <ESPAsyncWebServer.h>
AsyncWebServer server(80); 

#include "index.h"

#include "WiFiManager.h"

//SOME DEFINES
#include "tinyDefs.h"

//LVGL
#define WT_USE_LVGL
#include "WT32_SC01_PLUS.h"

SemaphoreHandle_t lvgl_mux;

//IF NO GUI SELECTED
#ifndef USE_GUI_RETRO || USE_GUI_SIMPLE
  #define USE_GUI_SIMPLE
#endif

//GUI
#ifdef USE_GUI_RETRO
  #include "RetroGUI.h"
  RetroGUI GUI;
#endif

#ifdef USE_GUI_SIMPLE
  #include "SimpleGUI.h"
  SimpleGUI GUI;
#endif

//STATIONS
#include "Stations.h"
static Stations tinyStations;
TinyStations stations(tinyStations);

//AUDIO
#include "Audio.h"     // see my repository at github "ESP32-audioI2S"
#include "audiotask.h"


#include <Preferences.h>
Preferences preferences;

#include <ArduinoJson.h>

JsonDocument  doc;
String jsonString;

String defaultJson = F("{'colorBackground':'#000000', colorStation':'#ffa500','colorMarker':'#e3e700','colorTuning':'#e32400','colorVolume':'#6aff12','stations':[{'shortName':'COSTA.D.MAR','StreamURL':'http://radio4.cdm-radio.com:8020/stream-mp3-Chill_autodj'},{'shortName':'Kiss.FM','StreamURL':'http://topradio-stream31.radiohost.de/kissfm_mp3-128'},{'shortName':'sun radio','StreamURL':'http://radio2.vip-radios.fm:8068/stream-128kmp3-CafeSoulside_autodj'}]}");

uint8_t _lastVolume = 5;
uint8_t _lastStation = 0;

uint8_t _maxVolume = 21;
uint8_t _num_stations = 1;

//FOR vTask_delay
uint32_t ms;

lv_obj_t * tempLabel;
LV_FONT_DECLARE(Berlin25_4);


// ****************** PREFERENCES (FOR LAST STATION PLAYED && LAST VOLUME *******************************************
void savePrefs() {
   preferences.begin(PREFS_NAME, false);
   preferences.putLong(LAST_VOLUME, _lastVolume);
   preferences.putLong(LAST_STATION, _lastStation);
  preferences.end();
}

void loadPrefs() {
   preferences.begin(PREFS_NAME, false);
  _lastVolume = preferences.getLong(LAST_VOLUME, 3);
  _lastStation= preferences.getLong(LAST_STATION, 0);
  preferences.end();
}

// ****************** UI SETTINGS (COLORS, STATIONS, ..) *******************************************

String loadJSON() {
   File file = LittleFS.open("/Settings.json", "r");
  if (file){
    String ret = file.readString();
    if(ret != "") {
      //GOT IT !
      return ret;
    }
  }
   return defaultJson; //IF NOT FOUND
}

String loadSettings()     //ONLY FOR WEBSERVER-CONFIG
{
  File file = LittleFS.open("/Settings.json", "r");
  if (file){
    String ret = file.readString();
    if(ret != "") {
      String payload = "var jsondata = ";
      payload+= ret;
      file.close();
      //return index_PayLoad; //ONLY FOR TESTING
      return payload;
    }
  }
  Serial.println("NO FILE FOUND - USING DEFAULTS..");
  return index_PayLoad;
}

bool saveSettings(String payload) {
  //UPDATE GUI

    stations.loadFromJson(tinyStations, payload);
    _num_stations = tinyStations.size();

    if(_lastStation > _num_stations -1) { _lastStation = _num_stations -1;}

    //UPDATE GUI
    xSemaphoreTakeRecursive(lvgl_mux, portMAX_DELAY);
    GUI.setStations(tinyStations);
    GUI.update(payload);
    GUI.tuneToStation(_lastStation);
    GUI.setVolumeIndicator(_lastVolume);
    audioConnecttohost(tinyStations[_lastStation].URL.c_str());
    xSemaphoreGiveRecursive(lvgl_mux);

  //SAVE IT
  File file = LittleFS.open("/Settings.json", "w");
  if (file)
      { 
        size_t bytesWritten = file.write((const uint8_t*)payload.c_str(), (size_t)payload.length());
        if(bytesWritten == payload.length()) {
          file.close();
          return true;
        }
      }
      Serial.println("NO FILE !!");
  return false;
}

// ****************** CALLBACKS FROM SETTINGS-WEBSERVER *******************************************

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void gotSettings(AsyncWebServerRequest *request) {

  int params = request->params();
  for (int i = 0; i < params; i++) {
    AsyncWebParameter* p = request->getParam(i);
    if(p->name() == "Settings") {
      saveSettings(p->value());
    }
  }
  request->redirect("/");
}

void onRoot(AsyncWebServerRequest *request) {

	  Serial.println("Web Server: New request received: => /");  // for debugging 

    String html = index_partOne ;
    html += loadSettings();
    html += index_partTwo;
    request->send(200, "text/html", html); 
}

// ****************** LVGL  TASK ***********************************
TaskHandle_t Task_lvgl;

void lvglTask(void *parameter) {
  //LVGL
  while(true)
  {
     xSemaphoreTakeRecursive(lvgl_mux, portMAX_DELAY);
      uint32_t ms = lv_timer_handler();
    xSemaphoreGiveRecursive(lvgl_mux);
    vTaskDelay(pdMS_TO_TICKS(ms ));
  }
}

void createLVGL_Task() {
  xTaskCreatePinnedToCore(
        lvglTask,              /* Function to implement the task */
        "lvglTask",            /* Name of the task */
        10000,                   /* Stack size in words */
        NULL,                   /* Task input parameter */
        LVGL_TASK_PRIO,         /* Priority of the task */
        &Task_lvgl,                 /* Task handle. */
        LVGL_TASK_CORE          /* Core where the task should run */
    );
}

void lvglTaskDelete(){
    vTaskDelete(Task_lvgl);
}

void setup()
{
  // Debug console
   static const char *TAG = "setup";

    Serial.begin(115200);
    delay(3000);                  //NEEDE BECAUSE USB-C
    Serial.setDebugOutput(true);
    Serial.flush();

    esp_log_level_set("*", ESP_LOG_VERBOSE); 

    //LVGL
    lvgl_mux = xSemaphoreCreateRecursiveMutex();
    init_display();
    lv_tick_set_cb(xTaskGetTickCount); //LVGL TICK

  // WE NEED A TEMPORARY MESSAGE (IF SOMETHING IS TO KNOW)
     tempLabel = lv_label_create(lv_scr_act());
    lv_obj_set_size(tempLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT); //90% WIDTH OF PARENT
    lv_obj_align(tempLabel, LV_ALIGN_CENTER, 0, 0);   //CENTER ON PARENT 
    lv_obj_set_style_text_font(tempLabel, &Berlin25_4, 0);  //CUSTOM FONT
    lv_obj_set_style_text_align(tempLabel, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_clear_flag(tempLabel, LV_OBJ_FLAG_SCROLLABLE); //DON'T USE SCROLLBARS
    String ss = "Starting .."; // ESPAsync_WiFiManager->getWiFiSSID(0);
    lv_label_set_text(tempLabel, ss.c_str()) ;
    //SHOW IT
    ms = lv_timer_handler();
    vTaskDelay(pdMS_TO_TICKS(ms));


// //**************************************************************************
// //                               WiFi-MANAGER_LITE
// //**************************************************************************

// bool _isWifi = wifimanager_begin();

if(wifimanager_begin())  // WE ARE CONNECTED
{
    ESP_LOGI(TAG,"CONNECTED TO WiFi...");

    if(!MDNS.begin(HOST_NAME)) {
        ESP_LOGE(TAG,"Error starting mDNS");      
      }
    else {
        MDNS.addService("http", "tcp", 80);
        ESP_LOGI(TAG,"mDNS name: %s" ,HOST_NAME);
        
    }
      //SET CALLBACKS FOR CONFIG PORTAL & START IT
    server.on("/", HTTP_GET, onRoot);
    server.on("/save", HTTP_POST, gotSettings);
    server.onNotFound(notFound);
    server.begin();

    //WE DONN'T NEED IT ANYMORE
    if(tempLabel) { lv_obj_delete(tempLabel);}

    //SCREEN BLACK
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x0), 0);
    lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER,0);     
    ms = lv_timer_handler();
    vTaskDelay(pdMS_TO_TICKS(ms));

    // START LVGL TASK
    createLVGL_Task();

    //LAST_VOLUME, LAST_STATION
    loadPrefs();
    // LAST_VOLUME
    if(_lastVolume > _maxVolume) {_lastVolume = _maxVolume;}
    if(_lastVolume < 0) {_lastVolume = 0;}
      //LAST STATION
    if(_lastStation < 0) { _lastStation = 0;}
    if(_lastStation > _num_stations) { _lastStation = _num_stations - 1; }

    savePrefs(); // TO BE SURE ...

    // STATIONS, COLORS ..
    String jsonSettings = loadJSON();
    stations.loadFromJson(tinyStations, jsonSettings);
    _num_stations = tinyStations.size();

    // PUSH TO GUI
    xSemaphoreTakeRecursive(lvgl_mux, portMAX_DELAY);
    GUI.setStations(tinyStations);
    GUI.begin(lv_scr_act(), jsonSettings);
    xSemaphoreGiveRecursive(lvgl_mux);

    //**************************************************************************
    //                             AUDIO
    //**************************************************************************

    //LET AUDIO DO ITS WORK - TASK ON CORE 1
    audioInit();

    _maxVolume = audioGetMaxVolume();
    GUI.setVolumeIndicatorMax(_maxVolume);



    audioSetVolume(_lastVolume);

    xSemaphoreTakeRecursive(lvgl_mux, portMAX_DELAY);
    GUI.setVolumeIndicator(_lastVolume);
    xSemaphoreGiveRecursive(lvgl_mux);


    
    xSemaphoreTakeRecursive(lvgl_mux, portMAX_DELAY);
    GUI.tuneToStation(_lastStation);
    xSemaphoreGiveRecursive(lvgl_mux);
    audioConnecttohost(tinyStations[_lastStation].URL.c_str());

  }

}

void loop()
{
  wifimanager_run();
  
  uint16_t vum =0;
  
  if(audioIsRunning){
      vum = audioGetVUlevel();
    }

  //UPDATE VU-METER (IF ANY)
  xSemaphoreTakeRecursive(lvgl_mux, portMAX_DELAY);
  GUI.setVUMeterValue(vum);
  xSemaphoreGiveRecursive(lvgl_mux);

}

// ************** WIFI_MANAGER CALLBACKS *************************************
void wifimanager_message(const char *info) {

    if(tempLabel){
      xSemaphoreTakeRecursive(lvgl_mux, portMAX_DELAY); 
      lv_label_set_text(tempLabel, info) ;
      ms = lv_timer_handler();
      xSemaphoreGiveRecursive(lvgl_mux);
      vTaskDelay(pdMS_TO_TICKS(ms));
    }
}

void wifimanager_drdStopped(const char* info){

    xSemaphoreTakeRecursive(lvgl_mux, portMAX_DELAY);
    GUI.updateDRDindicator(true);
    xSemaphoreGiveRecursive(lvgl_mux);
}

// ************** AUDIO CALLBACKS *************************************

void audio_showstation(const char* info) {
  xSemaphoreTakeRecursive(lvgl_mux, portMAX_DELAY);
  GUI.setStationPlaying(info);
  xSemaphoreGiveRecursive(lvgl_mux);
}

void audio_showstreamtitle(const char* info) {
  xSemaphoreTakeRecursive(lvgl_mux, portMAX_DELAY);
  GUI.setTitlePlaying(info);
  xSemaphoreGiveRecursive(lvgl_mux);
}

//******************* GUI CALLBACKS **********************************
// void gui_tuneToURL(String URL){

// }

// void gui_setVolume(uint8_t newVolume){

// }

void gui_volume_up(){
  if(_lastVolume < _maxVolume){
    _lastVolume++;
    audioSetVolume(_lastVolume);
    xSemaphoreTakeRecursive(lvgl_mux, portMAX_DELAY);
    GUI.setVolumeIndicator(_lastVolume);
    xSemaphoreGiveRecursive(lvgl_mux);
    savePrefs();
  }
}

void gui_volume_down(){
  if(_lastVolume > 0){
    _lastVolume--;
    audioSetVolume(_lastVolume);
    xSemaphoreTakeRecursive(lvgl_mux, portMAX_DELAY);
    GUI.setVolumeIndicator(_lastVolume);
    xSemaphoreGiveRecursive(lvgl_mux);

    savePrefs();
  }
}


void gui_station_next(){
  if(_lastStation < _num_stations -1 ) {
    _lastStation++;
    xSemaphoreTakeRecursive(lvgl_mux, portMAX_DELAY);
    GUI.tuneToStation(_lastStation);
    xSemaphoreGiveRecursive(lvgl_mux);

     audioConnecttohost(tinyStations[_lastStation].URL.c_str());
    savePrefs();
  }
}

void gui_station_prev(){
  if(_lastStation > 0 ) {
    _lastStation--;
    
    xSemaphoreTakeRecursive(lvgl_mux, portMAX_DELAY);
    GUI.tuneToStation(_lastStation);
    xSemaphoreGiveRecursive(lvgl_mux);

     audioConnecttohost(tinyStations[_lastStation].URL.c_str());
    savePrefs();
  }
}