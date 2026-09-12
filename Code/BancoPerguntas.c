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
    int reputacaoEsquerda;
    int reputacaoDireita;
    int competenciaEsquerda;
    int competenciaDireita;
    int energiaEsquerda;
    int energiaDireita;
    int eticaEsquerda;
    int eticaDireita;
    int dinheiroEsquerda;
    int dinheiroDireita;
} Evento;

Evento bancoPerguntas[] =
{
    {1,0,"Primeira tarefa","Seu gestor entrega uma atividade que voce nunca executou e precisa dela ainda hoje.","Pedir orientacao","Tentar sozinho",3,5,2,1,3,5,-2,-8,2,0,0,0},
    {2,0,"Erro encontrado","Depois de entregar uma planilha, voce percebe que enviou um valor incorreto.","Avisar o gestor","Corrigir sem avisar",4,2,4,-3,3,2,-2,-1,5,-5,0,0},
    {3,0,"Tres urgencias","Tres pessoas diferentes pedem tarefas urgentes para o mesmo horario.","Perguntar prioridade","Tentar fazer tudo",3,6,3,1,3,4,-2,-12,2,0,0,0},
    {4,0,"Documento confidencial","Voce encontra aberto no computador um documento com informacoes salariais da equipe.","Fechar o documento","Dar uma olhada",2,1,3,-5,1,0,0,0,7,-10,0,0},
    {5,0,"Tempo livre","Voce terminou todas as suas tarefas e ainda faltam duas horas para o fim do expediente.","Pedir nova tarefa","Esperar",5,1,4,-1,4,0,-4,2,2,0,0,0},
    {6,0,"Atalho","Um funcionario antigo mostra um jeito mais rapido de realizar sua tarefa, mas nao e o procedimento oficial.","Confirmar o metodo","Usar imediatamente",3,4,2,1,4,5,-1,2,4,-3,0,0},
    {7,0,"Feedback","Seu gestor critica uma tarefa que levou horas para voce concluir.","Perguntar como melhorar","Apenas refazer",5,3,3,1,5,2,-3,-5,2,0,0,0},
    {8,0,"Informacao faltando","Seu trabalho depende de uma informacao, mas seu supervisor esta em reuniao.","Adiantar outra tarefa","Decidir sozinho",3,5,2,1,3,4,-1,-3,2,0,0,0},
    {9,0,"Planilha desconhecida","Voce recebe uma planilha antiga sem nenhuma explicacao sobre como ela funciona.","Estudar antes","Alterar direto",5,4,2,-2,5,3,-4,-2,3,-2,0,0},
    {10,0,"Mensagem do cliente","Um cliente pergunta algo que voce ainda nao sabe responder.","Consultar alguem","Responder mesmo assim",4,3,3,-4,4,2,-2,-1,4,-4,0,0},
    {11,0,"Fim do expediente","Seu horario terminou, mas voce ainda esta no meio de uma atividade importante.","Finalizar a tarefa","Continuar amanha",5,2,3,0,4,1,-8,5,1,0,0,0},
    {12,0,"Duvida repetida","Voce ja perguntou duas vezes sobre uma tarefa, mas ainda nao entendeu completamente.","Perguntar novamente","Fingir que entendeu",4,2,2,-4,4,1,-2,-3,4,-4,0,0},

    {13,1,"Erro de colega","Voce percebe que um colega lancou uma informacao incorreta que pode afetar o resultado do setor.","Conversar com ele","Avisar o supervisor",5,4,5,1,4,3,-2,-1,4,3,0,0},
    {14,1,"Cliente esperando","Um cliente pede uma resposta urgente, mas voce ainda depende de outro setor.","Explicar e dar prazo","Esperar resposta final",5,3,5,-2,4,2,-2,0,3,1,0,0},
    {15,1,"Processo repetitivo","Voce percebe que uma tarefa feita todos os dias poderia ser automatizada.","Criar uma proposta","Seguir como sempre",7,2,5,0,7,1,-7,0,2,0,0,0},
    {16,1,"Colega faltou","Um colega faltou e varias tarefas dele foram repassadas para voce.","Negociar os prazos","Aceitar tudo",5,7,4,3,4,5,-3,-15,2,1,0,0},
    {17,1,"Ordens diferentes","Dois superiores passam instrucoes diferentes para a mesma atividade.","Pedir confirmacao","Escolher uma delas",4,5,4,-2,4,3,-2,-3,3,0,0,0},
    {18,1,"Prazo curto","Uma atividade prevista para amanha precisa ser entregue em duas horas.","Reorganizar prioridades","Fazer correndo",6,5,5,1,5,3,-6,-12,2,-1,0,0},
    {19,1,"Procedimento antigo","Voce percebe que o procedimento interno esta desatualizado e causa duvidas na equipe.","Sugerir atualizacao","Deixar como esta",6,2,4,0,6,1,-4,0,2,0,0,0},
    {20,1,"Solicitacao incompleta","Um setor envia uma solicitacao urgente sem as informacoes necessarias para executar o trabalho.","Pedir os dados","Tentar completar",5,4,4,0,5,3,-2,-5,3,-2,0,0},

    {21,2,"Numero estranho","Um numero muito diferente aparece em um relatorio que sera enviado para a diretoria.","Investigar","Entregar assim mesmo",7,2,5,-6,7,1,-5,0,4,-5,0,0},
    {22,2,"Prazo ou qualidade","Seu relatorio ainda esta incompleto e faltam poucos minutos para o prazo.","Entregar parcial","Atrasar e completar",5,7,3,1,5,6,-2,-8,2,2,0,0},
    {23,2,"Novo estagiario","Seu gestor pede que voce ajude um novo estagiario a entender o setor.","Treinar pessoalmente","Enviar os manuais",6,3,6,1,4,2,-7,-1,3,1,0,0},
    {24,2,"Erro do gestor","Voce percebe um erro na apresentacao do seu gestor minutos antes de uma reuniao.","Avisar","Nao interferir",5,1,5,-3,4,0,-2,0,4,-2,0,0},
    {25,2,"Dados incompletos","Voce precisa analisar um resultado, mas parte dos dados recebidos parece incompleta.","Solicitar novos dados","Estimar o restante",6,5,4,0,6,4,-3,-1,4,-2,0,0},
    {26,2,"Reuniao importante","Uma decisao esta sendo tomada em uma reuniao com base em informacao incorreta.","Interromper e avisar","Falar depois",7,4,5,0,6,3,-3,-1,5,1,0,0},
    {27,2,"Resultado inesperado","Sua analise apresenta um resultado muito diferente do previsto pela equipe.","Revisar tudo","Defender o resultado",6,7,3,5,7,6,-4,-3,3,2,0,0},

    {28,3,"Erro recorrente","O mesmo problema operacional aconteceu tres vezes neste mes.","Criar um controle","Corrigir novamente",8,3,6,0,8,2,-8,-2,4,0,0,0},
    {29,3,"Colega sobrecarregado","Um colega competente esta sobrecarregado e comecando a cometer erros.","Redistribuir trabalho","Nao interferir",6,2,7,-2,5,1,-4,0,5,-1,0,0},
    {30,3,"Discordancia","Seu superior escolheu uma solucao que voce acredita que aumentara os custos.","Apresentar argumentos","Executar sem questionar",7,3,5,1,7,2,-4,-1,4,0,0,0},
    {31,3,"Falha na entrega","Um problema aparece poucas horas antes de uma entrega importante para um cliente.","Avisar sobre o risco","Tentar resolver escondido",6,8,4,2,5,7,-3,-12,5,-4,0,0},
    {32,3,"Processo antigo","Um processo importante depende de varias etapas manuais que frequentemente geram erros.","Propor melhoria","Manter o processo",8,2,6,0,8,1,-7,0,3,0,0,0},
    {33,3,"Custo inesperado","Voce percebe que um projeto vai ultrapassar o valor inicialmente previsto.","Reportar agora","Esperar confirmar",6,5,5,1,6,4,-3,-2,5,1,0,0},

    {34,4,"Baixo desempenho","Um membro da equipe esta atrasando entregas constantemente.","Conversar primeiro","Reportar imediatamente",7,5,7,2,6,4,-4,-2,5,2,0,0},
    {35,4,"Nova tecnologia","Uma tecnologia nova poderia reduzir bastante o tempo de um processo importante.","Fazer teste controlado","Manter processo atual",9,2,6,1,9,2,-8,0,4,0,0,0},
    {36,4,"Erro antigo","Voce descobre um erro financeiro relevante que passou despercebido por meses.","Reportar imediatamente","Investigar primeiro",7,9,6,5,7,9,-2,-7,8,5,0,0},
    {37,4,"Pressao por prazo","Sua equipe pode cumprir o prazo apenas trabalhando sob forte pressao.","Negociar prazo","Manter o prazo",6,8,5,2,5,7,-2,-12,5,1,0,0},
    {38,4,"Risco operacional","Voce identifica uma falha que ainda nao causou problemas, mas pode causar no futuro.","Corrigir preventivamente","Esperar acontecer",8,2,7,-3,8,1,-6,0,6,-4,0,0},

    {39,5,"Conflito na equipe","Dois dos melhores funcionarios do setor nao conseguem mais trabalhar juntos.","Mediar o conflito","Separar os dois",8,6,8,3,7,5,-8,-3,6,3,0,0},
    {40,5,"Meta dificil","A diretoria estabelece uma meta que sua equipe dificilmente conseguira atingir.","Negociar a meta","Cobrar a equipe",8,7,7,1,7,6,-4,-15,5,0,0,0},
    {41,5,"Pedido de aumento","Um dos melhores funcionarios recebeu uma proposta de outra empresa.","Defender aumento","Aceitar a saida",7,5,8,-4,5,4,-3,-1,5,1,-100,0},
    {42,5,"Corte de custos","A empresa exige uma reducao de quinze por cento nos custos do setor.","Rever projetos","Cortar de todos",8,7,4,-3,7,5,-5,-8,4,0,100,150},
    {43,5,"Funcionario novo","Um funcionario novo tem potencial, mas esta cometendo muitos erros.","Desenvolver","Substituir",8,5,7,-2,6,3,-6,-1,6,1,-40,0},

    {44,6,"Cliente importante","Um cliente muito importante exige uma excecao aos procedimentos da empresa.","Manter as regras","Negociar excecao",7,9,4,7,5,7,-2,-5,8,2,0,100},
    {45,6,"Automacao","Uma nova tecnologia poderia automatizar grande parte das tarefas repetitivas do setor.","Investir e treinar","Manter estrutura",10,2,8,0,10,2,-8,0,6,0,-200,0},
    {46,6,"Contratacao","Existe orcamento para contratar apenas uma pessoa para uma area critica.","Contratar experiente","Desenvolver iniciante",7,9,6,7,8,9,-2,-7,4,6,-150,-60},
    {47,6,"Funcionario antigo","Um funcionario antigo e querido pela equipe esta com desempenho muito abaixo do necessario.","Plano de melhoria","Substituir",8,6,8,-5,7,5,-6,-2,7,1,-50,0},
    {48,6,"Projeto ruim","A empresa ja investiu muito dinheiro em um projeto que continua apresentando resultados ruins.","Encerrar projeto","Investir novamente",8,10,5,2,7,8,-4,-10,5,0,100,-250},
    {49,6,"Crise","Uma falha grave pode prejudicar varios clientes e ainda nao se tornou publica.","Comunicar e corrigir","Resolver em silencio",10,8,8,2,9,8,-8,-5,10,-8,-100,0}
};

Evento *obterBancoPerguntas(int *quantidade)
{
    *quantidade = sizeof(bancoPerguntas) / sizeof(bancoPerguntas[0]);
    return bancoPerguntas;
}