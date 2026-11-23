#!/usr/bin/env python3
"""Gera as cartas dos niveis Junior, Pleno, Senior e Tech Lider (3x, com acentos).

As ilustracoes sao PROVISORIAS (cor + icone do pilar), ate o Arthur entregar
as artes definitivas de 270x140. Para trocar uma ilustracao basta colocar a
arte no molde e regerar, ou editar a carta _S_/_A_/_D_ direto.

Uso (na raiz do projeto):
    python3 Ferramentas/gerar_cartas_niveis.py
"""
import os, sys, tempfile
from PIL import Image, ImageDraw

AQUI = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, AQUI)
import gerar_cartas as g
from cartas_niveis_dados import NIVEIS

RAIZ = os.path.dirname(AQUI)
MOLDE = os.path.join(AQUI, "molde_carta.png")

# area da ilustracao no molde 300x420
IL = (15, 106, 285, 247)

CORES = {            # fundo, faixa de baixo, icone
    "V": ((120, 78, 178), (86, 54, 134)),
    "A": ((44, 152, 152), (30, 108, 112)),
    "P": ((58, 100, 170), (38, 68, 124)),
    "D": ((214, 136, 58), (160, 94, 36)),
    "C": ((70, 156, 98), (46, 112, 68)),
}
CLARO = (250, 245, 235)

ICONES = {
 "A": ["#####.", "#...##", ".....#", "....#.", "...#..", "...#..", "......", "...#.."],
 "P": ["..###..", ".#...#.", ".#...#.", "#######", "#######", "###.###", "###.###", "#######"],
 "D": ["...#...", "...#...", "#######", "#.....#", "#.#.#.#", "#.#.#.#", "#.....#", "#.###.#", "#######"],
 "C": ["#######", "#.....#", "#.....#", "#.....#", ".#...#.", ".#...#.", "..#.#..", "...#..."],
}


def icone(d, pilar, cx, cy, px):
    if pilar == "V":     # balanca desequilibrada
        d.rectangle((cx - px // 2, cy - 5 * px, cx + px // 2 - 1, cy + 3 * px), fill=CLARO)
        d.rectangle((cx - 4 * px, cy + 3 * px, cx + 4 * px, cy + 4 * px - 1), fill=CLARO)
        d.polygon([(cx - 5 * px, cy - 3 * px), (cx + 5 * px, cy - 5 * px),
                   (cx + 5 * px, cy - 4 * px), (cx - 5 * px, cy - 2 * px)], fill=CLARO)
        d.rectangle((cx - 6 * px, cy - px, cx - 3 * px, cy), fill=CLARO)
        d.rectangle((cx + 3 * px, cy - 3 * px, cx + 6 * px, cy - 2 * px), fill=CLARO)
        return
    m = ICONES[pilar]
    w, h = len(m[0]), len(m)
    x0, y0 = cx - w * px // 2, cy - h * px // 2
    for j, linha in enumerate(m):
        for i, c in enumerate(linha):
            if c == "#":
                d.rectangle((x0 + i * px, y0 + j * px, x0 + (i + 1) * px - 1, y0 + (j + 1) * px - 1), fill=CLARO)


def molde_com_ilustracao(pilar, destino):
    im = Image.open(MOLDE).convert("RGBA")
    d = ImageDraw.Draw(im)
    fundo, faixa = CORES[pilar]
    d.rounded_rectangle(IL, radius=6, fill=fundo)
    d.rectangle((IL[0], IL[3] - 22, IL[2] - 1, IL[3] - 4), fill=faixa)
    icone(d, pilar, (IL[0] + IL[2]) // 2, (IL[1] + IL[3]) // 2 - 6, 9)
    im.save(destino)


def main():
    tmp = tempfile.mkdtemp()
    for nome_nivel, (pasta, _cargo, cartas) in NIVEIS.items():
        saida = os.path.join(RAIZ, "HUD", pasta)
        os.makedirs(saida, exist_ok=True)
        for (arq, titulo, texto, opA, opD, pilar, *_resto) in cartas:
            molde = os.path.join(tmp, "molde.png")
            molde_com_ilustracao(pilar, molde)
            c = (arq, titulo, texto, opA, opD)
            g.carta_situacao(c, molde)[0].save(os.path.join(saida, f"Carta {arq} _S_.png"), optimize=True)
            g.carta_opcao(c, molde, opA, "A").save(os.path.join(saida, f"Carta {arq} _A_.png"), optimize=True)
            g.carta_opcao(c, molde, opD, "D").save(os.path.join(saida, f"Carta {arq} _D_.png"), optimize=True)
            print("ok", pasta, arq)


if __name__ == "__main__":
    main()
