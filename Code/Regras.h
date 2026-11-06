#ifndef REGRAS_H
#define REGRAS_H

#include <stdbool.h>


// ==========================================================
// MEDIDORES
// ==========================================================
int obterValorMedidor(int medidor);

bool medidorEstaSeguro(int medidor);

bool todosMedidoresSeguros(void);

int obterConfianca(void);

int obterVies(void);

int obterPrivacidade(void);

int obterLucro(void);


// ==========================================================
// FIM DE JOGO
// ==========================================================
int obterMedidorDerrota(void);

bool jogadorPerdeu(void);

bool derrotaNoLimiteMaximo(void);

int obterFinalDerrota(void);


// ==========================================================
// PROPOSIÇÕES
// ==========================================================
bool privacidadeEmRisco(void);

bool viesEmRisco(void);

bool empresaEquilibrada(void);

bool caixaBaixo(void);

bool experienciaSuficiente(void);

bool auditoriaEmAndamento(void);

bool algumMedidorZerou(void);

bool algumMedidorNoMaximo(void);


// ==========================================================
// AUDITORIA
// ==========================================================
void definirAuditoriaEmAndamento(bool ativa);


// ==========================================================
// GAME OVER
// ==========================================================
bool regraGameOver(void);


#endif