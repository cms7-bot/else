# Ping pong do cantinho do descanso (Júnior)

Dois NPCs jogando sem parar. É só visual: não tem colisão com a bola e não mexe nos medidores.
O de cima (cabelo preto, blusa verde) dá passinhos pro lado. O de baixo (loiro, blusa azul) não sai do lugar.

## Arquivos e escala
| arquivo | tamanho | como desenhar |
|---|---|---|
| mesa_pingpong.png | 260x410 | escala 0.5 (igual às outras peças do cenário) |
| npc_pingpong_frente_corpo_parado.png | 66x96 | escala 1 (tamanho real, igual ao jogador) |
| npc_pingpong_frente_corpo_sem_braco.png | 66x96 | escala 1, junto com um dos braços abaixo |
| npc_pingpong_frente_braco_recua / bate / acompanha.png | 66x96 | escala 1, na MESMA posição do corpo |
| npc_pingpong_costas_parado / recua / bate / acompanha.png | 66x96 | escala 1 (corpo e braço juntos) |
| raquete_cabo_cima.png / raquete_cabo_baixo.png | 13x19 | escala 1 |
| bolinha.png | 6x6 | escala 1 (ou DrawCircle) |

Os .ase têm 1 camada (DESENHO), em pixel de arte. No NPC, 1 px de arte = 3 px de tela.

## Posições na tela
- Mesa: **300, 344**
- Loiro (de costas, fixo): **315, 499**
- De cima: x = **344 + xa**, y = **269**. O `xa` vai passando por **-14, 6, -6, 12** e depois repete.

Mão (onde vai a raquete), somada ao canto do NPC:

| pose | mão do de cima | mão do loiro |
|---|---|---|
| parado | 16, 64 | 49, 64 |
| recua | 6, 42 | 60, 48 |
| bate | 21, 69 | 54, 30 |
| acompanha | 30, 63 | 60, 39 |

- Raquete do de cima: `raquete_cabo_cima` em (cantoX + maoX - 6, cantoY + maoY - 2)
- Raquete do loiro: `raquete_cabo_baixo` em (cantoX + maoX - 6, cantoY + maoY - 16)

Pontos onde a bola encosta na raquete:
- Em cima: **(365 + xa, 346)**
- Embaixo: **(369, 521)**

## Ordem de desenho (de trás pra frente)
1. Corpo do de cima. Se a pose for parado, usa `corpo_parado`; senão, `corpo_sem_braco`.
2. Mesa
3. Braço do de cima (só quando não está parado), por cima da borda da mesa
4. Raquete do de cima
5. Sombra da bolinha e depois a bolinha
6. Loiro, com a pose dele
7. Raquete do loiro

## Tempo
- Uma travessia da bola dura **16 quadros de 60 ms** (cerca de 1 s).
- Contando `k` de 0 a 15 em cada travessia e `r` como o número da travessia:
  - O de cima rebate quando `r` é par; o loiro, quando `r` é ímpar.
  - Quem rebate: `k = 0` é **bate**, `k = 1..3` é **acompanha**, depois volta pra **parado**.
  - Quem vai receber: `k = 12..15` é **recua**; antes disso fica **parado**.
  - O de cima só anda nas travessias ímpares, quando a bola está indo pro loiro. Ele desliza do `xa` atual pro próximo durante os primeiros 60% da travessia.

## Bolinha
- `s = k / 16` (de 0 a 1). A posição vai em linha reta de um ponto de contato ao outro.
- A sombra fica sempre nessa posição, colada no chão.
- A bolinha sobe `alto` pixels acima da sombra:
  - se `s < 0.7`: `alto = 12 * sin(PI * s / 0.7)`. Ela voa e quica no lado de quem recebe.
  - senão: `alto = 7 * sin(PI * (s - 0.7) / 0.6)`. Ela sobe até a raquete.

Prévia animada: `GIF - Junior ping pong e gato.gif` e `ZOOM - Junior ping pong.gif`, na pasta do rascunho.
Gerador: `_gerador/junior_pingpong4.py` (cena) e `_gerador/exportar_pingpong.py` (peças).
