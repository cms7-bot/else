![image alt](https://github.com/cms7-bot/else/blob/f5a7c9954424bcde152639e27b5ae30efe460dda/else%20banner.jpeg)

<h3 align="center">
    Você começa como estagiário numa empresa de tecnologia.
  A IA está em todo lugar. O que você faz com ela é escolha sua.
</h3>

<p align="center">
  <img src="https://img.shields.io/badge/status-em%20desenvolvimento-0098DB">
  <img src="https://img.shields.io/badge/C-motor-203562?logo=c&logoColor=white">
  <img src="https://img.shields.io/badge/raylib-visual-1E579C">
  <img src="https://img.shields.io/badge/Haskell-regras-252446?logo=haskell&logoColor=white">
</p>

---

## Sobre 

O **else** é um jogo educativo sobre letramento em Inteligência Artificial. Você entra numa empresa de tecnologia como estagiário e, a cada fase, sobe um degrau na carreira até chegar a Tech Líder. Pelo caminho, a IA aparece em cada canto do escritório: na triagem de currículos, no relatório que ninguém revisou, no chatbot que atende os clientes. E é você quem decide quando usar, quando questionar e quando dizer não.

## Os quatro pilares 
Cada decisão mexe com quatro coisas ao mesmo tempo, e elas aparecem como medidores na tela durante todo o jogo.

| Pilar | A pergunta por trás |
|---|---|
| **Confiança** | Depois dessa decisão, as pessoas ainda acreditam em você e na empresa? |
| **Lucro** | Quanto vale ganhar mais se o custo cai em cima de alguém? |
| **Privacidade** | Que dados das pessoas você está entregando pra uma IA sem perceber? |
| **Viés** | A IA está tratando todo mundo do mesmo jeito? |

## Como o jogo funciona
Você explora o escritório em visão de cima, no estilo dos RPGs clássicos de 16 bits. Quando conversa com um colega ou usa um computador, surge uma carta com um dilema. Arraste pra esquerda ou pra direita pra decidir, e veja os medidores reagirem na hora.

```mermaid
flowchart LR
    A[Explorar o escritório] --> B[Interagir com colega ou computador]
    B --> C[Carta com um dilema]
    C --> D{Esquerda ou direita?}
    D --> E[Medidores mudam]
    E --> F{Algum medidor<br>zerou ou estourou?}
    F -- Não --> A
    F -- Sim --> G[Fim de jogo<br>com uma lição sobre IA]
```
Se qualquer medidor chegar a 0 ou a 100, o jogo acaba. E cada final explica o que deu errado e o que isso ensina sobre IA no mundo real. Equilíbrio é tudo: lucro demais custa confiança, privacidade demais trava a empresa.

## Arquitetura

O jogo é dividido em duas camadas, cada uma com uma responsabilidade clara.

```mermaid
flowchart LR
    J([Jogador]) -- teclado e mouse --> M

    subgraph M[Motor em C + raylib]
        L[Game loop] --> R[Desenho da tela<br>em pixel art]
        L --> E[Estado do jogo<br>cargo, medidores, cartas]
    end

    M -. decisão tomada .-> H[Regras em Haskell<br>pontuação e validação]
    H -. novos valores .-> M

    classDef camada fill:#203562,stroke:#0098DB,stroke-width:2px,color:#ffffff
    classDef futuro fill:#413A42,stroke:#96A2B3,stroke-width:2px,stroke-dasharray:5 5,color:#ffffff
    classDef jogador fill:#0098DB,stroke:#0098DB,color:#ffffff

    class L,R,E camada
    class H futuro
    class J jogador
```

| Camada | Tecnologia | O que faz |
|---|---|---|
| Motor e visual | C + [raylib](https://www.raylib.com/) | Game loop, leitura de teclado e mouse, desenho das telas e controle do estado do jogo |
| Regras | Haskell | Cálculo de pontuação e validação das regras, com funções puras (Unidade 2) |

### Por que essas escolhas

- **C** é requisito do projeto, e engines comerciais (como Unity ou Godot) não são permitidas. O desafio era ter um jogo visual sem sair do C.
- **raylib** resolve isso: é uma biblioteca gráfica leve, feita pra C, que permite desenhar sprites, tocar sons e ler o teclado sem esconder a lógica do jogo atrás de uma engine.
- **Haskell** cuida das regras porque funções puras sempre dão o mesmo resultado pra mesma entrada. Isso torna a pontuação previsível e fácil de testar.

---
## Como rodar

Baixe ou clone o repositório e abra o `jogo.exe` a partir da pasta do projeto (Windows).

<details>
<summary><b>Quero mexer no código</b></summary>

<br>

1. Instale o [MSYS2](https://www.msys2.org/) em `C:\msys64` e, no terminal **MSYS2 UCRT64**, rode:
```
   pacman -Syu
   pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-raylib
```
2. Abra a pasta do projeto no VS Code e confira o compilador com `gcc --version`
3. Compile e rode com `.\jogar.bat`

Deu problema? O [Guia Rápido](./Guia_Rapido_ExecutarJogo.md) resolve os erros mais comuns.

</details>

<details>
<summary><b>Estrutura do projeto</b></summary>

<br>

```
else/
├── Code/              # código-fonte em C
├── cenario/           # imagens dos escritórios
├── Botoes/            # sprites dos botões
├── assets/            # demais imagens do jogo
├── musicas/           # trilhas sonoras
├── EfeitosSonoros/    # sons de interface
├── Documentação/      # documentos de engenharia de software
├── jogar.bat          # compila e abre o jogo
└── jogo.exe           # última versão compilada
```

</details>

---





Toda a documentação de engenharia de software está disponível na pasta [`/Documentação`](./Documenta%C3%A7%C3%A3o):

| Documento | Descrição |
|---|---|
| [Visão](./Documenta%C3%A7%C3%A3o/visao.md) | Propósito, escopo e stakeholders do produto |
| [Requisitos](./Documenta%C3%A7%C3%A3o/requisitos.md) | Requisitos funcionais, não-funcionais e restrições |
| [Histórias de Usuário](./Documenta%C3%A7%C3%A3o/historias-usuario.md) | 15+ histórias no padrão 3Cs (Card, Conversation, Confirmation) |
| [Modelagem](./Documenta%C3%A7%C3%A3o/modelagem.md) | Diagrama e descrição de casos de uso |
| [Arquitetura](./Documenta%C3%A7%C3%A3o/arquitetura.md) | Componentes do sistema e decisões técnicas |
| [Processo](./Documenta%C3%A7%C3%A3o/processo.md) | Metodologia, papéis e fluxo de trabalho da equipe |
| [Testes](./Documenta%C3%A7%C3%A3o/testes.md) | Estratégias, tipos de teste e critérios de aceite |

---

## 📋 Gestão do projeto

Sprints, backlog e tarefas é feito no board do Jira:

🔗 [Board do projeto (trello)](https://trello.com/invite/b/6ab7228e40ff14f38be9f55b/ATTIa4223772b64eccef9d08a9bf0c63c73eED83464F/else-gestao-de-sprints-squad-8)

---

## Entrega 02: Modelagem e Prototipação

**Solução de prototipação escolhida:** Storyboard

### Demonstração

> [🎬 Screencast com áudio e legendas](https://www.youtube.com/watch?v=sPg2rsm8cXE)

### Modelagem e Prototipação

| Artefato | Onde encontrar |
|---|---|
| Diagramas de atividade | [FigJam](https://www.figma.com/board/3xdh1WRdeCV1OWxPhDDmk0) |
| Storyboards | [Figma](https://www.figma.com/design/jk3QiWfnkW9yzDmboFz8R9/Storyboards-Else) |
 

<details>
<summary><b>Rastreabilidade por história de usuário</b></summary>

<br>

| História | Diagrama de atividade | Storyboard |
|---|---|---|
| US01: Iniciar uma nova partida | [ver](https://www.figma.com/board/3xdh1WRdeCV1OWxPhDDmk0/US01---Iniciar-uma-nova-partida?node-id=97-563) | [ver](https://www.figma.com/design/2BYlnZ1Bia64w96XLPb5Eq/ELSE-%C2%B7-Storyboards-v2--US01-a-US15-?node-id=11-2&t=rp6Ep2IlsBj6sKU6-4) |
| US02: Tomar uma decisão e ver o impacto | [ver](https://www.figma.com/board/3xdh1WRdeCV1OWxPhDDmk0/US01---Iniciar-uma-nova-partida?node-id=100-513) | [ver](https://www.figma.com/design/2BYlnZ1Bia64w96XLPb5Eq/ELSE-%C2%B7-Storyboards-v2--US01-a-US15-?node-id=11-210&t=rp6Ep2IlsBj6sKU6-4) |
| US03: HUD e regras | [ver](https://www.figma.com/board/3xdh1WRdeCV1OWxPhDDmk0/US01---Iniciar-uma-nova-partida?node-id=101-593) | [ver](https://www.figma.com/design/2BYlnZ1Bia64w96XLPb5Eq/ELSE-%C2%B7-Storyboards-v2--US01-a-US15-?node-id=11-464&t=rp6Ep2IlsBj6sKU6-4) |
| US04: Consequências passadas | [ver](https://www.figma.com/board/3xdh1WRdeCV1OWxPhDDmk0/US01---Iniciar-uma-nova-partida?node-id=100-530) | [ver](https://www.figma.com/design/2BYlnZ1Bia64w96XLPb5Eq/ELSE-%C2%B7-Storyboards-v2--US01-a-US15-?node-id=11-736&t=rp6Ep2IlsBj6sKU6-4) |
| US05: Receber notificações inesperadas | [ver](https://www.figma.com/board/3xdh1WRdeCV1OWxPhDDmk0/US01---Iniciar-uma-nova-partida?node-id=100-537) | [ver](https://www.figma.com/design/2BYlnZ1Bia64w96XLPb5Eq/ELSE-%C2%B7-Storyboards-v2--US01-a-US15-?node-id=11-950&t=rp6Ep2IlsBj6sKU6-4) |
| US06: Aprender um conceito de IA após a decisão | [ver](https://www.figma.com/board/3xdh1WRdeCV1OWxPhDDmk0/US01---Iniciar-uma-nova-partida?node-id=100-541) | [ver](https://www.figma.com/design/2BYlnZ1Bia64w96XLPb5Eq/ELSE-%C2%B7-Storyboards-v2--US01-a-US15-?node-id=11-1202&t=rp6Ep2IlsBj6sKU6-4) |
| US07: Promoção e troca de escritório | [ver](https://www.figma.com/board/3xdh1WRdeCV1OWxPhDDmk0/US01---Iniciar-uma-nova-partida?node-id=100-548) | [ver](https://www.figma.com/design/2BYlnZ1Bia64w96XLPb5Eq/ELSE-%C2%B7-Storyboards-v2--US01-a-US15-?node-id=11-1455&t=rp6Ep2IlsBj6sKU6-4) |
| US08: Tela de fim de jogo | [ver](https://www.figma.com/board/3xdh1WRdeCV1OWxPhDDmk0/US01---Iniciar-uma-nova-partida?node-id=100-561) | [ver](https://www.figma.com/design/2BYlnZ1Bia64w96XLPb5Eq/ELSE-%C2%B7-Storyboards-v2--US01-a-US15-?node-id=11-1664&t=rp6Ep2IlsBj6sKU6-4) |
| US09: Reiniciar a partida | [ver](https://www.figma.com/board/3xdh1WRdeCV1OWxPhDDmk0/US01---Iniciar-uma-nova-partida?node-id=106-768) | [ver](https://www.figma.com/design/2BYlnZ1Bia64w96XLPb5Eq/ELSE-%C2%B7-Storyboards-v2--US01-a-US15-?node-id=11-1825&t=rp6Ep2IlsBj6sKU6-4) |
| US10: Compartilhar o resultado | [ver](https://www.figma.com/board/3xdh1WRdeCV1OWxPhDDmk0/US01---Iniciar-uma-nova-partida?node-id=109-2) | [ver](https://www.figma.com/design/2BYlnZ1Bia64w96XLPb5Eq/ELSE-%C2%B7-Storyboards-v2--US01-a-US15-?node-id=11-1932&t=rp6Ep2IlsBj6sKU6-4) |
| US11: Passar pela auditoria | [ver](https://www.figma.com/board/3xdh1WRdeCV1OWxPhDDmk0/US01---Iniciar-uma-nova-partida?node-id=106-766) | [ver](https://www.figma.com/design/2BYlnZ1Bia64w96XLPb5Eq/ELSE-%C2%B7-Storyboards-v2--US01-a-US15-?node-id=11-2001&t=rp6Ep2IlsBj6sKU6-4) |
| US12: Conversar com colegas no escritório | [ver](https://www.figma.com/board/3xdh1WRdeCV1OWxPhDDmk0/US01---Iniciar-uma-nova-partida?node-id=106-765) | [ver](https://www.figma.com/design/2BYlnZ1Bia64w96XLPb5Eq/ELSE-%C2%B7-Storyboards-v2--US01-a-US15-?node-id=11-2272&t=rp6Ep2IlsBj6sKU6-4) |
| US13: Resolver enigma da máquina de café | [ver](https://www.figma.com/board/3xdh1WRdeCV1OWxPhDDmk0/US01---Iniciar-uma-nova-partida?node-id=112-12) | [ver](https://www.figma.com/design/2BYlnZ1Bia64w96XLPb5Eq/ELSE-%C2%B7-Storyboards-v2--US01-a-US15-?node-id=11-2533&t=rp6Ep2IlsBj6sKU6-4) |
| US14: Desbloquear finais diferentes | [ver](https://www.figma.com/board/3xdh1WRdeCV1OWxPhDDmk0/US01---Iniciar-uma-nova-partida?node-id=100-585) | [ver](https://www.figma.com/design/2BYlnZ1Bia64w96XLPb5Eq/ELSE-%C2%B7-Storyboards-v2--US01-a-US15-?node-id=11-2713&t=rp6Ep2IlsBj6sKU6-4) |
| US15: Continuar de onde parei | [ver](https://www.figma.com/board/3xdh1WRdeCV1OWxPhDDmk0/US01---Iniciar-uma-nova-partida?node-id=105-746) | [ver](https://www.figma.com/design/2BYlnZ1Bia64w96XLPb5Eq/ELSE-%C2%B7-Storyboards-v2--US01-a-US15-?node-id=11-2800&t=rp6Ep2IlsBj6sKU6-4) |

</details>



