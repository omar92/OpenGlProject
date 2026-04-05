@echo off
REM Batch file to build and run the project using a hardcoded msbuild path

REM Set the path to msbuild.exe (update if your VS2022 is installed elsewhere)
set "MSBUILD_PATH=C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"

if not exist "%MSBUILD_PATH%" (
    echo MSBuild not found at: %MSBUILD_PATH%
    echo Please update MSBUILD_PATH in this file to match your Visual Studio installation.
    pause
    exit /b 1
)

cd /d "%~dp0"

REM Clear the log file
if exist log.txt del log.txt

REM Force a full rebuild
"%MSBUILD_PATH%" ..\openGLproject.sln /t:Rebuild /p:Configuration=Debug /p:Platform=x64
if errorlevel 1 (
    echo Build failed.
    pause
    exit /b 1
)

REM Copy shader files to output directory
copy "texture_vs.glsl" "..\x64\Debug\texture_vs.glsl" >nul
copy "texture_fs.glsl" "..\x64\Debug\texture_fs.glsl" >nul

REM Copy all .png files from project root
copy "*.png" "..\x64\Debug\" >nul
REM Copy all .png files from res folder
if exist "res" copy "res\*.png" "..\x64\Debug\" >nul

REM Run the executable and pause after
cd ..\x64\Debug
openGLproject.exe
set ERR=%ERRORLEVEL%
echo.
echo Application exited with code %ERR%
pause
