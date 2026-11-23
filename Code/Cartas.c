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
            return "Foram tantas respostas erradas que os clientes voltaram a fazer tudo na mao. E juraram nunca mais confiar numa IA.";

        case 2:
            return "Todo mundo confiava tanto na IA que ninguem conferiu o relatorio. Era tudo inventado.";

        case 3:
            return "A IA tratou todo mundo exatamente igual. Igualdade perfeita no papel; na vida real, ninguem foi atendido direito.";

        case 4:
            return "Ninguem programou a IA pra discriminar. Ela aprendeu sozinha, com os nossos dados. Os processos chegaram.";

        case 5:
            return "A IA conhecia muito bem os clientes. A internet inteira tambem, depois do vazamento.";

        case 6:
            return "Os dados estavam perfeitamente protegidos. Tao protegidos que nem a IA conseguia usar.";

        case 7:
            return "Sem dinheiro pra pagar o servidor, a IA foi desligada no meio de uma fra";

        case 8:
            return "Os acionistas comemoraram ate a investigacao comecar.";
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
void iniciarCartas(
    float *confianca,
    float *vies,
    float *privacidade,
    float *lucro
)
{
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
// ESCOLHA ESQUERDA
// ==========================================================
void escolherEsquerda()
{
    if (eventoAtual == 0)
        return;

    *confiancaApagado += eventoAtual->confiancaEsquerda;

    if (*confiancaApagado <= 0.0f)
        *confiancaApagado = 0.0f;

    if (*confiancaApagado >= 0.73f)
        *confiancaApagado = 0.73f;


    *viesApagado += eventoAtual->viesEsquerda;

    if (*viesApagado <= 0.0f)
        *viesApagado = 0.0f;

    if (*viesApagado >= 0.73f)
        *viesApagado = 0.73f;


    *privacidadeApagada += eventoAtual->privacidadeEsquerda;

    if (*privacidadeApagada <= 0.0f)
        *privacidadeApagada = 0.0f;

    if (*privacidadeApagada >= 0.73f)
        *privacidadeApagada = 0.73f;


    *lucroApagado += eventoAtual->lucroEsquerda;

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

    *confiancaApagado += eventoAtual->confiancaDireita;
    *viesApagado += eventoAtual->viesDireita;
    *privacidadeApagada += eventoAtual->privacidadeDireita;
    *lucroApagado += eventoAtual->lucroDireita;


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
#define DURACAO_ENTRADA 0.40f
#define DURACAO_TROCA   0.22f
#define DURACAO_SAIDA   0.32f

static float animEntrada = 1.0f;   // carta entra deslizando de baixo
static float animTroca = 1.0f;     // "virar" ao trocar entre situacao/A/D
static float animSaida = 1.0f;     // carta confirmada voa para o lado
static Texture2D texSaida = {0};
static int dirSaida = 0;           // -1 esquerda, +1 direita

static float limitar01(float t)
{
    if (t < 0.0f) return 0.0f;
    if (t > 1.0f) return 1.0f;
    return t;
}

static float easeOutBack(float t)
{
    const float c1 = 1.70158f;
    const float c3 = c1 + 1.0f;
    float x = t - 1.0f;
    return 1.0f + c3 * x * x * x + c1 * x * x;
}

static float easeOutCubic(float t)
{
    float x = 1.0f - t;
    return 1.0f - x * x * x;
}

static void avancarAnimacao(float *anim, float duracao, float dt)
{
    if (*anim < 1.0f)
        *anim = limitar01(*anim + dt / duracao);
}

// desenha a textura centrada em (cx, cy) com escala, rotacao e transparencia
static void desenharCartaAnimada(Texture2D tex, float cx, float cy, float escalaX, float escalaY, float rotacao, float alpha)
{
    Rectangle origem = {0, 0, (float)tex.width, (float)tex.height};
    Rectangle destino = {cx, cy, tex.width * escalaX, tex.height * escalaY};
    Vector2 pivo = {destino.width / 2.0f, destino.height / 2.0f};

    DrawTexturePro(tex, origem, destino, pivo, rotacao, Fade(WHITE, limitar01(alpha)));
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
            }

            cliqueNoS = 1;
            cliqueNoD = 0;
            cliqueNoA = 0;
        }


        return;
    }
    else{
    if(IsKeyPressed(KEY_A)){
        if (cliqueNoA == 0) { PlaySound(somPapel); animTroca = 0.0f; }

            cliqueNoA += 1;
            cliqueNoS = 0;
            cliqueNoD = 0;
    }
    if(IsKeyPressed(KEY_D)){
        if (cliqueNoD == 0) { PlaySound(somPapel); animTroca = 0.0f; }
            cliqueNoA = 0;
            cliqueNoS = 0;
            cliqueNoD += 1;
    }
    if(IsKeyPressed(KEY_S)){
            if (cliqueNoS == 0) animTroca = 0.0f;
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
    int yAtual = y;

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
                    MeasureText(
                        teste,
                        tamanho
                    ) > larguraMaxima
                )
                {
                    DrawText(
                        linha,
                        x,
                        yAtual,
                        tamanho,
                        cor
                    );

                    yAtual += tamanho + 8;

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
        DrawText(
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
    // carta confirmada: voa para o lado girando e some
    if (animSaida < 1.0f && dirSaida != 0 && texSaida.id != 0)
    {
        float e = animSaida * animSaida;

        DrawRectangle(0, 0, larguraResolucao, alturaResolucao, Fade(BLACK, 0.65f * (1.0f - animSaida)));

        desenharCartaAnimada(
            texSaida,
            larguraResolucao / 2.0f + dirSaida * 100 + dirSaida * e * 520.0f,
            alturaResolucao / 2.0f - 20 - e * 70.0f,
            1.0f - 0.2f * e,
            1.0f - 0.2f * e,
            dirSaida * e * 28.0f,
            1.0f - animSaida
        );
    }

    if (mostrarPromocao)
    {
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


    float alphaEntrada = limitar01(animEntrada * 2.5f);
    float easeEntrada = easeOutBack(animEntrada);
    Color corTecla = Fade(WHITE, alphaEntrada);

    DrawRectangle(0, 0, larguraResolucao, alturaResolucao, Fade(BLACK, 0.65f * alphaEntrada));

    // teclas (sobem junto com a carta)
    float subida = (1.0f - easeEntrada) * 160.0f;

    DrawTexture(teclaSapagada,(larguraResolucao - teclaSapagada.width) / 2,(+290) + (alturaResolucao - teclaSapagada.height)/2 + subida, corTecla);
    DrawTexture(teclaAapagada,(-100) + (larguraResolucao - teclaAapagada.width) / 2,(+290) + (alturaResolucao - teclaAapagada.height)/2 + subida, corTecla);
    DrawTexture(teclaDapagada,(+100) + (larguraResolucao - teclaDapagada.width) / 2,(+290) + (alturaResolucao - teclaDapagada.height)/2 + subida, corTecla);

    // carta: entra deslizando de baixo (com leve "quique") e flutua depois
    float flutuar = (animEntrada >= 1.0f) ? sinf((float)GetTime() * 2.0f) * 3.0f : 0.0f;
    float escalaEntrada = 0.85f + 0.15f * easeEntrada;

    // troca de carta: "vira" (achata e abre) com um pequeno salto
    float t = easeOutCubic(animTroca);
    float escalaX = escalaEntrada * (0.05f + 0.95f * t);
    float saltoTroca = (1.0f - t) * -14.0f;

    Texture2D cartaAtual;
    float deslocX = 0;
    Texture2D teclaAcesa;
    float tecX = 0;

    if (cliqueNoA == 1)
    {
        cartaAtual = eventoAtual->opcaoEsquerda; deslocX = -100;
        teclaAcesa = teclaAacesa; tecX = -100;
    }
    else if (cliqueNoD == 1)
    {
        cartaAtual = eventoAtual->opcaoDireita; deslocX = 100;
        teclaAcesa = teclaDacesa; tecX = 100;
    }
    else
    {
        cartaAtual = eventoAtual->situacao;
        teclaAcesa = teclaSacesa;
    }

    desenharCartaAnimada(
        cartaAtual,
        larguraResolucao / 2.0f + deslocX,
        alturaResolucao / 2.0f - 20 + subida + flutuar + saltoTroca,
        escalaX,
        escalaEntrada,
        0.0f,
        alphaEntrada
    );

    DrawTexture(teclaAcesa,(int)tecX + (larguraResolucao - teclaAcesa.width) / 2,(+290) + (alturaResolucao - teclaAcesa.height)/2 + subida, corTecla);
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