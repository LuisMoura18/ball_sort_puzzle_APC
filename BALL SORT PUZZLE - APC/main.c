/*  Projeto APC (2025.1) - Ball Sort Puzzle  
    Aluno: Luis Andre Moura Fernandes
    Matricula: 251004367
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 10
#define JOGADORES 10

//variaveis globais
char nome[20];
char matriz[MAX][MAX];
int altura, colunas, totJogador = 0;
FILE *fd;

//struct para salvar os jogadores
typedef struct{
    char nome[20];
    int pontos;
} Usuario;
Usuario jogador;
Usuario rank[JOGADORES];

//funcao p/ limpar tela
#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

//funcao para retornar a pagina anterior
void retornar(){
    printf("<Pressione ENTER para voltar>\n");
    getchar(); getchar();
}

//conta os simbolos de cada frasco
int contador(int coluna){
    int cont = 0;
    for (int i = 0; i < altura; i++){
        if (matriz[coluna][i] != 0){
            cont++;
        }
    }return cont;
}

//indica qual é o simbolo do topo
char topo(int coluna){
    for (int i = altura - 1; i >= 0; i--){
        if (matriz[coluna][i] != 0){
            return matriz[coluna][i];
        }
    }return 0;
}

//Apresenta a matriz a cada solicitação
void mostrar_matriz() {
    printf("\n");
    for (int i = altura - 1; i >= 0; i--) {
        for (int j = 0; j < colunas; j++) {
            if (matriz[j][i] != 0){
                printf("| %c ", matriz[j][i]);
            }else{
                printf("|   ");
            }
        }
        printf("|\n");
    }
    // Mostra o indice das colunas
    for (int j = 0; j < colunas; j++) {
        printf("  %d ", j + 1);
    }
    printf("\n\n");
}

//valida se os movimentos sao validos
int movimento_valido(int origem, int destino){
    origem--; destino--;//numero solicitado -1 devido a matriz comecar no 0
    //chama as funcoes para as devidas verificacoes
    int qntdO = contador(origem), qntdD = contador(destino);
    char SimbOri = topo(origem), SimbDes = topo(destino);
    if (origem >= colunas || destino >= colunas || origem < 0 || destino < 0){ //limites da matriz
        printf(">>Capacidade nao suportada<<\n");
        return 0;
    }
    if (qntdO == 0){ //sem simbolos no frasco de origem
        printf("Origem vazia\n"); return 0;
    }
    if(qntdD >= altura){ //se o frasco estiver cheio
        printf("Frasco cheio.\n"); return 0;
    }
    if(SimbDes == 0 || SimbOri == SimbDes){//condicao verdadeira
        return 1;
    }else{
        printf(">>Invalido<<\n"); return 0;
    }
}

//realiza na matriz o movimento solicitado pelo jogador 
void movimento_matriz(int origem, int destino){
    origem--; destino--; //numero solicitado -1 devido a matriz comecar no 0
    char simbolo;
    int espacoDestino = 0;
    //verifica se o frasco de destino tem espaço
    for (int j = 0; j < altura; j++){
        if (matriz[destino][j] == 0){
            espacoDestino = 1;
            break;
        }
    }
    if (!espacoDestino){
        printf("Frasco de destino está cheia!\n");
        return;
    }
    //verifica o simbolo que esta no topo
    for (int i = altura - 1; i >= 0; i--){
        if (matriz[origem][i] != 0){
            simbolo = matriz[origem][i];
            matriz[origem][i] = 0;
            //coloca no espaço vazio da base do fracso destino
            for (int j = 0; j < altura; j++){
                if (matriz[destino][j] == 0){
                    matriz[destino][j] = simbolo;
                    break;
                }
            }
            break;
        }
    }
}

//perguntas ao usuario e condicao de movimento
void perguntas(){
    int origem, destino;
    printf("Informe a coluna de origem: ");
    scanf("%d", &origem);
    printf("Informe a coluna de destino: ");
    scanf("%d", &destino);
    //se o movimento for valido, ele ira mudar na matriz
    if( movimento_valido(origem, destino) ){
        movimento_matriz(origem,destino);
        printf("Movimento Realizado com Sucesso!\n");
    }else{
        printf("Nao Eh Possivel Realizar o Movimento!\n");
        retornar();
    }
}

//verifica se todas os frascos foram concluidos
int concluido(){
    for (int i = 0; i < colunas; i++){
        char simbolo = matriz[i][0];
        int qntd = contador(i);
        if(qntd == 0){continue;}
        if(qntd != altura){return 0;}
        for(int j = 1; j < altura; j++){
            if(matriz[i][j] != simbolo){
                return 0;
            }
        }
    }
    return 1;
}

//funcao para rodar boa parte do jogo
void partida(){
    int fase = 1, continuar = 1;
    char resp;
    fd = fopen("entrada_v2.txt", "r");

    if(fd == NULL){//se o arquivo nao abrir, o jogo nao roda
        printf("Erro ao iniciar partida!\n");
        retornar();
        return;
    }

    while(1){
        memset(matriz, 0, sizeof(matriz)); //zera todos os espacos da matriz toda ver q ler uma fase nova
        colunas = 0; altura = 0;
        char simbolo, tmp;
        //le os simbolos ate encontrar o `-`
        while(1){
            int altFrasco;
            if (fscanf(fd, "%d", &altFrasco) != 1) break;
            //verifica se eh `-`
            int c = fgetc(fd);
            if (c == '-'){
                break;
            }else if(c != '\n') {
                ungetc(c, fd);//se nao for \n devolve o caractere
            } 
            if (altFrasco == 0) {continue;} //nao le os 0`s dps dos frascos
            if (colunas >= MAX) {break;}

            for (int i = 0; i < altFrasco; i++){
                if (fscanf(fd, " %c", &simbolo) == 1){
                    matriz[colunas][i] = simbolo;
                }
            }
            //define a maior altura do frasco
            if (altFrasco > altura) {
                altura = altFrasco;
            }
            colunas++;
    }
    //adiciona mais dois frascos para ter espaco para movimentacao
    for (int i = 0; i < altura; i++) {
        matriz[colunas][i] = 0;
        matriz[colunas + 1][i] = 0;
    }colunas += 2; 

    //se a coluna nao for lida, acaba
    if (colunas == 0) break;

    //loop para ler todas as fases do jogo
    while (!concluido()) {//encerra quando todos os frascos forem completos
        system(CLEAR);
        mostrar_matriz();
        perguntas();
    }
    system(CLEAR);
    printf("PARABENS!! VOCE VENCEU!!!\n");
    printf("PARABENS!! VOCE VENCEU!!!\n");
    printf("PARABENS!! VOCE VENCEU!!!\n\n");
    
    jogador.pontos += 100;
    printf("Quer continuar para a proxima fase? (S/N) ");
    scanf(" %c", &resp);
    if (resp != 'S' && resp != 's') {
        break;
    }
    fase++;
}
    //adiciona o jogador ao ranking antes de finalizar
    if (totJogador < JOGADORES){
        rank[totJogador] = jogador;
        totJogador++;
    }
    printf("Jogo Encerrado. Obrigado por jogar!\n");
    fclose(fd);
    retornar();
}

//funcao para zerar todos os dados da struct
void zerarRanking(){
    char opcao;
    system(CLEAR);
    printf("Deseja ZERAR o ranking? (S/N) ");
    scanf(" %c", &opcao);
    if(opcao == 'n' || opcao == 'N'){
        retornar();
    }else if (opcao == 's' || opcao == 'S'){
        memset(rank, 0, sizeof(rank));
        totJogador = 0;
        printf("Operacao realizada com sucesso\n");
        retornar();
    }
}

//ordena o ranking
int ordemRanking(const void *a, const void *b){
    Usuario *jogador1 = (Usuario *)a;
    Usuario *jogador2 = (Usuario *)b;
    return jogador2->pontos - jogador1->pontos;
}

//mostra ao jogador o ranking vazio ou com as pontuacoeos
void ranking(){
    system(CLEAR);
    if(totJogador == 0){
        printf("---------------------\n");
        printf("----RANKING VAZIO----\n");
        printf("---------------------\n");
        retornar();
        return;
    }
    qsort(rank, totJogador, sizeof(Usuario), ordemRanking);
    printf("---------- RANKING ---------\n");
    printf("----------------------------\n");
    for(int i = 0; i < totJogador; i++){
        printf("~%d - %s: %d\n", i+1, rank[i].nome, rank[i].pontos);
    }
    printf("\n\n");
    retornar();
}

//configuracoes do jogo
void config(){
    int opcao;
    do{
        system(CLEAR);
        printf("*** CONFIGURACOES ***\n\n");
        printf("1 - ZERAR Ranking.\n");
        printf("2 - Modo BLIND.\n");
        printf("3 - Editor de Fases.\n");
        printf("4 - Voltar ao menu principal.\n");
        printf("Digite a opcao desejada: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            zerarRanking();
            break;
        case 2:
            printf("Em desenvolvimento...\n");
            retornar();
            break;
        case 3:
            printf("Em desenvolvimento...\n");
            retornar();
            break;
        case 4:
            opcao = 4;
            break;
        default:
            printf("Opcao Invalida! \n");
            retornar();
            break;
        }
    }while(opcao != 4);
}

//Instrucoes basicas para o jogador, retorna ao pressionar ENTER;
void instrucao(){
    system(CLEAR);
    printf("****** INSTRUCOES ******\n\n");
    printf("OBJETIVO: \n O jogador devera organziar a mesma sequencia de simbolos no mesmo frasco.\n\n");
    printf("COMO JOGAR: \n");
    printf("- Indique o numero da coluna do simbolo de origen.\n");
    printf("- Em seguida informe o numero da coluna em que deseja coloca-lo.\n\n");
    printf(">>> Nao eh permitido <<<\n");
    printf("  * Mover para um frasco completo. \n");
    printf("  * Mover um simbolo para um outro simbolo diferente. \n");
    printf("  * Indicar um frasco inexistetne. \n\n");
    printf("OBS: \n");
    printf("- So eh permitido a movimentacao de um simbolo por vez!\n");
    printf("- Elabore uma estrategia antes e durante o jogo. \n\n");
    printf("****** DIVIRTA - SE *******\n");
    printf("***************************\n");
    retornar();
}

int main(){
    int opcao;
    system(CLEAR);
    //Tela inicial de boas vindas
    printf("Bem vindo(a) ao Jogo BALL SORT de APC!!!\n\n\n");
    printf("Autor: Luis Andre Moura.\n");
    printf("Professora: Carla Castanho.\n\n\n");
    printf(">>Aperte ENTER para comecar<<");
    getchar();

    do
    {
        system(CLEAR);
        //Menu principal do jogo
        printf("\n **** BALL SORT ****\n\n");
        printf("1 - Jogar\n");
        printf("2 - Configuracao\n");
        printf("3 - Instrucoes\n");
        printf("4 - Ranking\n");
        printf("5 - Sair\n\n");
        printf("Digite a opcao desejada: ");
        scanf("%d", &opcao);
        switch (opcao)
        {
        case 1:
            //possibilita novos jogadores a cada solicitacao
             printf("Informe seu nickname: ");
             scanf("%s", jogador.nome);
             jogador.pontos = 0;
            partida();
            break;
        case 2:
            config();
            break;
        case 3:
            instrucao();
            break;    
        case 4:
            ranking();
            break; 
        case 5:
            break;       
        default:
            printf("Opcao Invalida! \n");
            retornar();
            break;
        }
    } while (opcao != 5);
    system(CLEAR);
    return 0;
}