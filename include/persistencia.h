#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include "avaliacao.h"
#include "curso.h"
#include "forum.h"
#include "usuario.h"

/*
 * Sistema agrega as quatro estruturas principais. Os arquivos .txt ficam
 * na pasta "data" e guardam apenas IDs e textos, nunca enderecos de memoria.
 */
typedef struct {
    ListaUsuarios usuarios;
    ListaCursos cursos;
    ListaAvaliacoes avaliacoes;
    ListaTopicos topicos;
} Sistema;

void iniciar_sistema(Sistema *sistema);
void liberar_sistema(Sistema *sistema);

/* Le os arquivos na ordem hierarquica e reconstrui a memoria RAM. */
int carregar_sistema(Sistema *sistema, const char *pasta);

/* Exclusao em cascata: remove tambem notas, topicos e referencias de aulas. */
int excluir_curso_cascata(Sistema *sistema, int id_curso);
int excluir_usuario_cascata(Sistema *sistema, int id_usuario);
int excluir_aula_cascata(Sistema *sistema, int id_aula);
int excluir_modulo_cascata(Sistema *sistema, int id_modulo);

/* Salva novamente todas as estruturas em arquivos de texto. */
int salvar_sistema(const Sistema *sistema, const char *pasta);

/* RF04: cria um caderno em Markdown com as anotacoes do aluno. */
int exportar_caderno(const Sistema *sistema, int id_aluno,
                     const char *arquivo);

#endif
