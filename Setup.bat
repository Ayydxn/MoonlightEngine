@echo off

echo Setting up Moonlight Engine for Windows...

REM Checks if Python is installed. If it isn't, the script will jump to the PythonNotInstalledError label.
python --version > NUL
if ERRORLEVEL 1 goto PythonNotInstalledError

REM Execution of all the appropriate scripts necessary for setting up Moonlight to be built and run.
call py Scripts\Python\Setup.py

PAUSE
exit /b 0

REM Represents an error for when Python isn't installed.
:PythonNotInstalledError
echo Python is not installed! Please install Python and rerun this script afterwards.

PAUSE
exit /b 1
