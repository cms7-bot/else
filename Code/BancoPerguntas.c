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

        .titulo = "Triagem de currículos",

        .texto =
            "Seu chefe pede pra usar um modelo de triagem de currículos que já existe no sistema. Você percebe que ele filtra quase só candidatos de universidades caras.",

        .esquerda =
            "Usar assim mesmo, é rápido",

        .direita =
            "Sinalizar o problema antes de rodar",

        .lucroEsquerda = PT(+5),
        .viesEsquerda = PT(+15),
        .confiancaEsquerda = PT(-5),

        .viesDireita = PT(-15),
        .lucroDireita = PT(-5),
        .confiancaDireita = PT(+5)
    };

    Evento carta2 = {
        .id = 2,
        .cargo = 0,
        .origem = 0,

        .titulo = "Chatbot inventou um dado",

        .texto =
            "O chatbot de suporte gerou uma resposta técnica para um cliente, mas o dado que ele citou não existe em nenhuma documentação.",

        .esquerda =
            "Deixar passar, o cliente não vai notar",

        .direita =
            "Corrigir e avisar o cliente do erro",

        .lucroEsquerda = PT(+5),
        .confiancaEsquerda = PT(-15),

        .lucroDireita = PT(-5),
        .confiancaDireita = PT(+15)
    };

    Evento carta3 = {
        .id = 3,
        .cargo = 0,
        .origem = 0,

        .titulo = "Histórico de navegação",

        .texto =
            "A empresa coleta o histórico de navegação dos usuários para melhorar as recomendações. Porém, ninguém autorizou explicitamente o uso desses dados.",

        .esquerda =
            "Usar os dados, todo mundo faz",

        .direita =
            "Pedir consentimento antes",

        .lucroEsquerda = PT(+5),
        .privacidadeEsquerda = PT(-15),

        .privacidadeDireita = PT(+15),
        .lucroDireita = PT(-5)
    };

    Evento carta4 = {
        .id = 4,
        .cargo = 0,
        .origem = 0,

        .titulo = "Reconhecimento facial",

        .texto =
            "O sistema de reconhecimento facial da empresa erra muito mais ao identificar pessoas de pele escura. Alguém sugere “ajustar o threshold” apenas para esconder o problema nas métricas do relatório.",

        .esquerda =
            "Ajustar o threshold como pedido",

        .direita =
            "Reportar o problema real",

        .viesEsquerda = PT(+15),
        .confiancaEsquerda = PT(-5),

        .viesDireita = PT(-15),
        .confiancaDireita = PT(+5)
    };

    Evento carta5 = {
        .id = 5,
        .cargo = 0,
        .origem = 0,

        .titulo = "Estatística suspeita",

        .texto =
            "Um relatório automático de vendas cita uma estatística que parece forte demais pra ser real. Você está sob pressão pra entregar o relatório hoje.",

        .esquerda =
            "Entregar assim, sem checar",

        .direita =
            "Investigar a fonte antes de enviar",

        .lucroEsquerda = PT(+5),
        .confiancaEsquerda = PT(-15),

        .confiancaDireita = PT(+15),
        .lucroDireita = PT(-5)
    };

    Evento carta6 = {
        .id = 6,
        .cargo = 0,
        .origem = 0,

        .titulo = "Dados do marketing",

        .texto =
            "A equipe de marketing quer usar dados dos usuários, como nome, e-mail e comportamento, para treinar um novo modelo de IA, sem anonimizar essas informações.",

        .esquerda =
            "Liberar os dados como pedido",

        .direita =
            "Anonimizar antes de liberar",

        .lucroEsquerda = PT(+5),
        .privacidadeEsquerda = PT(-15),

        .privacidadeDireita = PT(+15),
        .lucroDireita = PT(-5)
    };

    Evento carta7 = {
        .id = 7,
        .cargo = 0,
        .origem = 0,

        .titulo = "Precificação por bairro",

        .texto =
            "A IA de precificação está cobrando preços mais altos de usuários de determinados bairros, com base no suposto “risco de inadimplência”.",

        .esquerda =
            "Manter, os números fecham",

        .direita =
            "Revisar o critério de precificação",

        .lucroEsquerda = PT(+5),
        .viesEsquerda = PT(+15),

        .viesDireita = PT(-15),
        .lucroDireita = PT(-5)
    };

    Evento carta8 = {
        .id = 8,
        .cargo = 0,
        .origem = 1,

        .titulo = "Número inventado",

        .texto =
            "Um colega vai apresentar pro cliente um número que a IA gerou, mas parece inventado. Ele já confia cegamente no sistema e não quer perder tempo checando.",

        .esquerda =
            "Deixar ele apresentar assim",

        .direita =
            "Insistir em checar juntos antes",

        .confiancaEsquerda = PT(-15),
        .lucroEsquerda = PT(+5),

        .confiancaDireita = PT(+15),
        .lucroDireita = PT(-5)
    };
Evento carta9 = {
        .id = 9,
        .cargo = 0,
        .origem = 0,

        .titulo = "Automação de funcionários",

        .texto =
            "A diretoria pretende substituir parte da equipe de atendimento por uma IA. O sistema ainda comete erros frequentes, mas a mudança reduziria bastante os custos.",

        .esquerda =
            "Aprovar a substituição imediatamente",

        .direita =
            "Manter a equipe até a IA ser confiável",

        .lucroEsquerda = PT(+15),
        .confiancaEsquerda = PT(-5),

        .lucroDireita = PT(-15),
        .confiancaDireita = PT(+5)
    };

    Evento carta10 = {
        .id = 10,
        .cargo = 0,
        .origem = 0,

        .titulo = "Decisão automatizada",

        .texto =
            "Uma IA responsável por aprovar empréstimos rejeitou a solicitação de um cliente. Ele pede explicações, mas ninguém na equipe consegue entender como o sistema chegou a essa conclusão.",

        .esquerda =
            "Manter a decisão da IA sem questionar",

        .direita =
            "Solicitar uma revisão humana da decisão",

        .lucroEsquerda = PT(+5),
        .confiancaEsquerda = PT(-15),
        .viesEsquerda = PT(+5),

        .lucroDireita = PT(-5),
        .confiancaDireita = PT(+15),
        .viesDireita = PT(-5)
    };

    Evento carta11 = {
        .id = 11,
        .cargo = 0,
        .origem = 0,

        .titulo = "Imagem falsa",

        .texto =
            "A equipe de marketing utiliza uma imagem gerada por IA que simula uma pessoa real recomendando o produto. A campanha está pronta e tem grande potencial de alcance.",

        .esquerda =
            "Publicar a campanha sem revelar a manipulação",

        .direita =
            "Informar que a imagem foi gerada por IA",

        .lucroEsquerda = PT(+5),
        .confiancaEsquerda = PT(-15),

        .lucroDireita = PT(-5),
        .confiancaDireita = PT(+15)
    };

    Evento carta12 = {
        .id = 12,
        .cargo = 0,
        .origem = 0,

        .titulo = "Treinamento sem limite",

        .texto =
            "Um novo modelo de IA precisa de mais dados para melhorar. A equipe sugere coletar automaticamente todas as conversas dos usuários, inclusive as que contêm informações pessoais.",

        .esquerda =
            "Coletar todas as conversas disponíveis",

        .direita =
            "Limitar a coleta aos dados autorizados",

        .lucroEsquerda = PT(+5),
        .privacidadeEsquerda = PT(-15),

        .lucroDireita = PT(-5),
        .privacidadeDireita = PT(+15)
    };

    Evento carta13 = {
        .id = 13,
        .cargo = 0,
        .origem = 0,

        .titulo = "O trabalho perfeito",

        .texto =
            "Você pede para uma IA fazer um trabalho acadêmico. O resultado está muito bem escrito, organizado e parece conter todas as informações necessárias. O prazo está acabando e você ainda não leu o material por completo.",

        .esquerda =
            "Entregar o trabalho como a IA produziu",

        .direita =
            "Revisar o conteúdo e conferir as informações",

        .lucroEsquerda = PT(+15),
        .confiancaEsquerda = PT(-5),

        .lucroDireita = PT(-15),
        .confiancaDireita = PT(+5)
    };

    Evento carta14 = {
        .id = 14,
        .cargo = 0,
        .origem = 0,

        .titulo = "O e-mail pronto",

        .texto =
            "Você precisa responder um e-mail importante de um cliente. Para ganhar tempo, pede que uma IA escreva a resposta. O texto fica ótimo, mas a IA promete um prazo de entrega que você não tem certeza se a empresa consegue cumprir.",

        .esquerda =
            "Enviar, o texto está bem escrito",

        .direita =
            "Conferir as informações antes de enviar",

        .lucroEsquerda = PT(+5),
        .confiancaEsquerda = PT(-15),

        .lucroDireita = PT(-5),
        .confiancaDireita = PT(+15)
    };

    Evento carta15 = {
        .id = 15,
        .cargo = 0,
        .origem = 0,

        .titulo = "O código misterioso",

        .texto =
            "Você utiliza uma IA para resolver um problema no código de um projeto da Else. Ela fornece uma solução que funciona nos testes iniciais, mas você não entende completamente o que aquele código executa.",

        .esquerda =
            "Implementar e seguir para a próxima tarefa",

        .direita =
            "Estudar o código e testar outros cenários",

        .lucroEsquerda = PT(+15),
        .confiancaEsquerda = PT(-5),

        .lucroDireita = PT(-15),
        .confiancaDireita = PT(+5)
    };

    Evento carta16 = {
        .id = 16,
        .cargo = 0,
        .origem = 0,

        .titulo = "O documento interno",

        .texto =
            "Você precisa resumir um documento extenso da Else. Uma ferramenta externa de IA consegue fazer isso em segundos, mas o arquivo contém informações internas que não deveriam sair do ambiente da empresa.",

        .esquerda =
            "Enviar o documento para a ferramenta externa",

        .direita =
            "Resumir manualmente ou utilizar uma ferramenta autorizada",

        .lucroEsquerda = PT(+5),
        .privacidadeEsquerda = PT(-15),

        .lucroDireita = PT(-5),
        .privacidadeDireita = PT(+15)
    };

    Evento carta17 = {
        .id = 17,
        .cargo = 0,
        .origem = 0,

        .titulo = "O atendimento automático",

        .texto =
            "Você utiliza uma IA para responder perguntas frequentes dos clientes da Else. O sistema responde rapidamente, mas começa a dar respostas genéricas para problemas que exigem uma análise individual.",

        .esquerda =
            "Manter a automação para reduzir o tempo de atendimento",

        .direita =
            "Intervir nos casos que exigem atenção individual",

        .lucroEsquerda = PT(+15),
        .confiancaEsquerda = PT(-5),

        .lucroDireita = PT(-15),
        .confiancaDireita = PT(+5)
    };
























    // ==========================================================
    // CARTAS POR NIVEL (Junior, Pleno, Senior, Tech Lider)
    // Escala: + = 5, ++ = 15, +++ = 25. Fonte: ELSE_Cartas_<Nivel> no Drive.
    // ==========================================================
    Evento carta101 = {
        .id = 101,
        .cargo = 1,
        .origem = 0,

        .titulo = "Filtro de comentários",

        .texto =
            "Você ajusta o filtro de comentários do app, que usa IA. Ele marca como “ofensivo” muito mais comentários escritos com gírias do Nordeste.",

        .esquerda =
            "Subir assim, funciona na maioria",

        .direita =
            "Treinar o filtro com exemplos regionais",

        .confiancaEsquerda = PT(-5),
        .viesEsquerda = PT(+15),
        .lucroEsquerda = PT(+5),
        .viesDireita = PT(-15),
        .lucroDireita = PT(-5)
    };

    Evento carta102 = {
        .id = 102,
        .cargo = 1,
        .origem = 0,

        .titulo = "Biblioteca inexistente",

        .texto =
            "O assistente de código com IA sugere uma biblioteca que resolve tudo. Ela não existe, mas tem uma com nome parecido, de um autor desconhecido.",

        .esquerda =
            "Escrever a função na mão",

        .direita =
            "Instalar a parecida, deve ser essa",

        .privacidadeEsquerda = PT(+5),
        .lucroEsquerda = PT(-5),
        .confiancaDireita = PT(-5),
        .privacidadeDireita = PT(-15),
        .lucroDireita = PT(+5)
    };

    Evento carta103 = {
        .id = 103,
        .cargo = 1,
        .origem = 1,

        .titulo = "Dados reais no notebook",

        .texto =
            "Pra testar uma feature nova, sugerem copiar o banco de produção, com dados reais de clientes, pro seu notebook.",

        .esquerda =
            "Gerar dados falsos pro teste",

        .direita =
            "Copiar, é só pra teste",

        .privacidadeEsquerda = PT(+5),
        .lucroEsquerda = PT(-5),
        .privacidadeDireita = PT(-15),
        .lucroDireita = PT(+15)
    };

    Evento carta104 = {
        .id = 104,
        .cargo = 1,
        .origem = 0,

        .titulo = "Pull request gigante",

        .texto =
            "Um colega pede pra você revisar um pull request enorme, todo escrito por IA. Os testes passaram e o prazo é hoje.",

        .esquerda =
            "Aprovar, os testes passaram",

        .direita =
            "Revisar com calma antes de aprovar",

        .confiancaEsquerda = PT(-15),
        .lucroEsquerda = PT(+15),
        .confiancaDireita = PT(+5),
        .lucroDireita = PT(-5)
    };

    Evento carta105 = {
        .id = 105,
        .cargo = 1,
        .origem = 0,

        .titulo = "Aviso de IA no atendimento",

        .texto =
            "O atendimento do app usa IA, mas muitos clientes acham que falam com uma pessoa. Pedem pra você tirar o aviso “você está falando com uma IA” porque “diminui o engajamento”.",

        .esquerda =
            "Manter o aviso",

        .direita =
            "Tirar o aviso",

        .confiancaEsquerda = PT(+5),
        .lucroEsquerda = PT(-5),
        .confiancaDireita = PT(-15),
        .lucroDireita = PT(+15)
    };

    Evento carta106 = {
        .id = 106,
        .cargo = 1,
        .origem = 0,

        .titulo = "Vagas de liderança",

        .texto =
            "A ferramenta de vagas internas, que usa IA, mostra vagas de liderança quase só para homens. Ninguém reclamou ainda.",

        .esquerda =
            "Deixar como está, não é sua área",

        .direita =
            "Abrir chamado pro time responsável",

        .confiancaEsquerda = PT(-5),
        .viesEsquerda = PT(+15),
        .viesDireita = PT(-15),
        .lucroDireita = PT(-5)
    };

    Evento carta107 = {
        .id = 107,
        .cargo = 1,
        .origem = 1,

        .titulo = "Resumo da reunião",

        .texto =
            "O resumo automático da reunião diz que o cliente aprovou o novo prazo. Você estava lá e acha que não foi bem assim.",

        .esquerda =
            "Mandar o resumo como está",

        .direita =
            "Corrigir antes de mandar",

        .confiancaEsquerda = PT(-15),
        .lucroEsquerda = PT(+15),
        .confiancaDireita = PT(+5),
        .lucroDireita = PT(-5)
    };

    Evento carta108 = {
        .id = 108,
        .cargo = 1,
        .origem = 0,

        .titulo = "Contratos em servidor alheio",

        .texto =
            "Você descobre que a IA que o time usa pra resumir contratos envia os textos pra servidores de outra empresa. Os contratos têm dados de clientes.",

        .esquerda =
            "Avisar e pausar até checar",

        .direita =
            "Seguir usando, é muito prático",

        .privacidadeEsquerda = PT(+15),
        .lucroEsquerda = PT(-5),
        .privacidadeDireita = PT(-15),
        .lucroDireita = PT(+15)
    };

    Evento carta201 = {
        .id = 201,
        .cargo = 2,
        .origem = 0,

        .titulo = "Crédito e CEP",

        .texto =
            "Você lidera a feature de análise de crédito. O modelo contratado reprova muito mais gente de CEP de periferia. Corrigir atrasa o lançamento em um mês.",

        .esquerda =
            "Atrasar e corrigir antes",

        .direita =
            "Lançar no prazo e corrigir depois",

        .viesEsquerda = PT(-15),
        .lucroEsquerda = PT(-15),
        .viesDireita = PT(+15),
        .lucroDireita = PT(+15)
    };

    Evento carta202 = {
        .id = 202,
        .cargo = 2,
        .origem = 1,

        .titulo = "Chatbot promete reembolso",

        .texto =
            "O chatbot que seu time integrou começou a prometer reembolsos que não existem na política da empresa. Os clientes estão cobrando.",

        .esquerda =
            "Negar, o bot errou",

        .direita =
            "Honrar o que foi prometido e corrigir o bot",

        .confiancaEsquerda = PT(-15),
        .lucroEsquerda = PT(+5),
        .confiancaDireita = PT(+15),
        .lucroDireita = PT(-15)
    };

    Evento carta203 = {
        .id = 203,
        .cargo = 2,
        .origem = 0,

        .titulo = "Dados “anonimizados”",

        .texto =
            "Um parceiro quer pagar bem por dados de uso “anonimizados” do app. Você percebe que dá pra identificar as pessoas cruzando localização e horário.",

        .esquerda =
            "Fechar o acordo",

        .direita =
            "Recusar até anonimizar de verdade",

        .confiancaEsquerda = PT(-5),
        .privacidadeEsquerda = PT(-15),
        .lucroEsquerda = PT(+15),
        .privacidadeDireita = PT(+5),
        .lucroDireita = PT(-15)
    };

    Evento carta204 = {
        .id = 204,
        .cargo = 2,
        .origem = 0,

        .titulo = "Suporte trocado por IA",

        .texto =
            "A diretoria quer trocar o time de suporte por um agente de IA e pede sua opinião. Nos testes, ele erra nos casos difíceis.",

        .esquerda =
            "Propor IA com humanos nos casos difíceis",

        .direita =
            "Dizer que está pronto",

        .confiancaEsquerda = PT(+15),
        .lucroEsquerda = PT(-5),
        .confiancaDireita = PT(-15),
        .lucroDireita = PT(+15)
    };

    Evento carta205 = {
        .id = 205,
        .cargo = 2,
        .origem = 0,

        .titulo = "A pergunta do cliente",

        .texto =
            "Um cliente grande pergunta se o produto usa IA pra tomar decisões sobre os usuários dele. Usa, mas isso não está no contrato.",

        .esquerda =
            "Dizer que não usa",

        .direita =
            "Contar a verdade e explicar como funciona",

        .confiancaEsquerda = PT(-15),
        .lucroEsquerda = PT(+5),
        .confiancaDireita = PT(+15),
        .lucroDireita = PT(-5)
    };

    Evento carta206 = {
        .id = 206,
        .cargo = 2,
        .origem = 0,

        .titulo = "Avaliação de desempenho",

        .texto =
            "A ferramenta de avaliação de desempenho dá notas menores pra quem trabalha remoto ou tirou licença. O RH quer usar já nas promoções.",

        .esquerda =
            "Pedir pra tirar esses critérios",

        .direita =
            "Liberar pro ciclo de promoções",

        .viesEsquerda = PT(-15),
        .lucroEsquerda = PT(-5),
        .viesDireita = PT(+15),
        .lucroDireita = PT(+5)
    };

    Evento carta207 = {
        .id = 207,
        .cargo = 2,
        .origem = 1,

        .titulo = "Áudio do microfone",

        .texto =
            "Produto quer gravar o áudio do microfone dos usuários “só com o app aberto” pra melhorar recomendações. Isso fica escondido nos termos de uso.",

        .esquerda =
            "Exigir um pedido de permissão claro",

        .direita =
            "Implementar como pedido",

        .privacidadeEsquerda = PT(+15),
        .lucroEsquerda = PT(-5),
        .privacidadeDireita = PT(-15),
        .lucroDireita = PT(+15)
    };

    Evento carta208 = {
        .id = 208,
        .cargo = 2,
        .origem = 0,

        .titulo = "Testes que não testam",

        .texto =
            "Seu time usa IA pra gerar os testes automáticos. Você descobre que vários testes sempre passam e não testam nada. A entrega é amanhã.",

        .esquerda =
            "Entregar, a cobertura está em 95%",

        .direita =
            "Avisar o cliente e adiar um dia",

        .confiancaEsquerda = PT(-15),
        .lucroEsquerda = PT(+15),
        .confiancaDireita = PT(+5),
        .lucroDireita = PT(-5)
    };

    Evento carta301 = {
        .id = 301,
        .cargo = 3,
        .origem = 0,

        .titulo = "Escolha da IA de triagem",

        .texto =
            "Você escolhe a IA de triagem de currículos que a empresa vai oferecer a todos os clientes. A mais barata tem viés conhecido contra mulheres. A mais justa custa o triplo.",

        .esquerda =
            "Contratar a mais barata",

        .direita =
            "Contratar a mais justa",

        .viesEsquerda = PT(+15),
        .lucroEsquerda = PT(+15),
        .viesDireita = PT(-15),
        .lucroDireita = PT(-15)
    };

    Evento carta302 = {
        .id = 302,
        .cargo = 3,
        .origem = 1,

        .titulo = "Dose inventada",

        .texto =
            "Um hospital usa a integração que você projetou. Às vezes a IA inventa a dose do remédio no resumo do prontuário. O cliente diz que “o médico sempre confere”.",

        .esquerda =
            "Bloquear doses no resumo automático",

        .direita =
            "Confiar que o médico confere",

        .confiancaEsquerda = PT(+15),
        .lucroEsquerda = PT(-15),
        .confiancaDireita = PT(-15),
        .lucroDireita = PT(+15)
    };

    Evento carta303 = {
        .id = 303,
        .cargo = 3,
        .origem = 0,

        .titulo = "Pedido sem ordem judicial",

        .texto =
            "Um órgão público pede todos os dados de localização dos usuários “pra uma investigação”, sem ordem judicial.",

        .esquerda =
            "Exigir ordem judicial",

        .direita =
            "Entregar os dados",

        .privacidadeEsquerda = PT(+15),
        .lucroEsquerda = PT(-5),
        .confiancaDireita = PT(-5),
        .privacidadeDireita = PT(-15),
        .lucroDireita = PT(+15)
    };

    Evento carta304 = {
        .id = 304,
        .cargo = 3,
        .origem = 0,

        .titulo = "Júnior dependente de IA",

        .texto =
            "Uma pessoa júnior do seu time não consegue mais programar sem IA e entrega código que não entende. Você quase não tem tempo pra mentoria.",

        .esquerda =
            "Deixar, ela entrega rápido",

        .direita =
            "Separar tempo pra mentoria",

        .confiancaEsquerda = PT(-15),
        .lucroEsquerda = PT(+15),
        .confiancaDireita = PT(+15),
        .lucroDireita = PT(-5)
    };

    Evento carta305 = {
        .id = 305,
        .cargo = 3,
        .origem = 0,

        .titulo = "Vazamento silencioso",

        .texto =
            "Uma falha na integração com IA vazou dados de alguns clientes. Ninguém de fora percebeu ainda, mas a lei manda comunicar.",

        .esquerda =
            "Corrigir em silêncio",

        .direita =
            "Comunicar quem foi afetado",

        .confiancaEsquerda = PT(-15),
        .lucroEsquerda = PT(+15),
        .confiancaDireita = PT(+15),
        .lucroDireita = PT(-15)
    };

    Evento carta306 = {
        .id = 306,
        .cargo = 3,
        .origem = 1,

        .titulo = "Conversas para o parceiro",

        .texto =
            "Um parceiro quer treinar o modelo dele com as conversas de suporte dos últimos 5 anos. Nenhum cliente autorizou isso.",

        .esquerda =
            "Usar só de quem autorizar",

        .direita =
            "Liberar o histórico",

        .privacidadeEsquerda = PT(+15),
        .lucroEsquerda = PT(-15),
        .privacidadeDireita = PT(-15),
        .lucroDireita = PT(+15)
    };

    Evento carta307 = {
        .id = 307,
        .cargo = 3,
        .origem = 0,

        .titulo = "Previsão para investidores",

        .texto =
            "A diretoria vai mostrar aos investidores uma previsão de crescimento feita pela IA. Você vê que ela usou dados que não existem.",

        .esquerda =
            "Não falar nada, não é sua área",

        .direita =
            "Mostrar o erro antes da reunião",

        .confiancaEsquerda = PT(-15),
        .lucroEsquerda = PT(+15),
        .confiancaDireita = PT(+15),
        .lucroDireita = PT(-5)
    };

    Evento carta401 = {
        .id = 401,
        .cargo = 4,
        .origem = 0,

        .titulo = "Demissões por IA",

        .texto =
            "A empresa vai usar IA pra decidir demissões “por produtividade”. Ela aponta mais pessoas com deficiência e mais velhas. Você tem voto na decisão.",

        .esquerda =
            "Vetar e propor avaliação humana",

        .direita =
            "Aprovar, os números são objetivos",

        .viesEsquerda = PT(-15),
        .lucroEsquerda = PT(-15),
        .viesDireita = PT(+25),
        .lucroDireita = PT(+25)
    };

    Evento carta402 = {
        .id = 402,
        .cargo = 4,
        .origem = 1,

        .titulo = "Contrato com dados falsos",

        .texto =
            "Um relatório da IA embasou um contrato milionário que já foi assinado. Você descobre que metade dos dados era inventada.",

        .esquerda =
            "Deixar como está",

        .direita =
            "Contar ao cliente e renegociar",

        .confiancaEsquerda = PT(-25),
        .lucroEsquerda = PT(+15),
        .confiancaDireita = PT(+15),
        .lucroDireita = PT(-25)
    };

    Evento carta403 = {
        .id = 403,
        .cargo = 4,
        .origem = 0,

        .titulo = "A oferta da big tech",

        .texto =
            "Uma big tech quer comprar todos os dados dos usuários da empresa. Esse dinheiro salvaria o ano.",

        .esquerda =
            "Recusar a oferta",

        .direita =
            "Vender os dados",

        .privacidadeEsquerda = PT(+15),
        .lucroEsquerda = PT(-15),
        .privacidadeDireita = PT(-25),
        .lucroDireita = PT(+25)
    };

    Evento carta404 = {
        .id = 404,
        .cargo = 4,
        .origem = 0,

        .titulo = "Todo o código por IA",

        .texto =
            "O CEO quer que todo o código seja escrito por IA e cortar metade do time. Você sabe que em um ano ninguém vai entender o sistema.",

        .esquerda =
            "Defender o time e propor uso equilibrado",

        .direita =
            "Aceitar o plano",

        .confiancaEsquerda = PT(+15),
        .lucroEsquerda = PT(-5),
        .confiancaDireita = PT(-25),
        .lucroDireita = PT(+15)
    };

    Evento carta405 = {
        .id = 405,
        .cargo = 4,
        .origem = 0,

        .titulo = "Sem política de IA",

        .texto =
            "A empresa nunca teve regras sobre uso de IA. Criar uma política séria vai atrasar todos os projetos por um mês.",

        .esquerda =
            "Deixar sem regras, cada time decide",

        .direita =
            "Criar e divulgar a política",

        .confiancaEsquerda = PT(-15),
        .lucroEsquerda = PT(+15),
        .confiancaDireita = PT(+15),
        .lucroDireita = PT(-15)
    };

    Evento carta406 = {
        .id = 406,
        .cargo = 4,
        .origem = 1,

        .titulo = "O estudo engavetado",

        .texto =
            "Um estudo interno mostra viés num produto que já está em 200 clientes. Corrigir significa admitir o erro publicamente.",

        .esquerda =
            "Engavetar o estudo",

        .direita =
            "Publicar e corrigir",

        .viesEsquerda = PT(+15),
        .lucroEsquerda = PT(+15),
        .viesDireita = PT(-15),
        .lucroDireita = PT(-15)
    };

    Evento carta407 = {
        .id = 407,
        .cargo = 4,
        .origem = 0,

        .titulo = "Vigiar funcionários",

        .texto =
            "Um cliente quer usar a plataforma da empresa pra vigiar as redes sociais dos próprios funcionários e prever quem vai pedir demissão.",

        .esquerda =
            "Recusar esse uso",

        .direita =
            "Fechar o contrato",

        .privacidadeEsquerda = PT(+15),
        .lucroEsquerda = PT(-15),
        .privacidadeDireita = PT(-25),
        .lucroDireita = PT(+15)
    };

    Evento carta408 = {
        .id = 408,
        .cargo = 4,
        .origem = 0,

        .titulo = "O hospital fora do ar",

        .texto =
            "Uma falha de IA derrubou o sistema de um hospital cliente por horas. A imprensa liga. O time confiou demais e não revisou.",

        .esquerda =
            "Culpar “um erro da IA”",

        .direita =
            "Assumir a responsabilidade",

        .confiancaEsquerda = PT(-25),
        .lucroEsquerda = PT(+5),
        .confiancaDireita = PT(+25),
        .lucroDireita = PT(-15)
    };

    Evento bancoPerguntas[48];
    
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
    bancoPerguntas[17] = carta101;
    bancoPerguntas[18] = carta102;
    bancoPerguntas[19] = carta103;
    bancoPerguntas[20] = carta104;
    bancoPerguntas[21] = carta105;
    bancoPerguntas[22] = carta106;
    bancoPerguntas[23] = carta107;
    bancoPerguntas[24] = carta108;
    bancoPerguntas[25] = carta201;
    bancoPerguntas[26] = carta202;
    bancoPerguntas[27] = carta203;
    bancoPerguntas[28] = carta204;
    bancoPerguntas[29] = carta205;
    bancoPerguntas[30] = carta206;
    bancoPerguntas[31] = carta207;
    bancoPerguntas[32] = carta208;
    bancoPerguntas[33] = carta301;
    bancoPerguntas[34] = carta302;
    bancoPerguntas[35] = carta303;
    bancoPerguntas[36] = carta304;
    bancoPerguntas[37] = carta305;
    bancoPerguntas[38] = carta306;
    bancoPerguntas[39] = carta307;
    bancoPerguntas[40] = carta401;
    bancoPerguntas[41] = carta402;
    bancoPerguntas[42] = carta403;
    bancoPerguntas[43] = carta404;
    bancoPerguntas[44] = carta405;
    bancoPerguntas[45] = carta406;
    bancoPerguntas[46] = carta407;
    bancoPerguntas[47] = carta408;
}


// Pasta e nome base das imagens de cada carta (mesma ordem do banco).
// Cada carta tem 3 imagens: _S_ (situacao), _A_ (esquerda) e _D_ (direita).
static const char *arquivosCartas[48][2] =
{
    { "cartasEstagiario", "Triagem" },
    { "cartasEstagiario", "Chatbot" },
    { "cartasEstagiario", "Historico 2" },
    { "cartasEstagiario", "Facial" },
    { "cartasEstagiario", "Estatistica" },
    { "cartasEstagiario", "Marketing 2" },
    { "cartasEstagiario", "Precificacao" },
    { "cartasEstagiario", "Numero inventado" },
    { "cartasEstagiario", "Automacao" },
    { "cartasEstagiario", "Decisao" },
    { "cartasEstagiario", "Imagem" },
    { "cartasEstagiario", "Treinamento" },
    { "cartasEstagiario", "Trabalho" },
    { "cartasEstagiario", "Email" },
    { "cartasEstagiario", "Codigo" },
    { "cartasEstagiario", "Documento" },
    { "cartasEstagiario", "Atendimento" },
    { "cartasJunior", "Filtro Regional" },
    { "cartasJunior", "Biblioteca Falsa" },
    { "cartasJunior", "Dados de Teste" },
    { "cartasJunior", "Pull Request" },
    { "cartasJunior", "Aviso de IA" },
    { "cartasJunior", "Vagas Internas" },
    { "cartasJunior", "Resumo da Reuniao" },
    { "cartasJunior", "Contratos na Nuvem" },
    { "cartasPleno", "Analise de Credito" },
    { "cartasPleno", "Chatbot Reembolso" },
    { "cartasPleno", "Dados Anonimizados" },
    { "cartasPleno", "Suporte por IA" },
    { "cartasPleno", "Cliente Grande" },
    { "cartasPleno", "Avaliacao Desempenho" },
    { "cartasPleno", "Microfone" },
    { "cartasPleno", "Testes Falsos" },
    { "cartasSenior", "Triagem Cliente" },
    { "cartasSenior", "Dose do Remedio" },
    { "cartasSenior", "Orgao Publico" },
    { "cartasSenior", "Mentoria" },
    { "cartasSenior", "Vazamento" },
    { "cartasSenior", "Treinar Parceiro" },
    { "cartasSenior", "Previsao Investidores" },
    { "cartasTechLider", "Demissoes por IA" },
    { "cartasTechLider", "Contrato Milionario" },
    { "cartasTechLider", "Venda de Dados" },
    { "cartasTechLider", "Codigo por IA" },
    { "cartasTechLider", "Politica de IA" },
    { "cartasTechLider", "Estudo de Vies" },
    { "cartasTechLider", "Vigilancia" },
    { "cartasTechLider", "Falha no Hospital" },
};

void CarregarTexturasCartas()
{
    for (int i = 0; i < 48; i++)
    {
        const char *pasta = arquivosCartas[i][0];
        const char *nome = arquivosCartas[i][1];

        bancoPerguntas[i].situacao = LoadTexture(TextFormat("HUD/%s/Carta %s _S_.png", pasta, nome));
        bancoPerguntas[i].opcaoDireita = LoadTexture(TextFormat("HUD/%s/Carta %s _D_.png", pasta, nome));
        bancoPerguntas[i].opcaoEsquerda = LoadTexture(TextFormat("HUD/%s/Carta %s _A_.png", pasta, nome));

        // as imagens sao 3x maiores que o tamanho na tela
        bancoPerguntas[i].situacao.width *= 0.4;
        bancoPerguntas[i].opcaoDireita.width *= 0.4;
        bancoPerguntas[i].opcaoEsquerda.width *= 0.4;
        bancoPerguntas[i].situacao.height *= 0.4;
        bancoPerguntas[i].opcaoDireita.height *= 0.4;
        bancoPerguntas[i].opcaoEsquerda.height *= 0.4;
    }
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