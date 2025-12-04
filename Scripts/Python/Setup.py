import PythonPackageUtils

PythonPackageUtils.AreRequiredPackagesInstalled()

import colorama
from colorama import Fore, Style

import subprocess
import sys

colorama.init()

if (sys.platform == "win32"):
    print(f"{Fore.GREEN}Running setup for Windows...{Style.RESET_ALL}")
    subprocess.check_call(["py", "Scripts/Python/Windows/Setup-Windows.py"])
elif (sys.platform.startswith("linux")):
    print(f"{Fore.GREEN}Running setup for Linux...{Style.RESET_ALL}")
    subprocess.check_call(["python3", "Scripts/Python/Linux/Setup-Linux.py"])