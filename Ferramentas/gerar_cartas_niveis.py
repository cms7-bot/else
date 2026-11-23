#!/usr/bin/env python3
"""Gera as cartas dos niveis Junior, Pleno, Senior e Tech Lider (3x, com acentos).

Usa a arte do Arthur (ilustracao, moldura e setas) das cartas S originais
300x420 em Ferramentas/originais_niveis/<pasta>/ e redesenha titulo,
descricao e escolhas com a fonte Inter em 3x, igual as cartas do Estagiario.
Os textos e efeitos ficam em cartas_niveis_dados.py.

Uso (na raiz do projeto):
    python3 Ferramentas/gerar_cartas_niveis.py
"""
import os, sys

AQUI = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, AQUI)
import gerar_cartas as g
from cartas_niveis_dados import NIVEIS

RAIZ = os.path.dirname(AQUI)


def main():
    for pasta, _cargo, cartas in NIVEIS.values():
        saida = os.path.join(RAIZ, "HUD", pasta)
        os.makedirs(saida, exist_ok=True)
        for (arq, titulo, texto, opA, opD, *_resto) in cartas:
            orig = os.path.join(AQUI, "originais_niveis", pasta, f"Carta {arq} _S_.png")
            c = (arq, titulo, texto, opA, opD)
            g.carta_situacao(c, orig)[0].save(os.path.join(saida, f"Carta {arq} _S_.png"), optimize=True)
            g.carta_opcao(c, orig, opA, "A").save(os.path.join(saida, f"Carta {arq} _A_.png"), optimize=True)
            g.carta_opcao(c, orig, opD, "D").save(os.path.join(saida, f"Carta {arq} _D_.png"), optimize=True)
            print("ok", pasta, arq)


if __name__ == "__main__":
    main()
