#pragma once

#include "GUIClass.h"

//CALLBACKS
extern __attribute__((weak)) void gui_tuneToURL(String URL);
extern __attribute__((weak)) void gui_setVolume(uint8_t newVolume);

extern __attribute__((weak)) void gui_volume_up();
extern __attribute__((weak)) void gui_volume_down();
extern __attribute__((weak)) void gui_station_next();
extern __attribute__((weak)) void gui_station_prev();


LV_FONT_DECLARE(Berlin25_4);

class SimpleGUI: public GuiClass {

  public:

    void update(String json) {

        LV_LOG_USER("UPDATE");
        //DELETE ALL CHILDS
        lv_obj_clean(_parent);

        decodeJSON(json);

        lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(_colorBackground), 0);
        lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER,0);     

        //CREATE LABEL FOR STATION_NAME
        _stationLabel = lv_label_create(_parent);
        lv_obj_set_size(_stationLabel, lv_pct(90), LV_SIZE_CONTENT); //90% WIDTH OF PARENT
        lv_obj_align(_stationLabel, LV_ALIGN_CENTER, 0, -50);   //CENTER ON PARENT 
        lv_obj_set_style_text_font(_stationLabel, &Berlin25_4, 0);  //CUSTOM FONT
        lv_obj_set_style_text_color(_stationLabel, lv_color_hex(_colorStation),0); //USE "STATION COLOR"
        lv_label_set_text(_stationLabel, "STATION") ;
        lv_obj_set_style_text_align(_stationLabel, LV_TEXT_ALIGN_CENTER, 0);
        lv_label_set_long_mode(_stationLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);  //SCROLL IF TOO LONG
        lv_obj_clear_flag(_stationLabel, LV_OBJ_FLAG_SCROLLABLE); //DON'T USE SCROLLBARS
        lv_label_set_text(_stationLabel, _station.c_str()) ;

        //CREATE LABEL FOR TITLE
        _titleLabel = lv_label_create(_parent);
        lv_obj_set_size(_titleLabel, lv_pct(90), LV_SIZE_CONTENT); //90% WIDTH OF PARENT
        lv_obj_align(_titleLabel, LV_ALIGN_CENTER, 0, 0); // CENTER
        lv_obj_set_style_text_font(_titleLabel, &Berlin25_4, 0);  //CUSTOM FONT
        lv_obj_set_style_text_color(_titleLabel, lv_color_hex(_colorStation),0); //USE "STATION COLOR"
        lv_label_set_text(_titleLabel, "TITLE") ;
        lv_label_set_long_mode(_titleLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);  //SCROLL IF TOO LONG
        lv_obj_set_style_text_align(_titleLabel, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_clear_flag(_titleLabel, LV_OBJ_FLAG_SCROLLABLE); //DON'T USE SCROLLBARS
        lv_label_set_text(_titleLabel, _title.c_str()) ;

        // VOLuME BAR
        _volumeBar = lv_bar_create(lv_screen_active());
        lv_obj_set_size(_volumeBar, lv_pct(100), 20);
        lv_obj_set_style_bg_opa(_volumeBar, LV_OPA_COVER, LV_PART_INDICATOR);
         lv_obj_set_style_bg_color(_volumeBar, lv_color_hex(_colorVolume) , LV_PART_INDICATOR);
        lv_obj_align(_volumeBar, LV_ALIGN_CENTER, 0, 50);   //CENTER ON PARENT 
        lv_bar_set_value(_volumeBar, _volumePercent, LV_ANIM_OFF);
        
        //TEST W. STYLE
         uint32_t _buttonColor = 0xDEA163;


        lv_style_init(&button_style);
        lv_color_t color1b  = lv_color_hex(_buttonColor);
        lv_color_t color2b = lv_color_darken(color1b, 64);
        lv_style_set_radius(&button_style, 4);
        lv_style_set_bg_opa(&button_style, LV_OPA_COVER);
        
        static lv_grad_dsc_t grad_b;
        grad_b.dir = LV_GRAD_DIR_VER;

        grad_b.stops_count = 3;

        grad_b.stops[0].color = color2b;
        grad_b.stops[0].opa = LV_OPA_COVER;
        grad_b.stops[0].frac = 0;

        grad_b.stops[1].color =color1b;
        grad_b.stops[1].opa = LV_OPA_COVER;
        grad_b.stops[1].frac = 100;

        grad_b.stops[2].color = color2b;
        grad_b.stops[2].opa = LV_OPA_COVER;
        grad_b.stops[2].frac = 255;

        lv_style_set_bg_grad(&button_style, &grad_b);
        lv_style_set_border_color(&button_style, lv_color_black());
        lv_style_set_border_width(&button_style, 0);

        lv_style_set_pad_all(&button_style, 1);
        lv_style_set_pad_gap(&button_style, 1);

        // BUTTON MATRIX
        static const char * btnm_map[] = {LV_SYMBOL_VOLUME_MID, LV_SYMBOL_PREV, LV_SYMBOL_NEXT, LV_SYMBOL_VOLUME_MAX ,""}; // 4 BUTTONS

        _buttonMatrix = lv_buttonmatrix_create(_parent);
        lv_obj_remove_style_all(_buttonMatrix);
        lv_obj_set_style_border_width(_buttonMatrix, 0, 0);
        lv_obj_set_style_bg_color(_buttonMatrix, lv_color_hex(_colorBackground), 0);
        lv_obj_set_style_bg_opa(_buttonMatrix, LV_OPA_COVER,0); 
        lv_obj_add_style(_buttonMatrix, &button_style, LV_PART_ITEMS);
        
        lv_obj_set_style_pad_column(_buttonMatrix, 2, LV_PART_MAIN);
        
        lv_buttonmatrix_set_map(_buttonMatrix, btnm_map);
        lv_obj_set_size(_buttonMatrix, lv_pct(100), lv_pct(20)); //100% WIDTH OF PARENT, 30% HEIGHT OF PARENT
        lv_obj_align(_buttonMatrix, LV_ALIGN_BOTTOM_MID, 0, 0); // BOTTOM ON PARENT
        lv_obj_clear_flag(_buttonMatrix, LV_OBJ_FLAG_SCROLLABLE); //DON'T USE SCROLLBARS
        lv_obj_add_event_cb(_buttonMatrix, _event_handler_buttonmatrix, LV_EVENT_ALL, NULL); // ADD EVENT_HANDLER
    }

   void begin(lv_obj_t * parent, String json){

        LV_LOG_USER("BEGIN");
        //SAVE
        _parent = parent;
        
        update(json);
        
    }



    //  STATION
    void setStationPlaying(const char * station){
        _station = station;
        if(_stationLabel) {
        lv_label_set_text(_stationLabel, station);
        }
    }
    
    // TITLE
    void setTitlePlaying(const char * title){
        _title = title;
        if(_titleLabel) {
         lv_label_set_text(_titleLabel, title);
        }
    }

    void setVolumeIndicatorMax(uint8_t maxValue)
    {
        _maxVolume = maxValue;
    }
    void setVolumeIndicator(uint8_t value){

        _volumePercent = (uint8_t) (100/_maxVolume * value);
        lv_bar_set_value(_volumeBar, _volumePercent, LV_ANIM_OFF);
    }

  private:
    lv_obj_t * _titleLabel;
    lv_obj_t * _stationLabel;
    lv_obj_t * _volumeBar;
    lv_obj_t * _buttonMatrix;
    lv_style_t button_style;
    uint8_t _maxVolume;
    uint8_t _volumePercent; //FOR BAR
    uint8_t _volume;

    //EVENTHANDLER FOR BUTTON_MATRIX
    static void _event_handler_buttonmatrix(lv_event_t * e) {
        lv_event_code_t code = lv_event_get_code(e);
        lv_obj_t * obj = (lv_obj_t *) lv_event_get_target(e);
        if(code == LV_EVENT_VALUE_CHANGED) {
            uint32_t id = lv_buttonmatrix_get_selected_button(obj);
            switch (id) {

            case 0:   //LEFTMOST
                Serial.println("VOL --");
                if(gui_volume_down) { gui_volume_down();} //IF CALLBACK -> CALL IT
                break;

            case 1:
                Serial.println("PREV");
                if(gui_station_prev) { gui_station_prev();} //IF CALLBACK -> CALL IT
                break;

            case 2:
                Serial.println("NEXT");
                if(gui_station_next) { gui_station_next();} //IF CALLBACK -> CALL IT
                break;

            case 3:   //RIGHTMOST
                Serial.println("VOL ++");
                if(gui_volume_up) { gui_volume_up();} //IF CALLBACK -> CALL IT
                break;

            default:
                break;

            }
        }
    }

};