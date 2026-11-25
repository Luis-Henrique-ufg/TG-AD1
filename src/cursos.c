#include "cursos.h"
#include "aulas.h"

#include <stdlib.h>
#include <string.h>

/* ============================================================================
 * IMPLEMENTACAO: CURSOS (Responsavel: Estudante 1)
 * Consulte docs/GUIA_ESTUDANTE_1.md e include/cursos.h antes de programar.
 * ========================================================================== */

void plataforma_inicializar(Plataforma *plat) {
    /* TODO (Estudante 1): definir plat->inicio = NULL e plat->total_cursos = 0. */
    (void)plat;
}

Curso *curso_criar(Plataforma *plat, const char *nome) {
    /* TODO (Estudante 1):
     * 1. Validar se plat != NULL e nome != NULL e nome[0] != '\0'.
     * 2. Alocar Curso com malloc(sizeof(Curso)) e checar se != NULL.
     * 3. Copiar nome com seguranca, definir aulas = NULL, total_aulas = 0, next = NULL.
     * 4. Inserir no FINAL da lista plat->inicio e incrementar plat->total_cursos. */
    (void)plat;
    (void)nome;
    return NULL;
}

Curso *curso_buscar_por_indice(const Plataforma *plat, int indice) {
    /* TODO (Estudante 1):
     * Percorrer plat->inicio ate a posicao 'indice' (comeca em 1).
     * Retornar NULL se indice < 1 ou indice > plat->total_cursos. */
    (void)plat;
    (void)indice;
    return NULL;
}

bool curso_remover(Plataforma *plat, int indice) {
    /* TODO (Estudante 1):
     * 1. Validar indice (1 .. plat->total_cursos).
     * 2. Encontrar o no 'atual' e seu 'anterior' na lista.
     * 3. Chamar aula_liberar_todas(atual).
     * 4. Desvincular 'atual' da lista, chamar free(atual) e decrementar total_cursos. */
    (void)plat;
    (void)indice;
    return false;
}

void plataforma_liberar(Plataforma *plat) {
    /* TODO (Estudante 1):
     * Percorrer todos os cursos: chamar aula_liberar_todas(curso) e dar free(curso).
     * Ao final: plat->inicio = NULL e plat->total_cursos = 0. */
    (void)plat;
}
