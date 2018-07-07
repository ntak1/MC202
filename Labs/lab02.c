/*NOME: NAOMI TAKEMOTO    RA: 184849    TURMA:C*/


/*------------------------------------LABORATORIO 02: COMPACTADOR/DESCOMPACTADOR DE ARQUIVOS-------------------------------------*/


#include<stdio.h>
#include "bitstream.h"
#include <inttypes.h>
#define TRUE 1
#define FALSE 0

typedef unsigned char byte;

struct node{
   byte valor;
   int frequencia;
   struct node *esq, *dir;
};
typedef struct node node;


void fix_up (int p,  node ** heap);
void constroi_heap (node ** heap, int tam_heap);
void heapify(node **heap, int tam_heap, int pai);
node* remove_min(node **heap, int *tam_heap);
void constroi_dicionario(node *p, bitstream dicionario[], bitstream caminho);
void codifica_huffman(node *p,bitstreamoutput *bso);
node * decod_arvore( bitstreaminput *bsi);
void derruba_arvore(node* p);


/*-----------------------------------------------------------------MAIN----------------------------------------------------------*/

int main (int argc, char * argv[]){
    
    FILE *entrada;
    FILE *saida;
    char *operacao = argv[1];
    char *arq_entrada = argv[2];
    char *arq_saida = argv[3];
    uint32_t tam_arq = 0;
    int *frequencia;
    int n_bytes, i, j, tam_heap = 0;
    byte temp;
    node **heap,*arvore = NULL,*no0,*no1, *novo_no;
    bitstreamoutput *bso;
    bitstream *dicionario;
    bitstream caminho;
    
    int bytes_escritos = 0;
     
    
    /*--------------------------------------------COMPACTACAO DE ARQUIVOS--------------------------------------------------------*/
    if((*operacao) == 'c'){
       
        /*abre os arquivos*/
        entrada = fopen(arq_entrada, "rb"); /*modo de leitura binaria*/
        saida = fopen(arq_saida, "wb");     /*modo de escrita binaria*/
        bso = criaEncodedOutput(saida);
        
         /*monta a tabela de frequencias*/
        frequencia = (int*)calloc(256, sizeof(int));
        if(frequencia == NULL){
            return 1;
        }
        while (TRUE){
            n_bytes = fread(&temp, 1 ,1,entrada);
            if(n_bytes == 0){
                break;
            }
            frequencia[(int)temp]++;
            tam_arq++;
        }
        
        /*escreve o tamanho do arquivo de saida*/
        fwrite(&tam_arq,sizeof(uint32_t),1, saida);
        
        /*monta um min heap com as frequencias nao nulas dos bytes*/
        heap = (node**)calloc(257,sizeof(node*));/*heap eh um vetor de apontadores para o tipo node*/
        j = 1;
        for ( i = 0; i< 257; i++){
            if(frequencia[i]){
                heap[j]= (node*)malloc(sizeof(node));
                heap[j]->frequencia = frequencia[i];
                heap[j]->valor = (byte)i;
                heap[j]->esq = heap[j]->dir = NULL;
                j++;
            }
        }
        tam_heap = j;
        constroi_heap( heap, tam_heap);
        free(frequencia);
        
        /*Constroi a arvore de huffman*/
        while(tam_heap > 2){/*lembrando que a posicao 0 eh descartada*/
            
            /*remove os dois menores valores (n0 <= n1) e atualiza o tamanho do heap*/
            no0 = remove_min(heap, &tam_heap);
            no1 = remove_min(heap, &tam_heap);
            
            /*cria um no novo que possui como frequencia a soma das frequencias dos dois menores nos*/
            novo_no = (node*)malloc(sizeof(node));
            novo_no->frequencia = no0->frequencia + no1->frequencia;
            novo_no->esq = no0;
            novo_no->dir = no1;
            
            /*insere esse no no heap, de modo que o heap continue sendo um heap*/
            tam_heap++;
            heap[tam_heap-1] = novo_no;/*insere no final*/
            fix_up(tam_heap-1, heap);  /*conserta heap*/
            
        }
        
        heap = (node**)realloc(heap,2*sizeof(node*));/*apenas a primeira posicao do heap tem a arvore de huffman*/
        
     
        tam_heap = j;
        dicionario = (bitstream*)calloc(256,sizeof(bitstream));/*aloca memoria para a tabela de codigos*/
        if(dicionario == NULL){
            printf("Erro na alocacao de memoria\n");
            exit(1);
        }
        for(i = 0; i< 256; i++){
            bsClean(&(dicionario[i]));
        }
       
       
        bsClean(&caminho);
        constroi_dicionario(heap[1],dicionario, caminho);
        
        codifica_huffman(heap[1], bso);
        rewind(entrada);
        while (TRUE){
            n_bytes = fread(&temp, 1 ,1,entrada);
            if(n_bytes == 0){
                break;
            }
            encodedOutputWrite(bso,&dicionario[(int)temp]);
        }
        destroiEncodedOutput(bso);
        free(dicionario);
        derruba_arvore(heap[1]);
        free(heap);
    /*-----------------------------------------------OPERACAO DE DESCOMPACTACAO--------------------------------------------------*/    
    }else if((*operacao) == 'd'){
        /*abreo o arquivo*/
        entrada = fopen(arq_entrada, "rb"); /*modo de leitura binaria*/
        saida = fopen(arq_saida, "wb");     /*modo de escrita binaria*/
        if(entrada == NULL || saida == NULL){
          exit(1);
        }
        /*le o tamanho do arquivo*/
        fread(&tam_arq, sizeof(uint32_t),1, entrada);
        
        
        bitstreaminput *bsi = criaEncodedInput(entrada);
        
        /*monta a arvore de hffman*/
        arvore = (node*)calloc(1, sizeof(node));
        arvore = decod_arvore(bsi);
        
        
        dicionario = (bitstream*)(calloc(256,sizeof(dicionario)));
        bsClean(&caminho);
        //constroi_dicionario(arvore, dicionario, caminho);/*constroi o dicionario*/
        node * p = arvore;
        int bit;
        bytes_escritos = 0;
        while(bytes_escritos < tam_arq){
            p = arvore;
            while(p->esq != NULL && p->dir != NULL){
               bit = bsiPop(bsi);
               if(bit == 1){
                    p = p->dir;
               }else if(bit == 0){
                   p = p->esq;
               }
           }
           fwrite(&(p->valor),sizeof(byte),1,saida);
          bytes_escritos += 1;
        }
        destroiEncodedInput(bsi);
        derruba_arvore(arvore);
    }else{
        printf("Erro operacao invalida\n");
    }
    
    /*fecha os arquivos*/
    if(entrada){
        fclose(entrada);
    }
    if (saida ){
        fclose(saida);
    }
    return 0;
}
   
/*recebe um vetor de apontadores para elementos do tipo node e rearranja esse vetor ate que ele seja um heap*/
void fix_up (int p,  node ** heap) {
    node *temp;
    /*enquanto a frequencia do no pai for maior que a do no filho troca*/
    while (p > 1 && heap[p/2]->frequencia > heap[p]->frequencia){
        temp = heap[p/2];
        heap[p/2] = heap[p];
        heap[p] = temp;
        p = p/2;
    }
}

/*transforma um vetor que nao eh um heap em um heap*/
void constroi_heap (node ** heap, int tam_heap) {
   int k;
   int f;
   node *temp;
   for (k = 1; k < tam_heap -1; ++k) {                   
      /* v[1..k] Ã© um heap*/
      f = k+1;
      /*enquanto a frequencia do pai eh maior que o filho troca
       (eh equivalente a uma chamada de fix up)*/
      while (f > 1 && heap[f/2]->frequencia > heap[f]->frequencia) {  
         temp = heap[f/2];
         heap[f/2] = heap[f];
         heap[f] = temp;     
         f /= 2;                        
      }
   }
}

/*Recebe o no pai, cujos filhos direito e equerdo sao heaps e faz com que a arvore pai seja
um heap*/
void heapify(node **heap, int tam_heap, int pai) {
    node *temp;
    int filho = 2*pai;
    while (filho <= tam_heap) {
        /*se a frequencia do filho esquerdo for maior que a do filho direito escolhe o filho direito*/
        if (filho < tam_heap && heap[filho]->frequencia > heap[filho+1]->frequencia) filho++;
        /*se o pai eh menor ou igual ao filho o heap ja foi arrumado*/
        if (heap[filho/2]->frequencia <= heap[filho]->frequencia) break;
        /*caso contrario troca pai e filho*/
        temp = heap[filho/2];
        heap[filho/2] = heap[filho];
        heap[filho] = temp;
        filho *= 2;/*vai para a proxima "geracao"*/
    }
}

/*Copia a raiz do heap para um novo no e depaois a remove do heap, retornando um apotnador para o no removido
que sera utilizado para construir a arvore*/
node* remove_min(node **heap, int *tam_heap){
    node * min = (node*)calloc(1,sizeof(node));
    if((*tam_heap) == 1){
        printf("Erro: tentando deletar o ultimo no\n");/*teste*/
        return NULL;
    }
    min->frequencia = heap[1]->frequencia;
    min->valor = heap[1]->valor;
    min->esq = heap[1]->esq;
    min->dir = heap[1]->dir;
    heap[1] = heap[(*tam_heap)-1];/*coloca o ultimo elemento como nova raiz*/
    (*tam_heap)--;
    heapify(heap, *tam_heap, 1);/*conserta o heap caso o pai esteja no lugar errado*/
    return min;
}

/*percorre a arvore em preordem, guardando o caminho, quando chega a uma folha, guarda o caminho no dicionario
 isto eh associa o valor do byte com o codigo de huffman*/
void constroi_dicionario(node *p, bitstream dicionario[], bitstream caminho){
    if( p != NULL){
        bitstream temp = caminho;
        if(p->esq){
            temp = bsPush(0,caminho);
            //printf("0");
             constroi_dicionario(p->esq, dicionario,temp);
        }
        temp = caminho;
        if(p->dir){
            temp = bsPush(1,caminho);
            //printf("1");
            constroi_dicionario(p->dir, dicionario,temp);
        }
        if(p->esq == NULL && p->dir == NULL){
            if((int)p->valor < 256){
                 dicionario[(int)p->valor] = temp;/*guarda a o codigo correspondente a posicao do vetor*/
            }
           
        }
    }
}

/*codifca a arvore de huffman no arquivo*/
void codifica_huffman(node *p,bitstreamoutput *bso){
    bitstream bs;
    bsClean(&bs);
    if(p){
        if(p->esq == NULL && p->dir == NULL){
            printf("1");
            bsPushInPlace(1, &bs);
            encodedOutputWrite(bso,&bs);/*escreve bitstream*/
            encodedOutputRawWrite(bso, p->valor);
        }else{
            printf("0");
            bsPushInPlace(0,&bs);
            encodedOutputWrite(bso,&bs);/*escreve bitstream*/
            codifica_huffman(p->esq, bso);
            codifica_huffman(p->dir, bso);
        }
    }
}


/*Le o arquivo de bit a bit, e reconstroi a arvore de huffman*/
node * decod_arvore(bitstreaminput *bsi){
    int bit = bsiPop(bsi);
    node *p = (node*)calloc(1,sizeof(node));
    if(bit){                        /*se o bit encontrado for 1, chegou-se a uma folha*/
        p->valor = bsiPopByte(bsi); /*os proximos 8 bits sao o o byte nao codificado*/
        p->esq = NULL;
        p->dir = NULL;
        return p;
    }else if(bit == 0){
        p->esq = decod_arvore( bsi);
        p->dir = decod_arvore(bsi);
    }
    return p;
}

/*libera no: devolve a memoria alocada para um no*/
void derruba_arvore(node* p){
    if (p){
        derruba_arvore(p->esq);
        derruba_arvore(p->dir);
        free(p);
    }
    p = NULL;
}







