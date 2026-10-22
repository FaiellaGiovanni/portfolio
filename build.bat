@echo off
rem Compiles the C++ generator and builds the site into public\
rem Run from the portfolio folder:  build.bat

cd /d "%~dp0"
if not exist build mkdir build

echo Compiling generator...
g++ -std=c++17 -O2 -Wall -Wextra ^
  generator\src\main.cpp ^
  generator\src\markdown.cpp ^
  generator\src\templates.cpp ^
  generator\src\util.cpp ^
  -o build\sitegen.exe
if errorlevel 1 (
  echo.
  echo Compile failed. See the errors above.
  exit /b 1
)

build\sitegen.exe
