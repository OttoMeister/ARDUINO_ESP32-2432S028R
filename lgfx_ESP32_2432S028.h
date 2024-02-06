// https://github.com/lovyan03/LovyanGFX/blob/master/examples/HowToUse/2_user_setting/2_user_setting.ino

class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_ILI9341 _panel_instance;
  lgfx::Bus_SPI _bus_instance;
  lgfx::Light_PWM _light_instance;
  lgfx::Touch_XPT2046 _touch_instance;
public:
  LGFX(void) {
    {                                     // Set up bus control.
      auto cfg = _bus_instance.config();  // get structure for bus configuration.
                                          // SPI bus configuration
      cfg.spi_host = HSPI_HOST;           // select SPI to use (VSPI_HOST or HSPI_HOST)
      cfg.spi_mode = 0;                   // set SPI communication mode (0 ~ 3)
      cfg.freq_write = 40000000;          // SPI clock for transmit (max 80MHz, round 80MHz to integer divisor)
      cfg.freq_read = 16000000;           // SPI clock for receive
      cfg.spi_3wire = false;              // set to true if receive is on MOSI pin
      cfg.use_lock = true;                // set to true if transaction lock is used
      cfg.dma_channel = 1;                // set use DMA channel (1or2,0=disable)(0=DMA not used)
      cfg.pin_sclk = 14;                  // set SPI SCLK pin number SCK
      cfg.pin_mosi = 13;                  // set MOSI pin number of SPI SDI
      cfg.pin_miso = 12;                  // set SPI's MISO pin number (-1 = disable) SDO
      cfg.pin_dc = 2;                     // set SPI D/C pin number (-1 = disable) RS
      // When using the common SPI bus with the SD card, be sure to set MISO without omitting it.
      _bus_instance.config(cfg);               // reflect the set value to the bus.
      _panel_instance.setBus(&_bus_instance);  // set the bus to the panel.
    }
    {                                       // Set the display panel control.
      auto cfg = _panel_instance.config();  // get structure for display panel settings.
      cfg.pin_cs = 15;                      // pin number to which CS is connected (-1 = disable)
      cfg.pin_rst = -1;                     // pin number to which RST is connected (-1 = disable)
      cfg.pin_busy = -1;                    // pin number to which BUSY is connected (-1 = disable)
      cfg.memory_width = 240;               // maximum width supported by driver IC
      cfg.memory_height = 320;              // maximum height supported by the driver IC
      cfg.panel_width = 240;                // actual displayable width
      cfg.panel_height = 320;               // actual displayable height
      cfg.offset_x = 0;                     // amount of panel offset in X direction
      cfg.offset_y = 0;                     // amount of offset in Y direction for the panel
      cfg.offset_rotation = 0;              // offset of rotation direction value 0~7 (4~7 is upside down)
      cfg.dummy_read_pixel = 8;             // number of dummy read bits before pixel read
      cfg.dummy_read_bits = 1;              // dummy read bits before out-of-pixel data read
      cfg.readable = true;                  // set to true if data read is possible
      cfg.invert = false;                   // set to true if the panel is inverted
      cfg.rgb_order = false;                // set to true if panel red and blue are interchangeable ok
      cfg.dlen_16bit = false;               // set to true if panel sends data length in 16bit units
      cfg.bus_shared = false;               // set to true to share bus with SD card
      _panel_instance.config(cfg);
    }
    {   // Set up backlight control. (Delete if not needed)
      auto cfg = _light_instance.config();  // get structure for backlight configuration
      cfg.pin_bl = 21;                      // pin number BL to which the backlight is connected
      cfg.invert = false;                   // true to invert backlight brightness
      cfg.freq = 44100;                     // PWM frequency of the backlight
      cfg.pwm_channel = 7;                  // channel number of PWM to use
      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);  //set backlight to panel.
    }
    {  // Configure touchscreen control settings. (remove if not needed)
      auto cfg = _touch_instance.config();
      cfg.x_min = 300;          // minimum X value (raw value) obtained from the touchscreen
      cfg.x_max = 3900;         // maximum X value from touchscreen (raw value)
      cfg.y_min = 180;          // smallest Y value (raw value) obtained from the touchscreen
      cfg.y_max = 3700;         // maximum Y value from touchscreen (raw value)
      cfg.pin_int = 36;         // pin number where INT is connected, TP IRQ
      cfg.bus_shared = false;   // set to true if using a common bus with the screen
      cfg.offset_rotation = 6;  // adjust if display and touch orientation do not match, set to 0~7
      // For SPI connection
      cfg.spi_host = VSPI_HOST;  // Select SPI to use (HSPI_HOST or VSPI_HOST)
      cfg.freq = 1000000;        // Set SPI clock
      cfg.pin_sclk = 25;         // pin number where SCLK is connected, TP CLK
      cfg.pin_mosi = 32;         // pin number where MOSI is connected, TP DIN
      cfg.pin_miso = 39;         // pin number Vwhere MISO is connected, TP DOUT
      cfg.pin_cs = 33;           // pin number where CS is connected, TP CS
      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);  // set the touch screen to the panel.
    }
    setPanel(&_panel_instance);  // set the panel to be used.
  }
};         
LGFX tft;  // create an instance of the prepared class.
