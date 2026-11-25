#ifndef ORDENACAO_H
#define ORDENACAO_H

#include "models.h"

/* ============================================================================
 * MODULO: ORDENACAO E BUSCA (Responsavel: Estudante 3)
 * ============================================================================
 * Ordena as listas encadeadas de cursos e aulas (usando o algoritmo escolhido
 * pelo grupo, como Bubble Sort ou Selection Sort em lista simples) e busca
 * cursos pelo nome sem diferenciar maiusculas/minusculas.
 * ========================================================================== */

/**
 * Ordena a lista plat->inicio em ordem alfabetica de nome (A-Z),
 * sem diferenciar maiusculas de minusculas.
 */
void ordenar_cursos_por_nome(Plataforma *plat);

/**
 * Ordena a lista plat->inicio do MAIOR progresso (%) para o MENOR.
 * Em caso de empate, pode manter a ordem ou desempatar por nome (A-Z).
 */
void ordenar_cursos_por_progresso(Plataforma *plat);

/**
 * Ordena a lista curso->aulas da MENOR duracao (minutos) para a MAIOR.
 */
void ordenar_aulas_por_duracao(Curso *curso);

/**
 * Busca o primeiro curso cujo nome contenha 'termo' (ignorando maiusculas/minusculas).
 * Retorna o ponteiro para o curso encontrado ou NULL se nao encontrar.
 */
Curso *buscar_curso_por_nome(const Plataforma *plat, const char *termo);

#endif /* ORDENACAO_H */
