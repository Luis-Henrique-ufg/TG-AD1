#ifndef FORUM_H
#define FORUM_H

#define TAM_TITULO_TOPICO 100
#define TAM_PERGUNTA 300
#define TAM_RESPOSTA 300

/* Sublista de respostas de um topico. */
typedef struct NoResposta {
    int id;
    int id_autor;
    char texto[TAM_RESPOSTA];
    struct NoResposta *proximo;
} NoResposta;

typedef struct {
    NoResposta *inicio;
    NoResposta *fim;
    int tamanho;
} ListaRespostas;

/* Topico principal do forum. */
typedef struct NoTopico {
    int id;
    int id_curso;
    int id_autor;
    char titulo[TAM_TITULO_TOPICO];
    char pergunta[TAM_PERGUNTA];
    ListaRespostas respostas;
    struct NoTopico *proximo;
} NoTopico;

typedef struct {
    NoTopico *inicio;
    NoTopico *fim;
    int tamanho;
} ListaTopicos;

void iniciar_topicos(ListaTopicos *lista);
void liberar_topicos(ListaTopicos *lista);
NoTopico *inserir_topico(ListaTopicos *lista, int id, int id_curso,
                         int id_autor, const char *titulo,
                         const char *pergunta);
NoTopico *buscar_topico_por_id(ListaTopicos *lista, int id);
int excluir_topico(ListaTopicos *lista, int id);
int remover_topicos_por_curso(ListaTopicos *lista, int id_curso);
int remover_topicos_por_autor(ListaTopicos *lista, int id_autor);
int remover_respostas_por_autor(ListaTopicos *lista, int id_autor);
int gerar_id_topico(const ListaTopicos *lista);

NoResposta *inserir_resposta(NoTopico *topico, int id, int id_autor,
                             const char *texto);
int gerar_id_resposta(const ListaTopicos *lista);

#endif
