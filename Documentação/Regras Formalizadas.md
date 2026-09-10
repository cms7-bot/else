# Regras e Invariantes dos Pilares e Medidores — else

---

## 1. Dois conjuntos diferentes, propositalmente

O jogo trabalha com dois conjuntos de 4 elementos que **não coincidem**:

| Conjunto | Elementos | Papel no jogo |
|---|---|---|
| **Pilares temáticos** | Viés, Alucinação, Privacidade, Dependência | Categorizam o **tema** de cada carta/dilema apresentado ao jogador |
| **Medidores** | Confiança, Privacidade, Lucro, Viés | Números de 0 a 100 que o jogador precisa manter equilibrados |

"Viés" e "Privacidade" existem nos dois conjuntos (o dilema de viés tende a mexer no medidor de Viés; o de privacidade, no de Privacidade). Já "Alucinação" e "Dependência" **não têm medidor próprio** — seus efeitos precisam ser distribuídos entre Confiança, Privacidade, Lucro e Viés. A tabela da seção 2 formaliza essa distribuição.

---

## 2. Mapeamento pilar → medidores

Para cada pilar, um medidor **primário** (o que a carta afeta com mais intensidade) e medidores **secundários** (efeito colateral menor, representando o trade-off da decisão).

| Pilar | Medidor primário | Medidores secundários | Lógica do trade-off |
|---|---|---|---|
| **Viés** | Viés | Confiança, Lucro | Corrigir/mitigar o viés → ↑ Confiança, ↓ Lucro (custo de correção). Ignorar → ↑ Lucro, ↑ Viés, ↓ Confiança. |
| **Alucinação** | Confiança | Lucro, Viés | Validar/revisar a saída do modelo antes de publicar → ↓ Lucro (custo/atraso), ↑ Confiança. Lançar sem validar → ↑ Lucro, ↓ Confiança; se o erro reforça um padrão discriminatório, também ↑ Viés. |
| **Privacidade** | Privacidade | Confiança, Lucro | Proteger dados do usuário → ↓ Lucro (menos monetização de dados), ↑ Privacidade, ↑ Confiança. Explorar dados → ↑ Lucro, ↓ Privacidade, ↓ Confiança. |
| **Dependência** | Lucro | Confiança, Privacidade | Incentivar uso contínuo/engajamento do usuário com a IA → ↑ Lucro, ↓ Confiança (preocupação pública com dependência excessiva). Medidas que reduzem a dependência (ex.: limites de uso, transparência) → ↓ Lucro no curto prazo, ↑ Confiança. |

> Nenhum pilar afeta os 4 medidores ao mesmo tempo — isso mantém cada dilema com uma leitura clara ("essa escolha custa Lucro mas ganha Confiança"), em vez de mudanças difusas em tudo.

---

## 3. Magnitude do impacto por decisão

Proposta de faixas (a validar em playtest/balanceamento):

| Tier | Δ no medidor primário | Δ nos medidores secundários | Quando usar |
|---|---|---|---|
| Leve | ±5 | ±2 | Dilemas da fase Estagiário / decisões de baixo risco |
| Moderado | ±10 | ±5 | Dilemas de fases intermediárias (Júnior/Pleno) |
| Severo | ±15 | ±8 | Dilemas de fases avançadas (Sênior/Tech Lead) ou eventos de alto impacto |

A ideia é que a **progressão de carreira** também aumente a magnitude das consequências — decisões de um Tech Lead pesam mais do que as de um Estagiário.

---

## 4. Regras de combinação (penalidades extras)

Além do impacto direto de cada carta, propõe-se que certos **padrões de escolha** gerem penalidade adicional, simulando consequências de negligência sistemática:

- **Sequência de negligência:** se o jogador escolhe 3 decisões seguidas que priorizam Lucro em detrimento do medidor primário do pilar (ex.: 3 dilemas de Privacidade seguidos escolhendo "explorar dados"), aplica-se uma penalidade extra de −5 em Confiança na 3ª decisão, representando um padrão identificado publicamente.
- **Erosão dupla:** se Privacidade e Viés estiverem simultaneamente abaixo de um limiar crítico (ex.: ambos < 30), dispara-se um evento adicional (`RF19`) de auditoria/investigação, com penalidade extra em Confiança até a situação ser resolvida.
- **Zona de alerta:** quando Confiança cai abaixo de um limiar (ex.: 20) sem zerar, o sistema pode disparar um evento de "crise de reputação" como aviso ao jogador antes do Game Over efetivo.

> Os limiares (30, 20, "3 decisões seguidas") são valores de partida sugeridos, não definitivos — dependem de balanceamento e devem ser ajustados em playtest.

---

## 5. Invariantes do sistema

1. **Limites dos medidores:** todo medidor m ∈ {Confiança, Privacidade, Lucro, Viés} deve satisfazer 0 ≤ m ≤ 100 em qualquer momento da partida.
2. **Clamping:** toda atualização de medidor deve ser limitada aos extremos — nenhuma decisão pode levar um medidor para além de 100 ou abaixo de 0; o excesso é descartado, não "transbordado" para outro medidor.
3. **Medidores só mudam na decisão:** durante a exploração livre do ambiente (`RF03`/`RF04`, ver US01), nenhum medidor é alterado — mudanças só ocorrem no processamento de uma decisão (`RF10`/`RF11`).
4. **Condição de Game Over:** a partida é encerrada imediatamente (`RF14`/`RF15`) quando qualquer medidor atinge 0 ou 100, antes de qualquer novo cenário ser apresentado.
5. **Estouro simultâneo:** se mais de um medidor atingir seu limite na mesma decisão, é preciso definir qual causa é exibida na tela de Game Over. Proposta: ordem de prioridade fixa (ex.: Confiança > Privacidade > Lucro > Viés) ou uma mensagem específica de "colapso múltiplo" — **decisão em aberto, a validar com o time**.
6. **Independência entre medidores:** o valor de um medidor nunca é derivado automaticamente de outro (ex.: Lucro não é calculado a partir de Confiança) — cada um só muda por efeito explícito de uma carta ou evento.

---
