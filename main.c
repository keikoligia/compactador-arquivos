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

typedef struct noFila{
noArvore *no;
struct noFila *prox;

}noFila;

typedef struct fila{
noFila *inicio;
byte tamanho;
int qtd;
} fila;

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

noFila *novoNoFila(noArvore *noArv)
{
    noFila *novo;
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

noLista *insereNoFila(noFila *n, fila *fil)
{
    if (!fil->inicio)
        fil->inicio = n; //primeira pos da fila recebe o argum
    else
        if(n->no->freq < fil->inicio->no->freq)
        {
            n->prox = fil->inicio;
            fil->inicio = n;
        }
    else
    {
        noFila *aux = fil->inicio->prox;
        noFila *aux2 = fil->inicio;

        while (aux && aux->no->freq <= n->no->freq)
        {
            aux2 = aux;
            aux = aux2->prox;
        }

        aux2->prox = n;
        n->prox = aux;
    }

    fil->qtd++;
}

void obterFreqByte(FILE *arq, unsigned int *fila)
{
    unsigned char ch;

    while (fread(&ch, sizeof(unsigned char), 1, arq))
        fila[(unsigned char)ch]++;

    rewind(arq);
}

noArvore criaSubArvore(fila fla)
{
    noFila *noFila = fla->inicio;
    noArvore *noArv = noFila->no;

    fla->inicio = noFila->prox;

    free(noFila);
    noFila = NULL;

    fla->qtd--;

    return noArv;
}

noArvore *fazerArvore(unsigned *f)
{
    fila fil = {0; NULL};

    for(int i = 0; i < 256; i++)//percorre o vetor de bytes para criar a arvore com o nodos
    {
      if(fil[i]) //se o byte existe:
        {
            insereNoFila(novoNoFila(novoNoArv(i, f[i], NULL, NULL)));
        }
    }

    while(fil.qtdtamanho > 1)
    {
        criaSubArvore(&fil);

        noArvore *direito = noArv;
        noArvore *esquerdo = noArv;
        noArvore *soma = novoNoArv('#', esquerdo->freq + direito->freq, esquerdo, direito);
        insereNoFila(novoNoFila(soma), &fla);
    }
    return criaSubArvore(&l);
}

void excluirArvore(noArvore *no)
{
    if(!no)


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
    case 1:
        compactar();
        break;
    
    case 2: 
        descompactar();
        break;

    case 3:
        break;

    default:
        printf("Digite um valor válido por favor! \n\n");
    }

    return 0;
}
void descompactar()
{
    FILE *arqComp;
    FILE *arqDesc;
    char nomeArqComp[20];
    char nomeArqDesc[20];
    unsigned lBytes[256] = {0};
    unsigned tamanho;
    unsigned posicao = 0;
    unsigned char aux = 0;

    printf("Digite o nome do arquivo a ser descompactado: \n");
    scanf("%s", nomeArqComp);

    if (arqComp == NULL)
        printf("Arquivo n�o encontrado!");

    print("Digite o nome do arquivo que guardar� a compacta��o: \n");
    scanf("%s", nomeArqDesc);

    if (arqComp == NULL)
        printf("Por favor digite corretamente o nome do arquivo!");

    fread(lBytes, 256, sizeof(lBytes[0]), arqComp)
    noArvore *root = fazerArvore(*lBytes);

    fread(&tamanho, 1, sizeof(tamanho), arqLer);

    while (posicao < tamanho)
    {
        noArvore *noAtual = root;

        while (noAtual->esq || noAtual->dir)
            noAtual = geraBit(arqComp, posicao++, &aux) ? noAtual->dir : noAtual->esq;

        fwrite(&(noAtual->c), 1, 1, arqDesc);
    }

    fseek(arqLer, 0L, SEEK_END);
    double tamanhoComp = ftell(arqComp);

    fseek(arqDesc, 0L, SEEK_END);
    double tamanhoDesc = ftell(arqDesc);

    printf("Arquivo de entrada: %s (%g bytes)\nArquivo de saida: %s (%g bytes)", nomeArqComp, tamanhoComp / 1000, nomeArqDesc, tamanhoDesc/ 1000);

    excluiArvore(raiz);

    fclose(arqComp);
    fclose(arqDesc);
}

void compactar()
{
    FILE *arqDesc;
    FILE *arqComp;
    char nomeArqDesc[20];
    char nomeArqComp[20];
    unsigned lBytes[256] ={0};

    printf("Digite o nome do arquivo a ser compactado: \n");
    scanf("%s", nomeArqDesc);

    if (arqDesc == NULL)
        printf("Arquivo n�o encontrado!");

    print("Digite o nome do arquivo que guardar� a compacta��o: \n");
    scanf("%s", nomeArqComp);

    if (arqComp == NULL)
        printf("Por favor digite corretamente o nome do arquivo!");

    fwrite(lBytes, 256, sizeof(lBytes[0]), arqComp)
    fazerArvore(*lBytes);
}
