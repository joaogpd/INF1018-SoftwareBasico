/*
João Gabriel Peixoto Drumond Lopes - 2210168 - 3WB
Jayme Augusto Avelino de Paiva - 2210289 - 3WB
*/
#include <stdio.h>
#include <limits.h>
#include "bigint.h"
#include <stdlib.h>

unsigned char byte1 = 0x00;
unsigned char byte2 = 0xFF;
unsigned char byte3 = 0x01;
unsigned char byte4 = 0x70;
unsigned char byte5 = 0xF0;
unsigned char byte6 = 0x02;
unsigned char byte7 = 0x04;
unsigned char byte8 = 0x08;
unsigned char byte9 = 0x10;
unsigned char byte10 = 0xAA;
unsigned char byte11 = 0xcd;
unsigned char byte12 = 0x8f;
unsigned char byte13 = 0xfe;
unsigned char byte14 = 0x80;

void dump(void* p, int n) {
    unsigned char* p1 = (unsigned char*)p;
    while (n--) {
        printf("%p - %02x\n", p1, *p1);
        p1++;
    }
}

// Compara dois BigInt. Caso exista diferenca, imprime "FAILED\n"
void compare_big(BigInt a, BigInt b) {
    for (int i = 0; i < 16; i++) {
        if (a[i] != b[i]) {
            printf("FAILED\n");
            return;
        }
    }
    printf("SUCCESS\n");
}

// Compara um BigInt com um long, levando em conta a extensao adequada. Caso exista diferenca, imprime "FAILED\n"
void compare_long(BigInt a, void* b) {
    unsigned char* b1 = (unsigned char*)b;
    for (int i = 0; i < 8; i++) {
        if (a[i] != *(b1 + i)) {
            printf("FAILED\n");
            return;
        }
    }
    if (*(b1 + 7) & 0x80) {
        for (int i = 8; i < 16; i++) {
            if (a[i] != 0xFF) {
                printf("FAILED\n");
                return;
            }
        }
    } else {
        for (int i = 8; i < 16; i++) {
            if (a[i] != 0x00) {
                printf("FAILED\n");
                return;
            }
        }
    }
    printf("SUCCESS\n");
}

// a: valor inserido na funcao. b: valor de retorno esperado
void teste_big_val(BigInt a, long b) {
    big_val(a, b);
    compare_long(a, &b);
}

// a: valor inserido na funcao. b: valor de retorno esperado
void teste_big_comp2(BigInt a, BigInt b){
    big_comp2(a, a);
    compare_big(a, b);
}

// a e b: valores inseridos na funcao. c: valor de retorno esperado
void teste_big_sum(BigInt a, BigInt b, BigInt c) {
    BigInt d;
    big_sum(d, a, b);
    compare_big(d, c);
}

// a e b: valores inseridos na funcao. c: valor de retorno esperado
void teste_big_sub(BigInt a, BigInt b, BigInt c) {
    BigInt d;
    big_sub(d, a, b);
    compare_big(d, c);
}

// a e b: valores inseridos na funcao. c: valor de retorno esperado
void teste_big_mul(BigInt a, BigInt b, BigInt c) {
    BigInt d;
    big_mul(d, a, b);
    compare_big(d, c);
}

// a: valor inserido na funcao. b: valor de retorno esperado. n: numero do shift
void teste_big_shl(BigInt a, BigInt b, int n) {
    BigInt c;
    big_shl(c, a, n);
    compare_big(c, b);
}

// a: valor inserido na funcao. b: valor de retorno esperado. n: numero do shift
void teste_big_shr(BigInt a, BigInt b, int n) {
    BigInt c;
    big_shr(c, a, n);
    compare_big(c, b);
}

// a: valor inserido na funcao. b: valor de retorno esperado. n: numero do shift
void teste_big_sar(BigInt a, BigInt b, int n) {
    BigInt c;
    big_sar(c, a, n);
    compare_big(c, b);
}

int main(void) {
    // Testagem big_val() //
    printf("big_val()\n");
    BigInt c;
    // numero positivo
    teste_big_val(c, 534534354);
    // numero negativo
    teste_big_val(c, -87635434);
    teste_big_val(c, LONG_MAX);
    teste_big_val(c, LONG_MIN);
    teste_big_val(c, 0);
    

    // Testagem big_comp2()
    printf("big_comp2()\n");
    // 1 e -1
    do {
    	BigInt a = {byte3, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1};
    	BigInt b = {byte2, byte2, byte2, byte2, byte2, byte2, byte2, byte2, byte2, byte2, byte2, byte2, byte2, byte2, byte2, byte2};
    	teste_big_comp2(a, b);
    } while (0);
    // -1 e 1
    do {
        BigInt a = {byte2, byte2, byte2, byte2, byte2, byte2, byte2, byte2, byte2, byte2, byte2, byte2, byte2, byte2, byte2, byte2};
        BigInt b = {byte3, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1};
        teste_big_comp2(a, b);
    } while (0);
    // 0 e 0
    do {
        BigInt a = {byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1};
        BigInt b = {byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1};
        teste_big_comp2(a, b);
    } while (0);
    // 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF e 0x00000000000000000000000000000001
    do {
        BigInt a = {byte2, byte2, byte2, byte2, byte2, byte2, byte2, byte2, byte2, byte2, byte2, byte2, byte2, byte2, byte2, byte2};
        BigInt b = {byte3, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1};
        teste_big_comp2(a, b);
    } while (0);
    // 0xFFFF010000000000000000000000F070 e 0x0000FEFFFFFFFFFFFFFFFFFFFFFF8F10
    do {
        BigInt a = {byte5, byte4, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte3, byte2, byte2};
    	BigInt b = {byte9, byte12, byte2, byte2, byte2, byte2, byte2, byte2, byte2, byte2, byte2, byte2, byte2, byte13, byte1, byte1};
    	teste_big_comp2(a, b);
    } while (0);
    // 0x0000FEFFFFFFFFFFFFFFFFFFFFFF8F10 e 0xFFFF010000000000000000000000F070
    do {
    	BigInt a = {byte9, byte12, byte2, byte2, byte2, byte2, byte2, byte2, byte2, byte2, byte2, byte2, byte2, byte13, byte1, byte1};
        BigInt b = {byte5, byte4, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte1, byte3, byte2, byte2};
    	teste_big_comp2(a, b);
    } while (0);
    
    // Testagem big_sum()
    printf("big_sum()\n");
    // 0xFF000000000000AAAAAAAAFEFEFEFEFE + 0x00FFFFFFFFFFAAAAAA04040404047070 (positivo + negativo)
    do {
        BigInt a = {byte13, byte13, byte13, byte13, byte13, byte10, byte10, byte10, byte10, byte1, byte1, byte1, byte1, byte1, byte1, byte2};
        BigInt b = {byte4, byte4, byte7, byte7, byte7, byte7, byte7, byte10, byte10, byte10, byte2, byte2, byte2, byte2, byte2, byte1};
        BigInt c = {0x6e, 0x6f, 0x03, 0x03, 0x03, 0xaf, 0xae, 0x54, 0x55, 0xab, byte2, byte2, byte2, byte2, byte2, byte2};
        teste_big_sum(a, b, c);
    } while (0);
    // 0xFF000000000000AAAAAAAAFEFEFEFEFE + 0xFFFFFFFFFFFFAAAAAA04040404047070 (negativo + negativo)
    do {
        BigInt a = {byte13, byte13, byte13, byte13, byte13, byte10, byte10, byte10, byte10, byte1, byte1, byte1, byte1, byte1, byte1, byte2};
        BigInt b = {byte4, byte4, byte7, byte7, byte7, byte7, byte7, byte10, byte10, byte10, byte2, byte2, byte2, byte2, byte2, byte2};
        BigInt c = {0x6e, 0x6f, 0x03, 0x03, 0x03, 0xaf, 0xae, 0x54, 0x55, 0xab, byte2, byte2, byte2, byte2, byte2, byte13};
        teste_big_sum(a, b, c);
    } while (0);
    // 0x00FFFFFFFFFFFFAAAAAAAA0101010101 + 0x08FFFFFEFEFE04040404707070808182 (positivo + positivo)
    do {
        BigInt a = {0x01, 0x01, 0x01, 0x01, 0x01, 0xaa, 0xaa, 0xaa, 0xaa, byte2, byte2, byte2, byte2, byte2, byte2, byte1};
        BigInt b = {0x82, 0x81, 0x80, 0x70, 0x70, 0x70, 0x04, 0x04, 0x04, 0x04, 0xfe, 0xfe, 0xfe, 0xff, 0xff, 0x08};
        BigInt c = {0x83, 0x82, 0x81, 0x71, 0x71, 0x1a, 0xaf, 0xae, 0xae, 0x03, 0xfe, 0xfe, 0xfe, 0xff, 0xff, 0x09};
        teste_big_sum(a, b, c);
    } while (0);
    // BIGINT_MIN + BIGINT_MIN
    do {
        BigInt a;
        for (int i = 0; i < 16; i++)
            a[i] = byte2;
        BigInt b;
        for (int j = 0; j < 16; j++)
            b[j] = byte2;
        BigInt c;
        for (int k = 0; k < 16; k++)
            c[k] = byte2;
        c[0] = 0xfe;
        teste_big_sum(a, b, c);
    } while (0);
    // BIGINT_MAX + BIGINT_MAX
    do {
    	BigInt a;
        for (int i = 0; i < 16; i++)
            a[i] = byte2;
        a[15] = 0x7f;
        BigInt b;
        for (int j = 0; j < 16; j++)
            b[j] = byte2;
        b[15] = 0x7f;
        BigInt c;
        for (int k = 0; k < 16; k++)
            c[k] = byte2;
        c[0] = 0xfe;
        teste_big_sum(a, b, c);
    } while (0);
    
    // Testagem big_sub()
    printf("big_sub()\n");
    // BIGINT_MIN - BIGINT_MIN
    do {
        BigInt a;
        for (int i = 0; i < 16; i++)
            a[i] = byte2;
        BigInt b;
        for (int j = 0; j < 16; j++)
            b[j] = byte2;
        BigInt c;
        for (int k = 0; k < 16; k++)
            c[k] = byte1;
        teste_big_sub(a, b, c);
    } while (0);
    // BIGINT_MAX - BIGINT_MAX
    do {
    	BigInt a;
        for (int i = 0; i < 16; i++)
            a[i] = byte2;
        a[15] = 0x7f;
        BigInt b;
        for (int j = 0; j < 16; j++)
            b[j] = byte2;
        b[15] = 0x7f;
        BigInt c;
        for (int k = 0; k < 16; k++)
            c[k] = byte1;
        teste_big_sub(a, b, c);
    } while (0);

    // Testagem big_mul()
    printf("big_mul()\n");
    // 0 * 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
    do {
    	BigInt a;
        for (int i = 0; i < 16; i++)
            a[i] = byte1;
        BigInt b;
        for (int j = 0; j < 16; j++)
            b[j] = byte2;
        BigInt c;
        for (int k = 0; k < 16; k++)
            c[k] = byte1;
        teste_big_mul(a, b, c);
    } while (0);
    // 0xFFFF0000010101020280807676767600 * 0x00FF0101909076768989808080808080 (negativo * positivo)
    do {
    	BigInt a = {0x00, 0x76, 0x76, 0x76, 0x76, 0x80, 0x80, 0x02, 0x02, 0x01, 0x01, 0x01, 0x00, 0x00, 0xff, 0xff};
    	BigInt b = {0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x89, 0x89, 0x76, 0x76, 0x90, 0x90, 0x01, 0x01, 0xff, 0x00};
    	BigInt c = {0x00, 0x00, 0x3b, 0x76, 0xb1, 0xec, 0x2c, 0x93, 0xbe, 0xa7, 0x87, 0xbd, 0x7f, 0x2b, 0x10, 0xdd};
    	teste_big_mul(a, b, c);
    } while (0);
    // BIGINT_MAX * BIGINT_MAX (positivo * positivo)
    do {
    	BigInt a;
        for (int i = 0; i < 16; i++)
            a[i] = byte2;
        a[15] = 0x7f;
        BigInt b;
        for (int j = 0; j < 16; j++)
            b[j] = byte2;
        b[15] = 0x7f;
        BigInt c;
        for (int k = 0; k < 16; k++)
            c[k] = byte1;
        c[0] = 0x01;
        teste_big_mul(a, b, c);
    } while (0);
    // BIGINT_MIN * BIGINT_MIN (negativo * negativo)
     do {
        BigInt a;
        for (int i = 0; i < 16; i++)
            a[i] = byte2;
        BigInt b;
        for (int j = 0; j < 16; j++)
            b[j] = byte2;
        BigInt c;
        for (int k = 0; k < 16; k++)
            c[k] = byte1;
        c[0] = 0x01;
        teste_big_mul(a, b, c);
    } while (0);
    
    // Testagem big_shl()
    printf("big_shl()\n");
    // 0xFEFEFEFEFEFE767676768989000000FF << 0
    do {
        BigInt a = {byte2, byte1, byte1, byte1, 0x89, 0x89, 0x76, 0x76, 0x76, 0x76, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe};
        BigInt b = {byte2, byte1, byte1, byte1, 0x89, 0x89, 0x76, 0x76, 0x76, 0x76, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe};
        int n = 0;
        teste_big_shl(a, b, n);
    } while (0);
    // 0xFEFEFEFEFEFE767676768989000000FF << 127
    do {
        BigInt a = {byte2, byte1, byte1, byte1, 0x89, 0x89, 0x76, 0x76, 0x76, 0x76, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe};
        BigInt b;
        for (int k = 0; k < 16; k++)
            b[k] = byte1;
        b[15] = 0x80;
        int n = 127;
        teste_big_shl(a, b, n);
    } while (0);
    // 0xFEFEFEFEFEFE767676768989000000FF << 45
    do {
        BigInt a = {byte2, byte1, byte1, byte1, 0x89, 0x89, 0x76, 0x76, 0x76, 0x76, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe};
        BigInt b = {0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x1f, 0x00, 0x00, 0x20, 0x31, 0xd1, 0xce, 0xce, 0xce, 0xce};
        int n = 45;
        teste_big_shl(a, b, n);
    } while (0);
    // Testagem big_shr()
    printf("big_shr()\n");
    // 0xFEFEFEFEFEFE767676768989000000FF >> 0
    do {
        BigInt a = {byte2, byte1, byte1, byte1, 0x89, 0x89, 0x76, 0x76, 0x76, 0x76, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe};
        BigInt b = {byte2, byte1, byte1, byte1, 0x89, 0x89, 0x76, 0x76, 0x76, 0x76, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe};
        int n = 0;
        teste_big_shr(a, b, n);
    } while (0);
    // 0xFEFEFEFEFEFE767676768989000000FF >> 127
    do {
        BigInt a = {byte2, byte1, byte1, byte1, 0x89, 0x89, 0x76, 0x76, 0x76, 0x76, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe};
        BigInt b;
        for (int k = 0; k < 16; k++)
            b[k] = byte1;
        b[0] = 0x01;
        int n = 127;
        teste_big_shr(a, b, n);
    } while (0);
    // 0xFEFEFEFEFEFE767676768989000000FF >> 45
    do {
        BigInt a = {byte2, byte1, byte1, byte1, 0x89, 0x89, 0x76, 0x76, 0x76, 0x76, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe};
        BigInt b = {0xb4, 0xb3, 0xb3, 0xb3, 0xf3, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00};
        int n = 45;
        teste_big_shr(a, b, n);
    } while (0);
    // Testagem big_sar()
    printf("big_sar()\n");
    // 0xFEFEFEFEFEFE767676768989000000FF >> 0 (negativo)
    do {
        BigInt a = {byte2, byte1, byte1, byte1, 0x89, 0x89, 0x76, 0x76, 0x76, 0x76, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe};
        BigInt b = {byte2, byte1, byte1, byte1, 0x89, 0x89, 0x76, 0x76, 0x76, 0x76, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe};
        int n = 0;
        teste_big_sar(a, b, n);
    } while (0);
    // 0xFEFEFEFEFEFE767676768989000000FF >> 127 (negativo)
    do {
        BigInt a = {byte2, byte1, byte1, byte1, 0x89, 0x89, 0x76, 0x76, 0x76, 0x76, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe};
        BigInt b;
        for (int k = 0; k < 16; k++)
            b[k] = byte2;
        b[0] = 0x01;
        int n = 127;
        teste_big_sar(a, b, n);
    } while (0);
    // 0xFEFEFEFEFEFE767676768989000000FF >> 45 (negativo)
    do {
        BigInt a = {byte2, byte1, byte1, byte1, 0x89, 0x89, 0x76, 0x76, 0x76, 0x76, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe};
        BigInt b = {0xb4, 0xb3, 0xb3, 0xb3, 0xf3, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
        int n = 45;
        teste_big_sar(a, b, n);
    } while (0);
    
    // 0x0EFEFEFEFEFE767676768989000000FF >> 0 (positivo)
    do {
        BigInt a = {byte2, byte1, byte1, byte1, 0x89, 0x89, 0x76, 0x76, 0x76, 0x76, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0x0e};
        BigInt b = {byte2, byte1, byte1, byte1, 0x89, 0x89, 0x76, 0x76, 0x76, 0x76, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0x0e};
        int n = 0;
        teste_big_sar(a, b, n);
    } while (0);
    // 0x0EFEFEFEFEFE767676768989000000FF >> 127 (positivo)
    do {
        BigInt a = {byte2, byte1, byte1, byte1, 0x89, 0x89, 0x76, 0x76, 0x76, 0x76, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0x0e};
        BigInt b;
        for (int k = 0; k < 16; k++)
            b[k] = byte1;
        int n = 127;
        teste_big_sar(a, b, n);
    } while (0);
    // 0x0EFEFEFEFEFE767676768989000000FF >> 45 (positivo)
    do {
        BigInt a = {byte2, byte1, byte1, byte1, 0x89, 0x89, 0x76, 0x76, 0x76, 0x76, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0x0e};
        BigInt b = {0xb4, 0xb3, 0xb3, 0xb3, 0xf3, 0xf7, 0xf7, 0xf7, 0xf7, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        int n = 45;
        teste_big_sar(a, b, n);
    } while (0);

    return 0;
}

