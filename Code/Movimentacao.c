#include "raylib.h"
#include <stdbool.h>

int telaInicio(Texture2D botaoJogar, RenderTexture2D telaVirtual, float escala, float offsetX, float offsetY, Music trilhaSonora1);
int telaAviso(RenderTexture2D telaVirtual, float escala, float offsetX, float offsetY);

void iniciarCartas(void);
void atualizarCartas(void);
void desenharCartas(int larguraResolucao, int alturaResolucao);
bool cartasEstaoAbertas(void);
void descarregarCartas(void);

int main()
{
    int larguraResolucao = 1280;
    int alturaResolucao = 720;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_UNDECORATED);
    InitWindow(larguraResolucao, alturaResolucao, "else");
    SetExitKey(KEY_NULL);

    int monitor = GetCurrentMonitor();
    int larguraMonitor = GetMonitorWidth(monitor);
    int alturaMonitor = GetMonitorHeight(monitor);
    Vector2 posicaoMonitor = GetMonitorPosition(monitor);

    SetWindowPosition(posicaoMonitor.x, posicaoMonitor.y);
    SetWindowSize(larguraMonitor, alturaMonitor);

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

    Texture2D fundoCeramica = LoadTexture("cenario/Cenario.png");
    Texture2D mesa = LoadTexture("cenario/mesa.png");
    mesa.height*=0.5;
    mesa.width*=0.5;
    Texture2D confianca = LoadTexture("HUD/hud_confianca_vazia.png");
    confianca.width*=0.8;
    confianca.height*=0.8;

    Texture2D lucro = LoadTexture("HUD/hud_lucro_vazio.png");
    lucro.width*=0.8;
    lucro.height*=0.8;

    Texture2D privacidade = LoadTexture("HUD/hud_privanca_vazia.png");
    privacidade.width*=0.7;
    privacidade.height*=0.7;

    Texture2D vies = LoadTexture("HUD/hud_vieis_vazio.png");
    vies.width*=0.8;
    vies.height*=0.8;

    Texture2D confiancaCheia = LoadTexture("HUD/hud_confianca_cheia.png");
    confiancaCheia.width*=0.8;
    confiancaCheia.height*=0.8;

    Texture2D lucroCheio = LoadTexture("HUD/hud_lucro_cheio.png");
    lucroCheio.width*=0.8;
    lucroCheio.height*=0.8;

    Texture2D privacidadeCheia = LoadTexture("HUD/hud_privanca_cheia.png");
    privacidadeCheia.width*=0.7;
    privacidadeCheia.height*=0.7;

    Texture2D viesCheio = LoadTexture("HUD/hud_vieis_cheio.png");
    viesCheio.width*=0.8;
    viesCheio.height*=0.8;

    Texture2D fundoHUD = LoadTexture("HUD/FUNDO.png");
    fundoHUD.width*=0.85;
    fundoHUD.height*=0.8;


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
    bool mensagem = false;
    bool cartaAbertaPararDeAndar = false;

    // 73% é o mínimo
    float porcentagemCadeadoApagado = 0.3f;
    float porcentagemViesApagado = 0.32f;
    float porcentagemPrivacidadeApagado = 0.0f;
    float porcentagemLucroApagado = 0.40f;
   

    iniciarCartas();

    int tela = -1;

    float tempoIntro = 0.0f;
    float tempoLogo = 2.0f;
    float tempoFade = 2.0f;

    float x = (larguraResolucao - frente1.width) / 2;
    float y = (alturaResolucao - frente1.height) / 2;

    int velocidade = 5;
    int direcao = 0;
    int frame = 0;
    float tempo = 0;
    int andando = 0;
    bool pausado = false;
    Texture2D texturaAtualGuardada = frente1;

    while (!WindowShouldClose())
    {
        float escalaX = (float)GetScreenWidth() / larguraResolucao;
        float escalaY = (float)GetScreenHeight() / alturaResolucao;
        float escala;
        int xGuardado = x;
        int yGuardado = y;

        

        if (escalaX < escalaY)
            escala = escalaX;
        else
            escala = escalaY;

        float larguraFinal = larguraResolucao * escala;
        float alturaFinal = alturaResolucao * escala;

        float offsetX = (GetScreenWidth() - larguraFinal) / 2;
        float offsetY = (GetScreenHeight() - alturaFinal) / 2;

        if (tela == 2 && IsKeyPressed(KEY_ESCAPE))
        {
            pausado = !pausado;

            if (pausado)
            {
                PauseMusicStream(trilhaSonora2);
                StopSound(somDePassos);
            }
            else
            {
                ResumeMusicStream(trilhaSonora2);
            }
        }

        if (tela == 2 && pausado)
        {
            BeginDrawing();

            ClearBackground(BLACK);

            Rectangle origemPause = {
                0,
                0,
                (float)telaVirtual.texture.width,
                -(float)telaVirtual.texture.height
            };

            Rectangle destinoPause = {
                offsetX,
                offsetY,
                larguraFinal,
                alturaFinal
            };

            DrawTexturePro(
                telaVirtual.texture,
                origemPause,
                destinoPause,
                (Vector2){0, 0},
                0,
                WHITE
            );

            DrawRectangle(
                offsetX,
                offsetY,
                larguraFinal,
                alturaFinal,
                Fade(BLACK, 0.40f)
            );

            const char *textoPause = "Jogo pausado";
            int tamanhoTexto = (int)(50 * escala);
            int larguraTexto = MeasureText(textoPause, tamanhoTexto);

            DrawText(
                textoPause,
                offsetX + larguraFinal / 2 - larguraTexto / 2,
                offsetY + alturaFinal / 2 - tamanhoTexto / 2,
                tamanhoTexto,
                WHITE
            );

            EndDrawing();

            continue;
        }

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

            if (telaInicio(botaoJogar, telaVirtual, escala, offsetX, offsetY, trilhaSonora1) == 1)
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

        // bool cartaEstavaAberta = cartasEstaoAbertas();

        // atualizarCartas();

        // bool cartaAgoraAberta = cartasEstaoAbertas();
        // bool bloquearMovimentoCarta = cartaEstavaAberta || cartaAgoraAberta;

        andando = 0;
        Texture2D texturaAtual = texturaAtualGuardada;

        bool botaoAeDapertados = IsKeyDown(KEY_D) && IsKeyDown(KEY_A);
        bool botaoWeSapertados = IsKeyDown(KEY_W) && IsKeyDown(KEY_S);

        if (!cartaAbertaPararDeAndar)
        {   
            
            if (!botaoAeDapertados && !botaoWeSapertados)
            {
                if (IsKeyDown(KEY_W))
                {
                   
                    if (!IsSoundPlaying(somDePassos))
                        PlaySound(somDePassos);

                    y -= velocidade;
                    direcao = 1;
                    andando = 1;

                    if (IsKeyPressed(KEY_W))
                        frame = 1;
                }

                if (IsKeyDown(KEY_S))
                {
                    if (!IsSoundPlaying(somDePassos))
                        PlaySound(somDePassos);

                    y += velocidade;
                    direcao = 0;
                    andando = 1;

                    if (IsKeyPressed(KEY_S))
                        frame = 1;
                }

                if (IsKeyDown(KEY_A))
                {
                    if (!IsSoundPlaying(somDePassos))
                        PlaySound(somDePassos);

                    x -= velocidade;
                    direcao = 2;
                    andando = 1;

                    if (IsKeyPressed(KEY_A))
                        frame = 1;
                }

                if (IsKeyDown(KEY_D))
                {
                    texturaAtual = direita2;

                    if (!IsSoundPlaying(somDePassos))
                        PlaySound(somDePassos);

                    x += velocidade;
                    direcao = 3;
                    andando = 1;

                    if (IsKeyPressed(KEY_D))
                        frame = 1;
                }
            }
            else
            {
                andando = 0;

                if ((texturaAtualGuardada.id == frente2.id) || (texturaAtualGuardada.id == frente3.id))
                    texturaAtual = frente1;

                if ((texturaAtualGuardada.id == costas2.id) || (texturaAtualGuardada.id == costas3.id))
                    texturaAtual = costas1;

                if ((texturaAtualGuardada.id == direita2.id) || (texturaAtualGuardada.id == direita3.id))
                    texturaAtual = direita1;

                if ((texturaAtualGuardada.id == esquerda2.id) || (texturaAtualGuardada.id == esquerda3.id))
                    texturaAtual = esquerda1;
            }
        }
        else
        {
            andando = 0;
            StopSound(somDePassos);

            if (direcao == 0)
                texturaAtual = frente1;
            else if (direcao == 1)
                texturaAtual = costas1;
            else if (direcao == 2)
                texturaAtual = esquerda1;
            else
                texturaAtual = direita1;
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

        if (!cartaAbertaPararDeAndar && !botaoAeDapertados && !botaoWeSapertados)
        {
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
        }
        if (x < 15) x = 15;

        if (x > larguraResolucao - texturaAtual.width - 15)
            x = larguraResolucao - texturaAtual.width - 15;

        if (y < 80) y = 80;

        if (y > alturaResolucao - texturaAtual.height - 20)
            y = alturaResolucao - texturaAtual.height - 20;

        if (x > larguraResolucao - texturaAtual.width)
            x = larguraResolucao - texturaAtual.width;

        if (y > alturaResolucao - texturaAtual.height)
            y = alturaResolucao - texturaAtual.height;
        
        BeginTextureMode(telaVirtual);
        ClearBackground(RAYWHITE); 
        
        DrawTexture(fundoCeramica, 0, 0, WHITE);
        Rectangle colisaoMesaBoneco_Boneco ={x,y,frente1.width,frente1.height};
        Rectangle colisaoMesaBoneco_Mesa ={(larguraResolucao-mesa.width)-20+mesa.width*0.1,mesa.height-40,mesa.width*0.8,mesa.height*0.3};
        if (CheckCollisionRecs(colisaoMesaBoneco_Boneco,colisaoMesaBoneco_Mesa)){
            mensagem = true;
            x = xGuardado;
            y= yGuardado;}
        
        
        if (x>=larguraResolucao-mesa.width-60 && y<=mesa.height)  {  
        DrawText("Pressione E para realizar a tarefa",larguraResolucao-400,60,20,GREEN);
        bool cartaEstavaAberta = cartasEstaoAbertas();

        atualizarCartas();

        bool cartaAgoraAberta = cartasEstaoAbertas();
        bool bloquearMovimentoCarta = cartaEstavaAberta || cartaAgoraAberta;
        cartaAbertaPararDeAndar = bloquearMovimentoCarta;


        }
       
        



    DrawTexture(mesa,(larguraResolucao-mesa.width)-20,mesa.height-40,WHITE);
    DrawTexture(texturaAtual, x, y, WHITE);
    desenharCartas(
        larguraResolucao,
        alturaResolucao
    );
    
    // DrawRectangle((larguraResolucao-confianca.width-vies.width-privacidade.width-lucro.width-90)/2,3,confianca.width+vies.width+privacidade.width+lucro.width+90,privacidade.height+5,ColorAlpha(BLACK,0.70));


DrawTexture(fundoHUD,((larguraResolucao-confianca.width)/2)-confianca.width-145,privacidade.height*0.05,WHITE);

DrawTexture(confianca,((larguraResolucao-confianca.width)/2)-confianca.width-135,privacidade.height*0.05,WHITE);   
BeginScissorMode(
    ((larguraResolucao - confianca.width) / 2) - confianca.width - 135 ,

    privacidade.height * 0.05,

    confiancaCheia.width * (1.0f -porcentagemCadeadoApagado),
    confiancaCheia.height
);
DrawTexture(confiancaCheia, ((larguraResolucao - confianca.width) / 2) - confianca.width - 135, privacidade.height * 0.05,
WHITE
);
 EndScissorMode();




DrawTexture(fundoHUD,((larguraResolucao-vies.width)/2)-125,privacidade.height*0.05,WHITE);
DrawTexture(vies,((larguraResolucao-vies.width)/2)-120,privacidade.height*0.05,WHITE);
BeginScissorMode(
    ((larguraResolucao-vies.width)/2)-120 ,

    privacidade.height * 0.05,

    viesCheio.width * (1.0f -porcentagemViesApagado),
    viesCheio.height
);
DrawTexture(viesCheio,((larguraResolucao-vies.width)/2)-120,privacidade.height*0.05,WHITE);

 EndScissorMode();


DrawTexture(fundoHUD,((larguraResolucao-privacidade.width)/2)+lucro.width-115,privacidade.height*0.05,WHITE);
DrawTexture(privacidade,((larguraResolucao-privacidade.width)/2)+lucro.width-105,privacidade.height*0.05,WHITE);

BeginScissorMode(
    ((larguraResolucao-privacidade.width)/2)+lucro.width-105 ,

    privacidade.height * 0.05,

    privacidadeCheia.width * (1.0f -porcentagemPrivacidadeApagado),
    privacidadeCheia.height
);
DrawTexture(privacidadeCheia,((larguraResolucao-privacidade.width)/2)+lucro.width-105,privacidade.height*0.05,WHITE);

 EndScissorMode();

DrawTexture(fundoHUD,((larguraResolucao-lucro.width)/2)+2*(lucro.width)-80,privacidade.height*0.05,WHITE);
DrawTexture(lucro,((larguraResolucao-lucro.width)/2)+2*(lucro.width)-75,privacidade.height*0.05,WHITE);
BeginScissorMode(
    ((larguraResolucao-lucro.width)/2)+2*(lucro.width)-75 ,

    privacidade.height * 0.05,

    lucroCheio.width * (1.0f -porcentagemViesApagado),
    lucroCheio.height
);
 DrawTexture(lucroCheio,((larguraResolucao-lucro.width)/2)+2*(lucro.width)-75,privacidade.height*0.05,WHITE);

 EndScissorMode();


    





        
        



        
        // DrawTexture(confianca,((larguraResolucao-confianca.width)/2)-confianca.width-135,privacidade.height*0.05,WHITE);
        // DrawTexture(vies,((larguraResolucao-vies.width)/2)-120,privacidade.height*0.05,WHITE);
        // DrawTexture(privacidade,((larguraResolucao-privacidade.width)/2)+lucro.width-105,privacidade.height*0.05,WHITE);
        // DrawTexture(lucro,((larguraResolucao-lucro.width)/2)+2*(lucro.width)-75,privacidade.height*0.05,WHITE);

        // DrawTexture(confiancaCheia,((larguraResolucao-confianca.width)/2)-confianca.width-135,privacidade.height*0.05,WHITE);
        // DrawTexture(viesCheio,((larguraResolucao-vies.width)/2)-120,privacidade.height*0.05,WHITE);
        // DrawTexture(privacidadeCheia,((larguraResolucao-privacidade.width)/2)+lucro.width-105,privacidade.height*0.05,WHITE);
        // DrawTexture(lucroCheio,((larguraResolucao-lucro.width)/2)+2*(lucro.width)-75,privacidade.height*0.05,WHITE);

       






        // DrawText("Pressione E para realizar a tarefa",25,70,20,GREEN);
        


        texturaAtualGuardada = texturaAtual;

        

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
    

    descarregarCartas();

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

    CloseAudioDevice();
    CloseWindow();

    return 0;
}