import subprocess

from colorama import Fore, Style

print(f"{Fore.GREEN}Generating Makefiles...{Style.RESET_ALL}")

subprocess.call(["chmod", "+x", "Engine/ThirdParty/Premake/premake5"])
subprocess.call(["Engine/ThirdParty/Premake/premake5", "gmake", "-cc=clang", "--verbose"])
