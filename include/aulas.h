#ifndef AULAS_H
#define AULAS_H

#include "models.h"

/* ============================================================================
 * MODULO: AULAS E PROGRESSO (Responsavel: Estudante 2)
 * ============================================================================
 * Gerencia a lista simplesmente encadeada de aulas dentro de cada Curso,
 * o status de conclusao e o calculo de progresso (%).
 * ========================================================================== */

/**
 * Aloca uma nova Aula e a insere no FINAL da lista curso->aulas.
 * - Requer curso != NULL, titulo != NULL/nao vazio e duracao_min >= 0.
 * - Se anotacao == NULL, armazena "".
 * - Define concluida = false e next = NULL.
 * - Incrementa curso->total_aulas.
 * Retorna o ponteiro para a aula criada ou NULL em caso de erro.
 */
Aula *aula_criar(Curso *curso, const char *titulo, int duracao_min,
                 const char *anotacao);

/**
 * Busca uma aula pela posicao na lista do curso (base 1: 1 .. curso->total_aulas).
 * Retorna NULL se indice estiver fora do intervalo.
 */
Aula *aula_buscar_por_indice(const Curso *curso, int indice);

/**
 * Remove a aula na posicao 'indice' (base 1: 1 .. curso->total_aulas).
 * Desvincula o no da lista, da free(aula) e decrementa curso->total_aulas.
 * Retorna true se removeu, false se indice for invalido.
 */
bool aula_remover(Curso *curso, int indice);

/**
 * Inverte o estado da aula: concluida <-> em aberto.
 * Retorna true se aula != NULL, false caso contrario.
 */
bool aula_alternar_concluida(Aula *aula);

/**
 * Calcula o progresso do curso de 0.0 ate 100.0.
 * Retorna 0.0 se curso == NULL ou curso->total_aulas == 0 (evita divisao por zero).
 */
double aula_calcular_progresso(const Curso *curso);

/**
 * Libera todas as aulas de um curso, deixando curso->aulas = NULL
 * e curso->total_aulas = 0.
 */
void aula_liberar_todas(Curso *curso);

#endif /* AULAS_H */
