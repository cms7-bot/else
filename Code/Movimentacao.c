#include "raylib.h"


int telaInicio(Texture2D botaoJogar);
int telaAviso();

int main()
{   

    int larguraResolucao = 1280;
    int alturaResolucao = 720;

    InitWindow(larguraResolucao, alturaResolucao, "else");
    SetTargetFPS(60);
    InitAudioDevice();

    Music trilhaSonora1 = LoadMusicStream("musicas/trilhaTemporaria.mp3");
    Music trilhaSonora2 = LoadMusicStream("musicas/JazzTrilha2.mp3");
    Music EscritorioTrilhaSonora = LoadMusicStream("musicas/Escritorio2.mp3");
    Sound somDePassos = LoadSound("EfeitosSonoros/Passos2.mp3");
    SetMusicVolume(trilhaSonora2, 0.05f);
    SetMusicVolume(trilhaSonora1, 0.10f);
    SetMusicVolume(EscritorioTrilhaSonora, 0.2f);
    PlayMusicStream(trilhaSonora1);
    PlayMusicStream(EscritorioTrilhaSonora);


    Texture2D fundoCeramica = LoadTexture("cenario/ceramica.png");

    Texture2D costas1 = LoadTexture("assets/Costas/BonecoParadoCostas.png");
    Texture2D costas2 = LoadTexture("assets/Costas/BonecoAndandoCostas1.png");
    Texture2D costas3 = LoadTexture("assets/Costas/BonecoAndandoCostas2.png");

    Texture2D frente1 = LoadTexture("assets/Frente/BonecoParadoFrente.png");
    Texture2D frente2 = LoadTexture("assets/Frente/BonecoAndandoFrente1.png");
    Texture2D frente3 = LoadTexture("assets/Frente/BonecoAndandoFrente2.png");

    Texture2D direita1 = LoadTexture("assets/LadoDireito/BonecoParadoLadoDireito.png");
    Texture2D direita2 = LoadTexture("assets/LadoDireito/BonecoAndandoLadoDireito1.png");
    Texture2D direita3 = LoadTexture("assets/LadoDireito/BonecoAndandoLadoDireito2.png");

    Texture2D esquerda1 = LoadTexture("assets/LadoEsquerdo/BonecoParadoLadoEsquerdo.png");
    Texture2D esquerda2 = LoadTexture("assets/LadoEsquerdo/BonecoAndandoLadoEsquerdo1.png");
    Texture2D esquerda3 = LoadTexture("assets/LadoEsquerdo/BonecoAndandoLadoEsquerdo2.png");

    Texture2D botaoJogar = LoadTexture("Botoes/BotaoJogar.png");

    int tela = 0;

    float x = (larguraResolucao - frente1.width)/2;
    float y = (alturaResolucao - frente1.height)/2;

    int velocidade = 5;
    int direcao = 0;
    int frame = 0;
    float tempo = 0;
    int andando = 0;

    while (!WindowShouldClose())
    {
        
        if (tela == 0)
        {   
            UpdateMusicStream(trilhaSonora1);
            if (telaInicio(botaoJogar) == 1)
            {
                tela = 1;
                PlayMusicStream(EscritorioTrilhaSonora);
                StopMusicStream(trilhaSonora1);
            }
            
            continue;
        }

        if (tela==1){
        UpdateMusicStream(EscritorioTrilhaSonora);

        if (telaAviso()==2)
        {    
        tela = 2;
        StopMusicStream(EscritorioTrilhaSonora);
        PlayMusicStream(trilhaSonora2);}
        continue;
        }

        if (tela == 2) {
        UpdateMusicStream(trilhaSonora2);
        }


        andando = 0;

        if (IsKeyDown(KEY_W))
        {
        if (!IsSoundPlaying(somDePassos)){
        PlaySound(somDePassos);
        }
            y -= velocidade;
            direcao = 1;
            andando = 1;
        }

        if (IsKeyDown(KEY_S))
        {
        if (!IsSoundPlaying(somDePassos)){
        PlaySound(somDePassos);
        }
            y += velocidade;
            direcao = 0;
            andando = 1;
        }

        if (IsKeyDown(KEY_A))
        {
        if (!IsSoundPlaying(somDePassos)){
        PlaySound(somDePassos);
        }
            x -= velocidade;
            direcao = 2;
            andando = 1;
        }

        if (IsKeyDown(KEY_D))
        {
        if (!IsSoundPlaying(somDePassos)){
        PlaySound(somDePassos);
        }
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

        DrawTexture(fundoCeramica, 0, 0, WHITE);

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