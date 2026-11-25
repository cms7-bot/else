#!/usr/bin/env python3
"""Gera as telas de promocao de cada cargo a partir de HUD/Promocao.png
(Estagiario -> Junior): troca so os nomes dos dois crachas.

Uso (na raiz do projeto):  python3 Ferramentas/gerar_promocoes.py
Saida: HUD/Promocao_2.png (Junior->Pleno), _3 (Pleno->Senior), _4 (Senior->Tech Lider)
"""
import os
from PIL import Image, ImageDraw, ImageFont

RAIZ = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
FONTE = os.path.join(RAIZ, "cenario", "maquina_cafe", "comum", "VT323-Regular.ttf")
CARGOS = ["Estagiário", "Júnior", "Pleno", "Sênior", "Tech Líder"]

# (x0, y0, x1, y1, cor do fundo, cor do texto, tamanho da fonte, centro x)
ANTES = (136, 224, 226, 240, (242, 242, 242), (105, 105, 105), 19, 180)
DEPOIS = (396, 252, 514, 272, (244, 242, 236), (63, 111, 181), 26, 455)


def escrever(d, caixa, nome):
    x0, y0, x1, y1, bg, cor, tam, cx = caixa
    d.rectangle((x0, y0, x1, y1), fill=bg)
    f = ImageFont.truetype(FONTE, tam)
    while f.getlength(nome) > (x1 - x0) and tam > 8:
        tam -= 1
        f = ImageFont.truetype(FONTE, tam)
    d.text((cx - f.getlength(nome) / 2, (y0 + y1) / 2), nome, font=f, fill=cor, anchor="lm")


for i in (1, 2, 3):          # i = cargo atual (antes da promocao)
    im = Image.open(os.path.join(RAIZ, "HUD", "Promocao.png")).convert("RGBA")
    d = ImageDraw.Draw(im)
    d.fontmode = "1"         # sem suavizacao: combina com o pixel art
    escrever(d, ANTES, CARGOS[i])
    escrever(d, DEPOIS, CARGOS[i + 1])
    im.save(os.path.join(RAIZ, "HUD", f"Promocao_{i + 1}.png"))
    print("ok", i + 1)
