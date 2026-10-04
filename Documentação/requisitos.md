# ELSE · Requisitos do MVP (Unidade 1)

Este documento lista os requisitos do MVP da Unidade 1 do ELSE. Ele segue as definições do Capítulo 3 do livro *Engenharia de Software Moderna* (Marco Tulio Valente):

- **Requisitos funcionais (RF)** definem **o que** o sistema deve fazer. São escritos em linguagem natural.
- **Requisitos não funcionais (RNF)** definem **sob quais restrições** o sistema deve funcionar. Sempre que possível, são escritos com uma **métrica**, para evitar frases genéricas como "o jogo deve ser rápido".

Cada RF aponta para a história de usuário (US) de onde ele vem, para manter a rastreabilidade.

---

## 1. Escopo do MVP da U1

O MVP da U1 é a **fase do Estagiário jogável do início ao fim**, em C com raylib.

| Entra na U1 | Fica para a U2 |
|---|---|
| Menu, sala do Estagiário, movimento e interação (US01) | Consequências de decisões passadas (US04) |
| Cartas de dilema e efeito nos medidores (US02) | Notificações inesperadas (US05) |
| HUD com os 4 medidores e o dia (US03, sem o painel de regras) | Promoção e troca de sala (US07) |
| Fim de jogo e jogar novamente (US08 e US09, versão simples) | Compartilhar resultado (US10) |
| Explicação do conceito de IA no verso da carta (US06), se der tempo | Auditoria (US11), conversa com colegas (US12), máquina de café (US13), galeria de finais (US14), salvar progresso (US15) |
| | Regras em Haskell e painel "Regras da empresa" |

---

## 2. Requisitos funcionais

| ID | Requisito | História | Prioridade |
|:--:|---|:--:|:--:|
| RF01 | O jogo deve exibir um menu principal com as opções Jogar, Instruções e Sair. | US01 | Must |
| RF02 | Ao escolher Jogar, o jogo deve iniciar uma partida na sala do Estagiário, com os 4 medidores em 50 e o contador no Dia 1. | US01 | Must |
| RF03 | O jogador deve mover o personagem com as teclas W, A, S e D dentro da sala, que tem enquadramento fixo (sem rolagem de câmera). | US01 | Must |
| RF04 | O jogador deve abrir um dilema ao interagir com um colega ou computador pela tecla E. Nenhum dilema aparece sozinho. | US01 | Must |
| RF05 | O jogo deve exibir cada dilema dentro de uma carta, com quem está falando, o texto do dilema e duas posturas possíveis. | US02 | Must |
| RF06 | O jogador deve escolher uma postura com a tecla A (esquerda) ou D (direita). | US02 | Must |
| RF07 | Após a escolha, o jogo deve alterar apenas os medidores previstos para aquele lado da carta. Os demais não mudam. | US02 | Must |
| RF08 | Um dilema já resolvido não deve aparecer de novo na mesma partida. | US02 | Must |
| RF09 | O HUD deve mostrar sempre os 4 medidores (Confiança, Viés, Privacidade e Lucro), com o valor real de cada um, e o dia atual. | US03 | Must |
| RF10 | Com uma carta aberta, o HUD não deve indicar qual lado melhora ou piora cada medidor. | US03 | Must |
| RF11 | Quando qualquer medidor chegar a 0 ou a 100, o jogo deve encerrar a partida e mostrar qual medidor causou o fim. | US08 | Must |
| RF12 | Na tela de fim de jogo, o jogador deve escolher entre Jogar novamente ou Sair. O jogo nunca deve fechar sem essa escolha. | US08, US09 | Must |
| RF13 | Ao escolher Jogar novamente, os medidores devem voltar a 50, o dia ao Dia 1 e as cartas devem ser embaralhadas de novo. | US09 | Must |
| RF14 | O jogo deve ignorar teclas que não são opção válida no menu, nas cartas e no fim de jogo, sem travar nem fechar. | Rubrica PIF | Must |
| RF15 | O personagem deve andar também na diagonal, com a mesma velocidade das outras direções. | US01 | Should |
| RF16 | Após a escolha, a carta deve virar e mostrar a consequência e o conceito de IA envolvido. Ao fechar a carta, nada fica na tela. | US06 | Should |
| RF17 | O jogador deve abrir um menu de pausa pela tecla Esc ou pelo botão no canto da tela. | US07 | Could |

---

## 3. Requisitos não funcionais

As métricas abaixo são **metas propostas pela equipe** e devem ser validadas nos testes.

| ID | Categoria | Requisito | Como medir |
|:--:|---|---|---|
| RNF01 | Plataforma | O jogo deve rodar como aplicativo de computador no Windows 10 e 11, sem precisar de internet ou servidor. | Abrir e jogar uma partida com a internet desligada |
| RNF02 | Tecnologia | O jogo deve ser feito em C com a biblioteca raylib, sem engine de jogos. Haskell entra só na U2. | Revisão do código |
| RNF03 | Compilação | O jogo deve compilar por um único script, sem erros e sem avisos. | 0 erros e 0 avisos com `gcc -Wall`, a partir de um clone novo do repositório |
| RNF04 | Desempenho | O jogo deve rodar a 60 quadros por segundo e abrir a tela inicial em até 5 segundos. | Medir num notebook usado em aula |
| RNF05 | Interface | A janela do jogo deve ter resolução de 1280 × 720. | Conferir ao abrir o jogo |
| RNF06 | Robustez | Teclas erradas não devem travar nem fechar o jogo. | 0 travamentos em 1 minuto apertando teclas aleatórias no menu, nas cartas e no fim de jogo |
| RNF07 | Usabilidade | O jogo deve ter pouco texto: o texto de cada carta deve ser curto, e custos e ganhos devem aparecer pelos medidores, não por frases. | Texto de cada carta com até 200 caracteres |
| RNF08 | Usabilidade | Um jogador que nunca viu o jogo deve conseguir resolver a primeira carta sem ajuda. | 3 pessoas de fora da equipe, cada uma em até 1 minuto |
| RNF09 | Acessibilidade | Os 4 medidores devem ser diferenciados por ícone e por cor, nunca só pela cor. | Revisão visual do HUD |
| RNF10 | Consistência | A mesma escolha, no mesmo estado da partida, deve sempre produzir o mesmo efeito nos medidores. | Repetir a mesma carta e escolha em duas partidas e comparar os valores |
| RNF11 | Manutenibilidade | Todas as cartas devem ficar num único arquivo de dados, de forma que adicionar uma carta não exija mudar a lógica do jogo. | Adicionar uma carta de teste alterando só o arquivo das cartas |
| RNF12 | Manutenibilidade | O código deve ter nomes claros, comentários em cada bloco principal e nenhuma lógica repetida sem necessidade. | Revisão pela rubrica da PIF |
| RNF13 | Versionamento | O código deve ficar no GitHub, com commits no padrão Conventional Commits e de todos os integrantes. | Histórico de commits |

---

## 4. Restrições da disciplina (PIF · Unidade 1)

Além dos requisitos acima, a entrega da U1 precisa respeitar a rubrica de Programação Imperativa e Funcional:

- Usar pelo menos **7 operadores distintos** da linguagem C.
- Validar entradas do jogador com laço **`while`**.
- Controlar o menu e o game loop com **`while`** ou **`do-while`**.
- Ter no README as instruções para compilar e executar pelo terminal, e os links para os artefatos das outras disciplinas.
