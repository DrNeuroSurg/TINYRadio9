#ifndef _STATIONS_H_
#define _STATIONS_H_

#include <Arduino.h>
#include <Array.h>
#include <ArduinoJson.h>

#define MAX_STATIONS 19

typedef struct {
  String ShortName;
  String URL;     
  uint8_t index;
  uint32_t startX;    //COORDINATES FOR GUI (IF NEEDED)
  uint32_t midX;
  uint32_t endX; 
} TinyStation;

//TYPE-DEF
typedef Array<TinyStation, MAX_STATIONS> Stations;

class TinyStations {

  public:
      TinyStations(Stations & array){
  
      array.clear();

      TinyStation station;

      station.ShortName = "COSTA.D.MAR";
      station.URL = "http://radio4.cdm-radio.com:8020/stream-mp3-Chill_autodj";
      station.index = 0;
      array.push_back(station);

      station.ShortName = "kiss.fm";
      station.URL = "http://topradio-stream31.radiohost.de/kissfm_mp3-128";
      station.index = 1;
      array.push_back(station);

      station.ShortName = "baelaric";
      station.URL = "https://radio.balearic-fm.com:8000/radio.mp3";
      station.index = 2;
      array.push_back(station);

      station.ShortName = "lounge.ch";
      station.URL = "http://fr1.streamhosting.ch/lounge128.mp3";
      station.index = 3;
      array.push_back(station);

      station.ShortName = "sun radio";
      station.URL = "http://radio2.vip-radios.fm:8068/stream-128kmp3-CafeSoulside_autodj";
      station.index = 4;
      array.push_back(station);


    }
   void loadFromJson(Stations & array, String json)
   {
      JsonDocument doc;

      DeserializationError error = deserializeJson(doc, json.c_str());

      if (error) {
          LV_LOG_USER("deserializeJson() failed: ");
          LV_LOG_USER(error.c_str());
          return;
      }
      
      array.clear();

      uint8_t idx = 0;
      for (JsonObject station : doc["stations"].as<JsonArray>()) {
          TinyStation _station;

        _station.ShortName = station["shortName"].as<String>();
        _station.URL = station["StreamURL"].as<String>();
        _station.index = idx;
        array.push_back(_station);
        idx++;

      }
   }

};


#endif