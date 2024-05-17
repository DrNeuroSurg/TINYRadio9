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
 * @file em11.h
 *
 */

#ifndef _EM11_H
#define _EM11_H

#ifdef __cplusplus
extern "C" {
#endif

    /*********************
     *      INCLUDES
     *********************/
#include "lvgl.h"
#include "../../lvgl/src/core/lv_obj.h"

/*********************
 *      DEFINES
 *********************/

 /**********************
  *      TYPEDEFS
  **********************/

    typedef struct {
        lv_obj_t obj;           // main panel
        lv_obj_t * canvas;
        int32_t _value;              /*Current value of the EM11 (percent)*/
        int32_t _minValue;
        int32_t _maxValue;
        float _factor;
        int32_t _rotation;
        lv_color_t _color;
        lv_color_t _bgrColor;
    } em11_t;

    extern const lv_obj_class_t em11_class;

    /**********************
     * GLOBAL PROTOTYPES
     **********************/

     /**
      * Create a EM11 object
      * @param parent        pointer to parent 
      * @return              pointer to the EM11 object
      */
    lv_obj_t* em11_create(lv_obj_t* parent);
    // void em11_set_height(lv_obj_t* obj, lv_coord_t value);
    // void em11_set_width(lv_obj_t* obj, lv_coord_t value);
    // void em11_set_bg_color(lv_obj_t* obj,lv_color_t value);
    // void em11_set_bg_opa(lv_obj_t * obj, lv_opa_t value) ;

    void em11_set_minMax(lv_obj_t * obj, int32_t min, int32_t max) ;
    void em11_set_value(lv_obj_t * obj, int32_t value);
    void em11_set_rotation(lv_obj_t * obj, int32_t rotation);

    
    /**********************
     *      MACROS
     **********************/


#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*_EM11_H*/
