/*NOME: NAOMI TAKEMOTO    RA:184849    TURMA C*/
/*LABORATORIO 03*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX 1002

typedef char *string;

/*lista duplamente encadeada que armazena os digitos do numerao*/
typedef struct _node{
    char digito;
    struct _node *ant, *prox;
} node;

typedef struct _numerao {
    char sinal; 
    int tam;
    node *cabeca;/*o campo prox de cabeca aponta para o primeiro elemento da lista duplamente encadeada*/
                 /*o campo ant  de cabeca aponta para o ultimo elemento da lista duplamente encadeada*/
} numerao;       

/*pilha que armazena os operandos*/
typedef struct _pilha{
    numerao *elemento;
    struct _pilha *prox;
} pilha;

/*----------------------------------------------------------PRAGMAS DAS FUNCOES--------------------------------------------------*/
numerao *criaNumeraoDeString(char *str);
numerao *destroiNumerao(numerao *n);
void imprimeNumerao(numerao *n);
numerao *soma(numerao *numA, numerao *numB);
void push(numerao *num, pilha *Pilha);
numerao *pop(pilha* Pilha);
void destroiPilha(pilha *Pilha);
numerao *multiplica(numerao *numA, numerao *numB);
numerao *subtrai(numerao *numA, numerao *numB);
void insereComeco(numerao* num, char valor);
int maiorIgualAbs (numerao *numA, numerao *numB);
node *copiaNumerao(numerao *numA, numerao *resto, int tam);
numerao *divideMod(numerao *numA, numerao *numB, char op);
void excluiFim(numerao *num);

/*------------------------------------------------------------MAIN---------------------------------------------------------------*/
int main(void){
    numerao *num = NULL,*numA = NULL, *numB= NULL, *aux = NULL;
    pilha *pilhaOperando = (pilha*)malloc(sizeof(pilha));
    char fim = 0;
    string str;
    int tam = 0, numOp = 0;
    
    /*comeca com a pilha de operandos vazia*/
    pilhaOperando->elemento = NULL;
    pilhaOperando->prox = NULL;
    
    while(!fim){
        str = (string)malloc(MAX*sizeof(char));
        scanf("%s",str);getchar();
        tam = strlen(str);
        
        /*le uma linha, se o usuario declar FIM, interrompe a execucao do programa, se for uma operacao, desempliha dois
         *operandos da pilha e executa a operacao indicada, por fim se for um operando empilha para futuro uso */
        /*FIM*/
        if(str[0] == 'F'){
            break;
        }
        /*OPERACAO DE SOMA*/
        else if(str[0] == '+'){
            numB = pop(pilhaOperando);
            numA = pop(pilhaOperando);
            
            /*se numA nao eh em modulo maior que numB, troca os valores*/
            if(!maiorIgualAbs(numA, numB)){
                 aux = numA; numA = numB; numB = aux;
            }
            
            /*se os dois sao positivos*/
            if(numA->sinal == '+' && numB->sinal == '+'){
                 numA = soma(numA,numB);
            }
            
            /*se numA eh positivo e numB eh negativo*/
            else if(numA->sinal == '+' && numB->sinal == '-'){
                numA = subtrai(numA, numB);
            }
            
            /*se numA eh negativo e numB ÃƒÂ© positivo*/
            else if(numA->sinal == '-' && numB->sinal == '+'){
                numA = subtrai(numA, numB);
                numA->sinal = '-';
            }
            
            /*se  numA eh negativo e numB e negativo*/
            else {
                numA = soma(numA, numB);
                numA->sinal = '-';
            }
            
            /*se o resultado for 0 o sinal sera mudado pra +*/
            if(numA->cabeca->prox->digito == 0){
                numA->sinal = '+';
            }
            
            imprimeNumerao(numA); 
            push(numA,pilhaOperando);
            
        }/*OPERACAO DE SUBTRACAO*/
        else if(str[0] == '-' && tam == 1){
            numB = pop(pilhaOperando);
            numA = pop(pilhaOperando);
            
            /*inverte o sinal do segundo operando*/
            if(numB->sinal == '+'){
                numB->sinal = '-';
            }else{
                numB->sinal = '+';
            }
            if(!maiorIgualAbs(numA, numB)){/*se numA e em modulo menor que numB, troca a ordem*/
                aux = numA; numA = numB; numB = aux;
            }
            if(numA->sinal == '+' && numB->sinal == '+'){
                numA = soma(numA,numB);
                numA->sinal = '+';
            }/*se numA eh positivo e numB eh negativo*/
            else if(numA->sinal == '+' && numB->sinal == '-'){
                numA = subtrai(numA, numB);
                numA->sinal = '+';
            }
            /*se numA eh negativo e numB ÃƒÂ© positivo*/
            else if(numA->sinal == '-' && numB->sinal == '+'){
                numA = subtrai(numA, numB);
                numA->sinal = '-';
            }/*se  numA eh negativo e numB e negativo*/
            else {
                numA = soma(numA, numB);
                numA->sinal = '-';
            }
            if(numA->cabeca->prox->digito == 0){
                numA->sinal = '+';
            }
            push(numA,pilhaOperando);
            imprimeNumerao(numA);
            
        }/*OPERACAO DE MULTIPLICACAO*/
        else if(str[0] == '*'){
            numB = pop(pilhaOperando);
            numA = pop(pilhaOperando);
            if(numB->tam >  numA->tam){
                aux = numA;numA = numB; numB = aux;
            }
            numA = multiplica(numA,numB);
            if(numA->cabeca->prox->digito == 0){
                numA->sinal = '+';
            }
            imprimeNumerao(numA);
            push(numA,pilhaOperando);
            
        }/*OPERACAO DE DIVISAO*/
        else if (str[0] == '/'){
            numB = pop(pilhaOperando);
            numA = pop(pilhaOperando);
            numA = divideMod(numA, numB, 1);
            if(numA->cabeca->prox->digito == 0){
                numA->sinal = '+';
            }
            imprimeNumerao(numA);
            push(numA,pilhaOperando);
            
        }/*OPERACAO DE RESTO*/
        else if (str[0] == '%'){
            numB = pop(pilhaOperando);
            numA = pop(pilhaOperando);
            numA = divideMod(numA, numB, 0);
            if(numA->cabeca->prox->digito == 0){
                numA->sinal = '+';
            }
            imprimeNumerao(numA);
            push(numA,pilhaOperando);
        }
        else{/*se nao for FIM, ou um operador, a entrada eh um operando*/
            num = criaNumeraoDeString(str);
            push(num,pilhaOperando);
            numOp++;   
        }
        free(str);
    }

    destroiPilha(pilhaOperando);
    if(str) free(str);
    return 0;
}

/*O QUE FAZ: 
 *retorna 0: se o modulo de A for menor que o modulo de B
 *retorna 1: se o modulo de A for maior o modulo de B
 *retorna -1: se o modulo de A for igual ao modulo de B
*/
int maiorIgualAbs (numerao *numA, numerao *numB){
    node *digA = numA->cabeca->prox, *digB = numB->cabeca->prox;
    if(numA->tam > numB->tam){
        return 1;
    }else if(numA->tam < numB->tam){
        return 0;
    }
    while(digA){
        if (digA->digito > digB->digito){
            return 1;
        }else if(digA->digito < digB->digito){
            return 0;
        }
        digA = digA->prox;
        digB = digB->prox;
    }
    return -1;/*caso os dois sejam iguais*/
}

/*O QUE FAZ: recebe como parametros um numerao e um char que contem o valor a ser adicionado na nova celula
 *           adiciona esse valor no comeco do numerao*/
void insereComeco(numerao* num, char valor){
    node *novo;
    novo = (node*)malloc(sizeof(node));
    novo->digito = valor;
    novo->ant= NULL;
    novo->prox = num->cabeca->prox;
    if(num->cabeca->prox){
        num->cabeca->prox->ant = novo;
    }   
    else{
        num->cabeca->prox = num->cabeca->ant = novo;
    }
    num->cabeca->prox = novo;
    num->tam ++;
}

/*O QUE FAZ: recebe como parametros um numerao e um char que contem o valor a ser adicionado na nova celula
 *            adiciona esse valor no fim do numerao*/
void insereFim(numerao * num, char valor){
    node *novo;
    novo = (node*)malloc(sizeof(node));
    novo->digito = valor;
    novo->prox = NULL;
    novo->ant = num->cabeca->ant;
    if(num->cabeca->ant){
        num->cabeca->ant->prox = novo;
    }else{/*para o caso em que numerao esta vazio*/
        num->cabeca->ant = num->cabeca->prox = novo;
    }
    num->cabeca->ant = novo;
    num->tam ++;
    
}


    
/*O QUE FAZ: rebece um ponteiro para um veto de char's(string) e cria um elemento do tipo numerao, alem disso devolve a memoria a
             locada pra str.
 *RETORNA:  um ponteiro para o elemento criado
*/
numerao *criaNumeraoDeString(string str){
    int i = 0;
    numerao *num = (numerao*)malloc(sizeof(numerao));
    num->cabeca = (node*)malloc(sizeof(node));
    num->tam = 0;
    num->cabeca->ant = num->cabeca->prox = NULL;
    if(str[0] == '-') {
        num->sinal = '-';
        i++;
    }
    else{
        num->sinal = '+';
    }
    if(str[0] == '\0'){
        num->sinal = '+';
        return num;
    }
    /*cria um no para cada digito contido na string*/
    while(str[i] != '\0'){
        insereFim(num, str[i]-48);
        i++;
    }
    return num;
}

/*O QUE FAZ: recebe um ponteiro para um elemento do tipo numerao, percorre esse elemento imprimindo seus digitos do mais para o 
             menos significativo
*/
void imprimeNumerao(numerao *n){
    node * aux =  n->cabeca->prox;
    if(n ->sinal == '-' && n->cabeca->prox->digito != 0) printf("%c",'-');
    while(aux){
        printf("%c", aux->digito + 48);
        aux = aux->prox;
    }
    printf("\n");
    return;
}

/*O QUE FAZ: recebe um ponteiro para um elemento do tipo numerao e devolve a memoria alocada para o sistema*/    
numerao *destroiNumerao(numerao *n){
    node *aux;
    while(n->cabeca->prox){
        aux = n->cabeca->prox;
        n->cabeca->prox = n->cabeca->prox->prox;
        free(aux);
    }
    free(n->cabeca);
    free(n);
    n = NULL;
    return n;
}

/*O QUE FAZ: soma os numeroa numA e numB e guarda o resultado em numA, esta funcao assume que o numero de digitos de numA eh
 *           maior ou igual ao numero de digitos de numB*/
numerao* soma (numerao *numA, numerao *numB){

    node *digA, *digB;
    char resultado = 0, fim = 0, vai1= 0,somaB = 0;
    digA = numA->cabeca->ant;
    digB = numB->cabeca->ant;
    somaB = digB->digito;
    
    /*percorre os numeros somando digito a digito, como na soma manual*/
    while(!fim){
        resultado = digA->digito + somaB + vai1;
        vai1 = resultado/10;
        digA->digito = resultado%10;
        digA = digA->ant;
        if(digB->ant){             /*enquanto b for tiver um digitos o valor a ser somada com o digito de A e somaB*/
             digB = digB->ant;
             somaB = digB->digito;
        }else{                    /*quando nao houver mais digitos em b correspondentes em A*/
            somaB = 0;
        }
        if(digA == NULL) {
            if(vai1 != 0){
                insereComeco(numA, vai1);
            }
            break;
        }
    }
    numB = destroiNumerao(numB);
    return numA;
    
}


/*O QUE FAZ: multplica os numeroa numA e numB e guarda o resultado em numA, esta funcao assume que o numero de digitos de numA eh
 *           maior ou igual ao numero de digitos de numB*/
numerao *multiplica(numerao *numA, numerao *numB){

    node *digA, *digB;
    numerao *aux, *produto1, *produto2;
    int ad0 = 0,i = 0;/*ad0 conta a ordem, potencia de 10 relacionada ao digito B que esta sendo tomado*/
    char resultado = 0, vaiN= 0;
    
    produto1 = criaNumeraoDeString("");/*produto comeca inicialmente com 0*/
    produto2 = criaNumeraoDeString("");/*produto comeca inicialmente com 0*/
    digA = numA->cabeca->ant;
    digB = numB->cabeca->ant;
    
    /*Ajusta o sinal do resultado*/
    
    if(numA->cabeca->prox->digito == 0 || numB->cabeca->prox->digito == 0){
        insereComeco(produto1, 0);
        produto1->sinal = '+';
    }else{
        /*multipla cada digito de numB por numA*/
         while(digB){
             if(ad0 == 0){/*amazena o resultado do produto do digito menos sgnificativo de b em produto1*/
                 while(digA){
                     resultado = (digA->digito) *(digB->digito)+vaiN;
                     vaiN = resultado/10;
                     insereComeco(produto1, resultado%10);
                     digA = digA->ant;
                     if(digA == NULL && vaiN != 0){
                         insereComeco(produto1,vaiN);
                     }
                }
                digB = digB->ant;
                ad0++;
            }
            else{/*calcula o produto dos demais digitos de b por numA e armazena o resultado em produto2*/
                vaiN = 0;
                digA = numA->cabeca->ant;
                while(digA){
                    resultado = (digA->digito) *(digB->digito)+vaiN;
                    vaiN = resultado/10;
                    insereComeco(produto2,resultado%10);
                    digA = digA->ant;
                    if(digA == NULL && vaiN != 0){
                        insereComeco(produto2, vaiN);
                    }
                }
                i = ad0;
                while(i){/*multplica produto2 pela potencia de 10 correspondente a ordem o digito b que esta multplcando numA*/
                    insereFim(produto2, 0);
                    i--;
                }/*Soma produto1 e produto2, armazenando o resultado no numero que contem mais digitos*/
                if(produto2->tam > produto1->tam){
                    aux = produto1; produto1 = produto2; produto2 = aux;
                }
                produto1 = soma(produto1, produto2);
                produto2 = criaNumeraoDeString("");
                ad0++;
                digB = digB->ant;
            }
        }
    }
    if(numA->sinal == numB->sinal){
        produto1->sinal = '+';
    }else{
        produto1->sinal = '-';
    }/*Se um dos operandos for 0, a o resultado da multiplicacao eh 0*/
    numA = destroiNumerao(numA);
    numB = destroiNumerao(numB);
    produto2 = destroiNumerao(produto2);
    return produto1;
}

/*O QUE FAZ: subtrai os numeroa numA e numB e guarda o resultado em numA, esta funcao assume que o numero de digitos de numA eh
 *           maior ou igual ao numero de digitos de numB*/
numerao *subtrai(numerao *numA, numerao *numB){
	   node  *digA, *digB, *auxA, *temp = NULL;
    char resultado = 0, fim = 0,subB = 0;
    digA = numA->cabeca->ant;
    digB = numB->cabeca->ant;
    subB = digB->digito;
    
    /*percorre os numeros somando digito a digito, como na soma manual*/
    while(!fim){
        /*se o digito de numA for maior ou igual ao digito do numero B, subtrai normalmente os digitos*/
        if(digA->digito >= subB){           
            resultado = digA->digito - subB;
            digA->digito = resultado%10;
            /*muda para o digito de maior importancia (para esquerda)*/
            if(digA->ant) digA = digA->ant;
            else{
                break;
            }
            if(digB->ant){            
                digB = digB->ant;
                subB = digB->digito;
            }else{                  
                subB = 0;
            }
        }
        /*se o digito de numA for menor*/
        if(digA->digito < subB) {
            if(digA->ant)
                auxA = digA->ant;/*procura um outro digito a esquerda que possa "emprestar" uma unidade*/
            while(auxA->ant  && (auxA->digito - 1 < 0)){
                auxA = auxA->ant;
            }
            temp = auxA;
            if(auxA){
                auxA->digito = auxA->digito - 1; /*quando acha, subtrai 1 desse digito*/
                temp = auxA->prox; 
            }
            while(temp != NULL && temp != digA){ /*"volta" ate digA que, precisava do emprestimo*/
                temp->digito = 10+ temp->digito - 1;
                temp = temp->prox;   
            }
            digA->digito = 10 + digA->digito - subB;
            /*muda para o digito de maior importancia (para esquerda)*/
            if (digA->ant)
                digA = digA->ant;
            else {break;}
            if(digB->ant){            
                digB = digB->ant;
                subB = digB->digito;
            }else{                    
                subB = 0;
            }
        }
        if(!digA ) break;
    }
    
    /*exclui os 0's a esquerda*/
    while(numA->cabeca->prox->digito == 0 && numA->cabeca->prox->prox != NULL){
        auxA = numA->cabeca->prox;
        numA->cabeca->prox = numA->cabeca->prox->prox;
        numA->cabeca->prox->ant = NULL;
        numA->tam --;
        free(auxA);
    }
    if(numA->cabeca->prox->digito == 0){
        numA->sinal = '+';
    }
    numB = destroiNumerao(numB);
    return numA;
}


/*O QUE FAZ: recebe dois numeroes, numA e resto (que esta inicialmente vazio), copia os valores de numA para resto
 *            e retorna um ponteiro do tipo node que aponta para o proximo digito (nao copiado) de numA*/
node* copiaNumerao(numerao *numA, numerao *resto, int tam){
    int i = 0;
    node *digA = numA->cabeca->prox;
    while(digA != NULL && i< tam){
        insereFim(resto,digA->digito);
        digA = digA->prox;
        i++;
    }
    return digA;
}

/*O QUE FAZ: 
 *  Se op for igual a 1 calcula e retorna o quociente da divisao inteira  dos valores absolutos de numA e numB
 *  Se op for igual a 0 calcula e retorna o resto da divisao inteira dos valores absolutos de numA e numB
 */
numerao *divideMod(numerao *numA, numerao *numB, char op){
    int  tamB = numB->tam;
    node *digA = NULL,*digB = NULL;
    numerao *resto,*q, *copNumB, *copResto, *qaux; 
    int bool, baixou = 0, j;
    char  qTent = 0;
    q = criaNumeraoDeString("");
    qaux = criaNumeraoDeString("");
    resto = criaNumeraoDeString("");
    copNumB = criaNumeraoDeString("");
    copResto = criaNumeraoDeString("");
    digB = numB->cabeca->prox;
    digA = numA->cabeca->prox;
    
    /*se numA eh menor que numB logo o quociente eh  0*/
    bool = maiorIgualAbs(numA, numB);
    if(bool == 0){
        if(op == 1){
            numA = destroiNumerao(numA);
            numB = destroiNumerao(numB);
            qaux = destroiNumerao(qaux);
            resto = destroiNumerao(resto);
            copResto = destroiNumerao(copResto);
            copNumB = destroiNumerao(copNumB);
            insereComeco(q, 0);
            return q;
        }else{
            copiaNumerao(numA, resto, numA->tam);
            resto->sinal = numA->sinal;
            numA = destroiNumerao(numA);
            numB = destroiNumerao(numB);
            qaux = destroiNumerao(qaux);
            q = destroiNumerao(q);
            copResto = destroiNumerao(copResto);
            copNumB = destroiNumerao(copNumB);
            return resto;
        }
    }
    else{ /*se numA eh em modulo maior que numB e tem mais digitos que este*/
        
        digA = copiaNumerao(numA, resto, tamB);
        bool = maiorIgualAbs(resto, numB);
        if(bool == 0){
            insereFim(resto,digA->digito);
            digA = digA->prox;
        }
        copiaNumerao(resto, copResto, resto->tam);
        copiaNumerao(numB, copNumB, tamB);
        /*Percorre o numA "baixando" seus digitos conforme vai dividindo o numero*/
        
        while(1){
            int j;
            j = maiorIgualAbs(resto,numB);
            if(resto->tam > numB->tam){
                qTent = ((resto->cabeca->prox->digito)*10 + (resto->cabeca->prox->prox->digito));
                qTent = qTent/(numB->cabeca->prox->digito + 1);
            }
            else{
                qTent = (resto->cabeca->prox->digito)/(numB->cabeca->prox->digito + 1);
            }

            while(1){
                qaux = criaNumeraoDeString("");
                if(qTent > 9){
                    insereFim(qaux,qTent/10);
                    insereFim(qaux,qTent%10);   
                }else{
                    insereFim(qaux, qTent);   
                }
                if(maiorIgualAbs(copNumB,qaux)) {
                     copNumB = multiplica(copNumB,qaux); qaux = NULL;  
                }
                else{
                    copNumB = multiplica(qaux,copNumB);/*q aux morre aqui*/
                }
                copResto = subtrai(copResto, copNumB);
                
                bool = maiorIgualAbs(copResto, numB);qaux = NULL; 
                if(bool == 0){/*Se o novo resto for menor que numB*/
                    qaux = criaNumeraoDeString("");
                    if(qTent > 9){
                        insereFim(qaux,qTent/10);
                        insereFim(qaux,qTent%10);
                        insereFim(q,qTent/10);
                        insereFim(q,qTent%10);
                    }else{
                        insereFim(qaux, qTent);
                        insereFim(q, qTent);
                    }
                    break;
                    
                }else if(bool == -1){
                    qTent++;
                    qaux = criaNumeraoDeString("");
                    if(qTent > 9){
                        insereFim(qaux,qTent/10);
                        insereFim(qaux,qTent%10);
                        insereFim(q,qTent/10);
                        insereFim(q,qTent%10);
                    }else{
                        insereFim(qaux, qTent);
                        insereFim(q, qTent);
                    }
                    
                    /*atualiza o resto*/
                    copResto = destroiNumerao(copResto);
                    copResto = NULL;
                    copResto = criaNumeraoDeString("");
                    insereFim(copResto,0);
                    break;
                    
                }else{
                    
                    qaux = criaNumeraoDeString("");
                    qTent++;/*incrementa a tentativa*/
                   
                    if(qTent > 9){
                        insereFim(qaux,qTent/10);
                        insereFim(qaux,qTent%10);
                    }else{
                        insereFim(qaux, qTent);
                    }
                    copResto = destroiNumerao(copResto);
                    copNumB = destroiNumerao(copNumB);
                    copNumB = criaNumeraoDeString("");
                    copResto = criaNumeraoDeString("");
                    copiaNumerao(numB, copNumB, numB->tam);
                    copiaNumerao(resto, copResto, resto->tam);
                }
            }
            /*depois que achou o q correto, baixa os digitos de numA ate que o resto fique maior ou igual a numB*/
            /*atulaiza o valor do resto*/
            resto = destroiNumerao(resto); resto = NULL;
            resto = criaNumeraoDeString("");
            copiaNumerao(copResto, resto, copResto->tam);/*atualiza o valor de resto*/
            copNumB = criaNumeraoDeString("");
            copiaNumerao(numB, copNumB, numB->tam);
            /*insere digito de A em resto*/
            baixou = 0;
            j = maiorIgualAbs(resto,numB);
            while(digA && j == 0){/*se diA eh nao nulo e o resto eh menor que numB*/
                j = maiorIgualAbs(resto,numB);
                while(j == 0 && digA){
                        if(resto->cabeca->prox->digito == 0){
                            resto->cabeca->prox->digito = digA->digito;
                            copResto->cabeca->prox->digito = digA->digito;
                             baixou ++;
                             digA = digA->prox;
                        }else{
                            insereFim(resto, digA->digito);
                            insereFim(copResto, digA->digito);
                            digA = digA->prox;
                            baixou ++;
                        }
                    j = maiorIgualAbs(resto,numB);
                    if(j == 0)
                    insereFim(q,0);
                }
            }
            if(!digA){
                j = maiorIgualAbs(numB,resto);
                if(j == 1){/*se numB eh maior que o resto*/
                     break;
                }  
            }
        }
    }
    resto->sinal = numA->sinal;
    /*Arruma o sinal*/
    if(numA->sinal != numB->sinal){
        q->sinal =  '-';
    }else{
        q->sinal = '+';
    }
    
    numA = destroiNumerao(numA);
    numB = destroiNumerao(numB);
    if(qaux){
        qaux = destroiNumerao(qaux);
    }
    if(copNumB){
        copNumB = destroiNumerao(copNumB);
    }
    if(op == 1){
        copResto = destroiNumerao(copResto);
        resto = destroiNumerao(resto);
        return q;
    }
    else{
        q = destroiNumerao(q);
        return resto;
    }
}

/*recebe um ponteiro para um elemento do tipo numerao e o endereco de memoria da pilha e empliha o elemento na pilha*/
void push(numerao *num, pilha *Pilha){
    pilha *novo = (pilha*)malloc(sizeof(pilha));
    novo->prox = Pilha->prox;
    Pilha->prox = novo;
    novo->elemento = num;
}

/*Exclui o topo da pilha*/
numerao* pop(pilha * Pilha){
    pilha *deleta;
    numerao * num;
    if(Pilha->prox){
        deleta = Pilha->prox;
        num = deleta->elemento;
        deleta->elemento = NULL;
        Pilha->prox  = deleta->prox;
        free(deleta);/*libera a memnoria da celula*/
        return num;
    }
    else{
        printf("Entrada invalida !\n");
        exit(1);
    }
    
}


void destroiPilha(pilha *Pilha){
    pilha *deleta = Pilha->prox;
    while(deleta){
        Pilha->prox = deleta->prox;
        free(deleta->elemento);
        free(deleta);
        deleta = Pilha->prox;
    }
    free(Pilha);
}










