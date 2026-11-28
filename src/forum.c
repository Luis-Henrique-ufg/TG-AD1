#include "forum.h"
#include "utils.h"

#include <stdlib.h>

void iniciar_topicos(ListaTopicos *lista)
{
    if (lista == NULL) {
        return;
    }
    lista->inicio = NULL;
    lista->fim = NULL;
    lista->tamanho = 0;
}

/* libera primeiro as respostas de cada topico e depois o topico. */
static void liberar_respostas(ListaRespostas *lista)
{
    NoResposta *atual = lista->inicio;
    NoResposta *proximo;

    while (atual != NULL) {
        proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    lista->inicio = NULL;
    lista->fim = NULL;
    lista->tamanho = 0;
}

void liberar_topicos(ListaTopicos *lista)
{
    NoTopico *atual;
    NoTopico *proximo;

    if (lista == NULL) {
        return;
    }
    atual = lista->inicio;
    while (atual != NULL) {
        proximo = atual->proximo;
        liberar_respostas(&atual->respostas);
        free(atual);
        atual = proximo;
    }
    iniciar_topicos(lista);
}

NoTopico *inserir_topico(ListaTopicos *lista, int id, int id_curso,
                         int id_autor, const char *titulo,
                         const char *pergunta)
{
    NoTopico *novo;

    if (lista == NULL) {
        return NULL;
    }
    novo = (NoTopico *)malloc(sizeof(NoTopico));
    if (novo == NULL) {
        return NULL;
    }

    novo->id = id;
    novo->id_curso = id_curso;
    novo->id_autor = id_autor;
    copiar_texto_seguro(novo->titulo, TAM_TITULO_TOPICO, titulo);
    copiar_texto_seguro(novo->pergunta, TAM_PERGUNTA, pergunta);
    novo->respostas.inicio = NULL;
    novo->respostas.fim = NULL;
    novo->respostas.tamanho = 0;
    novo->proximo = NULL;

    if (lista->inicio == NULL) {
        lista->inicio = novo;
    } else {
        lista->fim->proximo = novo;
    }
    lista->fim = novo;
    lista->tamanho++;
    return novo;
}

NoTopico *buscar_topico_por_id(ListaTopicos *lista, int id)
{
    NoTopico *atual;

    if (lista == NULL) {
        return NULL;
    }
    atual = lista->inicio;
    while (atual != NULL) {
        if (atual->id == id) {
            return atual;
        }
        atual = atual->proximo;
    }
    return NULL;
}

int excluir_topico(ListaTopicos *lista, int id)
{
    NoTopico *anterior = NULL;
    NoTopico *atual;

    if (lista == NULL) {
        return 0;
    }
    atual = lista->inicio;
    while (atual != NULL && atual->id != id) {
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
    liberar_respostas(&atual->respostas);
    free(atual);
    lista->tamanho--;
    return 1;
}

static int remover_topicos_se(ListaTopicos *lista, int por_curso, int id)
{
    NoTopico *anterior = NULL;
    NoTopico *atual;
    int removidos = 0;

    if (lista == NULL) {
        return 0;
    }
    atual = lista->inicio;
    while (atual != NULL) {
        NoTopico *proximo = atual->proximo;
        if ((por_curso && atual->id_curso == id) ||
            (!por_curso && atual->id_autor == id)) {
            if (anterior == NULL) {
                lista->inicio = proximo;
            } else {
                anterior->proximo = proximo;
            }
            if (lista->fim == atual) {
                lista->fim = anterior;
            }
            liberar_respostas(&atual->respostas);
            free(atual);
            lista->tamanho--;
            removidos++;
        } else {
            anterior = atual;
        }
        atual = proximo;
    }
    return removidos;
}

int remover_topicos_por_curso(ListaTopicos *lista, int id_curso)
{
    return remover_topicos_se(lista, 1, id_curso);
}

int remover_topicos_por_autor(ListaTopicos *lista, int id_autor)
{
    return remover_topicos_se(lista, 0, id_autor);
}

static int remover_respostas_se(ListaRespostas *respostas, int id_autor)
{
    NoResposta *anterior = NULL;
    NoResposta *atual;
    int removidas = 0;

    atual = respostas->inicio;
    while (atual != NULL) {
        NoResposta *proximo = atual->proximo;
        if (atual->id_autor == id_autor) {
            if (anterior == NULL) {
                respostas->inicio = proximo;
            } else {
                anterior->proximo = proximo;
            }
            if (respostas->fim == atual) {
                respostas->fim = anterior;
            }
            free(atual);
            respostas->tamanho--;
            removidas++;
        } else {
            anterior = atual;
        }
        atual = proximo;
    }
    return removidas;
}

int remover_respostas_por_autor(ListaTopicos *lista, int id_autor)
{
    NoTopico *topico;
    int removidas = 0;

    if (lista == NULL) {
        return 0;
    }
    topico = lista->inicio;
    while (topico != NULL) {
        removidas += remover_respostas_se(&topico->respostas, id_autor);
        topico = topico->proximo;
    }
    return removidas;
}

int gerar_id_topico(const ListaTopicos *lista)
{
    const NoTopico *atual;
    int maior = 0;

    if (lista == NULL) {
        return 1;
    }
    atual = lista->inicio;
    while (atual != NULL) {
        if (atual->id > maior) {
            maior = atual->id;
        }
        atual = atual->proximo;
    }
    return maior + 1;
}

NoResposta *inserir_resposta(NoTopico *topico, int id, int id_autor,
                             const char *texto)
{
    NoResposta *novo;

    if (topico == NULL) {
        return NULL;
    }
    novo = (NoResposta *)malloc(sizeof(NoResposta));
    if (novo == NULL) {
        return NULL;
    }

    novo->id = id;
    novo->id_autor = id_autor;
    copiar_texto_seguro(novo->texto, TAM_RESPOSTA, texto);
    novo->proximo = NULL;

    if (topico->respostas.inicio == NULL) {
        topico->respostas.inicio = novo;
    } else {
        topico->respostas.fim->proximo = novo;
    }
    topico->respostas.fim = novo;
    topico->respostas.tamanho++;
    return novo;
}

int gerar_id_resposta(const ListaTopicos *lista)
{
    const NoTopico *atual;
    const NoResposta *resposta;
    int maior = 0;

    if (lista == NULL) {
        return 1;
    }
    atual = lista->inicio;
    while (atual != NULL) {
        resposta = atual->respostas.inicio;
        while (resposta != NULL) {
            if (resposta->id > maior) {
                maior = resposta->id;
            }
            resposta = resposta->proximo;
        }
        atual = atual->proximo;
    }
    return maior + 1;
}
