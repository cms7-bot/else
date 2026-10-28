#ifndef REGRAS_H
#define REGRAS_H

#include <stdbool.h>

// ==========================================================
// MODULO DE REGRAS (Regras.c)
// Logica proposicional do ELSE: Relatorio de LMC (AV1, Squad 8).
// ==========================================================

// ---------- Medidores (escala 0 a 100, lidos das barras do HUD) ----------
int  obterValorMedidor(int medidor);   // 1 Confianca, 2 Vies, 3 Privacidade, 4 Lucro
bool medidorEstaSeguro(int medidor);   // entre 30 e 70
bool todosMedidoresSeguros(void);
int  obterConfianca(void);
int  obterVies(void);
int  obterPrivacidade(void);
int  obterLucro(void);

// ---------- Fim de jogo ----------
int  obterMedidorDerrota(void);        // 0 = ninguem perdeu; 1 a 4 = medidor que causou
bool jogadorPerdeu(void);              // aplica a regra g
bool derrotaNoLimiteMaximo(void);
int  obterFinalDerrota(void);          // 0 = sem derrota; 1 a 8 = qual final

// ---------- Proposicoes de estado (relatorio, secao 3.1) ----------
bool privacidadeEmRisco(void);         // p: Privacidade < 30
bool viesEmRisco(void);                // v: Vies > 70
bool empresaEquilibrada(void);         // b: os quatro medidores entre 30 e 70
bool caixaBaixo(void);                 // l: Lucro < 30
bool experienciaSuficiente(void);      // n: pelo menos 8 cartas resolvidas na fase
bool auditoriaEmAndamento(void);       // a: ha auditoria em andamento
bool algumMedidorZerou(void);          // z: algum medidor chegou a 0
bool algumMedidorNoMaximo(void);       // m: algum medidor chegou a 100

// Liga/desliga o estado da auditoria (usado pela futura regra de auditoria)
void definirAuditoriaEmAndamento(bool ativa);

// ---------- Regras compostas ----------
bool regraGameOver(void);              // g <-> (z v m)  (relatorio, secao 6.1)

#endif
