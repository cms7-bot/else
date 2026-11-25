#include "raylib.h"

// ==========================================================
// SAVE DOS FINAIS (arquivo finais.sav na pasta do jogo)
// ==========================================================
// 1 = final ja visto (pasta aberta), 0 = ainda bloqueado
static int finalLiberado[8] = {0, 0, 0, 0, 0, 0, 0, 0};

// Le o arquivo e preenche finalLiberado. Se o arquivo nao existe, fica tudo 0.
void carregarFinais(void)
{
    if (!FileExists("finais.sav"))
        return;

    int tamanho = 0;
    unsigned char *dados = LoadFileData("finais.sav", &tamanho);

    for (int i = 0; i < 8 && i < tamanho; i++)
        finalLiberado[i] = dados[i];

    UnloadFileData(dados);
}

// Chamada no game over: numero vai de 1 a 8 (mesma ordem das pastas)
void desbloquearFinal(int numero)
{
    if (numero < 1 || numero > 8)
        return;

    carregarFinais();
    finalLiberado[numero - 1] = 1;

    unsigned char dados[8];
    for (int i = 0; i < 8; i++)
        dados[i] = finalLiberado[i];

    SaveFileData("finais.sav", dados, 8);
}

// animacao padrao dos botoes (Cartas.c)
float escalaAnimadaBotao(const char *nome, bool hover);
Rectangle retanguloAnimadoBotao(const char *nome, Rectangle base, bool hover);

#define FONTE_TEXTO 0
#define FONTE_DESTAQUE 1
float medirTextoUI(const char *texto, float tamanho, int fonte);
void desenharTextoUI(const char *texto, float x, float y, float tamanho, Color cor, int fonte);
void abrirRankingMenu(void);
bool desenharRankingMenu(float escala, float offsetX, float offsetY);

int telaInicio(Texture2D botaoJogar, RenderTexture2D telaVirtual, float escalaTela, float offsetX, float offsetY, Music trilhaSonora1)
{
    // ==========================================================
    // TEXTURAS / ESTADO (persistem entre as chamadas)
    // ==========================================================
    static Texture2D fundoInicio = {0};
    static Texture2D botaoSom = {0};
    static Texture2D botaoMute = {0};
    static Texture2D botaoInstrucoes = {0};
    static Texture2D botaoFinais = {0};
    static Texture2D botaoRanking = {0};
    static Texture2D botaoExit = {0};
    static Texture2D telaInstrucoes = {0};
    static Texture2D botaoVoltar = {0};
    static Texture2D telaFinais = {0};
    static Texture2D pastaAberta = {0};
    static Texture2D arteFinal[8] = {0};
    static Texture2D pastaFechada = {0};
    static Texture2D fita = {0};

    extern int somMutadoGlobal;   // definido em Movimentacao.c
    static int telaMenu = 0;

    if (fundoInicio.id == 0)
        fundoInicio = LoadTexture("cenario/TelaInicialFundo4.png");

    if (botaoSom.id == 0)
        botaoSom = LoadTexture("Botoes/Som.png");

    if (botaoMute.id == 0)
        botaoMute = LoadTexture("Botoes/Mute.png");

    if (botaoInstrucoes.id == 0)
        botaoInstrucoes = LoadTexture("Botoes/botaoInstrucoes.png");

    if (botaoFinais.id == 0)
        botaoFinais = LoadTexture("Botoes/botaoFinais.png");

    if (botaoRanking.id == 0)
        botaoRanking = LoadTexture("Botoes/botaoRanking.png");

    if (botaoExit.id == 0)
        botaoExit = LoadTexture("Botoes/botaoExit.png");

    if (telaInstrucoes.id == 0)
        telaInstrucoes = LoadTexture("cenario/Telas/telaInstrucoes.png");

   if (botaoVoltar.id == 0)
    botaoVoltar = LoadTexture("Botoes/botaoVoltar.png");

    if (telaFinais.id == 0)
        telaFinais = LoadTexture("cenario/Telas/telaFinais.png");

    if (pastaAberta.id == 0)
        pastaAberta = LoadTexture("cenario/Telas/Finais/pastaAberta.png");

    if (pastaFechada.id == 0)
        pastaFechada = LoadTexture("cenario/Telas/Finais/pastaFechada.png");

    if (fita.id == 0)
        fita = LoadTexture("cenario/Telas/Finais/fita.png");

    for (int i = 0; i < 8; i++)
    {
        if (arteFinal[i].id == 0)
            arteFinal[i] = LoadTexture(TextFormat("cenario/Telas/Finais/final%d.png", i + 1));
    }


    // ==========================================================
    // RESOLUÇÃO
    // ==========================================================
    int larguraResolucao = 1280;
    int alturaResolucao = 720;


    // ==========================================================
    // MOUSE
    // ==========================================================
    Vector2 mouse = GetMousePosition();

    mouse.x = (mouse.x - offsetX) / escalaTela;
    mouse.y = (mouse.y - offsetY) / escalaTela;


    // ==========================================================
    // TELA DE INSTRUÇÕES
    // ==========================================================
    if (telaMenu == 1)
    {
        Rectangle areaUtilVoltar = {
            30,
            207,
            447,
            112
        };

        Rectangle botaoVoltarArea = {
            995,
            625,
            205,
            51
        };

        bool sobreVoltar = CheckCollisionPointRec(mouse, botaoVoltarArea);
        Rectangle botaoVoltarDesenho = retanguloAnimadoBotao("voltar", botaoVoltarArea, sobreVoltar);

        if (sobreVoltar && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            telaMenu = 0;

        if (IsKeyPressed(KEY_ESCAPE))
            telaMenu = 0;


        // ======================================================
        // DESENHO DA TELA DE INSTRUÇÕES
        // ======================================================
        BeginTextureMode(telaVirtual);

        ClearBackground(BLACK);

        DrawTexturePro(
            telaInstrucoes,
            (Rectangle){
                0,
                0,
                telaInstrucoes.width,
                telaInstrucoes.height
            },
            (Rectangle){
                0,
                0,
                larguraResolucao,
                alturaResolucao
            },
            (Vector2){0, 0},
            0,
            WHITE
        );

        DrawTexturePro(
            botaoVoltar,
            areaUtilVoltar,
            botaoVoltarDesenho,
            (Vector2){0, 0},
            0,
            WHITE
        );

        EndTextureMode();


        // ======================================================
        // TELA VIRTUAL -> JANELA
        // ======================================================
        BeginDrawing();

        ClearBackground(BLACK);

        Rectangle origemInstrucoes = {
            0,
            0,
            (float)telaVirtual.texture.width,
            -(float)telaVirtual.texture.height
        };

        Rectangle destinoInstrucoes = {
            offsetX,
            offsetY,
            larguraResolucao * escalaTela,
            alturaResolucao * escalaTela
        };

        DrawTexturePro(
            telaVirtual.texture,
            origemInstrucoes,
            destinoInstrucoes,
            (Vector2){0, 0},
            0,
            WHITE
        );

        EndDrawing();

        return 0;
    }

 // ======================================================
  // TELA FINAIS
// ======================================================

      if (telaMenu == 2)
    {
        Rectangle areaUtilVoltar = {
            30,
            207,
            447,
            112
        };

        Rectangle botaoVoltarArea = {
            995,
            625,
            205,
            51
        };

        bool sobreVoltar = CheckCollisionPointRec(mouse, botaoVoltarArea);
        Rectangle botaoVoltarDesenho = retanguloAnimadoBotao("voltar", botaoVoltarArea, sobreVoltar);

        if (sobreVoltar && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            telaMenu = 0;

        if (IsKeyPressed(KEY_ESCAPE))
            telaMenu = 0;


        // ======================================================
        // DESENHO DA TELA DE INSTRUÇÕES
        // ======================================================
        BeginTextureMode(telaVirtual);

        ClearBackground(BLACK);

        DrawTexturePro(
            telaFinais,
            (Rectangle){
                0,
                0,
                 telaFinais.width,
                 telaFinais.height
            },
            (Rectangle){
                0,
                0,
                larguraResolucao,
                alturaResolucao
            },
            (Vector2){0, 0},
            0,
            WHITE
        );

        // 8 pastas em grade: 4 por linha, 2 linhas
        for (int i = 0; i < 8; i++)
        {
            int coluna = i % 4;   // 0, 1, 2, 3, 0, 1, 2, 3
            int linha  = i / 4;   // 0, 0, 0, 0, 1, 1, 1, 1

            int x = 72  + coluna * 290;
            int y = 108 + linha  * 259;

            if (finalLiberado[i] == 1)
            {
                // ja visto: pasta aberta com a arte normal
                DrawTexture(pastaAberta, x, y, WHITE);
                DrawTexture(arteFinal[i], x + 37, y + 39, WHITE);
            }
            else
            {
                // bloqueado: pasta fechada com fita, sem mostrar a arte (sem spoiler)
                DrawTexture(pastaFechada, x, y, WHITE);
                DrawTexture(fita, x, y, WHITE);
            }
        }

        DrawTexturePro(
            botaoVoltar,
            areaUtilVoltar,
            botaoVoltarDesenho,
            (Vector2){0, 0},
            0,
            WHITE
        );

        EndTextureMode();


        // ======================================================
        // TELA VIRTUAL -> JANELA
        // ======================================================
        BeginDrawing();

        ClearBackground(BLACK);

        Rectangle origemInstrucoes = {
            0,
            0,
            (float)telaVirtual.texture.width,
            -(float)telaVirtual.texture.height
        };

        Rectangle destinoInstrucoes = {
            offsetX,
            offsetY,
            larguraResolucao * escalaTela,
            alturaResolucao * escalaTela
        };

        DrawTexturePro(
            telaVirtual.texture,
            origemInstrucoes,
            destinoInstrucoes,
            (Vector2){0, 0},
            0,
            WHITE
        );

        EndDrawing();

        return 0;
    }

    if (telaMenu == 3)
    {
        BeginDrawing();

        ClearBackground(BLACK);

        if (desenharRankingMenu(escalaTela, offsetX, offsetY))
            telaMenu = 0;

        EndDrawing();

        return 0;
    }

    // ==========================================================
    // BOTÕES (escala e áreas de clique)
    // ==========================================================
    float escala = 0.60f;
    float escalaInstrucoes = 0.60f;
    float escalaFinais = 0.60f;
    float escalaRanking = 0.60f;
    float escalaExit = 0.60f;

    Rectangle botao = {
        (larguraResolucao - 270) / 2,
        250,
        270,
        67
    };

    Rectangle botaoInstrucoesArea = {
        (larguraResolucao - 270) / 2,
        335,
        270,
        67
    };

    Rectangle botaoFinaisArea = {
        (larguraResolucao - 270) / 2,
        420,
        270,
        67
    };

    Rectangle botaoRankingArea = {
        (larguraResolucao - 270) / 2,
        505,
        270,
        67
    };

    Rectangle botaoExitArea = {
        (larguraResolucao - 270) / 2,
        590,
        270,
        67
    };

    Rectangle botaoAudio = {
        larguraResolucao - 100,
        alturaResolucao - 100,
        75,
        75
    };


    // ==========================================================
    // INTERAÇÃO: BOTÃO DE SOM / MUTE
    // ==========================================================
    bool sobreAudio = CheckCollisionPointRec(mouse, botaoAudio);
    Rectangle audioDesenho = retanguloAnimadoBotao("inicio.som", botaoAudio, sobreAudio);

    if (sobreAudio && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        somMutadoGlobal = !somMutadoGlobal;
        SetMasterVolume(somMutadoGlobal ? 0.0f : 1.0f);
    }


    // ==========================================================
    // INTERAÇÃO: BOTÃO JOGAR
    // ==========================================================
    bool sobre_botao = CheckCollisionPointRec(mouse, botao);
    escala = 0.60f * escalaAnimadaBotao("inicio.jogar", sobre_botao);

    if (sobre_botao && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            return 1;


    // ==========================================================
    // INTERAÇÃO: BOTÃO INSTRUÇÕES
    // ==========================================================
    bool sobre_botaoInstrucoesArea = CheckCollisionPointRec(mouse, botaoInstrucoesArea);
    escalaInstrucoes = 0.60f * escalaAnimadaBotao("inicio.instrucoes", sobre_botaoInstrucoesArea);

    if (sobre_botaoInstrucoesArea && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            telaMenu = 1;


    // ==========================================================
    // INTERAÇÃO: BOTÃO FINAIS
    // ==========================================================
    bool sobre_botaoFinaisArea = CheckCollisionPointRec(mouse, botaoFinaisArea);
    escalaFinais = 0.60f * escalaAnimadaBotao("inicio.finais", sobre_botaoFinaisArea);

    if (sobre_botaoFinaisArea && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            carregarFinais();   // le o save antes de mostrar as pastas
            telaMenu = 2;
        }


    bool sobre_botaoRankingArea = CheckCollisionPointRec(mouse, botaoRankingArea);
    escalaRanking = 0.60f * escalaAnimadaBotao("inicio.ranking", sobre_botaoRankingArea);

    if (sobre_botaoRankingArea && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        abrirRankingMenu();
        telaMenu = 3;
    }


    // ==========================================================
    // INTERAÇÃO: BOTÃO SAIR
    // ==========================================================
    bool sobre_botaoExitArea = CheckCollisionPointRec(mouse, botaoExitArea);
    escalaExit = 0.60f * escalaAnimadaBotao("inicio.sair", sobre_botaoExitArea);

    if (sobre_botaoExitArea && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            return -1;


    // ==========================================================
    // DESENHO NA TELA VIRTUAL
    // ==========================================================
    BeginTextureMode(telaVirtual);;

    DrawTexture(fundoInicio, 0, 0, WHITE);

    // ==========================================================
    // BOTÃO JOGAR
    // ==========================================================
    DrawTextureEx(
        botaoJogar,
        (Vector2){
            larguraResolucao / 2 - botaoJogar.width * escala / 2,
            botao.y + botao.height / 2 - botaoJogar.height * escala / 2
        },
        0,
        escala,
        WHITE
    );


    // ==========================================================
    // BOTÃO INSTRUÇÕES
    // ==========================================================
    DrawTextureEx(
        botaoInstrucoes,
        (Vector2){
            larguraResolucao / 2 - botaoInstrucoes.width * escalaInstrucoes / 2,
            botaoInstrucoesArea.y + botaoInstrucoesArea.height / 2 - botaoInstrucoes.height * escalaInstrucoes / 2
        },
        0,
        escalaInstrucoes,
        WHITE
    );


    // ==========================================================
    // BOTÃO FINAIS
    // ==========================================================
    DrawTextureEx(
        botaoFinais,
        (Vector2){
            larguraResolucao / 2 - botaoFinais.width * escalaFinais / 2,
            botaoFinaisArea.y + botaoFinaisArea.height / 2 - botaoFinais.height * escalaFinais / 2
        },
        0,
        escalaFinais,
        WHITE
    );


    DrawTextureEx(
        botaoRanking,
        (Vector2){
            larguraResolucao / 2 - botaoRanking.width * escalaRanking / 2,
            botaoRankingArea.y + botaoRankingArea.height / 2 - botaoRanking.height * escalaRanking / 2
        },
        0,
        escalaRanking,
        WHITE
    );


    // ==========================================================
    // BOTÃO SAIR
    // ==========================================================
    DrawTextureEx(
        botaoExit,
        (Vector2){
            larguraResolucao / 2 - botaoExit.width * escalaExit / 2,
            botaoExitArea.y + botaoExitArea.height / 2 - botaoExit.height * escalaExit / 2
        },
        0,
        escalaExit,
        WHITE
    );


    // ==========================================================
    // SOM / MUTE
    // ==========================================================
    Texture2D texturaAudio;

    if (somMutadoGlobal == 0)
        texturaAudio = botaoSom;
    else
        texturaAudio = botaoMute;

    DrawTexturePro(
        texturaAudio,
        (Rectangle){
            0,
            0,
            texturaAudio.width,
            texturaAudio.height
        },
        audioDesenho,
        (Vector2){0, 0},
        0,
        WHITE
    );

    EndTextureMode();


    // ==========================================================
    // TELA VIRTUAL -> JANELA
    // ==========================================================
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

#define INTRO_INBOX 0
#define INTRO_EMAIL 1
#define INTRO_ESCURECER 2
#define INTRO_MANHA 3

#define INTRO_TELA_X 288
#define INTRO_TELA_Y 72
#define INTRO_TELA_L 704
#define INTRO_TELA_A 384

static const char *introFrases[] = {
    "Olá! Parabéns, você foi aprovado no estágio da Else.",
    "Por aqui, a IA está em quase todas as decisões.",
    "Seu trabalho é simples: observar, aprender e ajudar.",
    "Mas cuidado: até as menores escolhas têm consequências.",
    "Te esperamos amanhã, às 18h. Não se atrase!",
    "Bia · RH Else"
};

#define INTRO_TOTAL_FRASES 6

static int contarLetras(const char *texto)
{
    int total = 0;
    int i = 0;

    while (texto[i] != '\0')
    {
        int tamanho = 0;
        GetCodepointNext(&texto[i], &tamanho);
        i += tamanho;
        total++;
    }

    return total;
}

static void copiarLetras(const char *texto, int letras, char *destino, int limite)
{
    int i = 0;
    int contadas = 0;

    while (texto[i] != '\0' && contadas < letras)
    {
        int tamanho = 0;
        GetCodepointNext(&texto[i], &tamanho);
        i += tamanho;
        contadas++;
    }

    if (i > limite - 1)
        i = limite - 1;

    for (int k = 0; k < i; k++)
        destino[k] = texto[k];

    destino[i] = '\0';
}

int telaAviso(RenderTexture2D telaVirtual, float escalaTela, float offsetX, float offsetY)
{
    static Texture2D fundoCasa = {0};
    static Texture2D iconeEmail = {0};
    static Texture2D avatarRh = {0};

    static int estado = INTRO_INBOX;
    static float tempo = 0;
    static int frase = 0;
    static int letras = 0;
    static float relogioLetra = 0;
    static bool digitando = false;

    int larguraResolucao = 1280;
    int alturaResolucao = 720;

    if (fundoCasa.id == 0)
    {
        fundoCasa = LoadTexture("cenario/Telas/introCasa.png");
        iconeEmail = LoadTexture("cenario/Telas/introIconeEmail.png");
        avatarRh = LoadTexture("cenario/Telas/introAvatarRh.png");
    }

    float dt = GetFrameTime();
    tempo += dt;

    bool avancar =
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT) ||
        IsKeyPressed(KEY_E) ||
        IsKeyPressed(KEY_ENTER) ||
        IsKeyPressed(KEY_SPACE);

    if (estado == INTRO_INBOX)
    {
        if (avancar || tempo > 3.2f)
        {
            estado = INTRO_EMAIL;
            tempo = 0;
            frase = 0;
            letras = 0;
            relogioLetra = 0;
            digitando = true;
        }
    }
    else if (estado == INTRO_EMAIL)
    {
        int totalFrase = contarLetras(introFrases[frase]);
        bool ultima = frase == INTRO_TOTAL_FRASES - 1;

        if (digitando)
        {
            relogioLetra += dt;

            while (relogioLetra >= 0.036f && letras < totalFrase)
            {
                relogioLetra -= 0.036f;
                letras++;
            }

            if (avancar)
                letras = totalFrase;

            if (letras >= totalFrase)
            {
                digitando = false;
                tempo = 0;
            }
        }
        else if (avancar || tempo > (ultima ? 2.4f : 1.7f))
        {
            if (ultima)
            {
                estado = INTRO_ESCURECER;
            }
            else
            {
                frase++;
                letras = 0;
                relogioLetra = 0;
                digitando = true;
            }

            tempo = 0;
        }
    }
    else if (estado == INTRO_ESCURECER)
    {
        if (tempo > 1.8f)
        {
            estado = INTRO_MANHA;
            tempo = 0;
        }
    }
    else if (estado == INTRO_MANHA)
    {
        if (tempo > 2.6f || (avancar && tempo > 0.8f))
        {
            estado = INTRO_INBOX;
            tempo = 0;
            frase = 0;
            letras = 0;

            return 2;
        }
    }

    Color papel = (Color){241, 239, 230, 255};
    Color tinta = (Color){27, 29, 38, 255};
    Color cinzaTexto = (Color){119, 117, 108, 255};
    Color azul = (Color){29, 58, 138, 255};
    Color vermelho = (Color){226, 69, 60, 255};
    Color rodapeCor = (Color){220, 216, 203, 255};
    Color linhaCor = (Color){214, 210, 196, 255};

    int x0 = INTRO_TELA_X;
    int y0 = INTRO_TELA_Y;
    int larg = INTRO_TELA_L;
    int alt = INTRO_TELA_A;

    bool piscando = ((int)(GetTime() * 2)) % 2 == 0;

    BeginTextureMode(telaVirtual);

    ClearBackground(BLACK);

    DrawTexture(fundoCasa, 0, 0, WHITE);

    DrawRectangle(x0 - 6, y0 - 6, larg + 12, alt + 12, BLACK);
    DrawRectangle(x0, y0, larg, alt, papel);

    DrawRectangle(x0, y0, larg, 40, azul);
    DrawTexture(iconeEmail, x0 + 8, y0, WHITE);

    DrawRectangle(x0, y0 + alt - 28, larg, 28, rodapeCor);

    if (estado == INTRO_INBOX)
    {
        for (int i = 0; i < 4; i++)
        {
            int y = y0 + 40 + i * 46;

            if (i == 0)
            {
                DrawRectangle(x0, y, larg, 46, (Color){255, 247, 214, 255});

                if (piscando)
                {
                    DrawRectangle(x0 + 12, y + 17, 12, 12, BLACK);
                    DrawRectangle(x0 + 14, y + 19, 8, 8, vermelho);
                }

                DrawRectangle(x0 + larg - 70, y + 12, 58, 22, BLACK);
                DrawRectangle(x0 + larg - 68, y + 14, 54, 18, vermelho);
            }

            DrawRectangle(x0, y + 43, larg, 3, linhaCor);
        }
    }
    else
    {
        DrawTexture(avatarRh, x0 + 18, y0 + 50, WHITE);

        for (int x = x0 + 12; x < x0 + larg - 12; x += 10)
            DrawRectangle(x, y0 + 100, 6, 3, (Color){185, 182, 168, 255});

        if (estado != INTRO_EMAIL || (frase == INTRO_TOTAL_FRASES - 1 && !digitando))
        {
            int ax = x0 + 18;
            int ay = y0 + 290;

            DrawRectangle(ax, ay, 320, 40, tinta);
            DrawRectangle(ax + 3, ay + 3, 314, 34, WHITE);

            DrawRectangle(ax + 12, ay + 8, 18, 24, tinta);
            DrawRectangle(ax + 14, ay + 10, 14, 20, WHITE);
            DrawRectangle(ax + 18, ay + 12, 6, 6, (Color){226, 168, 115, 255});
            DrawRectangle(ax + 16, ay + 24, 10, 4, azul);
        }
    }

    EndTextureMode();

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

    float e = escalaTela;

#define TX(x) (offsetX + (x) * e)
#define TY(y) (offsetY + (y) * e)

    desenharTextoUI("ELSE MAIL", TX(x0 + 54), TY(y0 + 13), 14 * e, WHITE, FONTE_DESTAQUE);
    desenharTextoUI(
        "DOM 18:40",
        TX(x0 + larg - 14) - medirTextoUI("DOM 18:40", 14 * e, FONTE_DESTAQUE),
        TY(y0 + 13),
        14 * e,
        WHITE,
        FONTE_DESTAQUE
    );

    const char *rodapeEsquerda = estado == INTRO_INBOX ? "CAIXA DE ENTRADA (1)" : "1 ANEXO";
    const char *rodapeDireita = estado == INTRO_INBOX ? "[E] ABRIR" : "[E] CONTINUAR";

    desenharTextoUI(rodapeEsquerda, TX(x0 + 12), TY(y0 + alt - 19), 11 * e, (Color){74, 74, 68, 255}, FONTE_DESTAQUE);

    if (piscando || estado != INTRO_INBOX)
    {
        desenharTextoUI(
            rodapeDireita,
            TX(x0 + larg - 12) - medirTextoUI(rodapeDireita, 11 * e, FONTE_DESTAQUE),
            TY(y0 + alt - 19 - (piscando ? 1 : 0)),
            11 * e,
            azul,
            FONTE_DESTAQUE
        );
    }

    if (estado == INTRO_INBOX)
    {
        const char *de[4] = {"RH Else", "Mãe", "CESAR School", "Loja de jogos"};
        const char *assunto[4] = {
            "Resultado do processo seletivo",
            "já almoçou???",
            "Notas do semestre disponíveis",
            "50% OFF só hoje!!!"
        };
        const char *quando[4] = {"", "ontem", "sex", "qua"};

        for (int i = 0; i < 4; i++)
        {
            int y = y0 + 40 + i * 46;
            Color cor = i == 0 ? tinta : cinzaTexto;

            desenharTextoUI(de[i], TX(x0 + 36), TY(y + 10), 26 * e, cor, FONTE_TEXTO);
            desenharTextoUI(assunto[i], TX(x0 + 200), TY(y + 10), 26 * e, cor, FONTE_TEXTO);

            if (i == 0)
            {
                desenharTextoUI(
                    "NOVO",
                    TX(x0 + larg - 41) - medirTextoUI("NOVO", 10 * e, FONTE_DESTAQUE) / 2,
                    TY(y + 19),
                    10 * e,
                    WHITE,
                    FONTE_DESTAQUE
                );
            }
            else
            {
                desenharTextoUI(
                    quando[i],
                    TX(x0 + larg - 14) - medirTextoUI(quando[i], 26 * e, FONTE_TEXTO),
                    TY(y + 10),
                    26 * e,
                    cor,
                    FONTE_TEXTO
                );
            }
        }
    }
    else
    {
        desenharTextoUI("De: rh@else.com.br", TX(x0 + 72), TY(y0 + 48), 22 * e, tinta, FONTE_TEXTO);
        desenharTextoUI("Assunto: Resultado do processo seletivo", TX(x0 + 72), TY(y0 + 70), 22 * e, tinta, FONTE_TEXTO);

        int ultimaVisivel = estado == INTRO_EMAIL ? frase : INTRO_TOTAL_FRASES - 1;

        for (int i = 0; i <= ultimaVisivel; i++)
        {
            char trecho[160];
            const char *texto = introFrases[i];

            if (estado == INTRO_EMAIL && i == frase)
                copiarLetras(texto, letras, trecho, sizeof(trecho));
            else
                copiarLetras(texto, 1000, trecho, sizeof(trecho));

            Color cor = i == INTRO_TOTAL_FRASES - 1 ? azul : tinta;
            float yLinha = y0 + 112 + i * 28;

            desenharTextoUI(trecho, TX(x0 + 18), TY(yLinha), 26 * e, cor, FONTE_TEXTO);

            if (estado == INTRO_EMAIL && i == frase && piscando)
            {
                float fim = medirTextoUI(trecho, 26 * e, FONTE_TEXTO);
                DrawRectangle(TX(x0 + 20) + fim, TY(yLinha + 3), 10 * e, 20 * e, cor);
            }
        }

        if (estado != INTRO_EMAIL || (frase == INTRO_TOTAL_FRASES - 1 && !digitando))
        {
            desenharTextoUI("cracha_estagiario.png", TX(x0 + 58), TY(y0 + 298), 22 * e, tinta, FONTE_TEXTO);
            desenharTextoUI(
                "12 KB",
                TX(x0 + 68) + medirTextoUI("cracha_estagiario.png", 22 * e, FONTE_TEXTO),
                TY(y0 + 300),
                18 * e,
                cinzaTexto,
                FONTE_TEXTO
            );
        }
    }

    if (estado == INTRO_ESCURECER || estado == INTRO_MANHA)
    {
        float progresso = estado == INTRO_MANHA ? 1.0f : tempo / 1.8f;
        float faixa = 430;
        float baseEscuro = progresso * (alturaResolucao + faixa);

        float alturaSolida = baseEscuro - faixa;

        if (alturaSolida > 0)
            DrawRectangle(offsetX, offsetY, larguraResolucao * e, alturaSolida * e, BLACK);

        DrawRectangleGradientV(
            offsetX,
            TY(alturaSolida),
            larguraResolucao * e,
            faixa * e,
            BLACK,
            (Color){0, 0, 0, 0}
        );

        if (estado == INTRO_MANHA)
        {
            float alfa = tempo / 0.8f;

            if (alfa > 1)
                alfa = 1;

            const char *manha = "NO DIA SEGUINTE...";
            float tamanho = 24 * e;

            desenharTextoUI(
                manha,
                TX(larguraResolucao / 2) - medirTextoUI(manha, tamanho, FONTE_DESTAQUE) / 2,
                TY(alturaResolucao / 2 - 12),
                tamanho,
                Fade(WHITE, alfa),
                FONTE_DESTAQUE
            );
        }
    }

#undef TX
#undef TY

    EndDrawing();

    return 0;
}
