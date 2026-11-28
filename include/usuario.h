#ifndef USUARIO_H
#define USUARIO_H

#define TAM_NOME 80
#define TAM_EMAIL 80
#define TAM_SENHA 40
#define MAX_HISTORICO 5

#define PERFIL_DOCENTE 1
#define PERFIL_DISCENTE 2

/*
 * Sublista simples: aulas concluidas por um discente.
 * Guardamos somente o ID da aula, pois a aula em si esta em curso.h.
 */
typedef struct NoAulaConcluida {
    int id_aula;
    struct NoAulaConcluida *proximo;
} NoAulaConcluida;

typedef struct {
    NoAulaConcluida *inicio;
    NoAulaConcluida *fim;
    int tamanho;
} ListaConcluidas;

/* Lista duplamente encadeada: historico com no maximo 5 aulas. */
typedef struct NoHistorico {
    int id_aula;
    struct NoHistorico *anterior;
    struct NoHistorico *proximo;
} NoHistorico;

typedef struct {
    NoHistorico *inicio;
    NoHistorico *fim;
    int tamanho;
} ListaHistorico;

/* Cadastro de uma pessoa no sistema. */
typedef struct {
    int id;
    char nome[TAM_NOME];
    char email[TAM_EMAIL];
    char senha[TAM_SENHA];
    int perfil;
    ListaConcluidas concluidas;
    ListaHistorico historico;
} Usuario;

typedef struct NoUsuario {
    Usuario dados;
    struct NoUsuario *proximo;
} NoUsuario;

typedef struct {
    NoUsuario *inicio;
    NoUsuario *fim;
    int tamanho;
} ListaUsuarios;

/* ---- Lista principal de usuarios ---- */
void iniciar_lista_usuarios(ListaUsuarios *lista);
void liberar_lista_usuarios(ListaUsuarios *lista);
NoUsuario *inserir_usuario(ListaUsuarios *lista, int id, const char *nome,
                           const char *email, const char *senha, int perfil);
NoUsuario *buscar_usuario_por_id(ListaUsuarios *lista, int id);
NoUsuario *buscar_usuario_por_email(ListaUsuarios *lista, const char *email);
NoUsuario *autenticar_usuario(ListaUsuarios *lista, const char *email,
                              const char *senha);
int excluir_usuario(ListaUsuarios *lista, int id);
int gerar_id_usuario(const ListaUsuarios *lista);

/* ---- Sublista de aulas concluidas ---- */
void iniciar_concluidas(ListaConcluidas *lista);
int marcar_aula_concluida(ListaConcluidas *lista, int id_aula);
int aula_esta_concluida(const ListaConcluidas *lista, int id_aula);
int remover_aula_concluida(ListaConcluidas *lista, int id_aula);
void liberar_concluidas(ListaConcluidas *lista);

/* ---- Historico recente (lista dupla) ---- */
void iniciar_historico(ListaHistorico *historico);
void registrar_historico(ListaHistorico *historico, int id_aula);
int remover_aula_historico(ListaHistorico *historico, int id_aula);
void liberar_historico(ListaHistorico *historico);

#endif
