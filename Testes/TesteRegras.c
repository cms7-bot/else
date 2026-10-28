// Testes do modulo de regras (Code/Regras.c). Nao precisa de raylib.
//
// Compilar e rodar (na raiz do projeto):
//   gcc Testes/TesteRegras.c Code/Regras.c -o teste_regras.exe
//   teste_regras.exe
#include <stdio.h>
#include <stdbool.h>
#include "../Code/Regras.h"

// No jogo, estas barras e o contador de cartas vivem no Cartas.c.
// Aqui sao simuladas para testar o Regras.c isolado.
float *confiancaApagado, *viesApagado, *privacidadeApagada, *lucroApagado;
static int cartasResolvidas = 0;
int obterCartasResolvidasFase(void) { return cartasResolvidas; }

static float bConf, bVies, bPriv, bLuc;
static int falhas = 0, total = 0;

static void confere(const char *nome, int obtido, int esperado)
{
    total++;
    if (obtido != esperado)
    {
        falhas++;
        printf("FALHOU: %s (obtido %d, esperado %d)\n", nome, obtido, esperado);
    }
}

// Define os medidores na escala 0 a 100 (converte para a barra "apagada" do HUD)
static float apagada(double valor) { return (float)(0.73 * (1.0 - valor / 100.0)); }

static void medidores(double conf, double priv, double luc, double vies)
{
    bConf = apagada(conf);
    bPriv = apagada(priv);
    bLuc  = apagada(luc);
    bVies = apagada(vies);
    confiancaApagado = &bConf;
    privacidadeApagada = &bPriv;
    lucroApagado = &bLuc;
    viesApagado = &bVies;
}

// Condicao de game over que existia antes no Movimentacao.c
static bool condicaoAntiga(float c, float v, float p, float l)
{
    return c <= 0 || c >= 0.73 || v <= 0 || v >= 0.73 ||
           p <= 0 || p >= 0.73 || l <= 0 || l >= 0.73;
}

int main(void)
{
    // ---- p: Privacidade < 30 ----
    medidores(50, 29, 50, 50); confere("p com Privacidade 29", privacidadeEmRisco(), 1);
    medidores(50, 30, 50, 50); confere("p com Privacidade 30", privacidadeEmRisco(), 0);

    // ---- v: Vies > 70 ----
    medidores(50, 50, 50, 71); confere("v com Vies 71", viesEmRisco(), 1);
    medidores(50, 50, 50, 70); confere("v com Vies 70", viesEmRisco(), 0);

    // ---- l: Lucro < 30 ----
    medidores(50, 50, 29, 50); confere("l com Lucro 29", caixaBaixo(), 1);
    medidores(50, 50, 30, 50); confere("l com Lucro 30", caixaBaixo(), 0);

    // ---- b: os quatro entre 30 e 70 ----
    medidores(50, 50, 50, 50); confere("b inicial (50,50,50,50)", empresaEquilibrada(), 1);
    medidores(30, 70, 30, 70); confere("b nos limites (30,70,30,70)", empresaEquilibrada(), 1);
    medidores(29, 50, 50, 50); confere("b com Confianca 29", empresaEquilibrada(), 0);
    medidores(50, 71, 50, 50); confere("b com Privacidade 71", empresaEquilibrada(), 0);
    medidores(50, 50, 29, 50); confere("b com Lucro 29", empresaEquilibrada(), 0);
    medidores(50, 50, 50, 71); confere("b com Vies 71", empresaEquilibrada(), 0);

    // ---- n: pelo menos 8 cartas ----
    cartasResolvidas = 7; confere("n com 7 cartas", experienciaSuficiente(), 0);
    cartasResolvidas = 8; confere("n com 8 cartas", experienciaSuficiente(), 1);

    // ---- a: auditoria ----
    confere("a comeca falsa", auditoriaEmAndamento(), 0);
    definirAuditoriaEmAndamento(true);  confere("a ligada", auditoriaEmAndamento(), 1);
    definirAuditoriaEmAndamento(false); confere("a desligada", auditoriaEmAndamento(), 0);

    // ---- z, m e g: tabela-verdade ----
    for (int zero = 0; zero <= 1; zero++)
    {
        for (int cem = 0; cem <= 1; cem++)
        {
            medidores(zero ? 0 : 50, cem ? 100 : 50, 50, 50);
            confere("z", algumMedidorZerou(), zero);
            confere("m", algumMedidorNoMaximo(), cem);
            confere("g <-> (z v m)", regraGameOver(), zero || cem);
            confere("jogadorPerdeu == g", jogadorPerdeu(), regraGameOver());
        }
    }

    // Cada medidor, em cada extremo, encerra o jogo e gera o final certo
    // (1 Conf 0, 2 Conf 100, 3 Vies 0, 4 Vies 100, 5 Priv 0, 6 Priv 100, 7 Lucro 0, 8 Lucro 100)
    medidores(0, 50, 50, 50);   confere("final Confianca 0",   obterFinalDerrota(), 1);
    medidores(100, 50, 50, 50); confere("final Confianca 100", obterFinalDerrota(), 2);
    medidores(50, 50, 50, 0);   confere("final Vies 0",        obterFinalDerrota(), 3);
    medidores(50, 50, 50, 100); confere("final Vies 100",      obterFinalDerrota(), 4);
    medidores(50, 0, 50, 50);   confere("final Privacidade 0", obterFinalDerrota(), 5);
    medidores(50, 100, 50, 50); confere("final Privacidade 100", obterFinalDerrota(), 6);
    medidores(50, 50, 0, 50);   confere("final Lucro 0",       obterFinalDerrota(), 7);
    medidores(50, 50, 100, 50); confere("final Lucro 100",     obterFinalDerrota(), 8);
    medidores(99, 1, 50, 50);   confere("99 e 1 nao encerram", regraGameOver(), 0);
    medidores(99, 1, 50, 50);   confere("sem final", obterFinalDerrota(), 0);

    // HUD ainda nao ligado: ninguem perdeu, medidores em 50
    confiancaApagado = viesApagado = privacidadeApagada = lucroApagado = 0;
    confere("HUD desligado: sem game over", regraGameOver(), 0);
    confere("HUD desligado: valor 50", obterConfianca(), 50);

    // ---- Equivalencia com a condicao antiga do game over ----
    for (int i = -5; i <= 80; i++)
    {
        for (int j = -5; j <= 80; j += 7)
        {
            bConf = i / 100.0f; bPriv = j / 100.0f; bVies = 0.33f; bLuc = 0.33f;
            confiancaApagado = &bConf; privacidadeApagada = &bPriv;
            viesApagado = &bVies; lucroApagado = &bLuc;
            confere("equivale ao game over antigo",
                    regraGameOver(), condicaoAntiga(bConf, bVies, bPriv, bLuc));
        }
    }
    bConf = bVies = bPriv = bLuc = 0.33f;
    confiancaApagado = &bConf; privacidadeApagada = &bPriv;
    viesApagado = &bVies; lucroApagado = &bLuc;
    confere("inicio da partida: sem game over", regraGameOver(), 0);

    printf("%d testes, %d falhas\n", total, falhas);
    return falhas == 0 ? 0 : 1;
}
