#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

/* Tamanho minimo necessario para guardar "HH:MM:SS" com o caractere final. */
#define TAM_MAX_HORARIO 12

/*
 * Le uma linha do teclado de forma segura.
 * Remove o '\n' final e troca o caractere '|' por '-', pois '|' e o
 * delimitador usado nos arquivos de persistencia.
 * Retorna 1 quando leu algo e 0 quando encontrou fim de arquivo/entrada invalida.
 */
int ler_texto(const char *mensagem, char *destino, size_t tamanho);

/* Le um inteiro usando ler_texto; retorna 1 em caso de sucesso. */
int ler_inteiro(const char *mensagem, int *destino);

/* Le um numero real usando ler_texto; retorna 1 em caso de sucesso. */
int ler_decimal(const char *mensagem, float *destino);

/* Valida uma string no formato HH:MM:SS (minutos e segundos de 0 a 59). */
int validar_horario(const char *texto);

/* Converte "HH:MM:SS" para a quantidade total de segundos. */
int horario_para_segundos(const char *texto);

/* Converte uma quantidade de segundos para "HH:MM:SS". */
void segundos_para_horario(int segundos, char *destino, size_t tamanho);

/* Copia texto garantindo o caractere final '\0' (usado pelas TADs). */
void copiar_texto_seguro(char *destino, size_t tamanho, const char *origem);

/* Repete a leitura ate o usuario digitar um horario valido. */
int ler_horario(const char *mensagem, char *destino, size_t tamanho);

#endif
