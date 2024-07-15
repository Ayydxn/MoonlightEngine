import subprocess
import importlib.util

required_packages = ["colorama"]

def AreRequiredPackagesInstalled():
    print("Checking for required Python packages...")

    for package in required_packages:
        IsPackageInstalled(package)

# Checks if the provided package is installed. If it isn't, we go and install it.
def IsPackageInstalled(package_name):
    package_spec = importlib.util.find_spec(package_name)
    if package_spec is None:
        print(f"Package {package_name} wasn't found. Installing it...")
        InstallPackage(package_name)

def InstallPackage(package_name):
    subprocess.check_call(["py", "-m", "pip", "install", package_name])
