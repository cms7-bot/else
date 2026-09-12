#include "raylib.h"

int telaInicio(Texture2D botaoJogar, RenderTexture2D telaVirtual, float escala, float offsetX, float offsetY);
int telaAviso(RenderTexture2D telaVirtual, float escala, float offsetX, float offsetY);

int main()
{
    int larguraResolucao = 1280;
    int alturaResolucao = 720;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(larguraResolucao, alturaResolucao, "else");
    SetTargetFPS(60);
    InitAudioDevice();

    RenderTexture2D telaVirtual = LoadRenderTexture(larguraResolucao, alturaResolucao);

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
    Texture2D logoInicio = LoadTexture("cenario/logoInicio.png");

    int tela = -1;

    float tempoIntro = 0.0f;
    float tempoLogo = 2.0f;
    float tempoFade = 1.0f;

    float x = (larguraResolucao - frente1.width) / 2;
    float y = (alturaResolucao - frente1.height) / 2;

    int velocidade = 5;
    int direcao = 0;
    int frame = 0;
    float tempo = 0;
    int andando = 0;

    while (!WindowShouldClose())
    {
        float escalaX = (float)GetScreenWidth() / larguraResolucao;
        float escalaY = (float)GetScreenHeight() / alturaResolucao;
        float escala;

        if (escalaX < escalaY)
            escala = escalaX;
        else
            escala = escalaY;

        float larguraFinal = larguraResolucao * escala;
        float alturaFinal = alturaResolucao * escala;

        float offsetX = (GetScreenWidth() - larguraFinal) / 2;
        float offsetY = (GetScreenHeight() - alturaFinal) / 2;

        if (tela == -1)
        {
            tempoIntro += GetFrameTime();

            float alpha = 0.0f;

            if (tempoIntro >= tempoLogo)
            {
                alpha = (tempoIntro - tempoLogo) / tempoFade;

                if (alpha > 1.0f)
                    alpha = 1.0f;
            }

            BeginTextureMode(telaVirtual);
            ClearBackground(BLACK);

            float escalaLogoX = (larguraResolucao * 0.55f) / logoInicio.width;
            float escalaLogoY = (alturaResolucao * 0.75f) / logoInicio.height;
            float escalaLogo;

            if (escalaLogoX < escalaLogoY)
                escalaLogo = escalaLogoX;
            else
                escalaLogo = escalaLogoY;

            float larguraLogo = logoInicio.width * escalaLogo;
            float alturaLogo = logoInicio.height * escalaLogo;

            DrawTextureEx(
                logoInicio,
                (Vector2){
                    (larguraResolucao - larguraLogo) / 2,
                    (alturaResolucao - alturaLogo) / 2
                },
                0,
                escalaLogo,
                WHITE
            );

            DrawRectangle(
                0,
                0,
                larguraResolucao,
                alturaResolucao,
                Fade(BLACK, alpha)
            );

            EndTextureMode();

            BeginDrawing();
            ClearBackground(BLACK);

            Rectangle origemIntro = {
                0,
                0,
                (float)telaVirtual.texture.width,
                -(float)telaVirtual.texture.height
            };

            Rectangle destinoIntro = {
                offsetX,
                offsetY,
                larguraFinal,
                alturaFinal
            };

            DrawTexturePro(
                telaVirtual.texture,
                origemIntro,
                destinoIntro,
                (Vector2){0, 0},
                0,
                WHITE
            );

            EndDrawing();

            if (tempoIntro >= tempoLogo + tempoFade)
                tela = 0;

            continue;
        }

        if (tela == 0)
        {
            UpdateMusicStream(trilhaSonora1);

            if (telaInicio(botaoJogar, telaVirtual, escala, offsetX, offsetY) == 1)
            {
                tela = 1;
                PlayMusicStream(EscritorioTrilhaSonora);
                StopMusicStream(trilhaSonora1);
            }

            continue;
        }

        if (tela == 1)
        {
            UpdateMusicStream(EscritorioTrilhaSonora);

            if (telaAviso(telaVirtual, escala, offsetX, offsetY) == 2)
            {
                tela = 2;
                StopMusicStream(EscritorioTrilhaSonora);
                PlayMusicStream(trilhaSonora2);
            }

            continue;
        }

        if (tela == 2)
            UpdateMusicStream(trilhaSonora2);

        andando = 0;

        if (IsKeyDown(KEY_W))
        {
            if (!IsSoundPlaying(somDePassos))
                PlaySound(somDePassos);

            y -= velocidade;
            direcao = 1;
            andando = 1;
        }

        if (IsKeyDown(KEY_S))
        {
            if (!IsSoundPlaying(somDePassos))
                PlaySound(somDePassos);

            y += velocidade;
            direcao = 0;
            andando = 1;
        }

        if (IsKeyDown(KEY_A))
        {
            if (!IsSoundPlaying(somDePassos))
                PlaySound(somDePassos);

            x -= velocidade;
            direcao = 2;
            andando = 1;
        }

        if (IsKeyDown(KEY_D))
        {
            if (!IsSoundPlaying(somDePassos))
                PlaySound(somDePassos);

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
                    frame = 0;
            }
        }
        else
        {
            frame = 0;
            tempo = 0;
        }

        Texture2D texturaAtual;

        if (direcao == 0)
        {
            if (frame == 0) texturaAtual = frente1;
            else if (frame == 1) texturaAtual = frente2;
            else texturaAtual = frente3;
        }
        else if (direcao == 1)
        {
            if (frame == 0) texturaAtual = costas1;
            else if (frame == 1) texturaAtual = costas2;
            else texturaAtual = costas3;
        }
        else if (direcao == 2)
        {
            if (frame == 0) texturaAtual = esquerda1;
            else if (frame == 1) texturaAtual = esquerda2;
            else texturaAtual = esquerda3;
        }
        else
        {
            if (frame == 0) texturaAtual = direita1;
            else if (frame == 1) texturaAtual = direita2;
            else texturaAtual = direita3;
        }

        if (x < 0) x = 0;
        if (y < 0) y = 0;

        if (x > larguraResolucao - texturaAtual.width)
            x = larguraResolucao - texturaAtual.width;

        if (y > alturaResolucao - texturaAtual.height)
            y = alturaResolucao - texturaAtual.height;

        BeginTextureMode(telaVirtual);

        ClearBackground(RAYWHITE);
        DrawTexture(fundoCeramica, 0, 0, WHITE);
        DrawTexture(texturaAtual, x, y, WHITE);

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
            larguraFinal,
            alturaFinal
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
    UnloadTexture(logoInicio);

    UnloadRenderTexture(telaVirtual);

    CloseWindow();

    return 0;
}