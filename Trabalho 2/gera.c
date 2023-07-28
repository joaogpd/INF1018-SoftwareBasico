/* João Gabriel Peixoto Drumond Lopes 2210168 3WB */
/* Jayme Augusto Avelino de Paiva 2210289 3WB */
// Nota 10 de 10

#include "gera.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// quantidade de bytes de código de máquina 
// gerada por cada possível comando em Simples
#define TAMPROLOGO 8
#define TAMRETORNOCONS 7
#define TAMRETORNOVAR 5
#define TAMATRIBCONS 7
#define TAMATRIBVAR 8
#define TAMATRIBPAR 3
#define TAMEXPRASCONS2 14
#define TAMEXPRMULCONS2 17
#define TAMEXPRASVAR2 12
#define TAMEXPRMULVAR2 13
#define TAMEXPRASCONSVAR 14
#define TAMEXPRMULCONSVAR 15
#define TAMEXPRASVARCONS 15
#define TAMEXPRMULVARCONS 15
#define TAMJMP 10

static void error(const char *msg, int line) {
  fprintf(stderr, "erro %s na linha %d\n", msg, line);
  exit(EXIT_FAILURE);
}

/*
Objetivo: produzir um conjunto de instruções em código de máquina 
que traduzem o prólogo de uma função. Salva o RA da função chamadora,
cria o RA da função chamada e aloca 32 bytes de espaço, de forma
a ser capaz de armazenar as 5 variáveis locais (de 4 bytes)
Passos:
- Escreve o código da instrução 'pushq %rbp' no array
- Escreve o código da instrução 'movq %rsp, %rbp' no array
- Escreve o código da instrução 'subq $32, %rsp' no array
Parâmetros:
- codigo: array, contém o código de máquina
Obs: tamanho desconsiderado, pois sempre será no início do array
*/
void prologue_instructions(unsigned char codigo[]) {
  // 55                     pushq   %rbp
  *(codigo) = 0x55;
  // 48 89 e5               movq    %rsp, %rbp
  *(codigo + 1) = 0x48;
  *(codigo + 2) = 0x89;
  *(codigo + 3) = 0xe5;
  // 48 83 ec 20            subq $32, %rsp
  *(codigo + 4) = 0x48;
  *(codigo + 5) = 0x83;
  *(codigo + 6) = 0xec;
  *(codigo + 7) = 0x20;
  // 8 bytes escritos, TAMPROLOGO = 8
}


/*
Objetivo: produzir um conjunto de instruções em código de máquina 
que traduzem o retorno de uma constante
Passos:
- Escreve o código do início da instrução 'movl $0xXXXXXXXX, %eax' no array
- Escreve o valor da constante no array
- Escreve o código da instrução 'leave' no array
- Escreve o código da instrução 'ret' no array
Parâmetros:
- val: valor da constante
- codigo: array, contém o código de máquina
- size: tamanho atual em uso do array
*/
void ret_instructions_cons(int val, unsigned char codigo[], int size) {
  // b8 XX XX XX XX       	movl    $0xXXXXXXXX ,%eax
  *(codigo + size) = 0xb8;
  *((int *)(codigo + size + 1)) = val;
  // c9                     leave
  *(codigo + size + 5) = 0xc9;
  // c3                     ret
  *(codigo + size + 6) = 0xc3;
  // 7 bytes escritos, TAMRETORNOCONS = 7
}

/* 
Nas funções subsequentes, é utilizado o seguinte cálculo para determinar
o valor em cada instrução correspondente ao acesso do valor alocado no 
registro de ativação pelo %rbp.
mem_val = 0xfc - 4 * (num_var - 1)
Sendo 0xfc o valor para num_var = 1
e num_var o índice da variável local
*/

/*

Objetivo: produzir um conjunto de instruções em código de máquina 
que traduzem o retorno de um valor em uma variável local
Passos:
- Escreve o código do ínicio da instrução 'movl -Y(%rbp), %eax' no array
- Escreve o valor de memória referente à variável no array
- Escreve o código da intrução 'leave' no array
- Escreve o código da instrução 'ret' no array 
Parâmetros:
- num_var: número da variável que deve ser retornada
- codigo: array, contém o código de máquina
- size: tamanho atual em uso do array
*/
void ret_instructions_var(int num_var, unsigned char codigo[], int size) {
  // valor referente a variável que deve ter valor retornado
  unsigned char mem_val = 0xfc - 4 * (num_var - 1);
  // 8b 45 YY               movl -Y(%rbp), %eax
  *(codigo + size) = 0x8b;
  *(codigo + size + 1) = 0x45;
  *(codigo + size + 2) = mem_val;
  // c9                     leave
  *(codigo + size + 3) = 0xc9;
  // c3                     ret
  *(codigo + size + 4) = 0xc3;
  // 5 bytes escritos, TAMRETORNOVAR = 5
}

/*
Objetivo: produzir um conjunto de instruções em código de máquina que 
traduzem a atribuição do valor de uma constante em uma variável local
Passos:
- Escreve o código do início da instrução 'movl $0xXXXXXXXX, -Y(%rbp)' no array
- Escreve o valor de memória referente a variável no array
- Escreve o valor da constante no array
Parâmetros:
- num_var: número da variável que recebe o valor
- val: valor da constante
- codigo: array, contém o código de máquina
- size: tamanho atual em uso do array 
*/
void attr_instructions_cons(int num_var, int val, unsigned char codigo[], int size) {
  // valor referente à variável que recebe o valor
  unsigned char mem_val = 0xfc - 4 * (num_var - 1);
  // c7 45 YY XX XX XX XX
  *(codigo + size) = 0xc7;
  *(codigo + size + 1) = 0x45;
  *(codigo + size + 2) = mem_val;
  *((int*)(codigo + size + 3)) = val; 
  // 7 bytes escritos, TAMATRIBCONS = 7
}

/*
Nas funções subsequentes, quando é necessário utilizar um registrador
temporário, é utilizado o %r10d. O código referente a ele dentro de uma
determinada instrução varia de instrução para instrução.
Em alguns casos, é 0x55, em outros, 
*/

/*
Objetivo: produzir um conjunto de instruções em código de máquina que 
traduzem a atribuição do valor de uma var local em uma variável local
Passos:
- Escreve o início da instrução 'movl -Y(%rbp), %r10d' no array
- Escreve o valor de memória referente à outra variável
- Escreve o início da instrução 'movl %r10d, -Y(%rbp)' no array
- Escreve o valor de memória referente à variável que vai receber o valor
Parâmetros:
- num_var1: número da variável que recebe o valor
- num_var2: número da outra variável
- codigo: array, contém o código de máquina
- size: tamanho atual em uso do array 
Obs: Usa %r10d como registrador temporário, pois não é permitida operação
de valor em espaço de memória para valor em espaço de memória
*/
void attr_instructions_var(int num_var1, int num_var2, unsigned char codigo[], int size) {
  // valor referente à variável que recebe o valor
  unsigned char mem_val1 = 0xfc - 4 * (num_var1 - 1);
  // valor referente a outra variável (que não recebe o valor)
  unsigned char mem_val2 = 0xfc - 4 * (num_var2 - 1);
  // 44 8b 55 YY            movl -Y(%rbp), %r10d
  *(codigo + size) = 0x44;
  *(codigo + size + 1) = 0x8b;
  *(codigo + size + 2) = 0x55;
  *(codigo + size + 3) = mem_val2;
  // 44 89 55 YY            movl %r10d, -Y(%rbp)
  *(codigo + size + 4) = 0x44;
  *(codigo + size + 5) = 0x89;
  *(codigo + size + 6) = 0x55;
  *(codigo + size + 7) = mem_val1;
  // 8 bytes escritos, TAMATRIBVAR = 8
}

/*
Objetivo: produzir um conjunto de instruções em código de máquina que 
traduzem a atribuição do valor de um parâmetro em uma variável local
Passos:
- Escreve o início da instrução 'movl %par, -Y(%rbp)' no array
- Escreve o valor referente ao parâmetro correto no array 
- Escreve o valor de memória referente a variável no array 
Parâmetros:
- num_var: número da variável que recebe o valor
- num_par: número do parâmetro
- codigo: array, contém o código de máquina
- size: tamanho atual em uso do array 
*/
void attr_instructions_par(int num_var, int num_par, unsigned char codigo[], int size) {
  // Valor referente à variável que recebe o valor
  unsigned char mem_val = 0xfc - 4 * (num_var - 1);
  // 0x89 PAR YY            movl %par, -Y(%rbp)
  *(codigo + size) = 0x89;
  if (num_par == 1) {
    // %edi = 0x7d
    *(codigo + size + 1) = 0x7d;
  } else if (num_par == 2) {
    // %esi = 0x75
    *(codigo + size + 1) = 0x75;
  } else if (num_par == 3) {
    // %edx = 0x55
    *(codigo + size + 1) = 0x55;
  }
  *(codigo + size + 2) = mem_val;
  // 3 bytes escritos, TAMATRIBPAR = 3
}

/*
Objetivo: produzir um conjunto de instruções em código de máquina que 
traduzem a operação de soma e diferença de constantes em variável local
Passos:
- Escreve o início da instrução 'movl $0xXXXXXXXX, -Y(%rbp)' no array
- Escreve o valor de memória referente à variável no array 
- Escreve o valor do primeiro elemento da operação no array
- Escreve o início da instrução 'addl $0xXXXXXXXX, -Y(%rbp)' ou 'subl $0xXXXXXXXX, -Y(%rbp)' no array
- Escreve o valor de memória referente à variável no array 
- Escreve o valor do segundo elemento da operação no array
Parâmetros:
- num_var: número da variável que recebe o valor
- val1: primeiro elemento da operação
- val2: segundo elemento da operação
- op: operador da operação
- codigo: array, contém o código de máquina
- size: tamanho atual em uso do array 
*/
void expr_instructions_add_sub_cons_cons(int num_var, int val1, int val2, char op, unsigned char codigo[], int size) {
  // valor referente à variável que recebe o valor
  unsigned char mem_val = 0xfc - 4 * (num_var - 1);
  // c7 45 YY XX XX XX XX   movl $0xXXXXXXXX, -Y(%rbp)
  *(codigo + size) = 0xc7;
  *(codigo + size + 1) = 0x45;
  *(codigo + size + 2) = mem_val;
  *((int*)(codigo + size + 3)) = val1;
  // 81 [45|6d] YY XX XX XX XX   [add|sub]l $0xXXXXXXXX, -Y(%rbp) 
  *(codigo + size + 7) = 0x81;
  if (op == '+') {
    *(codigo + size + 8) = 0x45;
  } else if (op == '-') {
    *(codigo + size + 8) = 0x6d;
  }
  *(codigo + size + 9) = mem_val;
  *((int*)(codigo + size + 10)) = val2;
  // 14 bytes escritos, TAMEXPRASCONS2 = 14
}

/*
Objetivo: produzir um conjunto de instruções em código de máquina que 
traduzem a operação de produto de constantes em variável local
Passos:
- Escreve o início da instrução 'movl $0xXXXXXXXX, %r10d' no array
- Escreve o valor do primeiro elemento da operação no array
- Escreve o início da instrução 'imull $0xXXXXXXXX, %r10d' no array
- Escreve o valor do segundo elemento da operação no array
- Escreve o início da instrução 'movl %r10d, -Y(%rbp)' no array
- Escreve o valor de memória referente à variável no array
Parâmetros:
- num_var: número da variável que recebe o valor
- val1: primeiro elemento da operação
- val2: segundo elemento da operação
- codigo: array, contém o código de máquina
- size: tamanho atual em uso do array 
Obs: Usa %r10d como registrador temporário, pois o resultado
de imull deve ser posto em registrador
*/
void expr_instructions_mul_cons_cons(int num_var, int val1, int val2, unsigned char codigo[], int size) {
  // valor referente à variável que recebe o valor
  unsigned char mem_val = 0xfc - 4 * (num_var - 1);
  // 41 ba XX XX XX XX      movl $0xXXXXXXXX, %r10d
  *(codigo + size) = 0x41;
  *(codigo + size + 1) = 0xba;
  *((int*)(codigo + size + 2)) = val1;
  // 45 69 d2 XX XX XX XX   imull $0xXXXXXXXX, %r10d
  *(codigo + size + 6) = 0x45;
  *(codigo + size + 7) = 0x69;
  *(codigo + size + 8) = 0xd2;
  *((int*)(codigo + size + 9)) = val2;
  // 44 89 55 YY            movl %r10d, -Y(%rbp)
  *(codigo + size + 13) = 0x44;
  *(codigo + size + 14) = 0x89;
  *(codigo + size + 15) = 0x55;
  *(codigo + size + 16) = mem_val;
  // 17 bytes escritos, TAMEXPRMULCONS2 = 17
}

/*
Objetivo: produzir um conjunto de instruções em código de máquina que 
traduzem a operação de soma e diferença de vars locais em variável local
Passos:
- Escreve o início da instrução 'movl -Y(%rbp), %r10d' no array
- Escreve o valor de memória referente à primeira variável da operação no array
- Escreve o início da instrução '[add|sub]l -Y(%rbp), %r10d' no array
- Escreve o valor de memória referente à segunda variável da operação no array
- Escreve o início da instrução 'movl %r10d, -Y(%rbp)' no array
- Escreve o valor de memória referente à variável que recebe o valor da operação no array
Parâmetros:
- num_var1: número da variável que recebe o valor
- num_var2: primeira variável da operação
- num_var3: segunda variável da operação
- op: operador da operação
- codigo: array, contém o código de máquina
- size: tamanho atual em uso do array 
Obs: Usa %r10d como registrador temporário, pois não é permitida operação
de valor em espaço de memória para valor em espaço de memória
*/
void expr_instructions_add_sub_var_var(int num_var1, int num_var2, int num_var3, char op, unsigned char codigo[], int size) {
  // variável que vai receber o valor
  unsigned char mem_val1 = 0xfc - 4 * (num_var1 - 1);
  // primeira variável da operação
  unsigned char mem_val2 = 0xfc - 4 * (num_var2 - 1);
  // segunda variável da operação
  unsigned char mem_val3 = 0xfc - 4 * (num_var3 - 1);
  // 44 8b 55 YY            movl -Y(%rbp), %r10d
  *(codigo + size) = 0x44;
  *(codigo + size + 1) = 0x8b;
  *(codigo + size + 2) = 0x55;
  *(codigo + size + 3) = mem_val2;  
  // 44 [03|2b] 55 YY       [add|sub]l -Y(%rbp), %r10d
  *(codigo + size + 4) = 0x44;
  if (op == '+') {
    *(codigo + size + 5) = 0x03;
  } else if (op == '-') {
    *(codigo + size + 5) = 0x2b;
  }
  *(codigo + size + 6) = 0x55;
  *(codigo + size + 7) = mem_val3;
  // 44 89 55 YY            movl %r10d, -Y(%rbp) 
  *(codigo + size + 8) = 0x44;
  *(codigo + size + 9) = 0x89;
  *(codigo + size + 10) = 0x55;
  *(codigo + size + 11) = mem_val1;
  // 12 bytes escritos, TAMEXPRASVAR2 = 12
}

/*
Objetivo: produzir um conjunto de instruções em código de máquina que 
traduzem a operação de produto de variáveis locais em variável local
Passos:
- Escreve o início da intrução 'movl -Y(%rbp), %r10d' no array
- Escreve o valor de memória referente à primeira variável da operação no array
- Escreve o início da intrução 'imull -Y(%rbp), %r10d' no array
- Escreve o valor de memória referente à segunda variável da operação no array
- Escreve o início da instrução 'movl %r10d, -Y(%rbp)' no array
- Escreve o valor de memória referente à variável que recebe o valor da operação no array
Parâmetros:
- num_var1: número da variável que recebe o valor
- num_var2: primeira variável da operação
- num_var3: segunda variável da operação
- codigo: array, contém o código de máquina
- size: tamanho atual em uso do array 
Obs: Usa %r10d como registrador temporário, pois não é permitida operação
de valor em espaço de memória para valor em espaço de memória
*/
void expr_instructions_mul_var_var(int num_var1, int num_var2, int num_var3, unsigned char codigo[], int size) {
  // variável que vai receber o valor
  unsigned char mem_val1 = 0xfc - 4 * (num_var1 - 1);
  // primeira variável da operação
  unsigned char mem_val2 = 0xfc - 4 * (num_var2 - 1);
  // segunda variável da operação
  unsigned char mem_val3 = 0xfc - 4 * (num_var3 - 1);
  // 44 8b 55 YY            movl -Y(%rbp), %r10d
  *(codigo + size) = 0x44;
  *(codigo + size + 1) = 0x8b;
  *(codigo + size + 2) = 0x55;
  *(codigo + size + 3) = mem_val2;  
  // 44 0f af 55 YY         imull -Y(%rbp), %r10d
  *(codigo + size + 4) = 0x44;
  *(codigo + size + 5) = 0x0f;
  *(codigo + size + 6) = 0xaf;
  *(codigo + size + 7) = 0x55;
  *(codigo + size + 8) = mem_val3;
  // 44 89 55 YY            movl %r10d, -Y(%rbp) 
  *(codigo + size + 9) = 0x44;
  *(codigo + size + 10) = 0x89;
  *(codigo + size + 11) = 0x55;
  *(codigo + size + 12) = mem_val1;
  // 13 bytes escritos, TAMEXPRMULVAR2 = 13
}

/*
Objetivo: produzir um conjunto de instruções em código de máquina que 
traduzem a operação de soma e diferença entre constante e var local
Passos:
- Escreve o início da intrução 'movl $XXXXXXXX, %r10d' no array
- Escreve o valor do primeiro elemento da operação no array
- Escreve o início da intrução 'addl -Y(%rbp), %r10d' no array
- Escreve o valor de memória referente à variável (segundo elemento) da operação no array
- Escreve o início da instrução 'movl %r10d, -Y(%rbp)' no array
- Escreve o valor de memória referente à variável que recebe o valor da operação no array
Parâmetros:
- num_var1: número da variável que recebe o valor
- num_var2: segunda variável da operação
- val: valor do primeiro elemento da operação
- op: operador da operação
- codigo: array, contém o código de máquina
- size: tamanho atual em uso do array 
Obs: Usa %r10d como registrador temporário, pois não é permitida operação
de valor em espaço de memória para valor em espaço de memória
*/
void expr_instructions_add_sub_cons_var(int num_var1, int num_var2, int val, char op, unsigned char codigo[], int size) {
  // variável que vai receber o valor
  unsigned char mem_val1 = 0xfc - 4 * (num_var1 - 1);
  // primeira variável da operação
  unsigned char mem_val2 = 0xfc - 4 * (num_var2 - 1);
  // 41 ba XX XX XX XX      movl $XXXXXXXX, %r10d
  *(codigo + size) = 0x41;
  *(codigo + size + 1) = 0xba;
  *((int*)(codigo + size + 2)) = val;  
  // 44 [03|2b] 55 YY          	[add|sub]l -Y(%rbp), %r10d
  *(codigo + size + 6) = 0x44;
  if (op == '+')
    *(codigo + size + 7) = 0x03;
  else if (op == '-') 
    *(codigo + size + 7) = 0x2b;
  *(codigo + size + 8) = 0x55;
  *(codigo + size + 9) = mem_val2;
  // 44 89 55 YY            movl %r10d, -Y(%rbp) 
  *(codigo + size + 10) = 0x44;
  *(codigo + size + 11) = 0x89;
  *(codigo + size + 12) = 0x55;
  *(codigo + size + 13) = mem_val1;
  // 14 bytes escritos, TAMEXPRASCONSVAR = 14
}

/*
Objetivo: produzir um conjunto de instruções em código de máquina que 
traduzem a operação de produto entre constante e variável local
Passos:
- Escreve o início da intrução 'movl $0xXXXXXXXX, %r10d' no array
- Escreve o valor do primeiro elemento da operação no array
- Escreve o início da intrução 'imull -Y(%rbp), %r10d' no array
- Escreve o valor de memória referente à variável (segundo elemento) da operação no array
- Escreve o início da instrução 'movl %r10d, -Y(%rbp)' no array
- Escreve o valor de memória referente à variável que recebe o valor da operação no array
Parâmetros:
- num_var1: número da variável que recebe o valor
- num_var2: segunda variável da operação
- val: valor do primeiro elemento da operação
- codigo: array, contém o código de máquina
- size: tamanho atual em uso do array 
Obs: Usa %r10d como registrador temporário, pois não é permitida operação
de valor em espaço de memória para valor em espaço de memória
*/
void expr_instructions_mul_cons_var(int num_var1, int num_var2, int val, unsigned char codigo[], int size) {
  // variável que vai receber o valor
  unsigned char mem_val1 = 0xfc - 4 * (num_var1 - 1);
  // primeira variável da operação
  unsigned char mem_val2 = 0xfc - 4 * (num_var2 - 1);
  // 41 ba XX XX XX XX      movl $0xXXXXXXXX, %r10d
  *(codigo + size) = 0x41;
  *(codigo + size + 1) = 0xba;
  *((int*)(codigo + size + 2)) = val;  
  // 44 0f af 55 YY          	imull -Y(%rbp),%r10d
  *(codigo + size + 6) = 0x44;
  *(codigo + size + 7) = 0x0f;
  *(codigo + size + 8) = 0xaf;
  *(codigo + size + 9) = 0x55;
  *(codigo + size + 10) = mem_val2;
  // 44 89 55 YY            movl %r10d, -Y(%rbp) 
  *(codigo + size + 11) = 0x44;
  *(codigo + size + 12) = 0x89;
  *(codigo + size + 13) = 0x55;
  *(codigo + size + 14) = mem_val1;
  // 15 bytes escritos, TAMEXPRMULCONSVAR = 15
}

/*
Objetivo: produzir um conjunto de instruções em código de máquina que 
traduzem a operação de soma e diferença entre var local e constante
Passos:
- Escreve o início da intrução 'movl -Y(%rbp), %r10d' no array
- Escreve o valor de memória referente à variável (primeiro elemento) da operação no array
- Escreve o início da intrução 'addl $0xXXXXXXXX, %r10d' no array
- Escreve o valor do segundo elemento da operação no array
- Escreve o início da instrução 'movl %r10d, -Y(%rbp)' no array
- Escreve o valor de memória referente à variável que recebe o valor da operação no array
Parâmetros:
- num_var1: número da variável que recebe o valor
- num_var2: segunda variável da operação
- val: valor do primeiro elemento da operação
- op: operador da operação
- codigo: array, contém o código de máquina
- size: tamanho atual em uso do array 
Obs: Usa %r10d como registrador temporário, pois não é permitida operação
de valor em espaço de memória para valor em espaço de memória
*/
void expr_instructions_add_sub_var_cons(int num_var1, int num_var2, int val, char op, unsigned char codigo[], int size) {
  // variável que vai receber o valor
  unsigned char mem_val1 = 0xfc - 4 * (num_var1 - 1);
  // primeira variável da operação
  unsigned char mem_val2 = 0xfc - 4 * (num_var2 - 1);
  // 44 8b 55 YY            movl -Y(%rbp), %r10d
  *(codigo + size) = 0x44;
  *(codigo + size + 1) = 0x8b;
  *(codigo + size + 2) = 0x55;
  *(codigo + size + 3) = mem_val2;  
  // 41 81 [c2|ea] XX XX XX XX   [add|sub]l $XXXXXXXX,%r10d
  *(codigo + size + 4) = 0x41;
  *(codigo + size + 5) = 0x81;
  if (op == '+') 
    *(codigo + size + 6) = 0xc2;
  else if (op == '-')
    *(codigo + size + 6) = 0xea;
  *((int*)(codigo + size + 7)) = val;
  // 44 89 55 YY            movl %r10d, -Y(%rbp) 
  *(codigo + size + 11) = 0x44;
  *(codigo + size + 12) = 0x89;
  *(codigo + size + 13) = 0x55;
  *(codigo + size + 14) = mem_val1;
  // 15 bytes escritos, TAMEXPRASVARCONS 15
}

/*
Objetivo: produzir um conjunto de instruções em código de máquina que 
traduzem a operação de produto entre variável local e constante
Passos:
- Escreve o início da intrução 'movl -Y(%rbp), %r10d' no array
- Escreve o valor de memória referente à variável (primeiro elemento) da operação no array
- Escreve o início da intrução 'imull $0xXXXXXXXX, %r10d' no array
- Escreve o valor do segundo elemento da operação no array
- Escreve o início da instrução 'movl %r10d, -Y(%rbp)' no array
- Escreve o valor de memória referente à variável que recebe o valor da operação no array
Parâmetros:
- num_var1: número da variável que recebe o valor
- num_var2: primeira variável da operação
- val: valor do segundo elemento da operação
- codigo: array, contém o código de máquina
- size: tamanho atual em uso do array 
Obs: Usa %r10d como registrador temporário, pois não é permitida operação
de valor em espaço de memória para valor em espaço de memória
*/
void expr_instructions_mul_var_cons(int num_var1, int num_var2, int val, unsigned char codigo[], int size) {
  // variável que vai receber o valor
  unsigned char mem_val1 = 0xfc - 4 * (num_var1 - 1);
  // primeira variável da operação
  unsigned char mem_val2 = 0xfc - 4 * (num_var2 - 1);
  // 44 8b 55 YY            movl -Y(%rbp), %r10d
  *(codigo + size) = 0x44;
  *(codigo + size + 1) = 0x8b;
  *(codigo + size + 2) = 0x55;
  *(codigo + size + 3) = mem_val2;  
  // 45 69 d2 XX XX XX XX   imull $0xXXXXXXXX,%r10d
  *(codigo + size + 4) = 0x45;
  *(codigo + size + 5) = 0x69;
  *(codigo + size + 6) = 0xd2;
  *((int*)(codigo + size + 7)) = val;
  // 44 89 55 YY            movl %r10d, -Y(%rbp) 
  *(codigo + size + 11) = 0x44;
  *(codigo + size + 12) = 0x89;
  *(codigo + size + 13) = 0x55;
  *(codigo + size + 14) = mem_val1;
  // 15 bytes escritos, TAMEXPRMULVARCONS = 15
}

/*
Objetivo: produzir um conjunto de instruções em código de máquina que 
traduzem a operação de desvio condicional para uma determinada linha
Passos:
- Escreve o início da intrução 'movl -Y(%rbp), %r10d' no array
- Escreve o valor de memória referente à variável (primeiro elemento) da operação no array
- Escreve o valor da intrução 'cmpl $0, %r10d' no array
- Escreve o valor da intrução 'jle label' no array
- Escreve o valor do offset no array, inicialmente zero
Parâmetros:
- num_var: número da variável que será comparada
- codigo: array, contém o código de máquina
- size: tamanho atual em uso do array 
Obs: Usa %r10d como registrador temporário, para 
instrução de comparação (cmpl)
*/
void jmp_instructions(int num_var, unsigned char codigo[], int size) {
  // valor referente à variável que terá valor comparado
  unsigned char mem_val = 0xfc - 4 * (num_var - 1);
  // 83 7d YY 00            cmpl $0, -Y(%rbp)
  *(codigo + size) = 0x83;
  *(codigo + size + 1) = 0x7d;
  *(codigo + size + 2) = mem_val;
  *(codigo + size + 3) = 0x00;
  // 0f 8e XX XX XX XX      jle label
  *(codigo + size + 4) = 0x0f;
  *(codigo + size + 5) = 0x8e;
  // offset inicialmente 0
  *((int*)(codigo + size + 6)) = 0x00000000;
  // 10 bytes escritos, TAMJMP = 10
}

/* 
Objetivo: escrever o offset da operação de desvio no array
Recebe o offset, o array e o tamanho (que representa o endereço 
inicial do conjunto de instruções de jump)
*/
void add_offset(int offset, unsigned char codigo[], int size) {
  *((int*)(codigo + size + 6)) = offset;
}

funcp gera(FILE *f, unsigned char codigo[]) {
  int line = 1;
  char c;
  // tamanho atual em uso do array
  int curr_size = 0;

  // referência do 'endereço' de cada linha 
  // (conjunto de instruções correspondentes)
  // usado para cálculo do offset
  unsigned short linha_endereco[31];

  // manter referencia de quais linhas possuem
  // desvios para linhas de valor maior. Caso o 
  // valor em i seja 0, não possui (impede jumps
  // para 0, que iriam para o prólogo)
  unsigned char jumps_n_maior[31];
  for (int i = 0; i < 31; i++)
    jumps_n_maior[i] = 0;

  // escrever prologo, abrindo 32 bytes no RA
  prologue_instructions(codigo);
  // linha de número 0 seria o prólogo, mas não 
  // pode ser acessada normalmente
  linha_endereco[line - 1] = curr_size;
  curr_size += TAMPROLOGO;

  while ((c = fgetc(f)) != EOF) {
    switch (c) {
    case 'r': { /* retorno */
      char var0;
      int idx0;
      if (fscanf(f, "et %c%d", &var0, &idx0) != 2)
        error("comando invalido", line);

      if (var0 == '$') {
        // retorno de constante
        linha_endereco[line] = curr_size;
        ret_instructions_cons(idx0, codigo, curr_size);
        curr_size += TAMRETORNOCONS;
      } else {
        // retorno de valor em variável local
        linha_endereco[line] = curr_size;
        ret_instructions_var(idx0, codigo, curr_size);
        curr_size += TAMRETORNOVAR;
      }
      printf("%d ret %c%d\n", line, var0, idx0);
      break;
    }
    case 'v': { /* atribuição e op. aritmetica */
      int idx0, idx1;
      char var0 = c, c0, var1;
      if (fscanf(f, "%d %c", &idx0, &c0) != 2)
        error("comando invalido", line);

      if (c0 == '<') { /* atribuição */
        if (fscanf(f, " %c%d", &var1, &idx1) != 2)
          error("comando invalido", line);
        if (var1 == '$') {
          // atribuição de constante em variável local
          linha_endereco[line] = curr_size;
          attr_instructions_cons(idx0, idx1, codigo, curr_size);
          curr_size += TAMATRIBCONS;
        } else if (var1 == 'v') {
          // atribuição de variável local em variável local
          linha_endereco[line] = curr_size;
          attr_instructions_var(idx0, idx1, codigo, curr_size);
          curr_size += TAMATRIBVAR;
        } else if (var1 == 'p') {
          // atribuição de parâmetro em variável local
          linha_endereco[line] = curr_size;
          attr_instructions_par(idx0, idx1, codigo, curr_size);
          curr_size += TAMATRIBPAR;
        }
        printf("%d %c%d < %c%d\n", line, var0, idx0, var1, idx1);
      } else { /* operação aritmética */
        char var2, op;
        int idx2;
        if (c0 != '=')
          error("comando invalido", line);
        if (fscanf(f, " %c%d %c %c%d", &var1, &idx1, &op, &var2, &idx2) != 5)
          error("comando invalido", line);
        if (var1 == '$' && var2 == '$') {
          if (op == '+' || op == '-') {
            // soma e diferença de constantes
            linha_endereco[line] = curr_size;
            expr_instructions_add_sub_cons_cons(idx0, idx1, idx2, op, codigo, curr_size);
            curr_size += TAMEXPRASCONS2;
          } else {
            // produto de constantes
            linha_endereco[line] = curr_size;
            expr_instructions_mul_cons_cons(idx0, idx1, idx2, codigo, curr_size);
            curr_size += TAMEXPRMULCONS2;
          }
        } else if (var1 == 'v' && var2 == 'v') {
          if (op == '+' || op == '-') {
            // soma e diferença de variáveis locais
            linha_endereco[line] = curr_size;
            expr_instructions_add_sub_var_var(idx0, idx1, idx2, op, codigo, curr_size);
            curr_size += TAMEXPRASVAR2;
          } else {
            // produto de variáveis locais
            linha_endereco[line] = curr_size;
            expr_instructions_mul_var_var(idx0, idx1, idx2, codigo, curr_size);
            curr_size += TAMEXPRMULVAR2;
          }
        } else if (var1 == '$' && var2 == 'v') {
          if (op == '+' || op == '-') {
            // soma e diferença entre constante e variável local
            linha_endereco[line] = curr_size;
            expr_instructions_add_sub_cons_var(idx0, idx2, idx1, op, codigo, curr_size);
            curr_size += TAMEXPRASCONSVAR;
          } else {
            // produto entre constante e variável local
            linha_endereco[line] = curr_size;
            expr_instructions_mul_cons_var(idx0, idx2, idx1, codigo, curr_size);
            curr_size += TAMEXPRMULCONSVAR;
          }
        } else if (var1 == 'v' && var2 == '$') {
          if (op == '+' || op == '-') {
            // soma e diferença entre variável local e constante
            linha_endereco[line] = curr_size;
            expr_instructions_add_sub_var_cons(idx0, idx1, idx2, op, codigo, curr_size);
            curr_size += TAMEXPRASVARCONS;
          } else {
            // produto entre variável local e constante
            linha_endereco[line] = curr_size;
            expr_instructions_mul_var_cons(idx0, idx1, idx2, codigo, curr_size);
            curr_size += TAMEXPRMULVARCONS;
          }
        } 
        printf("%d %c%d = %c%d %c %c%d\n", line, var0, idx0, var1, idx1, op,
               var2, idx2);
      }
      break;
    }
    case 'i': { /* desvio condicional */
      char var0;
      int idx0, n;
      if (fscanf(f, "flez %c%d %d", &var0, &idx0, &n) != 3)
        error("comando invalido", line);
      int offset = 0;
      // escreve a operação de desvio, indiferente à linha do jump, a princípio
      linha_endereco[line] = curr_size;  
      jmp_instructions(idx0, codigo, curr_size);
      curr_size += TAMJMP;
      /* 
      se n for linha anterior, pode ser calculado
      offset utilizando o array de endereços. No entanto,
      se for uma linha posterior, tem referência a linha atual 
      guardada para uso futuro (e cálculo do offset). 
      O cálculo é feito com base na próxima linha (conjunto de
      instruções), assim como o jmp normalmente
      */
      if (n < line) 
        offset = linha_endereco[n] - (linha_endereco[line] + TAMJMP);
      else if (n > line)
        jumps_n_maior[line] = n;
      // adiciona o offset calculado
      add_offset(offset, codigo, linha_endereco[line]);
      printf("%d iflez %c%d %d\n", line, var0, idx0, n);
      break;
    }
    default:
      error("comando desconhecido", line);
    }
    line++;
    fscanf(f, " ");
  }

  // operação para calcular offset para jumps em linha maior, pois não 
  // possível saber de antemão como o resto do programa vai estar
  // percorre o array jumps_n_maior até encontrar uma linha que possui
  // um valor diferente de 0, o que indica um jump para n maior, e calcula o
  // valor adequado do offset com base no valor da linha para qual se quer 
  // realizar o jump (n) e a linha atual (i) mais o tamanho da operação de jump, 
  // TAMJMP. Então, adiciona esse offset no local adequado
  for (int i = 0; i < 31; i++) {
    int n = jumps_n_maior[i];
    if (n != 0) {
      int offset = linha_endereco[n] - (linha_endereco[i] + TAMJMP);
      add_offset(offset, codigo, linha_endereco[i]);
    }
  }

  return (funcp)codigo;
}
