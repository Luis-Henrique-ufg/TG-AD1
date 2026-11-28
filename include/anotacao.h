#ifndef ANOTACAO_H
#define ANOTACAO_H

#define TAM_TEXTO_ANOTACAO 200

/* Uma anotacao esta sempre ligada a uma aula e a um aluno. */
typedef struct {
    int id;
    int id_aluno;
    int timestamp_segundos;
    char texto[TAM_TEXTO_ANOTACAO];
} Anotacao;

typedef struct NoAnotacao {
    Anotacao dados;
    struct NoAnotacao *proximo;
} NoAnotacao;

typedef struct {
    NoAnotacao *inicio;
    NoAnotacao *fim;
    int tamanho;
} ListaAnotacoes;

void iniciar_anotacoes(ListaAnotacoes *lista);
void liberar_anotacoes(ListaAnotacoes *lista);

/* Insere mantendo a ordem crescente de timestamp. */
NoAnotacao *inserir_anotacao(ListaAnotacoes *lista, int id, int id_aluno,
                             int timestamp_segundos, const char *texto);
NoAnotacao *buscar_anotacao_por_id(ListaAnotacoes *lista, int id);
int editar_anotacao(ListaAnotacoes *lista, int id, int novo_timestamp,
                    const char *novo_texto);
int remover_anotacao(ListaAnotacoes *lista, int id);
int remover_anotacoes_do_aluno(ListaAnotacoes *lista, int id_aluno);
int contar_anotacoes_do_aluno(const ListaAnotacoes *lista, int id_aluno);
int gerar_id_anotacao(const ListaAnotacoes *lista);

#endif
