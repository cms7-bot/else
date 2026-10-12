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

    
} Evento;


// ==========================================================
// BANCO DE PERGUNTAS
//
// ATENÇÃO: atualmente o jogo usa SOMENTE as cartas do cargo 0
// (Estagiário). As cartas dos cargos 1 em diante (ids 13 a 49)
// NÃO estão no jogo no momento e estão aqui apenas como
// material fora de contexto, para uso futuro.
// ==========================================================
Evento bancoPerguntas[] =
{

// ==========================================================
// CARGO 0 - ESTAGIÁRIO  (ÚNICAS CARTAS ATIVAS NO JOGO)
// ==========================================================
{
    1, 0, "Vies na triagem",
    "A empresa quer usar um sistema que filtra candidatos de universidades caras para acelerar a selecao.",
    "Usar o sistema para agilizar a contratacao.",
    "Sinalizar o problema e revisar os criterios.",
    0.11f, -0.11f,
    -0.11f, 0.11f,
    0.0f, 0.0f,
    -0.11f, 0.11f
},
{
    2, 0, "Dado inventado",
    "O chatbot citou um dado tecnico que nao aparece na documentacao.",
    "Deixar a resposta passar para nao atrasar a entrega.",
    "Corrigir o dado e avisar o cliente.",
    0.22f, -0.11f,
    0.0f, 0.0f,
    0.0f, 0.0f,
    -0.11f, 0.11f
},
{
    3, 0, "Dados sem consentimento",
    "A equipe quer usar o historico de navegacao dos usuarios para gerar recomendacoes sem pedir consentimento.",
    "Usar os dados para melhorar as recomendacoes.",
    "Solicitar consentimento antes de utilizar os dados.",
    0.0f, 0.0f,
    0.0f, 0.0f,
    0.22f, -0.11f,
    -0.11f, 0.11f
},
{
    4, 0, "Decisao automatizada",
    "A IA recomenda cortar um pequeno fornecedor com base em um padrao de risco, mas nao ha tempo para verificar os dados.",
    "Aprovar a recomendacao imediatamente.",
    "Revisar os dados antes de decidir.",
    0.11f, -0.11f,
    0.0f, 0.0f,
    0.0f, 0.0f,
    -0.11f, 0.11f
},
{
    5, 0, "Metrica manipulada",
    "O reconhecimento facial erra mais com pessoas de pele escura. Um colega sugere ajustar o limite apenas para esconder o problema nas metricas.",
    "Ajustar o limite para melhorar os resultados apresentados.",
    "Relatar o problema real e investigar a causa.",
    0.11f, -0.11f,
    -0.22f, 0.11f,
    0.0f, 0.0f,
    0.0f, 0.0f
},
{
    6, 0, "Estatistica suspeita",
    "Um relatorio automatico de vendas apresenta uma estatistica suspeita perto do prazo de entrega.",
    "Entregar o relatorio sem verificar o dado.",
    "Investigar a origem da estatistica.",
    0.22f, -0.11f,
    0.0f, 0.0f,
    0.0f, 0.0f,
    -0.11f, 0.11f
},
{
    7, 0, "Dados expostos",
    "A equipe de marketing quer receber nomes, emails e comportamentos dos usuarios para treinar um modelo, sem anonimizar os dados.",
    "Disponibilizar os dados como solicitado.",
    "Anonimizar os dados antes de compartilha-los.",
    0.0f, 0.0f,
    0.0f, 0.0f,
    0.22f, -0.11f,
    -0.22f, 0.11f
},
{
    8, 0, "Sem revisao humana",
    "Um sistema de credito aprova emprestimos automaticamente, e a equipe sugere retirar a revisao humana para ganhar velocidade.",
    "Retirar a revisao humana para acelerar o processo.",
    "Manter a revisao humana nas decisoes.",
    0.11f, -0.11f,
    0.0f, 0.0f,
    0.0f, 0.0f,
    -0.11f, 0.11f
},
{
    9, 0, "Preco discriminatorio",
    "A IA cobra precos maiores de usuarios de alguns bairros devido a uma estimativa de risco.",
    "Manter os precos porque os numeros parecem corretos.",
    "Revisar os criterios utilizados pelo sistema.",
    0.0, 0.0,
    -0.22f, 0.11f,
    0.0, 0.0,
    -0.22f, 0.11f
},
{
    10, 0, "Confianca cega",
    "Um colega vai apresentar um numero gerado por IA que parece inventado, mas confia nele sem verificar.",
    "Deixar o colega apresentar o numero sem conferir.",
    "Insistir em verificar o numero em conjunto.",
    0.22f, -0.11f,
    0.0, 0.0,
    0.0, 0.0,
    -0.11f, 0.11f
},


// ==========================================================
// A PARTIR DAQUI: CARTAS FORA DE CONTEXTO
// (cargos 1 a 6 - NÃO estão no jogo atualmente)
// ==========================================================

// ----------------------------------------------------------
// CARGO 1 - ASSISTENTE  (fora do jogo)
// ----------------------------------------------------------
    {13,1,"Erro de colega","Voce percebe que um colega lancou uma informacao incorreta que pode afetar o resultado do setor.","Conversar com ele","Avisar o supervisor",5,4,5,1,4,3,-2,-1},
    {14,1,"Cliente esperando","Um cliente pede uma resposta urgente, mas voce ainda depende de outro setor.","Explicar e dar prazo","Esperar resposta final",5,3,5,-2,4,2,-2,0},
    {15,1,"Processo repetitivo","Voce percebe que uma tarefa feita todos os dias poderia ser automatizada.","Criar uma proposta","Seguir como sempre",7,2,5,0,7,1,-7,0},
    {16,1,"Colega faltou","Um colega faltou e varias tarefas dele foram repassadas para voce.","Negociar os prazos","Aceitar tudo",5,7,4,3,4,5,-3,-15},
    {17,1,"Ordens diferentes","Dois superiores passam instrucoes diferentes para a mesma atividade.","Pedir confirmacao","Escolher uma delas",4,5,4,-2,4,3,-2,-3},
    {18,1,"Prazo curto","Uma atividade prevista para amanha precisa ser entregue em duas horas.","Reorganizar prioridades","Fazer correndo",6,5,5,1,5,3,-6,-12},
    {19,1,"Procedimento antigo","Voce percebe que o procedimento interno esta desatualizado e causa duvidas na equipe.","Sugerir atualizacao","Deixar como esta",6,2,4,0,6,1,-4,0},
    {20,1,"Solicitacao incompleta","Um setor envia uma solicitacao urgente sem as informacoes necessarias para executar o trabalho.","Pedir os dados","Tentar completar",5,4,4,0,5,3,-2,-5},

// ----------------------------------------------------------
// CARGO 2 - ANALISTA JUNIOR  (fora do jogo)
// ----------------------------------------------------------
    {21,2,"Numero estranho","Um numero muito diferente aparece em um relatorio que sera enviado para a diretoria.","Investigar","Entregar assim mesmo",7,2,5,-6,7,1,-5,0},
    {22,2,"Prazo ou qualidade","Seu relatorio ainda esta incompleto e faltam poucos minutos para o prazo.","Entregar parcial","Atrasar e completar",5,7,3,1,5,6,-2,-8},
    {23,2,"Novo estagiario","Seu gestor pede que voce ajude um novo estagiario a entender o setor.","Treinar pessoalmente","Enviar os manuais",6,3,6,1,4,2,-7,-1},
    {24,2,"Erro do gestor","Voce percebe um erro na apresentacao do seu gestor minutos antes de uma reuniao.","Avisar","Nao interferir",5,1,5,-3,4,0,-2,0},
    {25,2,"Dados incompletos","Voce precisa analisar um resultado, mas parte dos dados recebidos parece incompleta.","Solicitar novos dados","Estimar o restante",6,5,4,0,6,4,-3,-1},
    {26,2,"Reuniao importante","Uma decisao esta sendo tomada em uma reuniao com base em informacao incorreta.","Interromper e avisar","Falar depois",7,4,5,0,6,3,-3,-1},
    {27,2,"Resultado inesperado","Sua analise apresenta um resultado muito diferente do previsto pela equipe.","Revisar tudo","Defender o resultado",6,7,3,5,7,6,-4,-3},

// ----------------------------------------------------------
// CARGO 3 - ANALISTA PLENO  (fora do jogo)
// ----------------------------------------------------------
    {28,3,"Erro recorrente","O mesmo problema operacional aconteceu tres vezes neste mes.","Criar um controle","Corrigir novamente",8,3,6,0,8,2,-8,-2},
    {29,3,"Colega sobrecarregado","Um colega competente esta sobrecarregado e comecando a cometer erros.","Redistribuir trabalho","Nao interferir",6,2,7,-2,5,1,-4,0},
    {30,3,"Discordancia","Seu superior escolheu uma solucao que voce acredita que aumentara os custos.","Apresentar argumentos","Executar sem questionar",7,3,5,1,7,2,-4,-1},
    {31,3,"Falha na entrega","Um problema aparece poucas horas antes de uma entrega importante para um cliente.","Avisar sobre o risco","Tentar resolver escondido",6,8,4,2,5,7,-3,-12},
    {32,3,"Processo antigo","Um processo importante depende de varias etapas manuais que frequentemente geram erros.","Propor melhoria","Manter o processo",8,2,6,0,8,1,-7,0},
    {33,3,"Custo inesperado","Voce percebe que um projeto vai ultrapassar o valor inicialmente previsto.","Reportar agora","Esperar confirmar",6,5,5,1,6,4,-3,-2},

// ----------------------------------------------------------
// CARGO 4 - ANALISTA SENIOR  (fora do jogo)
// ----------------------------------------------------------
    {34,4,"Baixo desempenho","Um membro da equipe esta atrasando entregas constantemente.","Conversar primeiro","Reportar imediatamente",7,5,7,2,6,4,-4,-2},
    {35,4,"Nova tecnologia","Uma tecnologia nova poderia reduzir bastante o tempo de um processo importante.","Fazer teste controlado","Manter processo atual",9,2,6,1,9,2,-8,0},
    {36,4,"Erro antigo","Voce descobre um erro financeiro relevante que passou despercebido por meses.","Reportar imediatamente","Investigar primeiro",7,9,6,5,7,9,-2,-7},
    {37,4,"Pressao por prazo","Sua equipe pode cumprir o prazo apenas trabalhando sob forte pressao.","Negociar prazo","Manter o prazo",6,8,5,2,5,7,-2,-12},
    {38,4,"Risco operacional","Voce identifica uma falha que ainda nao causou problemas, mas pode causar no futuro.","Corrigir preventivamente","Esperar acontecer",8,2,7,-3,8,1,-6,0},

// ----------------------------------------------------------
// CARGO 5 - SUPERVISOR  (fora do jogo)
// ----------------------------------------------------------
    {39,5,"Conflito na equipe","Dois dos melhores funcionarios do setor nao conseguem mais trabalhar juntos.","Mediar o conflito","Separar os dois",8,6,8,3,7,5,-8,-3},
    {40,5,"Meta dificil","A diretoria estabelece uma meta que sua equipe dificilmente conseguira atingir.","Negociar a meta","Cobrar a equipe",8,7,7,1,7,6,-4,-15},
    {41,5,"Pedido de aumento","Um dos melhores funcionarios recebeu uma proposta de outra empresa.","Defender aumento","Aceitar a saida",7,5,8,-4,5,4,-3,-1},
    {42,5,"Corte de custos","A empresa exige uma reducao de quinze por cento nos custos do setor.","Rever projetos","Cortar de todos",8,7,4,-3,7,5,-5,-8},
    {43,5,"Funcionario novo","Um funcionario novo tem potencial, mas esta cometendo muitos erros.","Desenvolver","Substituir",8,5,7,-2,6,3,-6,-1},

// ----------------------------------------------------------
// CARGO 6 - GERENTE  (fora do jogo)
// ----------------------------------------------------------
    {44,6,"Cliente importante","Um cliente muito importante exige uma excecao aos procedimentos da empresa.","Manter as regras","Negociar excecao",7,9,4,7,5,7,-2,-5},
    {45,6,"Automacao","Uma nova tecnologia poderia automatizar grande parte das tarefas repetitivas do setor.","Investir e treinar","Manter estrutura",10,2,8,0,10,2,-8,0},
    {46,6,"Contratacao","Existe orcamento para contratar apenas uma pessoa para uma area critica.","Contratar experiente","Desenvolver iniciante",7,9,6,7,8,9,-2,-7},
    {47,6,"Funcionario antigo","Um funcionario antigo e querido pela equipe esta com desempenho muito abaixo do necessario.","Plano de melhoria","Substituir",8,6,8,-5,7,5,-6,-2},
    {48,6,"Projeto ruim","A empresa ja investiu muito dinheiro em um projeto que continua apresentando resultados ruins.","Encerrar projeto","Investir novamente",8,10,5,2,7,8,-4,-10},
    {49,6,"Crise","Uma falha grave pode prejudicar varios clientes e ainda nao se tornou publica.","Comunicar e corrigir","Resolver em silencio",10,8,8,2,9,8,-8,-5}
};


// ==========================================================
// ACESSO AO BANCO
// ==========================================================
Evento *obterBancoPerguntas(int *quantidade)
{
    *quantidade = sizeof(bancoPerguntas) / sizeof(bancoPerguntas[0]);
    return bancoPerguntas;
}