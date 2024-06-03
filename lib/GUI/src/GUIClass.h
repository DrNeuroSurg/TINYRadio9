#ifndef _GUI_CLASS_H_
#define _GUI_CLASS_H_
#include <Arduino.h>
#include <lvgl.h>
#include "ArduinoJson.h"
#include "Stations.h"

using namespace std;

class GuiClass {
    public:
        lv_obj_t * _parent;
        uint32_t _colorBackground;
        uint32_t _colorStation;
        uint32_t _colorMarker;
        uint32_t _colorTuning;
        uint32_t _colorVolume;

        String _title = "";
        String _station = "";
        uint8_t _volume;


        virtual void begin(lv_obj_t * parent, String json){

        };
        
        virtual void update(String json){
        };

        //"NOW PLAYING"
        virtual void setStationPlaying(const char * station, bool store){
        };
        virtual void setTitlePlaying(const char * title, bool store){
         };



        //INDICATORS
        virtual void setVolumeIndicatorMax(uint8_t maxValue){
        };
        virtual void setVolumeIndicator(uint8_t value){
        };
        virtual void setStationIndicator(uint32_t station_id){
        };
        
        virtual void updateDRDindicator(bool drdStopped){
        };

        virtual void setVUMeterValue(uint16_t value){
        };

        virtual void tuneToStation(uint8_t index)
        {
        };

        virtual void setStations(TinyStations stations) {
        };

        virtual uint32_t getStationMidX(uint8_t stationIndex) {
            return 0;
        };


        void decodeJSON(String json)
        {
            JsonDocument doc;

            DeserializationError error = deserializeJson(doc, json.c_str());

            if (error) {
                LV_LOG_USER("deserializeJson() failed: ");
                LV_LOG_USER(error.c_str());
                LV_LOG_USER(json.c_str());
                return;
            }

            _colorBackground = colorString2rgb(doc["colorBackground"]);
            _colorStation = colorString2rgb(doc["colorStation"]); // "#ffa500"
            _colorMarker = colorString2rgb(doc["colorMarker"]); // "#e3e700"
            _colorTuning = colorString2rgb(doc["colorTuning"]); // "#e32400"
            _colorVolume = colorString2rgb(doc["colorVolume"]); // "#6aff12"
        }

    private:



    long colorString2rgb(String colorString) {
        long rgb = (int) strtoul( &colorString[1], NULL, 16);
        return rgb;
    }
};

#endif