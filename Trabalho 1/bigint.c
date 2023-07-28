/*
João Gabriel Peixoto Drumond Lopes - 2210168 - 3WB
Jayme Augusto Avelino de Paiva - 2210289 - 3WB
Nota 9.7 de 10
*/
#include <stdio.h>
#include "bigint.h"

void big_val(BigInt res, long val){
    //ponteiro para varrer os bytes de val
    unsigned char* p = (unsigned char*)(&val);
    int i = 0;
    //inserindo bytes de val em res usando o ponteiro p
    while (i < (int)sizeof(long)){
        *(res+i) = *(p+i);
        i++;      
    }
    //preenchendo o resto dos bytes de bigint considerando se val é negativo ou positivo olhando o valor do ultimo bit de val via ponteiro p e operação lógica
    unsigned char lastByte = ((0x80&(*(p+7))) == 0 ? 0x00 : 0xFF);
    for (i=0;i < (int)sizeof(long);i++){
        *(res+8+i) = lastByte;
    }
}

void big_comp2(BigInt res, BigInt a){
    //variavel pra somar 1 depois de inverter os bits e pra carregar o "vai 1" após somar 1
    int acres = 1;
    //variavel pra saber se é necessário ter um "vai 1"
    int boolAux;
    //loop para percorrer os big ints "res" e "a"
    for (int i = 0; i<NUM_BITS/8; i++){
       //colocando em "res" os bits invertidos de "a" 
        *(res+i) = ~(*(a+i));
        //usando boolAux para saber se antes de somar o "acres" o byte valia 0
        boolAux = ((*(res+i)) != 0x00);
        //if para evitar entrar nesse bloco caso não haja "vai 1" (lembrando que acres já começa valendo 1 por conta do 1 que deve ser somado ao fazer o comp2)
        if (acres){
            //somando o acres no byte
            *(res+i) += acres;
            //verificando, se antes de somar o 1 o byte valia diferente de 0 e após somar ele passa a ser 0, é pq esse 1 deve ser passado adiante como "vai 1" dentro de acres
            acres = (boolAux && ((*(res+i)) == 0x00));
        }
    }
}

void big_sum(BigInt res, BigInt a, BigInt b){
    //variavel para aguardar acrescimo (vai 1) na soma
    int acres = 0;
    //loop para varrer os bigints recebidos nos parametros
    for (int i = 0; i < NUM_BITS/8; i++){
        //guardando valores dos bytes
        unsigned char v1 = *(a+i);
        unsigned char v2 = *(b+i);
        //realizando a soma dos valores em um int para não perder informação
        unsigned int sum = v1+v2;
        //colocando no byte de "res" o valor da soma mais o acressimo
        *(res+i) = sum + acres;
        //descobrindo se deve haver um acrescimo para o proximo byte (se sum+acres for maior que 255 é pq deve haver)
        acres = (sum+acres > 255); 
    }
}

void big_sub(BigInt res, BigInt a, BigInt b){
    //criando um bigint auxiliar "c" pra ser -b e usando a função big_sum para por em res a+c (a+(-b))
    BigInt c;
    big_comp2(c,b);
    big_sum(res,a,c);
}

//função auxiliar para copiar o valor de um bigint "a" para um bigint "res"
void copyBigInt(BigInt res, BigInt a){
    for(int i = 0;i<NUM_BITS/8;i++){
        *(res+i) = *(a+i);   
    }
}

void big_mul(BigInt res, BigInt a, BigInt b){
    //copiando os valores dos bigints "a" e "b" para bigints aux, para que possa alterar os aux sem alterar os originais
    BigInt aAux;
    BigInt bAux;
    copyBigInt(aAux, a);
    copyBigInt(bAux, b);
    
    //iniciando o valor de "res" como 0
    long zero = 0;
    big_val(res,zero);
    
    //descobrindo se os big ints recebidos por parâmetro são negativos ou positivos
    int boolANeg = (*(a+NUM_BITS/8-1))&(0x80);
    int boolBNeg = (*(b+NUM_BITS/8-1))&(0X80);

    //transformando os big ints aux em positivos caso eles sejam negativos
    if (boolANeg)
        big_comp2(aAux, aAux);

    if (boolBNeg)
        big_comp2(bAux, bAux);
    
    //variavel para guardar o valor correto a ser posto no byte, após retirada dos valores que devem ir pro byte seguinte
    int n;
    //variavel para guardar o valor total das contas byte a byte (sem retirada dos valores que devem ir pro byte seguinte)
    int val;
    //variavel para guardar o valor que deve ir pro byte seguinte
    int acres;
    //loop para percorrer os bigints
    for (int i=0; i<NUM_BITS/8; i++){
        //iniciando o acrescimo como 0 em todo loop para fazer as contas de cada "parcela" da multiplicação
        acres = 0;
        //loop para percorrer o segundo bigint na hora de fazer as contas, pegando o byte i do primeiro bigint e o byte j do segundo
        for (int j=0; j<NUM_BITS/8-i; j++){
            /*
              fazendo as contas pegando o byte i do primeiro bigint vezes o byte j do segundo, 
              somando com o valor que já está no byte (i+j) do resultado e somando com o valor do acréscimo vindo de contas anteriores
             */ 
            val = (*(aAux+i)) * (*(bAux+j)) + *(res+i+j) + acres;
            //valor a ser acrescido no proximo loop vai ser o quociente da divisão de val por 256
            acres = (int)val/256;
            //valor a ser posto no byte (i+j) do resultado vai ser igual ao val menos o valor a ser acrescido no proximo loop (considerando que o peso deste vai ser 256)
            n = val - acres*256;
            *(res+i+j) = n; 
        }
    }
    //transformando o valor de res em -res caso o sinal de um e apenas um dos parametros seja negativo
    if ((!(boolANeg && boolBNeg)) && (boolANeg || boolBNeg))
        big_comp2(res,res);
}

void big_shl(BigInt res, BigInt a, int n){
    //esse algoritmo apenas multiplica o valor de bigint por 10^n (base binaria) onde n é o número de casas a ser shiftada para a esquerda, se n=2, o valor é 100, 0 é 1, 5 é 100000 e por ae vai
    //obs.: o valor 10^n é calculado colocando o valor 1 no byte correto de um big int que antes era 0 e fazendo a quantidade de shifts para direita necessaria nesse byte
    long zero = 0;
    BigInt aux;
    big_val(aux,zero);

    int numBytes = (int)n/8;
    int numBits = n - numBytes*8;
    
    *(aux+numBytes) = 1;
    *(aux+numBytes) = *(aux+numBytes) << numBits;
    big_mul(res,a,aux);
}

void big_shr(BigInt res, BigInt a, int n){
    //criando um bigint aux que vale 0 inicialmente
    BigInt aux;
    long zero = 0;
    big_val(aux,zero);
    //calculando o numero de bytes inteiros que devem ser deslocados 
    int numBytes = (int)n/8;
    //calculando o numero de bits que devem ser deslocados além dos bytes inteiros
    int numBits = n-numBytes*8;

    //criando dois bytes auxiliares, um preenchido todo de 0s e outro de 1s
    unsigned char byteAux = 0;
    unsigned char secondByteAux = 0xFF;

    //loop para por preencher o byteAux com "n" 1s, "n" sendo o numBits
    for (int i = 0; i<numBits; i++){
        byteAux *= 2;
        byteAux += 1;
    }
    
    //fazendo secondByteAux ser preenchido 8-"n" 1s, onde n = numBits
    //se n for 3 secondByteAux será 0001 1111 e ByteAux será 0000 0111
    secondByteAux = secondByteAux<<(8-numBits);
    secondByteAux = ~secondByteAux;
    
    //bitsPerdidos é uma variável para guardar o valor que foi retirado do byte "n" pelo shift e deve ser acrescida no byte "n-1", na devida posição
    unsigned char bitsPerdidos = 0;

    //bitsAux é uma variável para guardar com o mesmo propósito da de cima, mas sem alterar a posição e ele guarda temporiariamente, para que bitsPerdidos permaneça com o valor correto 
    //e só receba o novo valor no momento certo
    unsigned char bitsAux;
    
    //percorrendo o bigint a de trás para frente começando no byte 15 (décimo sexto byte) e terminando no byte "n" onde n é numBytes
    for (int i = NUM_BITS/8-1; i>=numBytes; i--){
        //byte n-numBytes do aux vai ser igual ao byte n do bigint a
        *(aux+i-numBytes) = *(a+i);
        
        //caso tenha bits quebrados para ser deslocado entra nesse if
        if (numBits > 0){
            //bitsAux vai ser igual aos bits que vão ser perdidos no shift
            bitsAux = ((byteAux) & (*(aux+i-numBytes)));
            
            //fazendo o shift do byte
            *(aux+i-numBytes) = *(aux+i-numBytes)>>numBits;
            
            //certificando que após o shift os bits que vão ser acrecidos no byte serão 0, independente do sinal
            *(aux+i-numBytes) = (*(aux+i-numBytes))&(secondByteAux);

            //somando no byte os bitsPerdidos
            *(aux+i-numBytes) += bitsPerdidos;

            //fazendo o bitPerdidos ser igual ao bitsAux e deslocando eles para a posição certa para serem acrescidos ao byte seguinte do loop
            bitsPerdidos = bitsAux;
            bitsPerdidos = bitsPerdidos<<(8-numBits);
        }
    }

    //copiando o valor de aux para res
    copyBigInt(res,aux);
}

void big_sar(BigInt res, BigInt a, int n){
    //basicamente o mesmo código do big_shr, mas considerando o sinal

    //variavel para descobrir se o número é negativo ou nâo
    int isNeg = (*(a+15))&(0x80);
    BigInt aux;
    long zero = 0;
    big_val(aux,zero);
    int numBytes = (int)n/8;
    int numBits = n-numBytes*8;

    //fazendo aux ser todo preenchido de 1 caso o bigint a seja negativo
    if (isNeg){
        for (int i = 0; i<NUM_BITS/8; i++){
            *(aux+i) = 0xFF;
        }
    }

    unsigned char byteAux = 0;
    unsigned char secondByteAux = 0xFF;
    for (int i = 0; i<numBits; i++){
        byteAux *= 2;
        byteAux += 1;
    }
    secondByteAux = secondByteAux<<(8-numBits);
    secondByteAux = ~secondByteAux;

    unsigned char bitsPerdidos = 0;
    unsigned char bitsAux;

    for (int i = NUM_BITS/8-1; i>=numBytes; i--){
        *(aux+i-numBytes) = *(a+i);
        if (numBits > 0){
            bitsAux = ((byteAux) & (*(aux+i-numBytes)));
            *(aux+i-numBytes) = *(aux+i-numBytes)>>numBits;
            *(aux+i-numBytes) = (*(aux+i-numBytes))&(secondByteAux);
            *(aux+i-numBytes) += bitsPerdidos;
            bitsPerdidos = bitsAux;
            bitsPerdidos = bitsPerdidos<<(8-numBits);
        }
    }

    if(isNeg){
        //transformando os bits acrescentado após o shift em 1 caso o número seja negativo

        secondByteAux = ~secondByteAux;
        *(aux+15) = (*(aux+15))|secondByteAux;   
    }

    copyBigInt(res,aux);
}
