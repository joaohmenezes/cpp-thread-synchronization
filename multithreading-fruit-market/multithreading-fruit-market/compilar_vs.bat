@echo off
echo Tentando compilar com Visual Studio...

where cl >nul 2>nul
if %errorlevel% neq 0 (
    echo Procurando Visual Studio...
    
    if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" (
        call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
    ) else if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat" (
        call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
    ) else (
        echo Visual Studio nao encontrado!
        echo.
        echo Leia o arquivo INSTALACAO.txt para instalar o compilador.
        pause
        exit /b 1
    )
)

echo Compilando...
cl /EHsc /std:c++14 main.cpp /Fe:sistema_frutas.exe

if %errorlevel% equ 0 (
    echo Compilacao concluida!
    echo Execute: sistema_frutas.exe
) else (
    echo Erro na compilacao!
)

pause

