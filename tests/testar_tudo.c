#include "models.h"
#include "cursos.h"
#include "aulas.h"
#include "ordenacao.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

/* ============================================================================
 * TESTES AUTOMATICOS SIMPLES — EAD-CLI (Estudantes 1, 2 e 3)
 * ============================================================================
 * Rode este arquivo depois de implementar os modulos para confirmar que
 * listas encadeadas, progresso, ordenacao e busca funcionam sem bugs.
 * ========================================================================== */

int main(void) {
    Plataforma plat;
    plataforma_inicializar(&plat);
    assert(plat.inicio == NULL);
    assert(plat.total_cursos == 0);

    /* --- Bloco 1: Estudante 1 (Cursos) --- */
    assert(curso_criar(&plat, "") == NULL);
    Curso *c1 = curso_criar(&plat, "Estruturas de Dados em C");
    Curso *c2 = curso_criar(&plat, "Banco de Dados SQL");
    Curso *c3 = curso_criar(&plat, "Algoritmos Basicos");
    assert(c1 != NULL && c2 != NULL && c3 != NULL);
    assert(plat.total_cursos == 3);
    assert(curso_buscar_por_indice(&plat, 1) == c1);
    assert(curso_buscar_por_indice(&plat, 3) == c3);
    assert(curso_buscar_por_indice(&plat, 0) == NULL);

    /* --- Bloco 2: Estudante 2 (Aulas, Concluir e Progresso) --- */
    assert(aula_calcular_progresso(c1) == 0.0);
    Aula *a1 = aula_criar(c1, "Listas Encadeadas", 30, "Usar ponteiro next");
    Aula *a2 = aula_criar(c1, "Ponteiros", 15, NULL);
    Aula *a3 = aula_criar(c1, "Structs", 20, "Agrupar campos");
    assert(a1 != NULL && a2 != NULL && a3 != NULL);
    assert(c1->total_aulas == 3);
    assert(strcmp(a2->anotacao, "") == 0);

    /* Alternar estado de conclusao da aula */
    assert(aula_alternar_concluida(a1));
    assert(a1->concluida == true);
    assert(aula_alternar_concluida(a1));
    assert(a1->concluida == false);

    assert(aula_alternar_concluida(a1));
    assert(aula_alternar_concluida(a2));
    /* c1 tem 2/3 concluida (~66.6%), c2 tem 1/1 (100%) */
    Aula *b1 = aula_criar(c2, "SELECT e WHERE", 25, "");
    assert(b1 != NULL);
    assert(aula_alternar_concluida(b1));
    assert(aula_calcular_progresso(c2) > 99.9);

    /* --- Bloco 3: Estudante 3 (Ordenacao e Busca) --- */
    ordenar_aulas_por_duracao(c1);
    assert(c1->aulas->duracao_min == 15);
    assert(c1->aulas->next->duracao_min == 20);
    assert(c1->aulas->next->next->duracao_min == 30);

    ordenar_cursos_por_nome(&plat);
    assert(strcmp(plat.inicio->nome, "Algoritmos Basicos") == 0);

    ordenar_cursos_por_progresso(&plat);
    assert(strcmp(plat.inicio->nome, "Banco de Dados SQL") == 0);

    Curso *achado = buscar_curso_por_nome(&plat, "DADOS");
    assert(achado != NULL);
    assert(buscar_curso_por_nome(&plat, "Python") == NULL);

    /* --- Remocao e Liberacao de Memoria --- */
    assert(aula_remover(c1, 2));
    assert(c1->total_aulas == 2);
    assert(curso_remover(&plat, 1));
    assert(plat.total_cursos == 2);

    plataforma_liberar(&plat);
    assert(plat.inicio == NULL);
    assert(plat.total_cursos == 0);

    printf("[OK] Todos os testes passaram!\n");
    return 0;
}
