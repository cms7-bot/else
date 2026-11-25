#include "raylib.h"
#include "rlgl.h"
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "Regras.h"

int telaInicio(Texture2D botaoJogar, RenderTexture2D telaVirtual, float escala, float offsetX, float offsetY, Music trilhaSonora1);
int telaAviso(RenderTexture2D telaVirtual, float escala, float offsetX, float offsetY);
void desbloquearFinal(int numero);

void iniciarCartas(float *cadeado, float *vies, float *privacidade, float *lucro);
void atualizarCartas(void);
void desenharCartas(int larguraResolucao, int alturaResolucao);
void desenharCartasNaJanela(float escala, float offsetX, float offsetY);
void cancelarAnimacoesCartas(void);
void definirEstacaoCartas(int estacao);   // mesa/impressora onde o jogador esta
bool estacaoAtualLiberada(void);          // false = ja pegou cartas demais aqui

// animacao padrao de TODOS os botoes (Cartas.c)
float escalaAnimadaBotao(const char *nome, bool hover);
Rectangle retanguloAnimadoBotao(const char *nome, Rectangle base, bool hover);
void definirTransformacaoTelaCartas(float escala, float offsetX, float offsetY);

// FONTE DA INTERFACE (Cartas.c)
#define FONTE_TEXTO    0   // VT323: frases e textos longos
#define FONTE_DESTAQUE 1   // Press Start 2P: numeros e rotulos curtos
float medirTextoUI(const char *texto, float tamanho, int fonte);
void desenharTextoUI(const char *texto, float x, float y, float tamanho, Color cor, int fonte);
void agendarTextoUI(const char *texto, float x, float y, float tamanho, Color cor, bool centralizado, int fonte);
void desenharTextosNaJanela(float escala, float offsetX, float offsetY);
void limparTextosUI(void);
const char *nomeCargoExibicao(void);
bool cartasEstaoAbertas(void);
void definirOrigemCartas(int origem);
void descarregarCartas(void);
   void InicializarBancoPerguntas(void);   // protótipos no topo
   void CarregarTexturasCartas(void);


const char *obterMensagemFinal(void);
int obterDiasSobrevividos(void);
const char *nomeCargo(void);

// SOM GLOBAL: 1 = mudo (compartilhado com o menu inicial, em TelaInicio.c)
int somMutadoGlobal = 0;
int obterCargo(void);

// MAQUINA DE CAFE (MaquinaCafe.c)
void iniciarMaquinaCafe(void);
void descarregarMaquinaCafe(void);
void reiniciarMaquinaCafe(void);
bool maquinaCafeAberta(void);
void fecharMaquinaCafe(void);
void interagirBalcaoCafe(int larguraResolucao);
void desenharAvisoBalcaoCafe(void);
void atualizarDesenharMaquinaCafe(float escala, float offsetX, float offsetY);
void desenharTextoQuebrado(const char *texto, int x, int y, int larguraMaxima, int tamanho, Color cor);




// ==========================================================
// OBJETOS DO ESCRITÓRIO
// ==========================================================
typedef struct ObjetoCenario
{
    Texture2D textura;
    float x, y;
    Rectangle colisao;
    bool interativo;
    int origemCartas;
} ObjetoCenario;


static ObjetoCenario criarObjeto(
    const char *arquivo,
    float x,
    float y,
    Rectangle colisao,
    bool interativo,
    int origemCartas
)
{
    ObjetoCenario objeto;

    objeto.textura = LoadTexture(arquivo);

    objeto.textura.width *= 0.5;
    objeto.textura.height *= 0.5;

    objeto.x = x;
    objeto.y = y;

    objeto.colisao = (Rectangle){
        x + colisao.x,
        y + colisao.y,
        colisao.width,
        colisao.height
    };

    objeto.interativo = interativo;
    objeto.origemCartas = origemCartas;

    return objeto;
}


typedef struct AnimacaoMesa
{
    Texture2D quadros[3];
    int total;
} AnimacaoMesa;

typedef struct MesasNivel
{
    Texture2D frente;
    Texture2D costa;
    AnimacaoMesa frenteNpc1;
    AnimacaoMesa frenteNpc2;
    AnimacaoMesa costaNpc1;
    AnimacaoMesa costaNpc2;
} MesasNivel;

static MesasNivel mesasNivel[5];


static Texture2D carregarMetade(const char *arquivo)
{
    Texture2D textura = LoadTexture(arquivo);

    textura.width *= 0.5;
    textura.height *= 0.5;

    return textura;
}


static AnimacaoMesa carregarAnimacaoMesa(
    const char *pasta,
    const char *nome,
    int primeiro,
    int total
)
{
    AnimacaoMesa animacao = {0};

    for (int k = 0; k < total; k++)
    {
        animacao.quadros[k] =
            carregarMetade(TextFormat("%s/%s%d.png", pasta, nome, primeiro + k));
    }

    animacao.total = total;

    return animacao;
}


static void carregarMesasNiveis(void)
{
    #define MESAS_JR "cenario/elementos_junior/Mesas/"
    mesasNivel[1].frente = carregarMetade(MESAS_JR "mesaJuniorFrente.png");
    mesasNivel[1].costa = carregarMetade(MESAS_JR "mesaJuniorCosta.png");
    mesasNivel[1].frenteNpc1 = carregarAnimacaoMesa(MESAS_JR "MesaJuniorFrenteComNpc", "MesaJuniorFrenteComNpc", 1, 3);
    mesasNivel[1].costaNpc1 = carregarAnimacaoMesa(MESAS_JR "MesaJuniorCostaComNpc", "mesaJuniorCostaComNpc", 1, 3);
    mesasNivel[1].costaNpc2 = carregarAnimacaoMesa(MESAS_JR "MesaJuniorCostaComNpc2", "mesaJuniorCostaComNpc", 2, 2);

    #define MESAS_PL "cenario/elementos_pleno/Mesas/"
    mesasNivel[2].frente = carregarMetade(MESAS_PL "MesaPlenoFrente.png");
    mesasNivel[2].costa = carregarMetade(MESAS_PL "MesaPlenoCosta.png");
    mesasNivel[2].frenteNpc1 = carregarAnimacaoMesa(MESAS_PL "MesaPlenoFrenteComNpc", "MesaPlenoFrenteComNpc", 1, 3);
    mesasNivel[2].frenteNpc2 = carregarAnimacaoMesa(MESAS_PL "MesaPlenoFrenteComNpc2", "MesaPlenoFrenteComNpc", 2, 3);
    mesasNivel[2].costaNpc1 = carregarAnimacaoMesa(MESAS_PL "MesaPlenoCostaComNpc", "MesaPlenoCostaComNpc", 1, 2);
    mesasNivel[2].costaNpc2 = carregarAnimacaoMesa(MESAS_PL "MesaPlenoCostaComNpc2", "MesaPlenoCostaComNpc", 2, 2);

    #define MESAS_SR "cenario/elementos_senior/Mesas/"
    mesasNivel[3].frente = carregarMetade(MESAS_SR "MesaSeniorFrente.png");
    mesasNivel[3].costa = carregarMetade(MESAS_SR "MesaSeniorCosta.png");
    mesasNivel[3].frenteNpc1 = carregarAnimacaoMesa(MESAS_SR "MesaSeniorFrenteComNpc", "MesaSeniorFrenteComNpc", 1, 3);
    mesasNivel[3].frenteNpc2 = carregarAnimacaoMesa(MESAS_SR "MesaSeniorFrenteComNpc2", "MesaSeniorFrente", 1, 3);
    mesasNivel[3].costaNpc1 = carregarAnimacaoMesa(MESAS_SR "MesaSeniorCostaComNpc", "MesaSeniorCostaComNpc", 1, 3);
    mesasNivel[3].costaNpc2 = carregarAnimacaoMesa(MESAS_SR "MesaSeniorCostaComNpc2", "MesaSeniorCostaComNpc", 2, 2);

    #define MESAS_TL "cenario/elementos_techlead/Mesas/"
    mesasNivel[4].frente = carregarMetade(MESAS_TL "MesaFrenteTech.png");
    mesasNivel[4].costa = carregarMetade(MESAS_TL "MesaCostaTech.png");
    mesasNivel[4].frenteNpc1 = carregarAnimacaoMesa(MESAS_TL "MesaFrenteTechComNpc", "MesaFrenteTechComNpc", 1, 3);
    mesasNivel[4].frenteNpc2 = carregarAnimacaoMesa(MESAS_TL "MesaFrenteTechComNpc2", "MesaFrenteTechComNpc", 2, 3);
    mesasNivel[4].costaNpc1 = carregarAnimacaoMesa(MESAS_TL "MesaCostaTechComNpc", "MesaCostaTechComNpc", 1, 2);
    mesasNivel[4].costaNpc2 = carregarAnimacaoMesa(MESAS_TL "MesaCostaTechComNpc2", "MesaCostaTechComNpc", 2, 3);
}


static int quadroNpcFrente(int total, int passo)
{
    static const int sequencia[16] = {0,0,0,0,1,0,0,0,2,2,0,0,0,0,1,0};

    int quadro = sequencia[passo % 16];

    return quadro < total ? quadro : 0;
}


static int quadroNpcCosta(int total, int passo)
{
    static const int sequencia[4] = {0,1,0,2};

    if (total >= 3)
        return sequencia[passo % 4];

    return passo % 2;
}


static bool desenharMesaNivel(int cargo, int i, float x, float y, int passo)
{
    if (cargo < 1 || cargo > 4 || i < 6 || i > 13)
        return false;

    MesasNivel *mesas = &mesasNivel[cargo];
    bool deFrente = i <= 9;
    Texture2D textura = deFrente ? mesas->frente : mesas->costa;

    if (i == 7 && mesas->frenteNpc1.total > 0)
        textura = mesas->frenteNpc1.quadros[quadroNpcFrente(mesas->frenteNpc1.total, passo)];
    else if (i == 9 && mesas->frenteNpc2.total > 0)
        textura = mesas->frenteNpc2.quadros[quadroNpcFrente(mesas->frenteNpc2.total, passo + 8)];
    else if (i == 13 && mesas->costaNpc1.total > 0)
        textura = mesas->costaNpc1.quadros[quadroNpcCosta(mesas->costaNpc1.total, passo)];
    else if (i == 12 && mesas->costaNpc2.total > 0)
        textura = mesas->costaNpc2.quadros[quadroNpcCosta(mesas->costaNpc2.total, passo + 1)];

    if (textura.id == 0)
        return false;

    Vector2 posicao = deFrente
        ? (Vector2){x + 2.5f, y - 12.5f}
        : (Vector2){x + 2.5f, y};

    DrawTextureV(textura, posicao, WHITE);

    return true;
}


#define MAX_QUADROS_PECA 32
#define MAX_PECAS_CANTINHO 20

enum
{
    PECA_COMUM,
    PECA_CHAO,
    PECA_CAFE,
    PECA_FREEZER,
    PECA_PINGPONG,
    PECA_SINUCA
};

typedef struct PecaCantinho
{
    Texture2D quadros[MAX_QUADROS_PECA];
    int total;
    float duracao;
    const int *sequencia;
    int tamanhoSequencia;
    float x, y;
    Rectangle colisoes[4];
    int quantidadeColisoes;
    Rectangle areaInteracao;
    float base;
    int tipo;
} PecaCantinho;

typedef struct Cantinho
{
    PecaCantinho pecas[MAX_PECAS_CANTINHO];
    int quantidade;
} Cantinho;

static Cantinho cantinhos[5];

static Texture2D freezerAberto;
static Texture2D baloesFreezer[4];
static float tempoFreezerAberto = 0.0f;
static int balaoFreezer = 0;

static Texture2D pingPongCorpoParado, pingPongCorpoSemBraco;
static Texture2D pingPongBracoCima[4];
static Texture2D pingPongCostas[4];
static Texture2D raqueteCima, raqueteBaixo, bolinhaPingPong;

static Texture2D npcSinuca, maoSinuca;

static const int sequenciaMesaAlmoco[4] = {0, 1, 0, 2};
static const int sequenciaNpcJogando[8] = {0, 0, 1, 1, 1, 0, 0, 0};


static Texture2D carregarInteira(const char *arquivo)
{
    return LoadTexture(arquivo);
}


static PecaCantinho *novaPeca(int cargo, int tipo, float x, float y)
{
    Cantinho *cantinho = &cantinhos[cargo];
    PecaCantinho *peca = &cantinho->pecas[cantinho->quantidade++];

    *peca = (PecaCantinho){0};
    peca->tipo = tipo;
    peca->x = x;
    peca->y = y;
    peca->duracao = 0.2f;

    return peca;
}


static PecaCantinho *pecaParada(int cargo, int tipo, const char *arquivo, float x, float y)
{
    PecaCantinho *peca = novaPeca(cargo, tipo, x, y);

    peca->quadros[0] = carregarMetade(arquivo);
    peca->total = 1;

    return peca;
}


static PecaCantinho *pecaAnimada(
    int cargo,
    int tipo,
    const char *formato,
    int primeiro,
    int total,
    float duracao,
    float x,
    float y
)
{
    PecaCantinho *peca = novaPeca(cargo, tipo, x, y);

    for (int k = 0; k < total; k++)
        peca->quadros[k] = carregarMetade(TextFormat(formato, primeiro + k));

    peca->total = total;
    peca->duracao = duracao;

    return peca;
}


static void colisao(PecaCantinho *peca, float x, float y, float largura, float altura)
{
    peca->colisoes[peca->quantidadeColisoes++] =
        (Rectangle){peca->x + x, peca->y + y, largura, altura};
}


static void finalizarPeca(PecaCantinho *peca)
{
    float altura = peca->total > 0 ? peca->quadros[0].height : 0;

    if (peca->base == 0)
    {
        if (peca->quantidadeColisoes > 0)
            peca->base = peca->colisoes[0].y + peca->colisoes[0].height;
        else
            peca->base = peca->y + altura;
    }

    if (peca->areaInteracao.width == 0 && peca->quantidadeColisoes > 0)
    {
        Rectangle c = peca->colisoes[0];
        peca->areaInteracao = (Rectangle){c.x - 30, c.y - 30, c.width + 60, c.height + 60};
    }
}


static void carregarCantinhos(void)
{
    PecaCantinho *p;

    #define JR "cenario/elementos_junior/"
    p = pecaParada(1, PECA_CHAO, JR "carpete_descanso.png", 40, 332);
    p = pecaParada(1, PECA_COMUM, JR "planta_espada.png", 14, 98);
    colisao(p, 0, 66, 45, 44);
    p = pecaParada(1, PECA_CAFE, JR "balcao_cafe.png", 62, 64);
    colisao(p, 0, 85, 250, 57);
    p = pecaParada(1, PECA_COMUM, JR "banqueta.png", 110, 176);
    colisao(p, 0, 36, 35, 24);
    p = pecaParada(1, PECA_COMUM, JR "banqueta.png", 165, 176);
    colisao(p, 0, 36, 35, 24);
    p = pecaParada(1, PECA_COMUM, JR "banqueta.png", 220, 176);
    colisao(p, 0, 36, 35, 24);
    p = pecaParada(1, PECA_FREEZER, JR "freezer_energetico.png", 312, 65);
    colisao(p, 0, 87, 65, 58);
    p = pecaParada(1, PECA_COMUM, JR "sofa_descanso.png", 26, 330);
    colisao(p, 0, 20, 80, 190);
    p = pecaParada(1, PECA_COMUM, JR "mesa_redonda_descanso.png", 122, 410);
    colisao(p, 0, 20, 55, 35);
    p = pecaParada(1, PECA_COMUM, JR "poltrona_descanso.png", 186, 362);
    colisao(p, 0, 25, 65, 40);
    p = pecaParada(1, PECA_COMUM, JR "poltrona_descanso.png", 186, 450);
    colisao(p, 0, 25, 65, 40);
    p = novaPeca(1, PECA_PINGPONG, 300, 344);
    p->quadros[0] = carregarMetade(JR "PingPong/mesa_pingpong.png");
    p->total = 1;
    colisao(p, 10, 20, 110, 180);   // mesa (essa vem primeiro: define a ordem de desenho)
    colisao(p, 42, -14, 68, 34);    // NPC de cima (vai e volta na horizontal)
    colisao(p, 28, 200, 42, 49);    // NPC de baixo (do fim da mesa ate os pes)

    freezerAberto = carregarMetade(JR "freezer_energetico_aberto.png");
    for (int k = 0; k < 4; k++)
        baloesFreezer[k] = carregarMetade(TextFormat(JR "BaloesFreezer/balao_freezer_%d.png", k + 1));

    pingPongCorpoParado = carregarInteira(JR "PingPong/npc_pingpong_frente_corpo_parado.png");
    pingPongCorpoSemBraco = carregarInteira(JR "PingPong/npc_pingpong_frente_corpo_sem_braco.png");
    pingPongBracoCima[1] = carregarInteira(JR "PingPong/npc_pingpong_frente_braco_recua.png");
    pingPongBracoCima[2] = carregarInteira(JR "PingPong/npc_pingpong_frente_braco_bate.png");
    pingPongBracoCima[3] = carregarInteira(JR "PingPong/npc_pingpong_frente_braco_acompanha.png");
    pingPongCostas[0] = carregarInteira(JR "PingPong/npc_pingpong_costas_parado.png");
    pingPongCostas[1] = carregarInteira(JR "PingPong/npc_pingpong_costas_recua.png");
    pingPongCostas[2] = carregarInteira(JR "PingPong/npc_pingpong_costas_bate.png");
    pingPongCostas[3] = carregarInteira(JR "PingPong/npc_pingpong_costas_acompanha.png");
    raqueteCima = carregarInteira(JR "PingPong/raquete_cabo_cima.png");
    raqueteBaixo = carregarInteira(JR "PingPong/raquete_cabo_baixo.png");
    bolinhaPingPong = carregarInteira(JR "PingPong/bolinha.png");

    #define PL "cenario/elementos_pleno/"
    p = pecaParada(2, PECA_CHAO, PL "carpete_pleno.png", 20, 288);
    p = pecaParada(2, PECA_COMUM, PL "costela_de_adao.png", 16, 101);
    colisao(p, 0, 63, 60, 42);
    p = pecaParada(2, PECA_CAFE, PL "balcao_cafe.png", 74, 76);
    colisao(p, 0, 78, 250, 52);
    p = pecaAnimada(2, PECA_COMUM, PL "Microondas/Microondas%d.png", 1, 3, 0.25f, 259, 105);   // sempre ligado, em loop
    p->base = 206;
    p = pecaAnimada(2, PECA_COMUM, PL "TvRack/tv_rack_pleno_%02d.png", 0, 28, 0.15f, 120, 252);
    colisao(p, 0, 60, 130, 40);
    p = pecaParada(2, PECA_COMUM, PL "sofa_L_pleno.png", 24, 360);
    colisao(p, 0, 142, 220, 85);
    colisao(p, 0, 10, 80, 132);
    p = novaPeca(2, PECA_COMUM, 66, 403);
    p->quadros[0] = carregarInteira(PL "npc_jogando_controle_baixo.png");
    p->quadros[1] = carregarInteira(PL "npc_jogando_controle_cima.png");
    p->total = 2;
    p->duracao = 0.18f;
    p->sequencia = sequenciaNpcJogando;
    p->tamanhoSequencia = 8;
    p->base = 587;
    colisao(p, 32, 63, 30, 32);   // pernas esticadas (passavam por cima do personagem)
    p = pecaParada(2, PECA_COMUM, PL "mesa_centro_pleno.png", 116, 422);
    colisao(p, 0, 5, 75, 40);

    #define SR "cenario/elementos_senior/"
    p = pecaParada(3, PECA_CHAO, SR "carpete_senior.png", 30, 356);
    p = pecaParada(3, PECA_CAFE, SR "maquina_cafe.png", 30, 56);   // cantinho do cafe: +10 px (desencosta da parede)
    colisao(p, 0, 90, 80, 60);
    p = pecaParada(3, PECA_COMUM, SR "cantinho_cha.png", 120, 96);
    colisao(p, 0, 66, 100, 44);
    p = pecaParada(3, PECA_COMUM, SR "bonsai.png", 250, 81);
    colisao(p, 0, 75, 75, 50);
    p = pecaAnimada(3, PECA_COMUM, SR "PoltronaColega/PoltronaColega%d.png", 1, 4, 0.4f, 332, 98);
    colisao(p, 0, 65, 85, 43);
    p = pecaParada(3, PECA_COMUM, SR "luminaria.png", 352, 51);
    colisao(p, 93, 93, 27, 62);
    p->base = 206;
    p = pecaAnimada(3, PECA_COMUM, SR "Vitrola/aparador_vitrola_senior_%02d.png", 0, 4, 0.2f, 30, 236);   // borda esquerda alinhada ao tapete
    colisao(p, 0, 40, 50, 80);
    p = pecaParada(3, PECA_COMUM, SR "sofa_couro_senior.png", 26, 362);
    colisao(p, 0, 20, 80, 190);
    p = pecaParada(3, PECA_COMUM, SR "mesa_vidro_senior.png", 120, 428);   // centro na vertical do tapete
    colisao(p, 0, 10, 45, 75);
    p = pecaParada(3, PECA_COMUM, SR "poltrona_senior.png", 196, 372);
    colisao(p, 0, 25, 70, 45);
    p = pecaParada(3, PECA_COMUM, SR "poltrona_senior.png", 196, 470);
    colisao(p, 0, 25, 70, 45);
    p = pecaAnimada(3, PECA_SINUCA, SR "Sinuca/sinuca_senior_%02d.png", 0, 32, 0.12f, 300, 330);
    colisao(p, 5, 10, 105, 175);
    colisao(p, 47, 246, 20, 20);
    p->base = 596;

    npcSinuca = carregarInteira(SR "npc_sinuca.png");
    maoSinuca = carregarInteira(SR "npc_sinuca_mao.png");

    #define TL "cenario/elementos_techlead/"
    p = pecaParada(4, PECA_CHAO, TL "carpete_techlead.png", 24, 330);
    p = pecaParada(4, PECA_COMUM, TL "bambu.png", 20, 71);
    colisao(p, 0, 81, 50, 54);
    p = pecaParada(4, PECA_CAFE, TL "maquina_cafe.png", 74, 56);
    colisao(p, 0, 90, 80, 60);
    p = pecaParada(4, PECA_COMUM, TL "frigobar_coco.png", 164, 96);
    colisao(p, 0, 66, 65, 44);
    p = pecaAnimada(4, PECA_COMUM, TL "MesaAlmoco/MesaAlmoco%d.png", 1, 3, 0.5f, 240, 116);
    p->sequencia = sequenciaMesaAlmoco;
    p->tamanhoSequencia = 4;
    colisao(p, 0, 54, 198, 36);
    p = pecaParada(4, PECA_COMUM, TL "arquivo_aco.png", 1005, 99);
    colisao(p, 0, 54, 45, 36);
    p = pecaParada(4, PECA_COMUM, TL "quadro_sprint.png", 1058, 99);
    colisao(p, 0, 75, 95, 50);
    p = pecaParada(4, PECA_COMUM, TL "sofa_creme_techlead.png", 26, 336);
    colisao(p, 0, 20, 80, 190);
    p = pecaParada(4, PECA_COMUM, TL "puffs.png", 124, 470);
    colisao(p, 0, 10, 115, 30);
    p = pecaAnimada(4, PECA_COMUM, TL "FliperamaElse/fliperama_else_%02d.png", 0, 12, 0.2f, 452, 66);
    colisao(p, 0, 90, 90, 50);
    p = pecaAnimada(4, PECA_COMUM, TL "FliperamaSnake/fliperama_snake_%02d.png", 0, 12, 0.2f, 546, 66);
    colisao(p, 0, 90, 90, 50);
    p = novaPeca(4, PECA_COMUM, 473, 122);
    p->quadros[0] = carregarInteira(TL "npc_jogadora_fliperama.png");
    p->total = 1;
    colisao(p, 14, 76, 20, 18);

    for (int cargo = 1; cargo <= 4; cargo++)
        for (int i = 0; i < cantinhos[cargo].quantidade; i++)
            finalizarPeca(&cantinhos[cargo].pecas[i]);
}


static bool objetoEstagiarioAtivo(int i, int cargo)
{
    if (cargo < 1 || cargo > 4)
        return true;

    if (i == 0 || i == 1 || i == 2 || i == 16 || i == 17)
        return false;

    if (cargo == 4 && (i == 3 || i == 4))
        return false;

    return true;
}


static bool colideComCantinho(int cargo, Rectangle boneco)
{
    if (cargo < 1 || cargo > 4)
        return false;

    for (int i = 0; i < cantinhos[cargo].quantidade; i++)
    {
        PecaCantinho *peca = &cantinhos[cargo].pecas[i];

        for (int c = 0; c < peca->quantidadeColisoes; c++)
            if (CheckCollisionRecs(boneco, peca->colisoes[c]))
                return true;
    }

    return false;
}


static int pecaInterativaPerto(int cargo, Rectangle boneco)
{
    if (cargo < 1 || cargo > 4)
        return PECA_COMUM;

    bool pertoDoCafe = false;

    for (int i = 0; i < cantinhos[cargo].quantidade; i++)
    {
        PecaCantinho *peca = &cantinhos[cargo].pecas[i];

        if (!CheckCollisionRecs(boneco, peca->areaInteracao))
            continue;

        if (peca->tipo == PECA_FREEZER)
            return peca->tipo;

        if (peca->tipo == PECA_CAFE)
            pertoDoCafe = true;
    }

    return pertoDoCafe ? PECA_CAFE : PECA_COMUM;
}


static void usarFreezer(void)
{
    tempoFreezerAberto = 4.0f;
    balaoFreezer = GetRandomValue(0, 3);
}


static void atualizarCantinhos(void)
{
    float dt = GetFrameTime();

    if (tempoFreezerAberto > 0.0f)
        tempoFreezerAberto -= dt;
}


static Vector2 maoPingPong(bool deCima, int pose)
{
    static const Vector2 maosCima[4] = {{16, 64}, {6, 42}, {21, 69}, {30, 63}};
    static const Vector2 maosBaixo[4] = {{49, 64}, {60, 48}, {54, 30}, {60, 39}};

    return deCima ? maosCima[pose] : maosBaixo[pose];
}


static void desenharPingPong(PecaCantinho *mesa)
{
    static const float passos[4] = {-14, 6, -6, 12};

    int quadro = (int)(GetTime() / 0.06);
    int travessia = quadro / 16;
    int k = quadro % 16;
    float s = k / 16.0f;

    int indice = (travessia / 2) % 4;
    float xa = passos[indice];
    float xaProximo = passos[(indice + 1) % 4];
    bool cimaRebate = travessia % 2 == 0;

    if (!cimaRebate)
    {
        float t = s / 0.6f;
        if (t > 1.0f) t = 1.0f;
        xa = xa + (xaProximo - xa) * t;
    }

    int poseRebate = k == 0 ? 2 : (k <= 3 ? 3 : 0);
    int poseRecebe = k >= 12 ? 1 : 0;
    int poseCima = cimaRebate ? poseRebate : poseRecebe;
    int poseBaixo = cimaRebate ? poseRecebe : poseRebate;

    float cimaX = 344 + xa;
    float cimaY = 269;
    float baixoX = 315;
    float baixoY = 499;

    DrawTexture(poseCima == 0 ? pingPongCorpoParado : pingPongCorpoSemBraco, (int)cimaX, (int)cimaY, WHITE);
    DrawTexture(mesa->quadros[0], (int)mesa->x, (int)mesa->y, WHITE);

    if (poseCima != 0)
        DrawTexture(pingPongBracoCima[poseCima], (int)cimaX, (int)cimaY, WHITE);

    Vector2 maoCima = maoPingPong(true, poseCima);
    DrawTexture(raqueteCima, (int)(cimaX + maoCima.x - 6), (int)(cimaY + maoCima.y - 2), WHITE);

    Vector2 pontoCima = cimaRebate
        ? (Vector2){365 + passos[indice], 346}
        : (Vector2){365 + xaProximo, 346};
    Vector2 pontoBaixo = {369, 521};
    Vector2 de = cimaRebate ? pontoCima : pontoBaixo;
    Vector2 para = cimaRebate ? pontoBaixo : pontoCima;

    float bolaX = de.x + (para.x - de.x) * s;
    float bolaY = de.y + (para.y - de.y) * s;
    float alto = s < 0.7f
        ? 12.0f * sinf(PI * s / 0.7f)
        : 7.0f * sinf(PI * (s - 0.7f) / 0.6f);

    DrawEllipse((int)bolaX, (int)bolaY, 3, 1.5f, (Color){0, 0, 0, 80});
    DrawTexture(bolinhaPingPong, (int)(bolaX - 3), (int)(bolaY - 3 - alto), WHITE);

    DrawTexture(pingPongCostas[poseBaixo], (int)baixoX, (int)baixoY, WHITE);

    Vector2 maoBaixo = maoPingPong(false, poseBaixo);
    DrawTexture(raqueteBaixo, (int)(baixoX + maoBaixo.x - 6), (int)(baixoY + maoBaixo.y - 16), WHITE);
}


static int puxaTacoSinuca(int f)
{
    static const int mira[7] = {0, 6, 2, 8, 2, 12, 0};
    static const int volta[4] = {10, 4, 8, 2};

    if (f < 7) return mira[f];
    if (f >= 20 && f < 28) return 14;
    if (f >= 28) return volta[f - 28];

    return 0;
}


static void desenharSinuca(PecaCantinho *mesa)
{
    int f = (int)(GetTime() / mesa->duracao) % mesa->total;
    int puxa = puxaTacoSinuca(f);

    int tacoX = 372;
    int topo = 477 + puxa;
    int fim = 566 + puxa;

    DrawTexture(mesa->quadros[f], (int)mesa->x, (int)mesa->y, WHITE);
    DrawTexture(npcSinuca, 333, 500, WHITE);

    DrawRectangle(tacoX - 2, topo - 1, 5, fim - topo + 3, BLACK);
    DrawRectangle(tacoX - 1, topo, 3, fim - topo + 1, (Color){214, 184, 132, 255});
    DrawRectangle(tacoX - 1, fim - 26, 3, 27, (Color){96, 56, 34, 255});
    DrawRectangle(tacoX - 1, topo, 3, 3, (Color){70, 120, 200, 255});

    DrawTexture(maoSinuca, 366, 557, WHITE);
}


static void desenharPeca(PecaCantinho *peca)
{
    if (peca->tipo == PECA_PINGPONG)
    {
        desenharPingPong(peca);
        return;
    }

    if (peca->tipo == PECA_SINUCA)
    {
        desenharSinuca(peca);
        return;
    }

    Texture2D textura = peca->quadros[0];

    if (peca->tipo == PECA_FREEZER && tempoFreezerAberto > 0.0f)
    {
        textura = freezerAberto;
    }
    else if (peca->total > 1)
    {
        int passo = (int)(GetTime() / peca->duracao);

        if (peca->sequencia != NULL)
            textura = peca->quadros[peca->sequencia[passo % peca->tamanhoSequencia]];
        else
            textura = peca->quadros[passo % peca->total];
    }

    DrawTexture(textura, (int)peca->x, (int)peca->y, WHITE);

    if (peca->tipo == PECA_FREEZER && tempoFreezerAberto > 0.0f)
        DrawTexture(baloesFreezer[balaoFreezer], 406, 100, WHITE);
}


static void desenharCantinho(int cargo, float posicaoPes, bool depoisDoPersonagem)
{
    if (cargo < 1 || cargo > 4)
        return;

    for (int i = 0; i < cantinhos[cargo].quantidade; i++)
    {
        PecaCantinho *peca = &cantinhos[cargo].pecas[i];

        bool atras = peca->tipo == PECA_CHAO || peca->base <= posicaoPes;

        if (atras != depoisDoPersonagem)
            desenharPeca(peca);
    }
}


int main()
{
    // ==========================================================
    // JANELA / RESOLUÇÃO
    // ==========================================================
    int larguraResolucao = 1280;
    int alturaResolucao = 720;

    SetConfigFlags(
        FLAG_WINDOW_RESIZABLE |
        FLAG_WINDOW_UNDECORATED
    );

    InitWindow(
        larguraResolucao,
        alturaResolucao,
        "else"
    );
InicializarBancoPerguntas();
   CarregarTexturasCartas();


    SetExitKey(KEY_NULL);

    int monitor = GetCurrentMonitor();

    int larguraMonitor =
        GetMonitorWidth(monitor);

    int alturaMonitor =
        GetMonitorHeight(monitor);

    Vector2 posicaoMonitor =
        GetMonitorPosition(monitor);

    SetWindowPosition(
        posicaoMonitor.x,
        posicaoMonitor.y
    );

    SetWindowSize(
        larguraMonitor,
        alturaMonitor
    );

    SetTargetFPS(60);

    InitAudioDevice();

    RenderTexture2D telaVirtual =
        LoadRenderTexture(
            larguraResolucao,
            alturaResolucao
        );


    // ==========================================================
    // SOM
    // ==========================================================
    Music trilhaSonora1 =
        LoadMusicStream(
            "musicas/trilhaTemporaria.mp3"
        );

    Music trilhaSonora2 =
        LoadMusicStream(
            "musicas/JazzTrilha2.mp3"
        );

    Music EscritorioTrilhaSonora =
        LoadMusicStream(
            "musicas/Escritorio2.mp3"
        );

    Music gameOverSom =
        LoadMusicStream(
            "musicas/GameOverSong.mp3"
        );

    Sound somDePassos =
        LoadSound(
            "EfeitosSonoros/Passos2.mp3"
        );

    SetMusicVolume(
        trilhaSonora2,
        0.05f
    );

    SetMusicVolume(
        trilhaSonora1,
        0.10f
    );

    SetMusicVolume(
        EscritorioTrilhaSonora,
        0.2f
    );

    PlayMusicStream(
        trilhaSonora1
    );

    PlayMusicStream(
        EscritorioTrilhaSonora
    );


    // ==========================================================
    // TEXTURAS - CENÁRIO
    // ==========================================================
    Texture2D fundoCeramica =
        LoadTexture(
            "cenario/Cenario.png"
        );

    // fundo de cada cargo: 0 Estagiario, 1 Junior, 2 Pleno, 3 Senior, 4 Tech Lider
    Texture2D fundosCargo[5] =
    {
        fundoCeramica,
        LoadTexture("cenario/Cenarios/Junior/CenarioJuniorDia.png"),
        LoadTexture("cenario/Cenarios/Pleno/CenarioPlenoDia.png"),
        LoadTexture("cenario/Cenarios/Senior/CenarioSeniorDia.png"),
        LoadTexture("cenario/Cenarios/TechLider/CenarioTechDia.png")
    };

    carregarMesasNiveis();
    carregarCantinhos();

    // MAQUINA DE CAFE: carrega as pecas e comeca disponivel
    iniciarMaquinaCafe();
    reiniciarMaquinaCafe();


    #define ELEMENTOS "cenario/elementos_estagiario/"


    ObjetoCenario objetos[] =
    {
        criarObjeto(
            ELEMENTOS "planta.png",
            12,
            83,
            (Rectangle){5, 55, 45, 55},
            false,
            0
        ),

        criarObjeto(
            ELEMENTOS "balcao_cafe.png",
            67,
            91,
            (Rectangle){0, 45, 155, 70},
            true,   // MAQUINA DE CAFE: o balcao agora e interativo
            0
        ),

        criarObjeto(
            ELEMENTOS "bebedouro.png",
            234,
            79,
            (Rectangle){0, 60, 52, 65},
            false,
            0
        ),

        criarObjeto(
            ELEMENTOS "lixeira.png",
            1052,
            148,
            (Rectangle){5, 10, 40, 40},
            false,
            0
        ),

        criarObjeto(
            ELEMENTOS "folhas.png",
            1107,
            121,
            (Rectangle){0, 30, 50, 52},
            false,
            0
        ),

        criarObjeto(
            ELEMENTOS "impressora.png",
            1161,
            99,
            (Rectangle){0, 55, 80, 45},
            true,
            1
        ),

        criarObjeto(
            ELEMENTOS "MesaFrenteSemNpc.png",
            430,
            251,
            (Rectangle){10, 64, 142, 68},
            true,
            0
        ),

        criarObjeto(
            ELEMENTOS "MesaFrenteSemNpc.png",
            574,
            251,
            (Rectangle){10, 64, 142, 68},
            true,
            0
        ),

        criarObjeto(
            ELEMENTOS "MesaFrenteSemNpc.png",
            824,
            251,
            (Rectangle){10, 64, 142, 68},
            true,
            0
        ),

        criarObjeto(
            ELEMENTOS "MesaFrenteSemNpc.png",
            968,
            251,
            (Rectangle){10, 64, 142, 68},
            true,
            0
        ),


        criarObjeto(
            ELEMENTOS "MesaCostaSemNpc.png",
            573,
            473,
            (Rectangle){10, 45, 142, 75},
            true,
            0
        ),

        criarObjeto(
            ELEMENTOS "MesaCostaSemNpc2.png",
            834,
            473,
            (Rectangle){10, 45, 142, 75},
            true,
            0
        ),

        criarObjeto(
            ELEMENTOS "NpcMulherDigitando/NpcMulherDigitando1.png",
            979,
            473,
            (Rectangle){10, 45, 142, 75},
            true,
            0
        ),

        criarObjeto(
            ELEMENTOS "MesaCostaComNpc/MesaCostaComNpc1.png",
            428,
            473,
            (Rectangle){10, 45, 142, 75},
            true,
            0
        ),

        criarObjeto(
            ELEMENTOS "MesaCostaComNpc/MesaCostaComNpc2.png",
            428,
            473,
            (Rectangle){10, 45, 142, 75},
            true,
            0
        ),

        criarObjeto(
            ELEMENTOS "MesaCostaComNpc/MesaCostaComNpc3.png",
            428,
            473,
            (Rectangle){10, 45, 142, 75},
            true,
            0
        )
,

        // CANTINHO DO DESCANSO (objetos 16 e 17)
        criarObjeto(
            ELEMENTOS "sofa.png",
            28,
            348,
            (Rectangle){0, 10, 88, 190},
            false,
            0
        ),

        criarObjeto(
            ELEMENTOS "AquarioPeixe/AquarioPeixe1.png",
            30,
            242,
            (Rectangle){2, 45, 70, 55},
            false,
            0
        )
    };


    int quantidadeObjetos =
        sizeof(objetos) /
        sizeof(objetos[0]);

    int frameNpc = 0;
    float tempoAnimacaoNpc = 0;

    // ---- NOVOS NPCs ANIMADOS -------------------------------
    // mulher de costas (objeto 12): digitando, mãos alternando
    Texture2D npcMulher[3] = {
        LoadTexture(ELEMENTOS "NpcMulherDigitando/NpcMulherDigitando1.png"),
        LoadTexture(ELEMENTOS "NpcMulherDigitando/NpcMulherDigitando2.png"),
        LoadTexture(ELEMENTOS "NpcMulherDigitando/NpcMulherDigitando3.png")
    };
    int sequenciaMulher[4] = {0, 1, 0, 2};          // normal, mão esq., normal, mão dir.

    // NPC da 1ª fileira (no lugar da mesa vazia, objeto 7): pisca
    Texture2D npcFrente[2] = {
        LoadTexture(ELEMENTOS "NpcFrentePiscando/NpcFrentePiscando1.png"),
        LoadTexture(ELEMENTOS "NpcFrentePiscando/NpcFrentePiscando2.png")
    };
    int sequenciaFrente[16] = {0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0};   // 16 passos de 0,25s = 4s (só pisca)

    // o jogo desenha os objetos do cenário pela metade (igual ao criarObjeto)
    for (int k = 0; k < 3; k++)
    {
        npcMulher[k].width *= 0.5;  npcMulher[k].height *= 0.5;
    }
    for (int k = 0; k < 2; k++)
    {
        npcFrente[k].width *= 0.5;  npcFrente[k].height *= 0.5;
    }

    // bebedouro (objeto 2): de tempo em tempo sobem bolhas no galão
    Texture2D bebedouroBolhas[5];
    for (int k = 0; k < 5; k++)
    {
        bebedouroBolhas[k] = LoadTexture(TextFormat(ELEMENTOS "BebedouroBolhas/BebedouroBolhas%d.png", k + 1));
        bebedouroBolhas[k].width *= 0.5;
        bebedouroBolhas[k].height *= 0.5;
    }
    // tapete do cantinho: fica no CHAO, sem colisao (desenhado antes dos objetos)
    Texture2D tapete = LoadTexture(ELEMENTOS "tapete.png");
    tapete.width *= 0.5;
    tapete.height *= 0.5;

    // aquario (objeto 17): peixe nadando, 8 quadros
    Texture2D aquarioPeixe[8];
    for (int k = 0; k < 8; k++)
    {
        aquarioPeixe[k] = LoadTexture(TextFormat(ELEMENTOS "AquarioPeixe/AquarioPeixe%d.png", k + 1));
        aquarioPeixe[k].width *= 0.5;
        aquarioPeixe[k].height *= 0.5;
    }

    float tempoBolhas = 0.0f;
    float esperaBolhas = 3.0f;      // segundos parado até a próxima bolha (sorteado de novo a cada vez)

    float tempoNovosNpcs = 0.0f;
    int passoNovosNpcs = 0;

    Texture2D logoInicio =
        LoadTexture(
            "cenario/logoInicio.png"
        );

    Texture2D fundoCarta =
        LoadTexture(
            "cenario/Frame3.png"
        );

    fundoCarta.height *= 0.95;
    fundoCarta.width *= 0.95;


    // ==========================================================
    // HUD
    // ==========================================================
    Texture2D confianca =
        LoadTexture(
            "HUD/hud_confianca_vazia.png"
        );

    confianca.width *= 0.8;
    confianca.height *= 0.8;


    Texture2D lucro =
        LoadTexture(
            "HUD/hud_lucro_vazio.png"
        );

    lucro.width *= 0.8;
    lucro.height *= 0.8;


    Texture2D privacidade =
        LoadTexture(
            "HUD/hud_privanca_vazia.png"
        );

    privacidade.width *= 0.7;
    privacidade.height *= 0.7;


    Texture2D vies =
        LoadTexture(
            "HUD/hud_vieis_vazio.png"
        );

    vies.width *= 0.8;
    vies.height *= 0.8;


    Texture2D confiancaCheia =
        LoadTexture(
            "HUD/hud_confianca_cheia.png"
        );

    confiancaCheia.width *= 0.8;
    confiancaCheia.height *= 0.8;


    Texture2D lucroCheio =
        LoadTexture(
            "HUD/hud_lucro_cheio.png"
        );

    lucroCheio.width *= 0.8;
    lucroCheio.height *= 0.8;


    Texture2D privacidadeCheia =
        LoadTexture(
            "HUD/hud_privanca_cheia.png"
        );

    privacidadeCheia.width *= 0.7;
    privacidadeCheia.height *= 0.7;


    Texture2D viesCheio =
        LoadTexture(
            "HUD/hud_vieis_cheio.png"
        );

    viesCheio.width *= 0.8;
    viesCheio.height *= 0.8;


    Texture2D fundoHUD =
        LoadTexture(
            "HUD/FUNDO.png"
        );

    fundoHUD.width *= 0.85;
    fundoHUD.height *= 0.8;

    Texture2D telaContadorDeDias = LoadTexture("HUD/DiaContador2.png");
    telaContadorDeDias.height*=1.5;
    telaContadorDeDias.width*=2.3;


    // ==========================================================
    // PERSONAGEM
    // ==========================================================
    Texture2D costas1 =
        LoadTexture(
            "assets/Costas/BonecoParadoCostas.png"
        );

    Texture2D costas2 =
        LoadTexture(
            "assets/Costas/BonecoAndandoCostas1.png"
        );

    Texture2D costas3 =
        LoadTexture(
            "assets/Costas/BonecoAndandoCostas2.png"
        );


    Texture2D frente1 =
        LoadTexture(
            "assets/Frente/BonecoParadoFrente.png"
        );

    Texture2D frente2 =
        LoadTexture(
            "assets/Frente/BonecoAndandoFrente1.png"
        );

    Texture2D frente3 =
        LoadTexture(
            "assets/Frente/BonecoAndandoFrente2.png"
        );


    Texture2D direita1 =
        LoadTexture(
            "assets/LadoDireito/BonecoParadoLadoDireito.png"
        );

    Texture2D direita2 =
        LoadTexture(
            "assets/LadoDireito/BonecoAndandoLadoDireito1.png"
        );

    Texture2D direita3 =
        LoadTexture(
            "assets/LadoDireito/BonecoAndandoLadoDireito2.png"
        );


    Texture2D esquerda1 =
        LoadTexture(
            "assets/LadoEsquerdo/BonecoParadoLadoEsquerdo.png"
        );

    Texture2D esquerda2 =
        LoadTexture(
            "assets/LadoEsquerdo/BonecoAndandoLadoEsquerdo1.png"
        );

    Texture2D esquerda3 =
        LoadTexture(
            "assets/LadoEsquerdo/BonecoAndandoLadoEsquerdo2.png"
        );


    // ==========================================================
    // BOTÕES
    // ==========================================================
    Texture2D botaoJogar =
        LoadTexture(
            "Botoes/BotaoJogar.png"
        );

    Texture2D botaoExit =
        LoadTexture(
            "Botoes/botaoExit.png"
        );

    Texture2D botaoPlayAgain =
        LoadTexture(
            "Botoes/botaoPlayAgain.png"
        );

    Texture2D botaoShare =
        LoadTexture(
            "Botoes/botaoShare.png"
        );

    Texture2D botaoContinue =
        LoadTexture(
            "Botoes/BotaoContinue.png"
        );

    Texture2D botaoInstrucoesPausa =
        LoadTexture(
            "Botoes/botaoInstrucoes.png"
        );

    Texture2D botaoMenuInicial =
        LoadTexture(
            "Botoes/botaoMenuInicial.png"
        );


    // ==========================================================
    // TELAS
    // ==========================================================
    Texture2D telaPausa =
        LoadTexture(
            "cenario/TelaPausa.png"
        );

    Texture2D botaoVoltarPausa =
        LoadTexture(
            "Botoes/botaoVoltar.png"
        );

    Texture2D telaInstrucoesPausa =
        LoadTexture(
            "cenario/Telas/telaInstrucoes.png"
        );

    Texture2D painelFimExpediente =
        LoadTexture(
            "cenario/Telas/painelFimExpediente.png"
        );

    // memorandos da auditoria (entrada e saida)
    Texture2D telaAuditoria =
        LoadTexture(
            "cenario/Telas/telaAuditoria.png"
        );

    Texture2D telaAuditoriaFim =
        LoadTexture(
            "cenario/Telas/telaAuditoriaFim.png"
        );

    // botao CIENTE do memorando (verde = auditoria acabou, vermelho = comecou)
    Texture2D botaoCienteVerde =
        LoadTexture("Botoes/BOTAO ciente (elemento separado).png");
    Texture2D botaoCienteVermelho =
        LoadTexture("Botoes/BOTAO ciente (elemento separado) (1).png");
    Rectangle retCiente = { 528, 600, 224, 56 };   // tela virtual, dentro da folha do memorando


    // ==========================================================
    // ESTADO
    // ==========================================================
    int tela = -1;

    float tempoIntro = 0.0f;
    float tempoLogo = 2.0f;
    float tempoFade = 2.0f;


    // ==========================================================
    // MOVIMENTAÇÃO
    // ==========================================================
    float x = 300;
    float y = 170;
    int cargoAnterior = 0;

    int velocidade = 5;
    int direcao = 0;
    int andando = 0;


    // ==========================================================
    // ANIMAÇÃO
    // ==========================================================
    int frame = 0;
    float tempo = 0;

    Texture2D texturaAtualGuardada =
        frente1;


    // ==========================================================
    // HUD
    // ==========================================================
    float porcentagemConfiancaApagado =
        0.365f;

    float porcentagemViesApagado =
        0.365f;

    float porcentagemPrivacidadeApagado =
        0.365f;

    float porcentagemLucroApagado =
        0.365f;


    iniciarCartas(
        &porcentagemConfiancaApagado,
        &porcentagemViesApagado,
        &porcentagemPrivacidadeApagado,
        &porcentagemLucroApagado
    );
    reiniciarMaquinaCafe();   // MAQUINA DE CAFE


    // ==========================================================
    // CARTAS
    // ==========================================================
    bool mensagem =
        false;

    bool cartaAbertaPararDeAndar =
        false;

    bool abrirCarta =
        false;


    // ==========================================================
    // PAUSE / GAME OVER
    // ==========================================================
    bool pausado =
        false;

    bool instrucoesPausa =
        false;

    bool gameOver =
        false;

    // botoes da tela de jogo (retangulos na tela virtual 1280x720)
    Texture2D texBotaoMenu = LoadTexture("Botoes/BotaoPausa.png");
    Texture2D texBotaoSom = LoadTexture("Botoes/Som.png");
    Texture2D texBotaoMute = LoadTexture("Botoes/Mute.png");
    Rectangle retBotaoMenu = { 16, 16, 56, 56 };          // canto superior esquerdo
    Rectangle retBotaoSom = { larguraResolucao - 72, 88, 56, 56 };  // canto superior direito, abaixo do contador de dias

    // memorando da auditoria na tela: 0 = nenhum, 1 = entrada, 2 = saida
    int memorando =
        0;

    int k = 0;


    // ==========================================================
    // LOOP
    // ==========================================================

    while (!WindowShouldClose())
    {
        limparTextosUI();

        float escalaX =
            (float)GetScreenWidth() /
            larguraResolucao;

        float escalaY =
            (float)GetScreenHeight() /
            alturaResolucao;

        float escala;


        int xGuardado =
            x;

        int yGuardado =
            y;


        if (escalaX < escalaY)
            escala = escalaX;
        else
            escala = escalaY;


        float larguraFinal =
            larguraResolucao *
            escala;

        float alturaFinal =
            alturaResolucao *
            escala;


        float offsetX =
            (
                GetScreenWidth() -
                larguraFinal
            ) /
            2;


        float offsetY =
            (
                GetScreenHeight() -
                alturaFinal
            ) /
            2;


        // ======================================================
        // ESC / PAUSE
        // ======================================================
        // clique nos botoes da tela de jogo (mouse convertido para a tela virtual)
        bool cliqueBotaoMenu = false;

        if (
            tela == 2 &&
            !pausado &&
            !gameOver &&
            !maquinaCafeAberta() &&
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
        )
        {
            Vector2 mouseJogo = GetMousePosition();
            mouseJogo.x = (mouseJogo.x - offsetX) / escala;
            mouseJogo.y = (mouseJogo.y - offsetY) / escala;

            if (CheckCollisionPointRec(mouseJogo, retBotaoMenu))
            {
                cliqueBotaoMenu = true;
            }
            else if (CheckCollisionPointRec(mouseJogo, retBotaoSom))
            {
                somMutadoGlobal = !somMutadoGlobal;
                SetMasterVolume(somMutadoGlobal ? 0.0f : 1.0f);
            }
        }

        if (
            tela == 2 &&
            (IsKeyPressed(KEY_ESCAPE) || cliqueBotaoMenu)
        )
        {
            if (maquinaCafeAberta())   // MAQUINA DE CAFE: ESC so fecha a maquina
            {
                fecharMaquinaCafe();
            }
            else if (instrucoesPausa)
            {
                instrucoesPausa =
                    false;
            }
            else
            {
                pausado =
                    !pausado;


                if (pausado)
                {
                    PauseMusicStream(
                        trilhaSonora2
                    );

                    StopSound(
                        somDePassos
                    );
                }
                else
                {
                    ResumeMusicStream(
                        trilhaSonora2
                    );
                }
            }
        }


        // ======================================================
        // MENU DE PAUSA
        // ======================================================
        if (
            tela == 2 &&
            pausado
        )
        {
            BeginDrawing();

            ClearBackground(
                BLACK
            );


            Rectangle origemPause =
            {
                0,
                0,

                (float)telaVirtual.texture.width,
                -(float)telaVirtual.texture.height
            };


            Rectangle destinoJogoPause =
            {
                offsetX,
                offsetY,
                larguraFinal,
                alturaFinal
            };


            // ==================================================
            // JOGO CONGELADO
            // ==================================================
            DrawTexturePro(
                telaVirtual.texture,
                origemPause,
                destinoJogoPause,
                (Vector2){0, 0},
                0,
                WHITE
            );


            // Escurece o escritório
            DrawRectangle(
                offsetX,
                offsetY,
                larguraFinal,
                alturaFinal,
                Fade(
                    BLACK,
                    0.45f
                )
            );


            // ==================================================
            // INSTRUÇÕES
            // ==================================================
            if (instrucoesPausa)
            {
                DrawTexturePro(
                    telaInstrucoesPausa,

                    (Rectangle){
                        0,
                        0,
                        telaInstrucoesPausa.width,
                        telaInstrucoesPausa.height
                    },

                    destinoJogoPause,

                    (Vector2){0, 0},

                    0,

                    WHITE
                );


                Rectangle retVoltarInstrucoes =
                {
                    offsetX +
                    995 *
                    escala,

                    offsetY +
                    625 *
                    escala,

                    205 *
                    escala,

                    51 *
                    escala
                };


                Vector2 mouseInstrucoes =
                    GetMousePosition();


                // botao Voltar com a animacao padrao (desenhado por cima da imagem)
                Rectangle retVoltarDesenho = retanguloAnimadoBotao(
                    "voltar",
                    retVoltarInstrucoes,
                    CheckCollisionPointRec(mouseInstrucoes, retVoltarInstrucoes)
                );

                DrawTexturePro(
                    botaoVoltarPausa,
                    (Rectangle){30, 207, 447, 112},
                    retVoltarDesenho,
                    (Vector2){0, 0},
                    0,
                    WHITE
                );


                if (
                    IsMouseButtonPressed(
                        MOUSE_LEFT_BUTTON
                    ) &&
                    CheckCollisionPointRec(
                        mouseInstrucoes,
                        retVoltarInstrucoes
                    )
                )
                {
                    instrucoesPausa =
                        false;
                }


                EndDrawing();

                continue;
            }


            // ==================================================
            // PAINEL DE PAUSA
            // ==================================================
            float alturaPainelPausa =
                570.0f *
                escala;


            float escalaPainelPausa =
                alturaPainelPausa /
                (float)telaPausa.height;


            float larguraPainelPausa =
                telaPausa.width *
                escalaPainelPausa;


            // Post-it ocupa espaço à direita da imagem.
            // Corrige o centro visual da máquina.
            float ajustePainelX =
                65.0f *
                escala;


            Rectangle destinoTelaPausa =
            {
                offsetX +
                (
                    larguraFinal -
                    larguraPainelPausa
                ) /
                2 +
                ajustePainelX,

                offsetY +
                (
                    alturaFinal -
                    alturaPainelPausa
                ) /
                2,

                larguraPainelPausa,
                alturaPainelPausa
            };


            DrawTexturePro(
                telaPausa,

                (Rectangle){
                    0,
                    0,
                    telaPausa.width,
                    telaPausa.height
                },

                destinoTelaPausa,

                (Vector2){0, 0},

                0,

                WHITE
            );


            // ==================================================
            // BOTÕES DO PAUSE
            // ==================================================
            Rectangle areaUtilPause =
            {
                30,
                207,
                447,
                112
            };


            // Botoes maiores (a arte do botao tem proporcao 447 x 112)
            float larguraBotao =
                205.0f *
                escala;


            float alturaBotao =
                larguraBotao *
                areaUtilPause.height /
                areaUtilPause.width;


            float espacoBotoes =
                12.0f *
                escala;


            // Centro do quadrado cinza na imagem TelaPausa.png (585x585):
            // x = 230, y = 290. Os botoes ficam centralizados nele.
            float centroBotoesX =
                destinoTelaPausa.x +
                230.0f *
                escalaPainelPausa;

            float centroBotoesY =
                destinoTelaPausa.y +
                290.0f *
                escalaPainelPausa;

            float topoBotoes =
                centroBotoesY -
                (3 * alturaBotao + 2 * espacoBotoes) /
                2;


            // ==================================================
            // CONTINUAR
            // ==================================================
            Rectangle retContinueBase =
            {
                centroBotoesX -
                larguraBotao /
                2,

                topoBotoes,

                larguraBotao,
                alturaBotao
            };


            Rectangle retContinue =
                retContinueBase;


            // ==================================================
            // INSTRUÇÕES
            // ==================================================
            Rectangle retInstrucoesBase =
            {
                centroBotoesX -
                larguraBotao /
                2,

                topoBotoes +
                alturaBotao +
                espacoBotoes,

                larguraBotao,
                alturaBotao
            };


            Rectangle retInstrucoes =
                retInstrucoesBase;


            // ==================================================
            // MENU INICIAL
            // ==================================================
            Rectangle retMenuInicialBase =
            {
                centroBotoesX -
                larguraBotao /
                2,

                topoBotoes +
                2 * (alturaBotao + espacoBotoes),

                larguraBotao,
                alturaBotao
            };


            Rectangle retMenuInicial =
                retMenuInicialBase;


            Vector2 mousePause =
                GetMousePosition();


            // ==================================================
            // HOVER CONTINUAR
            // ==================================================
            retContinue = retanguloAnimadoBotao("pausa.continuar", retContinueBase, CheckCollisionPointRec(mousePause, retContinueBase));


            // ==================================================
            // HOVER INSTRUÇÕES
            // ==================================================
            retInstrucoes = retanguloAnimadoBotao("pausa.instrucoes", retInstrucoesBase, CheckCollisionPointRec(mousePause, retInstrucoesBase));


            // ==================================================
            // HOVER MENU INICIAL
            // ==================================================
            retMenuInicial = retanguloAnimadoBotao("pausa.menu", retMenuInicialBase, CheckCollisionPointRec(mousePause, retMenuInicialBase));


            // ==================================================
            // DESENHA CONTINUAR
            // ==================================================
            DrawTexturePro(
                botaoContinue,
                areaUtilPause,
                retContinue,
                (Vector2){0, 0},
                0,
                WHITE
            );


            // ==================================================
            // DESENHA INSTRUÇÕES
            // ==================================================
            DrawTexturePro(
                botaoInstrucoesPausa,
                areaUtilPause,
                retInstrucoes,
                (Vector2){0, 0},
                0,
                WHITE
            );


            // ==================================================
            // DESENHA MENU INICIAL
            // ==================================================
            DrawTexturePro(
                botaoMenuInicial,
                areaUtilPause,
                retMenuInicial,
                (Vector2){0, 0},
                0,
                WHITE
            );


            // ==================================================
            // CLIQUES
            // ==================================================
            if (
                IsMouseButtonPressed(
                    MOUSE_LEFT_BUTTON
                )
            )
            {
                // CONTINUAR
                if (
                    CheckCollisionPointRec(
                        mousePause,
                        retContinueBase
                    )
                )
                {
                    pausado =
                        false;


                    ResumeMusicStream(
                        trilhaSonora2
                    );
                }


                // INSTRUÇÕES
                if (
                    CheckCollisionPointRec(
                        mousePause,
                        retInstrucoesBase
                    )
                )
                {
                    instrucoesPausa =
                        true;
                }


                // MENU INICIAL
                if (
                    CheckCollisionPointRec(
                        mousePause,
                        retMenuInicialBase
                    )
                )
                {
                    pausado =
                        false;

                    instrucoesPausa =
                        false;


                    StopMusicStream(
                        trilhaSonora2
                    );


                    PlayMusicStream(
                        trilhaSonora1
                    );


                    tela =
                        0;


                    porcentagemConfiancaApagado =
                        0.365f;

                    porcentagemViesApagado =
                        0.365f;

                    porcentagemPrivacidadeApagado =
                        0.365f;

                    porcentagemLucroApagado =
                        0.365f;


                    x =
                        300;

                    y =
                        170;


                    direcao =
                        0;

                    andando =
                        0;

                    frame =
                        0;

                    tempo =
                        0;


                    cartaAbertaPararDeAndar =
                        false;

                    memorando =
                        0;

                    abrirCarta =
                        false;


                    iniciarCartas(
                        &porcentagemConfiancaApagado,
                        &porcentagemViesApagado,
                        &porcentagemPrivacidadeApagado,
                        &porcentagemLucroApagado
                    );
                    reiniciarMaquinaCafe();   // MAQUINA DE CAFE
                }
            }


            EndDrawing();

            continue;
        }


        // ======================================================
        // INTRO
        // ======================================================
        if (tela == -1)
        {
            tempoIntro +=
                GetFrameTime();


            float alpha =
                0.0f;


            if (
                tempoIntro >=
                tempoLogo
            )
            {
                alpha =
                    (
                        tempoIntro -
                        tempoLogo
                    ) /
                    tempoFade;


                if (alpha > 1.0f)
                    alpha =
                        1.0f;
            }


            BeginTextureMode(
                telaVirtual
            );


            ClearBackground(
                BLACK
            );


            float escalaLogoX =
                (
                    larguraResolucao *
                    0.55f
                ) /
                logoInicio.width;


            float escalaLogoY =
                (
                    alturaResolucao *
                    0.75f
                ) /
                logoInicio.height;


            float escalaLogo;


            if (
                escalaLogoX <
                escalaLogoY
            )
            {
                escalaLogo =
                    escalaLogoX;
            }
            else
            {
                escalaLogo =
                    escalaLogoY;
            }


            float larguraLogo =
                logoInicio.width *
                escalaLogo;


            float alturaLogo =
                logoInicio.height *
                escalaLogo;


            DrawTextureEx(
                logoInicio,

                (Vector2){
                    (
                        larguraResolucao -
                        larguraLogo
                    ) /
                    2,

                    (
                        alturaResolucao -
                        alturaLogo
                    ) /
                    2
                },

                0,

                escalaLogo,

                WHITE
            );


            DrawRectangle(
                0,
                0,
                larguraResolucao,
                alturaResolucao,
                Fade(
                    BLACK,
                    alpha
                )
            );


            EndTextureMode();


            BeginDrawing();

            ClearBackground(
                BLACK
            );


            Rectangle origemIntro =
            {
                0,
                0,

                (float)telaVirtual.texture.width,
                -(float)telaVirtual.texture.height
            };


            Rectangle destinoIntro =
            {
                offsetX,
                offsetY,
                larguraFinal,
                alturaFinal
            };


            DrawTexturePro(
                telaVirtual.texture,
                origemIntro,
                destinoIntro,
                (Vector2){0, 0},
                0,
                WHITE
            );


            EndDrawing();


            if (
                tempoIntro >=
                tempoLogo +
                tempoFade
            )
            {
                tela =
                    0;
            }


            continue;
        }


        // ======================================================
        // MENU INICIAL
        // ======================================================
        if (tela == 0)
        {
            UpdateMusicStream(
                trilhaSonora1
            );


            int resultadoMenu =
                telaInicio(
                    botaoJogar,
                    telaVirtual,
                    escala,
                    offsetX,
                    offsetY,
                    trilhaSonora1
                );


            if (
                resultadoMenu ==
                -1
            )
            {
                break;
            }


            if (
                resultadoMenu ==
                1
            )
            {
                tela =
                    1;


                PlayMusicStream(
                    EscritorioTrilhaSonora
                );


                StopMusicStream(
                    trilhaSonora1
                );
            }


            continue;
        }


        // ======================================================
        // AVISO
        // ======================================================
        if (tela == 1)
        {
            UpdateMusicStream(
                EscritorioTrilhaSonora
            );


            if (
                telaAviso(
                    telaVirtual,
                    escala,
                    offsetX,
                    offsetY
                ) ==
                2
            )
            {
                tela =
                    2;


                StopMusicStream(
                    EscritorioTrilhaSonora
                );


                PlayMusicStream(
                    trilhaSonora2
                );
            }


            continue;
        }


        // ======================================================
        // JOGO
        // ======================================================
        if (tela == 2)
        {
            UpdateMusicStream(
                trilhaSonora2
            );
        }


        // ======================================================
        // MOVIMENTAÇÃO
        // ======================================================
        andando =
            0;


        Texture2D texturaAtual =
            texturaAtualGuardada;


        // Ao escolher a carta com A/D a tecla ainda esta pressionada quando a
        // carta some: o boneco so volta a andar depois de soltar as teclas.
        static bool esperarSoltarTeclas = false;

        if (cartaAbertaPararDeAndar)
        {
            esperarSoltarTeclas = true;
        }
        else if (
            !IsKeyDown(KEY_W) && !IsKeyDown(KEY_A) &&
            !IsKeyDown(KEY_S) && !IsKeyDown(KEY_D)
        )
        {
            esperarSoltarTeclas = false;
        }


        bool botaoAeDapertados =
            IsKeyDown(KEY_D) &&
            IsKeyDown(KEY_A);


        bool botaoWeSapertados =
            IsKeyDown(KEY_W) &&
            IsKeyDown(KEY_S);


        if (
            !cartaAbertaPararDeAndar && !maquinaCafeAberta() &&
            !esperarSoltarTeclas &&
            !gameOver
        )
        {
            if (
                !botaoAeDapertados &&
                !botaoWeSapertados
            )
            {
                if (
                    IsKeyDown(KEY_W)
                )
                {
                    if (
                        !IsSoundPlaying(
                            somDePassos
                        )
                    )
                    {
                        PlaySound(
                            somDePassos
                        );
                    }


                    y -=
                        velocidade;


                    direcao =
                        1;


                    andando =
                        1;


                    if (
                        IsKeyPressed(
                            KEY_W
                        )
                    )
                    {
                        frame =
                            1;
                    }
                }


                if (
                    IsKeyDown(KEY_S)
                )
                {
                    if (
                        !IsSoundPlaying(
                            somDePassos
                        )
                    )
                    {
                        PlaySound(
                            somDePassos
                        );
                    }


                    y +=
                        velocidade;


                    direcao =
                        0;


                    andando =
                        1;


                    if (
                        IsKeyPressed(
                            KEY_S
                        )
                    )
                    {
                        frame =
                            1;
                    }
                }


                if (
                    IsKeyDown(KEY_A)
                )
                {
                    if (
                        !IsSoundPlaying(
                            somDePassos
                        )
                    )
                    {
                        PlaySound(
                            somDePassos
                        );
                    }


                    x -=
                        velocidade;


                    direcao =
                        2;


                    andando =
                        1;


                    if (
                        IsKeyPressed(
                            KEY_A
                        )
                    )
                    {
                        frame =
                            1;
                    }
                }


                if (
                    IsKeyDown(KEY_D)
                )
                {
                    texturaAtual =
                        direita2;


                    if (
                        !IsSoundPlaying(
                            somDePassos
                        )
                    )
                    {
                        PlaySound(
                            somDePassos
                        );
                    }


                    x +=
                        velocidade;


                    direcao =
                        3;


                    andando =
                        1;


                    if (
                        IsKeyPressed(
                            KEY_D
                        )
                    )
                    {
                        frame =
                            1;
                    }
                }
            }
            else
            {
                andando =
                    0;


                if (
                    texturaAtualGuardada.id ==
                    frente2.id ||
                    texturaAtualGuardada.id ==
                    frente3.id
                )
                {
                    texturaAtual =
                        frente1;
                }


                if (
                    texturaAtualGuardada.id ==
                    costas2.id ||
                    texturaAtualGuardada.id ==
                    costas3.id
                )
                {
                    texturaAtual =
                        costas1;
                }


                if (
                    texturaAtualGuardada.id ==
                    direita2.id ||
                    texturaAtualGuardada.id ==
                    direita3.id
                )
                {
                    texturaAtual =
                        direita1;
                }


                if (
                    texturaAtualGuardada.id ==
                    esquerda2.id ||
                    texturaAtualGuardada.id ==
                    esquerda3.id
                )
                {
                    texturaAtual =
                        esquerda1;
                }
            }
        }
        else
        {
            andando =
                0;


            StopSound(
                somDePassos
            );


            if (direcao == 0)
                texturaAtual =
                    frente1;

            else if (direcao == 1)
                texturaAtual =
                    costas1;

            else if (direcao == 2)
                texturaAtual =
                    esquerda1;

            else
                texturaAtual =
                    direita1;
        }


        // ======================================================
        // ANIMAÇÃO
        // ======================================================
        if (
            andando ==
            1
        )
        {
            tempo +=
                GetFrameTime();


            if (
                tempo >=
                0.12
            )
            {
                frame++;

                tempo =
                    0;


                if (
                    frame >=
                    3
                )
                {
                    frame =
                        0;
                }
            }
        }
        else
        {
            frame =
                0;

            tempo =
                0;
        }


        if (
            !cartaAbertaPararDeAndar && !maquinaCafeAberta() &&
            !botaoAeDapertados &&
            !botaoWeSapertados
        )
        {
            if (
                direcao ==
                0
            )
            {
                if (frame == 0)
                    texturaAtual =
                        frente1;

                else if (frame == 1)
                    texturaAtual =
                        frente2;

                else
                    texturaAtual =
                        frente3;
            }

            else if (
                direcao ==
                1
            )
            {
                if (frame == 0)
                    texturaAtual =
                        costas1;

                else if (frame == 1)
                    texturaAtual =
                        costas2;

                else
                    texturaAtual =
                        costas3;
            }

            else if (
                direcao ==
                2
            )
            {
                if (frame == 0)
                    texturaAtual =
                        esquerda1;

                else if (frame == 1)
                    texturaAtual =
                        esquerda2;

                else
                    texturaAtual =
                        esquerda3;
            }

            else
            {
                if (frame == 0)
                    texturaAtual =
                        direita1;

                else if (frame == 1)
                    texturaAtual =
                        direita2;

                else
                    texturaAtual =
                        direita3;
            }
        }


        // ======================================================
        // LIMITES
        // ======================================================
        if (x < 15)
            x =
                15;


        if (
            x >
            larguraResolucao -
            texturaAtual.width -
            15
        )
        {
            x =
                larguraResolucao -
                texturaAtual.width -
                15;
        }


        if (y < 80)
            y =
                80;


        if (
            y >
            alturaResolucao -
            texturaAtual.height -
            20
        )
        {
            y =
                alturaResolucao -
                texturaAtual.height -
                20;
        }


        if (
            x >
            larguraResolucao -
            texturaAtual.width
        )
        {
            x =
                larguraResolucao -
                texturaAtual.width;
        }


        if (
            y >
            alturaResolucao -
            texturaAtual.height
        )
        {
            y =
                alturaResolucao -
                texturaAtual.height;
        }


        // ======================================================
        // DESENHO
        // ======================================================
        BeginTextureMode(
            telaVirtual
        );


        ClearBackground(
            RAYWHITE
        );


        int cargoFundo = obterCargo();

        if (cargoFundo < 0 || cargoFundo > 4 || fundosCargo[cargoFundo].id == 0)
            cargoFundo = 0;

        if (cargoFundo != cargoAnterior)
        {
            Rectangle pesAoMudarCargo =
            {
                x + frente1.width * 0.25f,
                y + frente1.height * 0.80f,
                frente1.width * 0.50f,
                frente1.height * 0.18f
            };

            if (colideComCantinho(cargoFundo, pesAoMudarCargo))
            {
                x = 300;
                y = 170;
            }

            cargoAnterior = cargoFundo;
        }

        DrawTexture(
            fundosCargo[cargoFundo],
            0,
            0,
            WHITE
        );


        // ======================================================
        // COLISÃO
        // ======================================================
        Rectangle colisaoMesaBoneco_Boneco =
        {
            x + frente1.width * 0.25f,
            y + frente1.height * 0.80f,
            frente1.width * 0.50f,
            frente1.height * 0.18f
        };

        Rectangle colisaoCadeiras[8] =
        {
   
            {480, 283, 42, 21},
            {619, 283, 42, 21},
            {879, 283, 42, 21},
            {1025, 283, 42, 21},

          
            {481, 581, 47, 55},
            {627, 580, 51, 55},
            {886, 582, 63, 54},
            {1046, 584, 48, 57}
        };



// POSIÇÃO DOS PÉS DO PERSONAGM
float posicaoPes = colisaoMesaBoneco_Boneco.y +
                   colisaoMesaBoneco_Boneco.height;

// QUATRO CONDIÇÕES DE PROFUNIDADE

// Mesas superiores
bool mesaSuperiorAntesPersonagem =
    posicaoPes >= objetos[6].colisao.y;

bool mesaSuperiorDepoisPersonagem =
    posicaoPes < objetos[6].colisao.y;

// Mesas inferiores
bool mesaInferiorAntesPersonagem =
    posicaoPes >= objetos[10].colisao.y;

bool mesaInferiorDepoisPersonagem =
    posicaoPes < objetos[10].colisao.y;


        for (
            int i = 0;
            i < quantidadeObjetos;
            i++
        )
        {

            if (!objetoEstagiarioAtivo(i, cargoFundo))
            {
                continue;
            }

// VERIFICA SE A MESA DEVE SER DESENHADA DEPOIS DO PERSONAGEM

if (i >= 6 && i <= 9 && mesaSuperiorDepoisPersonagem)
{
    continue;
}

if (i >= 10 && i <= 13 && mesaInferiorDepoisPersonagem)
{
    continue;
}
            if (
                CheckCollisionRecs(
                    colisaoMesaBoneco_Boneco,
                    objetos[i].colisao
                )
            )
            {
                mensagem =
                    true;


                x =
                    xGuardado;


                y =
                    yGuardado;


                break;
            }
        }
        if (colideComCantinho(cargoFundo, colisaoMesaBoneco_Boneco))
        {
            mensagem = true;

            x = xGuardado;
            y = yGuardado;
        }

        for (int i = 0; i < 8; i++)
        {
            if (CheckCollisionRecs(
                colisaoMesaBoneco_Boneco,
                colisaoCadeiras[i]
            ))
            {
                mensagem = true;

                x = xGuardado;
                y = yGuardado;

                break;
            }
        }


        // ======================================================
        // INTERAÇÃO
        // ======================================================
        int objetoPerto =
            -1;


        for (
            int i = 0;
            i < quantidadeObjetos &&
            objetoPerto < 0;
            i++
        )
        {
            Rectangle areaInteracao =
            {
                objetos[i].colisao.x -
                30,

                objetos[i].colisao.y -
                30,

                objetos[i].colisao.width +
                60,

                objetos[i].colisao.height +
                60
            };


            if (
                objetos[i].interativo &&
                objetoEstagiarioAtivo(i, cargoFundo) &&
                CheckCollisionRecs(
                    colisaoMesaBoneco_Boneco,
                    areaInteracao
                )
            )
            {
                objetoPerto =
                    i;
            }
        }


        int pecaPerto =
            pecaInterativaPerto(cargoFundo, colisaoMesaBoneco_Boneco);

        bool podeUsarPeca =
            !gameOver && memorando == 0 &&
            !maquinaCafeAberta() && !cartasEstaoAbertas();

        if (pecaPerto == PECA_CAFE)
        {
            if (podeUsarPeca)
                interagirBalcaoCafe(larguraResolucao);
        }
        else if (pecaPerto == PECA_FREEZER)
        {
            if (podeUsarPeca)
            {
                agendarTextoUI(
                    "Pressione E para abrir o freezer",
                    larguraResolucao - 430,
                    66,
                    30,
                    GREEN,
                    false,
                    FONTE_TEXTO
                );

                if (IsKeyPressed(KEY_E))
                    usarFreezer();
            }
        }
        else if (objetoPerto == 1)   // MAQUINA DE CAFE: balcao do cafe
        {
            if (!gameOver && memorando == 0 && !maquinaCafeAberta())
                interagirBalcaoCafe(larguraResolucao);
        }
        else if (
            objetoPerto >=
            0
        )
        {
            definirOrigemCartas(
                objetos[
                    objetoPerto
                ].origemCartas
            );


            definirEstacaoCartas(objetoPerto);

            if (!cartasEstaoAbertas() && memorando == 0)
            {
                agendarTextoUI(
                    estacaoAtualLiberada()
                        ? "Pressione E para realizar a tarefa"
                        : "Sem tarefas aqui. Vá a outra mesa!",
                    larguraResolucao - 400,
                    66,
                    30,
                    estacaoAtualLiberada() ? GREEN : ORANGE,
                    false,
                    FONTE_TEXTO
                );
            }


            bool cartaEstavaAberta =
                cartasEstaoAbertas();


            if (!gameOver && memorando == 0)
            {
                definirTransformacaoTelaCartas(escala, offsetX, offsetY);
                atualizarCartas();
            }


            bool cartaAgoraAberta =
                cartasEstaoAbertas();


            cartaAbertaPararDeAndar =
                cartaEstavaAberta ||
                cartaAgoraAberta;


            // AUDITORIA: se a carta que acabou de ser resolvida disparou
            // ou encerrou a auditoria, mostra o memorando
            int eventoAuditoria =
                consumirEventoAuditoria();

            if (eventoAuditoria != 0)
                memorando =
                    eventoAuditoria;
        }


        // Com o memorando na tela o jogador nao anda nem abre cartas.
        // ENTER ou ESPACO fecham (o E abriria outra carta).
        if (memorando != 0)
        {
            cartaAbertaPararDeAndar =
                true;

            Vector2 mouseMemo = GetMousePosition();
            mouseMemo.x = (mouseMemo.x - offsetX) / escala;
            mouseMemo.y = (mouseMemo.y - offsetY) / escala;

            if (
                (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
                 CheckCollisionPointRec(mouseMemo, retCiente)) ||
                IsKeyPressed(KEY_ENTER) ||
                IsKeyPressed(KEY_SPACE)
            )
            {
                memorando =
                    0;

                cartaAbertaPararDeAndar =
                    false;
            }
        }


// ======================================================
// OBJETOS
// ======================================================

 tempoAnimacaoNpc += GetFrameTime();

        if (tempoAnimacaoNpc >= 0.25f)
        {
            tempoAnimacaoNpc = 0.0f;

            frameNpc++;

            if (frameNpc >= 3)
            {
                frameNpc = 0;
            }
        }

        tempoNovosNpcs += GetFrameTime();
        tempoBolhas += GetFrameTime();

        if (tempoNovosNpcs >= 0.25f)
        {
            tempoNovosNpcs = 0.0f;
            passoNovosNpcs = (passoNovosNpcs + 1) % 16;
        }

        atualizarCantinhos();

        if (cargoFundo == 0)
            DrawTexture(tapete, 112, 330, WHITE);   // tapete no chao, antes dos moveis

        desenharCantinho(cargoFundo, posicaoPes, false);

        for (int i = 0; i < quantidadeObjetos; i++)
        {

            if (!objetoEstagiarioAtivo(i, cargoFundo))
            {
                continue;
            }

            if (i == 17)   // aquario com peixe: desenhado depois do personagem (fica por cima dele)
            {
                continue;
            }

            if (i == 14 || i == 15)
            {
                continue;
            }

            if (desenharMesaNivel(cargoFundo, i, objetos[i].x, objetos[i].y, passoNovosNpcs))
            {
                continue;
            }


            if (i == 2)   // bebedouro com bolhas
            {
                int quadroBolha = 0;
                if (tempoBolhas >= esperaBolhas)
                {
                    quadroBolha = 1 + (int)((tempoBolhas - esperaBolhas) / 0.15f);   // cada quadro dura 0,15s
                    if (quadroBolha > 4)
                    {
                        quadroBolha = 0;
                        tempoBolhas = 0.0f;
                        esperaBolhas = GetRandomValue(20, 50) / 10.0f;   // próxima bolha entre 2 e 5s
                    }
                }
                DrawTexture(bebedouroBolhas[quadroBolha], objetos[2].x, objetos[2].y, WHITE);
            }
            else if (i == 12)   // mulher digitando
            {
                DrawTexture(npcMulher[sequenciaMulher[passoNovosNpcs % 4]],
                            objetos[12].x, objetos[12].y, WHITE);
            }
            else if (i == 7)   // NPC da frente (o PNG dele é deslocado meio pixel da arte pra direita/baixo)
            {
                DrawTextureV(npcFrente[sequenciaFrente[passoNovosNpcs % 16]],
                             (Vector2){objetos[7].x - 2.5f, objetos[7].y - 2.5f}, WHITE);
            }
            else if (i == 13)
            {
                DrawTexture(
                    objetos[13 + frameNpc].textura,
                    objetos[13].x,
                    objetos[13].y,
                    WHITE
                );
            }
            else
            {
                DrawTexture(
                    objetos[i].textura,
                    objetos[i].x,
                    objetos[i].y,
                    WHITE
                );
            }
        }

        desenharAvisoBalcaoCafe();   // MAQUINA DE CAFE: "!" em cima do balcao

        // ======================================================
        // PERSONAGEM
        // ======================================================
        DrawTexture(
            texturaAtual,
            x,
            y,
            WHITE
        );

        desenharCantinho(cargoFundo, posicaoPes, true);

        // aquario com peixe: camada por cima do personagem
        if (objetoEstagiarioAtivo(17, cargoFundo))
        {
            int quadroAquario = (int)(GetTime() / 0.2) % 8;
            DrawTexture(aquarioPeixe[quadroAquario], objetos[17].x, objetos[17].y, WHITE);
        }
    // ======================================================
// MESAS QUE FICAM NA FRENTE DO PERSONAGEM
// ======================================================
 
for (int i = 6; i <= 13; i++)
{
    bool mesaNaFrente =
        (i <= 9 && mesaSuperiorDepoisPersonagem) ||
        (i >= 10 && mesaInferiorDepoisPersonagem);

    if (mesaNaFrente && desenharMesaNivel(cargoFundo, i, objetos[i].x, objetos[i].y, passoNovosNpcs))
    {
        continue;
    }

    // MESAS SUPERIORES
    if (i >= 6 && i <= 9 && mesaSuperiorDepoisPersonagem)
    {
        if (i == 7)   // mesa com o NPC que pisca: mesmo desenho e deslocamento do primeiro laco
        {
            DrawTextureV(
                npcFrente[sequenciaFrente[passoNovosNpcs % 16]],
                (Vector2){objetos[7].x - 2.5f, objetos[7].y - 2.5f},
                WHITE
            );
        }
        else
        {
            DrawTexture(
                objetos[i].textura,
                objetos[i].x,
                objetos[i].y,
                WHITE
            );
        }
    }

    // MESAS INFERIORES
    else if (i >= 10 && i <= 13 && mesaInferiorDepoisPersonagem)
    {
        if (i == 12)
        {
            DrawTexture(
                npcMulher[sequenciaMulher[passoNovosNpcs % 4]],
                objetos[12].x,
                objetos[12].y,
                WHITE
            );
        }
        else if (i == 13)
        {
            DrawTexture(
                objetos[13 + frameNpc].textura,
                objetos[13].x,
                objetos[13].y,
                WHITE
            );
        }
        else
        {
            DrawTexture(
                objetos[i].textura,
                objetos[i].x,
                objetos[i].y,
                WHITE
            );
        }
    }
}


        // ======================================================
        // CARTAS
        // ======================================================
        desenharCartas(
            larguraResolucao,
            alturaResolucao
        );


        if (
            IsKeyPressed(
                KEY_P
            )
        )
        {
            abrirCarta =
                !abrirCarta;
        }


        if (abrirCarta)
        {
            DrawTexture(
                fundoCarta,
                0,
                0,
                WHITE
            );
        }


        // ======================================================
        // HUD - CONFIANÇA
        // ======================================================
        DrawTexture(
            fundoHUD,

            (
                (
                    larguraResolucao -
                    confianca.width
                ) /
                2
            ) -
            confianca.width -
            145,

            privacidade.height *
            0.05,

            WHITE
        );


        DrawTexture(
            confianca,

            (
                (
                    larguraResolucao -
                    confianca.width
                ) /
                2
            ) -
            confianca.width -
            135,

            privacidade.height *
            0.05,

            WHITE
        );


        BeginScissorMode(
            (
                (
                    larguraResolucao -
                    confianca.width
                ) /
                2
            ) -
            confianca.width -
            135,

            privacidade.height *
            0.05,

            confiancaCheia.width *
            (
                1.0f -
                porcentagemConfiancaApagado
            ),

            confiancaCheia.height
        );


        DrawTexture(
            confiancaCheia,

            (
                (
                    larguraResolucao -
                    confianca.width
                ) /
                2
            ) -
            confianca.width -
            135,

            privacidade.height *
            0.05,

            WHITE
        );


        EndScissorMode();


        // ======================================================
        // HUD - VIÉS
        // ======================================================
        DrawTexture(
            fundoHUD,

            (
                (
                    larguraResolucao -
                    vies.width
                ) /
                2
            ) -
            125,

            privacidade.height *
            0.05,

            WHITE
        );


        DrawTexture(
            vies,

            (
                (
                    larguraResolucao -
                    vies.width
                ) /
                2
            ) -
            120,

            privacidade.height *
            0.05,

            WHITE
        );


        BeginScissorMode(
            (
                (
                    larguraResolucao -
                    vies.width
                ) /
                2
            ) -
            120,

            privacidade.height *
            0.05,

            viesCheio.width *
            (
                1.0f -
                porcentagemViesApagado
            ),

            viesCheio.height
        );


        DrawTexture(
            viesCheio,

            (
                (
                    larguraResolucao -
                    vies.width
                ) /
                2
            ) -
            120,

            privacidade.height *
            0.05,

            WHITE
        );


        EndScissorMode();


        // ======================================================
        // HUD - PRIVACIDADE
        // ======================================================
        DrawTexture(
            fundoHUD,

            (
                (
                    larguraResolucao -
                    privacidade.width
                ) /
                2
            ) +
            lucro.width -
            115,

            privacidade.height *
            0.05,

            WHITE
        );


        DrawTexture(
            privacidade,

            (
                (
                    larguraResolucao -
                    privacidade.width
                ) /
                2
            ) +
            lucro.width -
            105,

            privacidade.height *
            0.05,

            WHITE
        );


        BeginScissorMode(
            (
                (
                    larguraResolucao -
                    privacidade.width
                ) /
                2
            ) +
            lucro.width -
            105,

            privacidade.height *
            0.05,

            privacidadeCheia.width *
            (
                1.0f -
                porcentagemPrivacidadeApagado
            ),

            privacidadeCheia.height
        );


        DrawTexture(
            privacidadeCheia,

            (
                (
                    larguraResolucao -
                    privacidade.width
                ) /
                2
            ) +
            lucro.width -
            105,

            privacidade.height *
            0.05,

            WHITE
        );


        EndScissorMode();


        // ======================================================
        // HUD - LUCRO
        // ======================================================
        
        DrawTexture(
            fundoHUD,

            (
                (
                    larguraResolucao -
                    lucro.width
                ) /
                2
            ) +
            2 *
            lucro.width -
            80,

            privacidade.height *
            0.05,

            WHITE
        );

        
        DrawTexture(
            lucro,

            (
                (
                    larguraResolucao -
                    lucro.width
                ) /
                2
            ) +
            2 *
            lucro.width -
            75,

            privacidade.height *
            0.05,

            WHITE
        );


        BeginScissorMode(
            (
                (
                    larguraResolucao -
                    lucro.width
                ) /
                2
            ) +
            2 *
            lucro.width -
            75,

            privacidade.height *
            0.05,

            lucroCheio.width *
            (
                1.0f -
                porcentagemLucroApagado
            ),

            lucroCheio.height
        );

        
        DrawTexture(
            lucroCheio,

            (
                (
                    larguraResolucao -
                    lucro.width
                ) /
                2
            ) +
            2 *
            lucro.width -
            75,

            privacidade.height *
            0.05,

            WHITE
        );


        EndScissorMode();


        texturaAtualGuardada =
            texturaAtual;

        DrawTexture(telaContadorDeDias,larguraResolucao-telaContadorDeDias.width,0,WHITE); //Monitor com a contagem de dias
        {
            // estamos dentro da tela virtual 1280x720: NAO multiplicar por escala
            const char *textoDias = TextFormat("Dia: %.2d", obterDiasSobrevividos());
            int tamanhoDias = 20;
            // a telinha do monitor ocupa x 8..55 e y 8..39 do PNG de 64x50
            int telaX = larguraResolucao - telaContadorDeDias.width + 8 * telaContadorDeDias.width / 64;
            int telaL = 48 * telaContadorDeDias.width / 64;
            int telaY = 8 * telaContadorDeDias.height / 50;
            int telaA = 32 * telaContadorDeDias.height / 50;
            // Press Start 2P: cada letra tem 1 em de largura, entao 13 cabe na telinha
            agendarTextoUI(textoDias,
                           telaX + telaL / 2.0f,
                           telaY + (telaA - 13) / 2.0f,
                           13, (Color){198, 183, 255, 255}, true, FONTE_DESTAQUE);
        }

        // botao de menu (abre a pausa) e botao de som (liga/desliga a musica)
        {
            Vector2 mouseHud = GetMousePosition();
            mouseHud.x = (mouseHud.x - offsetX) / escala;
            mouseHud.y = (mouseHud.y - offsetY) / escala;

            bool hudAtivo = !pausado && !gameOver && !maquinaCafeAberta();

            Rectangle menuDesenho = retanguloAnimadoBotao("hud.menu", retBotaoMenu, hudAtivo && CheckCollisionPointRec(mouseHud, retBotaoMenu));
            Rectangle somDesenho = retanguloAnimadoBotao("hud.som", retBotaoSom, hudAtivo && CheckCollisionPointRec(mouseHud, retBotaoSom));

            DrawTexturePro(texBotaoMenu,
                           (Rectangle){0, 0, texBotaoMenu.width, texBotaoMenu.height},
                           menuDesenho, (Vector2){0, 0}, 0, WHITE);

            Texture2D texSom = somMutadoGlobal ? texBotaoMute : texBotaoSom;
            DrawTexturePro(texSom,
                           (Rectangle){0, 0, texSom.width, texSom.height},
                           somDesenho, (Vector2){0, 0}, 0, WHITE);
        }

        // ======================================================
        // MEMORANDO DA AUDITORIA
        // ======================================================
        if (memorando != 0)
        {
            Texture2D memo =
                (memorando == 1) ?
                telaAuditoria :
                telaAuditoriaFim;

            DrawRectangle(
                0,
                0,
                larguraResolucao,
                alturaResolucao,
                Fade(
                    BLACK,
                    0.65f
                )
            );

            DrawTexture(
                memo,
                (larguraResolucao - memo.width) / 2,
                (alturaResolucao - memo.height) / 2,
                WHITE
            );

            // botao CIENTE (cresce ao passar o mouse, como os outros botoes)
            Vector2 mouseCiente = GetMousePosition();
            mouseCiente.x = (mouseCiente.x - offsetX) / escala;
            mouseCiente.y = (mouseCiente.y - offsetY) / escala;

            Texture2D texCiente = (memorando == 1) ? botaoCienteVermelho : botaoCienteVerde;
            Rectangle cienteDesenho = retanguloAnimadoBotao("auditoria.ciente", retCiente, CheckCollisionPointRec(mouseCiente, retCiente));

            DrawTexturePro(
                texCiente,
                (Rectangle){0, 0, (float)texCiente.width, (float)texCiente.height},
                cienteDesenho,
                (Vector2){0, 0},
                0,
                WHITE
            );
        }

        // MAQUINA DE CAFE: se estiver aberta, desenha por cima de tudo
        atualizarDesenharMaquinaCafe(escala, offsetX, offsetY);

        EndTextureMode();

        


        // ======================================================
        // TELA VIRTUAL -> JANELA
        // ======================================================
        BeginDrawing();


        ClearBackground(
            BLACK
        );


        Rectangle origem =
        {
            0,
            0,

            (float)telaVirtual.texture.width,
            -(float)telaVirtual.texture.height
        };


        Rectangle destino =
        {
            offsetX,
            offsetY,
            larguraFinal,
            alturaFinal
        };


        DrawTexturePro(
            telaVirtual.texture,
            origem,
            destino,
            (Vector2){0, 0},
            0,
            WHITE
        );


        // CARTAS: desenhadas na resolucao real da janela (nitidas),
        // e nao na tela virtual 1280x720 que e esticada
        if (memorando == 0)
            desenharCartasNaJanela(escala, offsetX, offsetY);
        else
            cancelarAnimacoesCartas();   // memorando da auditoria fica por cima de tudo
        desenharTextosNaJanela(escala, offsetX, offsetY);


        // ======================================================
        // GAME OVER
        // ======================================================
        if (
            regraGameOver()
        )
        {
            if (
                k ==
                0
            )
            {
                PlayMusicStream(
                    gameOverSom
                );

                // FINAIS: grava o final que aconteceu (1 a 8) no save
                desbloquearFinal(
                    obterFinalDerrota()
                );

                k =
                    1;
            }


            gameOver =
                true;


            UpdateMusicStream(
                gameOverSom
            );


            DrawRectangle(
                0,
                0,
                GetScreenWidth(),
                GetScreenHeight(),
                Fade(
                    BLACK,
                    0.65f
                )
            );


            DrawTextureEx(
                painelFimExpediente,

                (Vector2){
                    offsetX +
                    190 *
                    escala,

                    offsetY +
                    80 *
                    escala
                },

                0,

                escala,

                WHITE
            );


            // cargo em maiusculas (com acento: o TextToUpper so cobre ASCII)
            char cargoFinal[48];
            snprintf(cargoFinal, sizeof(cargoFinal), "%s", nomeCargoExibicao());

            for (unsigned char *p = (unsigned char *)cargoFinal; *p; p++)
            {
                if (*p >= 'a' && *p <= 'z')
                    *p -= 32;
                else if (*p == 0xC3 && p[1] >= 0xA0 && p[1] <= 0xBE && p[1] != 0xB7)
                {
                    p[1] -= 0x20;
                    p++;
                }
            }

            // VT323: os acentos das maiusculas (ESTAGIÁRIO, LÍDER) ficam legiveis
            float tamanhoCargo = 30 * escala;

            desenharTextoUI(
                cargoFinal,
                offsetX + 347 * escala - medirTextoUI(cargoFinal, tamanhoCargo, FONTE_TEXTO) / 2,
                offsetY + 432 * escala - tamanhoCargo / 2,
                tamanhoCargo,
                (Color){70, 74, 90, 255},
                FONTE_TEXTO
            );


            desenharTextoQuebrado(
                obterMensagemFinal(),

                offsetX +
                520 *
                escala,

                offsetY +
                230 *
                escala,

                530 *
                escala,

                34 *
                escala,

                WHITE
            );


            const char *textoDias =
                TextFormat(
                    "%d",
                    obterDiasSobrevividos()
                );


            float tamanhoDias = 40 * escala;

            desenharTextoUI(
                textoDias,
                offsetX + 735 * escala - medirTextoUI(textoDias, tamanhoDias, FONTE_TEXTO) / 2,
                offsetY + 400 * escala - tamanhoDias / 2,
                tamanhoDias,
                (Color){255, 214, 102, 255},
                FONTE_TEXTO
            );


            Rectangle areaUtil =
            {
                30,
                207,
                447,
                112
            };


            Rectangle retPlayAgain =
            {
                offsetX +
                266 *
                escala,

                offsetY +
                550 *
                escala,

                223.5f *
                escala,

                56 *
                escala
            };


            Rectangle retShare =
            {
                offsetX +
                529 *
                escala,

                offsetY +
                550 *
                escala,

                223.5f *
                escala,

                56 *
                escala
            };


            Rectangle retExit =
            {
                offsetX +
                792 *
                escala,

                offsetY +
                550 *
                escala,

                223.5f *
                escala,

                56 *
                escala
            };


            Vector2 mouse =
                GetMousePosition();


            Rectangle retPlayAgainDesenho = retanguloAnimadoBotao("fim.reiniciar", retPlayAgain, CheckCollisionPointRec(mouse, retPlayAgain));
            Rectangle retShareDesenho = retanguloAnimadoBotao("fim.compartilhar", retShare, CheckCollisionPointRec(mouse, retShare));
            Rectangle retExitDesenho = retanguloAnimadoBotao("fim.sair", retExit, CheckCollisionPointRec(mouse, retExit));


            DrawTexturePro(
                botaoPlayAgain,
                areaUtil,
                retPlayAgainDesenho,
                (Vector2){0, 0},
                0,
                WHITE
            );


            DrawTexturePro(
                botaoShare,
                areaUtil,
                retShareDesenho,
                (Vector2){0, 0},
                0,
                WHITE
            );


            DrawTexturePro(
                botaoExit,
                areaUtil,
                retExitDesenho,
                (Vector2){0, 0},
                0,
                WHITE
            );


            PauseMusicStream(
                trilhaSonora2
            );


            if (
                IsMouseButtonPressed(
                    MOUSE_LEFT_BUTTON
                )
            )
            {
                if (
                    CheckCollisionPointRec(
                        mouse,
                        retShare
                    )
                )
                {
                    rlDrawRenderBatchActive();


                    TakeScreenshot(
                        TextFormat(
                            "else_resultado_%ddias.png",
                            obterDiasSobrevividos()
                        )
                    );
                }


                if (
    CheckCollisionPointRec(
        mouse,
        retExit
    )
)
{
    PlayMusicStream(
        trilhaSonora1
    );

    k = 0;

    tela = -1;

    gameOver = false;

    porcentagemConfiancaApagado = 0.365f;
    porcentagemViesApagado = 0.365f;
    porcentagemPrivacidadeApagado = 0.365f;
    porcentagemLucroApagado = 0.365f;

    x = 300;
    y = 170;

    direcao = 0;
    andando = 0;
    frame = 0;
    tempo = 0;

    cartaAbertaPararDeAndar = false;
    memorando = 0;
    abrirCarta = false;

    iniciarCartas(
        &porcentagemConfiancaApagado,
        &porcentagemViesApagado,
        &porcentagemPrivacidadeApagado,
        &porcentagemLucroApagado
    );
    reiniciarMaquinaCafe();   // MAQUINA DE CAFE
}


                if (
    CheckCollisionPointRec(
        mouse,
        retPlayAgain
    )
)
{
    k = 0;

    PlayMusicStream(
        EscritorioTrilhaSonora
    );

    gameOver = false;

    tela = 1;

    porcentagemConfiancaApagado = 0.365f;
    porcentagemViesApagado = 0.365f;
    porcentagemPrivacidadeApagado = 0.365f;
    porcentagemLucroApagado = 0.365f;

    x = 300;
    y = 170;

    direcao = 0;
    andando = 0;
    frame = 0;
    tempo = 0;

    cartaAbertaPararDeAndar = false;
    memorando = 0;
    abrirCarta = false;

    iniciarCartas(
        &porcentagemConfiancaApagado,
        &porcentagemViesApagado,
        &porcentagemPrivacidadeApagado,
        &porcentagemLucroApagado
    );
    reiniciarMaquinaCafe();   // MAQUINA DE CAFE
}
            }
        // reiniciarPartidaSeNecessario();
        }


        EndDrawing();
    }


    // ==========================================================
    // FINALIZAÇÃO
    // ==========================================================
    descarregarCartas();


    for (
        int i = 0;
        i < quantidadeObjetos;
        i++
    )
    {
        UnloadTexture(
            objetos[i].textura
        );
    }

    // novos NPCs animados e bebedouro
    for (int k = 0; k < 3; k++) UnloadTexture(npcMulher[k]);
    for (int k = 0; k < 2; k++) UnloadTexture(npcFrente[k]);
    for (int k = 0; k < 5; k++) UnloadTexture(bebedouroBolhas[k]);
    for (int k = 0; k < 8; k++) UnloadTexture(aquarioPeixe[k]);
    UnloadTexture(tapete);


    UnloadTexture(costas1);
    UnloadTexture(costas2);
    UnloadTexture(costas3);

    UnloadTexture(frente1);
    UnloadTexture(frente2);
    UnloadTexture(frente3);

    UnloadTexture(direita1);
    UnloadTexture(direita2);
    UnloadTexture(direita3);

    UnloadTexture(esquerda1);
    UnloadTexture(esquerda2);
    UnloadTexture(esquerda3);


    UnloadTexture(botaoJogar);
    UnloadTexture(botaoExit);
    UnloadTexture(botaoPlayAgain);
    UnloadTexture(botaoShare);

    UnloadTexture(botaoContinue);
    UnloadTexture(botaoInstrucoesPausa);
    UnloadTexture(botaoMenuInicial);

    UnloadTexture(telaPausa);
    UnloadTexture(telaInstrucoesPausa);

    UnloadTexture(painelFimExpediente);
    UnloadTexture(botaoVoltarPausa);
    UnloadTexture(telaAuditoria);
    UnloadTexture(telaAuditoriaFim);
    UnloadTexture(botaoCienteVerde);
    UnloadTexture(botaoCienteVermelho);

    UnloadTexture(logoInicio);
    UnloadTexture(fundoCarta);
    UnloadTexture(fundoCeramica);

    for (int i = 1; i < 5; i++)
        UnloadTexture(fundosCargo[i]);

    descarregarMaquinaCafe();   // MAQUINA DE CAFE
    UnloadTexture(texBotaoMenu);
    UnloadTexture(texBotaoSom);
    UnloadTexture(texBotaoMute);

    UnloadTexture(confianca);
    UnloadTexture(lucro);
    UnloadTexture(privacidade);
    UnloadTexture(vies);

    UnloadTexture(confiancaCheia);
    UnloadTexture(lucroCheio);
    UnloadTexture(privacidadeCheia);
    UnloadTexture(viesCheio);

    UnloadTexture(fundoHUD);


    UnloadMusicStream(
        trilhaSonora1
    );

    UnloadMusicStream(
        trilhaSonora2
    );

    UnloadMusicStream(
        EscritorioTrilhaSonora
    );

    UnloadMusicStream(
        gameOverSom
    );


    UnloadSound(
        somDePassos
    );


    UnloadRenderTexture(
        telaVirtual
    );


    CloseAudioDevice();

    CloseWindow();


    return 0;
}
