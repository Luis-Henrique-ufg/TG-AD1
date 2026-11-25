#include "models.h"
#include "cursos.h"
#include "aulas.h"
#include "ordenacao.h"
#include "interface.h"

#include <stdio.h>

/* ============================================================================
 * MENU PRINCIPAL E INTEGRACAO (Responsavel: Estudante 4)
 * Consulte docs/GUIA_ESTUDANTE_4.md antes de programar.
 * ========================================================================== */

int main(void) {
    Plataforma plat;

    plataforma_inicializar(&plat);
    carregar_dados_exemplo(&plat);

    /* TODO (Estudante 4): implementar o loop do Dashboard (opcoes 0..6) e a
     * funcao auxiliar menu_curso(Curso *curso) (opcoes 0..4) chamando os
     * modulos dos Estudantes 1, 2 e 3. */
    printf("EAD-CLI (versao enxuta para 4 estudantes) pronto para implementacao!\n");
    printf("Consulte README.md e o seu guia em docs/GUIA_ESTUDANTE_X.md.\n");

    plataforma_liberar(&plat);
    return 0;
}
