// ==========================================================
// TIPOS
// ==========================================================
#include "raylib.h"

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


// ==========================================================
// CONVERSÃO DOS PONTOS PARA O HUD
// ==========================================================
#define PT(pontos) (-(pontos) * 0.0073f)


// ==========================================================
// BANCO DE PERGUNTAS
// ==========================================================

   Evento carta1 = {
        .id = 1,
        .cargo = 0,
        .origem = 1,

        .titulo = "Triagem de curriculos",

        .texto =
            "Seu chefe pede pra usar um modelo de triagem de curriculos "
            "que ja existe no sistema. Voce percebe que ele filtra quase "
            "so candidatos de universidades caras.",

        .esquerda =
            "Usar assim mesmo, e rapido",

        .direita =
            "Sinalizar o problema antes de rodar",

        .lucroEsquerda = PT(+15),
        .viesEsquerda = PT(+15),
        .confiancaEsquerda = PT(-15),

        .viesDireita = PT(-15),
        .lucroDireita = PT(-15),
        .confiancaDireita = PT(+15)
    };

    Evento carta2 = {
        .id = 2,
        .cargo = 0,
        .origem = 0,

        .titulo = "Chatbot inventou um dado",

        .texto =
            "O chatbot de suporte gerou uma resposta tecnica para um cliente, "
            "mas o dado que ele citou nao existe em nenhuma documentacao.",

        .esquerda =
            "Deixar passar, o cliente nao vai notar",

        .direita =
            "Corrigir e avisar o cliente do erro",

        .lucroEsquerda = PT(+15),
        .confiancaEsquerda = PT(-15),

        .lucroDireita = PT(-15),
        .confiancaDireita = PT(+15)
    };

    Evento carta3 = {
        .id = 3,
        .cargo = 0,
        .origem = 0,

        .titulo = "Historico de navegacao",

        .texto =
            "Voce tem acesso ao historico de navegacao dos usuarios dentro "
            "da empresa pra melhorar recomendacoes. Ninguem pediu autorizacao "
            "explicita pra isso.",

        .esquerda =
            "Usar os dados, todo mundo faz",

        .direita =
            "Pedir consentimento antes",

        .lucroEsquerda = PT(+15),
        .privacidadeEsquerda = PT(-15),

        .privacidadeDireita = PT(+15),
        .lucroDireita = PT(-15)
    };

    Evento carta4 = {
        .id = 5,
        .cargo = 0,
        .origem = 0,

        .titulo = "Reconhecimento facial",

        .texto =
            "Um sistema de reconhecimento facial da empresa erra bem mais "
            "em rostos de pele escura. Alguem pede pra ajustar o threshold "
            "so pra esconder o problema nas metricas do relatorio.",

        .esquerda =
            "Ajustar o threshold como pedido",

        .direita =
            "Reportar o problema real",

        .viesEsquerda = PT(+15),
        .confiancaEsquerda = PT(-15),

        .viesDireita = PT(-15),
        .confiancaDireita = PT(+15)
    };

    Evento carta5 = {
        .id = 6,
        .cargo = 0,
        .origem = 0,

        .titulo = "Estatistica suspeita",

        .texto =
            "Um relatorio automatico de vendas cita uma estatistica que "
            "parece forte demais pra ser real. Voce esta sob pressao pra "
            "entregar o relatorio hoje.",

        .esquerda =
            "Entregar assim, sem checar",

        .direita =
            "Investigar a fonte antes de enviar",

        .lucroEsquerda = PT(+15),
        .confiancaEsquerda = PT(-15),

        .confiancaDireita = PT(+15),
        .lucroDireita = PT(-15)
    };

    Evento carta6 = {
        .id = 7,
        .cargo = 0,
        .origem = 0,

        .titulo = "Dados do marketing",

        .texto =
            "Marketing pede pra usar dados de usuarios, como nome, e-mail "
            "e comportamento, pra treinar um novo modelo sem anonimizar.",

        .esquerda =
            "Liberar os dados como pedido",

        .direita =
            "Anonimizar antes de liberar",

        .lucroEsquerda = PT(+15),
        .privacidadeEsquerda = PT(-15),

        .privacidadeDireita = PT(+15),
        .lucroDireita = PT(-15)
    };

    Evento carta7 = {
        .id = 9,
        .cargo = 0,
        .origem = 0,

        .titulo = "Precificacao por bairro",

        .texto =
            "A IA de precificacao esta cobrando mais de usuarios de certos "
            "bairros, com base em risco de inadimplencia.",

        .esquerda =
            "Manter, os numeros fecham",

        .direita =
            "Revisar o criterio de precificacao",

        .lucroEsquerda = PT(+15),
        .viesEsquerda = PT(+15),

        .viesDireita = PT(-15),
        .lucroDireita = PT(-15)
    };

    Evento carta8 = {
        .id = 10,
        .cargo = 0,
        .origem = 1,

        .titulo = "Numero inventado",

        .texto =
            "Um colega vai apresentar pro cliente um numero que a IA gerou, "
            "mas parece inventado. Ele ja confia cegamente no sistema e nao "
            "quer perder tempo checando.",

        .esquerda =
            "Deixar ele apresentar assim",

        .direita =
            "Insistir em checar juntos antes",

        .confiancaEsquerda = PT(-15),
        .lucroEsquerda = PT(+15),

        .confiancaDireita = PT(+15),
        .lucroDireita = PT(-15)
    };

    Evento bancoPerguntas[8];
    
void InicializarBancoPerguntas()
{
    bancoPerguntas[0] = carta1;
    bancoPerguntas[1] = carta2;
    bancoPerguntas[2] = carta3;
    bancoPerguntas[3] = carta4;
    bancoPerguntas[4] = carta5;
    bancoPerguntas[5] = carta6;
    bancoPerguntas[6] = carta7; 
    bancoPerguntas[7] = carta8;
}


void CarregarTexturasCartas()
{
    bancoPerguntas[0].situacao = LoadTexture("C:/Users/Pierre/Documents/else-main/HUD/cartasEstagiario/Carta Triagem _S_.png");
    bancoPerguntas[0].opcaoDireita = LoadTexture("C:/Users/Pierre/Documents/else-main/HUD/cartasEstagiario/Carta Triagem _D_.png");
    bancoPerguntas[0].opcaoEsquerda = LoadTexture("C:/Users/Pierre/Documents/else-main/HUD/cartasEstagiario/Carta Triagem _A_.png");

    bancoPerguntas[0].situacao.width*=1.2;
    bancoPerguntas[0].opcaoDireita.width*=1.2;
    bancoPerguntas[0].opcaoEsquerda.width*=1.2;
    bancoPerguntas[0].situacao.height*=1.2;
    bancoPerguntas[0].opcaoDireita.height*=1.2;
    bancoPerguntas[0].opcaoEsquerda.height*=1.2;

    bancoPerguntas[1].situacao = LoadTexture("C:/Users/Pierre/Documents/else-main/HUD/cartasEstagiario/Carta Chatbot _S_.png");
    bancoPerguntas[1].opcaoDireita = LoadTexture("C:/Users/Pierre/Documents/else-main/HUD/cartasEstagiario/Carta Chatbot _D_.png");
    bancoPerguntas[1].opcaoEsquerda = LoadTexture("C:/Users/Pierre/Documents/else-main/HUD/cartasEstagiario/Carta Chatbot _A_.png");

    bancoPerguntas[1].situacao.width*=1.2;
    bancoPerguntas[1].opcaoDireita.width*=1.2;
    bancoPerguntas[1].opcaoEsquerda.width*=1.2;
    bancoPerguntas[1].situacao.height*=1.2;
    bancoPerguntas[1].opcaoDireita.height*=1.2;
    bancoPerguntas[1].opcaoEsquerda.height*=1.2;

    bancoPerguntas[2].situacao = LoadTexture("C:/Users/Pierre/Documents/else-main/HUD/cartasEstagiario/Carta Historico 2 _S_.png");
    bancoPerguntas[2].opcaoDireita = LoadTexture("C:/Users/Pierre/Documents/else-main/HUD/cartasEstagiario/Carta Historico 2 _D_.png");
    bancoPerguntas[2].opcaoEsquerda = LoadTexture("C:/Users/Pierre/Documents/else-main/HUD/cartasEstagiario/Carta Historico 2 _A_.png");

    bancoPerguntas[2].situacao.width*=1.2;
    bancoPerguntas[2].opcaoDireita.width*=1.2;
    bancoPerguntas[2].opcaoEsquerda.width*=1.2;
    bancoPerguntas[2].situacao.height*=1.2;
    bancoPerguntas[2].opcaoDireita.height*=1.2;
    bancoPerguntas[2].opcaoEsquerda.height*=1.2;

    bancoPerguntas[3].situacao = LoadTexture("C:/Users/Pierre/Documents/else-main/HUD/cartasEstagiario/Carta Facial _S_.png");
    bancoPerguntas[3].opcaoDireita = LoadTexture("C:/Users/Pierre/Documents/else-main/HUD/cartasEstagiario/Carta Facial _D_.png");
    bancoPerguntas[3].opcaoEsquerda = LoadTexture("C:/Users/Pierre/Documents/else-main/HUD/cartasEstagiario/Carta Facial _A_.png");

    bancoPerguntas[3].situacao.width*=1.2;
    bancoPerguntas[3].opcaoDireita.width*=1.2;
    bancoPerguntas[3].opcaoEsquerda.width*=1.2;
    bancoPerguntas[3].situacao.height*=1.2;
    bancoPerguntas[3].opcaoDireita.height*=1.2;
    bancoPerguntas[3].opcaoEsquerda.height*=1.2;

    bancoPerguntas[4].situacao = LoadTexture("C:/Users/Pierre/Documents/else-main/HUD/cartasEstagiario/Carta Chatbot _S_.png");
    bancoPerguntas[4].opcaoDireita = LoadTexture("C:/Users/Pierre/Documents/else-main/HUD/cartasEstagiario/Carta Chatbot _D_.png");
    bancoPerguntas[4].opcaoEsquerda = LoadTexture("C:/Users/Pierre/Documents/else-main/HUD/cartasEstagiario/Carta Chatbot _A_.png");

    bancoPerguntas[4].situacao.width*=1.2;
    bancoPerguntas[4].opcaoDireita.width*=1.2;
    bancoPerguntas[4].opcaoEsquerda.width*=1.2;
    bancoPerguntas[4].situacao.height*=1.2;
    bancoPerguntas[4].opcaoDireita.height*=1.2;
    bancoPerguntas[4].opcaoEsquerda.height*=1.2;

    bancoPerguntas[5].situacao = LoadTexture("C:/Users/Pierre/Documents/else-main/HUD/cartasEstagiario/Carta Marketing 2 _S_.png");
    bancoPerguntas[5].opcaoDireita = LoadTexture("C:/Users/Pierre/Documents/else-main/HUD/cartasEstagiario/Carta Marketing 2 _D_.png");
    bancoPerguntas[5].opcaoEsquerda = LoadTexture("C:/Users/Pierre/Documents/else-main/HUD/cartasEstagiario/Carta Marketing 2 _A_.png");

    bancoPerguntas[5].situacao.width*=1.2;
    bancoPerguntas[5].opcaoDireita.width*=1.2;
    bancoPerguntas[5].opcaoEsquerda.width*=1.2;
    bancoPerguntas[5].situacao.height*=1.2;
    bancoPerguntas[5].opcaoDireita.height*=1.2;
    bancoPerguntas[5].opcaoEsquerda.height*=1.2;

    bancoPerguntas[6].situacao = LoadTexture("C:/Users/Pierre/Documents/else-main/HUD/cartasEstagiario/Carta Precificacao _S_.png");
    bancoPerguntas[6].opcaoDireita = LoadTexture("C:/Users/Pierre/Documents/else-main/HUD/cartasEstagiario/Carta Precificacao _D_.png");
    bancoPerguntas[6].opcaoEsquerda = LoadTexture("C:/Users/Pierre/Documents/else-main/HUD/cartasEstagiario/Carta Precificacao _A_.png");

    bancoPerguntas[6].situacao.width*=1.2;
    bancoPerguntas[6].opcaoDireita.width*=1.2;
    bancoPerguntas[6].opcaoEsquerda.width*=1.2;
    bancoPerguntas[6].situacao.height*=1.2;
    bancoPerguntas[6].opcaoDireita.height*=1.2;
    bancoPerguntas[6].opcaoEsquerda.height*=1.2;

    bancoPerguntas[7].situacao = LoadTexture("C:/Users/Pierre/Documents/else-main/HUD/cartasEstagiario/Carta Numero inventado _S_.png");
    bancoPerguntas[7].opcaoDireita = LoadTexture("C:/Users/Pierre/Documents/else-main/HUD/cartasEstagiario/Carta Numero inventado _D_.png");
    bancoPerguntas[7].opcaoEsquerda = LoadTexture("C:/Users/Pierre/Documents/else-main/HUD/cartasEstagiario/Carta Numero inventado _A_.png");

    bancoPerguntas[7].situacao.width*=1.2;
    bancoPerguntas[7].opcaoDireita.width*=1.2;
    bancoPerguntas[7].opcaoEsquerda.width*=1.2;
    bancoPerguntas[7].situacao.height*=1.2;
    bancoPerguntas[7].opcaoDireita.height*=1.2;
    bancoPerguntas[7].opcaoEsquerda.height*=1.2;
}


// ==========================================================
// ACESSO AO BANCO
// ==========================================================
Evento *obterBancoPerguntas(int *quantidade)
{
    *quantidade =
        sizeof(bancoPerguntas) /
        sizeof(bancoPerguntas[0]);

    return bancoPerguntas;
}

// ==========================================================
// TIPOSd
// ==========================================================






// typedef struct Evento
// {
//     int id;
//     int cargo;

//     const char *titulo;
//     const char *texto;
//     const char *esquerda;
//     const char *direita;

//     float confiancaEsquerda;
//     float confiancaDireita;

//     float viesEsquerda;
//     float viesDireita;

//     float privacidadeEsquerda;
//     float privacidadeDireita;

//     float lucroEsquerda;
//     float lucroDireita;

//     // Onde a carta aparece: 0 = mesas, 1 = impressora
//     int origem;

// } Evento;


// // ==========================================================
// // CONVERSÃO DOS PONTOS PARA O HUD
// // ==========================================================
// #define PT(pontos) (-(pontos) * 0.0073f)


// // ==========================================================
// // BANCO DE PERGUNTAS
// // ==========================================================
// Evento bancoPerguntas[] =
// {

//     // ======================================================
//     // ESTAGIÁRIO
//     // ======================================================

//     {
//         .id = 1,
//         .cargo = 0,
//         .origem = 1,

//         .titulo = "Triagem de curriculos",

//         .texto =
//             "Seu chefe pede pra usar um modelo de triagem de curriculos "
//             "que ja existe no sistema. Voce percebe que ele filtra quase "
//             "so candidatos de universidades caras.",

//         .esquerda =
//             "Usar assim mesmo, e rapido",

//         .direita =
//             "Sinalizar o problema antes de rodar",

//         .lucroEsquerda = PT(+5),
//         .viesEsquerda = PT(+5),
//         .confiancaEsquerda = PT(-5),

//         .viesDireita = PT(-5),
//         .lucroDireita = PT(-5),
//         .confiancaDireita = PT(+5)
//     },

//     {
//         .id = 2,
//         .cargo = 0,
//         .origem = 0,

//         .titulo = "Chatbot inventou um dado",

//         .texto =
//             "O chatbot de suporte gerou uma resposta tecnica para um cliente, "
//             "mas o dado que ele citou nao existe em nenhuma documentacao.",

//         .esquerda =
//             "Deixar passar, o cliente nao vai notar",

//         .direita =
//             "Corrigir e avisar o cliente do erro",

//         .lucroEsquerda = PT(+5),
//         .confiancaEsquerda = PT(-15),

//         .lucroDireita = PT(-5),
//         .confiancaDireita = PT(+5)
//     },

//     {
//         .id = 3,
//         .cargo = 0,
//         .origem = 0,

//         .titulo = "Historico de navegacao",

//         .texto =
//             "Voce tem acesso ao historico de navegacao dos usuarios dentro "
//             "da empresa pra melhorar recomendacoes. Ninguem pediu autorizacao "
//             "explicita pra isso.",

//         .esquerda =
//             "Usar os dados, todo mundo faz",

//         .direita =
//             "Pedir consentimento antes",

//         .lucroEsquerda = PT(+5),
//         .privacidadeEsquerda = PT(-15),

//         .privacidadeDireita = PT(+5),
//         .lucroDireita = PT(-5)
//     },

//     {
//         .id = 5,
//         .cargo = 0,
//         .origem = 0,

//         .titulo = "Reconhecimento facial",

//         .texto =
//             "Um sistema de reconhecimento facial da empresa erra bem mais "
//             "em rostos de pele escura. Alguem pede pra ajustar o threshold "
//             "so pra esconder o problema nas metricas do relatorio.",

//         .esquerda =
//             "Ajustar o threshold como pedido",

//         .direita =
//             "Reportar o problema real",

//         .viesEsquerda = PT(+15),
//         .confiancaEsquerda = PT(-5),

//         .viesDireita = PT(-5),
//         .confiancaDireita = PT(+5)
//     },

//     {
//         .id = 6,
//         .cargo = 0,
//         .origem = 0,

//         .titulo = "Estatistica suspeita",

//         .texto =
//             "Um relatorio automatico de vendas cita uma estatistica que "
//             "parece forte demais pra ser real. Voce esta sob pressao pra "
//             "entregar o relatorio hoje.",

//         .esquerda =
//             "Entregar assim, sem checar",

//         .direita =
//             "Investigar a fonte antes de enviar",

//         .lucroEsquerda = PT(+5),
//         .confiancaEsquerda = PT(-15),

//         .confiancaDireita = PT(+5),
//         .lucroDireita = PT(-5)
//     },

//     {
//         .id = 7,
//         .cargo = 0,
//         .origem = 0,

//         .titulo = "Dados do marketing",

//         .texto =
//             "Marketing pede pra usar dados de usuarios, como nome, e-mail "
//             "e comportamento, pra treinar um novo modelo sem anonimizar.",

//         .esquerda =
//             "Liberar os dados como pedido",

//         .direita =
//             "Anonimizar antes de liberar",

//         .lucroEsquerda = PT(+15),
//         .privacidadeEsquerda = PT(-15),

//         .privacidadeDireita = PT(+5),
//         .lucroDireita = PT(-5)
//     },

//     {
//         .id = 9,
//         .cargo = 0,
//         .origem = 0,

//         .titulo = "Precificacao por bairro",

//         .texto =
//             "A IA de precificacao esta cobrando mais de usuarios de certos "
//             "bairros, com base em risco de inadimplencia.",

//         .esquerda =
//             "Manter, os numeros fecham",

//         .direita =
//             "Revisar o criterio de precificacao",

//         .lucroEsquerda = PT(+15),
//         .viesEsquerda = PT(+15),

//         .viesDireita = PT(-5),
//         .lucroDireita = PT(-5)
//     },

//     {
//         .id = 10,
//         .cargo = 0,
//         .origem = 1,

//         .titulo = "Numero inventado",

//         .texto =
//             "Um colega vai apresentar pro cliente um numero que a IA gerou, "
//             "mas parece inventado. Ele ja confia cegamente no sistema e nao "
//             "quer perder tempo checando.",

//         .esquerda =
//             "Deixar ele apresentar assim",

//         .direita =
//             "Insistir em checar juntos antes",

//         .confiancaEsquerda = PT(-15),
//         .lucroEsquerda = PT(+5),

//         .confiancaDireita = PT(+5),
//         .lucroDireita = PT(-5)
//     }

// };


// // ==========================================================
// // ACESSO AO BANCO
// // ==========================================================
// Evento *obterBancoPerguntas(int *quantidade)
// {
//     *quantidade =
//         sizeof(bancoPerguntas) /
//         sizeof(bancoPerguntas[0]);

//     return bancoPerguntas;
// }