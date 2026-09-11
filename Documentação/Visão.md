# Documento de Visão — else

---

## 1. Descrição geral do produto

**else** é um jogo de exploração e decisões, inspirado no jogo *Reigns*, no qual o jogador assume o papel de responsável por uma empresa/laboratório de Inteligência Artificial. O jogador explora os ambientes de cada fase em perspectiva top-down, movimentando o personagem com **WASD** e interagindo com NPCs e computadores para acionar cenários de decisão. A cada cenário, o jogador recebe um dilema real enfrentado por empresas de tecnologia — como vieses algorítmicos, vazamento de dados, deepfakes e pressão de investidores — e precisa decidir arrastando um cartão para a esquerda ou para a direita.

Cada decisão impacta quatro medidores que representam os pilares de equilíbrio da empresa: **Confiança**, **Privacidade**, **Lucro** e **Viés**. Conforme avança, o jogador progride por diferentes fases de carreira — de **Estagiário** a **Tech Lead** — cada uma com seu próprio ambiente e conjunto de dilemas. O objetivo é sobreviver o maior número de dias possível sem deixar nenhum medidor zerar ou estourar, enquanto aprende, na prática, conceitos reais de letramento em Inteligência Artificial.

O produto combina uma mecânica de exploração simples com o núcleo de decisão por *card-swipe*, já validado pelo gênero, e uma camada educativa: toasts explicativos, mini-quizzes, telas de auditoria e finais de jogo narrativos, todos baseados em situações reais do campo de IA.

---

## 2. Problema / Oportunidade

Discussões sobre Inteligência Artificial se tornaram parte do cotidiano em notícias, redes sociais e no ambiente de trabalho, mas grande parte da população não tem contato prático com os dilemas éticos e técnicos que envolvem o desenvolvimento e uso de sistemas de IA. Conceitos como viés algorítmico, alucinação de modelos, deepfakes e privacidade de dados costumam ser tratados de forma técnica ou abstrata, distante da experiência de quem não trabalha diretamente na área.

Ao mesmo tempo, jogos de decisão no estilo *Reigns* têm um histórico comprovado de engajamento por serem simples, rápidos e viciantes, sem exigir tutoriais extensos. **A oportunidade identificada é usar essa mecânica já validada como veículo para letramento em IA**, tornando o aprendizado sobre riscos, limitações e responsabilidades da tecnologia algo acessível, interativo e memorável — em vez de puramente teórico.

---

## 3. Objetivo do produto

### 3.1 Objetivo principal
Proporcionar uma experiência de jogo envolvente que, através de decisões simuladas de gestão de uma empresa de IA, promova o letramento prático do jogador sobre conceitos fundamentais de Inteligência Artificial e seus riscos éticos, sociais e técnicos.

### 3.2 Objetivos específicos
- Traduzir conceitos técnicos de IA (viés, alucinação, privacidade, deepfake, segurança de dados) em dilemas de fácil compreensão
- Demonstrar, na prática, que decisões sobre IA envolvem trade-offs — raramente existe uma opção sem consequência
- Gerar engajamento e replay por meio de uma mecânica simples, rápida e visualmente reativa
- Servir como material de apoio educativo, utilizável em contextos de ensino introdutório sobre IA

### 3.3 Público-alvo
- Estudantes e curiosos sobre tecnologia e Inteligência Artificial, sem necessidade de conhecimento técnico prévio
- Professores e educadores que busquem uma ferramenta lúdica de apoio para introduzir temas de ética e letramento em IA
- Jogadores casuais interessados em jogos de decisão rápida (fãs do gênero *Reigns*-like)

---

## 4. Escopo do produto

### 4.1 Escopo incluso na versão atual (MVP)
- Exploração de ambientes em perspectiva top-down, com movimentação do personagem via WASD
- Interação com NPCs e computadores para acionar cenários de decisão
- Mecânica principal de decisão por arraste (swipe esquerda/direita), com suporte a mouse e toque
- Sistema de 4 medidores (Confiança, Privacidade, Lucro, Viés) que reagem a cada decisão
- Baralho de cartas/dilemas baseados em situações reais do campo de IA
- Progressão de carreira em fases (Estagiário → Programador Júnior → Pleno → Sênior → Tech Lead), cada uma com seu ambiente
- Núcleo visual da IA que reage ao estado geral do sistema (estável, em alerta, crítico)
- Interações complementares: prévia de impacto ao segurar o cartão, investigação de dilema (arrastar para cima), consulta a conselheiros (duplo toque)
- Eventos dinâmicos: notificações inesperadas simulando notícias/eventos externos
- Camada educativa: toasts explicativos pós-decisão, mini-quiz de viés, tela de auditoria periódica
- Múltiplos finais de jogo, cada um associado a uma lição educativa distinta
- Opção de reiniciar a partida
- Interface responsiva (funcional em desktop e mobile)

### 4.2 Fora do escopo desta versão
- Modo multiplayer ou disputa entre jogadores
- Aplicativo mobile nativo (a versão atual roda via navegador/web)
- Sistema de contas de usuário, login ou persistência de progresso entre dispositivos
- Backend próprio ou ranking global entre jogadores
- Expansões pagas ou monetização
- Tradução para outros idiomas além do português

---

## 5. Stakeholders

| Stakeholder | Interesse no projeto |
|---|---|
| **Jogadores/Usuários finais** | Experiência de jogo envolvente e aprendizado prático sobre IA |
| **Professor(a) da disciplina** | Avaliação da aplicação correta de conceitos de Fundamentos de Desenvolvimento de Software (visão, requisitos, modelagem, arquitetura, processo e testes) |
| **Dupla desenvolvedora** | Entrega do projeto dentro do prazo, com qualidade técnica e de documentação |
| **Instituições educacionais (potenciais)** | Uso do jogo como material de apoio para ensino introdutório de IA e ética tecnológica |

---

## 6. Contexto e justificativa

O tema Inteligência Artificial está no centro do debate público e tecnológico atual, mas o letramento sobre o assunto ainda é desigual: enquanto profissionais da área discutem riscos e limitações com profundidade, grande parte da sociedade tem contato apenas superficial — muitas vezes limitado ao uso de chatbots ou geradores de imagem, sem entender os riscos por trás dessas ferramentas (viés, desinformação, privacidade, entre outros).

Jogos de decisão rápida, como o já consagrado *Reigns*, demonstraram que é possível ensinar sistemas complexos (na obra original, política e governança medieval) por meio de escolhas simples e consequências visíveis, sem depender de textos longos ou aulas formais. **REIGNS: IA** aplica esse mesmo princípio ao campo da Inteligência Artificial, aproveitando um formato já validado de engajamento para abordar um tema de relevância crescente e urgente.

Além do valor educacional, o projeto também serve como estudo de caso completo de desenvolvimento de software dentro da disciplina, cobrindo desde a concepção do produto até sua arquitetura técnica, processo de trabalho em equipe e estratégias de teste — permitindo à dupla aplicar, na prática, os fundamentos estudados ao longo do semestre.

---

## 7. Visão de sucesso

O projeto será considerado bem-sucedido se, ao final do desenvolvimento:
- O jogador conseguir completar uma partida do início ao fim sem instruções externas, apenas pela interface
- Cada decisão tomada resultar em uma consequência clara e compreensível nos medidores
- O jogador conseguir associar, mesmo que informalmente, pelo menos um conceito de IA (viés, alucinação, deepfake, privacidade, etc.) a uma situação vivida durante o jogo
- O protótipo funcionar corretamente em desktop e mobile, sem necessidade de instalação
- A documentação de engenharia de software (visão, requisitos, modelagem, arquitetura, processo, testes) refletir fielmente o produto implementado
