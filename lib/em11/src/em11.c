/*
MIT License

Copyright (c) 2024 DrNeurosurg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/**
 * @file em11.c
 *
 */

 /*********************
  *      INCLUDES
  *********************/
 #include <Arduino.h>

#include "em11.h"

  /*********************
   *      DEFINES
   *********************/
#define MY_CLASS    &em11_class

   /**********************
    *      TYPEDEFS
    **********************/


/**********************
 *  STATIC PROTOTYPES
 **********************/
static void em11_value_update(lv_obj_t * obj);
static void em11_draw(lv_event_t * e);
static void em11_event(const lv_obj_class_t * class_p, lv_event_t * e);
static void get_center(const lv_obj_t * obj, lv_point_t * center, int32_t * radius);

/**********************
 *  STATIC VARIABLES
 **********************/
const lv_obj_class_t em11_class = {
    .base_class = &lv_obj_class,
    .width_def = LV_PCT(100), // LV_DPI_DEF * 2,
    .height_def = LV_PCT(100),
    .instance_size = sizeof(em11_t),
    .name = "em11",
    .event_cb = em11_event
};

 /**********************
  *   GLOBAL FUNCTIONS
  **********************/

void default_styles()
{
    //lv_theme_t * current_theme = 
}

lv_obj_t * em11_create(lv_obj_t* parent)
{
    LV_LOG_INFO("begin");
    lv_obj_update_layout(parent);

    lv_obj_t * obj = lv_obj_class_create_obj(&em11_class,parent);
    LV_ASSERT_MALLOC(obj);
    if (obj == NULL) return NULL;

    lv_obj_class_init_obj(obj);


    em11_t * _em11 = (em11_t *)obj;
    lv_value_precise_t minSize = _min(lv_obj_get_width(parent), lv_obj_get_height(parent)) ;

    lv_obj_set_size((lv_obj_t *)_em11, minSize - 3, minSize - 3);
    lv_obj_center((lv_obj_t *) _em11);


    //DEFAULTS
    _em11->_rotation = 0;
    _em11->_minValue = 0.0;
    _em11->_maxValue = 32768.0;
    _em11->_factor = (float)(( _em11->_maxValue -  _em11->_minValue) / 100.0);
    _em11->_value = 50;
    _em11->_color = lv_color_hex(0x00EA7B);
    _em11->_bgrColor = lv_color_darken(_em11->_color , 160);
    lv_obj_set_style_bg_opa(obj, LV_OPA_TRANSP, 0);

    return obj;
}

void em11_set_value(lv_obj_t * obj, int32_t value) {
    em11_t* _em11 = (em11_t*)obj;
    _em11->_value = value;
    if(value > _em11->_maxValue) { _em11->_value = _em11->_maxValue; }
    if(value < _em11->_minValue) { _em11->_value = _em11->_minValue; }

    lv_obj_invalidate(obj);

}

void em11_set_minMax(lv_obj_t * obj, int32_t min, int32_t max) {
    em11_t* _em11 = (em11_t*)obj;
    _em11->_minValue = min;
    _em11->_maxValue = max;

    //CALCULATE NEW FACTOR
     _em11->_factor = (float)(( _em11->_maxValue - _em11->_minValue) / 100.0);
     lv_obj_invalidate(obj);
    
}

void em11_set_rotation(lv_obj_t * obj, int32_t rotation) { // -45 .. 45
    em11_t* _em11 = (em11_t*)obj;
    _em11->_rotation = constrain(rotation, -90, 90);
    lv_obj_invalidate(obj);
}

/**
 * Used internally to update arc angles after a value change
 * @param obj pointer to an em11 object
 */
static void em11_value_update(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    em11_t * _em11 = (em11_t *)obj;

}

static void em11_draw(lv_event_t * e) {

    lv_obj_t * obj = lv_event_get_current_target(e);
    em11_t * _em11 = (em11_t *)obj;

    lv_obj_update_layout(obj);

    lv_layer_t * layer = lv_event_get_layer(e);

    lv_value_precise_t valueIn = (lv_value_precise_t) ((1.8 * _em11->_value / _em11->_factor) / 2); //HALF OF VALUE

    lv_value_precise_t value2 = constrain(valueIn, 0, 90);

    //GET CENTER
    lv_point_t center;
    int32_t radius;
    get_center(obj, &center, &radius);

    // #define ROTATION  20

    lv_draw_arc_dsc_t dsc;
    lv_draw_arc_dsc_init(&dsc);

    dsc.center.x = center.x;
    dsc.center.y = center.y;
    dsc.width = (int32_t)(radius * 0.6);
    dsc.radius = radius;

    //BACKGROUND
    dsc.start_angle = 0;
    dsc.end_angle = 360;
    dsc.color = _em11->_bgrColor;
    lv_draw_arc(layer, &dsc);

    lv_value_precise_t delta_start = 0;
    lv_value_precise_t delta_end = 0;

    lv_value_precise_t start;
    lv_value_precise_t end;

    dsc.color = _em11->_color;

    //  //FIRST PART
    start = _em11->_rotation - value2;
    if(start < 0) {
        delta_start = 360 + start; //start1 ngative
        delta_end = 360;
        start = 0; 
        //DRAW DELTA
        dsc.start_angle = delta_start;
        dsc.end_angle = delta_end;
        lv_draw_arc(layer, &dsc);
    }

    end = _em11->_rotation + value2;
    if(end >360){
        delta_start = 0;
        delta_end = end - 360;
        end = 360;
         //DRAW DELTA
        dsc.start_angle = delta_start;
        dsc.end_angle = delta_end;
        lv_draw_arc(layer, &dsc);
    }

    dsc.start_angle = start;
    dsc.end_angle = end;
    
   lv_draw_arc(layer, &dsc);



    //  //SECOND PART (+ 180 DEG)
    start = 180 + _em11->_rotation - value2;
    if(start < 0) {
        delta_start = 360 + start; //start1 ngative
        delta_end = 360;
        start = 180; 
        //DRAW DELTA
        dsc.color = lv_color_hex(0xFF);
        dsc.start_angle = delta_start;
        dsc.end_angle = delta_end;
        lv_draw_arc(layer, &dsc);
    }

    end = 180 + _em11->_rotation + value2;
    if(end >360){
        
        delta_start = 0;
        delta_end =  end - 360;
        end = 360;
         //DRAW DELTA
         dsc.color = lv_color_hex(0xFF);
         dsc.start_angle = delta_start;
         dsc.end_angle = delta_end;
         lv_draw_arc(layer, &dsc);
    }

    dsc.color = _em11->_color;

    dsc.start_angle = start;
    dsc.end_angle = end;

    lv_draw_arc(layer, &dsc);
}


static void get_center(const lv_obj_t * obj, lv_point_t * center, int32_t * radius)
{
    int32_t left_bg = lv_obj_get_style_pad_left(obj, LV_PART_MAIN);
    int32_t right_bg = lv_obj_get_style_pad_right(obj, LV_PART_MAIN);
    int32_t top_bg = lv_obj_get_style_pad_top(obj, LV_PART_MAIN);
    int32_t bottom_bg = lv_obj_get_style_pad_bottom(obj, LV_PART_MAIN);

    int32_t r = (LV_MIN(lv_obj_get_width(obj) - left_bg - right_bg,
                        lv_obj_get_height(obj) - top_bg - bottom_bg)) / 2;

    center->x = obj->coords.x1 + r + left_bg;
    center->y = obj->coords.y1 + r + top_bg;

    if(radius) *radius = r;
}

static void em11_event(const lv_obj_class_t * class_p, lv_event_t * e) {

     LV_UNUSED(class_p);
    lv_result_t res;

    /*Call the ancestor's event handler*/
    res = lv_obj_event_base(MY_CLASS, e);
    if(res != LV_RESULT_OK) return;

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_current_target(e);
    em11_t * _em11 = (em11_t *)obj;

    if(code == LV_EVENT_DRAW_MAIN) {
            em11_draw(e);
    }

}
//#endif /*LV_USE_PANEL*/
