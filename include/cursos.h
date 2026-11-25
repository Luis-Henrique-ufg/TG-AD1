#ifndef CURSOS_H
#define CURSOS_H

#include "models.h"

/* ============================================================================
 * MODULO: CURSOS (Responsavel: Estudante 1)
 * ============================================================================
 * Gerencia a lista simplesmente encadeada de cursos na Plataforma
 * e a liberacao total de memoria no encerramento do programa.
 * ========================================================================== */

/**
 * Inicializa a plataforma vazia: inicio = NULL e total_cursos = 0.
 */
void plataforma_inicializar(Plataforma *plat);

/**
 * Aloca um novo Curso e o insere no FINAL da lista plat->inicio.
 * - Requer plat != NULL e nome != NULL e nome nao vazio (nome[0] != '\0').
 * - Inicializa aulas = NULL, total_aulas = 0, next = NULL.
 * - Incrementa plat->total_cursos.
 * Retorna o ponteiro para o curso criado ou NULL em caso de erro.
 */
Curso *curso_criar(Plataforma *plat, const char *nome);

/**
 * Busca um curso pela sua posicao na lista (base 1: 1 .. plat->total_cursos).
 * Retorna NULL se indice estiver fora do intervalo.
 */
Curso *curso_buscar_por_indice(const Plataforma *plat, int indice);

/**
 * Remove o curso na posicao 'indice' (base 1: 1 .. plat->total_cursos).
 * - Libera todas as aulas desse curso chamando aula_liberar_todas().
 * - Desvincula o no da lista, da free(curso) e decrementa plat->total_cursos.
 * Retorna true se removeu, false se indice for invalido.
 */
bool curso_remover(Plataforma *plat, int indice);

/**
 * Libera TODA a memoria da plataforma (todas as aulas de cada curso e todos os cursos).
 * Ao final, deixa plat->inicio = NULL e plat->total_cursos = 0.
 */
void plataforma_liberar(Plataforma *plat);

#endif /* CURSOS_H */
