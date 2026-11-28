#include "anotacao.h"
#include "utils.h"

#include <stdlib.h>
#include <string.h>

void iniciar_anotacoes(ListaAnotacoes *lista)
{
    if (lista == NULL) {
        return;
    }
    lista->inicio = NULL;
    lista->fim = NULL;
    lista->tamanho = 0;
}

/*
 * Funcao auxiliar: ja recebe um no pronto e o encaixa na posicao correta.
 * Assim a edicao pode mudar o timestamp sem precisar alocar outro no.
 */
static void ligar_no_ordenado(ListaAnotacoes *lista, NoAnotacao *no)
{
    NoAnotacao *anterior = NULL;
    NoAnotacao *atual = lista->inicio;

    while (atual != NULL &&
           atual->dados.timestamp_segundos < no->dados.timestamp_segundos) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (anterior == NULL) {
        no->proximo = lista->inicio;
        lista->inicio = no;
        if (lista->fim == NULL) {
            lista->fim = no;
        }
    } else {
        no->proximo = anterior->proximo;
        anterior->proximo = no;
        if (no->proximo == NULL) {
            lista->fim = no;
        }
    }
}

NoAnotacao *inserir_anotacao(ListaAnotacoes *lista, int id, int id_aluno,
                             int timestamp_segundos, const char *texto)
{
    NoAnotacao *novo;

    if (lista == NULL) {
        return NULL;
    }
    novo = (NoAnotacao *)malloc(sizeof(NoAnotacao));
    if (novo == NULL) {
        return NULL;
    }

    novo->dados.id = id;
    novo->dados.id_aluno = id_aluno;
    novo->dados.timestamp_segundos = timestamp_segundos;
    copiar_texto_seguro(novo->dados.texto, TAM_TEXTO_ANOTACAO, texto);
    novo->proximo = NULL;

    ligar_no_ordenado(lista, novo);
    lista->tamanho++;
    return novo;
}

NoAnotacao *buscar_anotacao_por_id(ListaAnotacoes *lista, int id)
{
    NoAnotacao *atual;

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

int editar_anotacao(ListaAnotacoes *lista, int id, int novo_timestamp,
                    const char *novo_texto)
{
    NoAnotacao *anterior = NULL;
    NoAnotacao *alvo;

    if (lista == NULL) {
        return 0;
    }

    alvo = lista->inicio;
    while (alvo != NULL && alvo->dados.id != id) {
        anterior = alvo;
        alvo = alvo->proximo;
    }
    if (alvo == NULL) {
        return 0;
    }

    if (anterior == NULL) {
        lista->inicio = alvo->proximo;
    } else {
        anterior->proximo = alvo->proximo;
    }
    if (lista->fim == alvo) {
        lista->fim = anterior;
    }

    alvo->dados.timestamp_segundos = novo_timestamp;
    copiar_texto_seguro(alvo->dados.texto, TAM_TEXTO_ANOTACAO, novo_texto);
    ligar_no_ordenado(lista, alvo);
    return 1;
}

int remover_anotacao(ListaAnotacoes *lista, int id)
{
    NoAnotacao *anterior = NULL;
    NoAnotacao *atual;

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

int remover_anotacoes_do_aluno(ListaAnotacoes *lista, int id_aluno)
{
    NoAnotacao *anterior = NULL;
    NoAnotacao *atual;
    int removidas = 0;

    if (lista == NULL) {
        return 0;
    }
    atual = lista->inicio;
    while (atual != NULL) {
        NoAnotacao *proximo = atual->proximo;
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

int contar_anotacoes_do_aluno(const ListaAnotacoes *lista, int id_aluno)
{
    const NoAnotacao *atual;
    int quantidade = 0;

    if (lista == NULL) {
        return 0;
    }
    atual = lista->inicio;
    while (atual != NULL) {
        if (atual->dados.id_aluno == id_aluno) {
            quantidade++;
        }
        atual = atual->proximo;
    }
    return quantidade;
}

int gerar_id_anotacao(const ListaAnotacoes *lista)
{
    const NoAnotacao *atual;
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

void liberar_anotacoes(ListaAnotacoes *lista)
{
    NoAnotacao *atual;
    NoAnotacao *proximo;

    if (lista == NULL) {
        return;
    }
    atual = lista->inicio;
    while (atual != NULL) {
        proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    iniciar_anotacoes(lista);
}
