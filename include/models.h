#ifndef MODELS_H
#define MODELS_H

#include <stdbool.h>

/* ============================================================================
 * CONTRATO COMPARTILHADO - EAD-CLI (Versao Enxuta para 4 Estudantes)
 * ============================================================================
 * Estrutura: 2 niveis de listas simplesmente encadeadas (apenas *next).
 *   Plataforma -> Curso -> Curso -> NULL
 *                   |
 *                   +-> Aula -> Aula -> NULL
 *
 * REGRA: nenhum integrante altera este arquivo sem avisar os outros 3.
 * ========================================================================== */

#define TAM_TEXTO 100
#define TAM_NOTA  200

/* Uma aula pertence a um Curso (lista simplesmente encadeada) */
typedef struct Aula {
    char titulo[TAM_TEXTO];
    char anotacao[TAM_NOTA];
    int duracao_min;
    bool concluida;
    struct Aula *next;
} Aula;

/* Um curso pertence a Plataforma (lista simplesmente encadeada) */
typedef struct Curso {
    char nome[TAM_TEXTO];
    Aula *aulas;
    int total_aulas;
    struct Curso *next;
} Curso;

/* Estado principal do programa */
typedef struct Plataforma {
    Curso *inicio;
    int total_cursos;
} Plataforma;

#endif /* MODELS_H */
