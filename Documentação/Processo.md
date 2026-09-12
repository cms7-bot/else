# Definição de Processo — [Nome a definir]
---

## 1. Metodologia adotada

Com uma equipe de 7 pessoas, o projeto passa a comportar uma estrutura mais próxima do **Scrum tradicional**, onde papéis formais não se justificam. Optou-se por um **Scrum simplificado com sprints curtos** (1 semana cada), combinado com um **quadro Kanban** para visualização do fluxo dentro de cada sprint.

Justificativa:
- Uma equipe de 7 pessoas precisa de papéis claros para evitar sobreposição de trabalho e gargalos de decisão
- Sprints curtos (1 semana) se encaixam melhor no calendário da disciplina do que sprints de 2-4 semanas, permitindo entregas parciais frequentes
- O quadro Kanban dentro de cada sprint mantém a simplicidade visual do fluxo de trabalho, sem burocratizar demais o processo

---

## 2. Papéis da equipe

| Papel | Responsável | Função |
|---|---|---|
| **Facilitador da sprint** | Conduz a Planning e a Retrospectiva, mantém o quadro Kanban atualizado, comunica bloqueios ao restante da equipe |
| **Guardião da Visão** | Confere se as entregas da semana continuam coerentes com o Documento de Visão antes da Review |
| **Revisor de tarefa** | Aprova ou pede ajustes antes de mover o card para "Concluído" |
| **Responsável pela entrega documental** | Principal responsável da entrega da tarefa em específico ||

---

## 3. Cerimônias Scrum

| Cerimônia | Frequência | Objetivo |
|---|---|---|
| **Planning** | Início de cada sprint (semanal) | Definir o que será feito na semana, distribuir os cards do backlog entre os membros |
| **Daily assíncrona** | 3x por semana (não precisa ser diária, dado o contexto acadêmico) | Cada integrante compartilha em texto (grupo/canal) o que fez, o que vai fazer e se há impedimentos |
| **Review** | Fim de cada sprint | Demonstração do que foi produzido (documento ou funcionalidade do protótipo) para o restante da equipe |
| **Retrospectiva** | Fim de cada sprint | O que funcionou bem, o que travou, o que ajustar no próximo sprint |

---

## 4. Fluxo de trabalho (quadro Kanban dentro do sprint)

```
BACKLOG  →  EM ANDAMENTO  →  EM REVISÃO  →  CONCLUÍDO
```

- **Backlog:** histórias de usuário e tarefas de documentação, priorizadas pelo PO
- **Em andamento:** limite de 1-2 cards por pessoa simultaneamente (WIP limit), para evitar dispersão em uma equipe grande
- **Em revisão:** toda tarefa concluída passa por revisão de **pelo menos 1 outro integrante** antes de ser considerada pronta — em equipe de 7, a revisão cruzada é dividida em pares fixos por sprint (definidos na Planning), não aleatória
- **Concluído:** aprovado pelo revisor e, no caso de documentos estruturais (Visão, Requisitos), também validado pelo PO

---

## 5. Ferramentas utilizadas

| Categoria | Ferramenta | Uso |
|---|---|---|
| Gestão do quadro | GitHub Projects (ou Trello) | Quadro Kanban com as 4 colunas, cards vinculados às issues do repositório |
| Versionamento de código | Git + GitHub | Controle de versão do protótipo e da documentação, com Pull Requests obrigatórios |
| Comunicação da equipe | Discord ou WhatsApp (canal único) | Alinhamentos rápidos, dailies assíncronas, avisos de bloqueios |
| Reuniões síncronas | Google Meet ou presencial | Planning, Review e Retrospectiva semanais |
| Documentação | Markdown (`.md`) no repositório, organizados em `/docs` | Visão, Requisitos, Histórias de Usuário, Modelagem, Arquitetura, Processo e Testes |

---

## 6. Fluxo de trabalho no Git

.......

---

## 7. Critérios de "pronto" 

Uma tarefa só é movida para **Concluído** quando:
- Foi revisada por pelo menos 1 outro integrante da equipe (par definido na Planning)
- É coerente com o Documento de Visão, sem contradizer decisões de escopo já tomadas
- No caso de funcionalidades do protótipo: foi testada manualmente em desktop e mobile, e validada pela pessoa responsável por Testes
- No caso de documentos estruturais (Visão, Requisitos, Arquitetura): validada também pelo Product Owner 
- Não possui pendências como "[Nome integrante]" ou seções incompletas
