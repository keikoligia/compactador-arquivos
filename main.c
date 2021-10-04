#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

typedef struct noArvore{
struct noArvore *dir;
struct noArvore *esq;
byte b;
int freq;
} noArvore;

typedef struct noLista{
noArvore *noArv;
struct noLista *prox;
}noLista;

typedef struct lista{
noLista *inicio;
byte tamanho;
} lista;

noArvore *novoNoArv(unsigned char c, int freq, noArvore *esq, noArvore *dir)
{
    noArvore *novo;
    if ((novo = malloc(sizeof(*novo))) == NULL)return NULL;

    novo->c = c;
    novo->freq = freq;
    novo->esq = esq;
    novo->dir = dir;

    return novo;
}

noLista *novoNoLista(noArvore *noArv)
{
    noLista *novo;
    if ((novo = malloc(sizeof(*novo))) == NULL) return NULL;

    novo->no = noArv;
    novo->prox = NULL;

    return novo;
}

int geraBit(FILE *entrada, int posicao, byte *aux )
{
    if(posicao % 8 == 0)
    {
        fread(aux, 1, 1, entrada);
    }

    return !!((*aux) & (1 << (posicao % 8)));
}

int geraBit(FILE *entrada, int posicao, byte *aux )
{
    if(posicao % 8 == 0)
    {
        fread(aux, 1, 1, entrada);
    }

    return !!((*aux) & (1 << (posicao % 8)));
}

noLista *insereNoLista(noLista *no, lista *list)
{
    if (!list->inicio)
        list->inicio = n; //primeira pos da lista recebe o argum
    else
        if(no->freq < list->inicio->no->freq)
        {
            n->prox = list->inicio;
            list->inicio = n;
        }
    else
    {
        noLista *aux = list->inicio->prox;
        noLista *aux2 = l->inicio;

        while (aux && aux->no->freq <= n->no->freq)
        {
            aux2 = aux;
            aux = aux2->prox;
        }

        aux2->prox = n;
        n->prox = aux;
    }

    l->qtd++;
}

void obterFreqByte(FILE *arq, unsigned int *lista)
{
    unsigned char ch;

    while (fread(&ch, 1, 1, arq))
        lista[(unsigned char)ch]++;

    rewind(arq);
}

noArvore *fazerArvore(unsigned *lista)
{
    lista list = {0; NULL}; //inicio e tamanho


}

void descompactar()
{
    FILE *arqComp;
    FILE *arqDesc;
    char nomeArqComp[20];
    char nomeArqDesc[20];
    unsigned lBytes[256] = {0};

    printf("Digite o nome do arquivo a ser descompactado: \n");
    scanf("%s", nomeArqComp);

    if (arqComp == NULL)
        printf("Arquivo não encontrado!");

    print("Digite o nome do arquivo que guardará a compactação: \n");
    scanf("%s", nomeArqDesc);

    if (arqComp == NULL)
        printf("Por favor digite corretamente o nome do arquivo!");

    fread(lBytes, 256, sizeof(lBytes[0]), arqComp)
    fazerArvore(*lBytes);
}
