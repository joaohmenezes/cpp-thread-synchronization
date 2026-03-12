@echo off
echo Tentando compilar com Dev-C++...

set DEVCPP_PATH=C:\Program Files (x86)\Dev-Cpp\MinGW64\bin

if exist "%DEVCPP_PATH%\g++.exe" (
    echo Dev-C++ encontrado!
    "%DEVCPP_PATH%\g++.exe" -std=c++11 main.cpp -o sistema_frutas.exe -pthread
    
    if %errorlevel% equ 0 (
        echo Compilacao concluida!
        echo Execute: sistema_frutas.exe
    ) else (
        echo Erro na compilacao!
    )
) else (
    echo Dev-C++ nao encontrado em: %DEVCPP_PATH%
    echo.
    echo Baixe Dev-C++ em: https://sourceforge.net/projects/orwelldevcpp/
    echo Ou leia INSTALACAO.txt para outras opcoes
)

pause

