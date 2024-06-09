/****************************************************************************************************************************
  This is for  WT32_SC01_PLUS boards only !

  Licensed under MIT license

  by DrNeurosurg 2024

  *****************************************************************************************************************************/

#pragma once

#include "GUIClass.h"
#include "tinyDefs.h"
#include "Stations.h"
#include "Retro20_4.h"
#include "Retro15_4.h"
#include "WT32_SC01_PLUS.h"
#include "em11.h"

//CALLBACKS
extern __attribute__((weak)) void gui_tuneToURL(String URL);
extern __attribute__((weak)) void gui_setVolume(uint8_t newVolume);

extern __attribute__((weak)) void gui_volume_up();
extern __attribute__((weak)) void gui_volume_down();
extern __attribute__((weak)) void gui_station_next();
extern __attribute__((weak)) void gui_station_prev();

// static Stations tinyStations;
// TinyStations stations(tinyStations);

LV_FONT_DECLARE(Berlin25_4);
LV_FONT_DECLARE(Berlin10_4);
LV_IMG_DECLARE(VU_METER_AMBER_SMALL);
LV_IMG_DECLARE(NEEDLE_RED);

class RetroGUI: public GuiClass {

  public:

    String last_title = "";
    String last_station = "";

  void update(String json) {
    
    LV_LOG_USER("UPDATE");

    //DELETE ALL CHILDS
    lv_obj_clean(_parent);

    decodeJSON(json);
    createStyles();

    //GRID

    //COLUMNS
    #ifdef USE_VU_METER
         static int32_t col_dsc[] = {85 +2 , LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
 
    #else
        static int32_t col_dsc[] = {54, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    #endif

    //ROWS
    static int32_t row_dsc[] = {54, 190, 20, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};


    static lv_point_precise_t top_line_points[] = { {0,0}, {TFT_HOR_RES, 0}};
    static lv_point_precise_t bottom_line_points[] = { {0,TFT_VER_RES -1}, {TFT_HOR_RES, TFT_VER_RES - 1}};
    static lv_point_precise_t separatorLine_points[] = { {col_dsc[0] + 5, 27}, {TFT_HOR_RES, 27}};


    //****************************************************************************************
    //                                   CREATE PARTS OF GUI                                 *
    //****************************************************************************************

    /*Create a container with grid*/
    
    cont = lv_obj_create(_parent);
    lv_obj_set_grid_dsc_array(cont, col_dsc, row_dsc);
    lv_obj_set_size(cont, TFT_HOR_RES, TFT_VER_RES);
    lv_obj_center(cont);
    lv_obj_add_style(cont, &cont_style, 0);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);

    //TOP_LINE
    lv_obj_t * topLine = lv_line_create(_parent);
    lv_line_set_points(topLine, top_line_points, 2);
    lv_obj_set_style_line_width(topLine, BORDER_WIDTH, 0);
    lv_obj_set_style_line_color(topLine, lv_color_hex(_colorStation), 0);
    lv_obj_set_style_line_opa(topLine, LV_OPA_COVER, 0);

    // #ifndef NO_BUTTONS
    // //SPARATOR_LINE TOP
    // lv_obj_t * separatorLine = lv_line_create(_parent);
    // lv_line_set_points(separatorLine, separatorLine_points, 2);
    // lv_obj_set_style_line_width(separatorLine, BORDER_WIDTH, 0);
    // lv_obj_set_style_line_color(separatorLine, lv_color_hex(_colorStation), 0);
    // lv_obj_set_style_line_opa(separatorLine, LV_OPA_COVER, 0);
    // #endif
    
    //TOP LEFT
    tl = createPart(cont,  true);
    lv_obj_set_grid_cell(tl, LV_GRID_ALIGN_STRETCH, 0, 1,
                            LV_GRID_ALIGN_STRETCH, 0, 1);
    

    //VU-METER
     #ifdef USE_VU_METER
        //RECTANGULAR BORDER
        lv_obj_set_style_radius(tl, 0, 0);
        lv_obj_set_style_border_width(tl, 2, 0);
        create_vuMeter(tl);
     #else
        //ROUND BORDER
        lv_obj_set_style_radius(tl, 25, 0);
        lv_obj_set_style_border_width(tl, 2, 0);
        em11 = em11_create(tl);
        em11_set_minMax(em11, 0, 32768);
        em11_set_rotation(em11, 30);
        em11_set_value(em11, 0);
        em11_set_minMax(em11,0,127);
     #endif


    //TOP - RIGHT
    tr = createPart(cont,  false);
    lv_obj_set_grid_cell(tr, LV_GRID_ALIGN_STRETCH, 1, 1,
                            LV_GRID_ALIGN_STRETCH, 0, 1);   

    // MIDDLE
    mid = createPart(cont,  true);
    lv_obj_set_grid_cell(mid, LV_GRID_ALIGN_STRETCH, 0, 2,
                            LV_GRID_ALIGN_STRETCH, 1, 1);
    //ONLY TOP BORDER
    lv_obj_set_style_border_side(mid, LV_BORDER_SIDE_TOP, 0);

    //STATION-LIST
    createStationList(mid);

    // BOTTOM
    bot_upper = createPart(cont,  true);
    lv_obj_set_grid_cell(bot_upper, LV_GRID_ALIGN_STRETCH, 0, 2,
                            LV_GRID_ALIGN_STRETCH, 2, 1);
    //ONLY TOP & BOTTOM BORDER
    lv_obj_set_style_border_side(bot_upper, LV_BORDER_SIDE_TOP + LV_BORDER_SIDE_BOTTOM, 0);
    
    scale_bot = createVolumeScale(bot_upper, &Berlin10_4, 0, VOLUME_STEPS); 





#ifdef NO_BUTTONS

    bot_lower = createPart(cont,  false);
    lv_obj_set_grid_cell(bot_lower, LV_GRID_ALIGN_STRETCH, 0, 2,
                            LV_GRID_ALIGN_STRETCH, 3, 1);
    //BOTTOM_LINE
    lv_obj_t * bottomLine = lv_line_create(_parent);
    lv_line_set_points(bottomLine, bottom_line_points, 2);
    lv_obj_set_style_line_width(bottomLine, BORDER_WIDTH, 0);
    lv_obj_set_style_line_color(bottomLine, lv_color_hex(_colorStation), 0);
    lv_obj_set_style_line_opa(bottomLine, LV_OPA_COVER, 0);


#else
    //BUTTONS
    buttons = createButtons(_parent);
    lv_obj_set_size(buttons, lv_pct(100), lv_pct(14)); //100% WIDTH OF PARENT, 100% HEIGHT OF PARENT
    
#endif


    //ZEIGER
    station_indicator = createStationIndicator(mid);
    volume_indicator = createVolumeIndicator(bot_upper);

    //TITLE & STATION LABEL
  

#ifdef NO_BUTTONS 
        LV_FONT_DECLARE(Berlin30_4);

        title_playing = lv_label_create(bot_lower);
        lv_obj_add_style(title_playing, &playing_style, 0);
        lv_obj_set_style_text_font(title_playing, &Berlin25_4, 0);
        lv_obj_set_size(title_playing, lv_pct(100), lv_pct(100));

        lv_obj_set_style_text_align(title_playing , LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_clear_flag(title_playing, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_align(title_playing, LV_ALIGN_CENTER, 0, 5);

        station_playing = lv_label_create(tr);
         lv_obj_add_style(station_playing, &playing_style, 0);
        lv_obj_set_style_text_font(station_playing, &Berlin30_4, 0);
        lv_obj_set_size(station_playing, lv_pct(100), lv_pct(100));

        lv_obj_set_style_text_align(station_playing , LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_clear_flag(station_playing, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_align(station_playing, LV_ALIGN_CENTER, 0, 10);

#else
    station_playing = createPlayingLabel(tr, &Retro20_4, true, false);
    title_playing = createPlayingLabel(tr, &Retro20_4, false, false);

#endif

    lv_label_set_long_mode(station_playing, LV_LABEL_LONG_SCROLL_CIRCULAR);  
    lv_label_set_text(station_playing, last_station.c_str());

    lv_label_set_long_mode(title_playing, LV_LABEL_LONG_SCROLL_CIRCULAR);  
    lv_label_set_text(title_playing, last_title.c_str());
  }

  void begin(lv_obj_t * parent, String json){

        LV_LOG_USER("BEGIN");
        
        _parent = parent;
        update(json);
       
    }

   
    void setStations(Stations stations)
    {
        _stations = stations;
    }

    //  STATION
    void setStationPlaying(const char * station, bool store = true){
        if(store) { last_station = station; }
        if(station_playing) {
        lv_label_set_text(station_playing, station);
        }
    }
    
    // TITLE
    void setTitlePlaying(const char * title, bool store = true){
        if(store) { last_title = title;}
        if(title_playing) {
         lv_label_set_text(title_playing, title);
        }
    }

    void setVolumeIndicatorMax(uint8_t maxValue)
    {
        _maxVolume = maxValue;
    }


    //ANIMATION CALLBACK
    static void anim_x_cb(void * var, int32_t v)
    {
        lv_obj_set_x((lv_obj_t *) var, v);
    }

    void setVolumeIndicator(uint8_t value){

        if(scale_bot) {
            uint32_t pw2 = POINTER_WIDTH / 2;
            
            uint32_t _lastVolumePosition = lv_obj_get_x(volume_indicator);
            //CALCULATE NEW POSITION
            int32_t width = lv_obj_get_width(scale_bot); //480.0;
            int32_t step = width / _maxVolume;
            uint32_t _nextVolumePosition = step * value;
            
            if(_nextVolumePosition < 0 ) {_nextVolumePosition = pw2 ;}

            #ifdef USE_ENCODER_VOLUME
                //SET IT DIRECT (W/O ANIMATION)
                lv_obj_set_x(volume_indicator,_nextVolumePosition);
            #else
                //ANIMATION
                float _pixel_time =   INDICATOR_MOVE_TIME / TFT_HOR_RES ; // PIXEL / mSec
                uint32_t _move_distance = abs ((int32_t) (_lastVolumePosition - _nextVolumePosition) );
                uint32_t _move_time =  _pixel_time * _move_distance; //_pixel_time * distance
                lv_anim_t a;
                lv_anim_init(&a);

                lv_anim_set_var(&a, volume_indicator);
                lv_anim_set_duration(&a, _move_time);
                lv_anim_set_values(&a, _lastVolumePosition, _nextVolumePosition);

                /*Set path (curve). Default is linear*/
                lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);

                //set Callback
                lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t) lv_obj_set_x);

                lv_anim_start(&a);
            #endif

        }
    }

    bool inRange(int val, int minimum, int maximum)
    {
         return ((minimum <= val) && (val <= maximum));
    }

    int8_t getStationIndexUnderIndicator(uint32_t indicatorPos) {

            int8_t ret = -1;
            uint8_t _numStations = _stations.size();
            for(uint8_t idx = 0; idx < _numStations; idx++)
            {
                if( inRange(indicatorPos, _stations[idx].startX,_stations[idx].endX ) ) {
                    return idx;
                }
            }
            return ret;
    }

    void setTuneIndicator(uint32_t newPosition) {
        lv_obj_set_x(station_indicator, newPosition - POINTER_WIDTH/2);
    }

    void tuneToStation(uint8_t station_id) {
        uint32_t pw2 = POINTER_WIDTH / 2;
        uint32_t nextStationMid = _stations[station_id].midX - pw2;

        float _pixel_time =   INDICATOR_MOVE_TIME / TFT_HOR_RES ; // PIXEL / mSec
        uint32_t _move_distance = abs ((int32_t) (nextStationMid - _lastStationMid) );
        uint32_t _move_time =  _pixel_time * _move_distance; //_pixel_time * distance
        
       
        //ANIMATION
        lv_anim_t a;
        lv_anim_init(&a);

        lv_anim_set_var(&a, station_indicator);
        lv_anim_set_duration(&a, _move_time);
        lv_anim_set_values(&a, _lastStationMid, nextStationMid);

        /*Set path (curve). Default is linear*/
        lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);

        //set Callback
        lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t) lv_obj_set_x);

        lv_anim_start(&a);

        _lastStationMid = nextStationMid;

    }

    void setVUMeterValue(uint16_t value){   

        //UPPER 8Bit = left, LOWER 8Bit 0 right

        uint8_t _left = value & 0xff;//map_l(value >> 8, 0, 127, 0, 11);
        uint8_t _right = (value >> 8) & 0xff;//map_l(value & 0x00FF, 0, 127, 0, 11);
        uint8_t _mean = (_left + _right) / 2;
        
        #ifdef USE_VU_METER
        //NEEDLE
            // 2700   //SENKRECHT MITTE
            // 2300  // LINKS
            // 3100  // RECHTS
            //0 = 2200, 127 = 3200 , DELTA = 100 , STEP = 1000/127 = 6
            //ANGLE = 2300 + 6 * value
            lv_image_set_rotation(needle, 2200 + 7 * _left);  // VOLLER AUSSCHALAG
            // lv_image_set_rotation(needle, 2300 + 6 * _left);  // REDUZIERTER AUSSCHLAG

        #else
            if(em11) {
            em11_set_value(em11, _left);
            }
        #endif

    };

     void updateDRDindicator(bool drdStopped){
        if(drdStopped) {
            if(tl) {
                lv_obj_set_style_border_width(tl, 0, 0);
            }
        }
     }

     uint32_t getStationMidX(uint8_t stationIndex) {
        return _stations[stationIndex].midX;
     }


  private:

    Stations _stations;

    uint8_t _maxVolume;
    uint8_t _volumePercent; //FOR BAR
    uint8_t _volume;
    uint32_t _lastStationMid = 0;

    uint32_t _pointer_width = 8;
    uint32_t _border_width = 2;
    uint32_t _volume_width;

    lv_obj_t * cont; //CONTAINER
    lv_obj_t * station_indicator; //SKALENZEIGER
    lv_obj_t * volume_indicator; //VOLUME
    lv_obj_t * tl;  //TOP LEFT
    lv_obj_t * tr;  //TOP RIGHT
    lv_obj_t * mid; //MIDDLE
    lv_obj_t * bot_upper; //BOTTOM
    lv_obj_t * bot_lower; //BOTTOM
    lv_obj_t * stationList;
    lv_obj_t * scale_bot;
    lv_obj_t * station_playing;
    lv_obj_t * title_playing;
    lv_obj_t * buttons;
    lv_obj_t * volume_scale;

    lv_obj_t * btn_volUp;
    lv_obj_t * btn_volDown;
    lv_obj_t * btn_tuneNext;
    lv_obj_t * btn_tunePrev;


    //VU-METER
    lv_obj_t * em11;

    lv_obj_t * vuMeter;
    lv_obj_t * needle;

    //STYLES
    lv_style_t cont_style;
    lv_style_t tuning_style;
    lv_style_t volume_style;
    lv_style_t part_style;
    lv_style_t playing_style;
    lv_style_t playing_style_inv;
    lv_style_t button_style;
    lv_style_t scale_style;
    lv_style_t vuMeter_style;


    String dummies[20] = {
      "NÜRNBERG",
      "NDR-WDR",
      "FREDERIKST.",
      "FLENSBURG",
      "MALMÖ",
      "SÜDWESTFUNK",
      "VATIKAN",
      "BRÜSSEL",
      "MÜNSTER",
      "BAYR.-RDFK.",
      "MTE CARLO",
      "LUXEMBURG",
      "SAARBRÜCKEN",
      "MERGENTHEIM",
      "SCHWEIZ",
      "BREMEN",
      "STAVANGER",
      "NORDEN BBC",
      "STRASSBURG",
      "LILLE"
    };


    void createStyles()  {
        //CONTAINER
        lv_style_init(&cont_style);
        lv_style_set_radius(&cont_style, 0);
        lv_style_set_border_color(&cont_style, lv_color_hex(0x0));
        lv_style_set_border_width(&cont_style,0);
        lv_style_set_bg_color(&cont_style, lv_color_hex(_colorBackground));
        lv_style_set_bg_opa(&cont_style, LV_OPA_COVER);
        lv_style_set_pad_all(&cont_style, 2);
        lv_style_set_pad_gap(&cont_style, 3);

        //ZEIGER
        lv_style_init(&tuning_style);
        lv_color_t color1  = lv_color_hex(_colorTuning);
        lv_color_t color2 = lv_color_darken(color1, 128);
        lv_style_set_radius(&tuning_style, 0);
        lv_style_set_bg_opa(&tuning_style, LV_OPA_COVER);
        static lv_grad_dsc_t grad;
        grad.dir = LV_GRAD_DIR_HOR;
        grad.stops_count = 3;
        grad.stops[0].color =color2;
        grad.stops[0].opa = LV_OPA_COVER;
        grad.stops[0].frac = 0;
        grad.stops[1].color =color1;
        grad.stops[1].opa = LV_OPA_COVER;
        grad.stops[1].frac = 100;
        grad.stops[2].color = color2;
        grad.stops[2].opa = LV_OPA_COVER;
        grad.stops[2].frac = 255;
        lv_style_set_bg_grad(&tuning_style, &grad);
        // lv_style_set_bg_color(&style, color1);
        lv_style_set_border_color(&tuning_style, lv_color_black());
        lv_style_set_border_width(&tuning_style, 0);
        lv_style_set_pad_all(&tuning_style, 3);
        lv_style_set_pad_gap(&tuning_style, 0);
        //SHADOW
        lv_style_set_shadow_color(&tuning_style,lv_color_black());
        lv_style_set_shadow_opa(&tuning_style, 64);
        lv_style_set_shadow_ofs_x(&tuning_style, POINTER_WIDTH / 3);
        lv_style_set_shadow_width(&tuning_style, POINTER_WIDTH / 3);

        //VOLUME
        lv_style_init(&volume_style);
        lv_color_t vcolor1  = lv_color_hex(_colorVolume);
        lv_color_t vcolor2 = lv_color_darken(vcolor1, 128);
        lv_style_set_radius(&volume_style, 2);
        lv_style_set_bg_opa(&volume_style, LV_OPA_COVER);
        static lv_grad_dsc_t grad_v;
        grad_v.dir = LV_GRAD_DIR_HOR;
        grad_v.stops_count = 3;
        grad_v.stops[0].color =vcolor2;
        grad_v.stops[0].opa = LV_OPA_COVER;
        grad_v.stops[0].frac = 0;
        grad_v.stops[1].color =vcolor1;
        grad_v.stops[1].opa = LV_OPA_COVER;
        grad_v.stops[1].frac = 100;
        grad_v.stops[2].color = vcolor2;
        grad_v.stops[2].opa = LV_OPA_COVER;
        grad_v.stops[2].frac = 255;
        lv_style_set_bg_grad(&volume_style, &grad_v);
        // lv_style_set_bg_color(&style, color1);
        lv_style_set_border_color(&volume_style, lv_color_black());
        lv_style_set_border_width(&volume_style, 0);
        lv_style_set_pad_all(&volume_style, 3);
        lv_style_set_pad_gap(&volume_style, 0);
        // //SHADOW
        lv_style_set_shadow_color(&volume_style,lv_color_black());
        lv_style_set_shadow_opa(&volume_style, 64);
        lv_style_set_shadow_ofs_x(&volume_style, POINTER_WIDTH / 3);
        lv_style_set_shadow_width(&volume_style, POINTER_WIDTH / 3);


        //MAIN PARTS
        lv_style_init(&part_style);
        lv_style_set_bg_color(&part_style, lv_color_hex(_colorMarker));
        lv_style_set_bg_opa(&part_style, LV_OPA_TRANSP);
        lv_style_set_pad_all(&part_style, 0);
        lv_style_set_border_color(&part_style, lv_color_hex(_colorStation));
        lv_style_set_border_width(&part_style, BORDER_WIDTH);
        lv_style_set_border_opa(&part_style, LV_OPA_COVER);
        lv_style_set_radius(&part_style, RADIUS);



        //NOW PLAYING
        lv_style_init(&playing_style);
        lv_style_set_radius(&playing_style, 8);
        lv_style_set_border_color(&playing_style, lv_color_black());
        lv_style_set_border_width(&playing_style,0);
        lv_style_set_bg_color(&playing_style, lv_color_hex(0x00));
        lv_style_set_bg_opa(&playing_style, LV_OPA_TRANSP);
        lv_style_set_pad_all(&playing_style, 0);
        lv_style_set_text_color(&playing_style, lv_color_hex(_colorStation));
        lv_style_set_align(&playing_style, LV_ALIGN_BOTTOM_MID);
        lv_style_set_text_align(&playing_style, LV_TEXT_ALIGN_CENTER);

        lv_style_init(&playing_style_inv);
        lv_style_set_radius(&playing_style_inv, 0);
        lv_style_set_border_color(&playing_style_inv, lv_color_hex(_colorStation));
        lv_style_set_border_width(&playing_style_inv,0);
        lv_style_set_bg_color(&playing_style_inv, lv_color_hex(_colorStation));
        lv_style_set_bg_opa(&playing_style_inv, LV_OPA_COVER);
        lv_style_set_pad_all(&playing_style_inv, 0);
        lv_style_set_text_color(&playing_style_inv, lv_color_hex(0x00));
        lv_style_set_align(&playing_style_inv, LV_ALIGN_BOTTOM_MID);
        lv_style_set_text_align(&playing_style_inv, LV_TEXT_ALIGN_CENTER);



   //BUTTON
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

        //VOLUME SCALE
        lv_style_init(&scale_style);
        lv_style_set_radius(&scale_style, 0);
        lv_style_set_border_color(&scale_style, lv_color_hex(0x0));
        lv_style_set_border_width(&scale_style,0);
        lv_style_set_bg_color(&scale_style, lv_color_hex(_colorBackground));
        lv_style_set_bg_opa(&scale_style, LV_OPA_TRANSP);
        lv_style_set_pad_all(&scale_style, 0);
        lv_style_set_pad_gap(&scale_style, 0);
        lv_style_set_text_color(&scale_style, lv_color_hex(_colorStation));
        lv_style_set_line_color(&scale_style, lv_color_hex(_colorStation));

        //VU-METER
        lv_style_init(&vuMeter_style);
        lv_style_set_bg_color(&vuMeter_style, lv_color_hex(0x00));
        lv_style_set_bg_opa(&scale_style, LV_OPA_TRANSP);

        #ifdef USE_VU_METER
            //RECTANGLE
            lv_style_set_border_color(&vuMeter_style,lv_color_hex(0x00AD52));
            lv_style_set_radius(&vuMeter_style, 0);
        #else
        //CIRCLE AROUND
            lv_style_set_arc_color(&vuMeter_style, lv_color_hex(0x00AD52));
            lv_style_set_arc_opa(&vuMeter_style, LV_OPA_COVER);
            lv_style_set_line_rounded(&vuMeter_style, false);
            lv_style_set_arc_rounded(&vuMeter_style,false);
            lv_style_set_arc_width(&vuMeter_style, 15);
        #endif
    }

    lv_obj_t * createPart(lv_obj_t * parent, bool border) {

        lv_obj_update_layout(parent);
        lv_obj_t * ret  = lv_obj_create(parent);
        lv_obj_remove_style_all(ret);
        lv_obj_set_size(ret, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
        lv_obj_add_style(ret, &part_style, 0);
        
        if(!border) {
        lv_obj_set_style_border_opa(ret, LV_OPA_TRANSP, 0);
        lv_obj_set_style_radius(ret, 0, 0);
        }

        lv_obj_clear_flag(ret, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_update_layout(ret);
        return ret;
    }

    lv_obj_t * createSingleButton(lv_obj_t * parent, const char * caption) {

            lv_obj_t * btn = lv_button_create(parent);
            lv_obj_remove_style_all(btn);
            lv_obj_add_style(btn, &button_style, 0);

            lv_obj_t * label = lv_label_create(btn);
            lv_label_set_text(label, caption);
            lv_obj_center(label);   

            return btn;
    }

    lv_obj_t * createButtons(lv_obj_t * parent){
        lv_obj_update_layout(parent);
        
        lv_obj_t *  btn_matrix = lv_buttonmatrix_create(parent);
        
        lv_obj_remove_style_all(btn_matrix);
        lv_obj_set_style_border_width(btn_matrix, 0, 0);
        lv_obj_set_style_bg_color(btn_matrix, lv_color_hex(_colorBackground), 0);
        lv_obj_set_style_bg_opa(btn_matrix, LV_OPA_COVER,0); 
        lv_obj_add_style(btn_matrix, &button_style, LV_PART_ITEMS);
        
        lv_obj_set_style_pad_column(btn_matrix, 2, LV_PART_MAIN);
        
        lv_buttonmatrix_set_map(btn_matrix, btnm_map);
      
        lv_obj_align(btn_matrix, LV_ALIGN_BOTTOM_MID, 0, 0); // BOTTOM ON PARENT
        lv_obj_clear_flag(btn_matrix, LV_OBJ_FLAG_SCROLLABLE); //DON'T USE SCROLLBARS
        lv_obj_add_event_cb(btn_matrix, _event_handler_buttonmatrix, LV_EVENT_ALL, NULL); // ADD EVENT_HANDLER

        return btn_matrix;
    }

    lv_obj_t * createPlayingLabel( lv_obj_t * parent, const lv_font_t * font, bool top, bool inverse) {
        
        lv_obj_t * ret = lv_label_create(parent);
        lv_obj_set_style_text_font(ret, font, 0);
        lv_obj_set_size(ret, lv_pct(100), lv_pct(100));
        lv_obj_update_layout(ret);

        if(inverse) {
        lv_obj_add_style(ret, &playing_style_inv, 0);
        lv_obj_set_style_bg_color(parent, lv_color_hex(_colorStation),0);
        lv_obj_set_style_bg_opa(parent, LV_OPA_COVER, 0);
        lv_obj_set_style_border_color(parent, lv_color_hex(0x00), 0);
        } else {
        lv_obj_add_style(ret, &playing_style, 0);
        }
        
        int32_t lh = font->line_height;
        int32_t yPos = lv_obj_get_height(ret)/2 - lh/2;

        if(top) {
            lv_obj_align(ret, LV_ALIGN_BOTTOM_MID, 0, 0);
        } else {
            lv_obj_align(ret, LV_ALIGN_BOTTOM_MID, 0, lh + 3);
        }

        lv_obj_set_style_text_align(ret , LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_clear_flag(ret, LV_OBJ_FLAG_SCROLLABLE);
        
        return ret;
    }

    lv_obj_t * createStationIndicator(lv_obj_t * parent) {
        
        lv_obj_update_layout(parent);

        int32_t w = lv_obj_get_width(parent);
        int32_t h = lv_obj_get_height(parent);

        lv_obj_t * ret  = lv_obj_create(parent);
        lv_obj_clear_flag(ret, LV_OBJ_FLAG_SCROLLABLE);

        lv_obj_add_style(ret, &tuning_style, 0);

        lv_obj_set_style_align(ret,LV_ALIGN_DEFAULT, 0);

        lv_obj_set_pos(ret, 0, 0); 
        lv_obj_set_size(ret, POINTER_WIDTH, lv_pct(100)); //TODO

        return ret;

    }

    lv_obj_t * createVolumeIndicator(lv_obj_t * parent) {
     
        lv_obj_update_layout(parent);

        int32_t w = lv_obj_get_width(parent);
        int32_t h = lv_obj_get_height(parent);

        lv_obj_t * ret  = lv_obj_create(parent);
        lv_obj_clear_flag(ret, LV_OBJ_FLAG_SCROLLABLE);

        lv_obj_add_style(ret, &volume_style, 0);

        lv_obj_set_style_align(ret,LV_ALIGN_DEFAULT, 0);

        lv_obj_set_pos(ret, 0, 0); 
        lv_obj_set_size(ret, POINTER_WIDTH, lv_pct(100)); //TODO

        return ret;

    }

    lv_obj_t * createStation( lv_obj_t * parent, const lv_font_t * font, bool inverse) {
    
        lv_obj_t * ret = lv_label_create(parent);
        lv_obj_set_style_text_font(ret, font, 0);
        lv_obj_update_layout(ret);

        if(inverse) {
        lv_obj_add_style(ret, &playing_style_inv, 0);
        lv_obj_set_style_bg_color(parent, lv_color_hex(_colorStation),0);
        lv_obj_set_style_bg_opa(parent, LV_OPA_COVER, 0);
        lv_obj_set_style_border_color(parent, lv_color_hex(0x00), 0);
        } else {
        lv_obj_add_style(ret, &playing_style, 0);
        }
        
        int32_t lh = font->line_height;
        int32_t yPos = lv_obj_get_height(ret)/2 - lh/2;

        lv_obj_align(ret, LV_ALIGN_BOTTOM_MID, 0, yPos);

        lv_obj_set_style_text_align(ret , LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_clear_flag(ret, LV_OBJ_FLAG_SCROLLABLE);

        return ret;
    }

    
   

    void createStationList( lv_obj_t * parent) {

        uint32_t stationY[28] = {3,2,4,1,5,0,6,3,2,4,1,5,0,6,3,2,4,1,5,0,6,3,2,4,1,5,0,6};

        uint32_t numStations = _stations.size(); //getNumStations();

        if(numStations > MAX_STATIONS ) {numStations = MAX_STATIONS; }

        //CALCULATE DISTRIBUTION

        int dy = numStations;
        int dx = MAX_STATIONS;

        int values[MAX_STATIONS] = {0};

        // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

        int delta = 2 * dy - dx; // Balance the line. Permute it up later.
        for(int x = 0; x < dx; x++) {
            if(delta > 0) {
                values[x] = 1;
                delta -= 2 * dx;
            }
            delta += 2 * dy;
        }

        uint8_t stationCounter = 0;

        for(uint8_t x = 0; x < MAX_STATIONS; x++) {

            lv_obj_t * statLabel = lv_obj_create(parent);
            lv_obj_add_style(statLabel, &playing_style, 0);
            lv_obj_set_size(statLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            
            lv_obj_t * statMarker = lv_obj_create(statLabel); //MARKER
            
            lv_obj_set_size(statMarker, X_INC, MARKER_HEIGHT);
            lv_obj_set_style_bg_color(statMarker, lv_color_hex(_colorStation), 0);
            lv_obj_set_style_bg_opa(statMarker, LV_OPA_COVER, 0);
            lv_obj_set_style_border_width(statMarker, 0, 0);
            lv_obj_set_style_radius(statMarker, 0, 0);

            lv_obj_align(statMarker, LV_ALIGN_LEFT_MID, 0, 0);
            lv_obj_update_layout(statMarker);

            lv_obj_t * statName = createStation(statLabel,  &Retro15_4, false); //TEXT

            if(values[x] == 0) {
                //DUMMY
                lv_label_set_text(statName, dummies[x].c_str());
                lv_obj_set_style_text_decor(statName, LV_TEXT_DECOR_NONE, 0);
            } else {

                //REAL STATION
                TinyStation stat = _stations.at(stationCounter);
                stat.ShortName.toUpperCase();

                lv_label_set_text(statName,  stat.ShortName.c_str());

                stat.startX = x * X_INC;
                stat.endX= x * X_INC + X_INC;
                stat.midX = x * X_INC + X_INC/2;

                _stations[stationCounter] = stat;

                lv_obj_set_style_text_decor(statName, LV_TEXT_DECOR_UNDERLINE, 0);
                lv_obj_set_style_bg_color(statMarker, lv_color_hex(_colorMarker), 0);

                stationCounter ++;
            }


            lv_obj_align(statName, LV_ALIGN_LEFT_MID, X_INC + 2, 0);
            lv_obj_align(statLabel, LV_ALIGN_TOP_LEFT, 0, 0);
            lv_obj_set_pos(statLabel, x * X_INC, stationY[x] * Y_INC + 6);

            lv_obj_update_layout(statLabel);

            lv_obj_update_layout(statLabel);
        } //x
    }

    lv_obj_t * createVolumeScale(lv_obj_t * parent,const lv_font_t * font, int min, int max) {
    

        lv_obj_t * container;

        static lv_style_t span_style;
        lv_style_init(&span_style);

        lv_style_set_border_width(&span_style,0);
        lv_style_set_radius(&span_style, 0);
        lv_style_set_pad_all(&span_style, 0);
        lv_style_set_pad_gap(&span_style, 0);
        lv_style_set_text_font(&span_style, font);
        lv_style_set_text_color(&span_style, lv_color_hex(_colorStation));
        lv_style_set_bg_color(&span_style, lv_color_hex(0x00));
        lv_style_set_bg_opa(&span_style, LV_OPA_TRANSP);

        container = lv_obj_create(parent);
        lv_obj_remove_style_all(container);
        lv_obj_add_style(container, &span_style, 0);
        lv_obj_set_size(container, lv_pct(100), lv_pct(100)); 

        lv_obj_t * label = lv_label_create(container);
        lv_obj_add_style(label, &span_style, 0);
        lv_label_set_text(label, "MIN");
        lv_obj_set_size(label, LV_SIZE_CONTENT, LV_SIZE_CONTENT);  /*Set smaller width to make the lines wrap*/
        lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_align(label, LV_ALIGN_LEFT_MID, 0, 0);
    
        label = lv_label_create(container);
        lv_obj_add_style(label, &span_style, 0);
        lv_label_set_text(label, "LAUTSTÄRKE");
        lv_obj_set_size(label, LV_SIZE_CONTENT, LV_SIZE_CONTENT);  /*Set smaller width to make the lines wrap*/
        lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

        label = lv_label_create(container);
        lv_obj_add_style(label, &span_style, 0);
        lv_label_set_text(label, "MAX");
        lv_obj_set_size(label, LV_SIZE_CONTENT, LV_SIZE_CONTENT);  /*Set smaller width to make the lines wrap*/
        lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_align(label, LV_ALIGN_RIGHT_MID, 0, 0);

        lv_obj_center(container);

        lv_obj_update_layout(container);

        _volume_width = lv_obj_get_width(container);

        return container;

    }

    void create_vuMeter(lv_obj_t * parent) {

        vuMeter = lv_image_create(parent);
        lv_image_set_src(vuMeter, &VU_METER_AMBER_SMALL);
        lv_obj_center(vuMeter);

        needle = lv_image_create(vuMeter);
        uint32_t w2 = lv_obj_get_width(vuMeter) / 2;
        lv_image_set_src(needle, &NEEDLE_RED);
        lv_obj_align(needle, LV_ALIGN_BOTTOM_LEFT, LV_PCT(50), 0);
        lv_image_set_pivot(needle, 0, 1);

        // lv_image_set_rotation(needle2, 2700);  //SENKRECHT MITTE
        lv_image_set_rotation(needle, 2300);  // LINKS
        //lv_image_set_rotation(needle2, 3100);  // RECHTS


    }

    //EVENTHANDLER FOR BUTTON_MATRIX
    static void _event_handler_buttonmatrix(lv_event_t * e) {
        lv_event_code_t code = lv_event_get_code(e);
        lv_obj_t * obj = (lv_obj_t *) lv_event_get_target(e);
        if(code == LV_EVENT_VALUE_CHANGED) {
            uint32_t id = lv_buttonmatrix_get_selected_button(obj);
            //BUTTONMATRIX SYMBOLS
            // {LV_SYMBOL_VOLUME_MID, LV_SYMBOL_PREV, LV_SYMBOL_NEXT, LV_SYMBOL_VOLUME_MAX ,""};
            char * btn = (char *) lv_buttonmatrix_get_button_text(obj,id);

            if(btn == LV_SYMBOL_VOLUME_MID) {
               // LV_LOG_USER("VOLUME--");
                if(gui_volume_down) { gui_volume_down();} //IF CALLBACK -> CALL IT

            } else if (btn == LV_SYMBOL_VOLUME_MAX) {
                 // LV_LOG_USER("VOLUME++");
                  if(gui_volume_up) { gui_volume_up();} //IF CALLBACK -> CALL IT

            } else if (btn == LV_SYMBOL_PREV) {
                  //LV_LOG_USER("STATION PREV");
                  if(gui_station_prev) { gui_station_prev();} 

            } else if (btn == LV_SYMBOL_NEXT) {
                  //LV_LOG_USER("STATION NEXT");
                  if(gui_station_next) { gui_station_next();}
            }
        }
    }

};