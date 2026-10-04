# ELSE · Documento de Visão

## 1. O que é o ELSE

O **ELSE** é um jogo educativo em pixel art sobre ética e uso responsável de Inteligência Artificial, inspirado no jogo *Reigns*. O jogador assume o papel de responsável pela IA de uma empresa e começa a carreira como **Estagiário**.

Ele anda por uma sala de escritório vista de cima, com enquadramento fixo, e interage com colegas e computadores. Cada interação abre um **dilema real do mundo da IA** (viés, alucinação, privacidade, dependência) apresentado numa carta. O jogador inclina a carta para a esquerda ou para a direita para escolher uma postura.

Cada escolha mexe em quatro medidores: **Confiança, Viés, Privacidade e Lucro**. Todos começam em 50 e devem ficar na faixa segura, entre 30 e 70. Se qualquer um chegar a 0 ou a 100, a empresa cai e a partida termina. O objetivo é equilibrar a empresa e crescer na carreira sem derrubá-la.

## 2. Problema e oportunidade

A Inteligência Artificial faz parte do dia a dia, mas a maioria das pessoas só tem contato com ela como usuária, sem entender os riscos por trás: discriminação por algoritmos, respostas inventadas, uso indevido de dados e dependência cega de sistemas automáticos. Esses temas costumam ser explicados de forma técnica ou abstrata.

Jogos de decisão rápida, como *Reigns*, mostram que é possível ensinar sistemas complexos com escolhas simples e consequências visíveis, sem textos longos. **A oportunidade do ELSE é usar essa mecânica, já conhecida e engajante, para levar letramento em IA a quem não é da área.**

## 3. Objetivos

**Objetivo principal:** fazer o jogador viver, na prática, os dilemas éticos de quem trabalha com IA e perceber que quase toda decisão tem um custo.

**Objetivos específicos:**
- Transformar conceitos de IA em dilemas de trabalho fáceis de entender.
- Mostrar que não existe escolha sem consequência: o que melhora um medidor costuma piorar outro.
- Ensinar com pouco texto, deixando os efeitos aparecerem pelos medidores.
- Gerar vontade de jogar de novo, com finais diferentes para cada forma de perder.

## 4. Público-alvo

- Estudantes e curiosos sobre tecnologia, sem conhecimento técnico de IA.
- Professores que buscam uma ferramenta lúdica para introduzir ética e IA.
- Jogadores casuais que gostam de jogos de decisão rápida.

## 5. Escopo

### 5.1 MVP da Unidade 1 (entrega de PIF)
A fase do **Estagiário** jogável do início ao fim: menu, sala com movimento, cartas de dilema, os 4 medidores no HUD, fim de jogo e jogar novamente. O detalhamento está em [Requisitos](./requisitos.md).

### 5.2 Unidade 2
Progressão de carreira (Estagiário, Júnior, Pleno, Sênior e Tech Líder), consequências de decisões passadas, notícias inesperadas, auditoria, conversa com colegas, enigma da máquina de café, galeria de finais, salvar progresso e regras do jogo em Haskell.

### 5.3 Fora do escopo
- Versão web ou para celular. O ELSE é um jogo de computador feito em C com raylib.
- Multiplayer, ranking online, contas de usuário ou servidor próprio.
- Monetização e tradução para outros idiomas.

## 6. Stakeholders

| Stakeholder | Interesse |
|---|---|
| Jogadores | Uma experiência divertida que ensine algo real sobre IA |
| Professores do Projeto Integrador (PIF, FDS, LMC, IHC e Gestão) | Avaliar a aplicação dos conteúdos de cada disciplina no produto |
| Squad 8 (7 integrantes) | Entregar o projeto no prazo, com qualidade técnica e documentação coerente |
| Instituições de ensino (potencial) | Usar o jogo como apoio para ensinar ética em IA |

## 7. Como saberemos que deu certo

- Um jogador que nunca viu o jogo consegue jogar uma partida inteira sem ajuda.
- Cada escolha produz um efeito claro e visível nos medidores.
- Ao final, o jogador consegue citar pelo menos um conceito de IA que viveu no jogo (por exemplo, viés ou alucinação).
- O jogo compila e roda no Windows sem erros.
- A documentação descreve fielmente o jogo que foi implementado.
