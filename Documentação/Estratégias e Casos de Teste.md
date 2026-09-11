# Estratégias e Casos de Teste — else

---

## 1. Objetivo

Definir a estratégia de testes do projeto else, garantindo que as funcionalidades descritas nas Histórias de Usuário e no Documento de Requisitos funcionem corretamente antes da entrega final, com foco em validar o **core loop do jogo**, a **atualização correta dos medidores** e a **camada educativa**, que são o núcleo de valor do produto.

## 2. Escopo dos testes

### Incluso
- Testes funcionais de todas as mecânicas de decisão (arraste, investigar, consultar conselheiro)
- Testes de limites nos 4 medidores (comportamento em 0 e 100)
- Testes de usabilidade do gesto de swipe (mouse e toque)
- Testes de responsividade (desktop e mobile)
- Testes manuais de regressão após cada sprint

### Fora do escopo
- Testes de carga/performance sob alto volume de usuários simultâneos (não se aplica a um jogo sem backend)
- Testes de segurança de infraestrutura (não há armazenamento de dados sensíveis nesta versão)
- Testes automatizados de ponta a ponta (fora do prazo da disciplina; fica como recomendação de trabalho futuro)

---

## 3. Tipos de teste aplicados

| Tipo de teste | Aplicação no projeto | Quando é executado |
|---|---|---|
| **Teste funcional manual** | Verificar se cada decisão atualiza corretamente os medidores, se o cartão certo é sorteado, se o núcleo muda de estado | A cada nova funcionalidade implementada |
| **Teste de limites (valores extremos)** | Verificar comportamento quando um medidor chega a 0 ou 100, e quando está próximo dos limites (ex: 1, 99) | Antes de cada entrega parcial (Review do sprint) |
| **Teste de usabilidade** | Verificar se o gesto de arraste é intuitivo sem instruções, com usuários reais (colegas de turma) | Ao final da implementação do core loop |
| **Teste de regressão** | Repetir testes já validados após alterações no código, garantindo que nada quebrou | A cada Pull Request mesclado na `main` |
| **Teste de compatibilidade/responsividade** | Verificar funcionamento em diferentes tamanhos de tela e navegadores | Antes da entrega final |
| **Teste de aceitação** | Validar se a história de usuário atende aos critérios definidos no padrão 3Cs (Confirmation) | Ao mover um card para "Concluído" no quadro |

---

## 4. Casos de teste

Cada caso de teste é derivado diretamente dos critérios de aceite (Confirmation) já definidos nas Histórias de Usuário.

| ID | História relacionada | Entrada / Pré-condição | Ação | Resultado esperado |
|---|---|---|---|---|
| CT01 | US01 | Jogo é aberto pela primeira vez | Carregar a tela inicial | Os 4 medidores exibem valor 60, o contador mostra "DIA 1" e um cartão é exibido |
| CT02 | US02 | Cartão de dilema visível | Arrastar mais de 90px para a direita e soltar | Opção da direita é aplicada; medidores são atualizados com os valores corretos |
| CT03 | US02 | Cartão de dilema visível | Arrastar mais de 90px para a esquerda e soltar | Opção da esquerda é aplicada; medidores são atualizados com os valores corretos |
| CT04 | US02 | Cartão de dilema visível | Arrastar menos de 90px e soltar | Cartão retorna à posição original; nenhum medidor é alterado |
| CT05 | US02 | Um medidor está em 95 | Escolher uma opção que soma +10 nesse medidor | Valor final trava em 100, não ultrapassa o limite |
| CT06 | US02 | Um medidor está em 5 | Escolher uma opção que subtrai 10 desse medidor | Valor final trava em 0, não fica negativo |
| CT07 | US03 | Cartão de dilema visível | Pressionar e segurar o cartão sem soltar | Indicadores de impacto aparecem nas laterais do cartão |
| CT08 | US04 | Cartão de dilema visível | Arrastar para cima o suficiente | Informação extra é revelada; contador de dias avança 1; nenhum medidor muda |
| CT09 | US05 | Cartão de dilema visível | Dar duplo toque no cartão | Frase de um conselheiro aparece na tela |
| CT10 | US06 | Média dos medidores acima de 45, nenhum abaixo de 20 | Renderizar a tela | Núcleo exibe estado "estável" |
| CT11 | US06 | Algum medidor abaixo de 20 | Renderizar a tela | Núcleo exibe estado "crítico" com animação de instabilidade |
| CT12 | US07 | Uma decisão acabou de ser tomada | Aguardar o sorteio de notificação | Notificação pode ou não aparecer; se aparecer, some sozinha após poucos segundos |
| CT13 | US08 | Uma decisão foi confirmada | Aguardar o processamento do resultado | Toast educativo relacionado ao dilema é exibido e desaparece automaticamente |
| CT14 | US10 | Mini quiz sorteado | Responder corretamente | Medidor de Ética aumenta; explicação é exibida |
| CT15 | US10 | Mini quiz sorteado | Responder incorretamente | Nenhum medidor é penalizado; explicação correta é exibida |
| CT16 | US11 | 10 decisões foram completadas | Verificar tela exibida | Tela de auditoria aparece automaticamente, listando as últimas decisões |
| CT17 | US12 | Um medidor atinge 0 | Verificar tela exibida | Tela de fim de jogo mostra a narrativa de derrota correta para esse medidor no mínimo |
| CT18 | US12 | Um medidor atinge 100 | Verificar tela exibida | Tela de fim de jogo mostra a narrativa de derrota correta para esse medidor no máximo |
| CT19 | US13 | Tela de fim de jogo visível | Tocar em "Reiniciar" | Medidores voltam a 60, contador volta a "DIA 1", novo cartão é sorteado |
| CT20 | US14 | Tela com largura ≤ 480px | Carregar o jogo | Todos os elementos aparecem sem cortes ou sobreposição |
| CT21 | US14 | Tela com largura ≥ 768px | Carregar o jogo | Layout permanece centralizado e legível |
| CT22 | US15 | Tela de fim de jogo visível | Tocar em "Compartilhar resultado" | Texto com dias sobrevividos e motivo da queda é copiado; confirmação visual é exibida |

---

## 5. Matriz de rastreabilidade (Histórias × Casos de teste)

| História de Usuário | Casos de teste cobrindo |
|---|---|
| US01 — Iniciar nova partida | CT01 |
| US02 — Decisão e impacto nos medidores | CT02, CT03, CT04, CT05, CT06 |
| US03 — Prévia de impacto | CT07 |
| US04 — Investigar dilema | CT08 |
| US05 — Consultar conselheiro | CT09 |
| US06 — Núcleo reage ao estado | CT10, CT11 |
| US07 — Notificações inesperadas | CT12 |
| US08 — Toast educativo | CT13 |
| US09 — Slider de parâmetro | *(a definir quando a mecânica for implementada)* |
| US10 — Mini quiz de viés | CT14, CT15 |
| US11 — Tela de auditoria | CT16 |
| US12 — Tela de fim de jogo | CT17, CT18 |
| US13 — Reiniciar partida | CT19 |
| US14 — Responsividade | CT20, CT21 |
| US15 — Compartilhar resultado | CT22 |
| US16 — Feedback tátil (bônus) | *(a definir — depende de dispositivo físico para validar a Vibration API)* |

> Toda história sem caso de teste associado deve ser sinalizada no quadro Kanban como bloqueada para "Concluído" até que o caso de teste correspondente seja criado.

---

## 6. Ambiente e ferramentas de teste

| Categoria | Detalhe |
|---|---|
| Navegadores testados | Chrome, Firefox e Safari (últimas versões estáveis) |
| Dispositivos testados | 1 notebook (desktop) + pelo menos 2 celulares com tamanhos de tela diferentes |
| Tipo de execução | Testes manuais, guiados pela tabela de casos de teste |
| Registro de execução | Planilha ou quadro simples marcando cada caso de teste como Passou / Falhou / Bloqueado, por sprint |
| Ferramenta de bugs | Issues do GitHub, com label `bug` |

---

## 7. Processo de gestão de bugs

1. Ao encontrar um problema, o integrante abre uma **Issue no GitHub** com: título curto, passos para reproduzir, resultado esperado vs. resultado obtido, e print/vídeo se possível
2. A issue recebe uma prioridade: **Crítica** (quebra o core loop), **Alta** (afeta uma funcionalidade importante), **Baixa** (visual/cosmético)
3. Bugs críticos e altos devem ser corrigidos antes da próxima Review; bugs baixos podem ser adiados para o sprint seguinte
4. Após a correção, o mesmo caso de teste que originou o bug é reexecutado (teste de regressão) antes de fechar a issue

---

## 8. Critérios de aceite gerais do MVP

O jogo é considerado pronto para entrega quando:
- Todos os casos de teste da seção 4 estão marcados como "Passou"
- Nenhum bug crítico ou alto está em aberto
- O jogo funciona sem erros visíveis em pelo menos 3 navegadores e 2 tamanhos de tela diferentes
- Uma partida completa (do início a um dos finais) pode ser jogada do começo ao fim sem travamentos
- Todos os textos educativos (toasts, auditoria, telas de fim de jogo) estão revisados e sem erros de português
