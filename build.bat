@echo off
set PATH=C:\msys64\ucrt64\bin;%PATH%

gcc "Code\Movimentacao.c" "Code\TelaInicio.c" -o jogo.exe ^
-IC:\msys64\ucrt64\include ^
-LC:\msys64\ucrt64\lib ^
-lraylib -lglfw3 -lopengl32 -lgdi32 -lwinmm

if errorlevel 1 (
    echo.
    echo ERRO NA COMPILACAO
    echo Verifique se o MSYS2, GCC e raylib estao instalados.
    pause
    exit /b 1
)

echo.
echo COMPILACAO CONCLUIDA COM SUCESSO
pause