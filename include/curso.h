#ifndef CURSO_H
#define CURSO_H

#include "anotacao.h"
#include "usuario.h"

#define TAM_TITULO 100
#define TAM_DESCRICAO 200

/* Nivel 3: aula. Guarda suas proprias anotacoes ordenadas. */
typedef struct NoAula {
    int id;
    char titulo[TAM_TITULO];
    int duracao_segundos;
    int eh_aula_valida;
    ListaAnotacoes anotacoes;
    struct NoAula *proximo;
} NoAula;

typedef struct {
    NoAula *inicio;
    NoAula *fim;
    int tamanho;
} ListaAulas;

/* Nivel 2: modulo. */
typedef struct NoModulo {
    int id;
    char titulo[TAM_TITULO];
    ListaAulas aulas;
    struct NoModulo *proximo;
} NoModulo;

typedef struct {
    NoModulo *inicio;
    NoModulo *fim;
    int tamanho;
} ListaModulos;

/* Nivel 1: curso. */
typedef struct NoCurso {
    int id;
    char titulo[TAM_TITULO];
    char descricao[TAM_DESCRICAO];
    ListaModulos modulos;
    struct NoCurso *proximo;
} NoCurso;

typedef struct {
    NoCurso *inicio;
    NoCurso *fim;
    int tamanho;
} ListaCursos;

/* ---- Cursos ---- */
void iniciar_cursos(ListaCursos *cursos);
void liberar_cursos(ListaCursos *cursos);
NoCurso *inserir_curso(ListaCursos *cursos, int id, const char *titulo,
                       const char *descricao);
NoCurso *buscar_curso_por_id(ListaCursos *cursos, int id);
int editar_curso(NoCurso *curso, const char *titulo, const char *descricao);
int excluir_curso(ListaCursos *cursos, int id);
int remover_referencias_a_curso(ListaCursos *cursos, int id_curso,
                                int (*remover)(void *, int), void *alvo);
int remover_referencias_a_aula(ListaCursos *cursos, int id_aula,
                               int (*remover)(void *, int), void *alvo);
int remover_anotacoes_de_aluno(ListaCursos *cursos, int id_aluno);
NoCurso *buscar_curso_do_modulo_wrapper(ListaCursos *cursos, int id_modulo);
NoModulo *buscar_modulo_da_aula_wrapper(ListaCursos *cursos, int id_aula);
int gerar_id_curso(const ListaCursos *cursos);

/* ---- Modulos ---- */
NoModulo *inserir_modulo(NoCurso *curso, int id, const char *titulo);
NoModulo *buscar_modulo_por_id(ListaCursos *cursos, int id);
NoModulo *buscar_modulo_no_curso(NoCurso *curso, int id);
int editar_modulo(NoModulo *modulo, const char *titulo);
int excluir_modulo(NoCurso *curso, int id);
int gerar_id_modulo(const ListaCursos *cursos);

/* ---- Aulas ---- */
NoAula *inserir_aula(NoModulo *modulo, int id, const char *titulo,
                     int duracao_segundos, int eh_aula_valida);
NoAula *buscar_aula_por_id(ListaCursos *cursos, int id);
NoAula *buscar_aula_no_modulo(NoModulo *modulo, int id);
int editar_aula(NoAula *aula, const char *titulo, int duracao_segundos,
                int eh_aula_valida);
int excluir_aula(NoModulo *modulo, int id);
int gerar_id_aula(const ListaCursos *cursos);

/* ---- RF03: progresso contando somente aulas validas ---- */
void calcular_progresso_modulo(const NoModulo *modulo,
                               const ListaConcluidas *concluidas,
                               int *total, int *feitas);
void calcular_progresso_curso(const ListaCursos *cursos,
                              const ListaConcluidas *concluidas,
                              int *total, int *feitas);

#endif
