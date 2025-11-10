@echo off
setlocal

cd /d %~dp0

for /R %%f in (*.dds) do (
    echo º¯È¯ Áß: %%f

    set "filepath=%%f"
    set "folderpath=%%~dpf"

    REM set "folderpath=!folderpath:~0,-1!"

    texconv.exe -y -m 0 -o . "%%f"
)

pause