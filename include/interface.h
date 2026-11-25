#ifndef INTERFACE_H
#define INTERFACE_H

#include "models.h"

/* ============================================================================
 * MODULO: INTERFACE DE TERMINAL (Responsavel: Estudante 4)
 * ============================================================================
 * Responsavel por leitura segura (fgets, sem scanf), exibicao das telas
 * no terminal (texto puro + ASCII) e carga dos dados iniciais de exemplo.
 * ========================================================================== */

/**
 * Exibe 'prompt' e le uma linha de texto com fgets, removendo o '\n' final.
 * Se o texto digitado for maior que o buffer, descarta o excesso do stdin.
 */
void ler_texto(const char *prompt, char *buffer, int tamanho);

/**
 * Exibe 'prompt' e repete a leitura (usando fgets + strtol) ate o usuario
 * digitar um numero inteiro valido entre 'min' e 'max' (inclusive).
 */
int ler_inteiro(const char *prompt, int min, int max);

/**
 * Imprime uma barra de progresso em ASCII puro (10 blocos), ex: [#######---].
 * Funciona em qualquer terminal Windows/Linux/macOS sem precisar de UTF-8/ANSI.
 */
void mostrar_barra_progresso(double progresso);

/**
 * Exibe a Tela 1 (Dashboard): lista numerada (#1..#N) dos cursos com
 * barra de progresso, total de aulas e o menu de opcoes.
 */
void exibir_dashboard(const Plataforma *plat);

/**
 * Exibe a Tela 2 (Tela do Curso): nome do curso, progresso geral, lista
 * numerada das aulas com [x]/[ ], duracao, anotacao e menu de opcoes.
 */
void exibir_curso(const Curso *curso);

/**
 * Cadastra 3 cursos e algumas aulas prontas na inicializacao usando
 * curso_criar() e aula_criar(), para facilitar testes e a apresentacao.
 */
void carregar_dados_exemplo(Plataforma *plat);

#endif /* INTERFACE_H */
