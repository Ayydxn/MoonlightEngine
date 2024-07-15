import os
import subprocess

from colorama import Fore, Style

os.chdir("../")

print(f"{Fore.GREEN}Generating Visual Studio project files...{Style.RESET_ALL}")

subprocess.call(["Engine/ThirdParty/Premake/premake5.exe", "vs2022"])
    