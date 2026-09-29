/*
 * Validador de CPF em C
 * Verifica os dois digitos verificadores de um CPF.
 * Use apenas CPFs ficticios nos testes (ex.: 529.982.247-25).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAM_BUFFER 64
#define TAM_CPF 11

/* Calcula um digito verificador a partir dos n primeiros digitos.
 * Os pesos vao de (n + 1) ate 2. */
static int calcular_digito(const int *digitos, int n) {
    int soma = 0;
    for (int i = 0; i < n; i++) {
        soma += digitos[i] * (n + 1 - i);
    }
    int resto = soma % 11;
    return (resto < 2) ? 0 : 11 - resto;
}

/* Copia para 'digitos' os numeros encontrados na entrada.
 * Aceita pontos, hifen e espacos. Retorna a quantidade de digitos
 * encontrados, ou -1 se houver algum caractere invalido. */
static int extrair_digitos(const char *entrada, int *digitos) {
    int total = 0;
    for (const char *p = entrada; *p != '\0'; p++) {
        if (isdigit((unsigned char)*p)) {
            if (total < TAM_CPF) {
                digitos[total] = *p - '0';
            }
            total++;
        } else if (!strchr(". -\t\r\n", *p)) {
            return -1;
        }
    }
    return total;
}

/* Retorna 1 se todos os digitos forem iguais (ex.: 111.111.111-11). */
static int todos_iguais(const int *digitos) {
    for (int i = 1; i < TAM_CPF; i++) {
        if (digitos[i] != digitos[0]) return 0;
    }
    return 1;
}

static int cpf_valido(const int *digitos) {
    if (todos_iguais(digitos)) return 0;
    int d1 = calcular_digito(digitos, 9);
    int d2 = calcular_digito(digitos, 10);
    return (d1 == digitos[9] && d2 == digitos[10]);
}

int main(void) {
    char *linha = malloc(TAM_BUFFER);
    int *digitos = malloc(TAM_CPF * sizeof(int));

    if (linha == NULL || digitos == NULL) {
        fprintf(stderr, "Erro: memoria insuficiente.\n");
        free(linha);
        free(digitos);
        return 1;
    }

    printf("Validador de CPF (digite 'sair' para encerrar)\n");

    while (1) {
        printf("\nCPF: ");
        if (fgets(linha, TAM_BUFFER, stdin) == NULL) break;

        linha[strcspn(linha, "\n")] = '\0';
        if (strcmp(linha, "sair") == 0) break;

        int total = extrair_digitos(linha, digitos);
        if (total < 0) {
            printf("Entrada invalida: use apenas numeros, pontos e hifen.\n");
        } else if (total != TAM_CPF) {
            printf("Entrada invalida: o CPF deve ter %d digitos (foram informados %d).\n",
                   TAM_CPF, total);
        } else if (cpf_valido(digitos)) {
            printf("CPF valido.\n");
        } else {
            printf("CPF invalido.\n");
        }
    }

    free(linha);
    free(digitos);
    return 0;
}
