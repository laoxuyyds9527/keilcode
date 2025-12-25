@echo off
echo Cleaning Keil output files...

REM 删除KEIL5的输出文件
del /s /q *.lst
del /s /q *.crf
del /s /q *.map
del /s /q *.o
del /s /q *.d
del /s /q *.axf
del /s /q *.dep
del /s /q *.uvguix.*
del /s /q *.htm
del /s /q *.lnp
del /s /q *.sct

REM 删除Source Insight工程
for /d %%i in (si4\*) do rd /s /q "%%i"

REM 保留可执行文件
::del /s /q *.hex

REM 保留KEIL5的魔术棒配置
::del /s /q *.uvoptx

echo Cleaning done!
pause
