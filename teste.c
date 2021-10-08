#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

FILE *arq = "C:\\huffman\\wbfala.mp4";
int pos = 2;
unsigned char c = 'b';

void gerarBit1(FILE *arq, int pos, unsigned char *aux)
{
    if(pos % 8 == 0)
    {
        fread(aux, 1, 1, arq);
    }

    printf("%d VERSAO ORIGINAL \n", !!((*aux) & (1 << (pos % 8))));
    return !!((*aux) & (1 << (pos % 8)));
}

void gerarBit2(FILE *arq, int pos, unsigned char *aux)
{
    if(pos % 8 == 0)
    {
      fread(aux, 1, 1, arq);
    }
    if(!(*aux) & (1 << (pos % 8))) 
    {
      *aux = 1;
    } 
    else
    {
        1 << (pos % 8); //desloca pra esquerda um bit
    }
    printf("%d", *aux);
}

int main()
{
  gerarBit1(arq, pos, &c);
  gerarBit2(arq, pos, &c);
}