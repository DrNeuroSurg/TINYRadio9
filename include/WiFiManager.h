#pragma once

#include <Arduino.h>
#include "esp_log.h"

#include "defines.h"

#include "tinyDefs.h"


//CALLBACKS

extern __attribute__((weak)) void wifimanager_message(const char*);
extern __attribute__((weak)) void wifimanager_drdStopped(const char*);


bool _drdStopped = false;

bool LOAD_DEFAULT_CONFIG_DATA = false;

ESP_WM_LITE_Configuration defaultConfig;
ESPAsync_WiFiManager_Lite* ESPAsync_WiFiManager;


//TODO:
#if USING_CUSTOMS_STYLE
const char NewCustomsStyle[] PROGMEM =
  "<style>div,input{padding:5px;font-size:1em;}input{width:95%;}body{text-align: center;}"\
  "button{background-color:blue;color:white;line-height:2.4rem;font-size:1.2rem;width:100%;}fieldset{border-radius:0.3rem;margin:0px;}</style>";
#endif


bool wifimanager_begin()
{

  esp_log_level_set("*", ESP_LOG_VERBOSE); 
  const char * TAG = "beginWiFi";

  ESP_LOGI(TAG,"Starting ESPAsync_WiFi");

#if USING_MRD
  Serial.println(ESP_MULTI_RESET_DETECTOR_VERSION);
#else
   ESP_LOGI(TAG,ESP_DOUBLE_RESET_DETECTOR_VERSION);
#endif

  ESPAsync_WiFiManager = new ESPAsync_WiFiManager_Lite();
  String AP_SSID = "TinyRadio";
  String AP_PWD  = "12345678";            //EMPTY PASSWORDS ARE FORBIDDEN :-()

  String msg = "Verbinde mit WLAN \n* ";
  msg +=  ESPAsync_WiFiManager->getWiFiSSID(0);
  msg += " *";  

    if(wifimanager_message) {
        wifimanager_message(msg.c_str());   //CALLBACK
    }

  // Set customized AP SSID and PWD
  ESPAsync_WiFiManager->setConfigPortal(AP_SSID, AP_PWD);

   ESPAsync_WiFiManager->setConfigPortalChannel(0);

    #if USING_CUSTOMS_STYLE
    ESPAsync_WiFiManager->setCustomsStyle(NewCustomsStyle);
    #endif

    #if USING_CUSTOMS_HEAD_ELEMENT
    ESPAsync_WiFiManager->setCustomsHeadElement(PSTR("<style>html{filter: invert(10%);}</style>"));
    #endif

    #if USING_CORS_FEATURE
    ESPAsync_WiFiManager->setCORSHeader(PSTR("Your Access-Control-Allow-Origin"));
    #endif

    // Set customized DHCP HostName
    ESPAsync_WiFiManager->begin(HOST_NAME);  //defined in TinyDefs.h

     if(WiFi.isConnected()) {
      ESP_LOGI(TAG,"CONNECTED TO WiFi...");

      WiFi.setSleep(false);

      return true;

     } else {

      ESP_LOGE(TAG, "WiFI_info: WiFi credentials are not correct");

      msg = "Bitte mit WLAN \n* ";
      msg += HOST_NAME;
      msg += "* \nverbinden !";
     if(wifimanager_message) {
           wifimanager_message(msg.c_str());
      }
    }    

    return false; //SHOULD NEVER HAPPEN
}

void wifimanager_run() // CALL IN "LOOP"
{
   ESPAsync_WiFiManager->run();

  if(!_drdStopped) {

    if(!drd->waitingForDRD())
    {
        //FALSE IF ENDED !
        _drdStopped = true;
        if(wifimanager_drdStopped){
            wifimanager_drdStopped("stopped");  //CALLBACK
            LV_LOG_USER("DRD STOPPED");
        }

    }
  }
}