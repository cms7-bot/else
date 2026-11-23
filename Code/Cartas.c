#include "raylib.h"
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "Regras.h"


// ==========================================================
// INDICADORES DO HUD (ponteiros para as barras)
// ==========================================================
float *confiancaApagado;
float *viesApagado;
float *privacidadeApagada;
float *lucroApagado;


// ==========================================================
// TIPOS
// ==========================================================
typedef struct Evento
{
    int id;
    int cargo;
    const char *titulo;
    const char *texto;
    const char *esquerda;
    const char *direita;

    Texture2D situacao;
    Texture2D opcaoEsquerda;
    Texture2D opcaoDireita;  

    float confiancaEsquerda;
    float confiancaDireita;
    float viesEsquerda;
    float viesDireita;
    float privacidadeEsquerda;
    float privacidadeDireita;
    float lucroEsquerda;
    float lucroDireita;

    // Onde a carta aparece: 0 = mesas, 1 = impressora
    int origem;

} Evento;

Evento *obterBancoPerguntas(int *quantidade);


// ==========================================================
// BANCO DE EVENTOS / CARTA ATUAL
// ==========================================================
static Evento *banco = 0;
static Evento *eventoAtual = 0;
static int quantidadeEventos = 0;
static int origemCartas = 0;


// ==========================================================
// CARGO / EXPERIÊNCIA
// ==========================================================
// 0 Estagiario, 1 Junior, 2 Pleno, 3 Senior, 4 Tech Lider
static int cargo = 0;
#define CARGO_MAXIMO 4


// ==========================================================
// ESTADO DA CARTA
// ==========================================================
static bool cartaAberta = false;
static Sound somConquista,somAplausos;
static bool mostrarPromocao = false;
#define CARTAS_PARA_PROMOCAO 7


// ==========================================================
// HISTÓRICO DE EVENTOS RECENTES
// ==========================================================
static int historico[8] = {-1,-1,-1,-1,-1,-1,-1,-1};


// ==========================================================
// CARGO
// ==========================================================
const char *nomeCargo()
{
    if (cargo == 0) return "Estagiario";
    if (cargo == 1) return "Junior";
    if (cargo == 2) return "Pleno";
    if (cargo == 3) return "Senior";

    return "Tech Lider";
}

int obterCargo()
{
    return cargo;
}

// nome do cargo para mostrar na tela (com acento). O nomeCargo() acima
// continua sem acento porque a maquina de cafe compara esse texto.
const char *nomeCargoExibicao()
{
    if (cargo == 0) return "Estagiário";
    if (cargo == 1) return "Júnior";
    if (cargo == 2) return "Pleno";
    if (cargo == 3) return "Sênior";

    return "Tech Líder";
}


// ==========================================================
// ESTADO DA CARTA
// ==========================================================
void definirOrigemCartas(int origem)
{
    origemCartas = origem;
}

bool cartasEstaoAbertas()
{
    return cartaAberta || mostrarPromocao;
}


// ==========================================================
// PARTIDA: DIAS, DERROTA E FINAIS
// ==========================================================
static int diasSobrevividos = 0;
static int cartasResolvidasFase = 0;
static bool partidaTerminou = false;


// ==========================================================
// MENSAGEM DO FINAL
// ==========================================================
const char *obterMensagemFinal()
{
    switch (obterFinalDerrota())
    {
        case 1:
            return "Foram tantas respostas erradas que os clientes voltaram a fazer tudo na mão. E juraram nunca mais confiar numa IA.";

        case 2:
            return "Todo mundo confiava tanto na IA que ninguém conferiu o relatório. Era tudo inventado.";

        case 3:
            return "A IA tratou todo mundo exatamente igual. Igualdade perfeita no papel; na vida real, ninguém foi atendido direito.";

        case 4:
            return "Ninguém programou a IA pra discriminar. Ela aprendeu sozinha, com os nossos dados. Os processos chegaram.";

        case 5:
            return "A IA conhecia muito bem os clientes. A internet inteira também, depois do vazamento.";

        case 6:
            return "Os dados estavam perfeitamente protegidos. Tão protegidos que nem a IA conseguia usar.";

        case 7:
            return "Sem dinheiro pra pagar o servidor, a IA foi desligada no meio de uma frase.";

        case 8:
            return "Os acionistas comemoraram até a investigação começar.";
    }

    return "";
}


// ==========================================================
// CONTADORES
// ==========================================================
int obterDiasSobrevividos()
{
    return diasSobrevividos;
}

int obterDiaAtual()
{
    return diasSobrevividos + 1;
}

int obterCartasResolvidasFase()
{
    return cartasResolvidasFase;
}


// ==========================================================
// REGISTRO DE CARTA RESOLVIDA
// ==========================================================
static void registrarCartaResolvida()
{
    cartasResolvidasFase++;

    // auditoria em andamento: a carta tira Confianca (a -> C-)
    aplicarPenalidadeAuditoria();

    if (jogadorPerdeu())
        partidaTerminou = true;
    else
        diasSobrevividos++;

    // disparo e saida da auditoria (so vale se o jogo nao acabou)
    avaliarAuditoria();

    if (!partidaTerminou && cargo < CARGO_MAXIMO && cartasResolvidasFase == CARTAS_PARA_PROMOCAO){
        mostrarPromocao = true;
        PlaySound(somConquista);
        PlaySound(somAplausos);
        *confiancaApagado = 0.365f;
        *viesApagado = 0.365f;
        *privacidadeApagada = 0.365f;
        *lucroApagado = 0.365f;
        }
}


// ==========================================================
// REINÍCIO DA PARTIDA
// ==========================================================
static void reiniciarPartidaSeNecessario()
{
    if (partidaTerminou && !jogadorPerdeu())
    {
        diasSobrevividos = 0;
        cartasResolvidasFase = 0;
        cargo = 0;
        partidaTerminou = false;
        mostrarPromocao = false;
        reiniciarAuditoria();

        for (int i = 0; i < 8; i++)
            historico[i] = -1;
    }
}


// ==========================================================
// HISTÓRICO
// ==========================================================
bool eventoFoiRecente(int id)
{
    for (int i = 0; i < 8; i++)
    {
        if (historico[i] == id)
            return true;
    }

    return false;
}

void adicionarHistorico(int id)
{
    for (int i = 7; i > 0; i--)
        historico[i] = historico[i - 1];

    historico[0] = id;
}


// ==========================================================
// PROMOÇÃO
// ==========================================================
// chamada quando o jogador clica em Continue na tela de promocao
static void promoverCargo()
{
    if (cargo < CARGO_MAXIMO)
        cargo++;

    cartasResolvidasFase = 0;
}


// ==========================================================
// SORTEIO DE CARTAS
// ==========================================================

// Se o cargo atual ainda nao tem cartas para esse lugar (mesa ou
// impressora), usa as cartas do Estagiario para o jogo nao ficar sem carta.
static int cargoDasCartas()
{
    for (int i = 0; i < quantidadeEventos; i++)
    {
        if (banco[i].cargo == cargo && banco[i].origem == origemCartas)
            return cargo;
    }

    return 0;
}

void sortearNovaCarta()
{
    reiniciarPartidaSeNecessario();

    int cargoBusca = cargoDasCartas();
    int disponiveis[100];
    int quantidadeDisponiveis = 0;

    for (int i = 0; i < quantidadeEventos; i++)
    {
        if (
            banco[i].cargo == cargoBusca &&
            banco[i].origem == origemCartas &&
            !eventoFoiRecente(banco[i].id)
        )
        {
            disponiveis[quantidadeDisponiveis] = i;
            quantidadeDisponiveis++;
        }
    }

    if (quantidadeDisponiveis == 0)
    {
        for (int i = 0; i < 8; i++)
            historico[i] = -1;

        for (int i = 0; i < quantidadeEventos; i++)
        {
            if (
                banco[i].cargo == cargoBusca &&
                banco[i].origem == origemCartas
            )
            {
                disponiveis[quantidadeDisponiveis] = i;
                quantidadeDisponiveis++;
            }
        }
    }

    if (quantidadeDisponiveis > 0)
    {
        int sorteado =
            GetRandomValue(0, quantidadeDisponiveis - 1);

        eventoAtual =
            &banco[disponiveis[sorteado]];

        adicionarHistorico(eventoAtual->id);
    }
}


// ==========================================================
// INICIALIZAÇÃO
// ==========================================================
static Texture2D teclaAapagada, teclaSapagada, teclaDapagada;
static Texture2D teclaAacesa,   teclaSacesa,   teclaDacesa;
static Sound somPapel;
static Texture2D texPromocao;
static Texture2D texBotaoContinue;
static Rectangle areaBotaoContinue = {0, 0, 0, 0};
static float escalaBotaoContinue = 1.0f;
#define ESCALA_HOVER_BOTAO 1.1f
#define PROMOCAO_BOTAO_OFFSET_Y 250
#define ESCALA_BASE_BOTAO 0.5f
#define PROMOCAO_BOTAO_OFFSET_Y 270
static Font obterFonteUI(void);   // fonte da interface (definida mais abaixo)

// estado das animacoes (valores iniciais definidos mais abaixo)
static float animEntrada, animTroca, animSaida;
static Texture2D texSaida;
static int dirSaida;

void iniciarCartas(
    float *confianca,
    float *vies,
    float *privacidade,
    float *lucro
)
{
    obterFonteUI();   // carrega a fonte agora, sem travar no meio do jogo
    // zera as animacoes: nada de carta (ou sobra de saida) aparecendo ao reiniciar
    animEntrada = 1.0f;
    animTroca = 1.0f;
    animSaida = 1.0f;
    dirSaida = 0;
    texSaida.id = 0;

    static bool texturasCarregadas = false;
    if(!texturasCarregadas){
    teclaAapagada = LoadTexture("Botoes/teclaAapagada.png");
    teclaSapagada = LoadTexture("Botoes/teclaSapagada.png");
    teclaDapagada = LoadTexture("Botoes/teclaDapagada.png");

    teclaAacesa = LoadTexture("Botoes/teclaAacesa.png");
    teclaSacesa = LoadTexture("Botoes/teclaSacesa.png");
    teclaDacesa = LoadTexture("Botoes/teclaDacesa.png");
    somPapel = LoadSound("EfeitosSonoros/EfeitoSonoroPapel.wav");
    SetSoundVolume(somPapel, 0.03f);
    somConquista = LoadSound("EfeitosSonoros/EfeitoSonoroConquista.wav");
    somAplausos = LoadSound("EfeitosSonoros/EfeitoSonoroAplausos.wav");
    texPromocao = LoadTexture("HUD/Promocao.png");
    texBotaoContinue = LoadTexture("Botoes/BotaoContinue.png");
    texturasCarregadas = true;
    }

    banco = obterBancoPerguntas(&quantidadeEventos);

    confiancaApagado = confianca;

    if (*confiancaApagado <= 0.0f)
        *confiancaApagado = 0.0f;

    if (*confiancaApagado >= 0.73f)
        *confiancaApagado = 0.73f;


    viesApagado = vies;

    if (*viesApagado <= 0.0f)
        *viesApagado = 0.0f;

    if (*viesApagado >= 0.73f)
        *viesApagado = 0.73f;


    privacidadeApagada = privacidade;

    if (*privacidadeApagada <= 0.0f)
        *privacidadeApagada = 0.0f;

    if (*privacidadeApagada >= 0.73f)
        *privacidadeApagada = 0.73f;


    lucroApagado = lucro;

    if (*lucroApagado <= 0.0f)
        *lucroApagado = 0.0f;

    if (*lucroApagado >= 0.73f)
        *lucroApagado = 0.73f;


    eventoAtual = 0;
    cartaAberta = false;

    diasSobrevividos = 0;
    cartasResolvidasFase = 0;
    cargo = 0;
    partidaTerminou = false;
    mostrarPromocao = false;
    escalaBotaoContinue = 1.0f;
    reiniciarAuditoria();

    for (int i = 0; i < 8; i++)
        historico[i] = -1;
}


// ==========================================================
// DIFICULDADE POR NIVEL
// Multiplica os efeitos das cartas do cargo (os valores das cartas seguem a
// escala dos documentos: + = 5, ++ = 15, +++ = 25). Alvo de dificuldade:
// Estagiario facil, Junior medio, Pleno dificil, Senior muito dificil,
// Tech Lider extremo.
// ==========================================================
static const float DIFICULDADE_POR_CARGO[5] = {1.15f, 1.65f, 1.55f, 1.45f, 1.35f};

static float multiplicadorDoNivel(void)
{
    int nivel = eventoAtual->cargo;

    if (nivel < 0 || nivel > 4)
        nivel = 0;

    return DIFICULDADE_POR_CARGO[nivel];
}


// ==========================================================
// ESCOLHA ESQUERDA
// ==========================================================
void escolherEsquerda()
{
    if (eventoAtual == 0)
        return;

    *confiancaApagado += eventoAtual->confiancaEsquerda * multiplicadorDoNivel();

    if (*confiancaApagado <= 0.0f)
        *confiancaApagado = 0.0f;

    if (*confiancaApagado >= 0.73f)
        *confiancaApagado = 0.73f;


    *viesApagado += eventoAtual->viesEsquerda * multiplicadorDoNivel();

    if (*viesApagado <= 0.0f)
        *viesApagado = 0.0f;

    if (*viesApagado >= 0.73f)
        *viesApagado = 0.73f;


    *privacidadeApagada += eventoAtual->privacidadeEsquerda * multiplicadorDoNivel();

    if (*privacidadeApagada <= 0.0f)
        *privacidadeApagada = 0.0f;

    if (*privacidadeApagada >= 0.73f)
        *privacidadeApagada = 0.73f;


    *lucroApagado += eventoAtual->lucroEsquerda * multiplicadorDoNivel();

    if (*lucroApagado <= 0.0f)
        *lucroApagado = 0.0f;

    if (*lucroApagado >= 0.73f)
        *lucroApagado = 0.73f;


    registrarCartaResolvida();
}


// ==========================================================
// ESCOLHA DIREITA
// ==========================================================
void escolherDireita()
{
    if (eventoAtual == 0)
        return;

    *confiancaApagado += eventoAtual->confiancaDireita * multiplicadorDoNivel();
    *viesApagado += eventoAtual->viesDireita * multiplicadorDoNivel();
    *privacidadeApagada += eventoAtual->privacidadeDireita * multiplicadorDoNivel();
    *lucroApagado += eventoAtual->lucroDireita * multiplicadorDoNivel();


    if (*confiancaApagado <= 0.0f)
        *confiancaApagado = 0.0f;

    if (*confiancaApagado >= 0.73f)
        *confiancaApagado = 0.73f;


    if (*viesApagado <= 0.0f)
        *viesApagado = 0.0f;

    if (*viesApagado >= 0.73f)
        *viesApagado = 0.73f;


    if (*privacidadeApagada <= 0.0f)
        *privacidadeApagada = 0.0f;

    if (*privacidadeApagada >= 0.73f)
        *privacidadeApagada = 0.73f;


    if (*lucroApagado <= 0.0f)
        *lucroApagado = 0.0f;

    if (*lucroApagado >= 0.73f)
        *lucroApagado = 0.73f;


    registrarCartaResolvida();
}


// ==========================================================
// ATUALIZAÇÃO
// ==========================================================


static int cliqueNoS = 1;
static int cliqueNoD = 0;
static int cliqueNoA =0;

// ==========================================================
// ANIMACOES DA CARTA (0.0 = comecando, 1.0 = terminada)
// ==========================================================
#define DURACAO_ENTRADA 0.45f
#define DURACAO_TROCA   0.28f
#define DURACAO_SAIDA   0.38f

static float animEntrada = 1.0f;   // carta entra deslizando de baixo
static float animTroca = 1.0f;     // carta gira/desliza ao trocar entre situacao/A/D
static int poseAnterior = 0;       // pose de onde a troca parte: -1 = A, 0 = situacao, +1 = D
static float animSaida = 1.0f;     // carta confirmada voa para o lado
static Texture2D texSaida = {0};
static int dirSaida = 0;           // -1 esquerda, +1 direita

static float limitar01(float t)
{
    if (t < 0.0f) return 0.0f;
    if (t > 1.0f) return 1.0f;
    return t;
}

static float easeOutCubic(float t)
{
    float x = 1.0f - t;
    return 1.0f - x * x * x;
}

static float easeInCubic(float t)
{
    return t * t * t;
}

// pose atual da carta: -1 = opcao A (esquerda), 0 = situacao, +1 = opcao D (direita)
static int poseAtualCarta(void)
{
    if (cliqueNoA == 1) return -1;
    if (cliqueNoD == 1) return 1;
    return 0;
}

static Texture2D texturaDaPose(int pose)
{
    if (pose < 0) return eventoAtual->opcaoEsquerda;
    if (pose > 0) return eventoAtual->opcaoDireita;
    return eventoAtual->situacao;
}

// guarda de onde a carta esta saindo e reinicia a animacao de troca
static void iniciarTrocaPose(void)
{
    poseAnterior = poseAtualCarta();
    animTroca = 0.0f;
}

static void avancarAnimacao(float *anim, float duracao, float dt)
{
    if (*anim < 1.0f)
        *anim = limitar01(*anim + dt / duracao);
}

// cartas pendentes: desenhadas depois que a tela virtual vai para a janela,
// na resolucao real da tela (as imagens das cartas sao 3x, ficam nitidas)
typedef struct CartaPendente
{
    Texture2D tex;
    float cx, cy, escalaX, escalaY, rotacao, alpha;
} CartaPendente;

#define MAX_CARTAS_PENDENTES 4
static CartaPendente cartasPendentes[MAX_CARTAS_PENDENTES];
static int totalCartasPendentes = 0;

// registra a textura centrada em (cx, cy) (coordenadas da tela virtual)
static void desenharCartaAnimada(Texture2D tex, float cx, float cy, float escalaX, float escalaY, float rotacao, float alpha)
{
    if (totalCartasPendentes >= MAX_CARTAS_PENDENTES)
        return;

    cartasPendentes[totalCartasPendentes++] = (CartaPendente){tex, cx, cy, escalaX, escalaY, rotacao, alpha};
}

// descarta o que estava para ser desenhado (ex.: memorando da auditoria na
// tela: a carta que saia nao pode aparecer por cima dele)
void cancelarAnimacoesCartas(void)
{
    totalCartasPendentes = 0;
    animSaida = 1.0f;
}

void desenharCartasNaJanela(float escala, float offsetX, float offsetY)
{
    for (int i = 0; i < totalCartasPendentes; i++)
    {
        CartaPendente *c = &cartasPendentes[i];

        Rectangle origem = {0, 0, (float)c->tex.width, (float)c->tex.height};
        Rectangle destino = {
            offsetX + c->cx * escala,
            offsetY + c->cy * escala,
            c->tex.width * c->escalaX * escala,
            c->tex.height * c->escalaY * escala
        };
        Vector2 pivo = {destino.width / 2.0f, destino.height / 2.0f};

        // sombra suave sob a carta (da profundidade)
        Rectangle sombra = destino;
        sombra.x += 4.0f * escala;
        sombra.y += 12.0f * escala;
        DrawTexturePro(c->tex, origem, sombra, pivo, c->rotacao, Fade(BLACK, limitar01(c->alpha) * 0.30f));

        DrawTexturePro(c->tex, origem, destino, pivo, c->rotacao, Fade(WHITE, limitar01(c->alpha)));
    }

    totalCartasPendentes = 0;
}

void atualizarCartas()
{
    float dt = GetFrameTime();
    avancarAnimacao(&animEntrada, DURACAO_ENTRADA, dt);
    avancarAnimacao(&animTroca, DURACAO_TROCA, dt);
    avancarAnimacao(&animSaida, DURACAO_SAIDA, dt);

    if (mostrarPromocao)
    {
        Vector2 mouse = GetMousePosition();
        bool sobreBotao = CheckCollisionPointRec(mouse, areaBotaoContinue);

        // animação suave: o botão cresce ao passar o mouse e volta ao tamanho normal ao sair
        float alvo = sobreBotao ? ESCALA_HOVER_BOTAO : 1.0f;
        escalaBotaoContinue += (alvo - escalaBotaoContinue) * 0.2f;

        if (sobreBotao && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            mostrarPromocao = false;
            escalaBotaoContinue = 1.0f;
            promoverCargo();
        }

        return;   // trava o resto enquanto a imagem está na tela
    }

    if (!cartaAberta)
    {
        

        
        
        if (IsKeyPressed(KEY_E))
        {
            sortearNovaCarta();

            if (eventoAtual != 0)
            {
                cartaAberta = true;
                animEntrada = 0.0f;
                animTroca = 1.0f;
                poseAnterior = 0;
            }

            cliqueNoS = 1;
            cliqueNoD = 0;
            cliqueNoA = 0;
        }


        return;
    }
    else{
    if(IsKeyPressed(KEY_A)){
        if (cliqueNoA == 0) { PlaySound(somPapel); iniciarTrocaPose(); }

            cliqueNoA += 1;
            cliqueNoS = 0;
            cliqueNoD = 0;
    }
    if(IsKeyPressed(KEY_D)){
        if (cliqueNoD == 0) { PlaySound(somPapel); iniciarTrocaPose(); }
            cliqueNoA = 0;
            cliqueNoS = 0;
            cliqueNoD += 1;
    }
    if(IsKeyPressed(KEY_S)){
            if (cliqueNoS == 0) iniciarTrocaPose();
            cliqueNoA = 0;
            cliqueNoS = 1;
            cliqueNoD = 0;
    }




    if (cliqueNoA==2)
    {
        texSaida = eventoAtual->opcaoEsquerda;
        dirSaida = -1;
        animSaida = 0.0f;

        escolherEsquerda();

        cartaAberta = false;
        eventoAtual = 0;

        return;
    }

    if (cliqueNoD==2)
    {
        texSaida = eventoAtual->opcaoDireita;
        dirSaida = 1;
        animSaida = 0.0f;

        escolherDireita();

        cartaAberta = false;
        eventoAtual = 0;

        return;
    }}
}


// ==========================================================
// FONTE DA INTERFACE (Inter, com acentos e suavizada)
// ==========================================================
static Font fonteUI;
static bool fonteUICarregada = false;

static Font obterFonteUI(void)
{
    if (!fonteUICarregada)
    {
        int glifos[256];
        int total = 0;

        for (int c = 32; c <= 255; c++)
            glifos[total++] = c;

        // aspas curvas, travessoes e reticencias
        int extras[] = {0x2018, 0x2019, 0x201C, 0x201D, 0x2013, 0x2014, 0x2026};
        for (int i = 0; i < 7; i++)
            glifos[total++] = extras[i];

        fonteUI = LoadFontEx("Fontes/Inter-SemiBold.otf", 80, glifos, total);

        if (fonteUI.texture.id == 0)
        {
            fonteUI = GetFontDefault();
        }
        else
        {
            GenTextureMipmaps(&fonteUI.texture);
            SetTextureFilter(fonteUI.texture, TEXTURE_FILTER_TRILINEAR);
        }

        fonteUICarregada = true;
    }

    return fonteUI;
}

float medirTextoUI(const char *texto, float tamanho)
{
    return MeasureTextEx(obterFonteUI(), texto, tamanho, 0.0f).x;
}

void desenharTextoUI(const char *texto, float x, float y, float tamanho, Color cor)
{
    DrawTextEx(obterFonteUI(), texto, (Vector2){x, y}, tamanho, 0.0f, cor);
}


// ==========================================================
// TEXTOS PENDENTES: desenhados na resolucao real da janela
// (a tela virtual 1280x720 e esticada e deixaria o texto borrado)
// ==========================================================
typedef struct TextoPendente
{
    char texto[96];
    float x, y, tamanho;
    Color cor;
    bool centralizado;
} TextoPendente;

#define MAX_TEXTOS_PENDENTES 12
static TextoPendente textosPendentes[MAX_TEXTOS_PENDENTES];
static int totalTextosPendentes = 0;

void limparTextosUI(void)
{
    totalTextosPendentes = 0;
}

// x, y e tamanho em coordenadas da tela virtual (1280x720)
void agendarTextoUI(const char *texto, float x, float y, float tamanho, Color cor, bool centralizado)
{
    if (totalTextosPendentes >= MAX_TEXTOS_PENDENTES)
        return;

    TextoPendente *t = &textosPendentes[totalTextosPendentes++];
    strncpy(t->texto, texto, sizeof(t->texto) - 1);
    t->texto[sizeof(t->texto) - 1] = '\0';
    t->x = x;
    t->y = y;
    t->tamanho = tamanho;
    t->cor = cor;
    t->centralizado = centralizado;
}

void desenharTextosNaJanela(float escala, float offsetX, float offsetY)
{
    for (int i = 0; i < totalTextosPendentes; i++)
    {
        TextoPendente *t = &textosPendentes[i];
        float tam = t->tamanho * escala;
        float x = offsetX + t->x * escala;

        if (t->centralizado)
            x -= medirTextoUI(t->texto, tam) / 2.0f;

        desenharTextoUI(t->texto, x, offsetY + t->y * escala, tam, t->cor);
    }

    totalTextosPendentes = 0;
}


// ==========================================================
// TEXTO QUEBRADO
// ==========================================================
void desenharTextoQuebrado(
    const char *texto,
    int x,
    int y,
    int larguraMaxima,
    int tamanho,
    Color cor
)
{
    char palavra[128] = "";
    char linha[512] = "";

    int indicePalavra = 0;
    float yAtual = y;

    for (int i = 0;; i++)
    {
        char caractere = texto[i];

        if (
            caractere != ' ' &&
            caractere != '\0'
        )
        {
            if (indicePalavra < 127)
            {
                palavra[indicePalavra] = caractere;

                indicePalavra++;

                palavra[indicePalavra] = '\0';
            }
        }
        else
        {
            if (indicePalavra > 0)
            {
                char teste[512] = "";

                if (strlen(linha) > 0)
                {
                    strcpy(teste, linha);
                    strcat(teste, " ");
                    strcat(teste, palavra);
                }
                else
                {
                    strcpy(teste, palavra);
                }

                if (
                    medirTextoUI(
                        teste,
                        tamanho
                    ) > larguraMaxima
                )
                {
                    desenharTextoUI(
                        linha,
                        x,
                        yAtual,
                        tamanho,
                        cor
                    );

                    yAtual += tamanho * 1.4f;

                    strcpy(
                        linha,
                        palavra
                    );
                }
                else
                {
                    strcpy(
                        linha,
                        teste
                    );
                }

                indicePalavra = 0;
                palavra[0] = '\0';
            }

            if (caractere == '\0')
                break;
        }
    }

    if (strlen(linha) > 0)
    {
        desenharTextoUI(
            linha,
            x,
            yAtual,
            tamanho,
            cor
        );
    }
}


// ==========================================================
// DESENHO DAS CARTAS
// ==========================================================





void desenharCartas(
    int larguraResolucao,
    int alturaResolucao
)
{
    totalCartasPendentes = 0;

    // carta confirmada: e arremessada para o lado (acelerando), continua
    // inclinando na mesma direcao e some no fim, como num "swipe"
    if (animSaida < 1.0f && dirSaida != 0 && texSaida.id != 0)
    {
        float e = easeInCubic(animSaida);
        float alphaSaida = (animSaida < 0.6f) ? 1.0f : 1.0f - (animSaida - 0.6f) / 0.4f;

        DrawRectangle(0, 0, larguraResolucao, alturaResolucao, Fade(BLACK, 0.65f * (1.0f - animSaida)));

        desenharCartaAnimada(
            texSaida,
            larguraResolucao / 2.0f + dirSaida * 100 + dirSaida * e * 900.0f,
            alturaResolucao / 2.0f - 20 + e * 50.0f,
            1.0f,
            1.0f,
            dirSaida * e * 14.0f,
            alphaSaida
        );
    }

    if (mostrarPromocao)
    {
        totalCartasPendentes = 0;   // a promocao fica por cima da carta que saia
        animSaida = 1.0f;

        DrawRectangle(0, 0, larguraResolucao, alturaResolucao, Fade(BLACK, 0.65f));
        DrawTexture(
            texPromocao,
            (larguraResolucao - texPromocao.width) / 2,
            (alturaResolucao - texPromocao.height) / 2,
            WHITE
        );

        // área clicável do botão (tamanho base reduzido, centralizado, abaixo da promoção)
        float larguraBotao = texBotaoContinue.width * ESCALA_BASE_BOTAO;
        float alturaBotao = texBotaoContinue.height * ESCALA_BASE_BOTAO;
        float centroX = larguraResolucao / 2.0f;
        float centroY = alturaResolucao / 2.0f + PROMOCAO_BOTAO_OFFSET_Y;

        areaBotaoContinue.x = centroX - larguraBotao / 2.0f;
        areaBotaoContinue.y = centroY - alturaBotao / 2.0f;
        areaBotaoContinue.width = larguraBotao;
        areaBotaoContinue.height = alturaBotao;

        // desenho com escala (cresce a partir do centro ao passar o mouse)
        Rectangle origem = {0, 0, (float)texBotaoContinue.width, (float)texBotaoContinue.height};
        Rectangle destino = {
            centroX,
            centroY,
            larguraBotao * escalaBotaoContinue,
            alturaBotao * escalaBotaoContinue
        };
        Vector2 pivo = {
            destino.width / 2.0f,
            destino.height / 2.0f
        };

        DrawTexturePro(texBotaoContinue, origem, destino, pivo, 0.0f, WHITE);

        return;
    }

    if (
        !cartaAberta ||
        eventoAtual == 0
    )
        return;


    float alphaEntrada = limitar01(animEntrada * 3.0f);
    float eEntrada = easeOutCubic(animEntrada);
    Color corTecla = Fade(WHITE, alphaEntrada);

    DrawRectangle(0, 0, larguraResolucao, alturaResolucao, Fade(BLACK, 0.65f * alphaEntrada));

    // entrada: carta e teclas sobem suavemente e a carta endireita (sem quique)
    float subida = (1.0f - eEntrada) * 150.0f;
    float inclinaEntrada = (1.0f - eEntrada) * -6.0f;
    float escalaEntrada = 0.94f + 0.06f * eEntrada;

    DrawTexture(teclaSapagada,(larguraResolucao - teclaSapagada.width) / 2,(+290) + (alturaResolucao - teclaSapagada.height)/2 + subida, corTecla);
    DrawTexture(teclaAapagada,(-100) + (larguraResolucao - teclaAapagada.width) / 2,(+290) + (alturaResolucao - teclaAapagada.height)/2 + subida, corTecla);
    DrawTexture(teclaDapagada,(+100) + (larguraResolucao - teclaDapagada.width) / 2,(+290) + (alturaResolucao - teclaDapagada.height)/2 + subida, corTecla);

    // flutuacao leve quando a carta esta parada
    float flutuar = (animEntrada >= 1.0f && animTroca >= 1.0f) ? sinf((float)GetTime() * 2.0f) * 2.5f : 0.0f;
    float centroY = alturaResolucao / 2.0f - 20 + subida + flutuar;

    int poseNova = poseAtualCarta();
    float e = easeOutCubic(animTroca);
    float cx = larguraResolucao / 2.0f + (poseAnterior + (poseNova - poseAnterior) * e) * 100.0f;
    float anguloNovo = poseNova * 15.0f;     // inclinacao ja presente nas imagens A/D
    float anguloVelho = poseAnterior * 15.0f;

    if (animTroca < 1.0f && poseAnterior != poseNova)
    {
        // a carta antiga gira e desliza para a nova pose enquanto some;
        // a nova chega girando da pose anterior e se firma
        desenharCartaAnimada(texturaDaPose(poseAnterior), cx, centroY, escalaEntrada, escalaEntrada,
                             inclinaEntrada + (anguloNovo - anguloVelho) * e, alphaEntrada * (1.0f - e));
        desenharCartaAnimada(texturaDaPose(poseNova), cx, centroY, escalaEntrada, escalaEntrada,
                             inclinaEntrada + (anguloVelho - anguloNovo) * (1.0f - e), alphaEntrada * limitar01(e * 1.6f));
    }
    else
    {
        desenharCartaAnimada(texturaDaPose(poseNova), cx, centroY, escalaEntrada, escalaEntrada,
                             inclinaEntrada, alphaEntrada);
    }

    Texture2D teclaAcesa = (poseNova < 0) ? teclaAacesa : (poseNova > 0) ? teclaDacesa : teclaSacesa;

    DrawTexture(teclaAcesa,(poseNova * 100) + (larguraResolucao - teclaAcesa.width) / 2,(+290) + (alturaResolucao - teclaAcesa.height)/2 + subida, corTecla);
}
// ==========================================================
// FINALIZAÇÃO
// ==========================================================
void descarregarCartas()
{
    cartaAberta = false;
    eventoAtual = 0;
    banco = 0;
    animSaida = 1.0f;
    texSaida.id = 0;
}