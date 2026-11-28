#include "utils.h"

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

/*
 * Toda leitura de terminal passa por aqui. Usamos fgets para nunca estourar
 * o vetor de destino e substituimos '|' por '-' para nao quebrar o arquivo
 * de persistencia, que usa '|' como delimitador.
 */
int ler_texto(const char *mensagem, char *destino, size_t tamanho)
{
    int limite;
    int i;

    if (destino == NULL || tamanho == 0) {
        return 0;
    }
    if (mensagem != NULL) {
        printf("%s", mensagem);
    }

    limite = (tamanho > (size_t)INT_MAX) ? INT_MAX : (int)tamanho;
    if (fgets(destino, limite, stdin) == NULL) {
        destino[0] = '\0';
        return 0;
    }

    destino[strcspn(destino, "\r\n")] = '\0';
    if (strchr(destino, '\n') == NULL && strchr(destino, '\r') == NULL &&
        (int)strlen(destino) == limite - 1) {
        int caractere;
        while ((caractere = getchar()) != '\n' && caractere != EOF) {
            /* Descarta o restante de uma linha maior que o buffer. */
        }
    }
    for (i = 0; destino[i] != '\0'; i++) {
        if (destino[i] == '|') {
            destino[i] = '-';
        }
    }
    return 1;
}

int ler_inteiro(const char *mensagem, int *destino)
{
    char linha[64];

    if (destino == NULL) {
        return 0;
    }
    if (!ler_texto(mensagem, linha, sizeof(linha))) {
        return 0;
    }
    if (sscanf(linha, "%d", destino) != 1) {
        return 0;
    }
    return 1;
}

int ler_decimal(const char *mensagem, float *destino)
{
    char linha[64];

    if (destino == NULL) {
        return 0;
    }
    if (!ler_texto(mensagem, linha, sizeof(linha))) {
        return 0;
    }
    if (sscanf(linha, "%f", destino) != 1) {
        return 0;
    }
    return 1;
}

/* Valida os caracteres e os intervalos de minutos e segundos. */
int validar_horario(const char *texto)
{
    int horas = 0;
    int minutos = 0;
    int segundos = 0;
    size_t i;

    if (texto == NULL || strlen(texto) != 8) {
        return 0;
    }
    if (texto[2] != ':' || texto[5] != ':') {
        return 0;
    }
    for (i = 0; i < 8; i++) {
        if (i == 2 || i == 5) {
            continue;
        }
        if (!isdigit((unsigned char)texto[i])) {
            return 0;
        }
    }
    if (sscanf(texto, "%2d:%2d:%2d", &horas, &minutos, &segundos) != 3) {
        return 0;
    }
    if (horas < 0 || horas > 99 || minutos < 0 || minutos > 59 ||
        segundos < 0 || segundos > 59) {
        return 0;
    }
    return 1;
}

int horario_para_segundos(const char *texto)
{
    int horas = 0;
    int minutos = 0;
    int segundos = 0;

    if (!validar_horario(texto)) {
        return 0;
    }
    sscanf(texto, "%2d:%2d:%2d", &horas, &minutos, &segundos);
    return horas * 3600 + minutos * 60 + segundos;
}

void segundos_para_horario(int segundos, char *destino, size_t tamanho)
{
    int horas;
    int minutos;

    if (destino == NULL || tamanho == 0) {
        return;
    }
    if (segundos < 0) {
        segundos = 0;
    }
    horas = segundos / 3600;
    minutos = (segundos % 3600) / 60;
    segundos = segundos % 60;
    snprintf(destino, tamanho, "%02d:%02d:%02d", horas, minutos, segundos);
}

void copiar_texto_seguro(char *destino, size_t tamanho, const char *origem)
{
    if (destino == NULL || tamanho == 0) {
        return;
    }
    if (origem == NULL) {
        destino[0] = '\0';
        return;
    }
    strncpy(destino, origem, tamanho - 1);
    destino[tamanho - 1] = '\0';
}

int ler_horario(const char *mensagem, char *destino, size_t tamanho)
{
    if (destino == NULL || tamanho < 9) {
        return 0;
    }
    for (;;) {
        if (!ler_texto(mensagem, destino, tamanho)) {
            return 0;
        }
        if (validar_horario(destino)) {
            return 1;
        }
        printf("Horario invalido! Use HH:MM:SS (exemplo: 00:12:30).\n");
    }
}
