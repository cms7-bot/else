#include "raylib.h"

int telaInicio(Texture2D botaoJogar)
{

    static Texture2D fundoInicio = {0};
    Music trilhaSonora1 = LoadMusicStream("trilhaTemporaria.mp3");
    UpdateMusicStream(trilhaSonora1);
    PlayMusicStream(trilhaSonora1);

    if (fundoInicio.id == 0)
    {
        fundoInicio = LoadTexture("cenario/fundoInicio.png");
    }
    int larguraResolucao = 1280;
    int alturaResolucao = 720;
    Vector2 mouse = GetMousePosition();


    float escala = 1.0f;

    Rectangle botao = {
        (larguraResolucao - 300)/ 2,
        (alturaResolucao-75)/2,
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

    BeginDrawing();

    ClearBackground(RAYWHITE);
    DrawTexture(fundoInicio,0,0,WHITE);

    DrawText("else", (larguraResolucao-175)/2, (alturaResolucao-100)*1.2/6, 100, WHITE);

    DrawTextureEx(
        botaoJogar,
        (Vector2){
        (larguraResolucao - botaoJogar.width*escala)/ 2,
        (alturaResolucao-botaoJogar.height*escala)/2,
        },
        0,
        escala,
        WHITE
    );
  
    EndDrawing();
    return 0;
}