#include "raylib.h"
#include <stdio.h>
#include <stdbool.h>

#define FONTE_TEXTO 0
#define FONTE_DESTAQUE 1
#define RANKING_MAXIMO 20
#define RANKING_ARQUIVO "ranking.txt"
#define CHEGOU_VITORIA 5

float medirTextoUI(const char *texto, float tamanho, int fonte);
void desenharTextoUI(const char *texto, float x, float y, float tamanho, Color cor, int fonte);
int obterPontuacao(void);
int obterCargo(void);
bool jogadorVenceu(void);

typedef struct
{
    char nome[4];
    int pontos;
    int chegou;
} Recorde;

static Recorde recordes[RANKING_MAXIMO];
static int totalRecordes = 0;
static int posicaoDestaque = -1;

static bool rankingAberto = false;
static bool digitando = false;
static bool registrado = false;
static char nomeDigitado[4] = "";
static int letrasDigitadas = 0;
static float tempoRanking = 0.0f;

static const Color COR_OURO = {242, 193, 78, 255};
static const Color COR_PRATA = {200, 204, 214, 255};
static const Color COR_BRONZE = {208, 138, 74, 255};
static const Color COR_CREME = {246, 239, 220, 255};
static const Color COR_AZUL = {106, 138, 216, 255};
static const Color COR_APAGADA = {122, 128, 160, 255};

static const char *textoChegou(int chegou)
{
    if (chegou == CHEGOU_VITORIA) return "VITÓRIA";
    if (chegou == 4) return "TECH LÍDER";
    if (chegou == 3) return "SÊNIOR";
    if (chegou == 2) return "PLENO";
    if (chegou == 1) return "JÚNIOR";
    return "ESTAGIÁRIO";
}

static void carregarRanking(void)
{
    totalRecordes = 0;

    FILE *arquivo = fopen(RANKING_ARQUIVO, "r");

    if (arquivo == NULL)
        return;

    char nome[16];
    int pontos;
    int chegou;

    while (totalRecordes < RANKING_MAXIMO && fscanf(arquivo, "%15s %d %d", nome, &pontos, &chegou) == 3)
    {
        Recorde *r = &recordes[totalRecordes];

        for (int i = 0; i < 3; i++)
            r->nome[i] = nome[i] ? nome[i] : '?';

        r->nome[3] = '\0';
        r->pontos = pontos;
        r->chegou = chegou;
        totalRecordes++;
    }

    fclose(arquivo);
}

static void salvarRanking(void)
{
    FILE *arquivo = fopen(RANKING_ARQUIVO, "w");

    if (arquivo == NULL)
        return;

    for (int i = 0; i < totalRecordes; i++)
        fprintf(arquivo, "%s %d %d\n", recordes[i].nome, recordes[i].pontos, recordes[i].chegou);

    fclose(arquivo);
}

static void registrarRecorde(void)
{
    carregarRanking();

    Recorde novo;

    for (int i = 0; i < 4; i++)
        novo.nome[i] = nomeDigitado[i];

    novo.pontos = obterPontuacao();
    novo.chegou = jogadorVenceu() ? CHEGOU_VITORIA : obterCargo();

    int posicao = totalRecordes;

    while (posicao > 0 && recordes[posicao - 1].pontos < novo.pontos)
        posicao--;

    registrado = true;
    posicaoDestaque = -1;

    if (posicao >= RANKING_MAXIMO)
        return;

    int ultimo = totalRecordes < RANKING_MAXIMO ? totalRecordes : RANKING_MAXIMO - 1;

    for (int i = ultimo; i > posicao; i--)
        recordes[i] = recordes[i - 1];

    recordes[posicao] = novo;

    if (totalRecordes < RANKING_MAXIMO)
        totalRecordes++;

    posicaoDestaque = posicao;
    salvarRanking();
}

static void retangulo(float x, float y, float largura, float altura, Color cor, float e, float ox, float oy)
{
    DrawRectangle((int)(ox + x * e), (int)(oy + y * e), (int)(largura * e + 0.5f), (int)(altura * e + 0.5f), cor);
}

static void texto(const char *conteudo, float x, float y, float tamanho, Color cor, int fonte, float e, float ox, float oy)
{
    desenharTextoUI(conteudo, ox + x * e, oy + y * e, tamanho * e, cor, fonte);
}

static void textoCentro(const char *conteudo, float y, float tamanho, Color cor, int fonte, float e, float ox, float oy)
{
    float largura = medirTextoUI(conteudo, tamanho * e, fonte);
    desenharTextoUI(conteudo, ox + 640 * e - largura / 2, oy + y * e, tamanho * e, cor, fonte);
}

static void desenharFundo(float e, float ox, float oy)
{
    DrawRectangleGradientV((int)ox, (int)oy, (int)(1280 * e), (int)(720 * e), (Color){28, 34, 56, 255}, (Color){11, 14, 24, 255});

    for (int y = 0; y < 720; y += 4)
        retangulo(0, (float)y, 1280, 1, (Color){0, 0, 0, 45}, e, ox, oy);
}

static void desenharTaca(float x, float y, Color cor, float e, float ox, float oy)
{
    retangulo(x + 3, y, 14, 8, cor, e, ox, oy);
    retangulo(x + 5, y + 8, 10, 2, cor, e, ox, oy);
    retangulo(x, y + 1, 3, 2, cor, e, ox, oy);
    retangulo(x, y + 3, 2, 3, cor, e, ox, oy);
    retangulo(x + 17, y + 1, 3, 2, cor, e, ox, oy);
    retangulo(x + 18, y + 3, 2, 3, cor, e, ox, oy);
    retangulo(x + 8, y + 10, 4, 3, cor, e, ox, oy);
    retangulo(x + 5, y + 13, 10, 3, cor, e, ox, oy);
}

static void desenharTabela(const char *rodape, float e, float ox, float oy)
{
    desenharFundo(e, ox, oy);

    textoCentro("MELHORES DA ELSE", 38, 34, (Color){0, 0, 0, 255}, FONTE_DESTAQUE, e, ox + 4 * e, oy + 4 * e);
    textoCentro("MELHORES DA ELSE", 38, 34, COR_OURO, FONTE_DESTAQUE, e, ox, oy);

    texto("POS", 395, 106, 12, COR_AZUL, FONTE_DESTAQUE, e, ox, oy);
    texto("PONTOS", 485, 106, 12, COR_AZUL, FONTE_DESTAQUE, e, ox, oy);
    texto("NOME", 625, 106, 12, COR_AZUL, FONTE_DESTAQUE, e, ox, oy);
    texto("CHEGOU A", 735, 106, 12, COR_AZUL, FONTE_DESTAQUE, e, ox, oy);

    if (totalRecordes == 0)
        textoCentro("NINGUÉM JOGOU AINDA", 330, 18, COR_CREME, FONTE_DESTAQUE, e, ox, oy);

    bool apagada = ((int)(tempoRanking / 0.35f)) % 2 == 1;

    for (int i = 0; i < totalRecordes; i++)
    {
        float y = 134 + i * 25;
        Color cor = COR_CREME;

        if (i == 0) cor = COR_OURO;
        if (i == 1) cor = COR_PRATA;
        if (i == 2) cor = COR_BRONZE;

        if (i == posicaoDestaque && apagada)
            cor.a = 50;

        if (i < 3)
            desenharTaca(345, y - 1, cor, e, ox, oy);

        texto(TextFormat("%dº", i + 1), 395, y, 16, cor, FONTE_DESTAQUE, e, ox, oy);
        texto(TextFormat("%05d", recordes[i].pontos), 485, y, 16, cor, FONTE_DESTAQUE, e, ox, oy);
        texto(recordes[i].nome, 625, y, 16, cor, FONTE_DESTAQUE, e, ox, oy);
        texto(textoChegou(recordes[i].chegou), 735, y, 16, cor, FONTE_DESTAQUE, e, ox, oy);
    }

    Color corRodape = COR_CREME;

    if (((int)(tempoRanking / 0.5f)) % 2 == 1)
        corRodape.a = 50;

    textoCentro(rodape, 664, 14, corRodape, FONTE_DESTAQUE, e, ox, oy);
}

static void desenharDigitar(float e, float ox, float oy)
{
    desenharFundo(e, ox, oy);

    textoCentro("SUA PONTUAÇÃO", 150, 16, COR_AZUL, FONTE_DESTAQUE, e, ox, oy);
    textoCentro(TextFormat("%05d", obterPontuacao()), 196, 56, (Color){0, 0, 0, 255}, FONTE_DESTAQUE, e, ox + 5 * e, oy + 5 * e);
    textoCentro(TextFormat("%05d", obterPontuacao()), 196, 56, COR_OURO, FONTE_DESTAQUE, e, ox, oy);
    textoCentro(jogadorVenceu() ? "VITÓRIA" : textoChegou(obterCargo()), 284, 16, COR_CREME, FONTE_DESTAQUE, e, ox, oy);

    textoCentro("DIGITE SUAS 3 LETRAS", 372, 16, COR_CREME, FONTE_DESTAQUE, e, ox, oy);

    bool cursorAceso = ((int)(tempoRanking / 0.4f)) % 2 == 0;

    for (int i = 0; i < 3; i++)
    {
        float x = 640 - 150 + i * 110;
        Color linha = i == letrasDigitadas && cursorAceso ? COR_OURO : COR_APAGADA;

        retangulo(x, 494, 80, 6, linha, e, ox, oy);

        if (i < letrasDigitadas)
        {
            char letra[2] = {nomeDigitado[i], '\0'};
            float largura = medirTextoUI(letra, 56 * e, FONTE_DESTAQUE);
            desenharTextoUI(letra, ox + (x + 40) * e - largura / 2, oy + 424 * e, 56 * e, COR_CREME, FONTE_DESTAQUE);
        }
    }

    if (letrasDigitadas == 3)
    {
        Color cor = COR_CREME;

        if (((int)(tempoRanking / 0.5f)) % 2 == 1)
            cor.a = 50;

        textoCentro("ENTER PARA CONFIRMAR", 600, 14, cor, FONTE_DESTAQUE, e, ox, oy);
    }
    else
    {
        textoCentro("BACKSPACE APAGA", 600, 12, COR_APAGADA, FONTE_DESTAQUE, e, ox, oy);
    }
}

static bool pediuParaSair(void)
{
    if (tempoRanking < 0.25f)
        return false;

    return
        IsKeyPressed(KEY_E) ||
        IsKeyPressed(KEY_ENTER) ||
        IsKeyPressed(KEY_ESCAPE) ||
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

void abrirRankingFim(void)
{
    rankingAberto = true;
    tempoRanking = 0.0f;

    if (!registrado)
    {
        digitando = true;
        letrasDigitadas = 0;
        nomeDigitado[0] = '\0';

        while (GetCharPressed() > 0)
        {
        }
    }
    else
    {
        carregarRanking();
    }
}

bool rankingFimAberto(void)
{
    return rankingAberto;
}

void novaPartidaRanking(void)
{
    rankingAberto = false;
    digitando = false;
    registrado = false;
    posicaoDestaque = -1;
}

void desenharRankingFim(float e, float ox, float oy)
{
    if (!rankingAberto)
        return;

    tempoRanking += GetFrameTime();

    if (digitando)
    {
        int letra = GetCharPressed();

        while (letra > 0)
        {
            if (letra >= 'a' && letra <= 'z')
                letra -= 32;

            if (letra >= 'A' && letra <= 'Z' && letrasDigitadas < 3)
            {
                nomeDigitado[letrasDigitadas++] = (char)letra;
                nomeDigitado[letrasDigitadas] = '\0';
            }

            letra = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) && letrasDigitadas > 0)
            nomeDigitado[--letrasDigitadas] = '\0';

        if (IsKeyPressed(KEY_ENTER) && letrasDigitadas == 3)
        {
            registrarRecorde();
            digitando = false;
            tempoRanking = 0.0f;
        }

        desenharDigitar(e, ox, oy);
        return;
    }

    if (pediuParaSair())
    {
        rankingAberto = false;
        return;
    }

    desenharTabela("APERTE E PARA CONTINUAR", e, ox, oy);
}

void abrirRankingMenu(void)
{
    carregarRanking();
    posicaoDestaque = -1;
    tempoRanking = 0.0f;
}

bool desenharRankingMenu(float e, float ox, float oy)
{
    tempoRanking += GetFrameTime();

    if (pediuParaSair())
        return true;

    desenharTabela("APERTE E PARA VOLTAR", e, ox, oy);
    return false;
}
