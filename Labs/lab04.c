/*NOME: NAOMI TAKEMOTO    RA: 184849   TURMA: C */
/*Para este laboratorio foram consutadas as seguintes fontes:
 *Implementacao da funcao de hash: https://www.ime.usp.br/~pf/mac0122-2002/aulas/hashing.html
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

#define BASE 241
#define ATOR 120
#define FILME 200
#define LINHA 320
#define F_ATOR 35
#define F_FILME 60

typedef char * string;
typedef struct node node;
typedef struct adjacencia adjacencia;


/*cada no pode representar tanto um filme como um ator*/
struct node{
    string nome;
    adjacencia *adj;/*lista de filmes caso o no contenha um ato, lista de atores coso o npo contenha um filme*/
    node *prox;
    node *predecessor;
    int caminho;
};

struct adjacencia{
    node *no;
    adjacencia *prox;
};

/*codigo da API disponibilizada na pagina do prof da disciplina*/
struct _fila {
    node **elementos;
    int inicio;
    int fim;
    int quantidade;
    int max;
};
typedef struct _fila Fila;



int achaPrimo(int n);
int hash(string str, int m);
node **criaTabelaHash(int m );
node *insereTabelaHash(string str, node ** tabela, int m);
node *buscaNode(string str, node ** tabela, int m);
void imprimeTabela(node ** tabela, int m);
void criaAdjacencia (node * no1, node *no2);
int caminhoMin(node ** tabelaAtores, int mAtor, node ** tabelaFilmes, int mFilme, node *ator1, node * ator2);
int filaCheia(Fila * fila);
int filaVazia(Fila * fila);
Fila *aumentaFila(Fila *fila);
int enqueue(node *elemento, Fila * fila);
node* dequeue(Fila *fila);
Fila * criaFila(int max);
void imprime_fila(Fila *fila);
void destroiTabela(node ** tabela, int m);

/*-----------------------------------------------------MAIN------------------------------------------------------------*/
int main (int argc, char *argv[]){
    
    int tam = 0,mAtor, mFilme, i = 0,a,b;
    FILE *bancoDados = NULL;
    FILE *busca = NULL;
    string arqBancoDados = argv[1];
    string arqBusca = argv[2];
    string nomeAtor,nomeFilme, nomeAtor2;
    node *endAtor, *endAtor2, *endFilme;
    node **tabelaAtores, **tabelaFilmes;
    
    
    string linha = malloc(sizeof(char)*LINHA);
 
    /*abre os arquivos*/
    bancoDados = fopen(arqBancoDados,"r");
    busca = fopen(arqBusca,"r");
    if(bancoDados == NULL || busca == NULL){
        printf("Erro na abertura do arquivo\n");
        exit(1);
    }
    
    /*determinacao do tamanho aproximado do nÃºmero de atores*/
    while(fgets(linha,LINHA, bancoDados)){
        tam++;
    }
	    
    /*calcula os tamnhos das tabelas de hash*/
    if (tam >= 100000){
        mAtor = achaPrimo(tam/F_ATOR);          /*acha o maior numero primo menor que n*/
        mFilme = achaPrimo(tam/F_FILME);
    }else{/*cria as tabelas de hash com fator de carga igual a 1*/
        if(tam >= 20){
            mAtor = achaPrimo(tam/2);
            mFilme = achaPrimo(mAtor/2);
        }else{
            mAtor = achaPrimo(tam);
            mFilme = mAtor;
        }
    }
    
    /*cria as tabelas de hash*/
    //printf("mAtor = %d\nmFilme = %d\n", mAtor,mFilme);
    tabelaAtores = criaTabelaHash(mAtor);
    tabelaFilmes = criaTabelaHash(mFilme);

    /*Le os dados do arquivo e monta o grafo*/
    rewind(bancoDados);
    while(fgets(linha,LINHA, bancoDados)){
        nomeAtor = strdup(strtok(linha, "\t"));
        nomeFilme = strtok(NULL, "\t");
        nomeFilme = strdup(strtok(nomeFilme, "\n"));
        endAtor = insereTabelaHash(nomeAtor,tabelaAtores, mAtor);
        endFilme = insereTabelaHash(nomeFilme,tabelaFilmes, mFilme);
        criaAdjacencia(endAtor, endFilme);
    }
    
    
    //printf("TABELA DE ATORES m = %d:\n", mAtor);fflush(stdout);
    //imprimeTabela(tabelaAtores, mAtor);
    //printf("TABELA DE FILMES: m = %d:\n", mFilme);fflush(stdout);
    //imprimeTabela(tabelaFilmes, mFilme);
    
    if(bancoDados){
        close(bancoDados);
    }
    
    free(linha);
    nomeAtor = malloc(ATOR*sizeof(char));
    nomeAtor2 = malloc(ATOR*sizeof(char));
    /*le os atores a serem buscados*/
    i = 0;
    while( fgets(nomeAtor, LINHA, busca)){
        
        nomeAtor = strtok(linha,"\n");
        endAtor = buscaNode(nomeAtor, tabelaAtores, mAtor);
        fgets(nomeAtor, LINHA, busca);
        nomeAtor = strtok(linha,"\n");
        endAtor2 = buscaNode(nomeAtor, tabelaAtores, mAtor);
        if(endAtor == NULL || endAtor2 == NULL){
            printf("Atore(s) nao se encontram na base de dados\n");fflush(stdout);
            exit(1);
        }
        
        if(caminhoMin(tabelaAtores,mAtor, tabelaFilmes,mFilme, endAtor2, endAtor) == 1){
            printf("%s\n", endAtor->nome);fflush(stdout);
            while(endAtor != endAtor->predecessor){
                endAtor = endAtor->predecessor;
                printf("trabalhou em \"%s\" com\n", endAtor->nome);fflush(stdout);
                endAtor = endAtor->predecessor;
                printf("%s\n", endAtor->nome);fflush(stdout);
            }
        }else{
            printf("CAMINHO NAO ENCONTRADO\n");
        }
        
        //printf("Fora do while mais interno\n");fflush(stdout);
    }
    
    if(busca){
        close(busca);
    }
    destroiTabela(tabelaAtores, mAtor);
    destroiTabela(tabelaFilmes,mFilme);
    
    
    free(linha);
    return 0;
}

/*O que faz: dado um numero n encontra o menor numero primo menor que n*/
int achaPrimo(int n){
    int divisor = 2;
    int primo = 0, limite;
    while(!primo && n >2 ){
        if (n%2 == 0){
            n--;
        }
        limite = n/2+1;
        for(divisor = 3; divisor < (limite + 1); divisor += 2){
        if (n%divisor == 0){
            n--;
            limite = n/2+1;
            break;
        }
        }
        if(divisor >= limite){
            break;
        }
    }
    return n;
}

/*O que faz: recebe como parametro uma string e calcula seu hash
 *Retorna: o valor do hash (h) calculado*/
int hash(string str, int m) {
    int i,h = str[0];
    for (i = 1; str[i] != '\0'; i++) {
          h = (h * 251 + str[i]) % m;
    }
    h = h%m;
    return h;
}

/*O que faz: cria tabela de hash, cada posicao da tabela possui um apontador para uma lista de atores/filmes
 *           se tabela[i] == NULL entao o bucket esta vazio.
 *Retorno: um apontador para um vetor para a tabela que guarda apontadores para os nos.
 */
node** criaTabelaHash(int m ){
    int i;
    node **tabela = (node**)malloc(m*sizeof(node*));
    for(i = 0; i<m; i++){
        tabela[i] = NULL;/*seta todas as posicoes para vazia*/
    }
    return tabela;
}

/*O que faz: se a posicao h da tabela estiver vazia insere na posicao h, caso contrario resolve a colisao por
 *           encadeamento
 *Retorna: um apontador para o novo no criado*/
node *insereTabelaHash(string str, node ** tabela, int m){
    int h;
    node *novo, *prox;
    h = hash(str, m);
    //printf("Nome: %-50s Hash = %d\n",str,h);fflush(stdout);
    if(tabela[h] == NULL){
        node *novo = (node*)malloc(sizeof(node));
        novo->nome = str;
        novo->adj = NULL;
        novo->prox = NULL;
        tabela[h] = novo;
        return novo;
    }else{
        prox = tabela[h];
        while(prox){
            if(strcmp(str,prox->nome) == 0){
                return prox;
            }
            prox = prox->prox;
        }
       
        novo = (node*)malloc(sizeof(node));
        novo->nome = str;
        novo->adj = NULL;
        novo->prox = tabela[h]->prox;
        tabela[h]->prox = novo;
        return novo;
    }
    
}

/*O que fax: recebe uma string que represnta um nome e localiza o no da tabela que contem esse nome
 *Retorna: a localizacao do no*/
node *buscaNode(string str, node ** tabela, int m){
    int h;
    node *prox;
    h = hash(str, m);
    prox = tabela[h];
    //printf("Procura: %s hash = %d\n", str, h);
    while(prox){
        if(strcmp(prox->nome, str) == 0){
            return prox;
        }
        prox = prox->prox;
    }
    return NULL;/*caso nao ache o no que contem a string*/
}

void imprimeTabela(node ** tabela, int m){
    int i = 0;
    node *prox;
    adjacencia *listaAdj;
    while(i<m){
        prox = tabela[i];
        while(prox){
            printf("tabela[ %d ] = %s\n", i, prox->nome);fflush(stdout);
            listaAdj = prox->adj;
            while(listaAdj){
                printf("%13s %s\n"," ",listaAdj->no->nome);
                listaAdj = listaAdj->prox;
                
            }
            prox = prox->prox;
        }
        if(!prox){
            printf("tabela[ %d ] = NULL\n", i);fflush(stdout);
        }
        i++;
    }
}

/*O que faz: adiciona o endereco do no2 na lista de asjacencias do no1 e adicona o no1 na lista de adjacencias
             do no2*/
void criaAdjacencia (node * no1, node *no2){
    adjacencia *aux,*listaAdjNo1,*listaAdjNo2;
    char achou = 0;
    if (no1->adj == NULL){
        listaAdjNo1 = (adjacencia*)malloc(sizeof(adjacencia));
        listaAdjNo1->no = no2;
        listaAdjNo1->prox = NULL;
        no1->adj = listaAdjNo1;
    }else{
        aux = no1->adj;
        while(aux){
            if (aux->no == no2){
                achou = 1;
                break;
            }
            aux = aux->prox;
        }
        if(!achou){
            listaAdjNo1 = (adjacencia*)malloc(sizeof(adjacencia));
            listaAdjNo1->no = no2;
            listaAdjNo1->prox = no1->adj->prox;
            no1->adj->prox = listaAdjNo1; 
        }
    }
    achou = 0;
    if (no2->adj == NULL){
        listaAdjNo2 = (adjacencia*)malloc(sizeof(adjacencia));
        listaAdjNo2->no = no1;
        listaAdjNo2->prox = NULL;
        no2->adj = listaAdjNo2;
    }else{
        aux = no2->adj;
        while(aux){
            if (aux->no == no1) {
                achou = 1;
                break;
            }
            aux = aux->prox;
        }
        if(!achou){
            listaAdjNo2 = (adjacencia*)malloc(sizeof(adjacencia));
            listaAdjNo2->no = no1;
            listaAdjNo2->prox = no2->adj->prox;
            no2->adj->prox = listaAdjNo2; 
        }   
    }
}

/*Recebe as duas tabelas de hahs que armazenam o grafo e pinta de branco os seus nos*/
int caminhoMin(node ** tabelaAtores, int mAtor, node ** tabelaFilmes, int mFilme, node *ator1, node * ator2){
    Fila *fila;
    int i = 0, achou = 0, sai = 0;
    node *prox, *aux;
    adjacencia *adj;
    
    fila = criaFila(10);
    
    /*faz todos os predecessores serem nulos e seta o tam do caminho do do no inicio como "infinito"*/
    while(i<mAtor){
        prox = tabelaAtores[i];/*pecorre cada bucket da tabela*/
        while(prox){
            if(prox != ator1){
                prox->predecessor = NULL;
                prox->caminho = INT_MAX;/*tam do caminho eh infinito*/
            }else{                      /*Se achou o ator de inicio*/
                prox->caminho = 0;
                prox->predecessor = prox;
                enqueue(prox,fila);
            }
            prox = prox->prox;
        }
        i++;
    }
    /*como nao ha atores na tabela de filmes nao ha necessidade de verificar se o no analizado eh o inicial*/
    i = 0;
    while(i<mFilme){
        prox = tabelaFilmes[i];
        while(prox){
            prox->predecessor = NULL;
            prox->caminho = INT_MAX;
            prox = prox->prox;
        }
        i++;
    }
    
    //printf("Fila: ");
    //imprime_fila(fila);
    while(!filaVazia(fila) && !sai){
       // printf("Fila: ");
   // imprime_fila(fila);

        aux = dequeue(fila);
        if(!aux){
            //printf("CAMINHO NAO ENCONTRADO\n");
            break;
        }
        adj = aux->adj;
       
        while(adj){    /*para todo os nos da lista de adjacencias de aux que nao foram visitados*/
            if(adj->no->predecessor == NULL){
                adj->no->predecessor = aux;
                enqueue(adj->no, fila);
                //printf("Fila: ");
                //imprime_fila(fila);
            }
            if(adj->no == ator2){
                achou = sai = 1;
                adj->no->predecessor = aux;
                break;
            }
            adj = adj->prox;
            
        }
    }
    free(fila);
    return achou;
}

void destroiTabela(node ** tabela, int m){
    int i = 0;
    node *deleta, *prox;
    adjacencia *adjDel;
    for (i = 0; i< m; i++){
        prox = tabela[i];
        while(prox){
            while(prox->adj){/*libera a lista de adjacencias de cada no*/
                adjDel = prox->adj;
                prox->adj = prox->adj->prox;
                free(adjDel);
            }
            deleta = prox;
            prox = prox->prox;
            free(deleta);
        }
    }
    free(tabela);
}
/*---------------------------------------FUNCOES DE FILA-----------------------------------------------------*/
/*O codigo abaixo e uma adaptacao da API fornecida no site da diciplina*/

Fila *criaFila(int max){
    Fila *fila = (Fila*)malloc(sizeof(Fila));
    if(!fila){
        printf("Erro: na funcao cria fila malloc falhou\n");
        exit(1);
    }
    fila->elementos = (node**)malloc(max*sizeof(node*));
    if(!fila->elementos){
        printf("Erro: na funcao cria fila malloc falhou\n");
        exit(1);
    }
    fila->inicio = 0;
    fila->fim = max -1;
    fila->quantidade = 0;
    fila->max = max;
    return fila;
}

/*O que faz: verifica se a pilha esta cheia
 *Retorno: 1 se a fila estiver cheia, 0 caso contrario*/
int filaCheia(Fila * fila){
    if(fila->quantidade == fila->max){
        return 1;
    }
    return 0;
}

/*O que faz: se dobra o espaco disponivel para a fila
 *Retorno: um apontador para a fila com o novo tamanho*/
Fila *aumentaFila(Fila *fila){
    fila->max = (fila->max)*2;
    fila = (Fila*)realloc(fila,fila->max);
    if(!fila){
        printf("Erro: na funcao cria fila malloc falhou\n");
        exit(1);
    }
    return fila;
    
}

int enqueue(node * elemento, Fila * fila){
    if(filaCheia(fila)){
        fila = aumentaFila(fila);
    }
    fila->fim = (fila->fim +1)%(fila->max); 
    fila->elementos[fila->fim] = elemento; /*adiciona o elemento depois incrementa*/
    fila->quantidade++;
    return 1;
}

int filaVazia(Fila * fila){
    if (!fila){
       printf("Erro: na funcao fila_vazia, o apontador fila == NULL\n");
        exit(1);
    }
   return(!fila->quantidade);/*se a fila estiver vazia retorna true*/
}

node *dequeue(Fila *fila){
    int max = fila->max;
    node *deletado;
    if(filaVazia(fila)){
        printf("Erro: na funcao dequeue, fila vazia\n");
        exit(1);
    }
    deletado = fila->elementos[fila->inicio];
    fila->inicio = (fila->inicio+1)%max;
    fila->quantidade--;
    return deletado;
}

void imprime_fila(Fila *fila){
	int inicio, fim, max;
    inicio = fila->inicio;
    fim = fila->fim;
    max = fila->max;
    while(inicio != fim){
        printf("%s",fila->elementos[inicio]->nome);
        inicio = (inicio+1)%max;
    }
    printf("%s",fila->elementos[inicio]->nome);
    printf("\n");
}










