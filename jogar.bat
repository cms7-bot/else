@echo off
cd /d "%~dp0"

set PATH=C:\msys64\ucrt64\bin;%PATH%

echo.
echo COMPILANDO O JOGO...
echo.

gcc "Code\Movimentacao.c" "Code\TelaInicio.c" -o jogo_novo.exe ^
-IC:\msys64\ucrt64\include ^
-LC:\msys64\ucrt64\lib ^
-lraylib -lglfw3 -lopengl32 -lgdi32 -lwinmm

if errorlevel 1 (
    if exist jogo_novo.exe del jogo_novo.exe

    echo.
    echo ERRO NA COMPILACAO
    echo O jogo.exe anterior foi mantido.
    echo.
    pause
    exit /b 1
)

move /Y jogo_novo.exe jogo.exe >nul

if errorlevel 1 (
    echo.
    echo NAO FOI POSSIVEL ATUALIZAR jogo.exe
    echo Feche o jogo caso ele ainda esteja aberto.
    echo.
    pause
    exit /b 1
)

echo.
echo COMPILADO COM SUCESSO
echo ABRINDO O JOGO...
echo.

jogo.exe