// ==========================================================
// MÁQUINA DE CAFÉ (US13)
// ----------------------------------------------------------
// O jogador chega perto do balcão do café e aperta E.
// Abre um close da máquina com a tampa aberta: de 3 a 5 chaves (A, B, C, D, E)
// ligam/desligam com clique e o botão branco (TESTAR) testa.
// Acertou: luz verde, café forte (cada medidor anda 15 rumo ao 50).
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
void agendarTextoUI(const char *texto, float x, float y, float tamanho, Color cor, bool centralizado, int fonte);
Rectangle retanguloAnimadoBotao(const char *nome, Rectangle base, bool hover);   // animacao padrao dos botoes

#define PASTA_MAQUINA "cenario/maquina_cafe/"
#define LIMITE_APAGADO 0.73f


// ##########################################################
// ENIGMAS POR NÍVEL
// ##########################################################

#define MAX_CHAVES 5
#define MAX_LUZES 2

static bool xou(bool x, bool y)
{
    return x != y;
}

// ---------------- ENIGMA 1 · ESTAGIÁRIO ----------------
// luz = (A E B) E (NÃO C)   ->  resposta: 1 1 0
static void luzEstagiario(const bool c[], bool luz[])
{
    luz[0] = (c[0] && c[1]) && !c[2];
}

// ---------------- ENIGMA 2 · JÚNIOR ----------------
// luz = (A XOU B) E (B E C) E (NÃO D)   ->  resposta: 0 1 1 0
static void luzJunior(const bool c[], bool luz[])
{
    luz[0] = xou(c[0], c[1]) && (c[1] && c[2]) && !c[3];
}

// ---------------- ENIGMA 3 · PLENO ----------------
// luz = (A NE B) E (B XOU C) E (C NOU D)   ->  resposta: 0 1 0 0
static void luzPleno(const bool c[], bool luz[])
{
    luz[0] = !(c[0] && c[1]) && xou(c[1], c[2]) && !(c[2] || c[3]);
}

// ---------------- ENIGMA 4 · SÊNIOR ----------------
// luz = NOU( A NE B , C XOU D , C OU E )   ->  resposta: 1 1 0 0 0
static void luzSenior(const bool c[], bool luz[])
{
    bool ne = !(c[0] && c[1]);
    luz[0] = !(ne || xou(c[2], c[3]) || (c[2] || c[4]));
}

// ---------------- ENIGMA 5 · TECH LÍDER ----------------
// luz 1 = (A XOU B) E (C NOU E)
// luz 2 = (B NE D) E (C OU D) E (D XOU E)   ->  resposta: 1 0 0 1 0
static void luzTechLider(const bool c[], bool luz[])
{
    luz[0] = xou(c[0], c[1]) && !(c[2] || c[4]);
    luz[1] = !(c[1] && c[3]) && (c[2] || c[3]) && xou(c[3], c[4]);
}


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
    void (*calcularLuzes)(const bool c[], bool luz[]);
    int qtdChaves;
    int xChave[MAX_CHAVES];
    bool placaPorEstado;
    int qtdLuzes;
    int xLuz[MAX_LUZES];
    bool letrasNoPainel;
    Rotulo rotulos[MAX_ROTULOS];
} Enigma;

// ==========================================================
// TABELA DE ENIGMAS: uma linha por nível
// ==========================================================
static const Enigma ENIGMAS[] =
{
    {
        "Estagiario", "estagiario", luzEstagiario,
        3, { 512, 640, 768 }, true, 1, { 160 }, false,
        {
            { "E",   578, 320, 0, true  },   // porta E de baixo
            { "E",   674, 232, 0, true  },   // porta E de cima
            { "NÃO", 804, 312, 1, false },
            { "A",   520, 416, 2, false },
            { "B",   648, 416, 2, false },
            { "C",   776, 416, 2, false },
        }
    },
    { "Junior",     "junior",    luzJunior,    4, { 496, 592, 688, 784 },      true,  1, { 160 },      true, { { NULL } } },
    { "Pleno",      "pleno",     luzPleno,     4, { 496, 592, 688, 784 },      true,  1, { 160 },      true, { { NULL } } },
    { "Senior",     "senior",    luzSenior,    5, { 480, 560, 640, 720, 800 }, false, 1, { 160 },      true, { { NULL } } },
    { "Tech Lider", "techlider", luzTechLider, 5, { 480, 560, 640, 720, 800 }, false, 2, { 145, 175 }, true, { { NULL } } },
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
static Peca lampApagada[QTD_ENIGMAS][MAX_LUZES], lampVerde[QTD_ENIGMAS][MAX_LUZES], lampVermelha[QTD_ENIGMAS][MAX_LUZES];
static Peca cabecaDuasLuzes[QTD_ENIGMAS];
static Peca chaveLigada, chaveDesligada;
static Peca xicaraCheia[3], xicaraGasta[3];
// placas de cada enigma: [enigma][combinação das chaves]
static Peca placas[QTD_ENIGMAS][1 << MAX_CHAVES];

static Font fonteVisor, fontePostit, fonteRotulo[3], fonteTentativas, fonteTestar;


// ##########################################################
// ESTADO
// ##########################################################
typedef enum { JOGANDO, ERROU, ACERTOU, PIFOU } EstadoMaquina;

static int enigmaAtual = -1;             // -1 = este cargo não tem enigma (a máquina não quebra)
static bool aberta = false;
static bool disponivel = true;           // volta a ficar disponível quando o cargo muda
static EstadoMaquina estado = JOGANDO;
static bool chave[MAX_CHAVES] = { false };
static int tentativas = 3;
static float tempoLuzVermelha = 0.0f;
static bool ultimaLuz[MAX_LUZES] = { false };
static bool soUmaLuz = false;
static int antes[4], depois[4];          // medidores antes/depois do café forte
static char cargoDaFase[32] = "";

// áreas clicáveis (tela virtual)
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
    for (int e = 0; e < QTD_ENIGMAS; e++)
    {
        const Enigma *en = &ENIGMAS[e];
        for (int k = 0; k < en->qtdLuzes; k++)
        {
            int x = en->xLuz[k];
            if (en->qtdLuzes == 1)
            {
                lampApagada[e][k]  = carregarPeca("comum/lampada_apagada", 620, 84);
                lampVerde[e][k]    = carregarPeca("comum/lampada_verde", 580, 44);
                lampVermelha[e][k] = carregarPeca("comum/lampada_vermelha", 620, 84);
            }
            else
            {
                lampApagada[e][k]  = carregarPeca(TextFormat("%s/lampada%d_apagada", en->pasta, x), x * 4 - 20, 84);
                lampVerde[e][k]    = carregarPeca(TextFormat("%s/lampada%d_verde", en->pasta, x), x * 4 - 60, 44);
                lampVermelha[e][k] = carregarPeca(TextFormat("%s/lampada%d_vermelha", en->pasta, x), x * 4 - 20, 84);
            }
        }
        if (en->qtdLuzes > 1)
            cabecaDuasLuzes[e] = carregarPeca(TextFormat("%s/cabeca_duas_luzes", en->pasta), 548, 68);
    }
    chaveLigada    = carregarPeca("comum/chave_ligada", 500, 468);
    chaveDesligada = carregarPeca("comum/chave_desligada", 500, 468);
    xicaraCheia[0] = carregarPeca("comum/xicara0_cheia", 1008, 580);
    xicaraCheia[1] = carregarPeca("comum/xicara1_cheia", 1088, 580);
    xicaraCheia[2] = carregarPeca("comum/xicara2_cheia", 1168, 580);
    xicaraGasta[0] = carregarPeca("comum/xicara0_gasta", 1008, 604);
    xicaraGasta[1] = carregarPeca("comum/xicara1_gasta", 1088, 604);
    xicaraGasta[2] = carregarPeca("comum/xicara2_gasta", 1168, 604);

    // placas de cada nível: <pasta>/placa_ABC.png (uma por combinação) ou <pasta>/placa.png (fios escondidos)
    for (int e = 0; e < QTD_ENIGMAS; e++)
    {
        const Enigma *en = &ENIGMAS[e];
        if (!en->placaPorEstado)
        {
            placas[e][0] = carregarPeca(TextFormat("%s/placa", en->pasta), 424, 144);
            continue;
        }
        for (int i = 0; i < (1 << en->qtdChaves); i++)
        {
            char bits[MAX_CHAVES + 1];
            for (int k = 0; k < en->qtdChaves; k++)
                bits[k] = ((i >> (en->qtdChaves - 1 - k)) & 1) ? '1' : '0';
            bits[en->qtdChaves] = '\0';
            placas[e][i] = carregarPeca(TextFormat("%s/placa_%s", en->pasta, bits), 424, 144);
        }
    }

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
    Peca comuns[] = { balcao, postit, visor, corpo, cafe, fumaca, chaveLigada, chaveDesligada };
    for (int i = 0; i < (int)(sizeof(comuns) / sizeof(comuns[0])); i++) UnloadTexture(comuns[i].textura);
    for (int k = 0; k < 3; k++) { UnloadTexture(xicaraCheia[k].textura); UnloadTexture(xicaraGasta[k].textura); }
    for (int e = 0; e < QTD_ENIGMAS; e++)
    {
        const Enigma *en = &ENIGMAS[e];
        int qtdPlacas = en->placaPorEstado ? (1 << en->qtdChaves) : 1;
        for (int i = 0; i < qtdPlacas; i++) UnloadTexture(placas[e][i].textura);
        for (int k = 0; k < en->qtdLuzes; k++)
        {
            UnloadTexture(lampApagada[e][k].textura);
            UnloadTexture(lampVerde[e][k].textura);
            UnloadTexture(lampVermelha[e][k].textura);
        }
        if (en->qtdLuzes > 1) UnloadTexture(cabecaDuasLuzes[e].textura);
    }
    UnloadFont(fonteVisor); UnloadFont(fontePostit); UnloadFont(fonteTentativas);
    for (int f = 0; f < 3; f++) UnloadFont(fonteRotulo[f]);
}


// ##########################################################
// FASE / RESET
// ##########################################################
static void zerarEnigma(void)
{
    for (int k = 0; k < MAX_CHAVES; k++) chave[k] = false;
    for (int k = 0; k < MAX_LUZES; k++) ultimaLuz[k] = false;
    tentativas = 3;
    estado = JOGANDO;
    tempoLuzVermelha = 0.0f;
    soUmaLuz = false;
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
// CAFÉ FORTE: cada medidor anda 15 pontos rumo ao 50 (sem passar)
// ##########################################################
static int aproximarDe50(int v)
{
    if (v < 50) { v += 15; if (v > 50) v = 50; }
    else if (v > 50) { v -= 15; if (v < 50) v = 50; }
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
        agendarTextoUI("Máquina indisponível até a próxima fase", larguraResolucao - 460, 66, 30, GRAY, false, 0);
        return;
    }

    agendarTextoUI("Pressione E para consertar a máquina", larguraResolucao - 430, 66, 30, GREEN, false, 0);

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
    bool sobreChave[MAX_CHAVES] = { false };
    bool sobreTestar = false;

    if (podeMexer)
    {
        for (int k = 0; k < enigma->qtdChaves; k++)
        {
            Rectangle area = { enigma->xChave[k] - 32, 456, 64, 96 };
            if (CheckCollisionPointRec(mouse, area))
            {
                emCimaDeAlgo = true;
                sobreChave[k] = true;
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
            sobreTestar = true;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                bool luz[MAX_LUZES] = { false };
                enigma->calcularLuzes(chave, luz);
                int acesas = 0;
                for (int k = 0; k < enigma->qtdLuzes; k++)
                {
                    ultimaLuz[k] = luz[k];
                    if (luz[k]) acesas++;
                }

                if (acesas == enigma->qtdLuzes)
                {
                    estado = ACERTOU;
                    tomarCafeForte();
                }
                else
                {
                    tentativas--;
                    tempoLuzVermelha = 1.2f;
                    soUmaLuz = (acesas > 0);
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

    // botao TESTAR: faz parte da imagem do corpo, entao redesenha o trecho
    // ampliado para ter a mesma animacao padrao dos outros botoes
    {
        Rectangle origemTestar = { AREA_TESTAR.x - corpo.x, AREA_TESTAR.y - corpo.y, AREA_TESTAR.width, AREA_TESTAR.height };
        Rectangle destinoTestar = retanguloAnimadoBotao("maquina.testar", AREA_TESTAR, sobreTestar);
        DrawTexturePro(corpo.textura, origemTestar, destinoTestar, (Vector2){ 0, 0 }, 0, WHITE);
    }
    if (enigma->qtdLuzes > 1) desenharPeca(cabecaDuasLuzes[enigmaAtual]);

    int indicePlaca = 0;
    if (enigma->placaPorEstado)
        for (int k = 0; k < enigma->qtdChaves; k++)
            indicePlaca = (indicePlaca << 1) | (chave[k] ? 1 : 0);
    desenharPeca(placas[enigmaAtual][indicePlaca]);

    for (int k = 0; k < enigma->qtdLuzes; k++)
    {
        if (estado == ACERTOU)
            desenharPeca(lampVerde[enigmaAtual][k]);
        else if (tempoLuzVermelha > 0)
            desenharPeca(ultimaLuz[k] ? lampVerde[enigmaAtual][k] : lampVermelha[enigmaAtual][k]);
        else
            desenharPeca(lampApagada[enigmaAtual][k]);
    }

    for (int k = 0; k < enigma->qtdChaves; k++)
    {
        Peca c = chave[k] ? chaveLigada : chaveDesligada;
        c.x = enigma->xChave[k] - 12;

        // chave com a animacao padrao dos botoes
        Rectangle baseChave = { (float)c.x, (float)c.y, (float)c.textura.width, (float)c.textura.height };
        Rectangle destinoChave = retanguloAnimadoBotao(TextFormat("maquina.chave%d", k), baseChave, sobreChave[k]);
        DrawTexturePro(c.textura, (Rectangle){ 0, 0, (float)c.textura.width, (float)c.textura.height },
                       destinoChave, (Vector2){ 0, 0 }, 0, WHITE);
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
    if (enigma->letrasNoPainel)
    {
        const char *nomesChaves[MAX_CHAVES] = { "A", "B", "C", "D", "E" };
        for (int k = 0; k < enigma->qtdChaves; k++)
            texto(fonteRotulo[2], nomesChaves[k], enigma->xChave[k] + 20, 496, (Color){ 20, 20, 26, 255 }, false);
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
    if (estado == ERROU && soUmaLuz)
    {
        texto(fonteVisor, "SÓ UMA LUZ", vx, vy, vermelho, false);
        texto(fonteVisor, "ACENDEU", vx, vy + 26, vermelho, false);
        texto(fonteVisor, TextFormat("restam %d", tentativas), vx, vy + 78, vermelho, false);
        texto(fonteVisor, tentativas == 1 ? "tentativa" : "tentativas", vx, vy + 104, vermelho, false);
    }
    else if (estado == ERROU)
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
