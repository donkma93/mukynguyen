@echo off
title Dung toan bo Mu Server
cd /d "%~dp0Mu Server"
powershell -NoProfile -ExecutionPolicy Bypass -File ".\stop-all.ps1"
echo.
echo Xong. Nhan phim bat ky de dong...
pause >nul
