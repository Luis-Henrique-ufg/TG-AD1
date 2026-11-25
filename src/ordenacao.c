#include "ordenacao.h"
#include "aulas.h"

#include <ctype.h>
#include <stddef.h>
#include <string.h>

/* ============================================================================
 * IMPLEMENTACAO: ORDENACAO E BUSCA (Responsavel: Estudante 3)
 * Consulte docs/GUIA_ESTUDANTE_3.md e include/ordenacao.h antes de programar.
 * DICA: voce pode usar Bubble Sort ou Selection Sort na lista encadeada!
 * ========================================================================== */

void ordenar_cursos_por_nome(Plataforma *plat) {
    /* TODO (Estudante 3):
     * Ordenar os nos de plat->inicio em ordem alfabetica de nome (A-Z),
     * ignorando maiusculas e minusculas. */
    (void)plat;
}

void ordenar_cursos_por_progresso(Plataforma *plat) {
    /* TODO (Estudante 3):
     * Ordenar plat->inicio do MAIOR para o MENOR resultado de aula_calcular_progresso(). */
    (void)plat;
}

void ordenar_aulas_por_duracao(Curso *curso) {
    /* TODO (Estudante 3):
     * Ordenar curso->aulas da MENOR para a MAIOR duracao_min. */
    (void)curso;
}

Curso *buscar_curso_por_nome(const Plataforma *plat, const char *termo) {
    /* TODO (Estudante 3):
     * Percorrer plat->inicio e retornar o primeiro Curso cujo nome contenha 'termo'
     * (ignorando maiusculas/minusculas), ou NULL se nao encontrar. */
    (void)plat;
    (void)termo;
    return NULL;
}
