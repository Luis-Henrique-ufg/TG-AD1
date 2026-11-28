#include "persistencia.h"
#include "utils.h"

#include <stdio.h>
#include <string.h>

/* Monta "pasta/arquivo" sem gravar enderecos de memoria em disco. */
static FILE *abrir_arquivo(const char *pasta, const char *nome, const char *modo)
{
    char caminho[300];

    if (pasta == NULL) {
        pasta = ".";
    }
    snprintf(caminho, sizeof(caminho), "%s/%s", pasta, nome);
    return fopen(caminho, modo);
}

void iniciar_sistema(Sistema *sistema)
{
    if (sistema == NULL) {
        return;
    }
    iniciar_lista_usuarios(&sistema->usuarios);
    iniciar_cursos(&sistema->cursos);
    iniciar_avaliacoes(&sistema->avaliacoes);
    iniciar_topicos(&sistema->topicos);
}

void liberar_sistema(Sistema *sistema)
{
    if (sistema == NULL) {
        return;
    }
    liberar_lista_usuarios(&sistema->usuarios);
    liberar_cursos(&sistema->cursos);
    liberar_avaliacoes(&sistema->avaliacoes);
    liberar_topicos(&sistema->topicos);
}

/*
 * A ordem de leitura respeita a hierarquia:
 * usuarios -> cursos -> modulos -> aulas -> anotacoes -> concluidas ->
 * avaliacoes -> topicos -> respostas.
 */
int carregar_sistema(Sistema *sistema, const char *pasta)
{
    FILE *arquivo;
    char linha[600];

    if (sistema == NULL) {
        return 0;
    }
    iniciar_sistema(sistema);

    /* 1) Usuarios */
    arquivo = abrir_arquivo(pasta, "usuarios.txt", "r");
    if (arquivo == NULL) {
        return 0;
    }
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        linha[strcspn(linha, "\r\n")] = '\0';
        int id = 0;
        int perfil = 0;
        char nome[TAM_NOME];
        char email[TAM_EMAIL];
        char senha[TAM_SENHA];

        if (sscanf(linha, "%d|%79[^|]|%79[^|]|%39[^|\n]|%d",
                   &id, nome, email, senha, &perfil) == 5) {
            inserir_usuario(&sistema->usuarios, id, nome, email, senha, perfil);
        }
    }
    fclose(arquivo);

    /* 2) Cursos */
    arquivo = abrir_arquivo(pasta, "cursos.txt", "r");
    if (arquivo == NULL) {
        return 0;
    }
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        linha[strcspn(linha, "\r\n")] = '\0';
        int id = 0;
        char titulo[TAM_TITULO];
        char descricao[TAM_DESCRICAO];

        if (sscanf(linha, "%d|%99[^|]|%199[^|\n]",
                   &id, titulo, descricao) == 3) {
            inserir_curso(&sistema->cursos, id, titulo, descricao);
        }
    }
    fclose(arquivo);

    /* 3) Modulos: o ID do curso vem primeiro no arquivo. */
    arquivo = abrir_arquivo(pasta, "modulos.txt", "r");
    if (arquivo == NULL) {
        return 0;
    }
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        linha[strcspn(linha, "\r\n")] = '\0';
        int id_curso = 0;
        int id_modulo = 0;
        char titulo[TAM_TITULO];
        NoCurso *curso;

        if (sscanf(linha, "%d|%d|%99[^|\n]",
                   &id_curso, &id_modulo, titulo) == 3) {
            curso = buscar_curso_por_id(&sistema->cursos, id_curso);
            if (curso != NULL) {
                inserir_modulo(curso, id_modulo, titulo);
            }
        }
    }
    fclose(arquivo);

    /* 4) Aulas: localizadas pelo ID do modulo. */
    arquivo = abrir_arquivo(pasta, "aulas.txt", "r");
    if (arquivo == NULL) {
        return 0;
    }
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        linha[strcspn(linha, "\r\n")] = '\0';
        int id_modulo = 0;
        int id_aula = 0;
        int duracao = 0;
        int valida = 0;
        char titulo[TAM_TITULO];
        NoModulo *modulo;

        if (sscanf(linha, "%d|%d|%99[^|]|%d|%d",
                   &id_modulo, &id_aula, titulo, &duracao, &valida) == 5) {
            modulo = buscar_modulo_por_id(&sistema->cursos, id_modulo);
            if (modulo != NULL) {
                inserir_aula(modulo, id_aula, titulo, duracao, valida);
            }
        }
    }
    fclose(arquivo);

    /* 5) Anotacoes: a insercao ja deixa a lista ordenada por timestamp. */
    arquivo = abrir_arquivo(pasta, "anotacoes.txt", "r");
    if (arquivo == NULL) {
        return 0;
    }
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        linha[strcspn(linha, "\r\n")] = '\0';
        int id_aula = 0;
        int id = 0;
        int id_aluno = 0;
        int timestamp = 0;
        char texto[TAM_TEXTO_ANOTACAO];
        NoAula *aula;

        if (sscanf(linha, "%d|%d|%d|%d|%199[^|\n]",
                   &id_aula, &id, &id_aluno, &timestamp, texto) == 5) {
            aula = buscar_aula_por_id(&sistema->cursos, id_aula);
            if (aula != NULL) {
                inserir_anotacao(&aula->anotacoes, id, id_aluno,
                                 timestamp, texto);
            }
        }
    }
    fclose(arquivo);

    /* 6) Aulas concluidas por cada discente */
    arquivo = abrir_arquivo(pasta, "concluidas.txt", "r");
    if (arquivo == NULL) {
        return 0;
    }
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        linha[strcspn(linha, "\r\n")] = '\0';
        int id_usuario = 0;
        int id_aula = 0;
        NoUsuario *usuario;

        if (sscanf(linha, "%d|%d", &id_usuario, &id_aula) == 2) {
            usuario = buscar_usuario_por_id(&sistema->usuarios, id_usuario);
            if (usuario != NULL &&
                buscar_aula_por_id(&sistema->cursos, id_aula) != NULL) {
                marcar_aula_concluida(&usuario->dados.concluidas, id_aula);
            }
        }
    }
    fclose(arquivo);

    /* 7) Avaliacoes */
    arquivo = abrir_arquivo(pasta, "avaliacoes.txt", "r");
    if (arquivo == NULL) {
        return 0;
    }
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        linha[strcspn(linha, "\r\n")] = '\0';
        int id = 0;
        int id_curso = 0;
        int id_aluno = 0;
        char titulo[TAM_ATIVIDADE];
        float nota = 0.0f;

        if (sscanf(linha, "%d|%d|%d|%99[^|]|%f",
                   &id, &id_curso, &id_aluno, titulo, &nota) == 5 &&
            buscar_curso_por_id(&sistema->cursos, id_curso) != NULL &&
            buscar_usuario_por_id(&sistema->usuarios, id_aluno) != NULL) {
            inserir_avaliacao(&sistema->avaliacoes, id, id_curso, id_aluno,
                              titulo, nota);
        }
    }
    fclose(arquivo);

    /* 8) Topicos do forum */
    arquivo = abrir_arquivo(pasta, "topicos.txt", "r");
    if (arquivo == NULL) {
        return 0;
    }
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        linha[strcspn(linha, "\r\n")] = '\0';
        int id = 0;
        int id_curso = 0;
        int id_autor = 0;
        char titulo[TAM_TITULO_TOPICO];
        char pergunta[TAM_PERGUNTA];

        if (sscanf(linha, "%d|%d|%d|%99[^|]|%299[^|\n]",
                   &id, &id_curso, &id_autor, titulo, pergunta) == 5 &&
            buscar_curso_por_id(&sistema->cursos, id_curso) != NULL &&
            buscar_usuario_por_id(&sistema->usuarios, id_autor) != NULL) {
            inserir_topico(&sistema->topicos, id, id_curso, id_autor,
                           titulo, pergunta);
        }
    }
    fclose(arquivo);

    /* 9) Respostas: ligadas ao ID do topico */
    arquivo = abrir_arquivo(pasta, "respostas.txt", "r");
    if (arquivo == NULL) {
        return 0;
    }
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        linha[strcspn(linha, "\r\n")] = '\0';
        int id_topico = 0;
        int id = 0;
        int id_autor = 0;
        char texto[TAM_RESPOSTA];
        NoTopico *topico;

        if (sscanf(linha, "%d|%d|%d|%299[^|\n]",
                   &id_topico, &id, &id_autor, texto) == 4 &&
            buscar_usuario_por_id(&sistema->usuarios, id_autor) != NULL) {
            topico = buscar_topico_por_id(&sistema->topicos, id_topico);
            if (topico != NULL) {
                inserir_resposta(topico, id, id_autor, texto);
            }
        }
    }
    fclose(arquivo);

    return 1;
}

static int remover_concluida(void *alvo, int id_aula)
{
    return remover_aula_concluida((ListaConcluidas *)alvo, id_aula);
}

static int remover_historico(void *alvo, int id_aula)
{
    return remover_aula_historico((ListaHistorico *)alvo, id_aula);
}

static void limpar_referencias_de_aulas(Sistema *sistema,
                                        int (*coletar)(ListaCursos *, int,
                                                       int (*)(void *, int),
                                                       void *),
                                        int id)
{
    NoUsuario *usuario = sistema->usuarios.inicio;

    while (usuario != NULL) {
        coletar(&sistema->cursos, id, remover_concluida,
                &usuario->dados.concluidas);
        coletar(&sistema->cursos, id, remover_historico,
                &usuario->dados.historico);
        usuario = usuario->proximo;
    }
}

int excluir_curso_cascata(Sistema *sistema, int id_curso)
{
    NoCurso *curso;

    if (sistema == NULL) {
        return 0;
    }
    curso = buscar_curso_por_id(&sistema->cursos, id_curso);
    if (curso == NULL) {
        return 0;
    }
    limpar_referencias_de_aulas(sistema, remover_referencias_a_curso,
                                id_curso);
    remover_avaliacoes_por_curso(&sistema->avaliacoes, id_curso);
    remover_topicos_por_curso(&sistema->topicos, id_curso);
    return excluir_curso(&sistema->cursos, id_curso);
}

int excluir_usuario_cascata(Sistema *sistema, int id_usuario)
{
    if (sistema == NULL ||
        buscar_usuario_por_id(&sistema->usuarios, id_usuario) == NULL) {
        return 0;
    }
    remover_avaliacoes_por_aluno(&sistema->avaliacoes, id_usuario);
    remover_topicos_por_autor(&sistema->topicos, id_usuario);
    remover_respostas_por_autor(&sistema->topicos, id_usuario);
    remover_anotacoes_de_aluno(&sistema->cursos, id_usuario);
    return excluir_usuario(&sistema->usuarios, id_usuario);
}

int excluir_aula_cascata(Sistema *sistema, int id_aula)
{
    NoModulo *modulo;

    if (sistema == NULL) {
        return 0;
    }
    modulo = buscar_modulo_da_aula_wrapper(&sistema->cursos, id_aula);
    if (modulo == NULL) {
        return 0;
    }
    limpar_referencias_de_aulas(sistema, remover_referencias_a_aula, id_aula);
    return excluir_aula(modulo, id_aula);
}

int excluir_modulo_cascata(Sistema *sistema, int id_modulo)
{
    NoCurso *curso;
    NoModulo *modulo;

    if (sistema == NULL) {
        return 0;
    }
    curso = buscar_curso_do_modulo_wrapper(&sistema->cursos, id_modulo);
    if (curso == NULL) {
        return 0;
    }
    modulo = buscar_modulo_no_curso(curso, id_modulo);
    if (modulo == NULL) {
        return 0;
    }
    {
        NoAula *aula = modulo->aulas.inicio;
        while (aula != NULL) {
            limpar_referencias_de_aulas(sistema, remover_referencias_a_aula,
                                        aula->id);
            aula = aula->proximo;
        }
    }
    return excluir_modulo(curso, id_modulo);
}

int salvar_sistema(const Sistema *sistema, const char *pasta)
{
    FILE *arquivo;
    const NoUsuario *usuario;
    const NoCurso *curso;
    const NoModulo *modulo;
    const NoAula *aula;
    const NoAnotacao *anotacao;
    const NoAulaConcluida *concluida;
    const NoAvaliacao *avaliacao;
    const NoTopico *topico;
    const NoResposta *resposta;

    if (sistema == NULL) {
        return 0;
    }

    arquivo = abrir_arquivo(pasta, "usuarios.txt", "w");
    if (arquivo == NULL) {
        return 0;
    }
    usuario = sistema->usuarios.inicio;
    while (usuario != NULL) {
        fprintf(arquivo, "%d|%s|%s|%s|%d\n", usuario->dados.id,
                usuario->dados.nome, usuario->dados.email,
                usuario->dados.senha, usuario->dados.perfil);
        usuario = usuario->proximo;
    }
    fclose(arquivo);

    arquivo = abrir_arquivo(pasta, "cursos.txt", "w");
    if (arquivo == NULL) {
        return 0;
    }
    curso = sistema->cursos.inicio;
    while (curso != NULL) {
        fprintf(arquivo, "%d|%s|%s\n", curso->id, curso->titulo,
                curso->descricao);
        curso = curso->proximo;
    }
    fclose(arquivo);

    arquivo = abrir_arquivo(pasta, "modulos.txt", "w");
    if (arquivo == NULL) {
        return 0;
    }
    curso = sistema->cursos.inicio;
    while (curso != NULL) {
        modulo = curso->modulos.inicio;
        while (modulo != NULL) {
            fprintf(arquivo, "%d|%d|%s\n", curso->id, modulo->id,
                    modulo->titulo);
            modulo = modulo->proximo;
        }
        curso = curso->proximo;
    }
    fclose(arquivo);

    arquivo = abrir_arquivo(pasta, "aulas.txt", "w");
    if (arquivo == NULL) {
        return 0;
    }
    curso = sistema->cursos.inicio;
    while (curso != NULL) {
        modulo = curso->modulos.inicio;
        while (modulo != NULL) {
            aula = modulo->aulas.inicio;
            while (aula != NULL) {
                fprintf(arquivo, "%d|%d|%s|%d|%d\n", modulo->id, aula->id,
                        aula->titulo, aula->duracao_segundos,
                        aula->eh_aula_valida);
                aula = aula->proximo;
            }
            modulo = modulo->proximo;
        }
        curso = curso->proximo;
    }
    fclose(arquivo);

    arquivo = abrir_arquivo(pasta, "anotacoes.txt", "w");
    if (arquivo == NULL) {
        return 0;
    }
    curso = sistema->cursos.inicio;
    while (curso != NULL) {
        modulo = curso->modulos.inicio;
        while (modulo != NULL) {
            aula = modulo->aulas.inicio;
            while (aula != NULL) {
                anotacao = aula->anotacoes.inicio;
                while (anotacao != NULL) {
                    fprintf(arquivo, "%d|%d|%d|%d|%s\n", aula->id,
                            anotacao->dados.id, anotacao->dados.id_aluno,
                            anotacao->dados.timestamp_segundos,
                            anotacao->dados.texto);
                    anotacao = anotacao->proximo;
                }
                aula = aula->proximo;
            }
            modulo = modulo->proximo;
        }
        curso = curso->proximo;
    }
    fclose(arquivo);

    arquivo = abrir_arquivo(pasta, "concluidas.txt", "w");
    if (arquivo == NULL) {
        return 0;
    }
    usuario = sistema->usuarios.inicio;
    while (usuario != NULL) {
        concluida = usuario->dados.concluidas.inicio;
        while (concluida != NULL) {
            fprintf(arquivo, "%d|%d\n", usuario->dados.id,
                    concluida->id_aula);
            concluida = concluida->proximo;
        }
        usuario = usuario->proximo;
    }
    fclose(arquivo);

    arquivo = abrir_arquivo(pasta, "avaliacoes.txt", "w");
    if (arquivo == NULL) {
        return 0;
    }
    avaliacao = sistema->avaliacoes.inicio;
    while (avaliacao != NULL) {
        fprintf(arquivo, "%d|%d|%d|%s|%.2f\n", avaliacao->dados.id,
                avaliacao->dados.id_curso, avaliacao->dados.id_aluno,
                avaliacao->dados.titulo_atividade, avaliacao->dados.nota);
        avaliacao = avaliacao->proximo;
    }
    fclose(arquivo);

    arquivo = abrir_arquivo(pasta, "topicos.txt", "w");
    if (arquivo == NULL) {
        return 0;
    }
    topico = sistema->topicos.inicio;
    while (topico != NULL) {
        fprintf(arquivo, "%d|%d|%d|%s|%s\n", topico->id, topico->id_curso,
                topico->id_autor, topico->titulo, topico->pergunta);
        topico = topico->proximo;
    }
    fclose(arquivo);

    arquivo = abrir_arquivo(pasta, "respostas.txt", "w");
    if (arquivo == NULL) {
        return 0;
    }
    topico = sistema->topicos.inicio;
    while (topico != NULL) {
        resposta = topico->respostas.inicio;
        while (resposta != NULL) {
            fprintf(arquivo, "%d|%d|%d|%s\n", topico->id, resposta->id,
                    resposta->id_autor, resposta->texto);
            resposta = resposta->proximo;
        }
        topico = topico->proximo;
    }
    fclose(arquivo);

    return 1;
}

/* RF04: caderno em Markdown com apenas as anotacoes do aluno logado. */
int exportar_caderno(const Sistema *sistema, int id_aluno,
                     const char *arquivo)
{
    FILE *saida;
    const NoCurso *curso;
    const NoModulo *modulo;
    const NoAula *aula;
    const NoAnotacao *anotacao;
    char horario[TAM_MAX_HORARIO];
    int encontradas = 0;

    if (sistema == NULL || arquivo == NULL) {
        return -1;
    }
    saida = fopen(arquivo, "w");
    if (saida == NULL) {
        return -1;
    }

    fprintf(saida, "# Caderno de estudos - aluno %d\n\n", id_aluno);
    curso = sistema->cursos.inicio;
    while (curso != NULL) {
        modulo = curso->modulos.inicio;
        while (modulo != NULL) {
            aula = modulo->aulas.inicio;
            while (aula != NULL) {
                anotacao = aula->anotacoes.inicio;
                while (anotacao != NULL) {
                    if (anotacao->dados.id_aluno == id_aluno) {
                        segundos_para_horario(
                            anotacao->dados.timestamp_segundos,
                            horario, sizeof(horario));
                        fprintf(saida, "## %s > %s > %s\n",
                                curso->titulo, modulo->titulo, aula->titulo);
                        fprintf(saida, "- [%s] %s\n\n", horario,
                                anotacao->dados.texto);
                        encontradas++;
                    }
                    anotacao = anotacao->proximo;
                }
                aula = aula->proximo;
            }
            modulo = modulo->proximo;
        }
        curso = curso->proximo;
    }
    fclose(saida);
    return encontradas;
}
