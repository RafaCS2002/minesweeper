#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct mineZone {
  char *gen;
  int *choice;
};

void chooseLevel(int *qntBomb, int *iSize, int *jSize);
void showZone(char **gen, int **choice, int iSize, int jSize);
void genZone(char **gen, int **choice, int qntBomb, int iSize, int jSize);
void testPrintMatrix(char **gen, int iSize, int jSize);

int main() {
  int qntBomb, iSize, jSize, Size, iChoice, jChoice, opc, posi;
  int first = 1, loop = 1;

  srand(time(NULL));

  chooseLevel(&qntBomb, &iSize, &jSize);

  // tamanho total
  Size = iSize * jSize;

  // Aloca os espacos
  struct mineZone zone = {(char *)malloc(Size), (int *)calloc(Size, sizeof(int))};

  // atribui . a tds as posicao em gen
  for (int i = 0; i < Size; i++) {
    *(zone.gen + i) = '.';
  }

  while (loop) {
    // printa o campo
    showZone(&zone.gen, &zone.choice, iSize, jSize);

    printf(
        "\n0 - Selecionar uma casa\n1 - Marcar/Desmarcar Bomba"
        "\nSelecionar: ");
    scanf("%d", &opc);

    switch (opc) {
      case 0:  // Seleciona casa
        printf("Escolha a linha: ");
        scanf("%d", &iChoice);
        printf("Escolha a Coluna: ");
        scanf("%d", &jChoice);

        // Estruturar para encontrar a posicao da matriz com
        // ponteiro
        // *(ponteiro + (coluna + (linha * tamanho_da_linha)))
        posi = jChoice + (iChoice * iSize);
        *(zone.choice + posi) = 1;
        if (first) {
          genZone(&zone.gen, &zone.choice, qntBomb, iSize, jSize);
          first = 0;
        }

        break;
      case 1:  // Marcar/Desmarcar Casa
        printf("Escolha a linha: ");
        scanf("%d", &iChoice);
        printf("Escolha a Coluna: ");
        scanf("%d", &jChoice);

        posi = jChoice + (iChoice * iSize);
        if (*(zone.choice + posi) == 1) {
          printf("Não pode ser marcada");
        } else if (*(zone.choice + posi) == 2) {
          *(zone.choice + posi) = 0;
        } else {
          *(zone.choice + posi) = 2;
        }
        break;
      default:
        printf("Comando não reconhecido\n");
        break;
    }

    // *(*endereco_ponteiro + (coluna + (linha * tamanho_da_linha)))
    // printa gen
    testPrintMatrix(&zone.gen, iSize, jSize);
  }

  return 0;
}

void chooseLevel(int *qntBomb, int *iSize, int *jSize) {
  int loop = 1, opc, i, j, qBChoose;

  while (loop) {
    printf(
        "0 - Principiante\n1 - Intermediário\n2 - "
        "Especialista\n3 - "
        "Customizado\nEscolha o tipo de campo: ");
    scanf("%d", &opc);

    switch (opc) {
      case 0:
        *qntBomb = 10;
        *iSize = *jSize = 9;
        loop = 0;
        break;

      case 1:
        *qntBomb = 40;
        *iSize = *jSize = 16;
        loop = 0;
        break;

      case 2:
        *qntBomb = 99;
        *iSize = 16;
        *jSize = 30;
        loop = 0;
        break;

      case 3:
        while (loop) {
          printf("\nLinhas e Colunas limitadas a 99");
          printf("\nInsira a quantidade de linhas:");
          scanf("%d", &i);
          printf("\nInsira a quantidade de colunas:");
          scanf("%d", &j);

          if ((i >= 7 && i <= 99) && (j >= 7 && j <= 99)) {
            printf("\nBombas limitadas a %d", (i * j - 9));
            printf("\nInsira a quantidade de Bombas:");
            scanf("%d", &qBChoose);
            if (qBChoose <= (i * j - 9)) {
              *iSize = i;
              *jSize = j;
              *qntBomb = qBChoose;
              loop = 0;
            } else {
              printf(
                  "\nColoque uma quantidade "
                  "dentro do limite de %d",
                  (i * j - 9));
            }
          } else {
            printf(
                "\nColoque uma quantidade "
                "dentro do limite de 7 a 99");
          }
        }

        break;

      default:
        printf("\nComando não reconhecido");
        break;
    }
  }
}

void showZone(char **gen, int **choice, int iSize, int jSize) {
  char line[jSize + 1];
  line[jSize] = '\n';

  // Printar indice de colunas
  printf("  ");
  for (int j = 0; j < jSize; j++) {
    if (j < 10) {
      printf("   %d", j);
    } else {
      printf("  %d", j);
    }
  }
  printf("\n");

  for (int i = 0; i < iSize; i++) {
    // Atribuir respectivo Caracter
    for (int j = 0; j < jSize; j++) {
      // *(*endereco_ponteiro + (coluna + (linha * tamanho_da_linha)))
      if (*(*choice + (j + (i * iSize))) == 2) {
        line[j] = '#';
      } else if (*(*choice + (j + (i * iSize))) == 1) {
        line[j] = *(*gen + j + (i * iSize));
      } else {
        line[j] = '-';
      }
    }

    // Printar Linhas
    if (i < 10) {
      printf("%d ", i);
    } else {
      printf("%d", i);
    }
    for (int j = 0; j < jSize + 1; j++) {
      printf(" | %c", line[j]);
    }
  }
}

void genZone(char **gen, int **choice, int qntBomb, int iSize, int jSize) {
  int posi, posiMax;
  int posiInit = 0, posiBomb = 0;
  int downLine = 0, upLine = 0;

  // Encontra a casa onde houve a selecao
  for (int i = 0; i < iSize; i++) {
    for (int j = 0; j < jSize; j++) {
      posi = j + (i * iSize);
      if (*(*choice + posi) == 1) {
        posiInit = posi;
      }
    }
  }

  posiMax = (jSize - 1) + ((iSize - 1) * iSize);
  for (int b = 0; b < qntBomb; b++) {
    posiBomb = (rand() % (posiMax + 1));

    // Checa se está na area selecionada inicialmente
    if (((posiBomb == posiInit - 1) || (posiBomb == posiInit + 1)) || posiBomb == posiInit) {
      b--;
    } else if (((posiBomb == (posiInit - iSize) - 1) || (posiBomb == (posiInit - iSize) + 1)) || posiBomb == (posiInit - iSize)) {
      b--;
    } else if (((posiBomb == (posiInit + iSize) - 1) || (posiBomb == (posiInit + iSize) + 1)) || posiBomb == (posiInit + iSize)) {
      b--;
    } else {
      *(*gen + posiBomb) = 'X';
    }
  }

  // Colocar os numeros no perimetro
  for (int i = 0; i < iSize; i++) {
    for (int j = 0; j < jSize; j++) {
      if (*(*gen + j + (i * iSize)) == 'X') {
        posiBomb = j + (i * iSize);
        for (int k = -1; k <= 1; k++) {
          upLine = iSize + k;
          downLine = -iSize + k;
          if (*(*gen + posiBomb + upLine) = '1') {  // Linha abaixo
            *(*gen + posiBomb + upLine) += 1;
          } else if (*(*gen + posiBomb + upLine) == '.') {  // Caso não tenha ponto
            *(*gen + posiBomb + upLine) = '1';
          }
          if (*(*gen + posiBomb + downLine) == '1') {  // Linha acima
            *(*gen + posiBomb + downLine) += 1;
          } else if (*(*gen + posiBomb + downLine) == '.') {  // Caso não tenha ponto
            *(*gen + posiBomb + downLine) = '1';
          }
          if (*(*gen + posiBomb + k) == '1') {  // Linha da bomba
            *(*gen + posiBomb + k) += 1;
          } else if (*(*gen + posiBomb + k) == '.') {  // Caso não tenha ponto
            *(*gen + posiBomb + k) = '1';
          }
        }
      }
    }
  }
}

void testPrintMatrix(char **gen, int iSize, int jSize) {
  char line[jSize + 1];
  line[jSize] = '\n';
  printf("\n\n");
  for (int i = 0; i < iSize; i++) {
    for (int j = 0; j < jSize; j++) {
      line[j] = *(*gen + j + (i * iSize));
    }
    for (int j = 0; j <= jSize; j++) printf(" | %c", line[j]);
  }

  printf("\n\n");
}