#include "raylib.h"

int telaInicio(Texture2D botaoJogar, RenderTexture2D telaVirtual, float escalaTela, float offsetX, float offsetY)
{


    static Texture2D fundoInicio = {0};



    if (fundoInicio.id == 0)
    {
        fundoInicio = LoadTexture("cenario/FundoInicio.png");
    }

    int larguraResolucao = 1280;
    int alturaResolucao = 720;

    Vector2 mouse = GetMousePosition();

    mouse.x = (mouse.x - offsetX) / escalaTela;
    mouse.y = (mouse.y - offsetY) / escalaTela;


    float escala = 1.0f;

    Rectangle botao = {
        (larguraResolucao - 300) / 2,
        (alturaResolucao - 75) / 2,
        300,
        75
    };

    if (CheckCollisionPointRec(mouse, botao))
    {
        escala = 1.1f;

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            return 1;
        }
    }


    BeginTextureMode(telaVirtual);

    ClearBackground(RAYWHITE);

    DrawTexture(fundoInicio, 0, 0, WHITE);

    DrawText(
        "else",
        (larguraResolucao - 175) / 2,
        (alturaResolucao - 100) * 1.2 / 6,
        100,
        WHITE
    );

    DrawTextureEx(
        botaoJogar,
        (Vector2){
            (larguraResolucao - botaoJogar.width * escala) / 2,
            (alturaResolucao - botaoJogar.height * escala) / 2,
        },
        0,
        escala,
        WHITE
    );
  
    EndTextureMode();


    BeginDrawing();

    ClearBackground(BLACK);

    Rectangle origem = {
        0,
        0,
        (float)telaVirtual.texture.width,
        -(float)telaVirtual.texture.height
    };

    Rectangle destino = {
        offsetX,
        offsetY,
        larguraResolucao * escalaTela,
        alturaResolucao * escalaTela
    };

    DrawTexturePro(
        telaVirtual.texture,
        origem,
        destino,
        (Vector2){0, 0},
        0,
        WHITE
    );

    EndDrawing();

    return 0;
}


int telaAviso(RenderTexture2D telaVirtual, float escalaTela, float offsetX, float offsetY){

    static Texture2D TelaAviso = {0};

    int larguraResolucao = 1280;
    int alturaResolucao = 720;
    
    if (TelaAviso.id == 0)
    {
        TelaAviso = LoadTexture("cenario/Aviso.png");
    }

    if (IsKeyDown(KEY_SPACE)){
        return 2;
    }


    BeginTextureMode(telaVirtual);

    ClearBackground(RAYWHITE);

    DrawTexture(TelaAviso, 0, 0, WHITE);

    EndTextureMode();


    BeginDrawing();

    ClearBackground(BLACK);

    Rectangle origem = {
        0,
        0,
        (float)telaVirtual.texture.width,
        -(float)telaVirtual.texture.height
    };

    Rectangle destino = {
        offsetX,
        offsetY,
        larguraResolucao * escalaTela,
        alturaResolucao * escalaTela
    };

    DrawTexturePro(
        telaVirtual.texture,
        origem,
        destino,
        (Vector2){0, 0},
        0,
        WHITE
    );

    EndDrawing();

    return 0;
}