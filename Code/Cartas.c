#include "raylib.h"
#include <stdbool.h>
#include <string.h>
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
static int cargo = 0;
static int experiencia = 0;


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
    if (cargo == 1) return "Assistente";
    if (cargo == 2) return "Analista Junior";
    if (cargo == 3) return "Analista Pleno";
    if (cargo == 4) return "Analista Senior";
    if (cargo == 5) return "Supervisor";

    return "Gerente";
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

    if (!partidaTerminou && cartasResolvidasFase == CARTAS_PARA_PROMOCAO){
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
void verificarPromocao()
{
    if (experiencia >= 1700) cargo = 6;
    else if (experiencia >= 1200) cargo = 5;
    else if (experiencia >= 800) cargo = 4;
    else if (experiencia >= 500) cargo = 3;
    else if (experiencia >= 250) cargo = 2;
    else if (experiencia >= 100) cargo = 1;
    else cargo = 0;
}


// ==========================================================
// SORTEIO DE CARTAS
// ==========================================================
void sortearNovaCarta()
{
    reiniciarPartidaSeNecessario();

    int disponiveis[100];
    int quantidadeDisponiveis = 0;

    for (int i = 0; i < quantidadeEventos; i++)
    {
        if (
            banco[i].cargo == cargo &&
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
                banco[i].cargo == cargo &&
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
    teclaAapagada = LoadTexture("C:/Users/Pierre/Documents/else-main/Botoes/teclaAapagada.png");
    teclaSapagada = LoadTexture("C:/Users/Pierre/Documents/else-main/Botoes/teclaSapagada.png");
    teclaDapagada = LoadTexture("C:/Users/Pierre/Documents/else-main/Botoes/teclaDapagada.png");

    teclaAacesa = LoadTexture("C:/Users/Pierre/Documents/else-main/Botoes/teclaAacesa.png");
    teclaSacesa = LoadTexture("C:/Users/Pierre/Documents/else-main/Botoes/teclaSacesa.png");
    teclaDacesa = LoadTexture("C:/Users/Pierre/Documents/else-main/Botoes/teclaDacesa.png");
    somPapel = LoadSound("C:/Users/Pierre/Documents/else-main/EfeitosSonoros/EfeitoSonoroPapel.wav");
    SetSoundVolume(somPapel, 0.03f);
    somConquista = LoadSound("C:/Users/Pierre/Documents/else-main/EfeitosSonoros/EfeitoSonoroConquista.wav");
    somAplausos = LoadSound("C:/Users/Pierre/Documents/else-main/EfeitosSonoros/EfeitoSonoroAplausos.wav");
    texPromocao = LoadTexture("C:/Users/Pierre/Documents/else-main/HUD/Promocao.png");
    texBotaoContinue = LoadTexture("C:/Users/Pierre/Documents/else-main/Botoes/BotaoContinue.png");
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


    verificarPromocao();
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


    verificarPromocao();
    registrarCartaResolvida();
}


// ==========================================================
// ATUALIZAÇÃO
// ==========================================================


static int cliqueNoS = 1;
static int cliqueNoD = 0;
static int cliqueNoA =0;

void atualizarCartas()
{
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
        }

        return;   // trava o resto enquanto a imagem está na tela
    }

    if (!cartaAberta)
    {
        

        
        
        if (IsKeyPressed(KEY_E))
        {
            sortearNovaCarta();

            if (eventoAtual != 0)
                cartaAberta = true;

            cliqueNoS = 1;
            cliqueNoD = 0;
            cliqueNoA = 0;
        }


        return;
    }
    else{
    if(IsKeyPressed(KEY_A)){
        if (cliqueNoA == 0) PlaySound(somPapel);

            cliqueNoA += 1;
            cliqueNoS = 0;
            cliqueNoD = 0;
    }
    if(IsKeyPressed(KEY_D)){
        if (cliqueNoD == 0) PlaySound(somPapel);
            cliqueNoA = 0;
            cliqueNoS = 0;
            cliqueNoD += 1;
    }
    if(IsKeyPressed(KEY_S)){

            cliqueNoA = 0;
            cliqueNoS = 1;
            cliqueNoD = 0;
    }




    if (cliqueNoA==2)
    {
        escolherEsquerda();

        cartaAberta = false;
        eventoAtual = 0;

        return;
    }

    if (cliqueNoD==2)
    {
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


    DrawRectangle(0, 0, larguraResolucao, alturaResolucao, Fade(BLACK, 0.65f));

    DrawTexture(teclaSapagada,(larguraResolucao - teclaSapagada.width) / 2,(+290) + (alturaResolucao - teclaSapagada.height)/2, WHITE);
    DrawTexture(teclaAapagada,(-100) + (larguraResolucao - teclaAapagada.width) / 2,(+290) + (alturaResolucao - teclaAapagada.height)/2, WHITE);
    DrawTexture(teclaDapagada,(+100) + (larguraResolucao - teclaDapagada.width) / 2,(+290) + (alturaResolucao - teclaDapagada.height)/2, WHITE);

    if (cliqueNoS == 1){
    DrawTexture(eventoAtual ->situacao, (larguraResolucao - eventoAtual ->situacao.width) / 2,(-20) + (alturaResolucao - eventoAtual ->situacao.height)/2, WHITE);
    DrawTexture(teclaSacesa,(larguraResolucao - teclaSacesa.width) / 2,(+290) + (alturaResolucao - teclaSacesa.height)/2, WHITE);
    }



    if (cliqueNoA == 1){
    DrawTexture(eventoAtual ->opcaoEsquerda, (-100) + (larguraResolucao - eventoAtual ->opcaoEsquerda.width) / 2,(-20) + (alturaResolucao - eventoAtual ->opcaoEsquerda.height)/2, WHITE);
    DrawTexture(teclaAacesa,(-100) + (larguraResolucao - teclaAacesa.width) / 2,(+290) + (alturaResolucao - teclaAacesa.height)/2, WHITE);

    }

    if (cliqueNoD == 1){
    DrawTexture(eventoAtual ->opcaoDireita, (+100) + (larguraResolucao - eventoAtual ->opcaoDireita.width) / 2,(-20) + (alturaResolucao - eventoAtual ->opcaoDireita.height)/2, WHITE);
    DrawTexture(teclaDacesa,(+100) + (larguraResolucao - teclaDacesa.width) / 2,(+290) + (alturaResolucao - teclaDacesa.height)/2, WHITE);
    }

}
// ==========================================================
// FINALIZAÇÃO
// ==========================================================
void descarregarCartas()
{
    cartaAberta = false;
    eventoAtual = 0;
    banco = 0;
}