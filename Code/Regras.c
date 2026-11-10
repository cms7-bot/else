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

// Auditoria: quanto cada carta tira de Confianca enquanto ela durar (em pontos, 0 a 100)
#define PENALIDADE_AUDITORIA 5

// 1 ponto do medidor = 0.0073 na barra apagada (0.73 / 100)
#define PONTO_NA_BARRA 0.0073f


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
// AUDITORIA (relatorio de LMC, secoes 5.3 e 6.2)
//
//   r <-> (p ^ v ^ ~a)   a auditoria dispara quando Privacidade < 30
//                        E Vies > 70 e nao ha outra em andamento
//   a -> C-              enquanto durar, cada carta tira Confianca
//   ~(p v v)             sai quando os DOIS voltam a faixa segura
//                        (De Morgan: ~(p v v) == ~p ^ ~v)
// ==========================================================
static bool auditoriaAtiva = false;

// Aviso para a tela mostrar o memorando:
// 0 = nada, 1 = a auditoria comecou, 2 = a auditoria terminou
static int eventoAuditoria = 0;


void definirAuditoriaEmAndamento(bool ativa)
{
    auditoriaAtiva = ativa;
}


bool auditoriaEmAndamento()
{
    return auditoriaAtiva;
}


// Nova partida: sem auditoria e sem memorando pendente
void reiniciarAuditoria()
{
    auditoriaAtiva = false;
    eventoAuditoria = 0;
}


// r <-> (p ^ v ^ ~a)
bool auditoriaDisparada()
{
    return
        privacidadeEmRisco() &&
        viesEmRisco() &&
        !auditoriaEmAndamento();
}


// condicao de saida: ~(p v v)
bool auditoriaPodeEncerrar()
{
    return
        !(
            privacidadeEmRisco() ||
            viesEmRisco()
        );
}


// a -> C-  (somada ao efeito da carta, nunca passa de 0 pontos)
void aplicarPenalidadeAuditoria()
{
    if (!auditoriaEmAndamento() || confiancaApagado == 0)
        return;

    *confiancaApagado +=
        PENALIDADE_AUDITORIA * PONTO_NA_BARRA;

    if (*confiancaApagado >= (float)LIMITE_APAGADO_MAXIMO)
        *confiancaApagado = (float)LIMITE_APAGADO_MAXIMO;
}


// Chamada depois de cada carta (ordem do relatorio, secao 6.2):
// 1) se o jogo acabou (g), a auditoria nao muda mais nada;
// 2) avalia a saida ~(p v v); 3) avalia o disparo r.
void avaliarAuditoria()
{
    if (regraGameOver())
        return;

    if (auditoriaEmAndamento() && auditoriaPodeEncerrar())
    {
        auditoriaAtiva = false;
        eventoAuditoria = 2;
    }

    if (auditoriaDisparada())
    {
        auditoriaAtiva = true;
        eventoAuditoria = 1;
    }
}


// Devolve o aviso pendente (1 = comecou, 2 = terminou) e limpa
int consumirEventoAuditoria()
{
    int evento = eventoAuditoria;

    eventoAuditoria = 0;

    return evento;
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