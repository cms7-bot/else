# 🎮 else

> Um jogo de exploração e decisões inspirado em *Reigns*, onde você explora ambientes, interage com NPCs e sistemas, e governa os rumos de uma empresa de Inteligência Artificial enquanto aprende conceitos reais de letramento em IA na prática.

![status](https://img.shields.io/badge/status-em%20desenvolvimento-yellow)

---

## 📖 Sobre o projeto

**else** é um jogo em que o jogador assume o papel de responsável por uma empresa de Inteligência Artificial. O jogador explora ambientes em perspectiva top-down (movimentação via **WASD**) e interage com NPCs e computadores para acionar cenários de decisão. Cada cenário apresenta um dilema real do campo de IA, resolvido por meio de uma carta que pode ser arrastada (*swipe*) para a esquerda ou direita. A cada dilema apresentado, é preciso equilibrar quatro medidores — **Confiança**, **Privacidade**, **Lucro** e **Viés** — enquanto aprende, na prática, sobre temas reais como viés algorítmico, alucinação de modelos, deepfakes e privacidade de dados. Conforme progride, o jogador avança por diferentes fases de carreira, de **Estagiário** a **Tech Lead**.


### 🎯 Objetivo do jogo

Sobreviver o maior número de dias possível, mantendo os quatro medidores equilibrados. O jogo termina quando qualquer medidor atinge o valor mínimo (0) ou máximo (100), revelando um final com uma lição educativa sobre o conceito de IA relacionado à causa da "queda".

---

## ✨ Funcionalidades

- 🧭 Exploração de ambientes top-down, com movimentação do personagem via WASD
- 🖥️ Interação com NPCs e computadores para acionar cenários de decisão
- 🃏 Sistema de decisões por arraste (esquerda/direita), com suporte a mouse e toque
- 📊 Quatro medidores dinâmicos (Confiança, Privacidade, Lucro, Viés) que reagem a cada escolha
- 🪜 Progressão de carreira em fases (Estagiário → Programador Júnior → Pleno → Sênior → Tech Lead)
- 🔮 Núcleo visual da IA que muda de aparência conforme o estado geral do sistema
- 🔍 Opção de "investigar" um dilema antes de decidir
- 🗣️ Conselheiros que oferecem dicas rápidas sobre o dilema atual
- 🔔 Notificações inesperadas simulando eventos do mundo real
- 💡 Toasts educativos explicando conceitos reais de IA após cada decisão
- 🎚️ Mini-interações (sliders e quizzes) para reforçar o aprendizado
- 🧾 Tela de auditoria periódica revisando decisões anteriores
- 🏁 Múltiplos finais, cada um com uma lição diferente sobre riscos de IA
- 📱 Interface responsiva (desktop e mobile)

---

## 🛠️ Tecnologias utilizadas

| Camada | Tecnologia |
|---|---|
| Interface |  |
| Tipografia | |
| Dados do jogo | |
| Versionamento | Git + GitHub |
| Gestão do projeto | GitHub Projects / Jira |

---


## 🕹️ Como jogar

1. Explore o ambiente da fase atual usando **WASD**
2. Interaja com NPCs ou computadores para acionar um cenário de decisão
3. Leia o dilema apresentado no cartão central
4. Arraste o cartão para a **esquerda** ou **direita** para escolher uma das duas opções
5. Acompanhe os quatro medidores (Confiança, Privacidade, Lucro, Viés) no topo da tela
6. Use os gestos extras quando disponíveis:
   - Segurar o cartão → ver prévia do impacto
   - Arrastar para cima → investigar o dilema
   - Duplo toque → consultar um conselheiro
7. Avance de fase conforme progride na carreira, sobrevivendo o maior número de dias possível sem deixar nenhum medidor zerar ou estourar

---

## 📁 Estrutura do repositório

```
reigns-ia/
├── index.html              # Protótipo jogável (estrutura, estilo e lógica)
├── assets/                 # Imagens, ícones e screenshots
├── docs/                   # Documentação do projeto (entrega acadêmica)
│   ├── visao.md             # Documento de Visão
│   ├── requisitos.md        # Requisitos funcionais e não-funcionais
│   ├── historias-usuario.md # Histórias de usuário (padrão 3Cs)
│   ├── modelagem.md          # Casos de uso e diagramas
│   ├── arquitetura.md        # Arquitetura em alto nível
│   ├── processo.md           # Definição de processo de desenvolvimento
│   └── testes.md             # Estratégias e casos de teste
└── README.md

---

## 📚 Documentação do projeto

Toda a documentação de engenharia de software está disponível na pasta [`/docs`](./docs):

| Documento | Descrição |
|---|---|
| [Visão](./docs/visao.md) | Propósito, escopo e stakeholders do produto |
| [Requisitos](./docs/requisitos.md) | Requisitos funcionais, não-funcionais e restrições |
| [Histórias de Usuário](./docs/historias-usuario.md) | 15+ histórias no padrão 3Cs (Card, Conversation, Confirmation) |
| [Modelagem](./docs/modelagem.md) | Diagrama e descrição de casos de uso |
| [Arquitetura](./docs/arquitetura.md) | Componentes do sistema e decisões técnicas |
| [Processo](./docs/processo.md) | Metodologia, papéis e fluxo de trabalho da equipe |
| [Testes](./docs/testes.md) | Estratégias, tipos de teste e critérios de aceite |

---

## 📋 Gestão do projeto

Sprints, backlog e tarefas é feito no board do Jira:

🔗 [Board do projeto (Jira)](https://algs2.atlassian.net/jira/software/c/projects/PI2E8/boards/8)

---

## 🗺️ Roadmap

- [x] Protótipo jogável com mecânica principal (swipe + medidores)
- [x] Núcleo visual reativo
- [x] Toasts educativos
- [ ] Tela de auditoria funcional
- [ ] Mini-interações (slider de parâmetro, quiz de viés)
- [ ] Persistência de progresso (recorde de dias sobrevividos)
- [ ] Expansão do baralho de dilemas
- [ ] Versão com backend para ranking entre jogadores

---

## 👥 Equipe


| Nome            | Responsabilidade |
|-----------------|------------------|
| Caio Moraes     | caio.moraes1102@gmail.com |
| Pierre Davi     |  pierremelo03@gmail.com |
| Arthur Henrique |  |
| Igor Almeida    |  |
| Daniela Menezes | dani.a.menezes@outlook.com |
| Athos Vieira    | avas@cesar.school |
| José André      | andrefmelo2008@gmail.com |


