#!/usr/bin/env python3
"""Regera as cartas (HUD/cartasEstagiario) em alta resolucao (3x) com acentos.

Uso (na raiz do projeto, com as imagens ORIGINAIS 300x420 em --origem):
    python3 Ferramentas/gerar_cartas.py --origem <pasta_com_cartas_originais> --saida HUD/cartasEstagiario

Mantem a moldura, a ilustracao e as setas de cada carta (ampliadas sem
suavizar, para preservar o pixel art) e redesenha titulo, descricao e opcoes
com a fonte VT323 (pixel art), em 3x. As variantes A/D sao inclinadas -/+15 graus.
"""
import argparse, os
from PIL import Image, ImageDraw, ImageFont
import numpy as np

K = 3                          # fator de ampliacao
W, H = 300, 420                # tamanho original da carta
BG = (179, 185, 209)
PLACA = (87, 91, 105)
MOLDURA = (51, 57, 65)
TEXTO = (24, 26, 36)
BRANCO = (245, 245, 245)
FONTE_PIXEL = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
                           "cenario", "maquina_cafe", "comum", "VT323-Regular.ttf")
ANGULO = 15

# (arquivo, titulo, descricao, opcao A (esquerda), opcao D (direita))
CARTAS = [
 ("Triagem", "Triagem de currículos",
  "Seu chefe pede pra usar um modelo de triagem de currículos que já existe no sistema. Você percebe que ele filtra quase só candidatos de universidades caras.",
  "Usar assim mesmo, é rápido", "Sinalizar o problema antes de rodar"),
 ("Chatbot", "Chatbot inventou um dado",
  "O chatbot de suporte gerou uma resposta técnica para um cliente, mas o dado que ele citou não existe em nenhuma documentação.",
  "Deixar passar, o cliente não vai notar", "Corrigir e avisar o cliente do erro"),
 ("Historico 2", "Histórico de navegação",
  "A empresa coleta o histórico de navegação dos usuários para melhorar as recomendações. Porém, ninguém autorizou explicitamente o uso desses dados.",
  "Usar os dados, todo mundo faz", "Pedir consentimento antes"),
 ("Facial", "Reconhecimento facial",
  "O sistema de reconhecimento facial da empresa erra muito mais ao identificar pessoas de pele escura. Alguém sugere “ajustar o threshold” apenas para esconder o problema nas métricas do relatório.",
  "Ajustar o threshold como pedido", "Reportar o problema real"),
 ("Estatistica", "Estatística suspeita",
  "Um relatório automático de vendas cita uma estatística que parece forte demais pra ser real. Você está sob pressão pra entregar o relatório hoje.",
  "Entregar assim, sem checar", "Investigar a fonte antes de enviar"),
 ("Marketing 2", "Dados do marketing",
  "A equipe de marketing quer usar dados dos usuários, como nome, e-mail e comportamento, para treinar um novo modelo de IA, sem anonimizar essas informações.",
  "Liberar os dados como pedido", "Anonimizar antes de liberar"),
 ("Precificacao", "Precificação por bairro",
  "A IA de precificação está cobrando preços mais altos de usuários de determinados bairros, com base no suposto “risco de inadimplência”.",
  "Manter, os números fecham", "Revisar o critério de precificação"),
 ("Numero inventado", "Número inventado",
  "Um colega vai apresentar pro cliente um número que a IA gerou, mas parece inventado. Ele já confia cegamente no sistema e não quer perder tempo checando.",
  "Deixar ele apresentar assim", "Insistir em checar juntos antes"),
 ("Automacao", "Automação de funcionários",
  "A diretoria pretende substituir parte da equipe de atendimento por uma IA. O sistema ainda comete erros frequentes, mas a mudança reduziria bastante os custos.",
  "Aprovar a substituição imediatamente", "Manter a equipe até a IA ser confiável"),
 ("Decisao", "Decisão automatizada",
  "Uma IA responsável por aprovar empréstimos rejeitou a solicitação de um cliente. Ele pede explicações, mas ninguém na equipe consegue entender como o sistema chegou a essa conclusão.",
  "Manter a decisão da IA sem questionar", "Solicitar uma revisão humana da decisão"),
 ("Imagem", "Imagem falsa",
  "A equipe de marketing utiliza uma imagem gerada por IA que simula uma pessoa real recomendando o produto. A campanha está pronta e tem grande potencial de alcance.",
  "Publicar a campanha sem revelar a manipulação", "Informar que a imagem foi gerada por IA"),
 ("Treinamento", "Treinamento sem limite",
  "Um novo modelo de IA precisa de mais dados para melhorar. A equipe sugere coletar automaticamente todas as conversas dos usuários, inclusive as que contêm informações pessoais.",
  "Coletar todas as conversas disponíveis", "Limitar a coleta aos dados autorizados"),
 ("Trabalho", "O trabalho perfeito",
  "Você pede para uma IA fazer um trabalho acadêmico. O resultado está muito bem escrito, organizado e parece conter todas as informações necessárias. O prazo está acabando e você ainda não leu o material por completo.",
  "Entregar o trabalho como a IA produziu", "Revisar o conteúdo e conferir as informações"),
 ("Email", "O e-mail pronto",
  "Você precisa responder um e-mail importante de um cliente. Para ganhar tempo, pede que uma IA escreva a resposta. O texto fica ótimo, mas a IA promete um prazo de entrega que você não tem certeza se a empresa consegue cumprir.",
  "Enviar, o texto está bem escrito", "Conferir as informações antes de enviar"),
 ("Codigo", "O código misterioso",
  "Você utiliza uma IA para resolver um problema no código de um projeto da Else. Ela fornece uma solução que funciona nos testes iniciais, mas você não entende completamente o que aquele código executa.",
  "Implementar e seguir para a próxima tarefa", "Estudar o código e testar outros cenários"),
 ("Documento", "O documento interno",
  "Você precisa resumir um documento extenso da Else. Uma ferramenta externa de IA consegue fazer isso em segundos, mas o arquivo contém informações internas que não deveriam sair do ambiente da empresa.",
  "Enviar o documento para a ferramenta externa", "Resumir manualmente ou utilizar uma ferramenta autorizada"),
 ("Atendimento", "O atendimento automático",
  "Você utiliza uma IA para responder perguntas frequentes dos clientes da Else. O sistema responde rapidamente, mas começa a dar respostas genéricas para problemas que exigem uma análise individual.",
  "Manter a automação para reduzir o tempo de atendimento", "Intervir nos casos que exigem atenção individual"),
]


def fonte(peso, px):
    # VT323 (pixel, com acentos) em todas as cartas; o "peso" so muda o tamanho/contorno
    return ImageFont.truetype(FONTE_PIXEL, int(round(px * K)))


def quebrar(texto, f, largura):
    linhas, atual = [], ""
    for palavra in texto.split():
        teste = (atual + " " + palavra).strip()
        if f.getlength(teste) <= largura or not atual:
            atual = teste
        else:
            linhas.append(atual)
            atual = palavra
    if atual:
        linhas.append(atual)
    return linhas


def ajustar(texto, peso, tam_max, tam_min, largura, altura, entrelinha):
    """Maior tamanho de fonte (em px originais) que cabe na caixa."""
    tam = tam_max
    while tam >= tam_min:
        f = fonte(peso, tam)
        linhas = quebrar(texto, f, largura * K)
        if len(linhas) * tam * entrelinha * K <= altura * K:
            return f, linhas, tam
        tam -= 0.25
    f = fonte(peso, tam_min)
    return f, quebrar(texto, f, largura * K), tam_min


def texto_centrado(d, linhas, f, tam, entrelinha, cx, cy, cor, alinhar="centro", x0=None, negrito=False):
    alt = len(linhas) * tam * entrelinha * K
    y = cy * K - alt / 2
    for linha in linhas:
        x = cx * K - f.getlength(linha) / 2 if alinhar == "centro" else x0 * K
        # ancora "lm": centraliza cada linha na vertical (independe da fonte)
        d.text((x, y + tam * entrelinha * K / 2), linha, font=f, fill=cor, anchor="lm",
               stroke_width=K // 3 if negrito else 0, stroke_fill=cor)
        y += tam * entrelinha * K


def base_sem_texto(orig):
    """Ampliacao da carta original, sem o titulo e sem a descricao."""
    im = Image.open(orig).convert("RGBA")
    mascara_ilustracao = np.array(im.convert("RGB")).astype(int)
    up = im.resize((W * K, H * K), Image.NEAREST)
    d = ImageDraw.Draw(up)
    d.rectangle((22 * K, 30 * K, 278 * K, 90 * K), fill=PLACA)        # apaga titulo
    d.rectangle((8 * K, 248 * K, 292 * K, 376 * K), fill=BG)          # apaga descricao
    return up, mascara_ilustracao


def carta_situacao(c, orig):
    _, titulo, desc, _, _ = c
    up, mask = base_sem_texto(orig)
    d = ImageDraw.Draw(up)
    f, linhas, tam = ajustar(titulo, "SemiBold", 25, 15, 240, 60, 1.0)
    texto_centrado(d, linhas, f, tam, 1.0, W / 2, 60, BRANCO, negrito=True)
    f, linhas, tam = ajustar(desc, "Regular", 20, 14, 266, 124, 1.08)
    texto_centrado(d, linhas, f, tam, 1.08, W / 2, 313, TEXTO, alinhar="esq", x0=17)
    return up, mask


def carta_opcao(c, orig, opcao, lado):
    up, mask = carta_situacao(c, orig)
    # faixa escura sobre a parte de cima da ilustracao (x 15..284, y 106..186)
    arr = np.array(up).astype(float)
    ys, ye = 106 * K, 187 * K
    mascara_bg = (np.abs(np.array(Image.open(orig).convert("RGB").resize((W * K, H * K), Image.NEAREST)).astype(int)
                         - np.array(BG)).sum(2) > 8)
    faixa = np.zeros(arr.shape[:2], bool)
    faixa[ys:ye, 15 * K:285 * K] = True
    # cantos arredondados da base da faixa
    m = Image.new("L", (W * K, H * K), 0)
    ImageDraw.Draw(m).rounded_rectangle((15 * K, ys, 285 * K - 1, ye), radius=8 * K, fill=255)
    faixa = (np.array(m) > 127) & mascara_bg
    arr[faixa, :3] *= 0.22
    up = Image.fromarray(arr.astype(np.uint8), "RGBA")
    d = ImageDraw.Draw(up)
    f, linhas, tam = ajustar(opcao, "Bold", 23, 15, 240, 68, 1.0)
    texto_centrado(d, linhas, f, tam, 1.0, W / 2, 146, BRANCO, negrito=True)
    # inclina: A = anti-horario, D = horario
    ang = ANGULO if lado == "A" else -ANGULO
    rgb = up.convert("RGB").rotate(ang, Image.BICUBIC, expand=True, fillcolor=MOLDURA)
    alfa = up.getchannel("A").rotate(ang, Image.BICUBIC, expand=True, fillcolor=0)
    rot = rgb.convert("RGBA")
    rot.putalpha(alfa)
    tela = Image.new("RGBA", (399 * K, 484 * K), (0, 0, 0, 0))
    tela.paste(rot, ((tela.width - rot.width) // 2, (tela.height - rot.height) // 2), rot)
    return tela


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--origem", required=True)
    ap.add_argument("--saida", required=True)
    a = ap.parse_args()
    os.makedirs(a.saida, exist_ok=True)
    for c in CARTAS:
        nome = c[0]
        s_orig = os.path.join(a.origem, f"Carta {nome} _S_.png")
        carta_situacao(c, s_orig)[0].save(os.path.join(a.saida, f"Carta {nome} _S_.png"), optimize=True)
        carta_opcao(c, s_orig, c[3], "A").save(os.path.join(a.saida, f"Carta {nome} _A_.png"), optimize=True)
        carta_opcao(c, s_orig, c[4], "D").save(os.path.join(a.saida, f"Carta {nome} _D_.png"), optimize=True)
        print("ok", nome)


if __name__ == "__main__":
    main()
