#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

//metodos e definições relacionadas a arvore
typedef unsigned char byte;

typedef struct noArvore{
struct noArvore *direita;
struct noArvore *esquerda;
byte b;
int frequencia;
} noArvore;

typedef struct noLista{
noArvore *noArv;
struct noLista *proximo;
}noLista;

typedef struct lista{
noLista *inicio;
byte tamanho;
} lista;

noArvore *criaNoArvore(noArvore *dir, noArvore *esq, byte bn, int freq)
{
    noArvore *novo;

    if ( ( novo = malloc(sizeof(*novo)) ) == NULL )
    {
        novo = NULL;
        return novo;
    }
    novo->b = bn;
    novo->esquerda = esq;
    novo->direita = dir;
    novo->frequencia = freq;
}

void limpaArvore (noArvore *no)
{
    if(!no)
        return;
    else
    {
    noArvore *direira = no->direita;
    noArvore *esquerda = no->esquerda;
    free(no);
    limpaArvore(no->direita);
    limpaArvore(no->esquerda);
    }
}

noArvore *retiraEmNoArvore (lista *l)
{
    noLista *aux = l->inicio;
    l->inicio = aux->proximo;
    noArvore *saida = l->inicio->noArv;
    free(aux);
    aux = NULL;
    l->tamanho--;
    return saida;

}

noLista *novoNoLista( noArvore *no)
{
    noLista *novo;

    if((novo = malloc(sizeof(*novo))) == NULL) return NULL;

    novo->noArv = no;
    novo->proximo = NULL;

    return novo;
}

void insereNaLista(lista *l, noLista *no)
{
    if(!l->inicio)
        l->inicio = no;
    else if(no->noArv->frequencia < l->inicio->noArv->frequencia)
    {
        no->proximo = l->inicio;
        l->inicio = no;
    }
    else
    {
        noLista *aux = l->inicio->proximo;
        noLista *aux2 = l->inicio;
        while(aux && aux->noArv->frequencia <= no->noArv->frequencia)
        {
            aux2 = aux;
            aux2 = aux->proximo;
        }
        aux2->proximo = no;
        no->proximo = aux;
    }
    l->tamanho++;
}
noArvore *criaArvore(unsigned int *arrayDeBytes)
{
    lista l = {NULL, 0};

    for(int i = 0; i < sizeof(byte); i++)
    {
        if(arrayDeBytes[1])
        {
            insereNaLista(&l,novoNoLista(criaNoArvore(NULL,NULL,i,arrayDeBytes[i])));
        }
    }
    while( (l.tamanho) > 1)
    {
        noArvore *dir = retiraEmNoArvore(&l);
        noArvore *esq = retiraEmNoArvore(&l);
        noArvore *pai = criaNoArvore(dir,esq,'¨',dir->frequencia + esq->frequencia);
        insereNaLista(&l,novoNoLista(pai));
    }
    return retiraEmNoArvore(&l);
}

void preencheVetor(FILE *arq, unsigned int *vetorBytes)
{
    byte b;
     while (fread(&b, 1, 1, arq) >= 1)
    {
        vetorBytes[(byte)b]++;
    }

    rewind(arq);
}

bool retiraCodigo (noArvore *n, byte b, char *buffer, int tamanho)
{
    if (!(n->esquerda || n->direita) && n->b == b)
    {
        buffer[tamanho] = '\0';
        return true;
    }

    else{
        bool encontrado = false;

         if (n->esquerda)
        {
            buffer[tamanho] = '0';
            encontrado = retiraCodigo(n->esquerda, b, buffer, tamanho + 1);
        }

        if (!encontrado && n->direita)
        {
            buffer[tamanho] = '1';
            encontrado = retiraCodigo(n->direita, b, buffer, tamanho + 1);
        }
        if (!encontrado)
        {
            buffer[tamanho] = '\0';
        }

        return encontrado;
    }
}
// metodos e definições de funcionamento do programa
int geraBit(FILE *entrada, int posicao, byte *aux )
{
    if(posicao % 8 == 0)
    {
        fread(aux, 1, 1, entrada);
    }

    return !!((*aux) & (1 << (posicao % 8)));
}

void descompactar (const char *compactado, const char *descompactado)
{
    unsigned arrayBytes[sizeof(byte)] = {0};

    FILE *arqCompactado = fopen(compactado, "rb");
    if(!arqCompactado)
    {
        printf("O arquivo não foi encontrado");
    }

    FILE *arqDescompactado = fopen(descompactado, "wb");
    if(!arqDescompactado)
    {
        printf("O arquivo não foi encontrado");
    }

    fread(arrayBytes,256,sizeof(arrayBytes[0]),arqCompactado);

    noArvore *arvore = criaArvore(arrayBytes);
    unsigned tamanho;
    unsigned posicao = 0;
    fread(&tamanho, 1, sizeof(tamanho), arqCompactado);
    byte aux = 0;

    while (posicao < tamanho)
    {
        noArvore *noAtual = arvore;

        while ( noAtual->esquerda || noAtual->direita )
        {
            noAtual = geraBit(arqCompactado, posicao++, &aux) ? noAtual->direita : noAtual->esquerda;
        }

        fwrite(&(noAtual->b), 1, 1, arqDescompactado);
    }

    limpaArvore(arvore);
    fclose(arqCompactado);
    fclose(arqDescompactado);
}

void compactar (const char *descompactado, const char *compactado)
{
 unsigned listaBytes[256] = {0};

 FILE *arqCompactado = fopen(compactado, "wb");
    if(!arqCompactado)
    {
        printf("O arquivo não foi encontrado");
    }

    FILE *arqDescompactado = fopen(descompactado, "rb");
    if(!arqDescompactado)
    {
        printf("O arquivo não foi encontrado");
    }

    preencheVetor(arqDescompactado, listaBytes);

    noArvore *arvore = criaArvore(listaBytes);

    fwrite(listaBytes,256, sizeof(listaBytes),arqCompactado);

    fseek(arqCompactado, sizeof(unsigned int), SEEK_CUR);

    byte b;
    unsigned tamanho = 0;
    byte aux = 0;
    while(fread(&b, 1,1,arqDescompactado) >= 1)
    {
        char buffer[1024] = {0};
        retiraCodigo(arvore, b, buffer, 0);

        for(char *i = buffer; *i; i++)
        {

            if(*i == '1')
            {
                aux = aux | (1<<(tamanho % 8));
            }
            if(tamanho % 8 == 0)
            {
                aux = 0;
            }
        }
    }

    fwrite(&aux,1,1 ,arqCompactado);
    fseek(arqCompactado,sizeof(unsigned int), SEEK_SET);
    fwrite(&tamanho,1,sizeof(unsigned),arqCompactado);


    limpaArvore(arvore);
    fclose(arqCompactado);
    fclose(arqDescompactado);

}


int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        printf("Uso: main [OPCAO] [ARQUIVO] [ARQUIVO]\n\n");
        printf("Opcoes:\n");
        printf("\t-c\tComprime\n");
        printf("\t-x\tDescomprime\n");
        printf("\nExemplo: ./main -c comprima.isso nisso.hx\n");

        return 0;
    }

    if (strcmp("-c", argv[1]) == 0)
    {
        if (strstr(argv[3], ".hx"))
        {
            compactar(argv[2], argv[3]);
        }
        else
        {
            printf("O arquivo resultante da compressao deve ter a extensao '.hx'.\n");
            printf("Exemplo: \n\t./main -c comprima.isso nisso.hx\n");

            return 0;
        }
    }
    else if (strcmp("-x", argv[1]) == 0)
    {
        if (strstr(argv[2], ".hx"))
        {
            descompactar(argv[2], argv[3]);
        }
        else
        {
            printf("O arquivo a ser descomprimido deve ter a extensao '.hx'.\n");
            printf("Exemplo: \n\t./main -x descomprime.hx nisso.extensao\n");

            return 0;
        }
    }
    else
    {
        printf("Uso: main [OPCAO] [ARQUIVO] [ARQUIVO]\n\n");
        printf("Opcoes:\n");
        printf("\t-c\tComprime\n");
        printf("\t-x\tDescomprime\n");
        printf("\nExemplo: ./main -c comprima.isso nisso.hx\n");

        return 0;
    }

    return 0;
}
