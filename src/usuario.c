#include "usuario.h"
#include "utils.h"

#include <stdlib.h>
#include <string.h>

/* Inicializa o descritor da lista principal de usuarios. */
void iniciar_lista_usuarios(ListaUsuarios *lista)
{
    if (lista == NULL) {
        return;
    }
    lista->inicio = NULL;
    lista->fim = NULL;
    lista->tamanho = 0;
}

/* Libera usuarios e tambem todas as sublistas de cada usuario. */
void liberar_lista_usuarios(ListaUsuarios *lista)
{
    NoUsuario *atual;
    NoUsuario *proximo;

    if (lista == NULL) {
        return;
    }

    atual = lista->inicio;
    while (atual != NULL) {
        proximo = atual->proximo;
        liberar_concluidas(&atual->dados.concluidas);
        liberar_historico(&atual->dados.historico);
        free(atual);
        atual = proximo;
    }
    iniciar_lista_usuarios(lista);
}

/* Insere sempre no fim, usando o ponteiro fim do descritor (O(1)). */
NoUsuario *inserir_usuario(ListaUsuarios *lista, int id, const char *nome,
                           const char *email, const char *senha, int perfil)
{
    NoUsuario *novo;

    if (lista == NULL) {
        return NULL;
    }

    novo = (NoUsuario *)malloc(sizeof(NoUsuario));
    if (novo == NULL) {
        return NULL;
    }

    novo->dados.id = id;
    copiar_texto_seguro(novo->dados.nome, TAM_NOME, nome);
    copiar_texto_seguro(novo->dados.email, TAM_EMAIL, email);
    copiar_texto_seguro(novo->dados.senha, TAM_SENHA, senha);
    novo->dados.perfil = perfil;
    iniciar_concluidas(&novo->dados.concluidas);
    iniciar_historico(&novo->dados.historico);
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

NoUsuario *buscar_usuario_por_id(ListaUsuarios *lista, int id)
{
    NoUsuario *atual;

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

NoUsuario *buscar_usuario_por_email(ListaUsuarios *lista, const char *email)
{
    NoUsuario *atual;

    if (lista == NULL || email == NULL) {
        return NULL;
    }
    atual = lista->inicio;
    while (atual != NULL) {
        if (strcmp(atual->dados.email, email) == 0) {
            return atual;
        }
        atual = atual->proximo;
    }
    return NULL;
}

NoUsuario *autenticar_usuario(ListaUsuarios *lista, const char *email,
                              const char *senha)
{
    NoUsuario *usuario = buscar_usuario_por_email(lista, email);

    if (usuario != NULL && strcmp(usuario->dados.senha, senha) == 0) {
        return usuario;
    }
    return NULL;
}

/* Remove um usuario e libera suas duas sublistas. */
int excluir_usuario(ListaUsuarios *lista, int id)
{
    NoUsuario *atual;
    NoUsuario *anterior = NULL;

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

    liberar_concluidas(&atual->dados.concluidas);
    liberar_historico(&atual->dados.historico);
    free(atual);
    lista->tamanho--;
    return 1;
}

/* Novo ID = maior ID existente + 1. */
int gerar_id_usuario(const ListaUsuarios *lista)
{
    const NoUsuario *atual;
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

/* ---- Sublista simples de aulas concluidas ---- */
void iniciar_concluidas(ListaConcluidas *lista)
{
    if (lista == NULL) {
        return;
    }
    lista->inicio = NULL;
    lista->fim = NULL;
    lista->tamanho = 0;
}

int marcar_aula_concluida(ListaConcluidas *lista, int id_aula)
{
    NoAulaConcluida *novo;

    if (lista == NULL || aula_esta_concluida(lista, id_aula)) {
        return 0;
    }

    novo = (NoAulaConcluida *)malloc(sizeof(NoAulaConcluida));
    if (novo == NULL) {
        return 0;
    }
    novo->id_aula = id_aula;
    novo->proximo = NULL;

    if (lista->inicio == NULL) {
        lista->inicio = novo;
    } else {
        lista->fim->proximo = novo;
    }
    lista->fim = novo;
    lista->tamanho++;
    return 1;
}

int aula_esta_concluida(const ListaConcluidas *lista, int id_aula)
{
    const NoAulaConcluida *atual;

    if (lista == NULL) {
        return 0;
    }
    atual = lista->inicio;
    while (atual != NULL) {
        if (atual->id_aula == id_aula) {
            return 1;
        }
        atual = atual->proximo;
    }
    return 0;
}

int remover_aula_concluida(ListaConcluidas *lista, int id_aula)
{
    NoAulaConcluida *anterior = NULL;
    NoAulaConcluida *atual;

    if (lista == NULL) {
        return 0;
    }
    atual = lista->inicio;
    while (atual != NULL && atual->id_aula != id_aula) {
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

void liberar_concluidas(ListaConcluidas *lista)
{
    NoAulaConcluida *atual;
    NoAulaConcluida *proximo;

    if (lista == NULL) {
        return;
    }
    atual = lista->inicio;
    while (atual != NULL) {
        proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    iniciar_concluidas(lista);
}

/* ---- Historico recente: lista duplamente encadeada ---- */
void iniciar_historico(ListaHistorico *historico)
{
    if (historico == NULL) {
        return;
    }
    historico->inicio = NULL;
    historico->fim = NULL;
    historico->tamanho = 0;
}

/*
 * Insere na cabeca em O(1). Se passar de 5 itens, remove a cauda em O(1)
 * usando o ponteiro anterior no ultimo no.
 */
void registrar_historico(ListaHistorico *historico, int id_aula)
{
    NoHistorico *novo;

    if (historico == NULL) {
        return;
    }

    novo = (NoHistorico *)malloc(sizeof(NoHistorico));
    if (novo == NULL) {
        return;
    }
    novo->id_aula = id_aula;
    novo->anterior = NULL;
    novo->proximo = historico->inicio;

    if (historico->inicio != NULL) {
        historico->inicio->anterior = novo;
    }
    historico->inicio = novo;
    if (historico->fim == NULL) {
        historico->fim = novo;
    }
    historico->tamanho++;

    while (historico->tamanho > MAX_HISTORICO) {
        NoHistorico *ultimo = historico->fim;
        historico->fim = ultimo->anterior;
        if (historico->fim != NULL) {
            historico->fim->proximo = NULL;
        } else {
            historico->inicio = NULL;
        }
        free(ultimo);
        historico->tamanho--;
    }
}

int remover_aula_historico(ListaHistorico *historico, int id_aula)
{
    NoHistorico *atual;
    int removidos = 0;

    if (historico == NULL) {
        return 0;
    }
    atual = historico->inicio;
    while (atual != NULL) {
        NoHistorico *proximo = atual->proximo;
        if (atual->id_aula == id_aula) {
            if (atual->anterior == NULL) {
                historico->inicio = proximo;
            } else {
                atual->anterior->proximo = proximo;
            }
            if (proximo != NULL) {
                proximo->anterior = atual->anterior;
            } else {
                historico->fim = atual->anterior;
            }
            free(atual);
            historico->tamanho--;
            removidos++;
        }
        atual = proximo;
    }
    return removidos;
}

void liberar_historico(ListaHistorico *historico)
{
    NoHistorico *atual;
    NoHistorico *proximo;

    if (historico == NULL) {
        return;
    }
    atual = historico->inicio;
    while (atual != NULL) {
        proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    iniciar_historico(historico);
}
