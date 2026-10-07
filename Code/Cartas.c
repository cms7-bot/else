#include "raylib.h"
#include <stdbool.h>
#include <string.h>

#define VALOR_INICIAL_MEDIDORES 50
#define FAIXA_SEGURA_MINIMA 30
#define FAIXA_SEGURA_MAXIMA 70

typedef struct Evento
{
    int id;
    int cargo;
    const char *titulo;
    const char *texto;
    const char *esquerda;
    const char *direita;
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
static int diasSobrevividos = 0;
static int cartasResolvidasFase = 0;

static int confianca = VALOR_INICIAL_MEDIDORES;
static int vies = VALOR_INICIAL_MEDIDORES;
static int privacidade = VALOR_INICIAL_MEDIDORES;
static int lucro = VALOR_INICIAL_MEDIDORES;

static bool cartaAberta = false;

static int medidorDerrota = 0;
static bool derrotaNoMaximo = false;

static int historico[8] = {-1,-1,-1,-1,-1,-1,-1,-1};

const char *nomeCargo()
{
    if (cargo == 0) return "Estagiario";
    if (cargo == 1) return "Junior";
    if (cargo == 2) return "Pleno";
    if (cargo == 3) return "Senior";
    return "Tech Lider";
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

int obterDiasSobrevividos()
{
    return diasSobrevividos;
}

int obterDiaAtual()
{
    return diasSobrevividos + 1;
}

int obterCartasResolvidasFase()
{
    return cartasResolvidasFase;
}

bool jogadorPerdeu()
{
    return medidorDerrota != 0;
}

int obterMedidorDerrota()
{
    return medidorDerrota;
}

bool derrotaNoLimiteMaximo()
{
    return derrotaNoMaximo;
}

// 1 Confianca 0, 2 Confianca 100, 3 Vies 0, 4 Vies 100, 5 Privacidade 0, 6 Privacidade 100, 7 Lucro 0, 8 Lucro 100
int obterFinalDerrota()
{
    if (medidorDerrota == 0)
        return 0;

    return (medidorDerrota - 1) * 2 + (derrotaNoMaximo ? 2 : 1);
}

// Mensagem de cada final provisoria

const char *obterMensagemFinal()
{
    switch (obterFinalDerrota())
    {
        case 1: return "Ninguem mais confia na empresa. Clientes e equipe foram embora.";
        case 2: return "Confianca cega: ninguem questiona nada, e o primeiro erro grave passou despercebido.";
        case 3: return "Corrigir tudo virou paralisia: a empresa parou de decidir e nada sai do lugar.";
        case 4: return "O vies tomou conta: a IA decide de forma injusta e a empresa virou alvo de processos.";
        case 5: return "Os dados dos usuarios vazaram. A empresa perdeu a licenca para operar.";
        case 6: return "Privacidade total: nenhum dado pode ser usado e a empresa nao consegue mais oferecer o servico.";
        case 7: return "A empresa faliu. Sem dinheiro, nao ha como manter equipe nem sistemas.";
        case 8: return "Lucro acima de tudo: a empresa virou alvo de investigacao por crescer a qualquer custo.";
    }
    return "";
}

int obterValorMedidor(int medidor)
{
    if (medidor == 1) return confianca;
    if (medidor == 2) return vies;
    if (medidor == 3) return privacidade;
    if (medidor == 4) return lucro;
    return 0;
}

bool medidorEstaSeguro(int medidor)
{
    int valor = obterValorMedidor(medidor);
    return valor >= FAIXA_SEGURA_MINIMA && valor <= FAIXA_SEGURA_MAXIMA;
}

bool todosMedidoresSeguros()
{
    for (int medidor = 1; medidor <= 4; medidor++)
    {
        if (!medidorEstaSeguro(medidor))
            return false;
    }
    return true;
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


void verificarDerrota()
{
    if (medidorDerrota != 0)
        return;

    if (confianca <= 0 || confianca >= 100) medidorDerrota = 1;
    else if (vies <= 0 || vies >= 100) medidorDerrota = 2;
    else if (privacidade <= 0 || privacidade >= 100) medidorDerrota = 3;
    else if (lucro <= 0 || lucro >= 100) medidorDerrota = 4;

 
    if (medidorDerrota != 0)
        derrotaNoMaximo = obterValorMedidor(medidorDerrota) >= 100;
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
    diasSobrevividos = 0;
    cartasResolvidasFase = 0;
    confianca = VALOR_INICIAL_MEDIDORES;
    vies = VALOR_INICIAL_MEDIDORES;
    privacidade = VALOR_INICIAL_MEDIDORES;
    lucro = VALOR_INICIAL_MEDIDORES;
    eventoAtual = 0;
    cartaAberta = false;
    medidorDerrota = 0;
    derrotaNoMaximo = false;

    for (int i = 0; i < 8; i++)
        historico[i] = -1;
}

void escolherEsquerda()
{
    if (eventoAtual == 0)
        return;

    confianca += eventoAtual->confiancaEsquerda;
    vies += eventoAtual->viesEsquerda;
    privacidade += eventoAtual->privacidadeEsquerda;
    lucro += eventoAtual->lucroEsquerda;

    limitarAtributos();
    verificarDerrota();

    cartasResolvidasFase++;

    if (!jogadorPerdeu())
        diasSobrevividos++;
}

void escolherDireita()
{
    if (eventoAtual == 0)
        return;

    confianca += eventoAtual->confiancaDireita;
    vies += eventoAtual->viesDireita;
    privacidade += eventoAtual->privacidadeDireita;
    lucro += eventoAtual->lucroDireita;

    limitarAtributos();
    verificarDerrota();

    cartasResolvidasFase++;

    if (!jogadorPerdeu())
        diasSobrevividos++;
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
}

void descarregarCartas()
{
    cartaAberta = false;
    eventoAtual = 0;
    banco = 0;
}