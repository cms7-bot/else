#include "raylib.h"


int telaInicio(Texture2D botaoJogar);

int main()
{   

    int larguraResolucao = 1280;
    int alturaResolucao = 720;

    InitWindow(larguraResolucao, alturaResolucao, "else");
    SetTargetFPS(60);
    InitAudioDevice();
    Music trilhaSonora1 = LoadMusicStream("musicas/trilhaTemporaria.mp3");
    Music trilhaSonora2 = LoadMusicStream("musicas/trilhaTemporaria2.mp3");
    PlayMusicStream(trilhaSonora1);

    Texture2D fundoCeramica = LoadTexture("cenario/ceramica.png");

    Texture2D costas1 = LoadTexture("assets/Costas1.png");
    Texture2D costas2 = LoadTexture("assets/Costas2.png");
    Texture2D costas3 = LoadTexture("assets/Costas3.png");

    Texture2D frente1 = LoadTexture("assets/frenteParado1.png");
    Texture2D frente2 = LoadTexture("assets/frenteParado2.png");
    Texture2D frente3 = LoadTexture("assets/frenteParado3.png");

    Texture2D direita1 = LoadTexture("assets/LadoDireito3.png");
    Texture2D direita2 = LoadTexture("assets/LadoDireito2.png");
    Texture2D direita3 = LoadTexture("assets/LadoDireito1.png");

    Texture2D esquerda1 = LoadTexture("assets/LadoEsquerdo3.png");
    Texture2D esquerda2 = LoadTexture("assets/LadoEsquerdo2.png");
    Texture2D esquerda3 = LoadTexture("assets/LadoEsquerdo1.png");

    Texture2D botaoJogar = LoadTexture("botoes/botaoPlay.png");

    int tela = 0;

    float x = 220;
    float y = 45;

    int velocidade = 5;
    int direcao = 0;
    int frame = 0;
    float tempo = 0;
    int andando = 0;

    while (!WindowShouldClose())
    {
        
        if (tela == 0)
        {
            if (telaInicio(botaoJogar) == 1)
            {
                tela = 1;
                PlayMusicStream(trilhaSonora2);
            }
            UpdateMusicStream(trilhaSonora1);
            continue;
        }
        
        UpdateMusicStream(trilhaSonora2);

        andando = 0;

        if (IsKeyDown(KEY_W))
        {
            y -= velocidade;
            direcao = 1;
            andando = 1;
        }

        if (IsKeyDown(KEY_S))
        {
            y += velocidade;
            direcao = 0;
            andando = 1;
        }

        if (IsKeyDown(KEY_A))
        {
            x -= velocidade;
            direcao = 2;
            andando = 1;
        }

        if (IsKeyDown(KEY_D))
        {
            x += velocidade;
            direcao = 3;
            andando = 1;
        }

        if (andando == 1)
        {
            tempo += GetFrameTime();

            if (tempo >= 0.12)
            {
                frame++;
                tempo = 0;

                if (frame >= 3)
                {
                    frame = 0;
                }
            }
        }
        else
        {
            frame = 0;
            tempo = 0;
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawTexture(fundoCeramica,0,0,WHITE);

        if (direcao == 0)
        {
            if (frame == 0) DrawTexture(frente1, x, y, WHITE);
            if (frame == 1) DrawTexture(frente2, x, y, WHITE);
            if (frame == 2) DrawTexture(frente3, x, y, WHITE);
        }

        if (direcao == 1)
        {
            if (frame == 0) DrawTexture(costas1, x, y, WHITE);
            if (frame == 1) DrawTexture(costas2, x, y, WHITE);
            if (frame == 2) DrawTexture(costas3, x, y, WHITE);
        }

        if (direcao == 2)
        {
            if (frame == 0) DrawTexture(esquerda1, x, y, WHITE);
            if (frame == 1) DrawTexture(esquerda2, x, y, WHITE);
            if (frame == 2) DrawTexture(esquerda3, x, y, WHITE);
        }

        if (direcao == 3)
        {
            if (frame == 0) DrawTexture(direita1, x, y, WHITE);
            if (frame == 1) DrawTexture(direita2, x, y, WHITE);
            if (frame == 2) DrawTexture(direita3, x, y, WHITE);
        }

        EndDrawing();
    }

    UnloadTexture(costas1);
    UnloadTexture(costas2);
    UnloadTexture(costas3);

    UnloadTexture(frente1);
    UnloadTexture(frente2);
    UnloadTexture(frente3);

    UnloadTexture(direita1);
    UnloadTexture(direita2);
    UnloadTexture(direita3);

    UnloadTexture(esquerda1);
    UnloadTexture(esquerda2);
    UnloadTexture(esquerda3);

    UnloadTexture(botaoJogar);

    CloseWindow();

    return 0;
}