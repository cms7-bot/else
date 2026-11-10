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

bool auditoriaDisparada(void);           // r <-> (p ^ v ^ ~a)

bool auditoriaPodeEncerrar(void);        // ~(p v v)

void aplicarPenalidadeAuditoria(void);   // a -> C- (Confianca cai a cada carta)

void avaliarAuditoria(void);             // chamar depois de cada carta

int consumirEventoAuditoria(void);       // 0 nada, 1 comecou, 2 terminou

void reiniciarAuditoria(void);           // nova partida


// ==========================================================
// GAME OVER
// ==========================================================
bool regraGameOver(void);


#endif