#include "raylib.h"
#include "rlgl.h"
#include <stdbool.h>
#include "Regras.h"

int telaInicio(Texture2D botaoJogar, RenderTexture2D telaVirtual, float escala, float offsetX, float offsetY, Music trilhaSonora1);
int telaAviso(RenderTexture2D telaVirtual, float escala, float offsetX, float offsetY);

void iniciarCartas(float *cadeado, float *vies, float *privacidade, float *lucro);
void atualizarCartas(void);
void desenharCartas(int larguraResolucao, int alturaResolucao);
bool cartasEstaoAbertas(void);
void definirOrigemCartas(int origem);
void descarregarCartas(void);

const char *obterMensagemFinal(void);
int obterDiasSobrevividos(void);
const char *nomeCargo(void);

// MAQUINA DE CAFE (MaquinaCafe.c)
void iniciarMaquinaCafe(void);
void descarregarMaquinaCafe(void);
void reiniciarMaquinaCafe(void);
bool maquinaCafeAberta(void);
void fecharMaquinaCafe(void);
void interagirBalcaoCafe(int larguraResolucao);
void desenharAvisoBalcaoCafe(void);
void atualizarDesenharMaquinaCafe(float escala, float offsetX, float offsetY);
void desenharTextoQuebrado(const char *texto, int x, int y, int larguraMaxima, int tamanho, Color cor);


// ==========================================================
// OBJETOS DO ESCRITÓRIO
// ==========================================================
typedef struct ObjetoCenario
{
    Texture2D textura;
    float x, y;
    Rectangle colisao;
    bool interativo;
    int origemCartas;
} ObjetoCenario;


static ObjetoCenario criarObjeto(
    const char *arquivo,
    float x,
    float y,
    Rectangle colisao,
    bool interativo,
    int origemCartas
)
{
    ObjetoCenario objeto;

    objeto.textura = LoadTexture(arquivo);

    objeto.textura.width *= 0.5;
    objeto.textura.height *= 0.5;

    objeto.x = x;
    objeto.y = y;

    objeto.colisao = (Rectangle){
        x + colisao.x,
        y + colisao.y,
        colisao.width,
        colisao.height
    };

    objeto.interativo = interativo;
    objeto.origemCartas = origemCartas;

    return objeto;
}


int main()
{
    // ==========================================================
    // JANELA / RESOLUÇÃO
    // ==========================================================
    int larguraResolucao = 1280;
    int alturaResolucao = 720;

    SetConfigFlags(
        FLAG_WINDOW_RESIZABLE |
        FLAG_WINDOW_UNDECORATED
    );

    InitWindow(
        larguraResolucao,
        alturaResolucao,
        "else"
    );

    SetExitKey(KEY_NULL);

    int monitor = GetCurrentMonitor();

    int larguraMonitor =
        GetMonitorWidth(monitor);

    int alturaMonitor =
        GetMonitorHeight(monitor);

    Vector2 posicaoMonitor =
        GetMonitorPosition(monitor);

    SetWindowPosition(
        posicaoMonitor.x,
        posicaoMonitor.y
    );

    SetWindowSize(
        larguraMonitor,
        alturaMonitor
    );

    SetTargetFPS(60);

    InitAudioDevice();

    RenderTexture2D telaVirtual =
        LoadRenderTexture(
            larguraResolucao,
            alturaResolucao
        );


    // ==========================================================
    // SOM
    // ==========================================================
    Music trilhaSonora1 =
        LoadMusicStream(
            "musicas/trilhaTemporaria.mp3"
        );

    Music trilhaSonora2 =
        LoadMusicStream(
            "musicas/JazzTrilha2.mp3"
        );

    Music EscritorioTrilhaSonora =
        LoadMusicStream(
            "musicas/Escritorio2.mp3"
        );

    Music gameOverSom =
        LoadMusicStream(
            "musicas/GameOverSong.mp3"
        );

    Sound somDePassos =
        LoadSound(
            "EfeitosSonoros/Passos2.mp3"
        );

    SetMusicVolume(
        trilhaSonora2,
        0.05f
    );

    SetMusicVolume(
        trilhaSonora1,
        0.10f
    );

    SetMusicVolume(
        EscritorioTrilhaSonora,
        0.2f
    );

    PlayMusicStream(
        trilhaSonora1
    );

    PlayMusicStream(
        EscritorioTrilhaSonora
    );


    // ==========================================================
    // TEXTURAS - CENÁRIO
    // ==========================================================
    Texture2D fundoCeramica =
        LoadTexture(
            "cenario/Cenario.png"
        );


    #define ELEMENTOS "cenario/elementos_estagiario/"


    ObjetoCenario objetos[] =
    {
        criarObjeto(
            ELEMENTOS "planta.png",
            12,
            83,
            (Rectangle){5, 55, 45, 55},
            false,
            0
        ),

        criarObjeto(
            ELEMENTOS "balcao_cafe.png",
            67,
            91,
            (Rectangle){0, 45, 155, 70},
            true,   // MAQUINA DE CAFE: o balcão agora é interativo
            0
        ),

        criarObjeto(
            ELEMENTOS "bebedouro.png",
            234,
            79,
            (Rectangle){0, 60, 52, 65},
            false,
            0
        ),

        criarObjeto(
            ELEMENTOS "lixeira.png",
            1052,
            148,
            (Rectangle){5, 10, 40, 40},
            false,
            0
        ),

        criarObjeto(
            ELEMENTOS "folhas.png",
            1107,
            121,
            (Rectangle){0, 30, 50, 52},
            false,
            0
        ),

        criarObjeto(
            ELEMENTOS "impressora.png",
            1161,
            99,
            (Rectangle){0, 55, 80, 45},
            true,
            1
        ),

        criarObjeto(
            ELEMENTOS "MesaFrenteSemNpc.png",
            430,
            251,
            (Rectangle){10, 64, 142, 68},
            true,
            0
        ),

        criarObjeto(
            ELEMENTOS "MesaFrenteSemNpc.png",
            574,
            251,
            (Rectangle){10, 64, 142, 68},
            true,
            0
        ),

        criarObjeto(
            ELEMENTOS "MesaFrenteSemNpc.png",
            824,
            251,
            (Rectangle){10, 64, 142, 68},
            true,
            0
        ),

        criarObjeto(
            ELEMENTOS "MesaFrenteSemNpc.png",
            968,
            251,
            (Rectangle){10, 64, 142, 68},
            true,
            0
        ),


        criarObjeto(
            ELEMENTOS "MesaCostaSemNpc.png",
            573,
            473,
            (Rectangle){10, 45, 142, 75},
            true,
            0
        ),

        criarObjeto(
            ELEMENTOS "MesaCostaSemNpc2.png",
            834,
            473,
            (Rectangle){10, 45, 142, 75},
            true,
            0
        ),

        criarObjeto(
            ELEMENTOS "NpcMulherDigitando/NpcMulherDigitando1.png",
            979,
            473,
            (Rectangle){10, 45, 142, 75},
            true,
            0
        ),

        criarObjeto(
            ELEMENTOS "MesaCostaComNpc/MesaCostaComNpc1.png",
            428,
            473,
            (Rectangle){10, 45, 142, 75},
            true,
            0
        ),

        criarObjeto(
            ELEMENTOS "MesaCostaComNpc/MesaCostaComNpc2.png",
            428,
            473,
            (Rectangle){10, 45, 142, 75},
            true,
            0
        ),

        criarObjeto(
            ELEMENTOS "MesaCostaComNpc/MesaCostaComNpc3.png",
            428,
            473,
            (Rectangle){10, 45, 142, 75},
            true,
            0
        )
    };


    int quantidadeObjetos =
        sizeof(objetos) /
        sizeof(objetos[0]);

    int frameNpc = 0;
    float tempoAnimacaoNpc = 0;

    // ---- NOVOS NPCs ANIMADOS -------------------------------
    // mulher de costas (objeto 12): digitando, mãos alternando
    Texture2D npcMulher[3] = {
        LoadTexture(ELEMENTOS "NpcMulherDigitando/NpcMulherDigitando1.png"),
        LoadTexture(ELEMENTOS "NpcMulherDigitando/NpcMulherDigitando2.png"),
        LoadTexture(ELEMENTOS "NpcMulherDigitando/NpcMulherDigitando3.png")
    };
    int sequenciaMulher[4] = {0, 1, 0, 2};          // normal, mão esq., normal, mão dir.

    // NPC da 1ª fileira (no lugar da mesa vazia, objeto 7): pisca
    Texture2D npcFrente[2] = {
        LoadTexture(ELEMENTOS "NpcFrentePiscando/NpcFrentePiscando1.png"),
        LoadTexture(ELEMENTOS "NpcFrentePiscando/NpcFrentePiscando2.png")
    };
    int sequenciaFrente[16] = {0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0};   // 16 passos de 0,25s = 4s (só pisca)

    // o jogo desenha os objetos do cenário pela metade (igual ao criarObjeto)
    for (int k = 0; k < 3; k++)
    {
        npcMulher[k].width *= 0.5;  npcMulher[k].height *= 0.5;
    }
    for (int k = 0; k < 2; k++)
    {
        npcFrente[k].width *= 0.5;  npcFrente[k].height *= 0.5;
    }

    // bebedouro (objeto 2): de tempo em tempo sobem bolhas no galão
    Texture2D bebedouroBolhas[5];
    for (int k = 0; k < 5; k++)
    {
        bebedouroBolhas[k] = LoadTexture(TextFormat(ELEMENTOS "BebedouroBolhas/BebedouroBolhas%d.png", k + 1));
        bebedouroBolhas[k].width *= 0.5;
        bebedouroBolhas[k].height *= 0.5;
    }
    float tempoBolhas = 0.0f;
    float esperaBolhas = 3.0f;      // segundos parado até a próxima bolha (sorteado de novo a cada vez)

    float tempoNovosNpcs = 0.0f;
    int passoNovosNpcs = 0;

    // MAQUINA DE CAFE: carrega as peças e começa disponível
    iniciarMaquinaCafe();
    reiniciarMaquinaCafe();

    Texture2D logoInicio =
        LoadTexture(
            "cenario/logoInicio.png"
        );

    Texture2D fundoCarta =
        LoadTexture(
            "cenario/Frame3.png"
        );

    fundoCarta.height *= 0.95;
    fundoCarta.width *= 0.95;


    // ==========================================================
    // HUD
    // ==========================================================
    Texture2D confianca =
        LoadTexture(
            "HUD/hud_confianca_vazia.png"
        );

    confianca.width *= 0.8;
    confianca.height *= 0.8;


    Texture2D lucro =
        LoadTexture(
            "HUD/hud_lucro_vazio.png"
        );

    lucro.width *= 0.8;
    lucro.height *= 0.8;


    Texture2D privacidade =
        LoadTexture(
            "HUD/hud_privanca_vazia.png"
        );

    privacidade.width *= 0.7;
    privacidade.height *= 0.7;


    Texture2D vies =
        LoadTexture(
            "HUD/hud_vieis_vazio.png"
        );

    vies.width *= 0.8;
    vies.height *= 0.8;


    Texture2D confiancaCheia =
        LoadTexture(
            "HUD/hud_confianca_cheia.png"
        );

    confiancaCheia.width *= 0.8;
    confiancaCheia.height *= 0.8;


    Texture2D lucroCheio =
        LoadTexture(
            "HUD/hud_lucro_cheio.png"
        );

    lucroCheio.width *= 0.8;
    lucroCheio.height *= 0.8;


    Texture2D privacidadeCheia =
        LoadTexture(
            "HUD/hud_privanca_cheia.png"
        );

    privacidadeCheia.width *= 0.7;
    privacidadeCheia.height *= 0.7;


    Texture2D viesCheio =
        LoadTexture(
            "HUD/hud_vieis_cheio.png"
        );

    viesCheio.width *= 0.8;
    viesCheio.height *= 0.8;


    Texture2D fundoHUD =
        LoadTexture(
            "HUD/FUNDO.png"
        );

    fundoHUD.width *= 0.85;
    fundoHUD.height *= 0.8;

    Texture2D telaContadorDeDias = LoadTexture("HUD/DiaContador2.png");
    telaContadorDeDias.height*=1.5;
    telaContadorDeDias.width*=2.3;


    // ==========================================================
    // PERSONAGEM
    // ==========================================================
    Texture2D costas1 =
        LoadTexture(
            "assets/Costas/BonecoParadoCostas.png"
        );

    Texture2D costas2 =
        LoadTexture(
            "assets/Costas/BonecoAndandoCostas1.png"
        );

    Texture2D costas3 =
        LoadTexture(
            "assets/Costas/BonecoAndandoCostas2.png"
        );


    Texture2D frente1 =
        LoadTexture(
            "assets/Frente/BonecoParadoFrente.png"
        );

    Texture2D frente2 =
        LoadTexture(
            "assets/Frente/BonecoAndandoFrente1.png"
        );

    Texture2D frente3 =
        LoadTexture(
            "assets/Frente/BonecoAndandoFrente2.png"
        );


    Texture2D direita1 =
        LoadTexture(
            "assets/LadoDireito/BonecoParadoLadoDireito.png"
        );

    Texture2D direita2 =
        LoadTexture(
            "assets/LadoDireito/BonecoAndandoLadoDireito1.png"
        );

    Texture2D direita3 =
        LoadTexture(
            "assets/LadoDireito/BonecoAndandoLadoDireito2.png"
        );


    Texture2D esquerda1 =
        LoadTexture(
            "assets/LadoEsquerdo/BonecoParadoLadoEsquerdo.png"
        );

    Texture2D esquerda2 =
        LoadTexture(
            "assets/LadoEsquerdo/BonecoAndandoLadoEsquerdo1.png"
        );

    Texture2D esquerda3 =
        LoadTexture(
            "assets/LadoEsquerdo/BonecoAndandoLadoEsquerdo2.png"
        );


    // ==========================================================
    // BOTÕES
    // ==========================================================
    Texture2D botaoJogar =
        LoadTexture(
            "Botoes/BotaoJogar.png"
        );

    Texture2D botaoExit =
        LoadTexture(
            "Botoes/botaoExit.png"
        );

    Texture2D botaoPlayAgain =
        LoadTexture(
            "Botoes/botaoPlayAgain.png"
        );

    Texture2D botaoShare =
        LoadTexture(
            "Botoes/botaoShare.png"
        );

    Texture2D botaoContinue =
        LoadTexture(
            "Botoes/BotaoContinue.png"
        );

    Texture2D botaoInstrucoesPausa =
        LoadTexture(
            "Botoes/botaoInstrucoes.png"
        );

    Texture2D botaoMenuInicial =
        LoadTexture(
            "Botoes/botaoMenuInicial.png"
        );


    // ==========================================================
    // TELAS
    // ==========================================================
    Texture2D telaPausa =
        LoadTexture(
            "cenario/TelaPausa.png"
        );

    Texture2D telaInstrucoesPausa =
        LoadTexture(
            "cenario/Telas/telaInstrucoes.png"
        );

    Texture2D painelFimExpediente =
        LoadTexture(
            "cenario/Telas/painelFimExpediente.png"
        );


    // ==========================================================
    // ESTADO
    // ==========================================================
    int tela = -1;

    float tempoIntro = 0.0f;
    float tempoLogo = 2.0f;
    float tempoFade = 2.0f;


    // ==========================================================
    // MOVIMENTAÇÃO
    // ==========================================================
    float x = 300;
    float y = 170;

    int velocidade = 5;
    int direcao = 0;
    int andando = 0;


    // ==========================================================
    // ANIMAÇÃO
    // ==========================================================
    int frame = 0;
    float tempo = 0;

    Texture2D texturaAtualGuardada =
        frente1;


    // ==========================================================
    // HUD
    // ==========================================================
    float porcentagemConfiancaApagado =
        0.365f;

    float porcentagemViesApagado =
        0.365f;

    float porcentagemPrivacidadeApagado =
        0.365f;

    float porcentagemLucroApagado =
        0.365f;


    iniciarCartas(
        &porcentagemConfiancaApagado,
        &porcentagemViesApagado,
        &porcentagemPrivacidadeApagado,
        &porcentagemLucroApagado
    );
    reiniciarMaquinaCafe();   // MAQUINA DE CAFE


    // ==========================================================
    // CARTAS
    // ==========================================================
    bool mensagem =
        false;

    bool cartaAbertaPararDeAndar =
        false;

    bool abrirCarta =
        false;


    // ==========================================================
    // PAUSE / GAME OVER
    // ==========================================================
    bool pausado =
        false;

    bool instrucoesPausa =
        false;

    bool gameOver =
        false;

    int k = 0;


    // ==========================================================
    // LOOP
    // ==========================================================

    while (!WindowShouldClose())
    {
        float escalaX =
            (float)GetScreenWidth() /
            larguraResolucao;

        float escalaY =
            (float)GetScreenHeight() /
            alturaResolucao;

        float escala;


        int xGuardado =
            x;

        int yGuardado =
            y;


        if (escalaX < escalaY)
            escala = escalaX;
        else
            escala = escalaY;


        float larguraFinal =
            larguraResolucao *
            escala;

        float alturaFinal =
            alturaResolucao *
            escala;


        float offsetX =
            (
                GetScreenWidth() -
                larguraFinal
            ) /
            2;


        float offsetY =
            (
                GetScreenHeight() -
                alturaFinal
            ) /
            2;


        // ======================================================
        // ESC / PAUSE
        // ======================================================
        if (
            tela == 2 &&
            IsKeyPressed(KEY_ESCAPE)
        )
        {
            if (maquinaCafeAberta())   // MAQUINA DE CAFE: ESC só fecha a máquina
            {
                fecharMaquinaCafe();
            }
            else if (instrucoesPausa)
            {
                instrucoesPausa =
                    false;
            }
            else
            {
                pausado =
                    !pausado;


                if (pausado)
                {
                    PauseMusicStream(
                        trilhaSonora2
                    );

                    StopSound(
                        somDePassos
                    );
                }
                else
                {
                    ResumeMusicStream(
                        trilhaSonora2
                    );
                }
            }
        }


        // ======================================================
        // MENU DE PAUSA
        // ======================================================
        if (
            tela == 2 &&
            pausado
        )
        {
            BeginDrawing();

            ClearBackground(
                BLACK
            );


            Rectangle origemPause =
            {
                0,
                0,

                (float)telaVirtual.texture.width,
                -(float)telaVirtual.texture.height
            };


            Rectangle destinoJogoPause =
            {
                offsetX,
                offsetY,
                larguraFinal,
                alturaFinal
            };


            // ==================================================
            // JOGO CONGELADO
            // ==================================================
            DrawTexturePro(
                telaVirtual.texture,
                origemPause,
                destinoJogoPause,
                (Vector2){0, 0},
                0,
                WHITE
            );


            // Escurece o escritório
            DrawRectangle(
                offsetX,
                offsetY,
                larguraFinal,
                alturaFinal,
                Fade(
                    BLACK,
                    0.45f
                )
            );


            // ==================================================
            // INSTRUÇÕES
            // ==================================================
            if (instrucoesPausa)
            {
                DrawTexturePro(
                    telaInstrucoesPausa,

                    (Rectangle){
                        0,
                        0,
                        telaInstrucoesPausa.width,
                        telaInstrucoesPausa.height
                    },

                    destinoJogoPause,

                    (Vector2){0, 0},

                    0,

                    WHITE
                );


                Rectangle retVoltarInstrucoes =
                {
                    offsetX +
                    995 *
                    escala,

                    offsetY +
                    625 *
                    escala,

                    205 *
                    escala,

                    51 *
                    escala
                };


                Vector2 mouseInstrucoes =
                    GetMousePosition();


                if (
                    IsMouseButtonPressed(
                        MOUSE_LEFT_BUTTON
                    ) &&
                    CheckCollisionPointRec(
                        mouseInstrucoes,
                        retVoltarInstrucoes
                    )
                )
                {
                    instrucoesPausa =
                        false;
                }


                EndDrawing();

                continue;
            }


            // ==================================================
            // PAINEL DE PAUSA
            // ==================================================
            float alturaPainelPausa =
                570.0f *
                escala;


            float escalaPainelPausa =
                alturaPainelPausa /
                (float)telaPausa.height;


            float larguraPainelPausa =
                telaPausa.width *
                escalaPainelPausa;


            // Post-it ocupa espaço à direita da imagem.
            // Corrige o centro visual da máquina.
            float ajustePainelX =
                65.0f *
                escala;


            Rectangle destinoTelaPausa =
            {
                offsetX +
                (
                    larguraFinal -
                    larguraPainelPausa
                ) /
                2 +
                ajustePainelX,

                offsetY +
                (
                    alturaFinal -
                    alturaPainelPausa
                ) /
                2,

                larguraPainelPausa,
                alturaPainelPausa
            };


            DrawTexturePro(
                telaPausa,

                (Rectangle){
                    0,
                    0,
                    telaPausa.width,
                    telaPausa.height
                },

                destinoTelaPausa,

                (Vector2){0, 0},

                0,

                WHITE
            );


            // ==================================================
            // BOTÕES DO PAUSE
            // ==================================================
            Rectangle areaUtilPause =
            {
                30,
                207,
                447,
                112
            };


            // Botões menores
            float larguraBotao =
                165.0f *
                escala;


            float alturaBotao =
                41.0f *
                escala;


            // Todos usam exatamente o mesmo centro
            float centroBotoesX =
                offsetX +
                larguraFinal /
                2;


            // ==================================================
            // CONTINUAR
            // ==================================================
            Rectangle retContinueBase =
            {
                centroBotoesX -
                larguraBotao /
                2,

                offsetY +
                300 *
                escala,

                larguraBotao,
                alturaBotao
            };


            Rectangle retContinue =
                retContinueBase;


            // ==================================================
            // INSTRUÇÕES
            // ==================================================
            Rectangle retInstrucoesBase =
            {
                centroBotoesX -
                larguraBotao /
                2,

                offsetY +
                352 *
                escala,

                larguraBotao,
                alturaBotao
            };


            Rectangle retInstrucoes =
                retInstrucoesBase;


            // ==================================================
            // MENU INICIAL
            // ==================================================
            Rectangle retMenuInicialBase =
            {
                centroBotoesX -
                larguraBotao /
                2,

                offsetY +
                404 *
                escala,

                larguraBotao,
                alturaBotao
            };


            Rectangle retMenuInicial =
                retMenuInicialBase;


            Vector2 mousePause =
                GetMousePosition();


            // ==================================================
            // HOVER CONTINUAR
            // ==================================================
            if (
                CheckCollisionPointRec(
                    mousePause,
                    retContinueBase
                )
            )
            {
                retContinue.width *=
                    1.10f;

                retContinue.height *=
                    1.10f;


                retContinue.x =
                    retContinueBase.x +
                    (
                        retContinueBase.width -
                        retContinue.width
                    ) /
                    2;


                retContinue.y =
                    retContinueBase.y +
                    (
                        retContinueBase.height -
                        retContinue.height
                    ) /
                    2;
            }


            // ==================================================
            // HOVER INSTRUÇÕES
            // ==================================================
            if (
                CheckCollisionPointRec(
                    mousePause,
                    retInstrucoesBase
                )
            )
            {
                retInstrucoes.width *=
                    1.10f;

                retInstrucoes.height *=
                    1.10f;


                retInstrucoes.x =
                    retInstrucoesBase.x +
                    (
                        retInstrucoesBase.width -
                        retInstrucoes.width
                    ) /
                    2;


                retInstrucoes.y =
                    retInstrucoesBase.y +
                    (
                        retInstrucoesBase.height -
                        retInstrucoes.height
                    ) /
                    2;
            }


            // ==================================================
            // HOVER MENU INICIAL
            // ==================================================
            if (
                CheckCollisionPointRec(
                    mousePause,
                    retMenuInicialBase
                )
            )
            {
                retMenuInicial.width *=
                    1.10f;

                retMenuInicial.height *=
                    1.10f;


                retMenuInicial.x =
                    retMenuInicialBase.x +
                    (
                        retMenuInicialBase.width -
                        retMenuInicial.width
                    ) /
                    2;


                retMenuInicial.y =
                    retMenuInicialBase.y +
                    (
                        retMenuInicialBase.height -
                        retMenuInicial.height
                    ) /
                    2;
            }


            // ==================================================
            // DESENHA CONTINUAR
            // ==================================================
            DrawTexturePro(
                botaoContinue,
                areaUtilPause,
                retContinue,
                (Vector2){0, 0},
                0,
                WHITE
            );


            // ==================================================
            // DESENHA INSTRUÇÕES
            // ==================================================
            DrawTexturePro(
                botaoInstrucoesPausa,
                areaUtilPause,
                retInstrucoes,
                (Vector2){0, 0},
                0,
                WHITE
            );


            // ==================================================
            // DESENHA MENU INICIAL
            // ==================================================
            DrawTexturePro(
                botaoMenuInicial,
                areaUtilPause,
                retMenuInicial,
                (Vector2){0, 0},
                0,
                WHITE
            );


            // ==================================================
            // CLIQUES
            // ==================================================
            if (
                IsMouseButtonPressed(
                    MOUSE_LEFT_BUTTON
                )
            )
            {
                // CONTINUAR
                if (
                    CheckCollisionPointRec(
                        mousePause,
                        retContinueBase
                    )
                )
                {
                    pausado =
                        false;


                    ResumeMusicStream(
                        trilhaSonora2
                    );
                }


                // INSTRUÇÕES
                if (
                    CheckCollisionPointRec(
                        mousePause,
                        retInstrucoesBase
                    )
                )
                {
                    instrucoesPausa =
                        true;
                }


                // MENU INICIAL
                if (
                    CheckCollisionPointRec(
                        mousePause,
                        retMenuInicialBase
                    )
                )
                {
                    pausado =
                        false;

                    instrucoesPausa =
                        false;


                    StopMusicStream(
                        trilhaSonora2
                    );


                    PlayMusicStream(
                        trilhaSonora1
                    );


                    tela =
                        0;


                    porcentagemConfiancaApagado =
                        0.365f;

                    porcentagemViesApagado =
                        0.365f;

                    porcentagemPrivacidadeApagado =
                        0.365f;

                    porcentagemLucroApagado =
                        0.365f;


                    x =
                        300;

                    y =
                        170;


                    direcao =
                        0;

                    andando =
                        0;

                    frame =
                        0;

                    tempo =
                        0;


                    cartaAbertaPararDeAndar =
                        false;

                    abrirCarta =
                        false;


                    iniciarCartas(
                        &porcentagemConfiancaApagado,
                        &porcentagemViesApagado,
                        &porcentagemPrivacidadeApagado,
                        &porcentagemLucroApagado
                    );
                    reiniciarMaquinaCafe();   // MAQUINA DE CAFE
                }
            }


            EndDrawing();

            continue;
        }


        // ======================================================
        // INTRO
        // ======================================================
        if (tela == -1)
        {
            tempoIntro +=
                GetFrameTime();


            float alpha =
                0.0f;


            if (
                tempoIntro >=
                tempoLogo
            )
            {
                alpha =
                    (
                        tempoIntro -
                        tempoLogo
                    ) /
                    tempoFade;


                if (alpha > 1.0f)
                    alpha =
                        1.0f;
            }


            BeginTextureMode(
                telaVirtual
            );


            ClearBackground(
                BLACK
            );


            float escalaLogoX =
                (
                    larguraResolucao *
                    0.55f
                ) /
                logoInicio.width;


            float escalaLogoY =
                (
                    alturaResolucao *
                    0.75f
                ) /
                logoInicio.height;


            float escalaLogo;


            if (
                escalaLogoX <
                escalaLogoY
            )
            {
                escalaLogo =
                    escalaLogoX;
            }
            else
            {
                escalaLogo =
                    escalaLogoY;
            }


            float larguraLogo =
                logoInicio.width *
                escalaLogo;


            float alturaLogo =
                logoInicio.height *
                escalaLogo;


            DrawTextureEx(
                logoInicio,

                (Vector2){
                    (
                        larguraResolucao -
                        larguraLogo
                    ) /
                    2,

                    (
                        alturaResolucao -
                        alturaLogo
                    ) /
                    2
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
                Fade(
                    BLACK,
                    alpha
                )
            );


            EndTextureMode();


            BeginDrawing();

            ClearBackground(
                BLACK
            );


            Rectangle origemIntro =
            {
                0,
                0,

                (float)telaVirtual.texture.width,
                -(float)telaVirtual.texture.height
            };


            Rectangle destinoIntro =
            {
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


            if (
                tempoIntro >=
                tempoLogo +
                tempoFade
            )
            {
                tela =
                    0;
            }


            continue;
        }


        // ======================================================
        // MENU INICIAL
        // ======================================================
        if (tela == 0)
        {
            UpdateMusicStream(
                trilhaSonora1
            );


            int resultadoMenu =
                telaInicio(
                    botaoJogar,
                    telaVirtual,
                    escala,
                    offsetX,
                    offsetY,
                    trilhaSonora1
                );


            if (
                resultadoMenu ==
                -1
            )
            {
                break;
            }


            if (
                resultadoMenu ==
                1
            )
            {
                tela =
                    1;


                PlayMusicStream(
                    EscritorioTrilhaSonora
                );


                StopMusicStream(
                    trilhaSonora1
                );
            }


            continue;
        }


        // ======================================================
        // AVISO
        // ======================================================
        if (tela == 1)
        {
            UpdateMusicStream(
                EscritorioTrilhaSonora
            );


            if (
                telaAviso(
                    telaVirtual,
                    escala,
                    offsetX,
                    offsetY
                ) ==
                2
            )
            {
                tela =
                    2;


                StopMusicStream(
                    EscritorioTrilhaSonora
                );


                PlayMusicStream(
                    trilhaSonora2
                );
            }


            continue;
        }


        // ======================================================
        // JOGO
        // ======================================================
        if (tela == 2)
        {
            UpdateMusicStream(
                trilhaSonora2
            );
        }


        // ======================================================
        // MOVIMENTAÇÃO
        // ======================================================
        andando =
            0;


        Texture2D texturaAtual =
            texturaAtualGuardada;


        bool botaoAeDapertados =
            IsKeyDown(KEY_D) &&
            IsKeyDown(KEY_A);


        bool botaoWeSapertados =
            IsKeyDown(KEY_W) &&
            IsKeyDown(KEY_S);


        if (
            !cartaAbertaPararDeAndar && !maquinaCafeAberta() &&
            !gameOver
        )
        {
            if (
                !botaoAeDapertados &&
                !botaoWeSapertados
            )
            {
                if (
                    IsKeyDown(KEY_W)
                )
                {
                    if (
                        !IsSoundPlaying(
                            somDePassos
                        )
                    )
                    {
                        PlaySound(
                            somDePassos
                        );
                    }


                    y -=
                        velocidade;


                    direcao =
                        1;


                    andando =
                        1;


                    if (
                        IsKeyPressed(
                            KEY_W
                        )
                    )
                    {
                        frame =
                            1;
                    }
                }


                if (
                    IsKeyDown(KEY_S)
                )
                {
                    if (
                        !IsSoundPlaying(
                            somDePassos
                        )
                    )
                    {
                        PlaySound(
                            somDePassos
                        );
                    }


                    y +=
                        velocidade;


                    direcao =
                        0;


                    andando =
                        1;


                    if (
                        IsKeyPressed(
                            KEY_S
                        )
                    )
                    {
                        frame =
                            1;
                    }
                }


                if (
                    IsKeyDown(KEY_A)
                )
                {
                    if (
                        !IsSoundPlaying(
                            somDePassos
                        )
                    )
                    {
                        PlaySound(
                            somDePassos
                        );
                    }


                    x -=
                        velocidade;


                    direcao =
                        2;


                    andando =
                        1;


                    if (
                        IsKeyPressed(
                            KEY_A
                        )
                    )
                    {
                        frame =
                            1;
                    }
                }


                if (
                    IsKeyDown(KEY_D)
                )
                {
                    texturaAtual =
                        direita2;


                    if (
                        !IsSoundPlaying(
                            somDePassos
                        )
                    )
                    {
                        PlaySound(
                            somDePassos
                        );
                    }


                    x +=
                        velocidade;


                    direcao =
                        3;


                    andando =
                        1;


                    if (
                        IsKeyPressed(
                            KEY_D
                        )
                    )
                    {
                        frame =
                            1;
                    }
                }
            }
            else
            {
                andando =
                    0;


                if (
                    texturaAtualGuardada.id ==
                    frente2.id ||
                    texturaAtualGuardada.id ==
                    frente3.id
                )
                {
                    texturaAtual =
                        frente1;
                }


                if (
                    texturaAtualGuardada.id ==
                    costas2.id ||
                    texturaAtualGuardada.id ==
                    costas3.id
                )
                {
                    texturaAtual =
                        costas1;
                }


                if (
                    texturaAtualGuardada.id ==
                    direita2.id ||
                    texturaAtualGuardada.id ==
                    direita3.id
                )
                {
                    texturaAtual =
                        direita1;
                }


                if (
                    texturaAtualGuardada.id ==
                    esquerda2.id ||
                    texturaAtualGuardada.id ==
                    esquerda3.id
                )
                {
                    texturaAtual =
                        esquerda1;
                }
            }
        }
        else
        {
            andando =
                0;


            StopSound(
                somDePassos
            );


            if (direcao == 0)
                texturaAtual =
                    frente1;

            else if (direcao == 1)
                texturaAtual =
                    costas1;

            else if (direcao == 2)
                texturaAtual =
                    esquerda1;

            else
                texturaAtual =
                    direita1;
        }


        // ======================================================
        // ANIMAÇÃO
        // ======================================================
        if (
            andando ==
            1
        )
        {
            tempo +=
                GetFrameTime();


            if (
                tempo >=
                0.12
            )
            {
                frame++;

                tempo =
                    0;


                if (
                    frame >=
                    3
                )
                {
                    frame =
                        0;
                }
            }
        }
        else
        {
            frame =
                0;

            tempo =
                0;
        }


        if (
            !cartaAbertaPararDeAndar && !maquinaCafeAberta() &&
            !botaoAeDapertados &&
            !botaoWeSapertados
        )
        {
            if (
                direcao ==
                0
            )
            {
                if (frame == 0)
                    texturaAtual =
                        frente1;

                else if (frame == 1)
                    texturaAtual =
                        frente2;

                else
                    texturaAtual =
                        frente3;
            }

            else if (
                direcao ==
                1
            )
            {
                if (frame == 0)
                    texturaAtual =
                        costas1;

                else if (frame == 1)
                    texturaAtual =
                        costas2;

                else
                    texturaAtual =
                        costas3;
            }

            else if (
                direcao ==
                2
            )
            {
                if (frame == 0)
                    texturaAtual =
                        esquerda1;

                else if (frame == 1)
                    texturaAtual =
                        esquerda2;

                else
                    texturaAtual =
                        esquerda3;
            }

            else
            {
                if (frame == 0)
                    texturaAtual =
                        direita1;

                else if (frame == 1)
                    texturaAtual =
                        direita2;

                else
                    texturaAtual =
                        direita3;
            }
        }


        // ======================================================
        // LIMITES
        // ======================================================
        if (x < 15)
            x =
                15;


        if (
            x >
            larguraResolucao -
            texturaAtual.width -
            15
        )
        {
            x =
                larguraResolucao -
                texturaAtual.width -
                15;
        }


        if (y < 80)
            y =
                80;


        if (
            y >
            alturaResolucao -
            texturaAtual.height -
            20
        )
        {
            y =
                alturaResolucao -
                texturaAtual.height -
                20;
        }


        if (
            x >
            larguraResolucao -
            texturaAtual.width
        )
        {
            x =
                larguraResolucao -
                texturaAtual.width;
        }


        if (
            y >
            alturaResolucao -
            texturaAtual.height
        )
        {
            y =
                alturaResolucao -
                texturaAtual.height;
        }


        // ======================================================
        // DESENHO
        // ======================================================
        BeginTextureMode(
            telaVirtual
        );


        ClearBackground(
            RAYWHITE
        );


        DrawTexture(
            fundoCeramica,
            0,
            0,
            WHITE
        );


        // ======================================================
        // COLISÃO
        // ======================================================
        Rectangle colisaoMesaBoneco_Boneco =
        {
            x,
            y,
            frente1.width,
            frente1.height
        };


        for (
            int i = 0;
            i < quantidadeObjetos;
            i++
        )
        {
            if (
                CheckCollisionRecs(
                    colisaoMesaBoneco_Boneco,
                    objetos[i].colisao
                )
            )
            {
                mensagem =
                    true;


                x =
                    xGuardado;


                y =
                    yGuardado;


                break;
            }
        }


        // ======================================================
        // INTERAÇÃO
        // ======================================================
        int objetoPerto =
            -1;


        for (
            int i = 0;
            i < quantidadeObjetos &&
            objetoPerto < 0;
            i++
        )
        {
            Rectangle areaInteracao =
            {
                objetos[i].colisao.x -
                30,

                objetos[i].colisao.y -
                30,

                objetos[i].colisao.width +
                60,

                objetos[i].colisao.height +
                60
            };


            if (
                objetos[i].interativo &&
                CheckCollisionRecs(
                    colisaoMesaBoneco_Boneco,
                    areaInteracao
                )
            )
            {
                objetoPerto =
                    i;
            }
        }


        if (objetoPerto == 1)   // MAQUINA DE CAFE: balcão do café
        {
            if (!gameOver && !maquinaCafeAberta())
                interagirBalcaoCafe(larguraResolucao);
        }
        else if (
            objetoPerto >=
            0
        )
        {
            definirOrigemCartas(
                objetos[
                    objetoPerto
                ].origemCartas
            );


            DrawText(
                "Pressione E para realizar a tarefa",
                larguraResolucao -
                400,
                75,
                20,
                GREEN
            );


            bool cartaEstavaAberta =
                cartasEstaoAbertas();


            if (!gameOver)
            {
                atualizarCartas();
            }


            bool cartaAgoraAberta =
                cartasEstaoAbertas();


            cartaAbertaPararDeAndar =
                cartaEstavaAberta ||
                cartaAgoraAberta;
        }


        // ======================================================
        // OBJETOS
        // ======================================================
// ======================================================
// OBJETOS
// ======================================================

 tempoAnimacaoNpc += GetFrameTime();

        if (tempoAnimacaoNpc >= 0.25f)
        {
            tempoAnimacaoNpc = 0.0f;

            frameNpc++;

            if (frameNpc >= 3)
            {
                frameNpc = 0;
            }
        }

        tempoNovosNpcs += GetFrameTime();
        tempoBolhas += GetFrameTime();

        if (tempoNovosNpcs >= 0.25f)
        {
            tempoNovosNpcs = 0.0f;
            passoNovosNpcs = (passoNovosNpcs + 1) % 16;
        }

        for (int i = 0; i < quantidadeObjetos; i++)
        {

            if (i == 14 || i == 15)
            {
                continue;
            }


            if (i == 2)   // bebedouro com bolhas
            {
                int quadroBolha = 0;
                if (tempoBolhas >= esperaBolhas)
                {
                    quadroBolha = 1 + (int)((tempoBolhas - esperaBolhas) / 0.15f);   // cada quadro dura 0,15s
                    if (quadroBolha > 4)
                    {
                        quadroBolha = 0;
                        tempoBolhas = 0.0f;
                        esperaBolhas = GetRandomValue(20, 50) / 10.0f;   // próxima bolha entre 2 e 5s
                    }
                }
                DrawTexture(bebedouroBolhas[quadroBolha], objetos[2].x, objetos[2].y, WHITE);
            }
            else if (i == 12)   // mulher digitando
            {
                DrawTexture(npcMulher[sequenciaMulher[passoNovosNpcs % 4]],
                            objetos[12].x, objetos[12].y, WHITE);
            }
            else if (i == 7)   // NPC da frente (o PNG dele é deslocado meio pixel da arte pra direita/baixo)
            {
                DrawTextureV(npcFrente[sequenciaFrente[passoNovosNpcs % 16]],
                             (Vector2){objetos[7].x - 2.5f, objetos[7].y - 2.5f}, WHITE);
            }
            else if (i == 13)
            {
                DrawTexture(
                    objetos[13 + frameNpc].textura,
                    objetos[13].x,
                    objetos[13].y,
                    WHITE
                );
            }
            else
            {
                DrawTexture(
                    objetos[i].textura,
                    objetos[i].x,
                    objetos[i].y,
                    WHITE
                );
            }
        }

        desenharAvisoBalcaoCafe();   // MAQUINA DE CAFE: "!" em cima do balcão

        // ======================================================
        // PERSONAGEM
        // ======================================================
        DrawTexture(
            texturaAtual,
            x,
            y,
            WHITE
        );


        // ======================================================
        // CARTAS
        // ======================================================
        desenharCartas(
            larguraResolucao,
            alturaResolucao
        );


        if (
            IsKeyPressed(
                KEY_P
            )
        )
        {
            abrirCarta =
                !abrirCarta;
        }


        if (abrirCarta)
        {
            DrawTexture(
                fundoCarta,
                0,
                0,
                WHITE
            );
        }


        // ======================================================
        // HUD - CONFIANÇA
        // ======================================================
        DrawTexture(
            fundoHUD,

            (
                (
                    larguraResolucao -
                    confianca.width
                ) /
                2
            ) -
            confianca.width -
            145,

            privacidade.height *
            0.05,

            WHITE
        );


        DrawTexture(
            confianca,

            (
                (
                    larguraResolucao -
                    confianca.width
                ) /
                2
            ) -
            confianca.width -
            135,

            privacidade.height *
            0.05,

            WHITE
        );


        BeginScissorMode(
            (
                (
                    larguraResolucao -
                    confianca.width
                ) /
                2
            ) -
            confianca.width -
            135,

            privacidade.height *
            0.05,

            confiancaCheia.width *
            (
                1.0f -
                porcentagemConfiancaApagado
            ),

            confiancaCheia.height
        );


        DrawTexture(
            confiancaCheia,

            (
                (
                    larguraResolucao -
                    confianca.width
                ) /
                2
            ) -
            confianca.width -
            135,

            privacidade.height *
            0.05,

            WHITE
        );


        EndScissorMode();


        // ======================================================
        // HUD - VIÉS
        // ======================================================
        DrawTexture(
            fundoHUD,

            (
                (
                    larguraResolucao -
                    vies.width
                ) /
                2
            ) -
            125,

            privacidade.height *
            0.05,

            WHITE
        );


        DrawTexture(
            vies,

            (
                (
                    larguraResolucao -
                    vies.width
                ) /
                2
            ) -
            120,

            privacidade.height *
            0.05,

            WHITE
        );


        BeginScissorMode(
            (
                (
                    larguraResolucao -
                    vies.width
                ) /
                2
            ) -
            120,

            privacidade.height *
            0.05,

            viesCheio.width *
            (
                1.0f -
                porcentagemViesApagado
            ),

            viesCheio.height
        );


        DrawTexture(
            viesCheio,

            (
                (
                    larguraResolucao -
                    vies.width
                ) /
                2
            ) -
            120,

            privacidade.height *
            0.05,

            WHITE
        );


        EndScissorMode();


        // ======================================================
        // HUD - PRIVACIDADE
        // ======================================================
        DrawTexture(
            fundoHUD,

            (
                (
                    larguraResolucao -
                    privacidade.width
                ) /
                2
            ) +
            lucro.width -
            115,

            privacidade.height *
            0.05,

            WHITE
        );


        DrawTexture(
            privacidade,

            (
                (
                    larguraResolucao -
                    privacidade.width
                ) /
                2
            ) +
            lucro.width -
            105,

            privacidade.height *
            0.05,

            WHITE
        );


        BeginScissorMode(
            (
                (
                    larguraResolucao -
                    privacidade.width
                ) /
                2
            ) +
            lucro.width -
            105,

            privacidade.height *
            0.05,

            privacidadeCheia.width *
            (
                1.0f -
                porcentagemPrivacidadeApagado
            ),

            privacidadeCheia.height
        );


        DrawTexture(
            privacidadeCheia,

            (
                (
                    larguraResolucao -
                    privacidade.width
                ) /
                2
            ) +
            lucro.width -
            105,

            privacidade.height *
            0.05,

            WHITE
        );


        EndScissorMode();


        // ======================================================
        // HUD - LUCRO
        // ======================================================
        
        DrawTexture(
            fundoHUD,

            (
                (
                    larguraResolucao -
                    lucro.width
                ) /
                2
            ) +
            2 *
            lucro.width -
            80,

            privacidade.height *
            0.05,

            WHITE
        );

        
        DrawTexture(
            lucro,

            (
                (
                    larguraResolucao -
                    lucro.width
                ) /
                2
            ) +
            2 *
            lucro.width -
            75,

            privacidade.height *
            0.05,

            WHITE
        );


        BeginScissorMode(
            (
                (
                    larguraResolucao -
                    lucro.width
                ) /
                2
            ) +
            2 *
            lucro.width -
            75,

            privacidade.height *
            0.05,

            lucroCheio.width *
            (
                1.0f -
                porcentagemLucroApagado
            ),

            lucroCheio.height
        );

        
        DrawTexture(
            lucroCheio,

            (
                (
                    larguraResolucao -
                    lucro.width
                ) /
                2
            ) +
            2 *
            lucro.width -
            75,

            privacidade.height *
            0.05,

            WHITE
        );


        EndScissorMode();


        texturaAtualGuardada =
            texturaAtual;

        DrawTexture(telaContadorDeDias,larguraResolucao-telaContadorDeDias.width,0,WHITE); //Monitor com a contagem de dias
        {
            // estamos dentro da tela virtual 1280x720: NAO multiplicar por escala
            const char *textoDias = TextFormat("Dia: %.2d", obterDiasSobrevividos());
            int tamanhoDias = 20;
            // a telinha do monitor ocupa x 8..55 e y 8..39 do PNG de 64x50
            int telaX = larguraResolucao - telaContadorDeDias.width + 8 * telaContadorDeDias.width / 64;
            int telaL = 48 * telaContadorDeDias.width / 64;
            int telaY = 8 * telaContadorDeDias.height / 50;
            int telaA = 32 * telaContadorDeDias.height / 50;
            DrawText(textoDias,
                     telaX + (telaL - MeasureText(textoDias, tamanhoDias)) / 2,
                     telaY + (telaA - tamanhoDias) / 2,
                     tamanhoDias, (Color){198, 183, 255, 255});
        }

        // MAQUINA DE CAFE: se estiver aberta, desenha por cima de tudo
        atualizarDesenharMaquinaCafe(escala, offsetX, offsetY);

        EndTextureMode();

        


        // ======================================================
        // TELA VIRTUAL -> JANELA
        // ======================================================
        BeginDrawing();


        ClearBackground(
            BLACK
        );


        Rectangle origem =
        {
            0,
            0,

            (float)telaVirtual.texture.width,
            -(float)telaVirtual.texture.height
        };


        Rectangle destino =
        {
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


        // ======================================================
        // GAME OVER
        // ======================================================
        if (
            regraGameOver()
        )
        {
            if (
                k ==
                0
            )
            {
                PlayMusicStream(
                    gameOverSom
                );

                k =
                    1;
            }


            gameOver =
                true;


            UpdateMusicStream(
                gameOverSom
            );


            DrawRectangle(
                0,
                0,
                GetScreenWidth(),
                GetScreenHeight(),
                Fade(
                    BLACK,
                    0.65f
                )
            );


            DrawTextureEx(
                painelFimExpediente,

                (Vector2){
                    offsetX +
                    190 *
                    escala,

                    offsetY +
                    80 *
                    escala
                },

                0,

                escala,

                WHITE
            );


            const char *cargoFinal =
                TextToUpper(
                    nomeCargo()
                );


            int tamanhoCargo =
                20 *
                escala;


            DrawText(
                cargoFinal,

                offsetX +
                347 *
                escala -
                MeasureText(
                    cargoFinal,
                    tamanhoCargo
                ) /
                2,

                offsetY +
                432 *
                escala -
                tamanhoCargo /
                2,

                tamanhoCargo,

                (Color){
                    70,
                    74,
                    90,
                    255
                }
            );


            desenharTextoQuebrado(
                obterMensagemFinal(),

                offsetX +
                520 *
                escala,

                offsetY +
                230 *
                escala,

                530 *
                escala,

                24 *
                escala,

                WHITE
            );


            const char *textoDias =
                TextFormat(
                    "%d",
                    obterDiasSobrevividos()
                );


            int tamanhoDias =
                28 *
                escala;


            DrawText(
                textoDias,

                offsetX +
                735 *
                escala -
                MeasureText(
                    textoDias,
                    tamanhoDias
                ) /
                2,

                offsetY +
                400 *
                escala -
                tamanhoDias /
                2,

                tamanhoDias,

                (Color){
                    255,
                    214,
                    102,
                    255
                }
            );


            Rectangle areaUtil =
            {
                30,
                207,
                447,
                112
            };


            Rectangle retPlayAgain =
            {
                offsetX +
                266 *
                escala,

                offsetY +
                550 *
                escala,

                223.5f *
                escala,

                56 *
                escala
            };


            Rectangle retShare =
            {
                offsetX +
                529 *
                escala,

                offsetY +
                550 *
                escala,

                223.5f *
                escala,

                56 *
                escala
            };


            Rectangle retExit =
            {
                offsetX +
                792 *
                escala,

                offsetY +
                550 *
                escala,

                223.5f *
                escala,

                56 *
                escala
            };


            DrawTexturePro(
                botaoPlayAgain,
                areaUtil,
                retPlayAgain,
                (Vector2){0, 0},
                0,
                WHITE
            );


            DrawTexturePro(
                botaoShare,
                areaUtil,
                retShare,
                (Vector2){0, 0},
                0,
                WHITE
            );


            DrawTexturePro(
                botaoExit,
                areaUtil,
                retExit,
                (Vector2){0, 0},
                0,
                WHITE
            );


            PauseMusicStream(
                trilhaSonora2
            );


            Vector2 mouse =
                GetMousePosition();


            if (
                IsMouseButtonPressed(
                    MOUSE_LEFT_BUTTON
                )
            )
            {
                if (
                    CheckCollisionPointRec(
                        mouse,
                        retShare
                    )
                )
                {
                    rlDrawRenderBatchActive();


                    TakeScreenshot(
                        TextFormat(
                            "else_resultado_%ddias.png",
                            obterDiasSobrevividos()
                        )
                    );
                }


                if (
    CheckCollisionPointRec(
        mouse,
        retExit
    )
)
{
    PlayMusicStream(
        trilhaSonora1
    );

    k = 0;

    tela = -1;

    gameOver = false;

    porcentagemConfiancaApagado = 0.365f;
    porcentagemViesApagado = 0.365f;
    porcentagemPrivacidadeApagado = 0.365f;
    porcentagemLucroApagado = 0.365f;

    x = 300;
    y = 170;

    direcao = 0;
    andando = 0;
    frame = 0;
    tempo = 0;

    cartaAbertaPararDeAndar = false;
    abrirCarta = false;

    iniciarCartas(
        &porcentagemConfiancaApagado,
        &porcentagemViesApagado,
        &porcentagemPrivacidadeApagado,
        &porcentagemLucroApagado
    );
    reiniciarMaquinaCafe();   // MAQUINA DE CAFE
}


                if (
    CheckCollisionPointRec(
        mouse,
        retPlayAgain
    )
)
{
    k = 0;

    PlayMusicStream(
        EscritorioTrilhaSonora
    );

    gameOver = false;

    tela = 1;

    porcentagemConfiancaApagado = 0.365f;
    porcentagemViesApagado = 0.365f;
    porcentagemPrivacidadeApagado = 0.365f;
    porcentagemLucroApagado = 0.365f;

    x = 300;
    y = 170;

    direcao = 0;
    andando = 0;
    frame = 0;
    tempo = 0;

    cartaAbertaPararDeAndar = false;
    abrirCarta = false;

    iniciarCartas(
        &porcentagemConfiancaApagado,
        &porcentagemViesApagado,
        &porcentagemPrivacidadeApagado,
        &porcentagemLucroApagado
    );
    reiniciarMaquinaCafe();   // MAQUINA DE CAFE
}
            }
        // reiniciarPartidaSeNecessario();
        }


        EndDrawing();
    }


    // ==========================================================
    // FINALIZAÇÃO
    // ==========================================================
    descarregarCartas();


    for (
        int i = 0;
        i < quantidadeObjetos;
        i++
    )
    {
        UnloadTexture(
            objetos[i].textura
        );
    }

    // novos NPCs animados e bebedouro
    for (int k = 0; k < 3; k++) UnloadTexture(npcMulher[k]);
    for (int k = 0; k < 2; k++) UnloadTexture(npcFrente[k]);
    for (int k = 0; k < 5; k++) UnloadTexture(bebedouroBolhas[k]);
    descarregarMaquinaCafe();   // MAQUINA DE CAFE


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
    UnloadTexture(botaoExit);
    UnloadTexture(botaoPlayAgain);
    UnloadTexture(botaoShare);

    UnloadTexture(botaoContinue);
    UnloadTexture(botaoInstrucoesPausa);
    UnloadTexture(botaoMenuInicial);

    UnloadTexture(telaPausa);
    UnloadTexture(telaInstrucoesPausa);

    UnloadTexture(painelFimExpediente);

    UnloadTexture(logoInicio);
    UnloadTexture(fundoCarta);
    UnloadTexture(fundoCeramica);

    UnloadTexture(confianca);
    UnloadTexture(lucro);
    UnloadTexture(privacidade);
    UnloadTexture(vies);

    UnloadTexture(confiancaCheia);
    UnloadTexture(lucroCheio);
    UnloadTexture(privacidadeCheia);
    UnloadTexture(viesCheio);

    UnloadTexture(fundoHUD);


    UnloadMusicStream(
        trilhaSonora1
    );

    UnloadMusicStream(
        trilhaSonora2
    );

    UnloadMusicStream(
        EscritorioTrilhaSonora
    );

    UnloadMusicStream(
        gameOverSom
    );


    UnloadSound(
        somDePassos
    );


    UnloadRenderTexture(
        telaVirtual
    );


    CloseAudioDevice();

    CloseWindow();


    return 0;
}