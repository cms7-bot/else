#include <stdbool.h>
#include "Regras.h"


extern float *confiancaApagado;
extern float *viesApagado;
extern float *privacidadeApagada;
extern float *lucroApagado;


int obterCartasResolvidasFase(void);


#define LIMITE_APAGADO_MAXIMO 0.73
#define FAIXA_SEGURA_MINIMA 30
#define FAIXA_SEGURA_MAXIMA 70
#define CARTAS_PARA_EXPERIENCIA 8


// ==========================================================
// PONTEIRO DO MEDIDOR
// ==========================================================
static float *ponteiroMedidor(int medidor)
{
    if (medidor == 1)
        return confiancaApagado;

    if (medidor == 2)
        return viesApagado;

    if (medidor == 3)
        return privacidadeApagada;

    if (medidor == 4)
        return lucroApagado;

    return 0;
}


// ==========================================================
// ESTADO DO MEDIDOR
// ==========================================================
static int estadoLimiteMedidor(int medidor)
{
    // decide pelo MESMO numero que aparece na barra (0..100),
    // assim "mostrou 100" = "perdeu" (antes a barra chegava em 100 e nada acontecia)
    if (ponteiroMedidor(medidor) == 0)
        return 0;

    int valor =
        obterValorMedidor(medidor);

    if (valor <= 0)
        return -1;

    if (valor >= 100)
        return 1;

    return 0;
}


// ==========================================================
// VALOR DO MEDIDOR
// ==========================================================
int obterValorMedidor(int medidor)
{
    float *apagado =
        ponteiroMedidor(medidor);

    if (apagado == 0)
        return 50;

    int valor =
        (int)(
            100.0f *
            (
                1.0f -
                *apagado /
                (float)LIMITE_APAGADO_MAXIMO
            ) +
            0.5f
        );

    if (valor < 0)
        valor = 0;

    if (valor > 100)
        valor = 100;

    return valor;
}


// ==========================================================
// FAIXA SEGURA
// ==========================================================
bool medidorEstaSeguro(int medidor)
{
    int valor =
        obterValorMedidor(medidor);

    return
        valor >= FAIXA_SEGURA_MINIMA &&
        valor <= FAIXA_SEGURA_MAXIMA;
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


// ==========================================================
// VALORES INDIVIDUAIS
// ==========================================================
int obterConfianca()
{
    return obterValorMedidor(1);
}

int obterVies()
{
    return obterValorMedidor(2);
}

int obterPrivacidade()
{
    return obterValorMedidor(3);
}

int obterLucro()
{
    return obterValorMedidor(4);
}


// ==========================================================
// DERROTA
// ==========================================================
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
    return regraGameOver();
}


bool derrotaNoLimiteMaximo()
{
    int medidor =
        obterMedidorDerrota();

    return
        medidor != 0 &&
        estadoLimiteMedidor(medidor) == 1;
}


// ==========================================================
// FINAL DA DERROTA
// ==========================================================
int obterFinalDerrota()
{
    int medidor =
        obterMedidorDerrota();

    if (medidor == 0)
        return 0;

    return
        (medidor - 1) * 2 +
        (derrotaNoLimiteMaximo() ? 2 : 1);
}


// ==========================================================
// PROPOSIÇÕES
// ==========================================================
bool privacidadeEmRisco()
{
    return
        obterPrivacidade() <
        FAIXA_SEGURA_MINIMA;
}


bool viesEmRisco()
{
    return
        obterVies() >
        FAIXA_SEGURA_MAXIMA;
}


bool empresaEquilibrada()
{
    return todosMedidoresSeguros();
}


bool caixaBaixo()
{
    return
        obterLucro() <
        FAIXA_SEGURA_MINIMA;
}


bool experienciaSuficiente()
{
    return
        obterCartasResolvidasFase() >=
        CARTAS_PARA_EXPERIENCIA;
}


// ==========================================================
// AUDITORIA
// ==========================================================
static bool auditoriaAtiva = false;


void definirAuditoriaEmAndamento(bool ativa)
{
    auditoriaAtiva = ativa;
}


bool auditoriaEmAndamento()
{
    return auditoriaAtiva;
}


// ==========================================================
// MEDIDORES EM 0
// ==========================================================
bool algumMedidorZerou()
{
    for (int medidor = 1; medidor <= 4; medidor++)
    {
        if (
            estadoLimiteMedidor(medidor) == -1
        )
            return true;
    }

    return false;
}


// ==========================================================
// MEDIDORES EM 100
// ==========================================================
bool algumMedidorNoMaximo()
{
    for (int medidor = 1; medidor <= 4; medidor++)
    {
        if (
            estadoLimiteMedidor(medidor) == 1
        )
            return true;
    }

    return false;
}


// ==========================================================
// GAME OVER
// ==========================================================
bool regraGameOver()
{
    return
        algumMedidorZerou() ||
        algumMedidorNoMaximo();
}