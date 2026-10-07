typedef struct Evento
{
    int id;
    int cargo;
    const char *titulo;
    const char *texto;
    const char *esquerda;
    const char *direita;
    int experienciaEsquerda;
    int experienciaDireita;
    int confiancaEsquerda;
    int confiancaDireita;
    int viesEsquerda;
    int viesDireita;
    int privacidadeEsquerda;
    int privacidadeDireita;
    int lucroEsquerda;
    int lucroDireita;
} Evento;

Evento bancoPerguntas[] =
{

    //experiencia (esq, dir) + medidores confianca, vies, privacidade, lucro (esq, dir). + e - = 5, ++ e -- = 15, nao citado = 0.
    {101,0,"Triagem de curriculos","Seu chefe pede pra usar um modelo de triagem de curriculos que ja existe no sistema. Voce percebe que ele filtra quase so candidatos de universidades caras.","Usar assim mesmo, e rapido","Sinalizar o problema antes de rodar",0,0,-5,5,5,-5,0,0,5,-5},
    {102,0,"Chatbot inventou um dado","O chatbot de suporte gerou uma resposta tecnica para um cliente, mas o dado que ele citou nao existe em nenhuma documentacao.","Deixar passar, o cliente nao vai notar","Corrigir e avisar o cliente do erro",0,0,-15,5,0,0,0,0,5,-5},
    {103,0,"Historico de navegacao","Voce tem acesso ao historico de navegacao dos usuarios dentro da empresa \"pra melhorar recomendacoes\". Ninguem pediu autorizacao explicita pra isso.","Usar os dados, todo mundo faz","Pedir consentimento antes",0,0,0,0,0,0,-15,5,5,-5},
    {105,0,"Reconhecimento facial","Um sistema de reconhecimento facial da empresa erra bem mais em rostos de pele escura. Alguem pede pra \"ajustar o threshold\" so pra esconder o problema nas metricas do relatorio.","Ajustar o threshold como pedido","Reportar o problema real",0,0,-5,5,15,-5,0,0,0,0},
    {106,0,"Estatistica suspeita","Um relatorio automatico de vendas cita uma estatistica que parece forte demais pra ser real. Voce esta sob pressao pra entregar o relatorio hoje.","Entregar assim, sem checar","Investigar a fonte antes de enviar",0,0,-15,5,0,0,0,0,5,-5},
    {107,0,"Dados do marketing","Marketing pede pra usar dados de usuarios (nome, e-mail, comportamento) pra treinar um novo modelo, sem anonimizar.","Liberar os dados como pedido","Anonimizar antes de liberar",0,0,0,0,0,0,-15,5,15,-5},
    {109,0,"Precificacao por bairro","A IA de precificacao esta cobrando mais de usuarios de certos bairros, com base em \"risco de inadimplencia\".","Manter, os numeros fecham","Revisar o criterio de precificacao",0,0,0,0,15,-5,0,0,15,-5},
    {110,0,"Numero inventado","Um colega vai apresentar pro cliente um numero que a IA gerou, mas parece inventado. Ele ja confia cegamente no sistema e nao quer perder tempo checando.","Deixar ele apresentar assim","Insistir em checar juntos antes",0,0,-15,5,0,0,0,0,5,-5},

    {13,1,"Erro de colega","Voce percebe que um colega lancou uma informacao incorreta que pode afetar o resultado do setor.","Conversar com ele","Avisar o supervisor",5,4,0,0,0,0,0,0,0,0},
    {14,1,"Cliente esperando","Um cliente pede uma resposta urgente, mas voce ainda depende de outro setor.","Explicar e dar prazo","Esperar resposta final",5,3,0,0,0,0,0,0,0,0},
    {15,1,"Processo repetitivo","Voce percebe que uma tarefa feita todos os dias poderia ser automatizada.","Criar uma proposta","Seguir como sempre",7,2,0,0,0,0,0,0,0,0},
    {16,1,"Colega faltou","Um colega faltou e varias tarefas dele foram repassadas para voce.","Negociar os prazos","Aceitar tudo",5,7,0,0,0,0,0,0,0,0},
    {17,1,"Ordens diferentes","Dois superiores passam instrucoes diferentes para a mesma atividade.","Pedir confirmacao","Escolher uma delas",4,5,0,0,0,0,0,0,0,0},
    {18,1,"Prazo curto","Uma atividade prevista para amanha precisa ser entregue em duas horas.","Reorganizar prioridades","Fazer correndo",6,5,0,0,0,0,0,0,0,0},
    {19,1,"Procedimento antigo","Voce percebe que o procedimento interno esta desatualizado e causa duvidas na equipe.","Sugerir atualizacao","Deixar como esta",6,2,0,0,0,0,0,0,0,0},
    {20,1,"Solicitacao incompleta","Um setor envia uma solicitacao urgente sem as informacoes necessarias para executar o trabalho.","Pedir os dados","Tentar completar",5,4,0,0,0,0,0,0,0,0},

    {21,2,"Numero estranho","Um numero muito diferente aparece em um relatorio que sera enviado para a diretoria.","Investigar","Entregar assim mesmo",7,2,0,0,0,0,0,0,0,0},
    {22,2,"Prazo ou qualidade","Seu relatorio ainda esta incompleto e faltam poucos minutos para o prazo.","Entregar parcial","Atrasar e completar",5,7,0,0,0,0,0,0,0,0},
    {23,2,"Novo estagiario","Seu gestor pede que voce ajude um novo estagiario a entender o setor.","Treinar pessoalmente","Enviar os manuais",6,3,0,0,0,0,0,0,0,0},
    {24,2,"Erro do gestor","Voce percebe um erro na apresentacao do seu gestor minutos antes de uma reuniao.","Avisar","Nao interferir",5,1,0,0,0,0,0,0,0,0},
    {25,2,"Dados incompletos","Voce precisa analisar um resultado, mas parte dos dados recebidos parece incompleta.","Solicitar novos dados","Estimar o restante",6,5,0,0,0,0,0,0,0,0},
    {26,2,"Reuniao importante","Uma decisao esta sendo tomada em uma reuniao com base em informacao incorreta.","Interromper e avisar","Falar depois",7,4,0,0,0,0,0,0,0,0},
    {27,2,"Resultado inesperado","Sua analise apresenta um resultado muito diferente do previsto pela equipe.","Revisar tudo","Defender o resultado",6,7,0,0,0,0,0,0,0,0},

    {28,3,"Erro recorrente","O mesmo problema operacional aconteceu tres vezes neste mes.","Criar um controle","Corrigir novamente",8,3,0,0,0,0,0,0,0,0},
    {29,3,"Colega sobrecarregado","Um colega competente esta sobrecarregado e comecando a cometer erros.","Redistribuir trabalho","Nao interferir",6,2,0,0,0,0,0,0,0,0},
    {30,3,"Discordancia","Seu superior escolheu uma solucao que voce acredita que aumentara os custos.","Apresentar argumentos","Executar sem questionar",7,3,0,0,0,0,0,0,0,0},
    {31,3,"Falha na entrega","Um problema aparece poucas horas antes de uma entrega importante para um cliente.","Avisar sobre o risco","Tentar resolver escondido",6,8,0,0,0,0,0,0,0,0},
    {32,3,"Processo antigo","Um processo importante depende de varias etapas manuais que frequentemente geram erros.","Propor melhoria","Manter o processo",8,2,0,0,0,0,0,0,0,0},
    {33,3,"Custo inesperado","Voce percebe que um projeto vai ultrapassar o valor inicialmente previsto.","Reportar agora","Esperar confirmar",6,5,0,0,0,0,0,0,0,0},

    {34,4,"Baixo desempenho","Um membro da equipe esta atrasando entregas constantemente.","Conversar primeiro","Reportar imediatamente",7,5,0,0,0,0,0,0,0,0},
    {35,4,"Nova tecnologia","Uma tecnologia nova poderia reduzir bastante o tempo de um processo importante.","Fazer teste controlado","Manter processo atual",9,2,0,0,0,0,0,0,0,0},
    {36,4,"Erro antigo","Voce descobre um erro financeiro relevante que passou despercebido por meses.","Reportar imediatamente","Investigar primeiro",7,9,0,0,0,0,0,0,0,0},
    {37,4,"Pressao por prazo","Sua equipe pode cumprir o prazo apenas trabalhando sob forte pressao.","Negociar prazo","Manter o prazo",6,8,0,0,0,0,0,0,0,0},
    {38,4,"Risco operacional","Voce identifica uma falha que ainda nao causou problemas, mas pode causar no futuro.","Corrigir preventivamente","Esperar acontecer",8,2,0,0,0,0,0,0,0,0},

    {39,5,"Conflito na equipe","Dois dos melhores funcionarios do setor nao conseguem mais trabalhar juntos.","Mediar o conflito","Separar os dois",8,6,0,0,0,0,0,0,0,0},
    {40,5,"Meta dificil","A diretoria estabelece uma meta que sua equipe dificilmente conseguira atingir.","Negociar a meta","Cobrar a equipe",8,7,0,0,0,0,0,0,0,0},
    {41,5,"Pedido de aumento","Um dos melhores funcionarios recebeu uma proposta de outra empresa.","Defender aumento","Aceitar a saida",7,5,0,0,0,0,0,0,0,0},
    {42,5,"Corte de custos","A empresa exige uma reducao de quinze por cento nos custos do setor.","Rever projetos","Cortar de todos",8,7,0,0,0,0,0,0,0,0},
    {43,5,"Funcionario novo","Um funcionario novo tem potencial, mas esta cometendo muitos erros.","Desenvolver","Substituir",8,5,0,0,0,0,0,0,0,0},

    {44,6,"Cliente importante","Um cliente muito importante exige uma excecao aos procedimentos da empresa.","Manter as regras","Negociar excecao",7,9,0,0,0,0,0,0,0,0},
    {45,6,"Automacao","Uma nova tecnologia poderia automatizar grande parte das tarefas repetitivas do setor.","Investir e treinar","Manter estrutura",10,2,0,0,0,0,0,0,0,0},
    {46,6,"Contratacao","Existe orcamento para contratar apenas uma pessoa para uma area critica.","Contratar experiente","Desenvolver iniciante",7,9,0,0,0,0,0,0,0,0},
    {47,6,"Funcionario antigo","Um funcionario antigo e querido pela equipe esta com desempenho muito abaixo do necessario.","Plano de melhoria","Substituir",8,6,0,0,0,0,0,0,0,0},
    {48,6,"Projeto ruim","A empresa ja investiu muito dinheiro em um projeto que continua apresentando resultados ruins.","Encerrar projeto","Investir novamente",8,10,0,0,0,0,0,0,0,0},
    {49,6,"Crise","Uma falha grave pode prejudicar varios clientes e ainda nao se tornou publica.","Comunicar e corrigir","Resolver em silencio",10,8,0,0,0,0,0,0,0,0}
};

Evento *obterBancoPerguntas(int *quantidade)
{
    *quantidade = sizeof(bancoPerguntas) / sizeof(bancoPerguntas[0]);
    return bancoPerguntas;
}