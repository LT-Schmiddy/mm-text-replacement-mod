import pathlib, subprocess, os, shutil

vcpkg_dir = pathlib.Path(__file__).parent.joinpath(f"vcpkg")

packages = [
    "nlohmann-json"
]

def get_dependencies() -> dict[str, str]:
    retVal = {
        'clang': shutil.which("clang"),
        'clang-cl': shutil.which("clang-cl"),
        'git': shutil.which("git") ,
        'cmake': shutil.which("cmake"),
    }
    
    for key, value in retVal.items():
        if value is None:
            raise RuntimeError(f"Couldn't find executable for '{key}', which is needed to build this project")
        else:
            print(f"Found {key} at '{value}'")
        
    retVal['ninja'] = shutil.which("ninja")
    
    if retVal['ninja'] is None:
        # raise RuntimeWarning(f"Couldn't find executable for ninja. Attempting to build N64Recomp tools may not work correctly.")
        print(f"WARNING: Couldn't find executable for ninja. Attempting to build N64Recomp tools may not work correctly.")
    else:
        print(f"Found ninja at '{retVal['ninja']}'")
    return retVal

def setup():
    deps = get_dependencies()
    
    # Downloading vcpkg:
    if not vcpkg_dir.exists():
        print("Downloading vcpkg.")  
        git_run = subprocess.run(
            [
                deps["git"],
                "clone",
                "https://github.com/microsoft/vcpkg"
            ]
        )
        if git_run.returncode != 0:
            raise RuntimeError("Git failed to download vcpkg.")
        
        # Bootstrapping vcpkg:
        bootstraper_name = "bootstrap-vcpkg.bat" if os.name == 'nt' else "bootstrap-vcpkg.sh"
        bootstrap_run = subprocess.run(
            [
                vcpkg_dir.joinpath(bootstraper_name),
                "-disableMetrics"
            ]
        )
        if bootstrap_run.returncode != 0:
            raise RuntimeError("Git failed to download vcpkg.")
    else:
        print("Vcpkg was already downloaded.")  
    # Installing packages:
    vcpkg_name = "vcpkg.exe" if os.name == 'nt' else "vcpkg"
    for i in packages:
        vcpkg_run = subprocess.run(
            [
                vcpkg_dir.joinpath(vcpkg_name),
                "install",
                i
            ]
        )
        if vcpkg_run.returncode != 0:
            raise RuntimeError("Git failed to download vcpkg.")

if __name__ == '__main__':
    setup()