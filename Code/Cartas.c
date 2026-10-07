#include "raylib.h"
#include <stdbool.h>
#include <string.h>

typedef struct Evento
{
    int id;
    int cargo;
    const char *titulo;
    const char *texto;
    const char *esquerda;
    const char *direita;
    int experienciaEsquerda;
    int experienciaDireita;
    int confiancaEsquerda;
    int confiancaDireita;
    int viesEsquerda;
    int viesDireita;
    int privacidadeEsquerda;
    int privacidadeDireita;
    int lucroEsquerda;
    int lucroDireita;
} Evento;

Evento *obterBancoPerguntas(int *quantidade);

static Evento *banco = 0;
static Evento *eventoAtual = 0;
static int quantidadeEventos = 0;

static int cargo = 0;
static int experiencia = 0;

static int confianca = 50;
static int vies = 50;
static int privacidade = 50;
static int lucro = 50;

static bool cartaAberta = false;

static int historico[8] = {-1,-1,-1,-1,-1,-1,-1,-1};

const char *nomeCargo()
{
    if (cargo == 0) return "Estagiario";
    if (cargo == 1) return "Assistente";
    if (cargo == 2) return "Analista Junior";
    if (cargo == 3) return "Analista Pleno";
    if (cargo == 4) return "Analista Senior";
    if (cargo == 5) return "Supervisor";
    return "Gerente";
}

bool cartasEstaoAbertas()
{
    return cartaAberta;
}

int obterConfianca()
{
    return confianca;
}

int obterVies()
{
    return vies;
}

int obterPrivacidade()
{
    return privacidade;
}

int obterLucro()
{
    return lucro;
}

bool eventoFoiRecente(int id)
{
    for (int i = 0; i < 8; i++)
    {
        if (historico[i] == id)
            return true;
    }
    return false;
}

void adicionarHistorico(int id)
{
    for (int i = 7; i > 0; i--)
        historico[i] = historico[i - 1];

    historico[0] = id;
}

void limitarAtributos()
{
    if (confianca < 0) confianca = 0;
    if (confianca > 100) confianca = 100;

    if (vies < 0) vies = 0;
    if (vies > 100) vies = 100;

    if (privacidade < 0) privacidade = 0;
    if (privacidade > 100) privacidade = 100;

    if (lucro < 0) lucro = 0;
    if (lucro > 100) lucro = 100;
}

void verificarPromocao()
{
    if (experiencia >= 1700) cargo = 6;
    else if (experiencia >= 1200) cargo = 5;
    else if (experiencia >= 800) cargo = 4;
    else if (experiencia >= 500) cargo = 3;
    else if (experiencia >= 250) cargo = 2;
    else if (experiencia >= 100) cargo = 1;
    else cargo = 0;
}

void sortearNovaCarta()
{
    int disponiveis[100];
    int quantidadeDisponiveis = 0;

    for (int i = 0; i < quantidadeEventos; i++)
    {
        if (banco[i].cargo == cargo && !eventoFoiRecente(banco[i].id))
        {
            disponiveis[quantidadeDisponiveis] = i;
            quantidadeDisponiveis++;
        }
    }

    if (quantidadeDisponiveis == 0)
    {
        for (int i = 0; i < 8; i++)
            historico[i] = -1;

        for (int i = 0; i < quantidadeEventos; i++)
        {
            if (banco[i].cargo == cargo)
            {
                disponiveis[quantidadeDisponiveis] = i;
                quantidadeDisponiveis++;
            }
        }
    }

    if (quantidadeDisponiveis > 0)
    {
        int sorteado = GetRandomValue(0, quantidadeDisponiveis - 1);
        eventoAtual = &banco[disponiveis[sorteado]];
        adicionarHistorico(eventoAtual->id);
    }
}

void iniciarCartas()
{
    banco = obterBancoPerguntas(&quantidadeEventos);

    cargo = 0;
    experiencia = 0;
    confianca = 50;
    vies = 50;
    privacidade = 50;
    lucro = 50;
    eventoAtual = 0;
    cartaAberta = false;

    for (int i = 0; i < 8; i++)
        historico[i] = -1;
}

void escolherEsquerda()
{
    if (eventoAtual == 0)
        return;

    experiencia += eventoAtual->experienciaEsquerda;
    confianca += eventoAtual->confiancaEsquerda;
    vies += eventoAtual->viesEsquerda;
    privacidade += eventoAtual->privacidadeEsquerda;
    lucro += eventoAtual->lucroEsquerda;

    limitarAtributos();
    verificarPromocao();
}

void escolherDireita()
{
    if (eventoAtual == 0)
        return;

    experiencia += eventoAtual->experienciaDireita;
    confianca += eventoAtual->confiancaDireita;
    vies += eventoAtual->viesDireita;
    privacidade += eventoAtual->privacidadeDireita;
    lucro += eventoAtual->lucroDireita;

    limitarAtributos();
    verificarPromocao();
}

void atualizarCartas()
{
    if (!cartaAberta)
    {
        if (IsKeyPressed(KEY_E))
        {
            sortearNovaCarta();

            if (eventoAtual != 0)
                cartaAberta = true;
        }

        return;
    }

    if (IsKeyPressed(KEY_A))
    {
        escolherEsquerda();
        cartaAberta = false;
        eventoAtual = 0;
        return;
    }

    if (IsKeyPressed(KEY_D))
    {
        escolherDireita();
        cartaAberta = false;
        eventoAtual = 0;
        return;
    }
}

void desenharTextoQuebrado(const char *texto, int x, int y, int larguraMaxima, int tamanho, Color cor)
{
    char palavra[128] = "";
    char linha[512] = "";
    int indicePalavra = 0;
    int yAtual = y;

    for (int i = 0;; i++)
    {
        char caractere = texto[i];

        if (caractere != ' ' && caractere != '\0')
        {
            if (indicePalavra < 127)
            {
                palavra[indicePalavra] = caractere;
                indicePalavra++;
                palavra[indicePalavra] = '\0';
            }
        }
        else
        {
            if (indicePalavra > 0)
            {
                char teste[512] = "";

                if (strlen(linha) > 0)
                {
                    strcpy(teste, linha);
                    strcat(teste, " ");
                    strcat(teste, palavra);
                }
                else
                {
                    strcpy(teste, palavra);
                }

                if (MeasureText(teste, tamanho) > larguraMaxima)
                {
                    DrawText(linha, x, yAtual, tamanho, cor);
                    yAtual += tamanho + 8;
                    strcpy(linha, palavra);
                }
                else
                {
                    strcpy(linha, teste);
                }

                indicePalavra = 0;
                palavra[0] = '\0';
            }

            if (caractere == '\0')
                break;
        }
    }

    if (strlen(linha) > 0)
        DrawText(linha, x, yAtual, tamanho, cor);
}

void desenharCartas(int larguraResolucao, int alturaResolucao)
{
    if (!cartaAberta || eventoAtual == 0)
        return;

    DrawRectangle(0,0,larguraResolucao,alturaResolucao,Fade(BLACK,0.65f));

    int larguraCarta = 540;
    int alturaCarta = 500;
    int xCarta = (larguraResolucao - larguraCarta) / 2;
    int yCarta = (alturaResolucao - alturaCarta) / 2;

    DrawRectangle(xCarta,yCarta,larguraCarta,alturaCarta,(Color){28,31,45,255});
    DrawRectangleLines(xCarta,yCarta,larguraCarta,alturaCarta,WHITE);

    const char *cargoAtual = nomeCargo();

    DrawText(
        cargoAtual,
        larguraResolucao / 2 - MeasureText(cargoAtual,22) / 2,
        yCarta + 20,
        22,
        SKYBLUE
    );

    DrawText(
        eventoAtual->titulo,
        larguraResolucao / 2 - MeasureText(eventoAtual->titulo,30) / 2,
        yCarta + 60,
        30,
        WHITE
    );

    desenharTextoQuebrado(
        eventoAtual->texto,
        xCarta + 40,
        yCarta + 125,
        larguraCarta - 80,
        22,
        WHITE
    );

    DrawText(
        "A",
        xCarta + 30,
        yCarta + alturaCarta - 105,
        28,
        SKYBLUE
    );

    desenharTextoQuebrado(
        eventoAtual->esquerda,
        xCarta + 30,
        yCarta + alturaCarta - 70,
        larguraCarta / 2 - 50,
        20,
        LIGHTGRAY
    );

    DrawText(
        "D",
        xCarta + larguraCarta / 2 + 20,
        yCarta + alturaCarta - 105,
        28,
        SKYBLUE
    );

    desenharTextoQuebrado(
        eventoAtual->direita,
        xCarta + larguraCarta / 2 + 20,
        yCarta + alturaCarta - 70,
        larguraCarta / 2 - 50,
        20,
        LIGHTGRAY
    );

    // DrawText(TextFormat("Cargo: %s",nomeCargo()),25,25,20,WHITE);
    // DrawText(TextFormat("EXP: %d",experiencia),25,50,20,WHITE);
}

void descarregarCartas()
{
    cartaAberta = false;
    eventoAtual = 0;
    banco = 0;
}