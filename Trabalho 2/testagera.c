/* João Gabriel Peixoto Drumond Lopes 2210168 3WB */
/* Jayme Augusto Avelino de Paiva 2210289 3WB */

#include "gera.h"
#include <stdio.h>

// tamanho máximo definido com base na quantidade máxima de linhas
// proposta no texto do trabalho (30), vezes o comando que gera a 
// maior quantidade em bytes em código de máquina, mais 8 bytes
// do tamanho do prólogo
#define MAXSIZE ((17 * 30) + 8)

/*
Função recebe: nome do arquivo a ser aberto, quantidade de parâmetros
para função gerada, parâmetros (em geral, caso inutilizados, valor = -1, 
mas esse valor não altera em nada) e o valor esperado pela execução da função.
Exibe mensagem de erro ou sucesso de acordo com o valor retornado
*/
void test_function(char* file_name, int qtd_par, int par1, int par2, int par3, int valor_esperado) {
  // inicializa o array na stack a cada execução
  // para prevenir valores indesejados de uma
  // execução anterior da função de teste
  unsigned char code_array[MAXSIZE];
  
  FILE* test_file = fopen(file_name, "r");
  if (test_file == NULL) {
    fprintf(stderr, "Erro ao abrir o arquivo %s\n", file_name);
    return;
  }
  funcp test_function;
  
  test_function = gera(test_file, code_array);
  
  // realiza a verificação da quantidade de parâmetros necessários
  // para a função gerada e que deve ser testada. Então, verifica se
  // valor retornado é igual ao valor esperado pela execução
  if (qtd_par == 1) {
    printf("Realizando teste de: %s, com parâmetro %d\n", file_name, par1);
    int valor_retornado = test_function(par1);
    if (valor_retornado == valor_esperado)
      printf("SUCCESS, expected %d and got %d\n", valor_esperado, valor_retornado);
    else 
      printf("FAILURE, expected %d and got %d\n", valor_esperado, valor_retornado);
  } else if (qtd_par == 2) {
    printf("Realizando teste de: %s, com parâmetros %d e %d\n", file_name, par1, par2);
    int valor_retornado = test_function(par1, par2);
    if (valor_retornado == valor_esperado)
      printf("SUCCESS, expected %d and got %d\n", valor_esperado, valor_retornado);
    else 
      printf("FAILURE, expected %d and got %d\n", valor_esperado, valor_retornado);
  } else if (qtd_par == 3) {
    printf("Realizando teste de: %s, com parâmetros %d, %d e %d\n", file_name, par1, par2, par3);
    int valor_retornado = test_function(par1, par2, par3);
    if (valor_retornado == valor_esperado)
      printf("SUCCESS, expected %d and got %d\n", valor_esperado, valor_retornado);
    else 
      printf("FAILURE, expected %d and got %d\n", valor_esperado, valor_retornado);
  } else {
    printf("Realizando teste de: %s\n", file_name);
    int valor_retornado = test_function();
    if (valor_retornado == valor_esperado)
      printf("SUCCESS, expected %d and got %d\n", valor_esperado, valor_retornado);
    else 
      printf("FAILURE, expected %d and got %d\n", valor_esperado, valor_retornado);
  }
  
  fclose(test_file);
}

int main(void) {
  // Teste 1: Fatorial
  test_function("fatorial.sp", 1, 5, -1, -1, 120);
  test_function("fatorial.sp", 1, 3, -1, -1, 6);
  test_function("fatorial.sp", 1, 0, -1, -1, 1);
  test_function("fatorial.sp", 1, 1, -1, -1, 1);
  test_function("fatorial.sp", 1, 10, -1, -1, 3628800);
  
  // Teste 2: Incremento simples
  test_function("incremento.sp", 1, 7, -1, -1, 8);
  test_function("incremento.sp", 1, -4, -1, -1, -3);
  test_function("incremento.sp", 1, 0, -1, -1, 1);
  
  // Teste 3: Negativo ou positivo
  test_function("enegativo.sp", 1, -3, -1, -1, 1);
  test_function("enegativo.sp", 1, 3, -1, -1, 0);
  test_function("enegativo.sp", 1, 0, -1, -1, 0);
  
  // Teste 4: f(x, y) = (x + y) * (x - y)
  test_function("funcxy1.sp", 2, 3, 4, -1, -7);
  test_function("funcxy1.sp", 2, -2, 4, -1, -12);
  test_function("funcxy1.sp", 2, 3, -4, -1, -7);

  // Teste 5: Dobra valor
  test_function("dobra.sp", 1, 4, -1, -1, 8);
  test_function("dobra.sp", 1, -3, -1, -1, -6);
  test_function("dobra.sp", 1, 0, -1, -1, 0);
  
  // Teste 6: Retorno de constante
  test_function("caso1.sp", 0, -1, -1, -1, 100);
  
  // Teste 7: Retorno de valor em variável local e 
  // atribuição de constante em variável local
  test_function("caso2.sp", 0, -1, -1, -1, 20);
  
  // Teste 8: Atribuição de constante negativa 
  test_function("caso3.sp", 0, -1, -1, -1, -12);
  
  // Teste 9: Retorno do valor no primeiro parâmetro
  test_function("caso4.sp", 1, 2, -1, -1, 2);
  
  // Teste 10: Retorno do valor no segundo parâmetro
  test_function("caso5.sp", 2, 2, -5, -1, -5);
  
  // Teste 11: Retorno do valor no terceiro parâmetro
  test_function("caso6.sp", 3, 2, -5, 8, 8);
  
  // Teste 12: Soma de valores constantes
  test_function("caso7.sp", 0, -1, -1, -1, 6);
  
  // Teste 13: Soma de variável e constante
  test_function("caso8.sp", 0, -1, -1, -1, -1);
  
  // Teste 14: Soma de constante e variável
  test_function("caso9.sp", 0, -1, -1, -1, 88);
  
  // Teste 15: Soma de valores em variável local
  test_function("caso10.sp", 0, -1, -1, -1, 0);
  
  // Teste 16: Diferença de constantes
  test_function("caso11.sp", 0, -1, -1, -1, -17);
  
  // Teste 17: Diferença de valores em variável local
  test_function("caso12.sp", 0, -1, -1, -1, 27);
  
  // Teste 18: Diferença de variável local e constante
  test_function("caso13.sp", 0, -1, -1, -1, 2);
  
  // Teste 19: Diferença de constante e variável local
  test_function("caso14.sp", 0, -1, -1, -1, -1);
  
  // Teste 20: Produto de constantes
  test_function("caso15.sp", 0, -1, -1, -1, -12);
  
  // Teste 21: Produto de variável local e constante
  test_function("caso16.sp", 0, -1, -1, -1, 20);
  
  // Teste 22: Produto de variável local e constante
  test_function("caso17.sp", 0, -1, -1, -1, 25);
  
  // Teste 23: Produto de variáveis locais
  test_function("caso18.sp", 0, -1, -1, -1, -24);
  
  // Teste 24: Desvio com variável igual a zero
  test_function("caso19.sp", 0, -1, -1, -1, 5);

  // Teste 25: Desvio com variável menor que zero
  // para linha de valor maior que o atual
  test_function("caso20.sp", 0, -1, -1, -1, 5);

  // Teste 26: Desvio com variável maior que zero
  test_function("caso21.sp", 0, -1, -1, -1, 6);

  // Teste 27: Desvio com variável menor que zero
  // para linha de valor menor que o atual
  test_function("caso22.sp", 0, -1, -1, -1, 4);

  // Teste 28: Desvio para a linha de número 30
  test_function("jump30.sp", 0, -1, -1, -1, -3);

  return 0;
}
