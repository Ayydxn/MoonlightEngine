import subprocess
import importlib.util
import sys

required_packages = ["colorama"]

def AreRequiredPackagesInstalled():
    print("Checking for required Python packages...")

    # Need to setup a virtual environment in the instance the Python installation is being managed by the OS's package manager.
    #if (sys.platform != "win32"):
    #    subprocess.check_call([sys.executable, "-m", "venv", "moonlight-engine"])
    #    subprocess.check_call(["source", "moonlight-engine/bin/activate"])

    for package in required_packages:
        IsPackageInstalled(package)

# Checks if the provided package is installed. If it isn't, we go and install it.
def IsPackageInstalled(package_name):
    package_spec = importlib.util.find_spec(package_name)
    if package_spec is None:
        print(f"Package {package_name} wasn't found. Installing it...")
        InstallPackage(package_name)

def InstallPackage(package_name):
    pip_command = [sys.executable, "-m", "pip", "install", package_name]

    subprocess.check_call(pip_command)
