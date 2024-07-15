import PythonPackageUtils

import colorama
from colorama import Fore, Style

import subprocess
import sys

PythonPackageUtils.AreRequiredPackagesInstalled()

colorama.init()

if (sys.platform == "win32"):
    print(f"{Fore.GREEN}Running setup for Windows...{Style.RESET_ALL}")
    subprocess.check_call(["py", "Python/Windows/Setup-Windows.py"])