// ==========================================================
// MÁQUINA DE CAFÉ (US13)
// ----------------------------------------------------------
// O jogador chega perto do balcão do café e aperta E.
// Abre um close da máquina com a tampa aberta: 3 chaves (A, B, C)
// ligam/desligam com clique e o botão branco (TESTAR) testa.
// Acertou: luz verde, café forte (cada medidor anda 10 rumo ao 50).
// Errou: gasta 1 das 3 tentativas. A máquina só volta na próxima fase.
//
// CADA NÍVEL (CARGO) TEM O SEU ENIGMA -> ver a TABELA DE ENIGMAS abaixo.
// Peças iguais em todos os níveis:  cenario/maquina_cafe/comum/
// Peças de cada nível (placas):     cenario/maquina_cafe/<pasta do nível>/
// ==========================================================
#include "raylib.h"
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "Regras.h"

// medidores (moram no Cartas.c)
extern float *confiancaApagado;
extern float *viesApagado;
extern float *privacidadeApagada;
extern float *lucroApagado;

const char *nomeCargo(void);

#define PASTA_MAQUINA "cenario/maquina_cafe/"
#define LIMITE_APAGADO 0.73f


// ##########################################################
// ENIGMAS POR NÍVEL
// ##########################################################

// ---------------- ENIGMA 1 · ESTAGIÁRIO ----------------
// luz = (A E B) E (NÃO C)   ->  resposta: A ligada, B ligada, C desligada
static bool luzEstagiario(bool a, bool b, bool c)
{
    return (a && b) && !c;
}

// ---------------- ENIGMA 2 · ASSISTENTE ----------------
// (ainda não feito: criar luzAssistente, a pasta "assistente" com as placas
//  e uma linha nova na tabela)


// texto escrito por cima da placa (nome das portas, rótulos A/B/C...)
typedef struct
{
    const char *texto;
    int x, y;
    int fonte;          // 0 = Press Start 12 (portas), 1 = VT323 24 (NÃO), 2 = Press Start 8 (A/B/C)
    bool centralizado;
} Rotulo;

#define MAX_ROTULOS 8

typedef struct
{
    const char *cargo;                    // igual ao nomeCargo() do Cartas.c
    const char *pasta;                    // pasta das placas em cenario/maquina_cafe/
    bool (*luzAcende)(bool a, bool b, bool c);
    Rotulo rotulos[MAX_ROTULOS];
} Enigma;

// ==========================================================
// TABELA DE ENIGMAS: uma linha por nível
// ==========================================================
static const Enigma ENIGMAS[] =
{
    {
        "Estagiario", "estagiario", luzEstagiario,
        {
            { "E",   578, 320, 0, true  },   // porta E de baixo
            { "E",   674, 232, 0, true  },   // porta E de cima
            { "NÃO", 804, 312, 1, false },
            { "A",   520, 416, 2, false },
            { "B",   648, 416, 2, false },
            { "C",   776, 416, 2, false },
        }
    },
    // { "Assistente", "assistente", luzAssistente, { ... } },
};

#define QTD_ENIGMAS ((int)(sizeof(ENIGMAS) / sizeof(ENIGMAS[0])))


// ##########################################################
// PEÇAS (imagem + posição na tela virtual 1280x720)
// ##########################################################
typedef struct
{
    Texture2D textura;
    int x;
    int y;
} Peca;

static Peca carregarPeca(const char *caminho, int x, int y)
{
    Peca p;
    p.textura = LoadTexture(TextFormat(PASTA_MAQUINA "%s.png", caminho));
    p.x = x;
    p.y = y;
    return p;
}

static void desenharPeca(Peca p)
{
    DrawTexture(p.textura, p.x, p.y, WHITE);
}

// peças comuns
static Peca balcao, postit, visor, corpo, cafe, fumaca;
static Peca lampApagada, lampVerde, lampVermelha;
static Peca chaveLigada, chaveDesligada;
static Peca xicaraCheia[3], xicaraGasta[3];
// placas de cada enigma: [enigma][combinação de A, B, C]
static Peca placas[QTD_ENIGMAS][8];

static Font fonteVisor, fontePostit, fonteRotulo[3], fonteTentativas, fonteTestar;


// ##########################################################
// ESTADO
// ##########################################################
typedef enum { JOGANDO, ERROU, ACERTOU, PIFOU } EstadoMaquina;

static int enigmaAtual = -1;             // -1 = este cargo não tem enigma (a máquina não quebra)
static bool aberta = false;
static bool disponivel = true;           // volta a ficar disponível quando o cargo muda
static EstadoMaquina estado = JOGANDO;
static bool chave[3] = { false, false, false };
static int tentativas = 3;
static float tempoLuzVermelha = 0.0f;
static int antes[4], depois[4];          // medidores antes/depois do café forte
static char cargoDaFase[32] = "";

// áreas clicáveis (tela virtual)
static const int X_CHAVE[3] = { 512, 640, 768 };
static const Rectangle AREA_TESTAR = { 452, 92, 52, 52 };


// ##########################################################
// CARREGAR / DESCARREGAR
// ##########################################################
void iniciarMaquinaCafe(void)
{
    balcao       = carregarPeca("comum/balcao", 0, 652);
    postit       = carregarPeca("comum/postit", 48, 144);
    visor        = carregarPeca("comum/visor", 932, 156);
    corpo        = carregarPeca("comum/corpo", 280, 64);
    cafe         = carregarPeca("comum/cafe", 600, 596);
    fumaca       = carregarPeca("comum/fumaca", 492, 0);
    lampApagada  = carregarPeca("comum/lampada_apagada", 620, 84);
    lampVerde    = carregarPeca("comum/lampada_verde", 580, 44);
    lampVermelha = carregarPeca("comum/lampada_vermelha", 620, 84);
    chaveLigada    = carregarPeca("comum/chave_ligada", 500, 468);
    chaveDesligada = carregarPeca("comum/chave_desligada", 500, 468);
    xicaraCheia[0] = carregarPeca("comum/xicara0_cheia", 1008, 580);
    xicaraCheia[1] = carregarPeca("comum/xicara1_cheia", 1088, 580);
    xicaraCheia[2] = carregarPeca("comum/xicara2_cheia", 1168, 580);
    xicaraGasta[0] = carregarPeca("comum/xicara0_gasta", 1008, 604);
    xicaraGasta[1] = carregarPeca("comum/xicara1_gasta", 1088, 604);
    xicaraGasta[2] = carregarPeca("comum/xicara2_gasta", 1168, 604);

    // placas de cada nível: <pasta>/placa_ABC.png
    for (int e = 0; e < QTD_ENIGMAS; e++)
        for (int i = 0; i < 8; i++)
            placas[e][i] = carregarPeca(TextFormat("%s/placa_%d%d%d", ENIGMAS[e].pasta,
                                                   (i >> 2) & 1, (i >> 1) & 1, i & 1), 424, 144);

    // fontes com acentos (caracteres 32 a 255)
    int letras[224];
    for (int i = 0; i < 224; i++) letras[i] = 32 + i;
    fonteVisor      = LoadFontEx(PASTA_MAQUINA "comum/VT323-Regular.ttf", 28, letras, 224);
    fontePostit     = LoadFontEx(PASTA_MAQUINA "comum/VT323-Regular.ttf", 26, letras, 224);
    fonteRotulo[0]  = LoadFontEx(PASTA_MAQUINA "comum/PressStart2P-Regular.ttf", 12, letras, 224);
    fonteRotulo[1]  = LoadFontEx(PASTA_MAQUINA "comum/VT323-Regular.ttf", 24, letras, 224);
    fonteRotulo[2]  = LoadFontEx(PASTA_MAQUINA "comum/PressStart2P-Regular.ttf", 8, letras, 224);
    fonteTentativas = LoadFontEx(PASTA_MAQUINA "comum/PressStart2P-Regular.ttf", 10, letras, 224);
    fonteTestar     = fonteRotulo[2];
}

void descarregarMaquinaCafe(void)
{
    Peca comuns[] = { balcao, postit, visor, corpo, cafe, fumaca, lampApagada, lampVerde, lampVermelha,
                      chaveLigada, chaveDesligada };
    for (int i = 0; i < (int)(sizeof(comuns) / sizeof(comuns[0])); i++) UnloadTexture(comuns[i].textura);
    for (int k = 0; k < 3; k++) { UnloadTexture(xicaraCheia[k].textura); UnloadTexture(xicaraGasta[k].textura); }
    for (int e = 0; e < QTD_ENIGMAS; e++)
        for (int i = 0; i < 8; i++) UnloadTexture(placas[e][i].textura);
    UnloadFont(fonteVisor); UnloadFont(fontePostit); UnloadFont(fonteTentativas);
    for (int f = 0; f < 3; f++) UnloadFont(fonteRotulo[f]);
}


// ##########################################################
// FASE / RESET
// ##########################################################
static void zerarEnigma(void)
{
    chave[0] = chave[1] = chave[2] = false;
    tentativas = 3;
    estado = JOGANDO;
    tempoLuzVermelha = 0.0f;
}

// procura na tabela o enigma do cargo atual (-1 se não tiver)
static int enigmaDoCargo(const char *cargo)
{
    for (int e = 0; e < QTD_ENIGMAS; e++)
        if (strcmp(ENIGMAS[e].cargo, cargo) == 0) return e;
    return -1;
}

static void comecarFase(void)
{
    strcpy(cargoDaFase, nomeCargo());
    enigmaAtual = enigmaDoCargo(cargoDaFase);
    disponivel = (enigmaAtual >= 0);     // cargo sem enigma: a máquina não quebra
    zerarEnigma();
}

void reiniciarMaquinaCafe(void)
{
    aberta = false;
    comecarFase();
}

// fase nova = cargo novo: troca o enigma e a máquina quebra de novo
static void verificarFaseNova(void)
{
    if (strcmp(cargoDaFase, nomeCargo()) != 0)
        comecarFase();
}


// ##########################################################
// ABRIR / FECHAR
// ##########################################################
bool maquinaCafeAberta(void)
{
    return aberta;
}

void fecharMaquinaCafe(void)
{
    aberta = false;
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    if (estado == ACERTOU || estado == PIFOU)
        disponivel = false;              // só volta na próxima fase
}


// ##########################################################
// CAFÉ FORTE: cada medidor anda 10 pontos rumo ao 50 (sem passar)
// ##########################################################
static int aproximarDe50(int v)
{
    if (v < 50) { v += 10; if (v > 50) v = 50; }
    else if (v > 50) { v -= 10; if (v < 50) v = 50; }
    return v;
}

static void tomarCafeForte(void)
{
    float *medidor[4] = { confiancaApagado, viesApagado, privacidadeApagada, lucroApagado };

    for (int i = 0; i < 4; i++)
    {
        antes[i] = obterValorMedidor(i + 1);
        depois[i] = aproximarDe50(antes[i]);
        // valor 0..100 -> "apagado" (0 = barra cheia, 0.73 = barra vazia)
        *medidor[i] = LIMITE_APAGADO * (1.0f - depois[i] / 100.0f);
    }
}


// ##########################################################
// PERTO DO BALCÃO (chamado quando o jogador está perto dele)
// ##########################################################
void interagirBalcaoCafe(int larguraResolucao)
{
    verificarFaseNova();
    if (enigmaAtual < 0) return;         // este cargo não tem enigma

    if (!disponivel)
    {
        DrawText("Maquina indisponivel ate a proxima fase", larguraResolucao - 460, 75, 20, GRAY);
        return;
    }

    DrawText("Pressione E para consertar a maquina", larguraResolucao - 430, 75, 20, GREEN);

    if (IsKeyPressed(KEY_E))
    {
        aberta = true;
        if (estado == ERROU) estado = JOGANDO;   // continua com as tentativas que sobraram
    }
}

// balãozinho "!" em cima do balcão enquanto a máquina estiver quebrada
void desenharAvisoBalcaoCafe(void)
{
    verificarFaseNova();
    if (!disponivel) return;

    int x = 112;
    int y = 52 + (int)(3.0f * sinf((float)GetTime() * 4.0f));   // sobe e desce
    DrawRectangle(x - 2, y - 2, 24, 24, BLACK);
    DrawRectangle(x, y, 20, 20, (Color){ 226, 87, 95, 255 });
    DrawTriangle((Vector2){ x + 6, y + 21 }, (Vector2){ x + 10, y + 27 }, (Vector2){ x + 14, y + 21 }, BLACK);
    DrawRectangle(x + 8, y + 3, 4, 9, WHITE);
    DrawRectangle(x + 8, y + 14, 4, 3, WHITE);
}


// ##########################################################
// TELA DA MÁQUINA (lógica + desenho), por cima de tudo
// ##########################################################
static void texto(Font f, const char *t, float x, float y, Color cor, bool centro)
{
    if (centro) x -= MeasureTextEx(f, t, f.baseSize, 0).x / 2;
    DrawTextEx(f, t, (Vector2){ x, y }, f.baseSize, 0, cor);
}

void atualizarDesenharMaquinaCafe(float escala, float offsetX, float offsetY)
{
    if (!aberta || enigmaAtual < 0) return;
    const Enigma *enigma = &ENIGMAS[enigmaAtual];

    // ---------------- lógica
    // mouse da janela -> tela virtual 1280x720
    Vector2 mouse = GetMousePosition();
    mouse.x = (mouse.x - offsetX) / escala;
    mouse.y = (mouse.y - offsetY) / escala;

    bool podeMexer = (estado == JOGANDO || estado == ERROU);
    bool emCimaDeAlgo = false;

    if (podeMexer)
    {
        for (int k = 0; k < 3; k++)
        {
            Rectangle area = { X_CHAVE[k] - 32, 456, 64, 96 };
            if (CheckCollisionPointRec(mouse, area))
            {
                emCimaDeAlgo = true;
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    chave[k] = !chave[k];
                    estado = JOGANDO;
                }
            }
        }

        if (CheckCollisionPointRec(mouse, AREA_TESTAR))
        {
            emCimaDeAlgo = true;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                if (enigma->luzAcende(chave[0], chave[1], chave[2]))
                {
                    estado = ACERTOU;
                    tomarCafeForte();
                }
                else
                {
                    tentativas--;
                    tempoLuzVermelha = 1.2f;
                    estado = (tentativas <= 0) ? PIFOU : ERROU;
                }
            }
        }
    }
    else if (IsKeyPressed(KEY_E))
    {
        fecharMaquinaCafe();             // acertou ou pifou: E sai
        return;
    }

    SetMouseCursor(emCimaDeAlgo ? MOUSE_CURSOR_POINTING_HAND : MOUSE_CURSOR_DEFAULT);
    if (tempoLuzVermelha > 0) tempoLuzVermelha -= GetFrameTime();

    // ---------------- desenho
    DrawRectangle(0, 0, 1280, 720, Fade(BLACK, 0.63f));   // escurece o escritório
    desenharPeca(balcao);
    desenharPeca(postit);
    desenharPeca(visor);
    for (int k = 0; k < 3; k++) desenharPeca(k < tentativas ? xicaraCheia[k] : xicaraGasta[k]);
    desenharPeca(corpo);
    desenharPeca(placas[enigmaAtual][(chave[0] << 2) | (chave[1] << 1) | chave[2]]);

    if (estado == ACERTOU)          desenharPeca(lampVerde);
    else if (tempoLuzVermelha > 0)  desenharPeca(lampVermelha);
    else                            desenharPeca(lampApagada);

    for (int k = 0; k < 3; k++)
    {
        Peca c = chave[k] ? chaveLigada : chaveDesligada;
        c.x += X_CHAVE[k] - X_CHAVE[0];
        desenharPeca(c);
    }
    if (estado == ACERTOU) desenharPeca(cafe);
    if (estado == PIFOU)   desenharPeca(fumaca);

    // rótulos da placa (vêm da tabela do enigma)
    Color silk = { 225, 232, 220, 255 }, claro = { 230, 230, 236, 255 };
    for (int r = 0; r < MAX_ROTULOS && enigma->rotulos[r].texto != NULL; r++)
    {
        const Rotulo *ro = &enigma->rotulos[r];
        texto(fonteRotulo[ro->fonte], ro->texto, ro->x, ro->y, silk, ro->centralizado);
    }
    texto(fonteTestar, "TESTAR", 476, 160, claro, true);
    texto(fonteTentativas, "TENTATIVAS", 1008, 556, claro, false);

    // post-it
    const char *recado[] = { "Máquina com defeito.", "Faça ela funcionar", "pra sair o café.", "",
                             "3 tentativas", "por fase.", "- TI" };
    for (int k = 0; k < 7; k++) texto(fontePostit, recado[k], 60, 178 + k * 22, (Color){ 40, 36, 44, 255 }, false);

    // visor
    Color verde = { 88, 220, 110, 255 }, vermelho = { 226, 87, 95, 255 };
    float vx = 946, vy = 172;
    if (estado == ERROU)
    {
        texto(fonteVisor, "NÃO FUNCIONOU", vx, vy, vermelho, false);
        texto(fonteVisor, TextFormat("restam %d", tentativas), vx, vy + 52, vermelho, false);
        texto(fonteVisor, tentativas == 1 ? "tentativa" : "tentativas", vx, vy + 78, vermelho, false);
    }
    else if (estado == ACERTOU)
    {
        const char *nomes[4] = { "Confiança ", "Viés      ", "Privacid. ", "Lucro     " };
        texto(fonteVisor, "CAFÉ FORTE!", vx, vy, verde, false);
        for (int i = 0; i < 4; i++)
            texto(fonteVisor, TextFormat("%s %d>%d", nomes[i], antes[i], depois[i]), vx, vy + 26 * (i + 1), verde, false);
        texto(fonteVisor, "E: voltar", vx, vy + 26 * 10, verde, false);
    }
    else if (estado == PIFOU)
    {
        texto(fonteVisor, "PIFOU DE VEZ", vx, vy, vermelho, false);
        texto(fonteVisor, "volte na", vx, vy + 52, vermelho, false);
        texto(fonteVisor, "próxima fase", vx, vy + 78, vermelho, false);
        texto(fonteVisor, "E: voltar", vx, vy + 26 * 10, vermelho, false);
    }
}
