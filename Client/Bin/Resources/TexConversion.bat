@echo off
setlocal enabledelayedexpansion

cd /d %~dp0

for /R %%f in (*.dds) do (
    echo 처리 중: %%f

    REM 폴더 경로 추출
    set "filepath=%%~f"
    set "folderpath=%%~dpf"
    
    echo  !filepath!
    echo  !folderpath!
    
    REM 마지막 백슬래시 제거 (필요 없음 texconv가 잘 처리함)
    set "folderpath=!folderpath:~0,-1!"

    REM 변환 실행
    texconv.exe -y -m 0 -o . "%%f"
)

pause