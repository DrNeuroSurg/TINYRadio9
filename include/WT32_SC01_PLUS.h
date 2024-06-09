
/* ***********************************************

by DrNeurosurg 2024

Arduino IDE Version: 2.3.2

Libraries used (Arduino):

LovyanGFX by lovyan03
Version 1.1.12

lvgl by kisvegabor
Version 9.1.0


Board: ESP32S3 Dev Module

Board Settings:

USB CDC On Boot: "Enabled"
CPU Frequency: "240MHz (WiFi)"
Core Debug Level: "None"
USB DFU On Boot: "Disabled"
Erase All Flash Before Sketch Upload: "Disabled"
Events Run On: "Core 1"
Flash Mode: "QIO 80MHz"
Flash Size: "8MB (64Mb)"
JTAG Adapter: "Disabled"
Arduino Runs On: "Core 1"
USB Firmware MSC On Boot: "Disabled"
Partition Scheme: "8M with spiffs (3MB APP/1.5MB SPIFFS)"
PSRAM: "QSPI PSRAM"
Upload Mode: "UARTO / Hardware CDC"
Upload Speed: "921600"
USB Mode: "Hardware CDC and JTAG"

*********************************************** */

#pragma once

#include <Arduino.h>
#include <LovyanGFX.hpp>

#ifdef WT_USE_LVGL
  #include <lvgl.h>
  #include "esp_heap_caps.h"
#endif  // WT_USE_LVGL


//#define SCR 30
#define LGFX_USE_V1

/* Change to your screen resolution */

// default: LANDSCAPE
#ifdef TFT_PORTRAIT
    #define TFT_HOR_RES 320
    #define TFT_VER_RES 480
#else
    #define TFT_HOR_RES 480
    #define TFT_VER_RES 320
#endif


/* I2S Pins */
#define WT_I2S_BCK 36
#define WT_I2S_WS  35
#define WT_I2S_DATA 37

#ifdef WT_USE_SDCARD
  /* SD-Card Pins */
  #define WT_SD_CS_PIN 41
  #define WT_SD_MOSI_PIN 40 //MOSI
  #define WT_SD_CLK_PIN  39
  #define WT_SD_MISO_PIN 38 //MISO

  #include <SPI.h>
  #include <SD.h>
    SPIClass hspi = SPIClass(HSPI);
    #define SD_SPI_FREQ 1000000
#endif


class LGFX : public lgfx::LGFX_Device
{

  lgfx::Panel_ST7796 _panel_instance;

  lgfx::Bus_Parallel8 _bus_instance;

  lgfx::Light_PWM _light_instance;

  lgfx::Touch_FT5x06 _touch_instance;

public:
  LGFX(void)
  {
    {
      auto cfg = _bus_instance.config();

      cfg.port = 0;
      cfg.freq_write = 40000000;
      cfg.pin_wr = 47; // pin number connecting WR
      cfg.pin_rd = -1; // pin number connecting RD
      cfg.pin_rs = 0;  // Pin number connecting RS(D/C)
      cfg.pin_d0 = 9;  // pin number connecting D0
      cfg.pin_d1 = 46; // pin number connecting D1
      cfg.pin_d2 = 3;  // pin number connecting D2
      cfg.pin_d3 = 8;  // pin number connecting D3
      cfg.pin_d4 = 18; // pin number connecting D4
      cfg.pin_d5 = 17; // pin number connecting D5
      cfg.pin_d6 = 16; // pin number connecting D6
      cfg.pin_d7 = 15; // pin number connecting D7

      _bus_instance.config(cfg);              // Apply the settings to the bus.
      _panel_instance.setBus(&_bus_instance); // Sets the bus to the panel.
    }

    {                                      // Set display panel control.
      auto cfg = _panel_instance.config(); // Get the structure for display panel settings.

      cfg.pin_cs = -1;   // Pin number to which CS is connected (-1 = disable)
      cfg.pin_rst = 4;   // pin number where RST is connected (-1 = disable)
      cfg.pin_busy = -1; // pin number to which BUSY is connected (-1 = disable)

      // * The following setting values ​​are set to general default values ​​for each panel, and the pin number (-1 = disable) to which BUSY is connected, so please try commenting out any unknown items.

      cfg.memory_width = 320;  // Maximum width supported by driver IC
      cfg.memory_height = 480; // Maximum height supported by driver IC
      cfg.panel_width = 320;   // actual displayable width
      cfg.panel_height = 480;  // actual displayable height
      cfg.offset_x = 0;        // Panel offset in X direction
      cfg.offset_y = 0;        // Panel offset in Y direction
#ifdef TFT_PORTRAIT
      cfg.offset_rotation = 2;
#else
      cfg.offset_rotation = 3;
#endif
      cfg.dummy_read_pixel = 8;
      cfg.dummy_read_bits = 1;
      cfg.readable = true;
      cfg.invert = true;
      cfg.rgb_order = false;
      cfg.dlen_16bit = false;
      cfg.bus_shared = true;

      _panel_instance.config(cfg);
    }

    {                                      // Set backlight control. (delete if not necessary)
      auto cfg = _light_instance.config(); // Get the structure for backlight configuration.

      cfg.pin_bl = 45;     // pin number to which the backlight is connected
      cfg.invert = false;  // true to invert backlight brightness
      cfg.freq = 44100;    // backlight PWM frequency
      cfg.pwm_channel = 0; // PWM channel number to use

      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance); // Sets the backlight to the panel.
    }

    { // Configure settings for touch screen control. (delete if not necessary)
      auto cfg = _touch_instance.config();

      cfg.x_min = 0;   // Minimum X value (raw value) obtained from the touchscreen
      cfg.x_max = 319; // Maximum X value (raw value) obtained from the touchscreen
      cfg.y_min = 0;   // Minimum Y value obtained from touchscreen (raw value)
      cfg.y_max = 479; // Maximum Y value (raw value) obtained from the touchscreen
      cfg.pin_int = 7; // pin number to which INT is connected
      cfg.bus_shared = false;
      cfg.offset_rotation = 0;

      // For I2C connection
      cfg.i2c_port = 0;    // Select I2C to use (0 or 1)
      cfg.i2c_addr = 0x38; // I2C device address number
      cfg.pin_sda = 6;     // pin number where SDA is connected
      cfg.pin_scl = 5;     // pin number to which SCL is connected
      cfg.freq = 400000;   // set I2C clock

      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance); // Set the touchscreen to the panel.
    }

    setPanel(&_panel_instance); // Sets the panel to use.
  }
};


LGFX tft; //"the"" TFT Panel


#ifdef WT_USE_LVGL
// /*Read the touchpad*/
void  touchPadRead(lv_indev_t * indev, lv_indev_data_t * data){

  uint16_t touchX, touchY;

  bool touched = tft.getTouch(&touchX, &touchY);

  if (!touched)
  {
    data->state = LV_INDEV_STATE_REL;
  }
  else
  {
    data->state = LV_INDEV_STATE_PR;

    /*Set the coordinates*/
    data->point.x = touchX;
    data->point.y = touchY;
  }
}


const unsigned int lvBufferSize = TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8);
// uint8_t lvBuffer[lvBufferSize];
// uint8_t lvBuffer2[lvBufferSize];
uint8_t *lvBuffer = (uint8_t *)heap_caps_malloc(lvBufferSize, MALLOC_CAP_INTERNAL | MALLOC_CAP_DMA);
uint8_t *lvBuffer2 = (uint8_t *)heap_caps_malloc(lvBufferSize, MALLOC_CAP_INTERNAL | MALLOC_CAP_DMA);



#endif // WT_USE_LVGL


void init_display() {

    //LOVYAN_GFX
    tft.init();

    tft.initDMA();
    tft.startWrite();

      
    #ifdef WT_USE_LVGL
    // **************** LVGL9 **********************************
    lv_init();

    static lv_display_t *display = lv_display_create(TFT_HOR_RES, TFT_VER_RES);
    lv_display_set_color_format(display, LV_COLOR_FORMAT_RGB565);

    lv_display_set_flush_cb(display, [](lv_display_t* display, const lv_area_t* area, unsigned char* data) {
        if (tft.getStartCount() == 0)
        {   // Processing if not yet started
             tft.startWrite();
        }
            uint32_t w = lv_area_get_width(area);
            uint32_t h = lv_area_get_height(area);
            lv_draw_sw_rgb565_swap(data, w*h);
            tft.pushImageDMA(area->x1, area->y1, w, h, (uint16_t*)data);  //DMA 
            lv_display_flush_ready(display);
        });


    //double buffering
    lv_display_set_buffers(display, lvBuffer, lvBuffer2, lvBufferSize, LV_DISPLAY_RENDER_MODE_PARTIAL);

    //create & register touchpad
    lv_indev_t * touchDriver = lv_indev_create();
    lv_indev_set_type(touchDriver, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(touchDriver , touchPadRead);
    lv_indev_enable(touchDriver, true);
    #endif // WT_USE_LVGL

}

