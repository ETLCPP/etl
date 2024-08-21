@echo off
setlocal enabledelayedexpansion

> test_file_list.txt (
    for %%f in (test_*.cpp) do (
        echo %%~nxf
    )
)

endlocal
