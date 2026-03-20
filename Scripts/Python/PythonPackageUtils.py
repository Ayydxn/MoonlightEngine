import subprocess
import importlib
import sys

required_packages = ["colorama"]

def AreRequiredPackagesInstalled():
    print("Checking for required Python packages...")

    all_installed = True

    for package in required_packages:
        if not IsPackageInstalled(package):
            print(f"Package '{package}' wasn't found. Installing it...")

            if InstallPackage(package):
                print(f"Successfully installed '{package}'.")
            else:
                print(f"Failed to install '{package}'. Please install it manually with:")
                print(f"  python -m pip install {package}")
                all_installed = False

    return all_installed

def IsPackageInstalled(package_name):
    try:
        __import__(package_name)
        return True
    except ImportError:
        return False

def InstallPackage(package_name):
    try:
        subprocess.check_call(
            [sys.executable, "-m", "pip", "install", package_name],
            stdout=subprocess.DEVNULL,
            stderr=subprocess.STDOUT
        )

        importlib.invalidate_caches()
        
        return True
    except subprocess.CalledProcessError:
        return False