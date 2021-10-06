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

typedef struct noFila
{
    noArvore *no;
    struct noFila *prox;
} noFila;

typedef struct fila
{
    noFila *inicio;
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

int gerarBit(FILE *arq, int pos, unsigned char *aux)
{
    if(pos % 8 == 0)
    {
        fread(aux, 1, 1, arq);
    }

    return !!((*aux) & (1 << (pos % 8)));
}

noFila *insereNoFila(noFila *n, fila *fil)
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

noArvore *criaSubArvore(fila *fla)
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
    fila fil = {0, NULL};

    for(int i = 0; i < 256; i++)//percorre o vetor de bytes para criar a arvore com o nodos
    {
      if(f[i]) //se o byte existe:
        {
            insereNoFila(novoNoFila(novoNoArv(i, f[i], NULL, NULL)), &f);
        }
    }

    while(fil.qtd > 1)
    {
        noArvore *direito = criaSubArvore(&fil);
        noArvore *esquerdo = criaSubArvore(&fil);
        noArvore *soma = novoNoArv('#', esquerdo->freq + direito->freq, esquerdo, direito);
        insereNoFila(novoNoFila(soma), &fil);
    }
    return criaSubArvore(&f);
}

void excluirArvore(noArvore *no)
{
    while(!no)
    {
        noArvore *esquerda = no->esq;
        noArvore *direita = no->dir;
        free(no);
    }
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
    unsigned tamanho;
    unsigned posicao = 0;
    unsigned char aux = 0;

    printf("Digite o nome do arquivo a ser descompactado: \n");
    scanf("%s", nomeArqComp);

    if (arqComp == NULL)
        printf("Arquivo não encontrado!");

    printf("Digite o nome do arquivo que guardará a compactação: \n");
    scanf("%s", nomeArqDesc);

    if (arqComp == NULL)
        printf("Por favor digite corretamente o nome do arquivo!");

    fread(lBytes, 256, sizeof(lBytes[0]), arqComp);
    noArvore *arvore = fazerArvore(*lBytes);

    fread(&tamanho, 1, sizeof(tamanho), arqComp);

    while (posicao < tamanho)
    {
        noArvore *noAtual = arvore;

        while (noAtual->esq || noAtual->dir)
            noAtual = gerarBit(arqComp, posicao++, &aux) ? noAtual->dir : noAtual->esq;

        fwrite(&(noAtual->c), 1, 1, arqDesc);
    }

    fseek(arqComp, 0L, SEEK_END);
    double tamanhoComp = ftell(arqComp);

    fseek(arqDesc, 0L, SEEK_END);
    double tamanhoDesc = ftell(arqDesc);

    printf("Arquivo de entrada: %s (%g bytes)\nArquivo de saida: %s (%g bytes)", nomeArqComp, tamanhoComp / 1000, nomeArqDesc, tamanhoDesc/ 1000);

    excluirArvore(arvore);

    fclose(arqComp);
    fclose(arqDesc);
}

void compactar()
{
    FILE *arqComp;
    FILE *arqDesc;
    char nomeArqComp[20];
    char nomeArqDesc[20];
    unsigned lBytes[256] = {0};
    unsigned char c;
    unsigned tamanho = 0;
    unsigned char aux = 0;

    printf("Digite o nome do arquivo a ser compactado: \n");
    scanf("%s", nomeArqComp);
    arqComp = fopen(nomeArqComp, "rb");

    if (arqComp == NULL)
        printf("Arquivo não encontrado!");

    printf("Digite o nome do arquivo que guardará a compactação: \n");
    scanf("%s", nomeArqDesc);
    arqDesc = fopen(nomeArqDesc, "wb");

    if (arqDesc == NULL)
        printf("Por favor digite corretamente o nome do arquivo!");

    obterFreqByte(arqComp, lBytes);

    noArvore *arvore = fazerArvore(lBytes);

    fwrite(lBytes, 256, sizeof(lBytes[0]), arqDesc);
    fseek(arqDesc, sizeof(unsigned int), SEEK_CUR);

    while (fread(&c, 1, 1, arqComp) >= 1)
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

    fwrite(&aux, 1, 1, arqDesc);
    fseek(arqDesc, 256 * sizeof(unsigned int), SEEK_SET);
    fwrite(&tamanho, 1, sizeof(unsigned), arqDesc);

    fseek(arqDesc, 0L, SEEK_END);
    double tamanhoDesc = ftell(arqDesc);

    fseek(arqComp, 0L, SEEK_END);
    double tamanhoComp = ftell(arqComp);

    printf("\nArquivo de entrada (descompactado): %s (%g bytes)\nArquivo de saida (compactado): %s (%g bytes)", nomeArqDesc, tamanhoDesc / 1000, nomeArqComp, tamanhoComp / 1000);

    excluirArvore(arvore);

    fclose(arqComp);
    fclose(arqDesc);

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
