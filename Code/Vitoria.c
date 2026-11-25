#include "raylib.h"
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define FONTE_TEXTO 0
#define FONTE_DESTAQUE 1

#define VITORIA_CERTIFICADO 0
#define VITORIA_ANOS_DEPOIS 1
#define VITORIA_CABANA 2

#define VITORIA_NADA 0
#define VITORIA_REINICIAR 1
#define VITORIA_COMPARTILHAR 2
#define VITORIA_SAIR 3

#define TOTAL_CONFETES 70
#define MAX_LINHAS 8

float medirTextoUI(const char *texto, float tamanho, int fonte);
void desenharTextoUI(const char *texto, float x, float y, float tamanho, Color cor, int fonte);
Rectangle retanguloAnimadoBotao(const char *nome, Rectangle base, bool hover);
int obterValorMedidor(int medidor);
void tocarSomVitoria(void);

static Texture2D fundoCertificado = {0};
static Texture2D fundoCabana = {0};
static Texture2D botaoContinuar = {0};
static Texture2D botaoReiniciar = {0};
static Texture2D botaoCompartilhar = {0};
static Texture2D botaoSair = {0};

static bool vitoriaIniciada = false;
static int etapa = VITORIA_CERTIFICADO;
static float tempoEtapa = 0.0f;
static float tempoLetra = 0.0f;
static int letrasMostradas = 0;

static float confeteX[TOTAL_CONFETES];
static float confeteY[TOTAL_CONFETES];
static float confeteVelocidade[TOTAL_CONFETES];
static float confeteGiro[TOTAL_CONFETES];
static int confeteCor[TOTAL_CONFETES];
static int confeteLargura[TOTAL_CONFETES];

static const Color CORES_CONFETE[6] = {
    {224, 90, 60, 255},
    {242, 193, 78, 255},
    {79, 179, 160, 255},
    {90, 138, 216, 255},
    {200, 106, 208, 255},
    {255, 255, 255, 255}
};

static const char *fraseCertificado = "Conferido a quem liderou a Else com ética, equilibrando confiança, viés, privacidade e lucro.";
static const char *legendaCabana = "Você se aposentou e foi morar no mato. O certificado foi pra parede da cabana, o crachá ficou num prego e o único alarme agora é o galo.";
static const char *nomesMedidores[4] = {"Confiança", "Viés", "Privacidade", "Lucro"};

static const Rectangle AREA_BOTAO = {30, 207, 447, 112};

static int contarLetrasVitoria(const char *texto, int bytes)
{
    int letras = 0;

    for (int i = 0; i < bytes; i++)
    {
        if (((unsigned char)texto[i] & 0xC0) != 0x80)
            letras++;
    }

    return letras;
}

static int bytesDasLetras(const char *texto, int bytes, int letras)
{
    int contadas = 0;

    for (int i = 0; i < bytes; i++)
    {
        if (((unsigned char)texto[i] & 0xC0) != 0x80)
        {
            if (contadas == letras)
                return i;

            contadas++;
        }
    }

    return bytes;
}

static int quebrarLinhas(const char *texto, float largura, float tamanho, int fonte, int inicio[], int tamanhoLinha[])
{
    int total = 0;
    int comeco = 0;
    int ultimoEspaco = -1;
    int comprimento = (int)strlen(texto);
    char pedaco[256];

    for (int i = 0; i <= comprimento && total < MAX_LINHAS; i++)
    {
        if (texto[i] != ' ' && texto[i] != '\0')
            continue;

        int fim = i - comeco;

        if (fim > 255)
            fim = 255;

        memcpy(pedaco, texto + comeco, fim);
        pedaco[fim] = '\0';

        if (medirTextoUI(pedaco, tamanho, fonte) > largura && ultimoEspaco > comeco)
        {
            inicio[total] = comeco;
            tamanhoLinha[total] = ultimoEspaco - comeco;
            total++;
            comeco = ultimoEspaco + 1;
        }

        ultimoEspaco = i;

        if (texto[i] == '\0' && total < MAX_LINHAS)
        {
            inicio[total] = comeco;
            tamanhoLinha[total] = comprimento - comeco;
            total++;
        }
    }

    return total;
}

static void retanguloVitoria(float x, float y, float largura, float altura, Color cor, float e, float ox, float oy)
{
    DrawRectangle((int)(ox + x * e), (int)(oy + y * e), (int)(largura * e + 0.5f), (int)(altura * e + 0.5f), cor);
}

static void textoCentroVitoria(const char *texto, float cx, float y, float tamanho, Color cor, int fonte, float e, float ox, float oy)
{
    float largura = medirTextoUI(texto, tamanho * e, fonte);
    desenharTextoUI(texto, ox + cx * e - largura / 2, oy + y * e, tamanho * e, cor, fonte);
}

static void desenharFundoInteiro(Texture2D textura, float e, float ox, float oy)
{
    DrawTexturePro(
        textura,
        (Rectangle){0, 0, (float)textura.width, (float)textura.height},
        (Rectangle){ox, oy, 1280 * e, 720 * e},
        (Vector2){0, 0},
        0,
        WHITE
    );
}

static void escurecer(float alfa, float e, float ox, float oy)
{
    if (alfa <= 0.0f)
        return;

    if (alfa > 1.0f)
        alfa = 1.0f;

    retanguloVitoria(0, 0, 1280, 720, Fade(BLACK, alfa), e, ox, oy);
}

static bool botaoClicado(const char *nome, Texture2D textura, float x, float y, float e, float ox, float oy)
{
    Rectangle area = {ox + x * e, oy + y * e, 224 * e, 56 * e};
    bool sobre = CheckCollisionPointRec(GetMousePosition(), area);
    Rectangle desenho = retanguloAnimadoBotao(nome, area, sobre);

    DrawTexturePro(textura, AREA_BOTAO, desenho, (Vector2){0, 0}, 0, WHITE);

    return sobre && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

static bool apertouAvancar(void)
{
    return
        IsKeyPressed(KEY_E) ||
        IsKeyPressed(KEY_ENTER) ||
        IsKeyPressed(KEY_SPACE) ||
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

static void sortearConfete(int i, bool emQualquerAltura)
{
    confeteX[i] = (float)GetRandomValue(0, 319);
    confeteY[i] = emQualquerAltura ? (float)GetRandomValue(0, 179) : -4.0f;
    confeteVelocidade[i] = 0.35f + GetRandomValue(0, 50) / 100.0f;
    confeteGiro[i] = GetRandomValue(0, 628) / 100.0f;
    confeteCor[i] = GetRandomValue(0, 5);
    confeteLargura[i] = GetRandomValue(1, 2);
}

static void carregarTexturasVitoria(void)
{
    if (fundoCertificado.id != 0)
        return;

    fundoCertificado = LoadTexture("cenario/Telas/vitoriaCertificado.png");
    fundoCabana = LoadTexture("cenario/Telas/vitoriaCabana.png");
    botaoContinuar = LoadTexture("Botoes/BotaoContinue.png");
    botaoReiniciar = LoadTexture("Botoes/botaoPlayAgain.png");
    botaoCompartilhar = LoadTexture("Botoes/botaoShare.png");
    botaoSair = LoadTexture("Botoes/botaoExit.png");
}

static void desenharCertificado(float e, float ox, float oy)
{
    desenharFundoInteiro(fundoCertificado, e, ox, oy);

    Color azul = {29, 58, 138, 255};
    Color tinta = {42, 36, 24, 255};

    textoCentroVitoria("CERTIFICADO", 640, 120, 34, azul, FONTE_DESTAQUE, e, ox, oy);
    textoCentroVitoria("DE RECONHECIMENTO", 640, 168, 13, (Color){138, 106, 42, 255}, FONTE_DESTAQUE, e, ox, oy);
    retanguloVitoria(470, 194, 340, 3, (Color){200, 164, 90, 255}, e, ox, oy);

    int inicio[MAX_LINHAS];
    int tamanhoLinha[MAX_LINHAS];
    int linhas = quebrarLinhas(fraseCertificado, 500 * e, 27 * e, FONTE_TEXTO, inicio, tamanhoLinha);
    char linha[256];

    for (int i = 0; i < linhas; i++)
    {
        memcpy(linha, fraseCertificado + inicio[i], tamanhoLinha[i]);
        linha[tamanhoLinha[i]] = '\0';
        textoCentroVitoria(linha, 640, 206 + i * 26, 27, tinta, FONTE_TEXTO, e, ox, oy);
    }

    for (int i = 0; i < 4; i++)
    {
        float x = i % 2 == 0 ? 420 : 650;
        float y = 304 + (i / 2) * 30;
        int valor = obterValorMedidor(i + 1);

        desenharTextoUI(nomesMedidores[i], ox + x * e, oy + (y - 5) * e, 22 * e, tinta, FONTE_TEXTO);
        retanguloVitoria(x + 110, y, 104, 14, tinta, e, ox, oy);
        retanguloVitoria(x + 112, y + 2, 100, 10, (Color){230, 220, 192, 255}, e, ox, oy);
        retanguloVitoria(x + 112, y + 2, (float)valor, 10, (Color){79, 179, 160, 255}, e, ox, oy);
    }

    textoCentroVitoria("Bia", 560, 384, 36, azul, FONTE_TEXTO, e, ox, oy);
    retanguloVitoria(480, 420, 160, 2, tinta, e, ox, oy);
    textoCentroVitoria("RH Else", 560, 426, 22, (Color){106, 90, 58, 255}, FONTE_TEXTO, e, ox, oy);

    float dt = GetFrameTime() * 60.0f;

    for (int i = 0; i < TOTAL_CONFETES; i++)
    {
        confeteY[i] += confeteVelocidade[i] * dt;
        confeteGiro[i] += 0.08f * dt;
        confeteX[i] += sinf(confeteGiro[i]) * 0.4f * dt;

        if (confeteY[i] > 182)
            sortearConfete(i, false);

        float altura = sinf(confeteGiro[i]) > 0 ? 2.0f : 1.0f;

        retanguloVitoria(
            floorf(confeteX[i]) * 4,
            floorf(confeteY[i]) * 4,
            confeteLargura[i] * 4.0f,
            altura * 4,
            CORES_CONFETE[confeteCor[i]],
            e, ox, oy
        );
    }
}

static void desenharCabana(float e, float ox, float oy)
{
    desenharFundoInteiro(fundoCabana, e, ox, oy);

    int inicio[MAX_LINHAS];
    int tamanhoLinha[MAX_LINHAS];
    int bytes = (int)strlen(legendaCabana);
    int linhas = quebrarLinhas(legendaCabana, 622 * e, 28 * e, FONTE_TEXTO, inicio, tamanhoLinha);
    float altura = 44 + linhas * 30;

    retanguloVitoria(38, 36, 666, altura, (Color){13, 16, 32, 230}, e, ox, oy);
    DrawRectangleLinesEx((Rectangle){ox + 38 * e, oy + 36 * e, 666 * e, altura * e}, 4 * e, WHITE);

    int restantes = letrasMostradas;
    char linha[256];

    for (int i = 0; i < linhas && restantes > 0; i++)
    {
        const char *comeco = legendaCabana + inicio[i];
        int letrasLinha = contarLetrasVitoria(comeco, tamanhoLinha[i]);
        int usar = restantes < letrasLinha ? restantes : letrasLinha;
        int corte = bytesDasLetras(comeco, tamanhoLinha[i], usar);

        memcpy(linha, comeco, corte);
        linha[corte] = '\0';
        desenharTextoUI(linha, ox + 60 * e, oy + (58 + i * 30) * e, 28 * e, WHITE, FONTE_TEXTO);

        restantes -= letrasLinha + 1;
    }

    int total = contarLetrasVitoria(legendaCabana, bytes);

    if (letrasMostradas < total || ((int)(tempoEtapa / 0.5f)) % 2 == 0)
    {
        int ultima = linhas > 0 ? linhas - 1 : 0;
        int letrasAntes = 0;
        int linhaCursor = 0;

        for (int i = 0; i < linhas; i++)
        {
            int letrasLinha = contarLetrasVitoria(legendaCabana + inicio[i], tamanhoLinha[i]);

            if (letrasMostradas <= letrasAntes + letrasLinha || i == ultima)
            {
                linhaCursor = i;
                break;
            }

            letrasAntes += letrasLinha + 1;
        }

        int usar = letrasMostradas - letrasAntes;
        int letrasLinha = contarLetrasVitoria(legendaCabana + inicio[linhaCursor], tamanhoLinha[linhaCursor]);

        if (usar > letrasLinha)
            usar = letrasLinha;

        if (usar < 0)
            usar = 0;

        int corte = bytesDasLetras(legendaCabana + inicio[linhaCursor], tamanhoLinha[linhaCursor], usar);

        memcpy(linha, legendaCabana + inicio[linhaCursor], corte);
        linha[corte] = '\0';

        float cursorX = 60 + medirTextoUI(linha, 28 * e, FONTE_TEXTO) / e + 2;

        if (tempoEtapa > 0.9f)
            retanguloVitoria(cursorX, 62 + linhaCursor * 30, 12, 22, WHITE, e, ox, oy);
    }
}

void reiniciarTelaVitoria(void)
{
    vitoriaIniciada = false;
}

int telaVitoria(float e, float ox, float oy, bool bloqueada)
{
    carregarTexturasVitoria();

    if (!vitoriaIniciada)
    {
        vitoriaIniciada = true;
        etapa = VITORIA_CERTIFICADO;
        tempoEtapa = 0.0f;
        tempoLetra = 0.0f;
        letrasMostradas = 0;

        for (int i = 0; i < TOTAL_CONFETES; i++)
            sortearConfete(i, true);

        tocarSomVitoria();
    }

    tempoEtapa += GetFrameTime();

    retanguloVitoria(0, 0, 1280, 720, BLACK, e, ox, oy);

    int acao = VITORIA_NADA;

    if (etapa == VITORIA_CERTIFICADO)
    {
        desenharCertificado(e, ox, oy);

        bool clicouContinuar = botaoClicado("vitoria.continuar", botaoContinuar, 1018, 628, e, ox, oy);

        escurecer(1.0f - tempoEtapa / 0.8f, e, ox, oy);

        if (!bloqueada && tempoEtapa > 0.8f && (clicouContinuar || apertouAvancar()))
        {
            etapa = VITORIA_ANOS_DEPOIS;
            tempoEtapa = 0.0f;
        }
    }
    else if (etapa == VITORIA_ANOS_DEPOIS)
    {
        float alfa = (tempoEtapa - 0.3f) / 1.0f;

        if (alfa < 0.0f) alfa = 0.0f;
        if (alfa > 1.0f) alfa = 1.0f;

        textoCentroVitoria("ANOS DEPOIS...", 640, 347, 26, Fade(WHITE, alfa), FONTE_DESTAQUE, e, ox, oy);

        if (!bloqueada && (tempoEtapa > 2.6f || (tempoEtapa > 0.4f && apertouAvancar())))
        {
            etapa = VITORIA_CABANA;
            tempoEtapa = 0.0f;
            tempoLetra = 0.0f;
            letrasMostradas = 0;
        }
    }
    else
    {
        int total = contarLetrasVitoria(legendaCabana, (int)strlen(legendaCabana));

        if (tempoEtapa > 0.9f && letrasMostradas < total)
        {
            tempoLetra += GetFrameTime();

            while (tempoLetra >= 0.038f && letrasMostradas < total)
            {
                tempoLetra -= 0.038f;
                letrasMostradas++;
            }
        }

        desenharCabana(e, ox, oy);
        escurecer(1.0f - tempoEtapa / 0.8f, e, ox, oy);

        if (letrasMostradas < total)
        {
            if (!bloqueada && tempoEtapa > 0.9f && apertouAvancar())
                letrasMostradas = total;
        }
        else
        {
            bool reiniciar = botaoClicado("vitoria.reiniciar", botaoReiniciar, 530, 628, e, ox, oy);
            bool compartilhar = botaoClicado("vitoria.compartilhar", botaoCompartilhar, 774, 628, e, ox, oy);
            bool sair = botaoClicado("vitoria.sair", botaoSair, 1018, 628, e, ox, oy);

            if (!bloqueada)
            {
                if (reiniciar) acao = VITORIA_REINICIAR;
                if (compartilhar) acao = VITORIA_COMPARTILHAR;
                if (sair) acao = VITORIA_SAIR;
            }
        }
    }

    return acao;
}
