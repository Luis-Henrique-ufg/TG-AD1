# Relatório Final — Plataforma de Ensino a Distância (EaD)

Projeto acadêmico de Algoritmos e Estruturas de Dados 1 (C99). Escopo restrito a `struct`, ponteiros, alocação dinâmica, listas encadeadas simples e duplas com descritores (`inicio`, `fim`, `tamanho`).

---

## 1. Arquitetura de diretórios e papel de cada arquivo

```
Trabalho Edmundo/
├── include/            # contratos (headers) das TADs e utilitários
│   ├── anotacao.h
│   ├── avaliacao.h
│   ├── curso.h
│   ├── forum.h
│   ├── persistencia.h
│   ├── usuario.h
│   └── utils.h
├── src/                # implementações C99
│   ├── anotacao.c
│   ├── avaliacao.c
│   ├── curso.c
│   ├── forum.c
│   ├── main.c
│   ├── persistencia.c
│   ├── usuario.c
│   └── utils.c
├── data/               # persistência em arquivos texto (delimitador '|')
│   ├── usuarios.txt
│   ├── cursos.txt
│   ├── modulos.txt
│   ├── aulas.txt
│   ├── anotacoes.txt
│   ├── concluidas.txt
│   ├── avaliacoes.txt
│   ├── topicos.txt
│   └── respostas.txt
├── sistema             # executável gerado (sistema.exe no Windows)
└── RELATORIO_FINAL.md  # este documento
```

### Headers (`include/`)

| Arquivo | Papel |
|---|---|
| `anotacao.h` | Define `Anotacao`, `NoAnotacao` e `ListaAnotacoes`; expõe inserção ordenada por timestamp, busca, edição, remoção por ID e remoção por aluno. |
| `avaliacao.h` | Define `Avaliacao`, `NoAvaliacao` e `ListaAvaliacoes` (lista ordenada por nota decrescente); expõe remoção por ID, por curso e por aluno. |
| `curso.h` | Define a hierarquia `NoCurso -> NoModulo -> NoAula` e seus descritores; expõe CRUD, geração de IDs, cálculo de progresso e helpers de limpeza de referências. |
| `forum.h` | Define `NoTopico` e `NoResposta` e seus descritores; expõe CRUD do fórum e remoções por curso/autor. |
| `persistencia.h` | Define `Sistema`, que agrega as quatro listas principais; declara carga, gravação, cascatas e exportação do caderno em Markdown. |
| `usuario.h` | Define `Usuario`, `ListaUsuarios`, `ListaConcluidas` (simples) e `ListaHistorico` (dupla, `MAX_HISTORICO = 5`); expõe autenticação, exclusão e manipulação das sublistas. |
| `utils.h` | Contrato de leitura segura de terminal e conversão de horários (`HH:MM:SS` ↔ segundos); documenta a substituição de `|` por `-`. |

### Implementações (`src/`)

| Arquivo | Papel |
|---|---|
| `anotacao.c` | Insere anotações em ordem crescente de timestamp (`ligar_no_ordenado`), edita realocando o nó na ordem, remove por ID ou por aluno e libera a sublista. |
| `avaliacao.c` | Mantém o ranking ordenado por nota decrescente (empate preserva ordem de chegada) e remove avaliações órfãs por curso/aluno. |
| `curso.c` | Implementa as três camadas da grade curricular com descritores; `excluir_curso` faz a cascata de memória módulos → aulas → anotações; implementa helpers que percorrem a grade para limpar referências em outras listas. |
| `forum.c` | Implementa tópicos e respostas com descritores; remove tópicos por curso/autor e respostas por autor para evitar órfãos após exclusões. |
| `main.c` | Toda a interface por terminal: menus de docente/discente, login, grade, avaliações, anotações, progresso, busca, histórico, fórum e exportação de caderno. Chama as cascatas de `persistencia.c`. |
| `persistencia.c` | Constrói/desmonta `Sistema`, lê e grava os 9 arquivos em `data/`, implementa as cascatas em nível de sistema e gera `caderno_aluno_<id>.md`. |
| `usuario.c` | Gerencia usuários, aulas concluídas e histórico recente; o histórico insere na cabeça e, ao exceder 5, remove a cauda em O(1) via `fim->anterior`. |
| `utils.c` | `fgets` + `strcspn` para remover `\r`/`\n`, descarte de resíduo de linhas longas, substituição de `|` por `-`, validação e conversão de horários. |

### Correções aplicadas na auditoria

- **CRLF:** todas as 9 leituras com `fgets` em `persistencia.c` e a leitura de terminal em `utils.c` agora usam `strcspn(linha, "\r\n")`.
- **Delimitador `|`:** `ler_texto()` substitui `|` por `-`; linhas maiores que o buffer são descartadas para não vazar para a próxima pergunta.
- **Cascata de curso:** `excluir_curso_cascata()` remove avaliações, tópicos/respostas e referências de aulas concluídas/histórico antes de liberar módulos, aulas e anotações.
- **Cascata de usuário:** `excluir_usuario_cascata()` remove notas, tópicos criados, respostas e anotações do aluno, além das sublistas concluídas/histórico.
- **RF07:** limite de 5 do histórico confirmado em `registrar_historico()` — remoção pela cauda com `fim = fim->anterior`, `fim->proximo = NULL` e `free`.
- **RNF03:** `liberar_sistema()` percorre usuários (com concluídas e histórico), cursos (com módulos, aulas e anotações), avaliações e tópicos (com respostas), cobrindo todos os `malloc` do projeto.
- **Carga defensiva:** registros órfãos (curso/aluno/aula inexistentes) são ignorados na leitura dos arquivos.

### Compilação

```
gcc -Wall -Wextra -std=c99 -pedantic -g src/*.c -Iinclude -o sistema
```

Compilada sem nenhum warning ou erro.

---

## 2. Layout exato dos arquivos em `data/`

Extraído diretamente dos `fprintf` de `salvar_sistema()` em `src/persistencia.c`. Todos os campos são separados por `|` e terminados por `\n`. A ordem de gravação é a mesma ordem de carregamento.

| Arquivo | Layout (`fprintf`) | Ordem dos campos |
|---|---|---|
| `usuarios.txt` | `%d\|%s\|%s\|%s\|%d\n` | `id`, `nome`, `email`, `senha`, `perfil` (1 = docente, 2 = discente) |
| `cursos.txt` | `%d\|%s\|%s\n` | `id_curso`, `titulo`, `descricao` |
| `modulos.txt` | `%d\|%d\|%s\n` | `id_curso`, `id_modulo`, `titulo` |
| `aulas.txt` | `%d\|%d\|%s\|%d\|%d\n` | `id_modulo`, `id_aula`, `titulo`, `duracao_segundos`, `eh_aula_valida` (1 = aula, 0 = material) |
| `anotacoes.txt` | `%d\|%d\|%d\|%d\|%s\n` | `id_aula`, `id_anotacao`, `id_aluno`, `timestamp_segundos`, `texto` |
| `concluidas.txt` | `%d\|%d\n` | `id_usuario`, `id_aula` |
| `avaliacoes.txt` | `%d\|%d\|%d\|%s\|%.2f\n` | `id_avaliacao`, `id_curso`, `id_aluno`, `titulo_atividade`, `nota` (2 casas decimais) |
| `topicos.txt` | `%d\|%d\|%d\|%s\|%s\n` | `id_topico`, `id_curso`, `id_autor`, `titulo`, `pergunta` |
| `respostas.txt` | `%d\|%d\|%d\|%s\n` | `id_topico`, `id_resposta`, `id_autor`, `texto` |

Exemplos reais da massa de dados atual:

```
usuarios.txt:  1|Ana Paula|prof@ufg.br|123|1
cursos.txt:    1|Algoritmos e Estruturas de Dados 1|Listas encadeadas e complexidade
modulos.txt:   1|1|Introducao e complexidade
aulas.txt:     1|1|Visao geral da disciplina|600|1
anotacoes.txt: 1|1|2|45|Revisar definicao de algoritmo antes da prova
concluidas.txt:2|1
avaliacoes.txt:1|1|2|Lista1 - complexidade|8.50
topicos.txt:   1|1|2|Dupla ou simples?|Quando devo usar lista duplamente encadeada neste projeto?
respostas.txt: 1|1|3|Use dupla quando precisar voltar, como no historico de 5 aulas.
```

Arquivo gerado à parte, fora de `data/`: `caderno_aluno_<id>.md`, com `# Caderno de estudos - aluno %d`, `## %s > %s > %s` (curso > módulo > aula) e `- [%s] %s` (`[HH:MM:SS] texto`).

---

## 3. Massa de dados para apresentação

- 1 docente (`prof@ufg.br` / `123`) e 2 discentes (`aluno1@ufg.br`, `aluno2@ufg.br` / `123`).
- 2 cursos, 4 módulos e 10 itens (8 aulas válidas + 2 materiais complementares).
- 10 anotações com timestamps distribuídas entre os alunos.
- 5 avaliações, 3 tópicos e 4 respostas no fórum.
- Progresso: `aluno1@ufg.br` = 5/7 aulas válidas (71,4%); `aluno2@ufg.br` = 0/7 (0,0%).
