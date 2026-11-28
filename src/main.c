/*
 * Plataforma de Ensino a Distancia (EaD) - projeto academico de AED1.
 * Toda entrada e saida de terminal fica neste arquivo (e em utils.c).
 * As TADs apenas manipulam estruturas de dados.
 */

#include "avaliacao.h"
#include "curso.h"
#include "forum.h"
#include "persistencia.h"
#include "usuario.h"
#include "utils.h"

#include <stdio.h>
#include <string.h>

#define PASTA_DADOS "data"

/* ---------- Auxiliares de apresentacao ---------- */

static void salvar_alteracoes(const Sistema *sistema)
{
    if (!salvar_sistema(sistema, PASTA_DADOS)) {
        printf("Aviso: nao foi possivel gravar os arquivos em %s/\n",
               PASTA_DADOS);
    }
}

static const char *nome_perfil(int perfil)
{
    if (perfil == PERFIL_DOCENTE) {
        return "Docente";
    }
    return "Discente";
}

static const NoUsuario *buscar_usuario_const(const ListaUsuarios *usuarios,
                                             int id)
{
    const NoUsuario *atual = usuarios->inicio;

    while (atual != NULL) {
        if (atual->dados.id == id) {
            return atual;
        }
        atual = atual->proximo;
    }
    return NULL;
}

static const NoAula *buscar_aula_const(const ListaCursos *cursos, int id)
{
    const NoCurso *curso = cursos->inicio;

    while (curso != NULL) {
        const NoModulo *modulo = curso->modulos.inicio;
        while (modulo != NULL) {
            const NoAula *aula = modulo->aulas.inicio;
            while (aula != NULL) {
                if (aula->id == id) {
                    return aula;
                }
                aula = aula->proximo;
            }
            modulo = modulo->proximo;
        }
        curso = curso->proximo;
    }
    return NULL;
}

static void imprimir_cursos(const ListaCursos *cursos)
{
    const NoCurso *curso = cursos->inicio;
    char horario[TAM_MAX_HORARIO];

    if (cursos->tamanho == 0) {
        printf("\nNenhum curso cadastrado.\n");
        return;
    }

    while (curso != NULL) {
        const NoModulo *modulo = curso->modulos.inicio;
        printf("\nCurso %d: %s\n  %s\n", curso->id, curso->titulo,
               curso->descricao);
        while (modulo != NULL) {
            const NoAula *aula = modulo->aulas.inicio;
            printf("  Modulo %d: %s\n", modulo->id, modulo->titulo);
            while (aula != NULL) {
                segundos_para_horario(aula->duracao_segundos, horario,
                                      sizeof(horario));
                printf("    Aula %d: %s (%s) [%s]\n", aula->id, aula->titulo,
                       horario, aula->eh_aula_valida ? "aula" : "material");
                aula = aula->proximo;
            }
            modulo = modulo->proximo;
        }
        curso = curso->proximo;
    }
}

/* filtrar = 1 mostra somente anotacoes do aluno informado. */
static void imprimir_anotacoes(const NoAula *aula, int id_aluno, int filtrar)
{
    const NoAnotacao *anotacao = aula->anotacoes.inicio;
    char horario[TAM_MAX_HORARIO];

    if (aula->anotacoes.tamanho == 0) {
        printf("    Nenhuma anotacao nesta aula.\n");
        return;
    }
    while (anotacao != NULL) {
        if (!filtrar || anotacao->dados.id_aluno == id_aluno) {
            segundos_para_horario(anotacao->dados.timestamp_segundos,
                                  horario, sizeof(horario));
            printf("    [%s] id %d | aluno %d | %s\n", horario,
                   anotacao->dados.id, anotacao->dados.id_aluno,
                   anotacao->dados.texto);
        }
        anotacao = anotacao->proximo;
    }
}

static void imprimir_ranking(const Sistema *sistema, int id_curso)
{
    const NoAvaliacao *avaliacao = sistema->avaliacoes.inicio;
    int posicao = 0;

    printf("\nRanking de desempenho:\n");
    while (avaliacao != NULL) {
        if (id_curso == 0 || avaliacao->dados.id_curso == id_curso) {
            const NoUsuario *aluno = buscar_usuario_const(
                &sistema->usuarios, avaliacao->dados.id_aluno);
            posicao++;
            printf("  %d) %s (aluno %d) - %s: %.2f\n", posicao,
                   aluno != NULL ? aluno->dados.nome : "(aluno removido)",
                   avaliacao->dados.id_aluno,
                   avaliacao->dados.titulo_atividade,
                   avaliacao->dados.nota);
        }
        avaliacao = avaliacao->proximo;
    }
    if (posicao == 0) {
        printf("  Nenhuma avaliacao encontrada.\n");
    }
}

static void ver_ranking(const Sistema *sistema)
{
    int id_curso;

    if (!ler_inteiro("ID do curso (0 = todos): ", &id_curso)) {
        return;
    }
    imprimir_ranking(sistema, id_curso);
}

static void listar_usuarios(const Sistema *sistema)
{
    const NoUsuario *atual = sistema->usuarios.inicio;

    printf("\nUsuarios cadastrados (%d):\n", sistema->usuarios.tamanho);
    while (atual != NULL) {
        printf("  %d | %s | %s | %s\n", atual->dados.id, atual->dados.nome,
               atual->dados.email, nome_perfil(atual->dados.perfil));
        atual = atual->proximo;
    }
}

/* ---------- Operacoes do docente ---------- */

static void gerenciar_curricular(Sistema *sistema)
{
    char titulo[TAM_TITULO];
    char descricao[TAM_DESCRICAO];
    char duracao[TAM_MAX_HORARIO];
    int opcao;
    int id;
    int id_pai;
    int inteiro;

    for (;;) {
        printf("\n--- Gestao curricular (docente) ---\n");
        printf(" 1) Listar grade curricular\n");
        printf(" 2) Novo curso\n");
        printf(" 3) Editar curso\n");
        printf(" 4) Excluir curso (cascata)\n");
        printf(" 5) Novo modulo\n");
        printf(" 6) Editar modulo\n");
        printf(" 7) Excluir modulo (cascata)\n");
        printf(" 8) Nova aula\n");
        printf(" 9) Editar aula\n");
        printf("10) Excluir aula (cascata)\n");
        printf(" 0) Voltar\n");
        if (!ler_inteiro("Opcao: ", &opcao)) {
            return;
        }

        switch (opcao) {
            case 1: {
                imprimir_cursos(&sistema->cursos);
                break;
            }
            case 2: {
                if (!ler_texto("Titulo do curso: ", titulo, sizeof(titulo))) {
                    return;
                }
                if (!ler_texto("Descricao: ", descricao, sizeof(descricao))) {
                    return;
                }
                if (inserir_curso(&sistema->cursos,
                                  gerar_id_curso(&sistema->cursos),
                                  titulo, descricao) != NULL) {
                    printf("Curso criado.\n");
                    salvar_alteracoes(sistema);
                } else {
                    printf("Falta de memoria.\n");
                }
                break;
            }
            case 3: {
                NoCurso *curso;
                if (!ler_inteiro("ID do curso: ", &id)) {
                    return;
                }
                curso = buscar_curso_por_id(&sistema->cursos, id);
                if (curso == NULL) {
                    printf("Curso nao encontrado.\n");
                    break;
                }
                if (!ler_texto("Novo titulo: ", titulo, sizeof(titulo))) {
                    return;
                }
                if (!ler_texto("Nova descricao: ", descricao,
                               sizeof(descricao))) {
                    return;
                }
                editar_curso(curso, titulo, descricao);
                printf("Curso atualizado.\n");
                salvar_alteracoes(sistema);
                break;
            }
            case 4: {
                if (!ler_inteiro("ID do curso a excluir: ", &id)) {
                    return;
                }
                if (excluir_curso_cascata(sistema, id)) {
                    printf("Curso e filhos excluidos.\n");
                    salvar_alteracoes(sistema);
                } else {
                    printf("Curso nao encontrado.\n");
                }
                break;
            }
            case 5: {
                NoCurso *curso;
                if (!ler_inteiro("ID do curso: ", &id_pai)) {
                    return;
                }
                curso = buscar_curso_por_id(&sistema->cursos, id_pai);
                if (curso == NULL) {
                    printf("Curso nao encontrado.\n");
                    break;
                }
                if (!ler_texto("Titulo do modulo: ", titulo, sizeof(titulo))) {
                    return;
                }
                if (inserir_modulo(curso, gerar_id_modulo(&sistema->cursos),
                                   titulo) != NULL) {
                    printf("Modulo criado.\n");
                    salvar_alteracoes(sistema);
                } else {
                    printf("Falta de memoria.\n");
                }
                break;
            }
            case 6: {
                NoModulo *modulo;
                if (!ler_inteiro("ID do modulo: ", &id)) {
                    return;
                }
                modulo = buscar_modulo_por_id(&sistema->cursos, id);
                if (modulo == NULL) {
                    printf("Modulo nao encontrado.\n");
                    break;
                }
                if (!ler_texto("Novo titulo: ", titulo, sizeof(titulo))) {
                    return;
                }
                editar_modulo(modulo, titulo);
                printf("Modulo atualizado.\n");
                salvar_alteracoes(sistema);
                break;
            }
            case 7: {
                if (!ler_inteiro("ID do modulo a excluir: ", &id)) {
                    return;
                }
                if (excluir_modulo_cascata(sistema, id)) {
                    printf("Modulo e aulas excluidos.\n");
                    salvar_alteracoes(sistema);
                }
                break;
            }
            case 8: {
                NoModulo *modulo;
                if (!ler_inteiro("ID do modulo: ", &id_pai)) {
                    return;
                }
                modulo = buscar_modulo_por_id(&sistema->cursos, id_pai);
                if (modulo == NULL) {
                    printf("Modulo nao encontrado.\n");
                    break;
                }
                if (!ler_texto("Titulo da aula: ", titulo, sizeof(titulo))) {
                    return;
                }
                if (!ler_horario("Duracao HH:MM:SS: ", duracao,
                                 sizeof(duracao))) {
                    return;
                }
                if (!ler_inteiro("1 = aula valida, 0 = material: ",
                                 &inteiro)) {
                    return;
                }
                if (inteiro != 0 && inteiro != 1) {
                    printf("Valor invalido. Use 0 ou 1.\n");
                    break;
                }
                if (inserir_aula(modulo, gerar_id_aula(&sistema->cursos),
                                 titulo, horario_para_segundos(duracao),
                                 inteiro) != NULL) {
                    printf("Aula criada.\n");
                    salvar_alteracoes(sistema);
                } else {
                    printf("Falta de memoria.\n");
                }
                break;
            }
            case 9: {
                NoAula *aula;
                if (!ler_inteiro("ID da aula: ", &id)) {
                    return;
                }
                aula = buscar_aula_por_id(&sistema->cursos, id);
                if (aula == NULL) {
                    printf("Aula nao encontrada.\n");
                    break;
                }
                if (!ler_texto("Novo titulo: ", titulo, sizeof(titulo))) {
                    return;
                }
                if (!ler_horario("Nova duracao HH:MM:SS: ", duracao,
                                 sizeof(duracao))) {
                    return;
                }
                if (!ler_inteiro("1 = aula valida, 0 = material: ",
                                 &inteiro)) {
                    return;
                }
                if (inteiro != 0 && inteiro != 1) {
                    printf("Valor invalido. Use 0 ou 1.\n");
                    break;
                }
                editar_aula(aula, titulo, horario_para_segundos(duracao),
                            inteiro);
                printf("Aula atualizada.\n");
                salvar_alteracoes(sistema);
                break;
            }
            case 10: {
                if (!ler_inteiro("ID da aula a excluir: ", &id)) {
                    return;
                }
                if (excluir_aula_cascata(sistema, id)) {
                    printf("Aula e anotacoes excluidas.\n");
                    salvar_alteracoes(sistema);
                }
                break;
            }
            case 0: {
                return;
            }
            default: {
                printf("Opcao invalida.\n");
                break;
            }
        }
    }
}

static void lancar_avaliacao(Sistema *sistema)
{
    char titulo[TAM_ATIVIDADE];
    float nota;
    int id_curso;
    int id_aluno;
    NoAvaliacao *nova;

    if (!ler_inteiro("ID do curso: ", &id_curso)) {
        return;
    }
    if (!ler_inteiro("ID do aluno: ", &id_aluno)) {
        return;
    }
    if (!ler_texto("Titulo da atividade: ", titulo, sizeof(titulo))) {
        return;
    }
    if (!ler_decimal("Nota (0 a 10): ", &nota)) {
        return;
    }
    if (buscar_curso_por_id(&sistema->cursos, id_curso) == NULL ||
        buscar_usuario_por_id(&sistema->usuarios, id_aluno) == NULL) {
        printf("Curso ou aluno inexistente.\n");
        return;
    }
    if (nota < 0.0f || nota > 10.0f) {
        printf("Nota fora do intervalo permitido.\n");
        return;
    }

    nova = inserir_avaliacao(&sistema->avaliacoes,
                             gerar_id_avaliacao(&sistema->avaliacoes),
                             id_curso, id_aluno, titulo, nota);
    if (nova != NULL) {
        printf("Nota lancada (id %d).\n", nova->dados.id);
        salvar_alteracoes(sistema);
    } else {
        printf("Falta de memoria.\n");
    }
}

static void cadastrar_usuario(Sistema *sistema)
{
    char nome[TAM_NOME];
    char email[TAM_EMAIL];
    char senha[TAM_SENHA];
    int perfil;
    NoUsuario *novo;

    if (!ler_texto("Nome: ", nome, sizeof(nome))) {
        return;
    }
    if (!ler_texto("Email: ", email, sizeof(email))) {
        return;
    }
    if (!ler_texto("Senha: ", senha, sizeof(senha))) {
        return;
    }
    if (!ler_inteiro("Perfil (1 docente, 2 discente): ", &perfil)) {
        return;
    }
    if (perfil != PERFIL_DOCENTE && perfil != PERFIL_DISCENTE) {
        printf("Perfil invalido.\n");
        return;
    }
    if (buscar_usuario_por_email(&sistema->usuarios, email) != NULL) {
        printf("Email ja cadastrado.\n");
        return;
    }

    novo = inserir_usuario(&sistema->usuarios,
                           gerar_id_usuario(&sistema->usuarios),
                           nome, email, senha, perfil);
    if (novo != NULL) {
        printf("Usuario %d criado.\n", novo->dados.id);
        salvar_alteracoes(sistema);
    } else {
        printf("Falta de memoria.\n");
    }
}

static void remover_usuario(Sistema *sistema, const NoUsuario *docente)
{
    int id;

    if (!ler_inteiro("ID do usuario a excluir: ", &id)) {
        return;
    }
    if (id == docente->dados.id) {
        printf("Voce nao pode excluir o proprio login durante a sessao.\n");
        return;
    }
                if (excluir_usuario_cascata(sistema, id)) {
        printf("Usuario removido.\n");
        salvar_alteracoes(sistema);
    } else {
        printf("Usuario nao encontrado.\n");
    }
}

static void menu_forum(Sistema *sistema, const NoUsuario *usuario)
{
    char titulo[TAM_TITULO_TOPICO];
    char texto[TAM_PERGUNTA];
    int opcao;
    int id;
    int id_curso;
    NoTopico *topico;
    const NoTopico *atual;
    const NoResposta *resposta;

    for (;;) {
        printf("\n--- Forum colaborativo ---\n");
        printf("1) Listar topicos e respostas\n");
        printf("2) Criar topico\n");
        printf("3) Responder topico\n");
        printf("4) Excluir topico\n");
        printf("0) Voltar\n");
        if (!ler_inteiro("Opcao: ", &opcao)) {
            return;
        }

        switch (opcao) {
            case 1: {
                atual = sistema->topicos.inicio;
                if (sistema->topicos.tamanho == 0) {
                    printf("Nenhum topico aberto.\n");
                    break;
                }
                while (atual != NULL) {
                    printf("\nTopico %d (curso %d): %s\n", atual->id,
                           atual->id_curso, atual->titulo);
                    printf("  Pergunta: %s\n", atual->pergunta);
                    resposta = atual->respostas.inicio;
                    while (resposta != NULL) {
                        printf("  Resposta %d (autor %d): %s\n",
                               resposta->id, resposta->id_autor,
                               resposta->texto);
                        resposta = resposta->proximo;
                    }
                    atual = atual->proximo;
                }
                break;
            }
            case 2: {
                if (!ler_inteiro("ID do curso: ", &id_curso)) {
                    return;
                }
                if (buscar_curso_por_id(&sistema->cursos, id_curso) == NULL) {
                    printf("Curso nao encontrado.\n");
                    break;
                }
                if (!ler_texto("Titulo do topico: ", titulo, sizeof(titulo))) {
                    return;
                }
                if (!ler_texto("Pergunta: ", texto, sizeof(texto))) {
                    return;
                }
                if (inserir_topico(&sistema->topicos,
                                   gerar_id_topico(&sistema->topicos),
                                   id_curso, usuario->dados.id,
                                   titulo, texto) != NULL) {
                    printf("Topico criado.\n");
                    salvar_alteracoes(sistema);
                } else {
                    printf("Falta de memoria.\n");
                }
                break;
            }
            case 3: {
                if (!ler_inteiro("ID do topico: ", &id)) {
                    return;
                }
                topico = buscar_topico_por_id(&sistema->topicos, id);
                if (topico == NULL) {
                    printf("Topico nao encontrado.\n");
                    break;
                }
                if (!ler_texto("Resposta: ", texto, sizeof(texto))) {
                    return;
                }
                if (inserir_resposta(topico,
                                     gerar_id_resposta(&sistema->topicos),
                                     usuario->dados.id, texto) != NULL) {
                    printf("Resposta publicada.\n");
                    salvar_alteracoes(sistema);
                } else {
                    printf("Falta de memoria.\n");
                }
                break;
            }
            case 4: {
                if (!ler_inteiro("ID do topico a excluir: ", &id)) {
                    return;
                }
                topico = buscar_topico_por_id(&sistema->topicos, id);
                if (topico == NULL) {
                    printf("Topico nao encontrado.\n");
                    break;
                }
                if (usuario->dados.perfil != PERFIL_DOCENTE &&
                    topico->id_autor != usuario->dados.id) {
                    printf("Somente o autor ou um docente pode excluir.\n");
                    break;
                }
                if (excluir_topico(&sistema->topicos, id)) {
                    printf("Topico excluido.\n");
                    salvar_alteracoes(sistema);
                }
                break;
            }
            case 0: {
                return;
            }
            default: {
                printf("Opcao invalida.\n");
                break;
            }
        }
    }
}

static void menu_docente(Sistema *sistema, NoUsuario *docente)
{
    int opcao;

    printf("\nBem-vindo, %s (docente).\n", docente->dados.nome);
    for (;;) {
        printf("\n===== MENU DOCENTE =====\n");
        printf(" 1) Gerenciar grade curricular\n");
        printf(" 2) Lancar nota de avaliacao\n");
        printf(" 3) Ver ranking da turma\n");
        printf(" 4) Forum\n");
        printf(" 5) Cadastrar usuario\n");
        printf(" 6) Excluir usuario\n");
        printf(" 7) Listar usuarios\n");
        printf(" 0) Sair do perfil\n");
        if (!ler_inteiro("Opcao: ", &opcao)) {
            return;
        }

        switch (opcao) {
            case 1: {
                gerenciar_curricular(sistema);
                break;
            }
            case 2: {
                lancar_avaliacao(sistema);
                break;
            }
            case 3: {
                ver_ranking(sistema);
                break;
            }
            case 4: {
                menu_forum(sistema, docente);
                break;
            }
            case 5: {
                cadastrar_usuario(sistema);
                break;
            }
            case 6: {
                remover_usuario(sistema, docente);
                break;
            }
            case 7: {
                listar_usuarios(sistema);
                break;
            }
            case 0: {
                return;
            }
            default: {
                printf("Opcao invalida.\n");
                break;
            }
        }
    }
}

/* ---------- Operacoes do discente ---------- */

static void acessar_aula(Sistema *sistema, NoUsuario *aluno)
{
    int id_aula;
    NoAula *aula;
    char horario[TAM_MAX_HORARIO];

    imprimir_cursos(&sistema->cursos);
    if (!ler_inteiro("\nID da aula para acessar (0 = voltar): ", &id_aula)) {
        return;
    }
    if (id_aula == 0) {
        return;
    }

    aula = buscar_aula_por_id(&sistema->cursos, id_aula);
    if (aula == NULL) {
        printf("Aula nao encontrada.\n");
        return;
    }

    registrar_historico(&aluno->dados.historico, id_aula);
    segundos_para_horario(aula->duracao_segundos, horario, sizeof(horario));
    printf("\nAula %d: %s (%s)\n", aula->id, aula->titulo, horario);
    printf("Suas anotacoes nesta aula, em ordem cronologica:\n");
    imprimir_anotacoes(aula, aluno->dados.id, 1);
    salvar_alteracoes(sistema);
}

static void marcar_concluida(Sistema *sistema, NoUsuario *aluno)
{
    int id_aula;
    NoAula *aula;

    if (!ler_inteiro("ID da aula concluida: ", &id_aula)) {
        return;
    }
    aula = buscar_aula_por_id(&sistema->cursos, id_aula);
    if (aula == NULL) {
        printf("Aula nao encontrada.\n");
        return;
    }
    if (marcar_aula_concluida(&aluno->dados.concluidas, id_aula)) {
        printf("Aula marcada como concluida.\n");
        salvar_alteracoes(sistema);
    } else {
        printf("Essa aula ja estava concluida.\n");
    }
}

static void menu_anotacoes(Sistema *sistema, NoUsuario *aluno)
{
    char horario[TAM_MAX_HORARIO];
    char texto[TAM_TEXTO_ANOTACAO];
    int opcao;
    int id;
    int id_aula;
    int id_anotacao;
    NoAula *aula;
    NoAnotacao *anotacao;

    for (;;) {
        printf("\n--- Minhas anotacoes ---\n");
        printf("1) Criar anotacao\n");
        printf("2) Editar anotacao\n");
        printf("3) Listar anotacoes de uma aula\n");
        printf("4) Remover anotacao\n");
        printf("0) Voltar\n");
        if (!ler_inteiro("Opcao: ", &opcao)) {
            return;
        }

        switch (opcao) {
            case 1: {
                if (!ler_inteiro("ID da aula: ", &id_aula)) {
                    return;
                }
                aula = buscar_aula_por_id(&sistema->cursos, id_aula);
                if (aula == NULL) {
                    printf("Aula nao encontrada.\n");
                    break;
                }
                if (!ler_horario("Timestamp HH:MM:SS: ", horario,
                                 sizeof(horario))) {
                    return;
                }
                if (!ler_texto("Texto da anotacao: ", texto,
                               sizeof(texto))) {
                    return;
                }
                id = gerar_id_anotacao(&aula->anotacoes);
                if (inserir_anotacao(&aula->anotacoes, id, aluno->dados.id,
                                     horario_para_segundos(horario),
                                     texto) != NULL) {
                    printf("Anotacao %d criada.\n", id);
                    salvar_alteracoes(sistema);
                } else {
                    printf("Falta de memoria.\n");
                }
                break;
            }
            case 2: {
                if (!ler_inteiro("ID da aula: ", &id_aula)) {
                    return;
                }
                aula = buscar_aula_por_id(&sistema->cursos, id_aula);
                if (aula == NULL) {
                    printf("Aula nao encontrada.\n");
                    break;
                }
                imprimir_anotacoes(aula, aluno->dados.id, 1);
                if (!ler_inteiro("ID da anotacao a editar: ", &id_anotacao)) {
                    return;
                }
                anotacao = buscar_anotacao_por_id(&aula->anotacoes,
                                                  id_anotacao);
                if (anotacao == NULL ||
                    anotacao->dados.id_aluno != aluno->dados.id) {
                    printf("Anotacao nao encontrada para este aluno.\n");
                    break;
                }
                if (!ler_horario("Novo timestamp HH:MM:SS: ", horario,
                                 sizeof(horario))) {
                    return;
                }
                if (!ler_texto("Novo texto: ", texto, sizeof(texto))) {
                    return;
                }
                if (editar_anotacao(&aula->anotacoes, id_anotacao,
                                    horario_para_segundos(horario), texto)) {
                    printf("Anotacao atualizada e reordenada.\n");
                    salvar_alteracoes(sistema);
                }
                break;
            }
            case 3: {
                if (!ler_inteiro("ID da aula: ", &id_aula)) {
                    return;
                }
                aula = buscar_aula_por_id(&sistema->cursos, id_aula);
                if (aula == NULL) {
                    printf("Aula nao encontrada.\n");
                    break;
                }
                printf("Anotacoes de %s em ordem cronologica:\n",
                       aula->titulo);
                imprimir_anotacoes(aula, aluno->dados.id, 1);
                break;
            }
            case 4: {
                if (!ler_inteiro("ID da aula: ", &id_aula)) {
                    return;
                }
                aula = buscar_aula_por_id(&sistema->cursos, id_aula);
                if (aula == NULL) {
                    printf("Aula nao encontrada.\n");
                    break;
                }
                imprimir_anotacoes(aula, aluno->dados.id, 1);
                if (!ler_inteiro("ID da anotacao a remover: ",
                                 &id_anotacao)) {
                    return;
                }
                anotacao = buscar_anotacao_por_id(&aula->anotacoes,
                                                  id_anotacao);
                if (anotacao == NULL ||
                    anotacao->dados.id_aluno != aluno->dados.id) {
                    printf("Anotacao nao encontrada para este aluno.\n");
                    break;
                }
                if (remover_anotacao(&aula->anotacoes, id_anotacao)) {
                    printf("Anotacao removida.\n");
                    salvar_alteracoes(sistema);
                }
                break;
            }
            case 0: {
                return;
            }
            default: {
                printf("Opcao invalida.\n");
                break;
            }
        }
    }
}

static void ver_progresso(const Sistema *sistema, const NoUsuario *aluno)
{
    const NoCurso *curso = sistema->cursos.inicio;
    int total = 0;
    int feitas = 0;

    calcular_progresso_curso(&sistema->cursos, &aluno->dados.concluidas,
                             &total, &feitas);
    printf("\nProgresso global de %s: %d/%d aulas validas (%.1f%%)\n",
           aluno->dados.nome, feitas, total,
           total == 0 ? 0.0 : (double)feitas * 100.0 / total);

    while (curso != NULL) {
        const NoModulo *modulo = curso->modulos.inicio;
        printf("  Curso %d - %s\n", curso->id, curso->titulo);
        while (modulo != NULL) {
            int modulo_total = 0;
            int modulo_feitas = 0;
            calcular_progresso_modulo(modulo, &aluno->dados.concluidas,
                                      &modulo_total, &modulo_feitas);
            printf("    Modulo %d (%s): %d/%d (%.1f%%)\n", modulo->id,
                   modulo->titulo, modulo_feitas, modulo_total,
                   modulo_total == 0
                       ? 0.0
                       : (double)modulo_feitas * 100.0 / modulo_total);
            modulo = modulo->proximo;
        }
        curso = curso->proximo;
    }
}

static void exportar_caderno_aluno(const Sistema *sistema,
                                   const NoUsuario *aluno)
{
    char arquivo[80];
    int quantidade;

    snprintf(arquivo, sizeof(arquivo), "caderno_aluno_%d.md",
             aluno->dados.id);
    quantidade = exportar_caderno(sistema, aluno->dados.id, arquivo);
    if (quantidade < 0) {
        printf("Falha ao criar o arquivo %s.\n", arquivo);
    } else {
        printf("Caderno gerado: %s (%d anotacoes).\n", arquivo, quantidade);
    }
}

static void ver_historico(const Sistema *sistema, const NoUsuario *aluno)
{
    const NoHistorico *item = aluno->dados.historico.inicio;

    printf("\nHistorico recente (%d/%d):\n", aluno->dados.historico.tamanho,
           MAX_HISTORICO);
    while (item != NULL) {
        const NoAula *aula = buscar_aula_const(&sistema->cursos,
                                               item->id_aula);
        printf("  Aula %d: %s\n", item->id_aula,
               aula != NULL ? aula->titulo : "(aula removida)");
        item = item->proximo;
    }
}

static void buscar_aula_e_anotacoes(const Sistema *sistema,
                                    const NoUsuario *aluno)
{
    char chave[TAM_TITULO];
    char horario[TAM_MAX_HORARIO];
    const NoCurso *curso;
    const NoModulo *modulo;
    const NoAula *aula;
    const NoAnotacao *anotacao;
    int opcao;
    int id;
    int encontrados;

    for (;;) {
        printf("\n--- Busca linear filtrada ---\n");
        printf("1) Buscar aula por ID\n");
        printf("2) Buscar palavra no titulo das aulas\n");
        printf("3) Buscar palavra nas minhas anotacoes\n");
        printf("0) Voltar\n");
        if (!ler_inteiro("Opcao: ", &opcao)) {
            return;
        }

        switch (opcao) {
            case 1: {
                if (!ler_inteiro("ID da aula: ", &id)) {
                    return;
                }
                aula = buscar_aula_const(&sistema->cursos, id);
                if (aula == NULL) {
                    printf("Aula nao encontrada.\n");
                    break;
                }
                segundos_para_horario(aula->duracao_segundos, horario,
                                      sizeof(horario));
                printf("Aula %d: %s (%s)\n", aula->id, aula->titulo,
                       horario);
                imprimir_anotacoes(aula, aluno->dados.id, 1);
                break;
            }
            case 2: {
                if (!ler_texto("Palavra-chave: ", chave, sizeof(chave))) {
                    return;
                }
                encontrados = 0;
                curso = sistema->cursos.inicio;
                while (curso != NULL) {
                    modulo = curso->modulos.inicio;
                    while (modulo != NULL) {
                        aula = modulo->aulas.inicio;
                        while (aula != NULL) {
                            if (strstr(aula->titulo, chave) != NULL) {
                                printf("Aula %d: %s (curso %d, modulo %d)\n",
                                       aula->id, aula->titulo, curso->id,
                                       modulo->id);
                                encontrados++;
                            }
                            aula = aula->proximo;
                        }
                        modulo = modulo->proximo;
                    }
                    curso = curso->proximo;
                }
                if (encontrados == 0) {
                    printf("Nenhum titulo contem essa palavra.\n");
                }
                break;
            }
            case 3: {
                if (!ler_texto("Palavra-chave: ", chave, sizeof(chave))) {
                    return;
                }
                encontrados = 0;
                curso = sistema->cursos.inicio;
                while (curso != NULL) {
                    modulo = curso->modulos.inicio;
                    while (modulo != NULL) {
                        aula = modulo->aulas.inicio;
                        while (aula != NULL) {
                            anotacao = aula->anotacoes.inicio;
                            while (anotacao != NULL) {
                                if (anotacao->dados.id_aluno ==
                                        aluno->dados.id &&
                                    strstr(anotacao->dados.texto, chave) !=
                                        NULL) {
                                    segundos_para_horario(
                                        anotacao->dados.timestamp_segundos,
                                        horario, sizeof(horario));
                                    printf("Aula %d [%s]: %s\n", aula->id,
                                           horario, anotacao->dados.texto);
                                    encontrados++;
                                }
                                anotacao = anotacao->proximo;
                            }
                            aula = aula->proximo;
                        }
                        modulo = modulo->proximo;
                    }
                    curso = curso->proximo;
                }
                if (encontrados == 0) {
                    printf("Nenhuma anotacao sua contem essa palavra.\n");
                }
                break;
            }
            case 0: {
                return;
            }
            default: {
                printf("Opcao invalida.\n");
                break;
            }
        }
    }
}

static void menu_discente(Sistema *sistema, NoUsuario *aluno)
{
    int opcao;

    printf("\nBem-vindo, %s (discente).\n", aluno->dados.nome);
    for (;;) {
        printf("\n===== MENU DISCENTE =====\n");
        printf(" 1) Ver grade e acessar aula\n");
        printf(" 2) Marcar aula como concluida\n");
        printf(" 3) Minhas anotacoes\n");
        printf(" 4) Meu progresso academico\n");
        printf(" 5) Exportar caderno de estudos\n");
        printf(" 6) Ver ranking da turma\n");
        printf(" 7) Forum\n");
        printf(" 8) Buscar aula ou anotacao\n");
        printf(" 9) Meu historico recente\n");
        printf(" 0) Sair do perfil\n");
        if (!ler_inteiro("Opcao: ", &opcao)) {
            return;
        }

        switch (opcao) {
            case 1: {
                acessar_aula(sistema, aluno);
                break;
            }
            case 2: {
                marcar_concluida(sistema, aluno);
                break;
            }
            case 3: {
                menu_anotacoes(sistema, aluno);
                break;
            }
            case 4: {
                ver_progresso(sistema, aluno);
                break;
            }
            case 5: {
                exportar_caderno_aluno(sistema, aluno);
                break;
            }
            case 6: {
                ver_ranking(sistema);
                break;
            }
            case 7: {
                menu_forum(sistema, aluno);
                break;
            }
            case 8: {
                buscar_aula_e_anotacoes(sistema, aluno);
                break;
            }
            case 9: {
                ver_historico(sistema, aluno);
                break;
            }
            case 0: {
                return;
            }
            default: {
                printf("Opcao invalida.\n");
                break;
            }
        }
    }
}

/* ---------- Programa principal ---------- */

int main(void)
{
    Sistema sistema;
    NoUsuario *usuario;
    char email[TAM_EMAIL];
    char senha[TAM_SENHA];
    int opcao;

    iniciar_sistema(&sistema);
    if (!carregar_sistema(&sistema, PASTA_DADOS)) {
        printf("Erro ao carregar os arquivos da pasta %s/\n", PASTA_DADOS);
        liberar_sistema(&sistema);
        return 1;
    }
    printf("Sistema carregado: %d usuario(s), %d curso(s), %d avaliacao(oes), "
           "%d topico(s).\n", sistema.usuarios.tamanho,
           sistema.cursos.tamanho, sistema.avaliacoes.tamanho,
           sistema.topicos.tamanho);

    for (;;) {
        printf("\n===== PLATAFORMA EaD =====\n");
        printf("1) Login\n");
        printf("0) Sair e salvar\n");
        if (!ler_inteiro("Opcao: ", &opcao)) {
            break;
        }
        if (opcao == 0) {
            break;
        }
        if (opcao != 1) {
            printf("Opcao invalida.\n");
            continue;
        }

        if (!ler_texto("Email: ", email, sizeof(email))) {
            break;
        }
        if (!ler_texto("Senha: ", senha, sizeof(senha))) {
            break;
        }
        usuario = autenticar_usuario(&sistema.usuarios, email, senha);
        if (usuario == NULL) {
            printf("Login invalido.\n");
            continue;
        }

        if (usuario->dados.perfil == PERFIL_DOCENTE) {
            menu_docente(&sistema, usuario);
        } else {
            menu_discente(&sistema, usuario);
        }
    }

    salvar_alteracoes(&sistema);
    liberar_sistema(&sistema);
    printf("Encerrado. Memoria liberada e dados salvos.\n");
    return 0;
}
