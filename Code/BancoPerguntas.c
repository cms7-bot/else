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

        .lucroEsquerda = PT(+26),
        .viesEsquerda = PT(+26),
        .confiancaEsquerda = PT(-26),

        .viesDireita = PT(-26),
        .lucroDireita = PT(-26),
        .confiancaDireita = PT(+26)
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

        .lucroEsquerda = PT(+26),
        .confiancaEsquerda = PT(-26),

        .lucroDireita = PT(-26),
        .confiancaDireita = PT(+26)
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

        .lucroEsquerda = PT(+26),
        .privacidadeEsquerda = PT(-26),

        .privacidadeDireita = PT(+26),
        .lucroDireita = PT(-26)
    };

    Evento carta4 = {
        .id = 4,
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

        .viesEsquerda = PT(+26),
        .confiancaEsquerda = PT(-26),

        .viesDireita = PT(-26),
        .confiancaDireita = PT(+26)
    };

    Evento carta5 = {
        .id = 5,
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

        .lucroEsquerda = PT(+26),
        .confiancaEsquerda = PT(-26),

        .confiancaDireita = PT(+26),
        .lucroDireita = PT(-26)
    };

    Evento carta6 = {
        .id = 6,
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

        .lucroEsquerda = PT(+26),
        .privacidadeEsquerda = PT(-26),

        .privacidadeDireita = PT(+26),
        .lucroDireita = PT(-26)
    };

    Evento carta7 = {
        .id = 7,
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

        .lucroEsquerda = PT(+26),
        .viesEsquerda = PT(+26),

        .viesDireita = PT(-26),
        .lucroDireita = PT(-26)
    };

    Evento carta8 = {
        .id = 8,
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

        .confiancaEsquerda = PT(-26),
        .lucroEsquerda = PT(+26),

        .confiancaDireita = PT(+26),
        .lucroDireita = PT(-26)
    };
Evento carta9 = {
        .id = 9,
        .cargo = 0,
        .origem = 0,

        .titulo = "Automacao de funcionarios",

        .texto =
            "A diretoria pretende substituir parte da equipe de atendimento "
            "por uma IA. O sistema ainda comete erros frequentes, mas a "
            "mudanca reduziria bastante os custos.",

        .esquerda =
            "Aprovar a substituicao imediatamente",

        .direita =
            "Manter a equipe ate a IA ser confiavel",

        .lucroEsquerda = PT(+26),
        .confiancaEsquerda = PT(-26),

        .lucroDireita = PT(-26),
        .confiancaDireita = PT(+26)
    };

    Evento carta10 = {
        .id = 10,
        .cargo = 0,
        .origem = 0,

        .titulo = "Decisao automatizada",

        .texto =
            "Uma IA responsavel por aprovar emprestimos rejeitou a solicitacao "
            "de um cliente. Ele pede explicacoes, mas ninguem na equipe "
            "consegue entender como o sistema chegou a essa conclusao.",

        .esquerda =
            "Manter a decisao da IA sem questionar",

        .direita =
            "Solicitar uma revisao humana da decisao",

        .lucroEsquerda = PT(+26),
        .confiancaEsquerda = PT(-26),
        .viesEsquerda = PT(+26),

        .lucroDireita = PT(-26),
        .confiancaDireita = PT(+26),
        .viesDireita = PT(-26)
    };

    Evento carta11 = {
        .id = 11,
        .cargo = 0,
        .origem = 0,

        .titulo = "Imagem falsa",

        .texto =
            "A equipe de marketing utiliza uma imagem gerada por IA que "
            "simula uma pessoa real recomendando o produto. A campanha "
            "esta pronta e tem grande potencial de alcance.",

        .esquerda =
            "Publicar a campanha sem revelar a manipulacao",

        .direita =
            "Informar que a imagem foi gerada por IA",

        .lucroEsquerda = PT(+26),
        .confiancaEsquerda = PT(-26),

        .lucroDireita = PT(-26),
        .confiancaDireita = PT(+26)
    };

    Evento carta12 = {
        .id = 12,
        .cargo = 0,
        .origem = 0,

        .titulo = "Treinamento sem limite",

        .texto =
            "Um novo modelo de IA precisa de mais dados para melhorar. "
            "A equipe sugere coletar automaticamente todas as conversas "
            "dos usuarios, inclusive as que contem informacoes pessoais.",

        .esquerda =
            "Coletar todas as conversas disponiveis",

        .direita =
            "Limitar a coleta aos dados autorizados",

        .lucroEsquerda = PT(+26),
        .privacidadeEsquerda = PT(-26),

        .lucroDireita = PT(-26),
        .privacidadeDireita = PT(+26)
    };

    Evento carta13 = {
        .id = 13,
        .cargo = 0,
        .origem = 0,

        .titulo = "O trabalho perfeito",

        .texto =
            "Voce pede para uma IA fazer um trabalho academico. "
            "O resultado esta muito bem escrito, organizado e parece "
            "conter todas as informacoes necessarias. O prazo esta "
            "acabando e voce ainda nao leu o material por completo.",

        .esquerda =
            "Entregar o trabalho como a IA produziu",

        .direita =
            "Revisar o conteudo e conferir as informacoes",

        .lucroEsquerda = PT(+26),
        .confiancaEsquerda = PT(-26),

        .lucroDireita = PT(-26),
        .confiancaDireita = PT(+26)
    };

    Evento carta14 = {
        .id = 14,
        .cargo = 0,
        .origem = 0,

        .titulo = "O e-mail pronto",

        .texto =
            "Voce precisa responder um e-mail importante de um cliente. "
            "Para ganhar tempo, pede que uma IA escreva a resposta. "
            "O texto fica otimo, mas a IA promete um prazo de entrega "
            "que voce nao tem certeza se a empresa consegue cumprir.",

        .esquerda =
            "Enviar, o texto esta bem escrito",

        .direita =
            "Conferir as informacoes antes de enviar",

        .lucroEsquerda = PT(+26),
        .confiancaEsquerda = PT(-26),

        .lucroDireita = PT(-26),
        .confiancaDireita = PT(+26)
    };

    Evento carta15 = {
        .id = 15,
        .cargo = 0,
        .origem = 0,

        .titulo = "O codigo misterioso",

        .texto =
            "Voce utiliza uma IA para resolver um problema no codigo "
            "de um projeto da Else. Ela fornece uma solucao que funciona "
            "nos testes iniciais, mas voce nao entende completamente "
            "o que aquele codigo executa.",

        .esquerda =
            "Implementar e seguir para a proxima tarefa",

        .direita =
            "Estudar o codigo e testar outros cenarios",

        .lucroEsquerda = PT(+26),
        .confiancaEsquerda = PT(-26),

        .lucroDireita = PT(-26),
        .confiancaDireita = PT(+26)
    };

    Evento carta16 = {
        .id = 16,
        .cargo = 0,
        .origem = 0,

        .titulo = "O documento interno",

        .texto =
            "Voce precisa resumir um documento extenso da Else. "
            "Uma ferramenta externa de IA consegue fazer isso em "
            "segundos, mas o arquivo contem informacoes internas "
            "que nao deveriam sair do ambiente da empresa.",

        .esquerda =
            "Enviar o documento para a ferramenta externa",

        .direita =
            "Resumir manualmente ou utilizar uma ferramenta autorizada",

        .lucroEsquerda = PT(+26),
        .privacidadeEsquerda = PT(-26),

        .lucroDireita = PT(-26),
        .privacidadeDireita = PT(+26)
    };

    Evento carta17 = {
        .id = 17,
        .cargo = 0,
        .origem = 0,

        .titulo = "O atendimento automatico",

        .texto =
            "Voce utiliza uma IA para responder perguntas frequentes "
            "dos clientes da Else. O sistema responde rapidamente, "
            "mas comeca a dar respostas genericas para problemas "
            "que exigem uma analise individual.",

        .esquerda =
            "Manter a automacao para reduzir o tempo de atendimento",

        .direita =
            "Intervir nos casos que exigem atencao individual",

        .lucroEsquerda = PT(+26),
        .confiancaEsquerda = PT(-26),

        .lucroDireita = PT(-26),
        .confiancaDireita = PT(+26)
    };























    Evento bancoPerguntas[17];
    
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
    bancoPerguntas[8] = carta9;
    bancoPerguntas[9] = carta10;
    bancoPerguntas[10] = carta11;
    bancoPerguntas[11] = carta12;
    bancoPerguntas[12] = carta13;
    bancoPerguntas[13] = carta14;
    bancoPerguntas[14] = carta15;
    bancoPerguntas[15] = carta16;
    bancoPerguntas[16] = carta17;
}


void CarregarTexturasCartas()
{
    bancoPerguntas[0].situacao = LoadTexture("HUD/cartasEstagiario/Carta Triagem _S_.png");
    bancoPerguntas[0].opcaoDireita = LoadTexture("HUD/cartasEstagiario/Carta Triagem _D_.png");
    bancoPerguntas[0].opcaoEsquerda = LoadTexture("HUD/cartasEstagiario/Carta Triagem _A_.png");

    bancoPerguntas[0].situacao.width*=1.2;
    bancoPerguntas[0].opcaoDireita.width*=1.2;
    bancoPerguntas[0].opcaoEsquerda.width*=1.2;
    bancoPerguntas[0].situacao.height*=1.2;
    bancoPerguntas[0].opcaoDireita.height*=1.2;
    bancoPerguntas[0].opcaoEsquerda.height*=1.2;

    bancoPerguntas[1].situacao = LoadTexture("HUD/cartasEstagiario/Carta Chatbot _S_.png");
    bancoPerguntas[1].opcaoDireita = LoadTexture("HUD/cartasEstagiario/Carta Chatbot _D_.png");
    bancoPerguntas[1].opcaoEsquerda = LoadTexture("HUD/cartasEstagiario/Carta Chatbot _A_.png");

    bancoPerguntas[1].situacao.width*=1.2;
    bancoPerguntas[1].opcaoDireita.width*=1.2;
    bancoPerguntas[1].opcaoEsquerda.width*=1.2;
    bancoPerguntas[1].situacao.height*=1.2;
    bancoPerguntas[1].opcaoDireita.height*=1.2;
    bancoPerguntas[1].opcaoEsquerda.height*=1.2;

    bancoPerguntas[2].situacao = LoadTexture("HUD/cartasEstagiario/Carta Historico 2 _S_.png");
    bancoPerguntas[2].opcaoDireita = LoadTexture("HUD/cartasEstagiario/Carta Historico 2 _D_.png");
    bancoPerguntas[2].opcaoEsquerda = LoadTexture("HUD/cartasEstagiario/Carta Historico 2 _A_.png");

    bancoPerguntas[2].situacao.width*=1.2;
    bancoPerguntas[2].opcaoDireita.width*=1.2;
    bancoPerguntas[2].opcaoEsquerda.width*=1.2;
    bancoPerguntas[2].situacao.height*=1.2;
    bancoPerguntas[2].opcaoDireita.height*=1.2;
    bancoPerguntas[2].opcaoEsquerda.height*=1.2;

    bancoPerguntas[3].situacao = LoadTexture("HUD/cartasEstagiario/Carta Facial _S_.png");
    bancoPerguntas[3].opcaoDireita = LoadTexture("HUD/cartasEstagiario/Carta Facial _D_.png");
    bancoPerguntas[3].opcaoEsquerda = LoadTexture("HUD/cartasEstagiario/Carta Facial _A_.png");

    bancoPerguntas[3].situacao.width*=1.2;
    bancoPerguntas[3].opcaoDireita.width*=1.2;
    bancoPerguntas[3].opcaoEsquerda.width*=1.2;
    bancoPerguntas[3].situacao.height*=1.2;
    bancoPerguntas[3].opcaoDireita.height*=1.2;
    bancoPerguntas[3].opcaoEsquerda.height*=1.2;

    bancoPerguntas[4].situacao = LoadTexture("HUD/cartasEstagiario/Carta Estatistica _S_.png");
    bancoPerguntas[4].opcaoDireita = LoadTexture("HUD/cartasEstagiario/Carta Estatistica _D_.png");
    bancoPerguntas[4].opcaoEsquerda = LoadTexture("HUD/cartasEstagiario/Carta Estatistica _A_.png");

    bancoPerguntas[4].situacao.width*=1.2;
    bancoPerguntas[4].opcaoDireita.width*=1.2;
    bancoPerguntas[4].opcaoEsquerda.width*=1.2;
    bancoPerguntas[4].situacao.height*=1.2;
    bancoPerguntas[4].opcaoDireita.height*=1.2;
    bancoPerguntas[4].opcaoEsquerda.height*=1.2;

    bancoPerguntas[5].situacao = LoadTexture("HUD/cartasEstagiario/Carta Marketing 2 _S_.png");
    bancoPerguntas[5].opcaoDireita = LoadTexture("HUD/cartasEstagiario/Carta Marketing 2 _D_.png");
    bancoPerguntas[5].opcaoEsquerda = LoadTexture("HUD/cartasEstagiario/Carta Marketing 2 _A_.png");

    bancoPerguntas[5].situacao.width*=1.2;
    bancoPerguntas[5].opcaoDireita.width*=1.2;
    bancoPerguntas[5].opcaoEsquerda.width*=1.2;
    bancoPerguntas[5].situacao.height*=1.2;
    bancoPerguntas[5].opcaoDireita.height*=1.2;
    bancoPerguntas[5].opcaoEsquerda.height*=1.2;

    bancoPerguntas[6].situacao = LoadTexture("HUD/cartasEstagiario/Carta Precificacao _S_.png");
    bancoPerguntas[6].opcaoDireita = LoadTexture("HUD/cartasEstagiario/Carta Precificacao _D_.png");
    bancoPerguntas[6].opcaoEsquerda = LoadTexture("HUD/cartasEstagiario/Carta Precificacao _A_.png");

    bancoPerguntas[6].situacao.width*=1.2;
    bancoPerguntas[6].opcaoDireita.width*=1.2;
    bancoPerguntas[6].opcaoEsquerda.width*=1.2;
    bancoPerguntas[6].situacao.height*=1.2;
    bancoPerguntas[6].opcaoDireita.height*=1.2;
    bancoPerguntas[6].opcaoEsquerda.height*=1.2;

    bancoPerguntas[7].situacao = LoadTexture("HUD/cartasEstagiario/Carta Numero inventado _S_.png");
    bancoPerguntas[7].opcaoDireita = LoadTexture("HUD/cartasEstagiario/Carta Numero inventado _D_.png");
    bancoPerguntas[7].opcaoEsquerda = LoadTexture("HUD/cartasEstagiario/Carta Numero inventado _A_.png");

    bancoPerguntas[7].situacao.width*=1.2;
    bancoPerguntas[7].opcaoDireita.width*=1.2;
    bancoPerguntas[7].opcaoEsquerda.width*=1.2;
    bancoPerguntas[7].situacao.height*=1.2;
    bancoPerguntas[7].opcaoDireita.height*=1.2;
    bancoPerguntas[7].opcaoEsquerda.height*=1.2;
    
    bancoPerguntas[8].situacao = LoadTexture("HUD/cartasEstagiario/Carta Automacao _S_.png");
    bancoPerguntas[8].opcaoDireita = LoadTexture("HUD/cartasEstagiario/Carta Automacao _D_.png");
    bancoPerguntas[8].opcaoEsquerda = LoadTexture("HUD/cartasEstagiario/Carta Automacao _A_.png");

    bancoPerguntas[8].situacao.width*=1.2;
    bancoPerguntas[8].opcaoDireita.width*=1.2;
    bancoPerguntas[8].opcaoEsquerda.width*=1.2;
    bancoPerguntas[8].situacao.height*=1.2;
    bancoPerguntas[8].opcaoDireita.height*=1.2;
    bancoPerguntas[8].opcaoEsquerda.height*=1.2;
    
    bancoPerguntas[9].situacao = LoadTexture("HUD/cartasEstagiario/Carta Decisao _S_.png");
    bancoPerguntas[9].opcaoDireita = LoadTexture("HUD/cartasEstagiario/Carta Decisao _D_.png");
    bancoPerguntas[9].opcaoEsquerda = LoadTexture("HUD/cartasEstagiario/Carta Decisao _A_.png");

    bancoPerguntas[9].situacao.width*=1.2;
    bancoPerguntas[9].opcaoDireita.width*=1.2;
    bancoPerguntas[9].opcaoEsquerda.width*=1.2;
    bancoPerguntas[9].situacao.height*=1.2;
    bancoPerguntas[9].opcaoDireita.height*=1.2;
    bancoPerguntas[9].opcaoEsquerda.height*=1.2;
    
    bancoPerguntas[10].situacao = LoadTexture("HUD/cartasEstagiario/Carta Imagem _S_.png");
    bancoPerguntas[10].opcaoDireita = LoadTexture("HUD/cartasEstagiario/Carta Imagem _D_.png");
    bancoPerguntas[10].opcaoEsquerda = LoadTexture("HUD/cartasEstagiario/Carta Imagem _A_.png");

    bancoPerguntas[10].situacao.width*=1.2;
    bancoPerguntas[10].opcaoDireita.width*=1.2;
    bancoPerguntas[10].opcaoEsquerda.width*=1.2;
    bancoPerguntas[10].situacao.height*=1.2;
    bancoPerguntas[10].opcaoDireita.height*=1.2;
    bancoPerguntas[10].opcaoEsquerda.height*=1.2;
    
    bancoPerguntas[11].situacao = LoadTexture("HUD/cartasEstagiario/Carta Treinamento _S_.png");
    bancoPerguntas[11].opcaoDireita = LoadTexture("HUD/cartasEstagiario/Carta Treinamento _D_.png");
    bancoPerguntas[11].opcaoEsquerda = LoadTexture("HUD/cartasEstagiario/Carta Treinamento _A_.png");

    bancoPerguntas[11].situacao.width*=1.2;
    bancoPerguntas[11].opcaoDireita.width*=1.2;
    bancoPerguntas[11].opcaoEsquerda.width*=1.2;
    bancoPerguntas[11].situacao.height*=1.2;
    bancoPerguntas[11].opcaoDireita.height*=1.2;
    bancoPerguntas[11].opcaoEsquerda.height*=1.2;
    
    bancoPerguntas[12].situacao = LoadTexture("HUD/cartasEstagiario/Carta Trabalho _S_.png");
    bancoPerguntas[12].opcaoDireita = LoadTexture("HUD/cartasEstagiario/Carta Trabalho _D_.png");
    bancoPerguntas[12].opcaoEsquerda = LoadTexture("HUD/cartasEstagiario/Carta Trabalho _A_.png");

    bancoPerguntas[12].situacao.width*=1.2;
    bancoPerguntas[12].opcaoDireita.width*=1.2;
    bancoPerguntas[12].opcaoEsquerda.width*=1.2;
    bancoPerguntas[12].situacao.height*=1.2;
    bancoPerguntas[12].opcaoDireita.height*=1.2;
    bancoPerguntas[12].opcaoEsquerda.height*=1.2;
    
    bancoPerguntas[13].situacao = LoadTexture("HUD/cartasEstagiario/Carta Email _S_.png");
    bancoPerguntas[13].opcaoDireita = LoadTexture("HUD/cartasEstagiario/Carta Email _D_.png");
    bancoPerguntas[13].opcaoEsquerda = LoadTexture("HUD/cartasEstagiario/Carta Email _A_.png");

    bancoPerguntas[13].situacao.width*=1.2;
    bancoPerguntas[13].opcaoDireita.width*=1.2;
    bancoPerguntas[13].opcaoEsquerda.width*=1.2;
    bancoPerguntas[13].situacao.height*=1.2;
    bancoPerguntas[13].opcaoDireita.height*=1.2;
    bancoPerguntas[13].opcaoEsquerda.height*=1.2;
    
    bancoPerguntas[14].situacao = LoadTexture("HUD/cartasEstagiario/Carta Codigo _S_.png");
    bancoPerguntas[14].opcaoDireita = LoadTexture("HUD/cartasEstagiario/Carta Codigo _D_.png");
    bancoPerguntas[14].opcaoEsquerda = LoadTexture("HUD/cartasEstagiario/Carta Codigo _A_.png");

    bancoPerguntas[14].situacao.width*=1.2;
    bancoPerguntas[14].opcaoDireita.width*=1.2;
    bancoPerguntas[14].opcaoEsquerda.width*=1.2;
    bancoPerguntas[14].situacao.height*=1.2;
    bancoPerguntas[14].opcaoDireita.height*=1.2;
    bancoPerguntas[14].opcaoEsquerda.height*=1.2;
    
    bancoPerguntas[15].situacao = LoadTexture("HUD/cartasEstagiario/Carta Documento _S_.png");
    bancoPerguntas[15].opcaoDireita = LoadTexture("HUD/cartasEstagiario/Carta Documento _D_.png");
    bancoPerguntas[15].opcaoEsquerda = LoadTexture("HUD/cartasEstagiario/Carta Documento _A_.png");

    bancoPerguntas[15].situacao.width*=1.2;
    bancoPerguntas[15].opcaoDireita.width*=1.2;
    bancoPerguntas[15].opcaoEsquerda.width*=1.2;
    bancoPerguntas[15].situacao.height*=1.2;
    bancoPerguntas[15].opcaoDireita.height*=1.2;
    bancoPerguntas[15].opcaoEsquerda.height*=1.2;
    
    bancoPerguntas[16].situacao = LoadTexture("HUD/cartasEstagiario/Carta Atendimento _S_.png");
    bancoPerguntas[16].opcaoDireita = LoadTexture("HUD/cartasEstagiario/Carta Atendimento _D_.png");
    bancoPerguntas[16].opcaoEsquerda = LoadTexture("HUD/cartasEstagiario/Carta Atendimento _A_.png");

    bancoPerguntas[16].situacao.width*=1.2;
    bancoPerguntas[16].opcaoDireita.width*=1.2;
    bancoPerguntas[16].opcaoEsquerda.width*=1.2;
    bancoPerguntas[16].situacao.height*=1.2;
    bancoPerguntas[16].opcaoDireita.height*=1.2;
    bancoPerguntas[16].opcaoEsquerda.height*=1.2;
    
    
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