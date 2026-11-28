#ifndef AVALIACAO_H
#define AVALIACAO_H

#define TAM_ATIVIDADE 100

/* Cada nota pertence a um curso e a um aluno. */
typedef struct {
    int id;
    int id_curso;
    int id_aluno;
    char titulo_atividade[TAM_ATIVIDADE];
    float nota;
} Avaliacao;

typedef struct NoAvaliacao {
    Avaliacao dados;
    struct NoAvaliacao *proximo;
} NoAvaliacao;

typedef struct {
    NoAvaliacao *inicio;
    NoAvaliacao *fim;
    int tamanho;
} ListaAvaliacoes;

void iniciar_avaliacoes(ListaAvaliacoes *lista);
void liberar_avaliacoes(ListaAvaliacoes *lista);

/* Insere em ordem decrescente de nota; em empate, mantem a ordem de chegada. */
NoAvaliacao *inserir_avaliacao(ListaAvaliacoes *lista, int id, int id_curso,
                               int id_aluno, const char *titulo_atividade,
                               float nota);
NoAvaliacao *buscar_avaliacao_por_id(ListaAvaliacoes *lista, int id);
int remover_avaliacao(ListaAvaliacoes *lista, int id);
int remover_avaliacoes_por_curso(ListaAvaliacoes *lista, int id_curso);
int remover_avaliacoes_por_aluno(ListaAvaliacoes *lista, int id_aluno);
int gerar_id_avaliacao(const ListaAvaliacoes *lista);

#endif
