#include "avaliacao.h"
#include "utils.h"

#include <stdlib.h>

void iniciar_avaliacoes(ListaAvaliacoes *lista)
{
    if (lista == NULL) {
        return;
    }
    lista->inicio = NULL;
    lista->fim = NULL;
    lista->tamanho = 0;
}

/* Ordem decrescente de nota; notas iguais mantem a ordem de insercao. */
NoAvaliacao *inserir_avaliacao(ListaAvaliacoes *lista, int id, int id_curso,
                               int id_aluno, const char *titulo_atividade,
                               float nota)
{
    NoAvaliacao *anterior = NULL;
    NoAvaliacao *atual;
    NoAvaliacao *novo;

    if (lista == NULL) {
        return NULL;
    }
    novo = (NoAvaliacao *)malloc(sizeof(NoAvaliacao));
    if (novo == NULL) {
        return NULL;
    }

    novo->dados.id = id;
    novo->dados.id_curso = id_curso;
    novo->dados.id_aluno = id_aluno;
    copiar_texto_seguro(novo->dados.titulo_atividade, TAM_ATIVIDADE,
                        titulo_atividade);
    novo->dados.nota = nota;

    atual = lista->inicio;
    while (atual != NULL && atual->dados.nota >= nota) {
        anterior = atual;
        atual = atual->proximo;
    }
    novo->proximo = atual;

    if (anterior == NULL) {
        lista->inicio = novo;
    } else {
        anterior->proximo = novo;
    }
    if (novo->proximo == NULL) {
        lista->fim = novo;
    }
    lista->tamanho++;
    return novo;
}

NoAvaliacao *buscar_avaliacao_por_id(ListaAvaliacoes *lista, int id)
{
    NoAvaliacao *atual;

    if (lista == NULL) {
        return NULL;
    }
    atual = lista->inicio;
    while (atual != NULL) {
        if (atual->dados.id == id) {
            return atual;
        }
        atual = atual->proximo;
    }
    return NULL;
}

int remover_avaliacao(ListaAvaliacoes *lista, int id)
{
    NoAvaliacao *anterior = NULL;
    NoAvaliacao *atual;

    if (lista == NULL) {
        return 0;
    }
    atual = lista->inicio;
    while (atual != NULL && atual->dados.id != id) {
        anterior = atual;
        atual = atual->proximo;
    }
    if (atual == NULL) {
        return 0;
    }

    if (anterior == NULL) {
        lista->inicio = atual->proximo;
    } else {
        anterior->proximo = atual->proximo;
    }
    if (lista->fim == atual) {
        lista->fim = anterior;
    }
    free(atual);
    lista->tamanho--;
    return 1;
}

int remover_avaliacoes_por_curso(ListaAvaliacoes *lista, int id_curso)
{
    NoAvaliacao *anterior = NULL;
    NoAvaliacao *atual;
    int removidas = 0;

    if (lista == NULL) {
        return 0;
    }
    atual = lista->inicio;
    while (atual != NULL) {
        NoAvaliacao *proximo = atual->proximo;
        if (atual->dados.id_curso == id_curso) {
            if (anterior == NULL) {
                lista->inicio = proximo;
            } else {
                anterior->proximo = proximo;
            }
            if (lista->fim == atual) {
                lista->fim = anterior;
            }
            free(atual);
            lista->tamanho--;
            removidas++;
        } else {
            anterior = atual;
        }
        atual = proximo;
    }
    return removidas;
}

int remover_avaliacoes_por_aluno(ListaAvaliacoes *lista, int id_aluno)
{
    NoAvaliacao *anterior = NULL;
    NoAvaliacao *atual;
    int removidas = 0;

    if (lista == NULL) {
        return 0;
    }
    atual = lista->inicio;
    while (atual != NULL) {
        NoAvaliacao *proximo = atual->proximo;
        if (atual->dados.id_aluno == id_aluno) {
            if (anterior == NULL) {
                lista->inicio = proximo;
            } else {
                anterior->proximo = proximo;
            }
            if (lista->fim == atual) {
                lista->fim = anterior;
            }
            free(atual);
            lista->tamanho--;
            removidas++;
        } else {
            anterior = atual;
        }
        atual = proximo;
    }
    return removidas;
}

int gerar_id_avaliacao(const ListaAvaliacoes *lista)
{
    const NoAvaliacao *atual;
    int maior = 0;

    if (lista == NULL) {
        return 1;
    }
    atual = lista->inicio;
    while (atual != NULL) {
        if (atual->dados.id > maior) {
            maior = atual->dados.id;
        }
        atual = atual->proximo;
    }
    return maior + 1;
}

void liberar_avaliacoes(ListaAvaliacoes *lista)
{
    NoAvaliacao *atual;
    NoAvaliacao *proximo;

    if (lista == NULL) {
        return;
    }
    atual = lista->inicio;
    while (atual != NULL) {
        proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    iniciar_avaliacoes(lista);
}
