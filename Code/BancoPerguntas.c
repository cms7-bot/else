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

    float confiancaEsquerda;
    float confiancaDireita;

    float viesEsquerda;
    float viesDireita;

    float privacidadeEsquerda;
    float privacidadeDireita;

    float lucroEsquerda;
    float lucroDireita;

<<<<<<< HEAD
    // Onde a carta aparece: 0 = mesas, 1 = impressora
    int origem;

=======
    // 0 = mesas
    // 1 = impressora
    int origem;
>>>>>>> 18b125a (Atualiza menu de pausa e ajustes gerais)

} Evento;


// ==========================================================
// CONVERSÃO DOS PONTOS PARA O HUD
// ==========================================================
#define PT(pontos) (-(pontos) * 0.0073f)


// ==========================================================
// BANCO DE PERGUNTAS
// ==========================================================
Evento bancoPerguntas[] =
{

    // ======================================================
    // ESTAGIÁRIO
    // ======================================================

<<<<<<< HEAD
{
    // Carta 1 - Viés (contratação)
    .id = 1, .cargo = 0, .origem = 1,
    .titulo = "Triagem de curriculos",
    .texto = "Seu chefe pede pra usar um modelo de triagem de curriculos que ja existe no sistema. Voce percebe que ele filtra quase so candidatos de universidades caras.",
    .esquerda = "Usar assim mesmo, e rapido",
    .direita = "Sinalizar o problema antes de rodar",
    .lucroEsquerda = PT(+5), .viesEsquerda = PT(+5), .confiancaEsquerda = PT(-5),
    .viesDireita = PT(-5), .lucroDireita = PT(-5), .confiancaDireita = PT(+5)
},
{
    // Carta 2 - Alucinação
    .id = 2, .cargo = 0,
    .titulo = "Chatbot inventou um dado",
    .texto = "O chatbot de suporte gerou uma resposta tecnica para um cliente, mas o dado que ele citou nao existe em nenhuma documentacao.",
    .esquerda = "Deixar passar, o cliente nao vai notar",
    .direita = "Corrigir e avisar o cliente do erro",
    .lucroEsquerda = PT(+5), .confiancaEsquerda = PT(-15),
    .lucroDireita = PT(-5), .confiancaDireita = PT(+5)
},
{
    // Carta 3 - Privacidade
    .id = 3, .cargo = 0,
    .titulo = "Historico de navegacao",
    .texto = "Voce tem acesso ao historico de navegacao dos usuarios dentro da empresa \"pra melhorar recomendacoes\". Ninguem pediu autorizacao explicita pra isso.",
    .esquerda = "Usar os dados, todo mundo faz",
    .direita = "Pedir consentimento antes",
    .lucroEsquerda = PT(+5), .privacidadeEsquerda = PT(-15),
    .privacidadeDireita = PT(+5), .lucroDireita = PT(-5)
},
{
    // Carta 5 - Viés (reconhecimento facial)
    .id = 5, .cargo = 0,
    .titulo = "Reconhecimento facial",
    .texto = "Um sistema de reconhecimento facial da empresa erra bem mais em rostos de pele escura. Alguem pede pra \"ajustar o threshold\" so pra esconder o problema nas metricas do relatorio.",
    .esquerda = "Ajustar o threshold como pedido",
    .direita = "Reportar o problema real",
    .viesEsquerda = PT(+15), .confiancaEsquerda = PT(-5),
    .viesDireita = PT(-5), .confiancaDireita = PT(+5)
},
{
    // Carta 6 - Alucinação
    .id = 6, .cargo = 0,
    .titulo = "Estatistica suspeita",
    .texto = "Um relatorio automatico de vendas cita uma estatistica que parece forte demais pra ser real. Voce esta sob pressao pra entregar o relatorio hoje.",
    .esquerda = "Entregar assim, sem checar",
    .direita = "Investigar a fonte antes de enviar",
    .lucroEsquerda = PT(+5), .confiancaEsquerda = PT(-15),
    .confiancaDireita = PT(+5), .lucroDireita = PT(-5)
},
{
    // Carta 7 - Privacidade (versão atual: esquerda = Lucro ++)
    .id = 7, .cargo = 0,
    .titulo = "Dados do marketing",
    .texto = "Marketing pede pra usar dados de usuarios (nome, e-mail, comportamento) pra treinar um novo modelo, sem anonimizar.",
    .esquerda = "Liberar os dados como pedido",
    .direita = "Anonimizar antes de liberar",
    .lucroEsquerda = PT(+15), .privacidadeEsquerda = PT(-15),
    .privacidadeDireita = PT(+5), .lucroDireita = PT(-5)
},
{
    // Carta 9 - Viés (precificação) (versão atual: esquerda = Lucro ++)
    .id = 9, .cargo = 0,
    .titulo = "Precificacao por bairro",
    .texto = "A IA de precificacao esta cobrando mais de usuarios de certos bairros, com base em \"risco de inadimplencia\".",
    .esquerda = "Manter, os numeros fecham",
    .direita = "Revisar o criterio de precificacao",
    .lucroEsquerda = PT(+15), .viesEsquerda = PT(+15),
    .viesDireita = PT(-5), .lucroDireita = PT(-5)
},
{
    // Carta 10 - Alucinação + Dependência (carta final da fase)
    .id = 10, .cargo = 0, .origem = 1,
    .titulo = "Numero inventado",
    .texto = "Um colega vai apresentar pro cliente um numero que a IA gerou, mas parece inventado. Ele ja confia cegamente no sistema e nao quer perder tempo checando.",
    .esquerda = "Deixar ele apresentar assim",
    .direita = "Insistir em checar juntos antes",
    .confiancaEsquerda = PT(-15), .lucroEsquerda = PT(+5),
    .confiancaDireita = PT(+5), .lucroDireita = PT(-5)
},
=======
    {
        .id = 1,
        .cargo = 0,
        .origem = 1,
>>>>>>> 18b125a (Atualiza menu de pausa e ajustes gerais)

        .titulo = "Triagem de curriculos",

        .texto =
            "Seu chefe pede pra usar um modelo de triagem de curriculos "
            "que ja existe no sistema. Voce percebe que ele filtra quase "
            "so candidatos de universidades caras.",

        .esquerda =
            "Usar assim mesmo, e rapido",

        .direita =
            "Sinalizar o problema antes de rodar",

        .lucroEsquerda = PT(+5),
        .viesEsquerda = PT(+5),
        .confiancaEsquerda = PT(-5),

        .viesDireita = PT(-5),
        .lucroDireita = PT(-5),
        .confiancaDireita = PT(+5)
    },

    {
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

        .lucroEsquerda = PT(+5),
        .confiancaEsquerda = PT(-15),

        .lucroDireita = PT(-5),
        .confiancaDireita = PT(+5)
    },

    {
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

        .lucroEsquerda = PT(+5),
        .privacidadeEsquerda = PT(-15),

        .privacidadeDireita = PT(+5),
        .lucroDireita = PT(-5)
    },

    {
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
        .confiancaEsquerda = PT(-5),

        .viesDireita = PT(-5),
        .confiancaDireita = PT(+5)
    },

    {
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

        .lucroEsquerda = PT(+5),
        .confiancaEsquerda = PT(-15),

        .confiancaDireita = PT(+5),
        .lucroDireita = PT(-5)
    },

    {
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

        .privacidadeDireita = PT(+5),
        .lucroDireita = PT(-5)
    },

    {
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

        .viesDireita = PT(-5),
        .lucroDireita = PT(-5)
    },

    {
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
        .lucroEsquerda = PT(+5),

        .confiancaDireita = PT(+5),
        .lucroDireita = PT(-5)
    }

};


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