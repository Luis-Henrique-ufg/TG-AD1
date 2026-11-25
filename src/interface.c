#include "interface.h"
#include "cursos.h"
#include "aulas.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================================
 * IMPLEMENTACAO: INTERFACE DE TERMINAL (Responsavel: Estudante 4)
 * Consulte docs/GUIA_ESTUDANTE_4.md e include/interface.h antes de programar.
 * ========================================================================== */

void ler_texto(const char *prompt, char *buffer, int tamanho) {
    /* TODO (Estudante 4): imprimir prompt, ler com fgets, remover '\n' e
     * limpar o restante do stdin caso a linha seja maior que 'tamanho'. */
    (void)prompt;
    if (buffer != NULL && tamanho > 0) {
        buffer[0] = '\0';
    }
}

int ler_inteiro(const char *prompt, int min, int max) {
    /* TODO (Estudante 4): usar ler_texto + strtol em loop ate o usuario
     * digitar um numero valido entre min e max. */
    (void)prompt;
    (void)max;
    return min;
}

void mostrar_barra_progresso(double progresso) {
    /* TODO (Estudante 4): imprimir barra ASCII de 10 blocos, ex: [#######---]. */
    (void)progresso;
}

void exibir_dashboard(const Plataforma *plat) {
    /* TODO (Estudante 4): imprimir a Tela 1 (lista de cursos #1..#N com
     * barra de progresso e o menu [1]..[6], [0] Sair). */
    (void)plat;
}

void exibir_curso(const Curso *curso) {
    /* TODO (Estudante 4): imprimir a Tela 2 (progresso do curso, lista de aulas
     * com [x]/[ ], duracao, anotacao e menu [1]..[4], [0] Voltar). */
    (void)curso;
}

void carregar_dados_exemplo(Plataforma *plat) {
    /* TODO (Estudante 4): cadastrar 3 cursos e algumas aulas prontas usando
     * curso_criar(), aula_criar() e aula_alternar_concluida(). */
    (void)plat;
}
