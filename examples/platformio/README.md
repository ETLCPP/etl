ETL PlatformIO demo
===================

> Template to demonstrate how to use ETL with PlatformIO.

1. Open this folder in VSCode and agree if it suggests to install extension.
2. Use `Terminal -> Run Build Task... -> PlatformIO: Build` menu to compile.
3. Run `.pio/Build/native/program`.

## Details

Use `platformio.ini` for example, see comments inside.

`include` folder contains config, required for `etl`. All is as transparent
as possible. Set all additional variables via `build_flags` option in
`platform.ini`. Currently only `PROFILE_GCC_GENERIC` set to make things work.
