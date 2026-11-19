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
    static Texture2D botaoExit = {0};
    static Texture2D telaInstrucoes = {0};
    static Texture2D botaoVoltar = {0};
    static Texture2D telaFinais = {0};
    static Texture2D pastaAberta = {0};
    static Texture2D arteFinal[8] = {0};
    static Texture2D pastaFechada = {0};
    static Texture2D fita = {0};

    static int mutado = 0;
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

        Rectangle botaoVoltarDesenho = botaoVoltarArea;

        if (CheckCollisionPointRec(mouse, botaoVoltarArea))
        {
            botaoVoltarDesenho.width *= 1.10f;
            botaoVoltarDesenho.height *= 1.10f;

            botaoVoltarDesenho.x =
                botaoVoltarArea.x +
                (botaoVoltarArea.width - botaoVoltarDesenho.width) / 2;

            botaoVoltarDesenho.y =
                botaoVoltarArea.y +
                (botaoVoltarArea.height - botaoVoltarDesenho.height) / 2;

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                telaMenu = 0;
        }

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

        Rectangle botaoVoltarDesenho = botaoVoltarArea;

        if (CheckCollisionPointRec(mouse, botaoVoltarArea))
        {
            botaoVoltarDesenho.width *= 1.10f;
            botaoVoltarDesenho.height *= 1.10f;

            botaoVoltarDesenho.x =
                botaoVoltarArea.x +
                (botaoVoltarArea.width - botaoVoltarDesenho.width) / 2;

            botaoVoltarDesenho.y =
                botaoVoltarArea.y +
                (botaoVoltarArea.height - botaoVoltarDesenho.height) / 2;

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                telaMenu = 0;
        }

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

    // ==========================================================
    // BOTÕES (escala e áreas de clique)
    // ==========================================================
    float escala = 0.60f;
    float escalaInstrucoes = 0.60f;
    float escalaFinais = 0.60f;
    float escalaExit = 0.60f;

    Rectangle botao = {
        (larguraResolucao - 270) / 2,
        275,
        270,
        67
    };

    Rectangle botaoInstrucoesArea = {
        (larguraResolucao - 270) / 2,
        365,
        270,
        67
    };

    Rectangle botaoFinaisArea = {
        (larguraResolucao - 270) / 2,
        455,
        270,
        67
    };

    Rectangle botaoExitArea = {
        (larguraResolucao - 270) / 2,
        545,
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
    if (CheckCollisionPointRec(mouse, botaoAudio))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (mutado == 0)
            {
                mutado = 1;
                SetMusicVolume(trilhaSonora1, 0.0f);
            }
            else
            {
                mutado = 0;
                SetMusicVolume(trilhaSonora1, 0.10f);
            }
        }
    }


    // ==========================================================
    // INTERAÇÃO: BOTÃO JOGAR
    // ==========================================================
    if (CheckCollisionPointRec(mouse, botao))
    {
        escala = 0.66f;

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            return 1;
    }


    // ==========================================================
    // INTERAÇÃO: BOTÃO INSTRUÇÕES
    // ==========================================================
    if (CheckCollisionPointRec(mouse, botaoInstrucoesArea))
    {
        escalaInstrucoes = 0.66f;

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            telaMenu = 1;
    }


    // ==========================================================
    // INTERAÇÃO: BOTÃO FINAIS
    // ==========================================================
    if (CheckCollisionPointRec(mouse, botaoFinaisArea))
    {
        escalaFinais = 0.66f;

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            carregarFinais();   // le o save antes de mostrar as pastas
            telaMenu = 2;
        }
    }


    // ==========================================================
    // INTERAÇÃO: BOTÃO SAIR
    // ==========================================================
    if (CheckCollisionPointRec(mouse, botaoExitArea))
    {
        escalaExit = 0.66f;

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            return -1;
    }


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

    if (mutado == 0)
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
        botaoAudio,
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

int telaAviso(RenderTexture2D telaVirtual, float escalaTela, float offsetX, float offsetY)
{
    // ==========================================================
    // TEXTURAS (persistem entre as chamadas)
    // ==========================================================
    static Texture2D TelaAviso = {0};


    // ==========================================================
    // MONITOR
    // ==========================================================
    int monitor = GetCurrentMonitor();
    int larguraMonitor = GetMonitorWidth(monitor);
    int alturaMonitor = GetMonitorHeight(monitor);


    // ==========================================================
    // RESOLUÇÃO
    // ==========================================================
    int larguraResolucao = 1280;
    int alturaResolucao = 720;

    if (TelaAviso.id == 0)
        TelaAviso = LoadTexture("cenario/Aviso.png");


    // ==========================================================
    // ÁREA DE CLIQUE
    // ==========================================================
    Rectangle clicar = {
        larguraResolucao - 365,
        alturaResolucao - 105,
        300,
        60
    };


    // ==========================================================
    // DESENHO NA TELA VIRTUAL
    // ==========================================================
    BeginTextureMode(telaVirtual);

    ClearBackground(RAYWHITE);

    DrawTexture(TelaAviso, 0, 0, WHITE);

    EndTextureMode();


    // ==========================================================
    // MOUSE / INTERAÇÃO
    // ==========================================================
    Vector2 mouse = GetMousePosition();

    mouse.x = (mouse.x - offsetX) / escalaTela;
    mouse.y = (mouse.y - offsetY) / escalaTela;

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
        CheckCollisionPointRec(mouse, clicar))
    {
        return 2;
    }


    // ==========================================================
    // TELA VIRTUAL -> JANELA
    // ==========================================================
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