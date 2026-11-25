#include "aulas.h"

#include <stdlib.h>
#include <string.h>

/* ============================================================================
 * IMPLEMENTACAO: AULAS E PROGRESSO (Responsavel: Estudante 2)
 * Consulte docs/GUIA_ESTUDANTE_2.md e include/aulas.h antes de programar.
 * ========================================================================== */

Aula *aula_criar(Curso *curso, const char *titulo, int duracao_min,
                 const char *anotacao) {
    /* TODO (Estudante 2):
     * 1. Validar curso != NULL, titulo != NULL/nao vazio e duracao_min >= 0.
     * 2. Alocar Aula com malloc(sizeof(Aula)) e checar se != NULL.
     * 3. Copiar titulo e anotacao ("" se NULL), definir duracao_min, concluida = false, next = NULL.
     * 4. Inserir no FINAL de curso->aulas e incrementar curso->total_aulas. */
    (void)curso;
    (void)titulo;
    (void)duracao_min;
    (void)anotacao;
    return NULL;
}

Aula *aula_buscar_por_indice(const Curso *curso, int indice) {
    /* TODO (Estudante 2):
     * Percorrer curso->aulas ate a posicao 'indice' (comeca em 1).
     * Retornar NULL se indice < 1 ou indice > curso->total_aulas. */
    (void)curso;
    (void)indice;
    return NULL;
}

bool aula_remover(Curso *curso, int indice) {
    /* TODO (Estudante 2):
     * Encontrar o no na posicao 'indice' (1-based) guardando o ponteiro 'anterior',
     * desvincular da lista, dar free(atual) e decrementar curso->total_aulas. */
    (void)curso;
    (void)indice;
    return false;
}

bool aula_alternar_concluida(Aula *aula) {
    /* TODO (Estudante 2) - [FUNCIONALIDADE MAIS FACIL DO PROJETO]:
     * Se aula == NULL retornar false; senao aula->concluida = !aula->concluida e retornar true. */
    (void)aula;
    return false;
}

double aula_calcular_progresso(const Curso *curso) {
    /* TODO (Estudante 2):
     * Se curso == NULL ou curso->total_aulas == 0 retornar 0.0.
     * Contar quantas aulas tem concluida == true e retornar (100.0 * concluidas) / curso->total_aulas. */
    (void)curso;
    return 0.0;
}

void aula_liberar_todas(Curso *curso) {
    /* TODO (Estudante 2):
     * Percorrer curso->aulas dando free em cada no (salvando o proximo antes do free!).
     * Ao final: curso->aulas = NULL e curso->total_aulas = 0. */
    (void)curso;
}
