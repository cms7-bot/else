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
    int reputacaoEsquerda;
    int reputacaoDireita;
    int competenciaEsquerda;
    int competenciaDireita;
    int energiaEsquerda;
    int energiaDireita;
    int eticaEsquerda;
    int eticaDireita;
    int dinheiroEsquerda;
    int dinheiroDireita;
} Evento;

Evento *obterBancoPerguntas(int *quantidade);

static Evento *banco = 0;
static Evento *eventoAtual = 0;
static int quantidadeEventos = 0;

static int cargo = 0;
static int experiencia = 0;
static int reputacao = 50;
static int competencia = 50;
static int energia = 100;
static int etica = 50;
static int dinheiro = 0;

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
    if (reputacao < 0) reputacao = 0;
    if (reputacao > 100) reputacao = 100;

    if (competencia < 0) competencia = 0;
    if (competencia > 100) competencia = 100;

    if (energia < 0) energia = 0;
    if (energia > 100) energia = 100;

    if (etica < 0) etica = 0;
    if (etica > 100) etica = 100;
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
    reputacao = 50;
    competencia = 50;
    energia = 100;
    etica = 50;
    dinheiro = 0;
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
    reputacao += eventoAtual->reputacaoEsquerda;
    competencia += eventoAtual->competenciaEsquerda;
    energia += eventoAtual->energiaEsquerda;
    etica += eventoAtual->eticaEsquerda;
    dinheiro += eventoAtual->dinheiroEsquerda;

    limitarAtributos();
    verificarPromocao();
}

void escolherDireita()
{
    if (eventoAtual == 0)
        return;

    experiencia += eventoAtual->experienciaDireita;
    reputacao += eventoAtual->reputacaoDireita;
    competencia += eventoAtual->competenciaDireita;
    energia += eventoAtual->energiaDireita;
    etica += eventoAtual->eticaDireita;
    dinheiro += eventoAtual->dinheiroDireita;

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

    DrawText(
        eventoAtual->esquerda,
        xCarta + 30,
        yCarta + alturaCarta - 70,
        20,
        LIGHTGRAY
    );

    int larguraDireita = MeasureText(eventoAtual->direita,20);

    DrawText(
        "D",
        xCarta + larguraCarta - 50,
        yCarta + alturaCarta - 105,
        28,
        SKYBLUE
    );

    DrawText(
        eventoAtual->direita,
        xCarta + larguraCarta - larguraDireita - 30,
        yCarta + alturaCarta - 70,
        20,
        LIGHTGRAY
    );

    DrawText(TextFormat("Cargo: %s",nomeCargo()),25,25,20,WHITE);
    DrawText(TextFormat("EXP: %d",experiencia),25,50,20,WHITE);
    DrawText(TextFormat("Reputacao: %d",reputacao),25,75,20,WHITE);
    DrawText(TextFormat("Competencia: %d",competencia),25,100,20,WHITE);
    DrawText(TextFormat("Energia: %d",energia),25,125,20,WHITE);
    DrawText(TextFormat("Etica: %d",etica),25,150,20,WHITE);
    DrawText(TextFormat("Dinheiro: R$ %d",dinheiro),25,175,20,WHITE);
}

void descarregarCartas()
{
    cartaAberta = false;
    eventoAtual = 0;
    banco = 0;
}