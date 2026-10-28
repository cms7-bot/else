// ==========================================================
// MODULO DE REGRAS
//
// Aqui ficam as regras do jogo escritas em logica proposicional
// (Relatorio de LMC - AV1 - Squad 8). Cada proposicao do dicionario
// (secao 3) virou uma funcao que devolve true ou false, e a regra de
// game over usa essas funcoes com a mesma formula do relatorio.
//
// As funcoes de medidores e de fim de jogo que estavam no Cartas.c
// foram movidas para ca sem mudar o comportamento.
// Este arquivo NAO desenha nada: so le o estado e responde V ou F.
// ==========================================================
#include <stdbool.h>
#include "Regras.h"

// Barras do HUD: sao definidas no Cartas.c e ligadas pelo iniciarCartas()
extern float *confiancaApagado;
extern float *viesApagado;
extern float *privacidadeApagada;
extern float *lucroApagado;

// Contador de cartas da fase atual (fica no Cartas.c)
int obterCartasResolvidasFase(void);

// ==========================================================
// ESCALA E FAIXAS DOS MEDIDORES
// ==========================================================
// Escala: a barra "apagada" vai de 0 a 0.73 e representa 100 a 0
// pontos. Apagada >= 0.73 = medidor em 0; apagada <= 0 = em 100
// (mesma regra de game over do Movimentacao.c).
#define LIMITE_APAGADO_MAXIMO 0.73
#define FAIXA_SEGURA_MINIMA 30
#define FAIXA_SEGURA_MAXIMA 70

// Cartas resolvidas na fase para valer a proposicao n (relatorio, secao 3.1)
#define CARTAS_PARA_EXPERIENCIA 8

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

// Fim de jogo: aplica a regra g <-> (z v m) (relatorio de LMC, secao 6.1)
bool jogadorPerdeu()
{
    return regraGameOver();
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

// ==========================================================
// PROPOSICOES DE ESTADO (relatorio, secao 3.1)
// Cada funcao devolve o valor de verdade da proposicao agora.
// ==========================================================

// p : Privacidade < 30   (risco de abuso de dados)
bool privacidadeEmRisco(void)
{
    return obterPrivacidade() < FAIXA_SEGURA_MINIMA;
}

// v : Vies > 70   (risco de discriminacao)
bool viesEmRisco(void)
{
    return obterVies() > FAIXA_SEGURA_MAXIMA;
}

// b : os quatro medidores estao entre 30 e 70   (empresa equilibrada)
bool empresaEquilibrada(void)
{
    return todosMedidoresSeguros();
}

// l : Lucro < 30   (caixa baixo)
bool caixaBaixo(void)
{
    return obterLucro() < FAIXA_SEGURA_MINIMA;
}

// n : o jogador resolveu pelo menos 8 cartas na fase atual   (experiencia suficiente)
bool experienciaSuficiente(void)
{
    return obterCartasResolvidasFase() >= CARTAS_PARA_EXPERIENCIA;
}

// a : ha uma auditoria em andamento   (empresa sob auditoria)
// A auditoria ainda nao existe no jogo, entao fica falsa ate a tarefa
// de auditoria chamar definirAuditoriaEmAndamento().
static bool auditoriaAtiva = false;

void definirAuditoriaEmAndamento(bool ativa)
{
    auditoriaAtiva = ativa;
}

bool auditoriaEmAndamento(void)
{
    return auditoriaAtiva;
}

// z : algum medidor chegou a 0   (colapso por falta)
bool algumMedidorZerou(void)
{
    for (int medidor = 1; medidor <= 4; medidor++)
    {
        if (estadoLimiteMedidor(medidor) == -1)
            return true;
    }
    return false;
}

// m : algum medidor chegou a 100   (colapso por excesso)
bool algumMedidorNoMaximo(void)
{
    for (int medidor = 1; medidor <= 4; medidor++)
    {
        if (estadoLimiteMedidor(medidor) == 1)
            return true;
    }
    return false;
}

// ==========================================================
// REGRA DE GAME OVER (relatorio, secao 6.1)
// g <-> (z v m)
// A partida termina se, e somente se, algum medidor chegou a 0 ou a 100.
// ==========================================================
bool regraGameOver(void)
{
    return algumMedidorZerou() || algumMedidorNoMaximo();
}
