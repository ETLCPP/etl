
#ifndef ETL_EMBEDDED_TEMPLATE_LIBRARY_INCLUDED
#define ETL_EMBEDDED_TEMPLATE_LIBRARY_INCLUDED

#if defined(TEENSYDUINO) 

    #if defined(__AVR_ATmega32U4__)
        #define ARDUINO_BOARD "Teensy 2.0"
    #elif defined(__AVR_AT90USB1286__)       
        #define ARDUINO_BOARD "Teensy++ 2.0"
    #elif defined(__MK20DX128__)       
        #define ARDUINO_BOARD "Teensy 3.0"
    #elif defined(__MK20DX256__)       
        #define ARDUINO_BOARD "Teensy 3.2" // and Teensy 3.1
    #elif defined(__MKL26Z64__)       
        #define ARDUINO_BOARD "Teensy LC"
    #elif defined(__MK64FX512__)
        #define ARDUINO_BOARD "Teensy 3.5"
    #elif defined(__MK66FX1M0__)
        #define ARDUINO_BOARD "Teensy 3.6"
    #else
       #define ARDUINO_BOARD "Unknown"
    #endif

#else // --------------- Arduino ------------------

    #if defined(ARDUINO_AVR_ADK)       
        #define ARDUINO_BOARD "Mega Adk"
    #elif defined(ARDUINO_AVR_BT)
        #define ARDUINO_BOARD "Bt"
    #elif defined(ARDUINO_AVR_DUEMILANOVE)       
        #define ARDUINO_BOARD "Duemilanove"
    #elif defined(ARDUINO_AVR_ESPLORA)       
        #define ARDUINO_BOARD "Esplora"
    #elif defined(ARDUINO_AVR_ETHERNET)       
        #define ARDUINO_BOARD "Ethernet"
    #elif defined(ARDUINO_AVR_FIO)       
        #define ARDUINO_BOARD "Fio"
    #elif defined(ARDUINO_AVR_GEMMA)
        #define ARDUINO_BOARD "Gemma"
    #elif defined(ARDUINO_AVR_LEONARDO)       
        #define ARDUINO_BOARD "Leonardo"
    #elif defined(ARDUINO_AVR_LILYPAD)
        #define ARDUINO_BOARD "Lilypad"
    #elif defined(ARDUINO_AVR_LILYPAD_USB)
        #define ARDUINO_BOARD "Lilypad Usb"
    #elif defined(ARDUINO_AVR_MEGA)       
        #define ARDUINO_BOARD "Mega"
    #elif defined(ARDUINO_AVR_MEGA2560)       
        #define ARDUINO_BOARD "Mega 2560"
    #elif defined(ARDUINO_AVR_MICRO)       
        #define ARDUINO_BOARD "Micro"
    #elif defined(ARDUINO_AVR_MINI)       
        #define ARDUINO_BOARD "Mini"
    #elif defined(ARDUINO_AVR_NANO)       
        #define ARDUINO_BOARD "Nano"
    #elif defined(ARDUINO_AVR_NG)       
        #define ARDUINO_BOARD "NG"
    #elif defined(ARDUINO_AVR_PRO)       
        #define ARDUINO_BOARD "Pro"
    #elif defined(ARDUINO_AVR_ROBOT_CONTROL)       
        #define ARDUINO_BOARD "Robot Ctrl"
    #elif defined(ARDUINO_AVR_ROBOT_MOTOR)       
        #define ARDUINO_BOARD "Robot Motor"
    #elif defined(ARDUINO_AVR_UNO)       
        #define ARDUINO_BOARD "Uno"
    #elif defined(ARDUINO_AVR_YUN)       
        #define ARDUINO_BOARD "Yun"
    #elif defined(ARDUINO_SAM_DUE)       
        #define ARDUINO_BOARD "Due"
    #elif defined(ARDUINO_SAMD_ZERO)
        #define ARDUINO_BOARD "Zero"
    #elif defined(ARDUINO_ARC32_TOOLS)       
        #define ARDUINO_BOARD "101"
    #else
       #define ARDUINO_BOARD "Unknown"
    #endif
#endif

#endif
