#include <stdio.h>
#include <stdlib.h>

/*
  habilidades_tabuleiro.c
  Programa em C que cria um tabuleiro 10x10 e três matrizes de habilidade (cone, cruz e octaedro)
  As matrizes são construídas dinamicamente usando loops e condicionais.
  As habilidades são sobrepostas ao tabuleiro a partir de pontos de origem definidos no código.

  Símbolos no tabuleiro impresso:
    0 -> Água
    3 -> Navio
    5 -> Área afetada pela habilidade

  Como compilar:
    gcc -std=c99 -Wall -o habilidades_tabuleiro habilidades_tabuleiro.c
  Como executar:
    ./habilidades_tabuleiro
*/

#define ROWS 10
#define COLS 10
#define SK_SIZE 5 /* tamanho fixo das matrizes de habilidade (5x5) */

/* protótipos */
void inicializar_tabuleiro(int tab[ROWS][COLS]);
void imprimir_tabuleiro(int tab[ROWS][COLS]);
void construir_cone(int mat[SK_SIZE][SK_SIZE]);
void construir_cruz(int mat[SK_SIZE][SK_SIZE]);
void construir_octaedro(int mat[SK_SIZE][SK_SIZE]);
void aplicar_habilidade(int tab[ROWS][COLS], int mat[SK_SIZE][SK_SIZE], int orig_r, int orig_c);

int main(void) {
    int tab[ROWS][COLS];
    int cone[SK_SIZE][SK_SIZE];
    int cruz[SK_SIZE][SK_SIZE];
    int octaedro[SK_SIZE][SK_SIZE];

    /* inicializa tabuleiro com água (0) e alguns navios (3) para demonstração */
    inicializar_tabuleiro(tab);

    /* exemplo de navios pré-posicionados (valores 3) - fixo no código conforme simplificação */
    tab[2][2] = 3; tab[2][3] = 3; tab[2][4] = 3; /* pequeno navio horizontal */
    tab[6][7] = 3; tab[7][7] = 3; /* navio vertical */

    /* construir matrizes de habilidade dinamicamente */
    construir_cone(cone);
    construir_cruz(cruz);
    construir_octaedro(octaedro);

    /* pontos de origem das habilidades (linha, coluna) - definidos diretamente no código */
    int orig_cone_r = 1, orig_cone_c = 3;    /* coloque o topo do cone no tabuleiro */
    int orig_cruz_r = 4, orig_cruz_c = 5;
    int orig_oct_r  = 7, orig_oct_c  = 2;

    /* aplicar habilidades sobre o tabuleiro; áreas afetadas serão marcadas com 5
       Observação: se já houver um navio (valor 3) naquela célula, não sobrescrevemos. */
    aplicar_habilidade(tab, cone, orig_cone_r, orig_cone_c);
    aplicar_habilidade(tab, cruz, orig_cruz_r, orig_cruz_c);
    aplicar_habilidade(tab, octaedro, orig_oct_r, orig_oct_c);

    /* exibir o tabuleiro final */
    imprimir_tabuleiro(tab);

    return 0;
}

/* inicializa todo o tabuleiro com 0 (água) */
void inicializar_tabuleiro(int tab[ROWS][COLS]){
    for(int i=0;i<ROWS;i++){
        for(int j=0;j<COLS;j++){
            tab[i][j] = 0;
        }
    }
}

/* imprime o tabuleiro usando números separados por espaço */
void imprimir_tabuleiro(int tab[ROWS][COLS]){
    printf("Tabuleiro (0=água, 3=navio, 5=área afetada):\n\n");
    for(int i=0;i<ROWS;i++){
        for(int j=0;j<COLS;j++){
            printf("%d ", tab[i][j]);
        }
        printf("\n");
    }
}

/* constrói uma matriz 5x5 que representa um cone apontando para baixo.
   Estratégia: centro da matriz é SK_SIZE/2 (2). O "topo" do cone será a linha 0,
   o cone se alarga para baixo usando condicionais (largura aumenta com a linha).
   Exemplo resultante (1 = afetado):
   0 0 1 0 0
   0 1 1 1 0
   1 1 1 1 1
   0 0 0 0 0
   0 0 0 0 0
*/
void construir_cone(int mat[SK_SIZE][SK_SIZE]){
    int center = SK_SIZE/2; /* 2 */
    for(int i=0;i<SK_SIZE;i++){
        for(int j=0;j<SK_SIZE;j++){
            /* inicializa com 0 */
            mat[i][j] = 0;
            /* queremos um cone de altura 3 (linhas 0..2). A largura aceitável na linha i
               será: valor absoluto da diferença <= i  */
            int diff = (j - center) < 0 ? -(j - center) : (j - center);
            if(i <= 2){
                if(diff <= i){
                    mat[i][j] = 1;
                }
            }
        }
    }
}

/* constrói uma matriz 5x5 em forma de cruz com origem no centro.
   Estratégia: marca-se a linha central e a coluna central com 1. */
void construir_cruz(int mat[SK_SIZE][SK_SIZE]){
    int center = SK_SIZE/2;
    for(int i=0;i<SK_SIZE;i++){
        for(int j=0;j<SK_SIZE;j++){
            if(i == center || j == center) mat[i][j] = 1;
            else mat[i][j] = 0;
        }
    }
}

/* constrói uma matriz 5x5 que representa a vista frontal de um octaedro (losango/diamante).
   Estratégia: usa distância Manhattan (|dr| + |dc|) <= radius para formar o losango.
   Para SK_SIZE=5, radius = 2 (center at 2,2). */
void construir_octaedro(int mat[SK_SIZE][SK_SIZE]){
    int center = SK_SIZE/2;
    int radius = center; /* 2 */
    for(int i=0;i<SK_SIZE;i++){
        for(int j=0;j<SK_SIZE;j++){
            int diff_i = (i - center) < 0 ? -(i - center) : (i - center);
            int diff_j = (j - center) < 0 ? -(j - center) : (j - center);
            int manhattan = diff_i + diff_j;
            if(manhattan <= radius) mat[i][j] = 1;
            else mat[i][j] = 0;
        }
    }
}

/* aplica a matriz de habilidade 'mat' sobre o tabuleiro 'tab' centrando-a em (orig_r, orig_c)
   A mapeação é feita calculando o deslocamento entre o centro da matriz e cada célula, e
   validando limites para não ultrapassar o tabuleiro. */
void aplicar_habilidade(int tab[ROWS][COLS], int mat[SK_SIZE][SK_SIZE], int orig_r, int orig_c){
    int center = SK_SIZE/2; /* índice central da matriz de habilidade */

    for(int i=0;i<SK_SIZE;i++){
        for(int j=0;j<SK_SIZE;j++){
            if(mat[i][j] != 1) continue; /* só nos pontos afetados */

            /* calcula posição no tabuleiro correspondente à célula (i,j) da matriz */
            int board_r = orig_r - center + i; /* centraliza a matriz no ponto de origem */
            int board_c = orig_c - center + j;

            /* verifica limites do tabuleiro */
            if(board_r < 0 || board_r >= ROWS || board_c < 0 || board_c >= COLS) continue;

            /* marca a área afetada com 5 somente se for água (0). Se já houver navio (3),
               mantemos o 3 para visualizar navios intactos sobre a área. */
            if(tab[board_r][board_c] == 0){
                tab[board_r][board_c] = 5;
            }
        }
    }
}