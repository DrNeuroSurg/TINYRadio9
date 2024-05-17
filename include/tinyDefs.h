#ifndef _TINY_DEFS_H_
#define _TINY_DEFS_H_


//GUI SELECTION
//*** IMPORTANT : ONLY ONE GUI ALLOWED

//UNCOMMENT WHAT YOU WOULD HAVE
#define USE_GUI_RETRO
//#define USE_GUI_SIMPLE

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


  //BUTTONMATRIX SYMBOLS
  static const char * btnm_map[] = {LV_SYMBOL_VOLUME_MID, LV_SYMBOL_PREV, LV_SYMBOL_NEXT, LV_SYMBOL_VOLUME_MAX ,""};

#endif //_TINY_DEFS_H_