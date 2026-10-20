# else: Histórias de Usuário

Histórias de Usuário, Critérios de Aceitação e Definition of Done

Projeto Integrador 2026.2 · Squad 8 · Análise e Desenvolvimento de Sistemas, CESAR School

## Visão geral

O else é um jogo educativo em pixel art sobre ética e uso responsável de Inteligência Artificial. O jogador começa como estagiário em uma empresa de tecnologia onde a IA está presente em todo o trabalho, decide o que fazer com ela e avança na carreira de Estagiário a Tech Líder, tomando decisões apresentadas como cards que podem ser arrastados para a esquerda ou para a direita. Cada decisão altera quatro medidores, Confiança, Privacidade, Lucro e Viés, que começam em 50 e devem ficar na faixa segura, entre 30 e 70. Se algum deles chegar a 0 ou a 100, a partida termina.

Este documento reúne as 15 histórias de usuário do produto. Cada história traz a descrição no formato padrão, os critérios de aceitação no formato Dado, Quando, Então, os pontos que ainda dependem de decisão da equipe e a Definition of Done específica. A Definition of Done comum a todas as histórias está na última seção. As regras de jogo citadas aqui seguem o Relatório da Mecânica e da Lógica Proposicional (LMC, AV1).

## Resumo das histórias

| ID   | História                                        | Objetivo para o jogador                            |
|------|-------------------------------------------------|----------------------------------------------------|
| US01 | Iniciar uma nova partida                        | Começar a jogar como Estagiário                    |
| US02 | Tomar uma decisão e ver o impacto               | Escolher uma postura diante de um dilema           |
| US03 | HUD de medidores e regras da empresa            | Acompanhar a situação da empresa                   |
| US04 | Enfrentar as consequências de decisões passadas | Perceber que as escolhas têm efeito no futuro      |
| US05 | Receber notificações inesperadas                | Sentir a tensão do dia a dia na empresa            |
| US06 | Aprender um conceito de IA após a decisão       | Aprender um conceito real enquanto joga            |
| US07 | Ser promovido e mudar de escritório             | Perceber o próprio crescimento na carreira         |
| US08 | Tela de fim de jogo                             | Entender por que a partida terminou                |
| US09 | Reiniciar a partida                             | Tentar uma nova estratégia                         |
| US10 | Compartilhar o resultado                        | Mostrar o resultado para outras pessoas            |
| US11 | Passar por uma auditoria e sair dela            | Corrigir a empresa quando ela entra em risco ético |
| US12 | Conversar com colegas no escritório             | Descobrir histórias e pistas no ambiente           |
| US13 | Resolver o enigma da máquina de café            | Ganhar um power-up com raciocínio lógico           |
| US14 | Desbloquear finais diferentes                   | Ter motivo para jogar de novo                      |
| US15 | Continuar de onde parei                         | Não perder o progresso ao fechar o jogo            |

## Histórias de usuário

### US01: Iniciar uma nova partida

#### Descrição

Como jogador, quero iniciar uma nova partida, para começar a tomar decisões no dia a dia de uma empresa onde a IA está em todo lugar.

#### Critérios de aceitação

- Dado que o jogador está na tela inicial, quando seleciona "Jogar", então uma nova partida é inicializada.

- Dado que a partida foi inicializada, seja no primeiro acesso ou após um reinício (US09), então o sistema exibe o escritório da fase Estagiário, os 4 medidores em 50 e o contador "DIA 1".

- Dado que o escritório foi carregado, então o jogador movimenta o personagem livremente com WASD, sem que nenhum card de dilema apareça automaticamente.

- Dado que a partida acabou de começar, então nenhum medidor foi alterado em relação ao valor inicial.

- Dado que o jogador leva o personagem até um NPC de dilema ou um computador, quando interage, então o primeiro dilema é acionado (US02).

#### Pontos em aberto

- Definir os limites do mapa da fase Estagiário e o comportamento de colisão do personagem.

#### Definition of Done específica

- Lógica de inicialização implementada como função única, reutilizada pela US09 sem duplicação de código.

- Valor inicial dos 4 medidores parametrizável, definido em um único lugar do código.

- Movimentação com WASD testada nos 4 eixos e nas diagonais, sem travar nas bordas do ambiente.

- Teste confirmando que o primeiro acesso e o reinício levam ao mesmo estado inicial.

### US02: Tomar uma decisão e ver o impacto

#### Descrição

Como jogador, quero responder a dilemas éticos sobre o uso de IA, para refletir sobre as consequências das minhas decisões no trabalho.

#### Critérios de aceitação

- Dado que o jogador está no escritório, quando interage com um NPC de dilema ou um computador, então um card com um cenário contextualizado é exibido (por exemplo: dados desbalanceados, respostas incorretas dadas com confiança, coleta de dados sem consentimento ou delegação excessiva à IA).

- Dado que um card está visível, quando o jogador o arrasta para a esquerda ou para a direita, então cada lado representa uma postura diferente diante do dilema.

- Dado que o jogador confirma uma escolha, quando a decisão é processada, então somente os medidores previstos para aquele lado mudam, e os demais permanecem iguais.

- Dado que a escolha foi confirmada, então o card fecha e o controle volta ao jogador no escritório.

- Dado que o jogador solta o card sem arrastar até o limite, então nenhuma decisão é aplicada e o card volta ao centro.

- Dado que uma partida está em andamento, então nenhum dilema se repete dentro da mesma sessão.

- Dado que um lado do card tem uma pré-condição falsa (por exemplo, contratar uma consultoria de ética com o Lucro abaixo de 30), então esse lado fica bloqueado, a condição aparece escrita nele e o card só pode ir para o outro lado.

#### Pontos em aberto

- Definir a distância mínima de arrasto que confirma uma decisão.

#### Definition of Done específica

- Testes cobrindo o arrasto para os dois lados, incluindo os valores-limite dos medidores (0 e 100).

- Cada dilema cadastrado com texto de cenário, duas posturas e o efeito de cada uma nos medidores.

- Nenhum medidor é alterado sem um arrasto completo; um gesto parcial não aplica efeito.

- Base inicial com pelo menos 8 dilemas cobrindo os 4 pilares educativos: confiança, privacidade, lucro e viés.

- Efeitos e pré-condições de cada card calculados pelo módulo de regras em Haskell, seguindo as fórmulas do relatório de LMC.

### US03: HUD de medidores e regras da empresa

#### Descrição

Como jogador, quero ver os 4 medidores sempre na tela e consultar as regras da empresa com seus valores atuais, para entender a situação da empresa e planejar minhas decisões.

#### Critérios de aceitação

- Dado que uma partida está em andamento, então os medidores de Confiança, Privacidade, Lucro e Viés ficam visíveis em um HUD fixo durante todo o jogo.

- Dado que uma decisão foi confirmada, quando os medidores mudam, então cada barra anima a variação e indica se o valor subiu ou desceu.

- Dado que um medidor sai da faixa segura, entre 30 e 70, então ele recebe um destaque visual de alerta.

- Dado que um card de decisão está visível, então o HUD não mostra nenhuma prévia do efeito das escolhas.

- Dado que o jogador abre o painel Regras da empresa, então as regras de promoção, auditoria e fim de jogo aparecem com a fórmula, o valor atual de cada proposição e o resultado, recalculados a cada jogada.

#### Pontos em aberto

- Definir o visual do painel de regras para que as fórmulas fiquem claras para jogadores sem formação em lógica.

#### Definition of Done específica

- Faixa segura testada nos dois limites (30 e 70) de cada medidor.

- Valores do painel calculados pela mesma função do módulo Haskell que o jogo usa, sem fórmulas reescritas na interface.

- Animação de variação testada para subida e para descida de cada medidor.

- Medidores diferenciados por cor e por ícone, garantindo leitura por jogadores com daltonismo.

### US04: Enfrentar as consequências de decisões passadas

#### Descrição

Como jogador, quero que algumas das minhas decisões gerem novos dilemas dias depois, para sentir que as escolhas têm peso e acompanham a minha trajetória.

#### Critérios de aceitação

- Dado que o jogador tomou uma decisão marcada como geradora de consequência, quando o número de dias definido passa, então um card de consequência ligado àquela escolha entra no baralho.

- Dado que o card de consequência é exibido, então o texto faz referência clara à decisão original.

- Dado que o jogador escolheu o outro lado do dilema original, então o card de consequência correspondente não aparece.

- Dado que o jogador responde ao card de consequência, então ele segue as mesmas regras de decisão da US02, incluindo o formato de implicação para cada lado.

#### Pontos em aberto

- Definir quantos dias depois a consequência aparece e quantos dilemas da base geram consequência.

- Definir se uma consequência pode gerar outra consequência, formando uma cadeia.

- Incluir no relatório de LMC uma proposição que registre a decisão anterior, para que o card de consequência também seja descrito em lógica.

#### Definition of Done específica

- Registro das decisões que geram consequência armazenado no estado da partida.

- Teste confirmando que a consequência só aparece para o lado escolhido.

- Teste confirmando que o reinício da partida (US09) apaga as consequências pendentes.

### US05: Receber notificações inesperadas

#### Descrição

Como jogador, quero que notícias aleatórias apareçam na tela após minhas decisões, para sentir a tensão de uma empresa real em que a IA está por toda parte sem que isso interrompa o jogo.

#### Critérios de aceitação

- Dado que uma decisão foi confirmada, quando o sorteio de probabilidade (cerca de 28%) é bem-sucedido, então uma notícia do banco de notícias aparece no topo da tela.

- Dado que uma notícia está visível, quando o tempo definido passa (por exemplo, 4 segundos), então ela desaparece sozinha, sem exigir interação.

- Dado que o jogador não interage com a notícia, então cards, medidores e contador de dias seguem funcionando normalmente.

#### Pontos em aberto

- Definir como a notícia convive com o aviso educativo da US06 quando os dois são exibidos na mesma decisão.

- Definir se uma notícia pode se repetir dentro da mesma partida.

#### Definition of Done específica

- Banco de notícias com quantidade suficiente para uma partida média, ou regra de repetição definida.

- Teste de sobreposição visual com o aviso da US06.

- Validado que a notificação nunca bloqueia cliques em elementos do jogo.

### US06: Aprender um conceito de IA após a decisão

#### Descrição

Como jogador, quero ver, após cada decisão, uma explicação curta da consequência da minha escolha e do conceito de IA envolvido, para entender o que aconteceu e aprender algo prático enquanto jogo.

#### Critérios de aceitação

- Dado que uma decisão foi confirmada, quando o resultado é processado, então um aviso aparece no rodapé da tela explicando a consequência da escolha e o conceito de IA relacionado (por exemplo: viés algorítmico, alucinação ou deepfake).

- Dado que o aviso está visível, quando o tempo definido passa, então ele desaparece sozinho.

- Dado que o aviso desapareceu, então o jogo continua normalmente, sem exigir clique.

#### Pontos em aberto

- Definir a duração do aviso na tela e a regra de convivência com a notícia da US05.

#### Definition of Done específica

- Texto educativo de cada dilema revisado tecnicamente, com checagem dos conceitos de IA citados.

- Teste de sobreposição visual com a notícia da US05.

### US07: Ser promovido e mudar de escritório

#### Descrição

Como jogador, quero ser promovido quando cumprir as condições da empresa e passar a trabalhar em um novo escritório, para perceber que estou crescendo na carreira.

#### Critérios de aceitação

- Dado que os quatro medidores estão entre 30 e 70, o jogador resolveu pelo menos 8 cartas na fase, não há auditoria em andamento e a partida não terminou, quando as regras são avaliadas, então o jogador é promovido e uma tela anuncia o novo cargo.

- Dado que a promoção foi exibida, quando o jogador confirma, então o escritório do novo cargo é carregado, seguindo a ordem Estagiário, Júnior, Pleno, Sênior e Tech Líder.

- Dado que o jogador mudou de escritório, então os medidores e o contador de dias continuam de onde estavam.

- Dado que o jogador está em qualquer escritório, então o cargo atual aparece no HUD.

- Dado que o jogador está no escritório, então um quadro de promoção mostra uma lâmpada para cada condição, acesa quando a condição é verdadeira.

#### Pontos em aberto

- Definir o que acontece quando o jogador conclui a fase de Tech Líder, a última da carreira.

#### Definition of Done específica

- Arquitetura de fases documentada no repositório do projeto.

- Troca de escritório implementada de forma que novas fases possam ser adicionadas sem reescrever a navegação.

- Escopo do MVP limitado ao escritório do Estagiário; as demais fases ficam previstas para a Unidade 2.

- Promoção avaliada depois do fim de jogo, conforme a ordem de avaliação do relatório de LMC, garantindo que não é possível ser promovido e perder na mesma jogada.

### US08: Tela de fim de jogo

#### Descrição

Como jogador, quero ver uma tela de encerramento explicativa quando algum medidor chegar a 0 ou a 100, para entender por que minha trajetória na empresa terminou.

#### Critérios de aceitação

- Dado que um medidor chega a 0, quando o estado é verificado, então a tela de fim de jogo exibe a narrativa daquele medidor no mínimo.

- Dado que um medidor chega a 100, quando o estado é verificado, então a tela de fim de jogo exibe a narrativa daquele medidor no máximo.

- Dado que a tela de fim de jogo é exibida, então o total de dias sobrevividos aparece na tela.

- Dado que a tela de fim de jogo está visível, então o jogador tem acesso às opções de reiniciar (US09) e compartilhar o resultado (US10).

#### Pontos em aberto

- Definir a regra de prioridade quando mais de um medidor atinge o limite na mesma decisão.

#### Definition of Done específica

- 8 narrativas de fim de jogo (4 medidores em 2 extremos) seguindo os textos definidos no relatório de LMC.

- Regra de prioridade para limites simultâneos definida, implementada e coberta por teste.

### US09: Reiniciar a partida

#### Descrição

Como jogador, quero reiniciar o jogo com um clique depois de perder, para tentar uma nova estratégia de decisões.

#### Critérios de aceitação

- Dado que a tela de fim de jogo está visível, quando o jogador clica em "Reiniciar", então todos os medidores voltam ao valor inicial definido na US01.

- Dado que o jogo foi reiniciado, então o contador volta a exibir "DIA 1".

- Dado que o jogo foi reiniciado, então o baralho de dilemas é embaralhado novamente.

- Dado que a tela de fim de jogo está visível, quando o jogador clica em "Voltar ao menu", então a tela inicial é exibida.

#### Pontos em aberto

- Confirmar se, após o reinício, o primeiro card aparece imediatamente ou apenas na primeira interação com um NPC, como define a US01.

#### Definition of Done específica

- Reinício usa a mesma função de inicialização da US01, sem duplicar lógica.

- Teste garantindo que reiniciar após qualquer tipo de derrota restabelece o estado inicial.

- Reinício também limpa a auditoria em andamento, a contagem de cartas da fase e o uso da máquina de café.

### US10: Compartilhar o resultado

#### Descrição

Como jogador, quero copiar meu resultado ao final da partida, com os dias sobrevividos e o motivo da queda, para mostrar aos amigos ou comparar desempenhos.

#### Critérios de aceitação

- Dado que a tela de fim de jogo está visível, quando o jogador clica em "Compartilhar resultado", então um texto com os dias sobrevividos e o motivo da queda é copiado para a área de transferência.

- Dado que o texto foi copiado, então uma confirmação visual rápida é exibida, como "Copiado!".

- Dado que a cópia falha, quando o jogador clica no botão, então uma mensagem de erro amigável é exibida sem interromper o jogo.

#### Pontos em aberto

- Definir o formato exato do texto compartilhado.

#### Definition of Done específica

- Caso de falha na cópia testado.

- Texto testado com diferentes combinações de dias sobrevividos e motivos de queda (US08).

### US11: Passar por uma auditoria e sair dela

#### Descrição

Como jogador, quero que a empresa entre em auditoria quando a privacidade e o viés estiverem em risco ao mesmo tempo, para sentir o peso das falhas éticas e precisar corrigi-las.

#### Critérios de aceitação

- Dado que a Privacidade está abaixo de 30, o Viés está acima de 70 e não há auditoria em andamento, quando as regras são avaliadas, então uma auditoria é disparada e um aviso é exibido.

- Dado que há uma auditoria em andamento, quando o jogador resolve um card, então a Confiança cai, somando-se ao efeito do próprio card.

- Dado que há uma auditoria em andamento, então nenhuma outra auditoria é disparada e o jogador não pode ser promovido.

- Dado que a Privacidade volta a 30 ou mais e o Viés volta a 70 ou menos, então a auditoria termina; recuperar apenas um dos dois medidores não é suficiente.

#### Pontos em aberto

- Definir quanto de Confiança cada card custa durante a auditoria.

#### Definition of Done específica

- Regras de disparo e de saída testadas contra as tabelas-verdade do relatório de LMC, incluindo a equivalência de De Morgan.

- Testes dos invariantes: uma auditoria não é disparada sobre outra e não há promoção durante a auditoria.

### US12: Conversar com colegas no escritório

#### Descrição

Como jogador, quero interagir com os colegas enquanto ando pelo escritório, para descobrir histórias e pistas sobre os dilemas que vou enfrentar.

#### Critérios de aceitação

- Dado que o personagem está perto de um colega de conversa, quando o jogador aperta a tecla de interação, então um balão de fala é exibido.

- Dado que existem vários colegas no escritório, então cada um tem falas próprias.

- Dado que o dia ou os medidores mudaram, então as falas dos colegas podem mudar de acordo com a situação.

- Dado que o jogador fecha o balão, então o controle do personagem volta imediatamente.

#### Pontos em aberto

- Definir quais colegas são de conversa e quais acionam dilemas (US01 e US02).

- Definir se conversar consome algum dia.

#### Definition of Done específica

- Banco de falas por colega cadastrado e revisado.

- Teste confirmando que conversar não altera nenhum medidor.

### US13: Resolver o enigma da máquina de café

#### Descrição

Como jogador, quero resolver um circuito de portas lógicas na máquina de café, para ganhar o Café forte, que aproxima os medidores do equilíbrio.

#### Critérios de aceitação

- Dado que o personagem está perto da máquina de café, quando o jogador interage, então um circuito com interruptores, fios e portas lógicas ligados a uma lâmpada é exibido.

- Dado que o jogador liga os interruptores de forma que a lâmpada acenda, então a máquina é consertada e o Café forte é aplicado na hora.

- Dado que o Café forte é aplicado, então cada medidor se aproxima 10 pontos de 50, sem ultrapassar esse valor.

- Dado que o jogador erra, então perde uma das três tentativas da fase, sem outra penalidade.

- Dado que a máquina já foi consertada na fase atual ou as três tentativas acabaram, então ela fica indisponível até a próxima fase.

#### Pontos em aberto

- Definir o que conta como uma tentativa: cada vez que o jogador confirma o circuito ou cada interruptor acionado.

- Definir se o Café forte pode ajudar a encerrar uma auditoria, já que aproxima Privacidade e Viés da faixa segura.

#### Definition of Done específica

- Validação do circuito implementada como regra pura e coberta por testes, em conjunto com a frente de Lógica Matemática.

- Um circuito por fase, com dificuldade crescente: AND e NOT no Estagiário, OR no Júnior, XOR no Pleno, NAND no Sênior e apenas NAND no Tech Líder.

### US14: Desbloquear finais diferentes

#### Descrição

Como jogador, quero descobrir que cada forma de perder leva a um final diferente, para ter vontade de jogar de novo e conhecer todos eles.

#### Critérios de aceitação

- Dado que a partida termina (US08), quando o final é novo, então ele é registrado como desbloqueado e o jogo exibe "Novo final desbloqueado!".

- Dado que cada medidor tem dois limites, então o jogo possui até 8 finais diferentes.

- Dado que o jogador abre a galeria de finais no menu principal, então os finais já vistos aparecem com nome e os demais aparecem como "???".

#### Pontos em aberto

- Definir se os finais bloqueados exibem alguma dica de como alcançá-los.

#### Definition of Done específica

- Galeria persistida entre sessões, em integração com a US15.

- Teste confirmando que um final repetido não é registrado duas vezes.

### US15: Continuar de onde parei

#### Descrição

Como jogador, quero que meu progresso seja salvo automaticamente, para fechar o jogo e continuar depois sem perder o que já joguei.

#### Critérios de aceitação

- Dado que existe um jogo salvo, quando o jogador abre a tela inicial, então a opção "Continuar" fica habilitada.

- Dado que o jogador escolhe "Continuar", então cargo, medidores, dia atual, cartas resolvidas na fase, auditoria em andamento, uso da máquina de café e finais desbloqueados são carregados corretamente.

- Dado que não existe jogo salvo, então a opção "Continuar" aparece desabilitada.

- Dado que o jogador fecha e abre o jogo novamente, então os dados salvos permanecem sem corrupção ou perda.

#### Pontos em aberto

- Definir em que momento o salvamento acontece: a cada decisão, a cada dia ou ao fechar o jogo.

- Definir se escolher "Jogar" substitui o jogo salvo anterior.

#### Definition of Done específica

- Formato do arquivo de save documentado.

- Teste de leitura de um arquivo de save corrompido, com tratamento sem travar o jogo.

## Definition of Done comum a todas as histórias

Além dos itens específicos de cada história, toda história só é considerada concluída quando:

- O código foi revisado e aprovado por pelo menos uma outra pessoa da equipe.

- Todos os critérios de aceitação foram validados manualmente ou por teste automatizado.

- Não há bugs bloqueantes ou críticos conhecidos em aberto.

- O jogo compila e roda sem erros após a integração da história.

- Não há regressão em histórias relacionadas ou dependentes.

- O merge foi feito na branch principal sem conflitos pendentes.