typedef struct Evento
{
    int id;
    int cargo;
    const char *titulo;
    const char *texto;
    const char *esquerda;
    const char *direita;
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
    {1,0,"Triagem de curriculos","Seu chefe pede pra usar um modelo de triagem de curriculos que ja existe no sistema. Voce percebe que ele filtra quase so candidatos de universidades caras.","Usar assim mesmo, e rapido","Sinalizar o problema antes de rodar",-5,5,5,-5,0,0,5,-5},
    {2,0,"Chatbot inventou um dado","O chatbot de suporte gerou uma resposta tecnica para um cliente, mas o dado que ele citou nao existe em nenhuma documentacao.","Deixar passar, o cliente nao vai notar","Corrigir e avisar o cliente do erro",-15,5,0,0,0,0,5,-5},
    {3,0,"Historico de navegacao","Voce tem acesso ao historico de navegacao dos usuarios dentro da empresa \"pra melhorar recomendacoes\". Ninguem pediu autorizacao explicita pra isso.","Usar os dados, todo mundo faz","Pedir consentimento antes",0,0,0,0,-15,5,5,-5},
    {5,0,"Reconhecimento facial","Um sistema de reconhecimento facial da empresa erra bem mais em rostos de pele escura. Alguem pede pra \"ajustar o threshold\" so pra esconder o problema nas metricas do relatorio.","Ajustar o threshold como pedido","Reportar o problema real",-5,5,15,-5,0,0,0,0},
    {6,0,"Estatistica suspeita","Um relatorio automatico de vendas cita uma estatistica que parece forte demais pra ser real. Voce esta sob pressao pra entregar o relatorio hoje.","Entregar assim, sem checar","Investigar a fonte antes de enviar",-15,5,0,0,0,0,5,-5},
    {7,0,"Dados do marketing","Marketing pede pra usar dados de usuarios (nome, e-mail, comportamento) pra treinar um novo modelo, sem anonimizar.","Liberar os dados como pedido","Anonimizar antes de liberar",0,0,0,0,-15,5,15,-5},
    {9,0,"Precificacao por bairro","A IA de precificacao esta cobrando mais de usuarios de certos bairros, com base em \"risco de inadimplencia\".","Manter, os numeros fecham","Revisar o criterio de precificacao",0,0,15,-5,0,0,15,-5},
    {10,0,"Numero inventado","Um colega vai apresentar pro cliente um numero que a IA gerou, mas parece inventado. Ele ja confia cegamente no sistema e nao quer perder tempo checando.","Deixar ele apresentar assim","Insistir em checar juntos antes",-15,5,0,0,0,0,5,-5}
};

Evento *obterBancoPerguntas(int *quantidade)
{
    *quantidade = sizeof(bancoPerguntas) / sizeof(bancoPerguntas[0]);
    return bancoPerguntas;
}