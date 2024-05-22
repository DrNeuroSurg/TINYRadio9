#ifndef _TINY_DEFS_H_
#define _TINY_DEFS_H_




//#define USE_ENCODER


#ifdef USE_ENCODER
//PINS
  #define ENC_VOUME_BUTTON_PIN    10
  #define ENC_VOLUME_A_PIN        11
  #define ENC_VOLUME_B_PIN        12


  #define ENC_TUNE_BUTTON_PIN     13
  #define ENC_TUNE_A_PIN          14
  #define ENC_TUNE_B_PIN          21

#endif

#include <lvgl.h>

#define HOST_NAME   "TinyRadio" //FOR CONNECT VIA BROWSER -> use TinyRadio.local !

  //LVGL
  #define LVGL_TASK_DELAY_MS   10

  #define LVGL_TASK_CORE 1
  #define LVGL_TASK_PRIO 2


  // //PREFERENCES
  #define PREFS_NAME    "TinyRadio"
  #define LAST_STATION  "rec_lastStation"
  #define LAST_VOLUME   "rec_lastVolume"


  //AUDIO
  #define VOLUME_STEPS 40
  #define MAX_STATIONS 19

  // // GUI
  #define RADIUS 2
  #define BORDER_WIDTH 1
  #define POINTER_WIDTH 8  

  #define X_INC  20
  #define Y_INC  27

  #define LABEL_WIDTH 6 * X_INC
  #define MARKER_HEIGHT 6
  #define MARKER_WIDTH X_INC

  #define INDICATOR_MOVE_TIME 2000 // 2 sec. (2000 mS) from left to right

  //BUTTONMATRIX SYMBOLS
  static const char * btnm_map[] = {LV_SYMBOL_VOLUME_MID, LV_SYMBOL_PREV, LV_SYMBOL_NEXT, LV_SYMBOL_VOLUME_MAX ,""};

#endif //_TINY_DEFS_H_