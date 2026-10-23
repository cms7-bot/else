#include "raylib.h"
#include <stdbool.h>
#include <string.h>


// ==========================================================
// INDICADORES DO HUD (ponteiros para as barras)
// ==========================================================
float *confiancaApagado;
float *viesApagado;
float *privacidadeApagada;
float *lucroApagado;


// ==========================================================
// TIPOS
// ==========================================================
typedef struct Evento
{
    int id;
    int cargo;
    const char *titulo;
    const char *texto;
    const char *esquerda;
    const char *direita;
    float confiancaEsquerda;
    float confiancaDireita;
    float viesEsquerda;
    float viesDireita;
    float privacidadeEsquerda;
    float privacidadeDireita;
    float lucroEsquerda;
    float lucroDireita;

} Evento;

Evento *obterBancoPerguntas(int *quantidade);


// ==========================================================
// BANCO DE EVENTOS / CARTA ATUAL
// ==========================================================
static Evento *banco = 0;
static Evento *eventoAtual = 0;
static int quantidadeEventos = 0;


// ==========================================================
// CARGO / EXPERIÊNCIA
// ==========================================================
static int cargo = 0;
static int experiencia = 0;


// ==========================================================
// ESTADO DA CARTA
// ==========================================================
static bool cartaAberta = false;


// ==========================================================
// HISTÓRICO DE EVENTOS RECENTES
// ==========================================================
static int historico[8] = {-1,-1,-1,-1,-1,-1,-1,-1};


// ==========================================================
// CARGO
// ==========================================================
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


// ==========================================================
// ESTADO DA CARTA
// ==========================================================
bool cartasEstaoAbertas()
{
    return cartaAberta;
}

// ==========================================================
// PARTIDA: DIAS, DERROTA E FINAIS
//
// Tudo aqui LE os medidores direto das barras do HUD. Assim
// continua certo quando o Movimentacao.c zera as barras no
// "jogar de novo", sem precisar chamar nada de novo.
// Escala: a barra "apagada" vai de 0 a 0.73 e representa 100 a 0
// pontos. Apagada >= 0.73 = medidor em 0; apagada <= 0 = em 100
// (mesma regra de game over do Movimentacao.c).
// ==========================================================
#define LIMITE_APAGADO_MAXIMO 0.73
#define FAIXA_SEGURA_MINIMA 30
#define FAIXA_SEGURA_MAXIMA 70

static int diasSobrevividos = 0;
static int cartasResolvidasFase = 0;
static bool partidaTerminou = false;

// Medidores: 1 Confianca, 2 Vies, 3 Privacidade, 4 Lucro
static float *ponteiroMedidor(int medidor)
{
    if (medidor == 1) return confiancaApagado;
    if (medidor == 2) return viesApagado;
    if (medidor == 3) return privacidadeApagada;
    if (medidor == 4) return lucroApagado;
    return 0;
}

// -1 = medidor em 0, +1 = medidor em 100, 0 = ainda em jogo
static int estadoLimiteMedidor(int medidor)
{
    float *apagado = ponteiroMedidor(medidor);

    if (apagado == 0) return 0;
    if (*apagado >= LIMITE_APAGADO_MAXIMO) return -1;
    if (*apagado <= 0) return 1;
    return 0;
}

// Valor do medidor de 0 a 100 (50 se o HUD ainda nao foi ligado).
int obterValorMedidor(int medidor)
{
    float *apagado = ponteiroMedidor(medidor);

    if (apagado == 0) return 50;

    int valor = (int)(100.0f * (1.0f - *apagado / (float)LIMITE_APAGADO_MAXIMO) + 0.5f);

    if (valor < 0) valor = 0;
    if (valor > 100) valor = 100;
    return valor;
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

int obterConfianca() { return obterValorMedidor(1); }
int obterVies() { return obterValorMedidor(2); }
int obterPrivacidade() { return obterValorMedidor(3); }
int obterLucro() { return obterValorMedidor(4); }

// 0 = ninguem perdeu; 1 a 4 = medidor que causou a derrota
int obterMedidorDerrota()
{
    for (int medidor = 1; medidor <= 4; medidor++)
    {
        if (estadoLimiteMedidor(medidor) != 0)
            return medidor;
    }
    return 0;
}

bool jogadorPerdeu()
{
    return obterMedidorDerrota() != 0;
}

bool derrotaNoLimiteMaximo()
{
    int medidor = obterMedidorDerrota();
    return medidor != 0 && estadoLimiteMedidor(medidor) == 1;
}

// 1 Confianca 0, 2 Confianca 100, 3 Vies 0, 4 Vies 100,
// 5 Privacidade 0, 6 Privacidade 100, 7 Lucro 0, 8 Lucro 100
int obterFinalDerrota()
{
    int medidor = obterMedidorDerrota();

    if (medidor == 0)
        return 0;

    return (medidor - 1) * 2 + (derrotaNoLimiteMaximo() ? 2 : 1);
}

// Mensagem de cada final Definitiva 
const char *obterMensagemFinal()
{
    switch (obterFinalDerrota())
    {
         // CONFIANCA chegou a 0 -> final "Sem Credito"
        case 1: return  "Foram tantas respostas erradas que os clientes voltaram a fazer tudo na mao. E juraram nunca mais confiar numa IA.";
         // CONFIANCA chegou a 100 -> final "Fe Cega"
        case 2: return "Todo mundo confiava tanto na IA que ninguem conferiu o relatorio. Era tudo inventado.";
         // VIES chegou a 0 -> final "Regua"
        case 3: return  "A IA tratou todo mundo exatamente igual. Igualdade perfeita no papel; na vida real, ninguem foi atendido direito.";
        // VIES chegou a 100 -> final "Tendencioso"
        case 4: return  "Ninguem programou a IA pra discriminar. Ela aprendeu sozinha, com os nossos dados. Os processos chegaram.";
        // PRIVACIDADE chegou a 0 -> final "Peneira"
        case 5: return  "A IA conhecia muito bem os clientes. A internet inteira tambem, depois do vazamento.";
        // PRIVACIDADE chegou a 100 -> final "Cadeado"
        case 6: return "Os dados estavam perfeitamente protegidos. Tao protegidos que nem a IA conseguia usar.";
         // LUCRO chegou a 0 -> final "Mao-Aberta" ATENCAO: a frase termina cortada ("fra") DE PROPOSITO.
        case 7: return "Sem dinheiro pra pagar o servidor, a IA foi desligada no meio de uma fra";
        // LUCRO chegou a 100 -> final "Cifrao"
        case 8: return "Os acionistas comemoraram ate a investigacao comecar.";
    }
    return "";
}

// Contador de dias: cada carta resolvida sem derrota = 1 dia sobrevivido.
int obterDiasSobrevividos() { return diasSobrevividos; }
int obterDiaAtual() { return diasSobrevividos + 1; }
int obterCartasResolvidasFase() { return cartasResolvidasFase; }

static void registrarCartaResolvida()
{
    cartasResolvidasFase++;

    if (jogadorPerdeu())
        partidaTerminou = true;
    else
        diasSobrevividos++;
}

// Quando o jogador volta a jogar (o Movimentacao zera as barras),
// o primeiro sorteio de carta zera dias e cartas resolvidas.
static void reiniciarPartidaSeNecessario()
{
    if (partidaTerminou && !jogadorPerdeu())
    {
        diasSobrevividos = 0;
        cartasResolvidasFase = 0;
        partidaTerminou = false;

        // Baralho embaralhado de novo (US09): esquece as cartas ja vistas.
        for (int i = 0; i < 8; i++)
            historico[i] = -1;
    }
}



// ==========================================================
// HISTÓRICO
// ==========================================================
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


// ==========================================================
// ATRIBUTOS / PROMOÇÃO ====> Será implementado futuramente
// ==========================================================
// void limitarAtributos()
// {
//     if (reputacao < 0) reputacao = 0;
//     if (reputacao > 100) reputacao = 100;

//     if (competencia < 0) competencia = 0;
//     if (competencia > 100) competencia = 100;

//     if (energia < 0) energia = 0;
//     if (energia > 100) energia = 100;

//     if (etica < 0) etica = 0;
//     if (etica > 100) etica = 100;
// }

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


// ==========================================================
// SORTEIO DE CARTAS
// ==========================================================
void sortearNovaCarta()
{
    reiniciarPartidaSeNecessario();

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


// ==========================================================
// INICIALIZAÇÃO
// ==========================================================
void iniciarCartas( float *confianca, float *vies, float *privacidade, float *lucro)
{
    banco = obterBancoPerguntas(&quantidadeEventos);

    confiancaApagado = confianca;
        if (*confiancaApagado<=0.0f){
            *confiancaApagado=0.0f;
        }
        if (*confiancaApagado>=0.73f){
            *confiancaApagado=0.73f;
        }
    viesApagado = vies;
        if (*viesApagado<=0.0f){
            *viesApagado=0.0f;
        }
        if (*viesApagado>=0.73){
            *viesApagado=0.73;
        }

    privacidadeApagada = privacidade;
        if (*privacidadeApagada<=0.0f){
            *privacidadeApagada=0.0f;
        }
        if (*privacidadeApagada>=0.73f){
            *privacidadeApagada=0.73f;
        }
    lucroApagado = lucro;
        if (*lucroApagado<=0.0f){
            *lucroApagado=0.0f;
        }
        if (*lucroApagado>=0.73f){
            *lucroApagado=0.73f;
        }




    eventoAtual = 0;
    cartaAberta = false;
    diasSobrevividos = 0;
    cartasResolvidasFase = 0;
    partidaTerminou = false;

    for (int i = 0; i < 8; i++)
        historico[i] = -1;
}


// ==========================================================
// ESCOLHAS (ESQUERDA / DIREITA)
// ==========================================================
void escolherEsquerda()
{
    if (eventoAtual == 0)
        return;

    *confiancaApagado += eventoAtual->confiancaEsquerda;
        if (*confiancaApagado<=0.0f){
            *confiancaApagado=0.0f;
        }
        if (*confiancaApagado>=0.73f){
            *confiancaApagado=0.73f;
        }

    *viesApagado += eventoAtual->viesEsquerda;
        if (*viesApagado<=0.0f){
            *viesApagado=0.0f;
        }
        if (*viesApagado>=0.73f){
            *viesApagado=0.73f;
        }

    *privacidadeApagada += eventoAtual->privacidadeEsquerda;
        if (*privacidadeApagada<=0.0f){
            *privacidadeApagada=0.0f;
        }
        if (*privacidadeApagada>=0.73f){
            *privacidadeApagada=0.73f;
        }
    *lucroApagado += eventoAtual->lucroEsquerda;
        if (*lucroApagado<=0.0f){
            *lucroApagado=0.0f;
        }
        if (*lucroApagado>=0.73f){
            *lucroApagado=0.73f;
        }

    // limitarAtributos();
    verificarPromocao();
    registrarCartaResolvida();
}

void escolherDireita()
{
    if (eventoAtual == 0)
        return;

    *confiancaApagado += eventoAtual->confiancaDireita;
    *viesApagado += eventoAtual->viesDireita;
    *privacidadeApagada += eventoAtual->privacidadeDireita;
    *lucroApagado += eventoAtual->lucroDireita;


    // limitarAtributos();
    verificarPromocao();
    registrarCartaResolvida();
}


// ==========================================================
// ATUALIZAÇÃO (ENTRADA DO JOGADOR)
// ==========================================================
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


// ==========================================================
// DESENHO
// ==========================================================
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

    // ------------------------------------------------------
    // Fundo escurecido e moldura da carta
    // ------------------------------------------------------
    DrawRectangle(0,0,larguraResolucao,alturaResolucao,Fade(BLACK,0.65f));

    int larguraCarta = 540;
    int alturaCarta = 500;
    int xCarta = (larguraResolucao - larguraCarta) / 2;
    int yCarta = (alturaResolucao - alturaCarta) / 2;

    DrawRectangle(xCarta,yCarta,larguraCarta,alturaCarta,(Color){28,31,45,255});
    DrawRectangleLines(xCarta,yCarta,larguraCarta,alturaCarta,WHITE);

    // ------------------------------------------------------
    // Cargo, título e texto
    // ------------------------------------------------------
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

    // ------------------------------------------------------
    // Opções (esquerda / direita)
    // ------------------------------------------------------
    DrawText(
        "A",
        xCarta + 30,
        yCarta + alturaCarta - 105,
        28,
        SKYBLUE
    );

    DrawText(
        eventoAtual->esquerda,
        xCarta -350,
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
        xCarta + larguraCarta - larguraDireita +200,
        yCarta + alturaCarta - 70,
        20,
        LIGHTGRAY
    );

}


// ==========================================================
// FINALIZAÇÃO
// ==========================================================
void descarregarCartas()
{
    cartaAberta = false;
    eventoAtual = 0;
    banco = 0;
}
