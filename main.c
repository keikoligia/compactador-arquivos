#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

typedef struct noArvore
{
    int freq;
    unsigned char c;
    struct noArvore *esq;
    struct noArvore *dir;
} noArvore;

typedef struct noListaLigada
{
    noArvore *no;
    struct noListaLigada *prox;
} noListaLigada;

typedef struct lista
{
    noListaLigada *inicio;
    int qtd;
} lista;

noArvore *novoNoArv(unsigned char c, int freq, noArvore *esq, noArvore *dir)
{
    noArvore *novo;
    if ((novo = malloc(sizeof(*novo))) == NULL)
        return NULL;

    novo->c = c;
    novo->freq = freq;
    novo->esq = esq;
    novo->dir = dir;

    return novo;
}

noListaLigada *novoNoLista(noArvore *noArv)
{
    noListaLigada *novo;
    if ((novo = malloc(sizeof(*novo))) == NULL) 
        return NULL;

    novo->no = noArv;
    novo->prox = NULL;

    return novo;
}

int gerarBit(FILE *arq, int pos, unsigned char *aux)
{
    if(pos % 8 == 0)
    {
        fread(aux, 1, 1, arq);
    }

    int result = ((*aux) & (1 << (pos % 8))); 

    if(result != 0) return 1;
    else
        return 0;
}

void insereNoFila(noListaLigada *n, lista *l)
{
    if (!l->inicio)
        l->inicio = n;

    else if (n->no->freq < l->inicio->no->freq)
    {
        n->prox = l->inicio;
        l->inicio = n;
    }
    else
    {
        noListaLigada *aux = l->inicio->prox;
        noListaLigada *aux2 = l->inicio;

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
    unsigned char c;

    while (fread(&c, 1, 1, arq))
        lista[(unsigned char)c]++;

    fseek(arq, 0, SEEK_SET);
}

noArvore *criaSubArvore(lista *list)
{
    noListaLigada *noListaLigada = list->inicio;
    noArvore *noArv = noListaLigada->no;

    list->inicio = noListaLigada->prox;

    free(noListaLigada);
    noListaLigada = NULL;

    list->qtd--;

    return noArv;
}

noArvore *fazerArvore(unsigned *list)
{
    lista l = {NULL, 0};

    for (int i = 0; i < 256; i++)
        if (list[i])
            insereNoFila(novoNoLista(novoNoArv(i, list[i], NULL, NULL)), &l);

    while (l.qtd > 1)
    {
        noArvore *noEsq = criaSubArvore(&l);
        noArvore *noDir = criaSubArvore(&l);

        noArvore *soma = novoNoArv('#', noEsq->freq + noDir->freq, noEsq, noDir);

        insereNoFila(novoNoLista(soma), &l);
    }

    return criaSubArvore(&l);
}

bool buscaCodigoByte(noArvore *no, unsigned char c, char *buffer, int tam)
{
    if (!(no->esq || no->dir) && no->c == c)
    {
        buffer[tam] = '\0';
        return true;
    }
    else
    {
        bool codEncontrado = false;

        if (no->esq)
        {
            buffer[tam] = '0';
            codEncontrado = buscaCodigoByte(no->esq, c, buffer, tam + 1);
        }

        if (!codEncontrado && no->dir)
        {
            buffer[tam] = '1';
            codEncontrado = buscaCodigoByte(no->dir, c, buffer, tam + 1);
        }

        if (!codEncontrado)
            buffer[tam] = '\0';

        return codEncontrado;
    }
}

void descompactar()
{
    FILE *arqComp;
    FILE *arqDesc;
    char nomeArqComp[20];
    char nomeArqDesc[20];
    unsigned lBytes[256] = {0};
    unsigned posicao = 0;
    unsigned char aux = 0;

    printf("Digite o nome do arquivo a ser descompactado: \n");
    scanf("%s", nomeArqComp);
    arqComp = fopen(nomeArqComp, "rb");

    if (arqComp == NULL)
        printf("Arquivo não encontrado!");

    printf("Digite o nome do arquivo que guardará a compactação: \n");
    scanf("%s", nomeArqDesc);
    arqDesc = fopen(nomeArqDesc, "wb");

    if (arqDesc == NULL)
        printf("Por favor digite corretamente o nome do arquivo!");

    fread(lBytes, sizeof(unsigned int), 256,  arqComp);

    noArvore *arvore = fazerArvore(lBytes);

    unsigned tamanho;

    fread(&tamanho, sizeof(tamanho), 1, arqComp);
    
    //guarda, tamanho em bytes da unidade a ser lida, total, arquivo
    
    while (posicao < tamanho)
    {
        noArvore *noAtual = arvore;

        //enquanto o nó nao for folha
        while (noAtual->esq || noAtual->dir)
        {
            if(gerarBit(arqComp, posicao++, &aux))
                noAtual = noAtual->dir;
            else
                noAtual = noAtual->esq;
        }

        fwrite(&(noAtual->c), 1, 1, arqDesc);
    }
    
    fclose(arqComp);
    fclose(arqDesc);
}

void compactar()
{
     FILE *arqParaComp;
    FILE *arqComp;
    char nomeArqParaComp[20];
    char nomeArqComp[20];
    unsigned lBytes[256] = {0};
    unsigned char c;
    unsigned tamanho = 0;
    unsigned char aux = 0;

    printf("Digite o nome do arquivo a ser compactado: \n");
    scanf("%s", nomeArqParaComp);

    arqParaComp = fopen(nomeArqParaComp, "rb");

    if (arqParaComp == NULL)
        printf("Arquivo não encontrado!");

    printf("Digite o nome do arquivo que guardará a compactacao: \n");
    scanf("%s", nomeArqComp);

    arqComp = fopen(nomeArqComp, "wb");

    if (arqComp == NULL)
        printf("Por favor digite corretamente o nome do arquivo!");

    obterFreqByte(arqParaComp, lBytes);

    noArvore *arvore = fazerArvore(lBytes);

    fwrite(lBytes, 256, sizeof(lBytes[0]), arqComp);
    fseek(arqComp, sizeof(unsigned int), SEEK_CUR);

    while (fread(&c, 1, 1, arqParaComp) >= 1)
    {
        char buffer[1024] = {0};

        buscaCodigoByte(arvore, c, buffer, 0);

        for (char *i = buffer; *i; i++)
        {
            if (*i == '1')
            {
                aux = aux | (1 << (tamanho % 8));
            }
            tamanho++;

            if (tamanho % 8 == 0)
            {
                fwrite(&aux, 1, 1, arqComp);
                aux = 0;
            }
        }
    }

    fwrite(&aux, 1, 1, arqComp);
    fseek(arqComp, 256 * sizeof(unsigned int), SEEK_SET);
    fwrite(&tamanho, 1, sizeof(unsigned), arqComp);

    fclose(arqParaComp);
    fclose(arqComp);
}

int escolherOpcao()
{
    printf("O que deseja fazer? \n");

    printf("Digite [1] para compactar um arquivo \n");
    printf("Digite [2] para descompactar um arquivo \n");
    printf("Digite [3] para sair do programa \n");

    char opcao;
    scanf("%c", &opcao);

    switch (opcao)
    {
        case '1':
            compactar();
            break;

        case '2':
            descompactar();
            break;

        case '3':
            break;
        default:
            printf("Valor de operacao invalido\n");
            break;
    }
    return 0;
}

int main()
{
    escolherOpcao();
    return 0;
}
