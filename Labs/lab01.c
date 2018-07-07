/*NOME:NAOMI TAKEMOTO TURMA: C  RA: 184849*/
/*sub 13*/
/*------------------------------------------------LABORATORIO 1 PACIENCIA --------------------------------------------------------*/



#include<stdio.h>
#include<stdlib.h>


/*-----------------------------------------------DEFINICAO DE TIPOS DE DADOS-----------------------------------------------------*/
struct Carta {
    int num;
    char naipe;
    int aberta;
};
typedef struct Carta Carta;

struct Celula{
    struct Celula *prox;
    Carta carta;
};
typedef struct Celula Celula;/*cria o tipo pilha que é uma lista ligada que trabalha sob LIFO*/



/*---------------------------------------------------------MAIN------------------------------------------------------------------*/
int main(int argc, char * argv[]){
    /*PRAGMAS DAS FUNCOES*/
    void push(Carta carta, Celula **topo);
    int pilha_vazia(Celula *topo);
    void pop (Celula ** topo);
    Celula *le_carta (Celula *topo);
    void imprime_carta(Carta carta);
    void imprime_pilha(Celula *topo);
    void imprime_topo(Celula *topo);
    int transfere_estoque_descarte(Celula *topo_estoque, Celula * topo_descarte, int *movimentos);
    int vira_carta_jogo(Celula **topo_jogo, int *movimentos);
    int  move_pilha_jogo(Celula **topo_jogo, Celula** topo_saida, int *movimentos);
    void esvazia_pilha(Celula *topo);
    int procura_A(Celula ** topo_jogo, Celula ** topo_saida,int * movimentos);
    int transfere_descarte_jogo(Celula *topo_descarte, Celula **topo_jogo,Celula *topo_estoque,Celula **topo_saida, int * movimentos);
    int move_rei( Celula **topo_jogo, Celula **topo_saida, int *movimentos);
    int sobe_carta (Celula ** topo_jogo, Celula **topo_saida, int *movimentos);
    int procura_A_descarte(Celula * topo,Celula *topo_estoque, Celula ** topo_saida,int * movimentos);
    int descarte_saida(Celula * topo_descarte,Celula *topo_estoque, Celula ** topo_saida,int * movimentos);
    
    /*VARIAVEIS DE CONTROLE*/
    int FIM = 0, i = 0, j = 0, n_cartas = 0;
    int movimentos, mov_ant, conseguiu = 0;
    
    /*CRIA PILHAS DE JOGO, SAIDA, ESTOQUE E DESCARTE*/
    Celula **topo_jogo = (Celula**)malloc(7*sizeof(Celula*));/*esse vetor contem apontadores para o topo de cada uma das pilhas de jogo*/
    Celula **topo_saida = (Celula**)malloc(4*sizeof(Celula*));/*vetor que contem apontadores para as pilhas de jogo*/
    Celula *topo_estoque = (Celula*)malloc(sizeof(Celula));
    Celula *topo_descarte = (Celula*)malloc(sizeof(Celula));
    
    /*INICIALIZA OS APONTADORES PARA INDICAR PILHA VAZIA*/
    for (i = 0; i< 7; i++){
         topo_jogo[i] = (Celula*)malloc(sizeof(Celula));
		       topo_jogo[i]->prox = NULL;/*topo_jogo[i] eh a cabeca que aponta para o prox elemento da pilha*/
    }
    (topo_estoque->prox) = NULL;
    (topo_descarte->prox) = NULL;
    for(i = 0; i<4; i++){
        topo_saida[i] = (Celula*)malloc(sizeof(Celula));
        topo_saida[i]->prox = NULL;
    }
    
    /*LE AS CARTAS DO ARQUIVO DE ENTRADA*/
    for (i = 0; i < 7; i++){
        for(j = i; j< 7; j++){
            topo_jogo[j] = le_carta(topo_jogo[j]);
        }
    }
    while(n_cartas < 24){
        topo_estoque = le_carta(topo_estoque);
        n_cartas++;
    }

    /*-----------------------------------------------------COMPUTADOR COMECA A JOGAR --------------------------------------------*/
   
    movimentos = 0;
    mov_ant = 0;
    conseguiu = 0;
    while(!FIM){
        /*MOVIMENTA DO ESTOQUE PARA O DESCARTE SE O DESCARTE ESTIVER VAZIO*/
        if(pilha_vazia(topo_descarte) && !pilha_vazia(topo_estoque)){
            conseguiu = transfere_estoque_descarte(topo_estoque,topo_descarte, &movimentos);
        }else{
            conseguiu = 0;
        }
        if(conseguiu){
            if(movimentos >= 1000){
                break;
            }
            continue;
        }
        
        /*ABRE AS CARTAS  DE JOGO*/
        conseguiu = vira_carta_jogo(topo_jogo, &movimentos);
        if(conseguiu){
            if(movimentos >= 1000){
                break;
            }
            continue;
        }
        /*VERIFICA SE AS CARTAS DE JOGO PODEM SER LEVADAS PARA A PILHA DE SAIDA CORRESPONDENTE*/
        conseguiu = sobe_carta(topo_jogo,topo_saida,&movimentos);
        if(conseguiu){
            if(movimentos >= 1000){
                break;
            }
            
            continue;
        }
        
        /*PROCURA AS NO TOPO DO ESTOQUE E TRANSFERE PARA A SAIDA ENQUANTO ESSA MOVIMENTACAO FOR VALIDA*/
        conseguiu = procura_A_descarte(topo_descarte, topo_estoque, topo_saida, &movimentos);
        if(conseguiu){
            if(movimentos >= 1000){
                break;
            }
            continue;
        }
        
        /*VERIFICA SE HA AS(es) PARA SEREM LEVADOS A PILHA DE SAIDA CORRESPONDENTE*/
        conseguiu = procura_A(topo_jogo,topo_saida,&movimentos);
        if(conseguiu){
            if(movimentos >= 1000){
                break;
            }
            continue;
        }
       
        /*MOVE CARTAS DO DESCARTE PARA A SAIDA ENQUANTO FOR POSSIVEL*/
        conseguiu = descarte_saida(topo_descarte,topo_estoque,topo_saida, &movimentos);
        if(conseguiu){
            if(movimentos >= 1000){
                break;
            }
            continue;
        }
       
        
        /*PROCURA E TRANSFERE, SE POSSIVEL, AS CARTAS DO TOPO DAS PILHAS DE JOGO PARA O TOPO DAS  PILHAS DE SAIDA*/
        conseguiu = sobe_carta(topo_jogo,topo_saida,&movimentos);
        if(conseguiu){
            if(movimentos >= 1000){
                break;
            }
            continue;
        }
        
        /*MOVE CARTAS DO DESCARTE PARA A SAIDA ENQUANTO FOR POSSIVEL*/
        conseguiu = transfere_descarte_jogo(topo_descarte,topo_jogo,topo_estoque,topo_saida,&movimentos);
        if(conseguiu){
            if(movimentos >= 1000){
                break;
            }
            continue;
        }
        
        /*CASO HAJA UMA PILHA DE JOGO VAZIA E UM REI EM UMA DAS CARTAS ABERTAS, TRANFERE O REI E SUA SUBPILHA PARA A PILHA VAZIA*/
        conseguiu = move_pilha_jogo(topo_jogo,topo_saida, &movimentos);
        if(conseguiu){
            if(movimentos >= 1000){
                break;
            }
            continue;
        }
       
        
        /*FAZ MOVIMENTACOES ENTRE PILHAS*/
         conseguiu = move_rei(topo_jogo,topo_saida, &movimentos);
        if(conseguiu){
            if(movimentos >= 1000){
                break;
            }
        }
        
        /*TRANSFERE CARTA DO ESTOQUE PARA A PILHA DE DESCARTE*/
        conseguiu = transfere_estoque_descarte(topo_estoque,topo_descarte, &movimentos);
        if(conseguiu){
            if(movimentos >= 1000){
                break;
            }
            continue;
        }
        
        /*PARA O JOGO CASO ULTRAPASSE 1000 JOGADAS OU NAO HAJA MAIS MOVIMENTACOES*/
        if( movimentos >= 1000 || movimentos - mov_ant == 0 || !conseguiu){
           break;
       }
        mov_ant = movimentos;
    }
 
    /*LIBERA A MEMORIA ALOCADA*/
    for (i = 0; i<7; i++){
        esvazia_pilha(topo_jogo[i]);
        free(topo_jogo[i]);
    }
    free(topo_jogo);
    esvazia_pilha(topo_estoque);
    free(topo_estoque);
    esvazia_pilha(topo_descarte);
    free(topo_descarte);
    for(i = 0; i<4; i++){
        esvazia_pilha(topo_saida[i]);
        free(topo_saida[i]);
    }
    free(topo_saida);
    
    /*GAME OVER*/
    printf("FIM\n");
    return 0;
}
/*-------------------------------------------------------FUNCOES-----------------------------------------------------------------*/



/*O QUE FAZ: verifica se a pilha esta vazia(topo->prox == NULL), ou se nao foi inicializado corretamente(topo == NULL)*/
int pilha_vazia(Celula *topo){
    if (topo == NULL){
        printf("Erro: apontador topo = NULL");
        exit(1);
    }
    return(topo->prox == NULL);/*retorna veradeiro se vazio e falso se nao*/
}



/*O QUE FAZ: insere um elemento no topo da pilha*/
void push(Carta carta, Celula ** topo){
    Celula *novo;
    novo = (Celula *)malloc(sizeof(Celula));/*o apontador para qual topo aponta recebe o endereco de memoria de uma celula*/
    if(novo == NULL){
        printf("Erro memoria recusada");
    }
    novo->carta = carta;
    novo->prox = (*topo)->prox;/*novo recebe NULL*/
    (*topo)->prox = novo;/*novo é um apontador para uma pilha*/
    return;
}



/*O QUE FAZ: deleta o elemento no topo na pilha*/
void pop(Celula **topo){
    Celula *deleta;
    if ((*topo) == NULL ){
        printf("Stack underflow o ponteiro nao foi inicializado\n");
    }
    else if((*topo)->prox == NULL){
       return;
    }
    else{
         deleta= (*topo)->prox;/*o primeiro elemento da pilha*/
         (*topo)->prox  = deleta->prox;
         if (deleta != NULL){
             free(deleta);
         }
    }
    return;
}



/*O QUE FAZ: le as cartas cdo arquivo de entrada,convertendo a numeracao do baralho para int e empilhando as cartas em sua pilha.*/
Celula *le_carta (Celula *topo){
    Carta carta_temp;
    char num;

    scanf("%c", &(num));
    if(num == '1'){
        getchar();/*move o cursor para nao ler o 0*/
        scanf("%c\n",&(carta_temp.naipe));
    }
    else{
        scanf("%c\n", &(carta_temp.naipe));
    }
    /*armazena o numero da carta como int*/
    switch(num){
         case 'A': carta_temp.num = 1;break;
         case '2': carta_temp.num = 2;break;
         case '3': carta_temp.num = 3;break;
         case '4': carta_temp.num = 4;break;
         case '5': carta_temp.num = 5;break;
         case '6': carta_temp.num = 6;break;
         case '7': carta_temp.num = 7;break;
         case '8': carta_temp.num = 8;break;
         case '9': carta_temp.num = 9;break;
         case '1': carta_temp.num = 10;break;
         case 'J': carta_temp.num = 11;break;
         case 'Q': carta_temp.num = 12;break;
         case 'K': carta_temp.num = 13;break;
    }
    carta_temp.aberta = 0;   /*todas as cartas lidas devem iniciar o jogo fechadas*/
    push(carta_temp, &topo);/*empilha a carta na pilha dada*/
    return topo;            /*retorna um apontador para a pilha na qual as cartas foram armazenadas*/
}

/*O QUE FAZ: dada uma carta, converte a numeracao int para a numeracao do baralho e imprime numero e naipe.*/
void imprime_carta(Carta carta){
    printf(" ");
    switch(carta.num){
        case 1 :printf("A"); break;
        case 11:printf("J"); break;
        case 12:printf("Q"); break;
        case 13:printf("K"); break;
        default: printf("%d", carta.num);
    }
    printf("%c", carta.naipe);
    return;
}



/*O QUE FAZ: percorre as pilha de jogo verificando se ha algum topo nao aberto, se sim abre a carta*/
int vira_carta_jogo(Celula **topo_jogo, int *movimentos){
    int i = 0;
    int transfere = 0;
    for ( i = 0; i<7; i++){
        /*se a pila nao estiver vazia e o topo estiver fechado abre a carta*/
        if(!pilha_vazia(topo_jogo[i]) && topo_jogo[i]->prox->carta.aberta == 0){
            transfere = 1;
            topo_jogo[i]->prox->carta.aberta = 1;
            printf("J%d J%d", i+1,i+1);
            imprime_carta(topo_jogo[i]->prox->carta);
            printf("\n");
            (*movimentos)++;
            return 1;/*retorna que houve movimentacao*/
        }
    }
    return transfere;
}



/*O QUE FAZ: imprime uma pilha do topo ate a base*/
void imprime_pilha(Celula *topo){
    Celula *ptr = topo->prox;
    if(topo->prox == NULL){
        return;
    }
    else{
        while(ptr){
            imprime_carta(ptr->carta);
            ptr = ptr->prox;
        }
        printf("\n");
    }
    return;
}

/*O QUE FAZ: libera cada celula da pilha deixando somente a cabeca.*/
void esvazia_pilha(Celula *topo){
   while((topo) != NULL && (topo)->prox!= NULL){
       pop(&topo);
   }
}

/*O QUE FAZ: modifica os apontadores das pilhas de estoque e de descarte para fazer a op de tranferencia da pilha de estoque para a de
 * descarte*/
int transfere_estoque_descarte(Celula *topo_estoque, Celula * topo_descarte, int *movimentos){
    Carta carta;
    int transfere = 0;
    if(topo_estoque->prox == NULL){
        return 0;
    }
    carta = topo_estoque->prox->carta;
    carta.aberta = 1;
    push(carta,&topo_descarte);
    pop(&topo_estoque);
    printf("E D");
    imprime_carta(carta);
    printf("\n");
    (*movimentos)++;
    transfere = 1;
    return transfere;
}

/*O QUE FAZ: verifica se o topo da pilha de descarte pode ser encaixado em alguma pilha de saida*/
int procura_A_descarte(Celula * topo_descarte,Celula *topo_estoque, Celula ** topo_saida,int * movimentos){
    int i, transfere = 0;
    Carta carta;
    char naipe[4] = {'O','C','P','E'};
    if (topo_descarte->prox != NULL){
        carta = topo_descarte->prox->carta;
    }else{
        return transfere;
    }
    while( carta.num == 1 && carta.aberta == 1){
        for (i = 0; i<4; i++){
            if(carta.naipe == naipe[i]){
                transfere = 1;
                push(carta,&topo_saida[i]);
                pop(&topo_descarte);
                printf("D S%d", i+1);
                imprime_carta(carta);
                printf("\n");
                movimentos++;
                return transfere;
            }
        }
    }
    return transfere;
}

/*O QUE FAZ: procura As(es) nas pilhas de jogo e transefere para as pilha de saida*/
int procura_A(Celula ** topo_jogo, Celula ** topo_saida,int * movimentos){
    int i, j, transferiu = 0;
    Carta carta;
    char naipe[4] = {'O','C','P','E'};
    /*percorre o vetor topo_jogo para verificar se ha um A, caso haja transfere a carta para a pilha de saida correspondente*/
    for(i = 0; i<7; i++){
        if(!pilha_vazia(topo_jogo[i])){
            carta = topo_jogo[i]->prox->carta;
            if(carta.num == 1){
               j = 0;
                while(j < 4){
                    if(carta.naipe == naipe[j]){
                        transferiu = 1;
                        push(carta,&topo_saida[j]);
                        pop(&topo_jogo[i]);
                        printf("J%d S%d",i+1, j+1);
                        imprime_carta(carta);
                        printf("\n");
                        (*movimentos)++;
                        vira_carta_jogo(topo_jogo, movimentos);
                        return transferiu;
                    }
                    j++;
                }
            }
        }
    }
    return transferiu;
}

/*O QUE FAZ: Percorre as pilhas de jogo verificando se o topo pode subir para a pilha de saida correspondente a seu naipe, enquanto */
int sobe_carta (Celula ** topo_jogo, Celula **topo_saida, int *movimentos){
    int i,j;
    Carta carta_jogo;
    int transferiu = 0;
    char naipe[4] = {'O','C','P','E'};
    for(i = 0; i<7; i++){/*perocorre as pilhas de jogo*/
        if(!pilha_vazia(topo_jogo[i])){
            if(topo_jogo[i]->prox != NULL){
                carta_jogo = topo_jogo[i]->prox->carta;
                j = 0;
                while(j<4){/*perocorre as pilhas de saida verificando se a carta da pilha de jogo pode ser encaixada*/
                    if(topo_jogo[i]->prox != NULL){
                        carta_jogo = topo_jogo[i]->prox->carta;
                    }/*se a carta puder ser encaixada, encaixa*/
                    if ( topo_saida[j]->prox != NULL && carta_jogo.num == topo_saida[j]->prox->carta.num +1 && carta_jogo.naipe == naipe[j]){
                        if(carta_jogo.aberta == 1){
                            transferiu = 1;
                            push(carta_jogo,&topo_saida[j]);
                            pop(&topo_jogo[i]);
                            printf("J%d S%d",i+1, j+1);
                            imprime_carta(carta_jogo);
                            printf("\n");
                            (*movimentos)++;
                            return transferiu;
                        }
                    }
                    j++;
                }
            }
        }
    }
    return transferiu;
}


/*O QUE FAZ: percorre as pilhas de jogo buscando uma pilha vazia, se achar procura por uma carta aberta que seja rei e transfere essa*/
int move_rei( Celula **topo_jogo,Celula **topo_saida, int *movimentos){
    int i, j, cont, transferiu = 0;
    Celula *base;/*aponta para a carta candidata a ser movida para uma pilha vazia*/
    Celula *prox;
    Carta *temp = (Carta *)malloc(13*sizeof(Carta));/*aloca um vetor auxiliar*/
    
    for (i = 0; i<7 && !transferiu; i++){
        if(topo_jogo[i]->prox == NULL){/*se a pilha estiver vazia*/
            for(j = 0; j< 7 && !transferiu; j++){
                if(i == j) continue;
                else{/*se a pilha j for diferente da pilha i*/
                    if(!pilha_vazia(topo_jogo[j])){
                        base = topo_jogo[j]->prox;
                        while(base != NULL && base->carta.aberta == 1 && topo_jogo[i]->prox == NULL){
                            if(base->carta.num == 13){
                                transferiu  = 1;
                                printf("J%d J%d",j+1,i+1);
                                (*movimentos)++;
                                cont = 0;
                                prox = topo_jogo[j]->prox;
                                while(prox != NULL && prox != base->prox){/*copia as cartas para o vetor auxiliar*/
                                    temp[cont] = prox->carta;
                                    cont++;
                                    prox = prox->prox;
                                }
                                cont = cont -1;
                                while(cont >= 0){/*empilha as cartas do vetor auxiliar para o a pilha de destino*/
                                    push(temp[cont], &topo_jogo[i]);
                                    pop(&topo_jogo[j]);
                                    imprime_carta(temp[cont]);
                                    cont = cont -1;
                                }
                                printf("\n");
                                free(temp);
                                return transferiu;
                            }
                            base = base->prox;
                        }
                    }
                }
            }
        }
    }
    if(!transferiu) free(temp);
    return transferiu;
}

/*O QUE FAZ: analiza o topo de uma pilha i, procurando dentre as cartas abertas das demais pilhas (j), uma carta que possa ser empilhada*/
int move_pilha_jogo(Celula **topo_jogo,Celula ** topo_saida, int * movimentos){
    int i , j = 0, cont = 0, vermelho_i, vermelho_j, transferiu = 0;
    Celula *base;/*eh a carta de maior valor numerico de uma pilha j que pode ser transferida para uma pilha i*/
    Celula *prox;
    Carta carta_i;
    Carta *temp = (Carta *)malloc(13*sizeof(Carta));
    for ( i = 0; i< 7; i++){
        if(pilha_vazia(topo_jogo[i])){continue;}
         carta_i = topo_jogo[i]->prox->carta;
        for(j = 0; j< 7; j++){
            if( (i != j) && !pilha_vazia(topo_jogo[i]) && !pilha_vazia(topo_jogo[j])){/*verifica se as pilhas estao vazias*/
                base = topo_jogo[j]->prox;
                while ( base != NULL && base->carta.aberta == 1 && base->carta.num < carta_i.num){
                     carta_i = topo_jogo[i]->prox->carta;
                   
                     if(carta_i.naipe == 'C' || carta_i.naipe == 'O'){
                         vermelho_i = 1;
                    }else{
                         vermelho_i = 0;
                    }
                    if(base->carta.naipe == 'O' || base->carta.naipe == 'C'){
                         vermelho_j = 1;
                    }
                    else{
                        vermelho_j = 0;
                    }
                    if(carta_i.num == (base->carta.num +1)){
                        /*se a carta da pilha i tem cor diferente da carta da pilha j*/
                       if((vermelho_i == 1 && vermelho_j == 0 )|| (vermelho_i == 0 && vermelho_j == 1)){
                            transferiu = 1;
                            prox = topo_jogo[j]->prox;
                            printf("J%d J%d", j+1, i+1);
                            (*movimentos)++;
                            
                            /*copia a pilha para um vetor auxiliar*/
                            cont = 0;
                            while(prox != NULL && prox != base->prox){
                                temp[cont] = prox->carta;
                                cont++;
                                prox = prox->prox;
                            }
                            cont = cont -1;/*cont sai do while qnd prox->prox == base->prox*/
                            while(cont >= 0){
                                push(temp[cont],&topo_jogo[i]);/*copia a carta para a pilha de destino (i)*/
                                pop(&topo_jogo[j]);/*deleta a carta da pilha de origem j*/
                                imprime_carta(temp[cont]);
                                cont--;
                                
                            }
                            printf("\n");
                           free(temp);
                           return (transferiu);
                          
                        }
                   }
                  
                  base = base->prox;/*caso nao tenha achado a carta que se encaixa perfeitamente na pilha i olha a proxima carta da pilha se ela continuar sendo menor que a carta no topo da pilha i*/
                }
            }
        }
    }
    if (!transferiu) free(temp);
    return (transferiu);
}



/*O QUE FAZ: imprime o topo da pilha*/
void imprime_topo(Celula *topo){
    Carta carta;
    if (topo == NULL){
    	exit(1);
	}
    if(topo != NULL &&topo->prox != NULL){
        carta = topo->prox->carta;
        imprime_carta(carta);
    }
}



/*O QUE FAZ: transfere cartas do descarte para as pilhas de jogo enquanto for possível.*/
int transfere_descarte_jogo(Celula *topo_descarte, Celula **topo_jogo,Celula *topo_estoque,Celula **topo_saida, int * movimentos){
    Carta carta_descarte;
    int i, vermelho_d = -1,vermelho_j = -1, transferiu = 0;
    if(topo_descarte->prox == NULL){
        return 0;
    }
    carta_descarte = topo_descarte->prox->carta;
    
    /*verifica a cor da carta no topo da pilha de descarte*/
    if(carta_descarte.naipe == 'O' || carta_descarte.naipe == 'C'){
        vermelho_d= 1;
    }else{
        vermelho_d = 0;
    }
    for(i = 0; i<7 && !transferiu; i++){
        if(topo_jogo[i]->prox != NULL){
            if(topo_jogo[i]->prox->carta.num == carta_descarte.num +1){
               
                /*verifica a cor da carta que esta na pilha de jogo*/
                if (topo_jogo[i]->prox->carta.naipe == 'O' || topo_jogo[i]->prox->carta.naipe == 'C'){
                    vermelho_j = 1;
                }else{
                    vermelho_j = 0;
                }
                /*se as cores das cartas forem diferentes a carta do descarte encaixa na pilha de jogo*/
                if((vermelho_d == 1 && vermelho_j == 0 )|| (vermelho_d == 0 && vermelho_j == 1)){
                    transferiu = 1;
                    printf("D J%d", i+1);
                    imprime_carta(carta_descarte);
                    printf("\n");
                    push(carta_descarte, &topo_jogo[i]);
                    pop(&topo_descarte);
                    (*movimentos)++;
                    break;
                }
            }
        }else if(carta_descarte.num == 13){/*se for um rei pode baixar em uma pilha vazia*/
            transferiu = 1;
            printf("D J%d", i+1);
            imprime_carta(carta_descarte);
            printf("\n");
            push(carta_descarte, &topo_jogo[i]);
            pop(&topo_descarte);
            (*movimentos)++;
        }
    }
    return transferiu;
}



/*O QUE FAZ: transfere carta do descarte para a pilha de saida correspondente*/
int descarte_saida(Celula * topo_descarte,Celula *topo_estoque, Celula ** topo_saida,int * movimentos){
    int i, movimentou = 0;
    Carta carta;
    char naipe[4] = {'O','C','P','E'};
    if (topo_descarte->prox != NULL){
        carta = topo_descarte->prox->carta;
    }else{
        return 0;
    }
    i = 0;
    while (i <4){
        if(topo_saida[i]->prox == NULL){
            i = i+i;
        }if(i > 3){
            break;
        }
        if(topo_saida[i]->prox != NULL){
            /*se os naipes correspondem e a carta da pilha de jogo eh a proxima da pilha de saida realiza a transferencia*/
            if(carta.naipe == naipe[i] &&  carta.num -1 == topo_saida[i]->prox->carta.num){
                push(carta,&topo_saida[i]);
                pop(&topo_descarte);
                printf("D S%d", i+1);
                imprime_carta(carta);
                printf("\n");
                movimentos++;
                movimentou = 1;
                return movimentou;
            }
        }
        i++;
    }
    return movimentou;/*retorna se foi ou nao possivel realizar a movimentacao*/
}
    
    
    
    
    
    
    
    
    
