@echo off
setlocal enabledelayedexpansion

cd ../include/etl

> header_file_list.txt (
    for %%f in (*.h) do (
        echo %%~nxf
    )
)

cd ../../test

endlocal
