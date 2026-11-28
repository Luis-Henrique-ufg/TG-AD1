#include "curso.h"
#include "utils.h"

#include <stdlib.h>

/* Libera o nivel mais interno (aulas e suas anotacoes). */
static void liberar_aulas(ListaAulas *aulas)
{
    NoAula *atual;
    NoAula *proximo;

    if (aulas == NULL) {
        return;
    }
    atual = aulas->inicio;
    while (atual != NULL) {
        proximo = atual->proximo;
        liberar_anotacoes(&atual->anotacoes);
        free(atual);
        atual = proximo;
    }
    aulas->inicio = NULL;
    aulas->fim = NULL;
    aulas->tamanho = 0;
}

/* Exclusao em cascata: modulos -> aulas -> anotacoes. */
static void liberar_modulos(ListaModulos *modulos)
{
    NoModulo *atual;
    NoModulo *proximo;

    if (modulos == NULL) {
        return;
    }
    atual = modulos->inicio;
    while (atual != NULL) {
        proximo = atual->proximo;
        liberar_aulas(&atual->aulas);
        free(atual);
        atual = proximo;
    }
    modulos->inicio = NULL;
    modulos->fim = NULL;
    modulos->tamanho = 0;
}

void iniciar_cursos(ListaCursos *cursos)
{
    if (cursos == NULL) {
        return;
    }
    cursos->inicio = NULL;
    cursos->fim = NULL;
    cursos->tamanho = 0;
}

void liberar_cursos(ListaCursos *cursos)
{
    NoCurso *atual;
    NoCurso *proximo;

    if (cursos == NULL) {
        return;
    }
    atual = cursos->inicio;
    while (atual != NULL) {
        proximo = atual->proximo;
        liberar_modulos(&atual->modulos);
        free(atual);
        atual = proximo;
    }
    iniciar_cursos(cursos);
}

NoCurso *inserir_curso(ListaCursos *cursos, int id, const char *titulo,
                       const char *descricao)
{
    NoCurso *novo;

    if (cursos == NULL) {
        return NULL;
    }
    novo = (NoCurso *)malloc(sizeof(NoCurso));
    if (novo == NULL) {
        return NULL;
    }

    novo->id = id;
    copiar_texto_seguro(novo->titulo, TAM_TITULO, titulo);
    copiar_texto_seguro(novo->descricao, TAM_DESCRICAO, descricao);
    novo->modulos.inicio = NULL;
    novo->modulos.fim = NULL;
    novo->modulos.tamanho = 0;
    novo->proximo = NULL;

    if (cursos->inicio == NULL) {
        cursos->inicio = novo;
    } else {
        cursos->fim->proximo = novo;
    }
    cursos->fim = novo;
    cursos->tamanho++;
    return novo;
}

NoCurso *buscar_curso_por_id(ListaCursos *cursos, int id)
{
    NoCurso *atual;

    if (cursos == NULL) {
        return NULL;
    }
    atual = cursos->inicio;
    while (atual != NULL) {
        if (atual->id == id) {
            return atual;
        }
        atual = atual->proximo;
    }
    return NULL;
}

int editar_curso(NoCurso *curso, const char *titulo, const char *descricao)
{
    if (curso == NULL) {
        return 0;
    }
    copiar_texto_seguro(curso->titulo, TAM_TITULO, titulo);
    copiar_texto_seguro(curso->descricao, TAM_DESCRICAO, descricao);
    return 1;
}

int excluir_curso(ListaCursos *cursos, int id)
{
    NoCurso *anterior = NULL;
    NoCurso *atual;

    if (cursos == NULL) {
        return 0;
    }
    atual = cursos->inicio;
    while (atual != NULL && atual->id != id) {
        anterior = atual;
        atual = atual->proximo;
    }
    if (atual == NULL) {
        return 0;
    }

    if (anterior == NULL) {
        cursos->inicio = atual->proximo;
    } else {
        anterior->proximo = atual->proximo;
    }
    if (cursos->fim == atual) {
        cursos->fim = anterior;
    }
    liberar_modulos(&atual->modulos);
    free(atual);
    cursos->tamanho--;
    return 1;
}

int remover_referencias_a_curso(ListaCursos *cursos, int id_curso,
                                int (*remover)(void *, int), void *alvo)
{
    NoCurso *curso = cursos != NULL ? cursos->inicio : NULL;
    int removidos = 0;

    while (curso != NULL) {
        if (curso->id == id_curso) {
            NoModulo *modulo = curso->modulos.inicio;
            while (modulo != NULL) {
                NoAula *aula = modulo->aulas.inicio;
                while (aula != NULL) {
                    removidos += remover(alvo, aula->id);
                    aula = aula->proximo;
                }
                modulo = modulo->proximo;
            }
        }
        curso = curso->proximo;
    }
    return removidos;
}

int remover_referencias_a_aula(ListaCursos *cursos, int id_aula,
                               int (*remover)(void *, int), void *alvo)
{
    NoCurso *curso = cursos != NULL ? cursos->inicio : NULL;
    int removidos = 0;

    while (curso != NULL) {
        NoModulo *modulo = curso->modulos.inicio;
        while (modulo != NULL) {
            if (buscar_aula_no_modulo(modulo, id_aula) != NULL) {
                removidos += remover(alvo, id_aula);
            }
            modulo = modulo->proximo;
        }
        curso = curso->proximo;
    }
    return removidos;
}

int remover_anotacoes_de_aluno(ListaCursos *cursos, int id_aluno)
{
    NoCurso *curso = cursos != NULL ? cursos->inicio : NULL;
    int removidas = 0;

    while (curso != NULL) {
        NoModulo *modulo = curso->modulos.inicio;
        while (modulo != NULL) {
            NoAula *aula = modulo->aulas.inicio;
            while (aula != NULL) {
                removidas += remover_anotacoes_do_aluno(&aula->anotacoes,
                                                        id_aluno);
                aula = aula->proximo;
            }
            modulo = modulo->proximo;
        }
        curso = curso->proximo;
    }
    return removidas;
}

NoCurso *buscar_curso_do_modulo_wrapper(ListaCursos *cursos, int id_modulo)
{
    NoCurso *curso = cursos != NULL ? cursos->inicio : NULL;

    while (curso != NULL) {
        if (buscar_modulo_no_curso(curso, id_modulo) != NULL) {
            return curso;
        }
        curso = curso->proximo;
    }
    return NULL;
}

NoModulo *buscar_modulo_da_aula_wrapper(ListaCursos *cursos, int id_aula)
{
    NoCurso *curso = cursos != NULL ? cursos->inicio : NULL;

    while (curso != NULL) {
        NoModulo *modulo = curso->modulos.inicio;
        while (modulo != NULL) {
            if (buscar_aula_no_modulo(modulo, id_aula) != NULL) {
                return modulo;
            }
            modulo = modulo->proximo;
        }
        curso = curso->proximo;
    }
    return NULL;
}

int gerar_id_curso(const ListaCursos *cursos)
{
    const NoCurso *atual;
    int maior = 0;

    if (cursos == NULL) {
        return 1;
    }
    atual = cursos->inicio;
    while (atual != NULL) {
        if (atual->id > maior) {
            maior = atual->id;
        }
        atual = atual->proximo;
    }
    return maior + 1;
}

NoModulo *inserir_modulo(NoCurso *curso, int id, const char *titulo)
{
    NoModulo *novo;

    if (curso == NULL) {
        return NULL;
    }
    novo = (NoModulo *)malloc(sizeof(NoModulo));
    if (novo == NULL) {
        return NULL;
    }

    novo->id = id;
    copiar_texto_seguro(novo->titulo, TAM_TITULO, titulo);
    novo->aulas.inicio = NULL;
    novo->aulas.fim = NULL;
    novo->aulas.tamanho = 0;
    novo->proximo = NULL;

    if (curso->modulos.inicio == NULL) {
        curso->modulos.inicio = novo;
    } else {
        curso->modulos.fim->proximo = novo;
    }
    curso->modulos.fim = novo;
    curso->modulos.tamanho++;
    return novo;
}

NoModulo *buscar_modulo_no_curso(NoCurso *curso, int id)
{
    NoModulo *atual;

    if (curso == NULL) {
        return NULL;
    }
    atual = curso->modulos.inicio;
    while (atual != NULL) {
        if (atual->id == id) {
            return atual;
        }
        atual = atual->proximo;
    }
    return NULL;
}

NoModulo *buscar_modulo_por_id(ListaCursos *cursos, int id)
{
    NoCurso *atual;
    NoModulo *modulo;

    if (cursos == NULL) {
        return NULL;
    }
    atual = cursos->inicio;
    while (atual != NULL) {
        modulo = buscar_modulo_no_curso(atual, id);
        if (modulo != NULL) {
            return modulo;
        }
        atual = atual->proximo;
    }
    return NULL;
}

int editar_modulo(NoModulo *modulo, const char *titulo)
{
    if (modulo == NULL) {
        return 0;
    }
    copiar_texto_seguro(modulo->titulo, TAM_TITULO, titulo);
    return 1;
}

int excluir_modulo(NoCurso *curso, int id)
{
    NoModulo *anterior = NULL;
    NoModulo *atual;

    if (curso == NULL) {
        return 0;
    }
    atual = curso->modulos.inicio;
    while (atual != NULL && atual->id != id) {
        anterior = atual;
        atual = atual->proximo;
    }
    if (atual == NULL) {
        return 0;
    }

    if (anterior == NULL) {
        curso->modulos.inicio = atual->proximo;
    } else {
        anterior->proximo = atual->proximo;
    }
    if (curso->modulos.fim == atual) {
        curso->modulos.fim = anterior;
    }
    liberar_aulas(&atual->aulas);
    free(atual);
    curso->modulos.tamanho--;
    return 1;
}

int gerar_id_modulo(const ListaCursos *cursos)
{
    const NoCurso *curso;
    const NoModulo *modulo;
    int maior = 0;

    if (cursos == NULL) {
        return 1;
    }
    curso = cursos->inicio;
    while (curso != NULL) {
        modulo = curso->modulos.inicio;
        while (modulo != NULL) {
            if (modulo->id > maior) {
                maior = modulo->id;
            }
            modulo = modulo->proximo;
        }
        curso = curso->proximo;
    }
    return maior + 1;
}

NoAula *inserir_aula(NoModulo *modulo, int id, const char *titulo,
                     int duracao_segundos, int eh_aula_valida)
{
    NoAula *nova;

    if (modulo == NULL) {
        return NULL;
    }
    nova = (NoAula *)malloc(sizeof(NoAula));
    if (nova == NULL) {
        return NULL;
    }

    nova->id = id;
    copiar_texto_seguro(nova->titulo, TAM_TITULO, titulo);
    nova->duracao_segundos = duracao_segundos;
    nova->eh_aula_valida = eh_aula_valida;
    nova->anotacoes.inicio = NULL;
    nova->anotacoes.fim = NULL;
    nova->anotacoes.tamanho = 0;
    nova->proximo = NULL;

    if (modulo->aulas.inicio == NULL) {
        modulo->aulas.inicio = nova;
    } else {
        modulo->aulas.fim->proximo = nova;
    }
    modulo->aulas.fim = nova;
    modulo->aulas.tamanho++;
    return nova;
}

NoAula *buscar_aula_no_modulo(NoModulo *modulo, int id)
{
    NoAula *atual;

    if (modulo == NULL) {
        return NULL;
    }
    atual = modulo->aulas.inicio;
    while (atual != NULL) {
        if (atual->id == id) {
            return atual;
        }
        atual = atual->proximo;
    }
    return NULL;
}

NoAula *buscar_aula_por_id(ListaCursos *cursos, int id)
{
    NoCurso *curso;
    NoModulo *modulo;
    NoAula *aula;

    if (cursos == NULL) {
        return NULL;
    }
    curso = cursos->inicio;
    while (curso != NULL) {
        modulo = curso->modulos.inicio;
        while (modulo != NULL) {
            aula = buscar_aula_no_modulo(modulo, id);
            if (aula != NULL) {
                return aula;
            }
            modulo = modulo->proximo;
        }
        curso = curso->proximo;
    }
    return NULL;
}

int editar_aula(NoAula *aula, const char *titulo, int duracao_segundos,
                int eh_aula_valida)
{
    if (aula == NULL) {
        return 0;
    }
    copiar_texto_seguro(aula->titulo, TAM_TITULO, titulo);
    aula->duracao_segundos = duracao_segundos;
    aula->eh_aula_valida = eh_aula_valida;
    return 1;
}

int excluir_aula(NoModulo *modulo, int id)
{
    NoAula *anterior = NULL;
    NoAula *atual;

    if (modulo == NULL) {
        return 0;
    }
    atual = modulo->aulas.inicio;
    while (atual != NULL && atual->id != id) {
        anterior = atual;
        atual = atual->proximo;
    }
    if (atual == NULL) {
        return 0;
    }

    if (anterior == NULL) {
        modulo->aulas.inicio = atual->proximo;
    } else {
        anterior->proximo = atual->proximo;
    }
    if (modulo->aulas.fim == atual) {
        modulo->aulas.fim = anterior;
    }
    liberar_anotacoes(&atual->anotacoes);
    free(atual);
    modulo->aulas.tamanho--;
    return 1;
}

int gerar_id_aula(const ListaCursos *cursos)
{
    const NoCurso *curso;
    const NoModulo *modulo;
    const NoAula *aula;
    int maior = 0;

    if (cursos == NULL) {
        return 1;
    }
    curso = cursos->inicio;
    while (curso != NULL) {
        modulo = curso->modulos.inicio;
        while (modulo != NULL) {
            aula = modulo->aulas.inicio;
            while (aula != NULL) {
                if (aula->id > maior) {
                    maior = aula->id;
                }
                aula = aula->proximo;
            }
            modulo = modulo->proximo;
        }
        curso = curso->proximo;
    }
    return maior + 1;
}

/* RF03: somente itens com eh_aula_valida == 1 entram no calculo. */
void calcular_progresso_modulo(const NoModulo *modulo,
                               const ListaConcluidas *concluidas,
                               int *total, int *feitas)
{
    const NoAula *aula;

    if (modulo == NULL || total == NULL || feitas == NULL) {
        return;
    }
    *total = 0;
    *feitas = 0;

    aula = modulo->aulas.inicio;
    while (aula != NULL) {
        if (aula->eh_aula_valida == 1) {
            (*total)++;
            if (aula_esta_concluida(concluidas, aula->id)) {
                (*feitas)++;
            }
        }
        aula = aula->proximo;
    }
}

void calcular_progresso_curso(const ListaCursos *cursos,
                              const ListaConcluidas *concluidas,
                              int *total, int *feitas)
{
    const NoCurso *curso;
    int parcial_total = 0;
    int parcial_feitas = 0;

    if (total == NULL || feitas == NULL) {
        return;
    }
    *total = 0;
    *feitas = 0;

    if (cursos == NULL) {
        return;
    }
    curso = cursos->inicio;
    while (curso != NULL) {
        const NoModulo *modulo = curso->modulos.inicio;
        while (modulo != NULL) {
            calcular_progresso_modulo(modulo, concluidas, &parcial_total,
                                      &parcial_feitas);
            *total += parcial_total;
            *feitas += parcial_feitas;
            modulo = modulo->proximo;
        }
        curso = curso->proximo;
    }
}
