# ELSE · Processo de Trabalho

## 1. Metodologia

A Squad 8 usa um **Scrum simplificado**, adaptado ao calendário acadêmico:

- **Sprints de duas semanas**, seguindo o cronograma oficial do Projeto Integrador (W01 a W17-18).
- **Organização semanal dentro da sprint:** o trabalho é planejado e acompanhado semana a semana, de domingo a sábado.
- **Priorização com MoSCoW:** Must Have (obrigatório), Should Have (importante), Could Have (se der tempo) e Won't Have (não nesta fase).

## 2. Papéis

| Papel | Quem | O que faz |
|---|---|---|
| Product Owner (PO) | Daniela dos Anjos | Mantém e prioriza o backlog, escreve as histórias de usuário, define o que entra em cada entrega e aprova o que é entregue do ponto de vista do produto |
| Scrum Master (SM) | Igor Almeida | Cuida do processo e do quadro, acompanha prazos e dependências e ajuda a remover bloqueios |

Cada frente de trabalho segue a **matriz RACI** abaixo:
**R** faz · **A** aprova (só uma pessoa) · **C** é consultado · **I** é informado.

| Frente | R | A | C |
|---|---|---|---|
| Gestão ágil: backlog e histórias (FP2) | Dani | Dani | Almeida, Arthur |
| Processo, quadro e cerimônias | Almeida | Dani | Todos |
| Engenharia de Software (FDS) | Caio, Athos | Almeida | Pierre, França |
| Design de Interação (IHC) | Dani, Arthur, França | Arthur | Almeida |
| Arte, assets e UI visual | França | Dani | Arthur, Almeida |
| Lógica Matemática (LMC) | Pierre, Caio | Pierre | Athos |
| Motor em C + raylib (PIF) | Almeida, Pierre | Almeida | França, Arthur, Dani |
| Regras em Haskell (PIF) | Athos, Caio, Arthur | Athos | Pierre |

Quem não aparece numa linha é **informado** sobre aquela frente.

## 3. Rotina semanal

| Dia | O que acontece |
|---|---|
| Domingo | A PO publica no grupo o comunicado da semana, com prazos, responsáveis e dependências |
| Até terça | Ajustes no backlog da semana |
| Terça | Reunião curta depois da aula (20 minutos) |
| Véspera de entrega | Congelamento: depois disso, só correções |
| Sábado | Cada integrante atualiza seus cartões no Trello |

## 4. Quadro no Trello

O quadro é organizado **por sprint**: cada coluna é uma sprint do cronograma, e a sprint atual tem o marcador **⬅ ATUAL**. Cada tarefa fica na coluna da semana em que precisa ser entregue.

Cada cartão traz:
- **No título:** a frente (como [PIF] ou [FDS]) e o nome do responsável.
- **Na descrição:** prioridade, RACI, o que fazer, o critério de pronto, a quem consultar em caso de dúvida e quem aprova no final.
- **Na data:** o prazo limite da tarefa. Entregar antes é sempre bem-vindo.

**Limite de trabalho em andamento:** no máximo 2 cartões por pessoa ao mesmo tempo.

## 5. Comunicação

| Canal | Uso |
|---|---|
| Grupo da equipe | Comunicado semanal fixado e avisos importantes. Evitar mensagens soltas |
| Comentários no cartão do Trello | Dúvidas e discussões sobre uma tarefa específica |
| Slack da turma | Dúvidas com os professores |
| Reunião de terça | Alinhamento rápido da semana |

**Regra de bloqueio:** se travou, avisa no mesmo dia, principalmente quando outra pessoa depende da sua tarefa.

## 6. Versionamento

- Código e documentação ficam no GitHub: [cms7-bot/else](https://github.com/cms7-bot/else).
- As mensagens de commit seguem o padrão **Conventional Commits**:

| Tipo | Quando usar | Exemplo |
|---|---|---|
| `feat` | Nova funcionalidade | `feat(cartas): adiciona cartas do Estagiário` |
| `fix` | Correção de erro | `fix(build): unifica scripts de compilação` |
| `docs` | Documentação | `docs: adiciona requisitos do MVP da U1` |
| `refactor` | Melhoria de código sem mudar o comportamento | `refactor(hud): remove repetição no carregamento de texturas` |

- Todos os integrantes devem ter commits próprios, já que a distribuição de commits é avaliada.

## 7. Ferramentas

| Ferramenta | Uso |
|---|---|
| Trello | Gestão das sprints e das tarefas |
| GitHub | Versionamento do código e da documentação |
| Figma e FigJam | Wireframes, storyboards e diagramas |
| Miro | Rascunhos e sketches |
| Google Drive | Documentos de trabalho, como o conteúdo das cartas |
| PlantUML | Diagramas de atividades |

## 8. Definition of Done

Uma tarefa só está pronta quando:
- Cumpre o critério de pronto descrito no cartão.
- Foi revisada e aprovada pelo aprovador (A da RACI).
- Se for código: compila sem erros e tem commit no padrão Conventional Commits.
- O link da entrega (commit, Figma, vídeo ou documento) está colado no cartão.
