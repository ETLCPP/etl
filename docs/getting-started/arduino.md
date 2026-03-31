---
title: "Arduino"
weight: 5
---

This page describes the issue specific to using the ETL with the Arduino platform.  

The Arduino version of the ETL may be downloaded either through the Arduino IDE's library manager, or the ZIP fileavailable from the GitHub repository.  

To alleviate possible header name clashes, the ETL headers are accessed via the `etl` sub-directory.

For the Arduino IDE you must also include `Embedded_Template_Library.h`, otherwise the IDE will not believe that the library exists! This is not necessary if you are using PlatformIO.  

If you wish to identify the Arduino board then `Embedded_Template_Library.h` will define `ARDUINO_BOARD` as a literal string. If the board cannot be recognised then this will be set to "Unknown".

You must also define `ETL_NO_STL` if you are not using the STL.

```cpp
#define ETL_NO_STL

#include <Arduino>
#include <Embedded_Template_Library.h> // Mandatory for Arduino IDE only
#include <etl/vector.h>                // Use the ETL's vector class

String board(ARDUINO_BOARD);

void setup()
{

}

void loop()
{

}
```