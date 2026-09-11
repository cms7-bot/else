@echo off
set PATH=C:\msys64\ucrt64\bin;%PATH%

gcc "Code\Movimentacao.c" "Code\TelaInicio.c" -o jogo.exe ^
-IC:\msys64\ucrt64\include ^
-LC:\msys64\ucrt64\lib ^
-lraylib -lglfw3 -lopengl32 -lgdi32 -lwinmm

if errorlevel 1 (
    echo.
    echo ERRO NA COMPILACAO
    pause
    exit /b 1
)

echo.
echo COMPILADO COM SUCESSO
echo ABRINDO O JOGO...
echo.

jogo.exe