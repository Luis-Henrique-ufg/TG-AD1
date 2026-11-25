# EAD-CLI — Gerenciador de Cursos e Aulas no Terminal (C)

Projeto enxuto em **C padrão (C11)** desenvolvido em grupo de **4 estudantes** para a disciplina de **Algoritmos e Estruturas de Dados (TG-AD1)**, com foco direto em **Listas Encadeadas (`malloc`/`free`)** e **Ordenação**.

Sem bibliotecas externas, sem APIs de sistema operacional e sem complexidade desnecessária — apenas C puro (`stdio.h`, `stdlib.h`, `string.h`, `ctype.h`, `stdbool.h`).

---

## Sumário

- [Funcionalidade Mais Fácil (Ponto de Partida)](#funcionalidade-mais-fácil-ponto-de-partida)
- [Requisitos Funcionais (RF)](#requisitos-funcionais-rf)
- [Requisitos Não Funcionais (RNF)](#requisitos-não-funcionais-rnf)
- [Casos de Uso (Use Cases)](#casos-de-uso-use-cases)
- [Protótipo das Telas (Terminal)](#protótipo-das-telas-terminal)
- [Divisão de Tarefas (4 Estudantes)](#divisão-de-tarefas-4-estudantes)
- [Estrutura do Repositório](#estrutura-do-repositório)
- [Como Compilar e Executar](#como-compilar-e-executar)
- [Documentação Completa](#documentação-completa)

---

## Funcionalidade Mais Fácil (Ponto de Partida)

> **⭐ Funcionalidade Mais Fácil do Projeto:** **`Marcar / Desmarcar Aula como Concluída`** (`RF03` / `UC03`)
>
> - **Por que é a mais fácil?** Não usa `malloc`/`free` nem altera ponteiros `*next`. É apenas verificar `if (aula == NULL) return false;` e inverter um booleano (`aula->concluida = !aula->concluida;`). O resultado já aparece na hora na tela (`[ ]` ↔ `[x]`) e na barra de progresso do curso!
> - **Onde fica:** `aula_alternar_concluida()` em [src/aulas.c](src/aulas.c).

**Primeiro passo mais fácil para cada um dos 4 estudantes:**
- **Estudante 1:** `plataforma_inicializar()` em `src/cursos.c` (2 linhas!).
- **Estudante 2:** `aula_alternar_concluida()` em `src/aulas.c` (3 linhas!).
- **Estudante 3:** `buscar_curso_por_nome()` em `src/ordenacao.c` (um único `while` na lista!).
- **Estudante 4:** `mostrar_barra_progresso()` em `src/interface.c` (um `for` de 0 a 9 imprimindo `#` ou `-`!).

---

## Requisitos Funcionais (RF)

| ID | Funcionalidade | Descrição | Dificuldade | Responsável |
| :--- | :--- | :--- | :---: | :--- |
| **RF01** | **Gerenciar Cursos (CRUD)** | Criar, listar, selecionar por número (`#1..#N`) e remover cursos armazenados em uma **lista simplesmente encadeada**. | Média | Estudante 1 + Estudante 4 |
| **RF02** | **Gerenciar Aulas (CRUD)** | Criar, listar, selecionar por número e remover aulas (com título, duração em minutos e uma anotação curta) em uma **lista simplesmente encadeada** dentro de cada curso. | Média | Estudante 2 + Estudante 4 |
| **RF03** | **Marcar / Desmarcar Aula como Concluída ⭐ *(Mais Fácil)*** | Alternar o status da aula entre `[ ]` (em aberto) e `[x]` (concluída). | **Muito Fácil** | Estudante 2 + Estudante 4 |
| **RF04** | **Calcular Progresso do Curso** | Calcular a porcentagem de aulas concluídas (`0%` a `100%`, retornando `0%` se não houver aulas) e exibir em barra ASCII `[#######---]`. | Fácil | Estudante 2 + Estudante 4 |
| **RF05** | **Ordenar Cursos e Aulas** | Ordenar listas encadeadas usando o algoritmo escolhido pelo grupo (ex.: Bubble Sort ou Selection Sort):<br>• **Cursos:** por nome (A–Z) e por maior progresso (`%`).<br>• **Aulas:** por menor duração (minutos). | Média | Estudante 3 + Estudante 4 |
| **RF06** | **Buscar Curso por Nome** | Procurar o primeiro curso cujo nome contenha o termo digitado, sem diferenciar maiúsculas de minúsculas. | Fácil | Estudante 3 + Estudante 4 |
| **RF07** | **Dados Iniciais de Exemplo** | Carregar automaticamente 3 cursos e algumas aulas na inicialização para facilitar testes e a apresentação. | Fácil | Estudante 4 |

---

## Requisitos Não Funcionais (RNF)

| ID | Categoria | Descrição |
| :--- | :--- | :--- |
| **RNF01** | **Portabilidade** | Escrito em **C11 padrão**, sem bibliotecas externas e sem chamadas específicas de Windows/Linux; compila em qualquer sistema com um único comando `gcc`. |
| **RNF02** | **Gestão de Memória** | Todo `malloc` deve ser checado contra `NULL` e toda memória alocada deve ser liberada com `free` em cascata (`Aulas → Cursos`) ao remover cursos ou encerrar o programa. |
| **RNF03** | **Entrada Segura** | Toda entrada do teclado deve usar `fgets` (e `strtol` para inteiros), evitando `scanf` e loops infinitos quando o usuário digita texto inválido. |
| **RNF04** | **Compilação Limpa** | Deve compilar com `-Wall -Wextra -std=c11` apresentando **zero warnings**. |
| **RNF05** | **Modularidade (4 Pessoas)** | Contrato fechado em `include/models.h` e 1 arquivo `.c` por estudante, evitando conflitos de Git. |

---

## Casos de Uso (Use Cases)

- **Ator Principal:** Estudante (usuário do terminal).
- **Sistema:** **EAD-CLI**.

### Resumo dos Casos de Uso

| Código | Caso de Uso | Requisitos | Tela |
| :--- | :--- | :--- | :--- |
| **UC01** | Gerenciar Cursos (Criar, Listar e Remover) | RF01, RF04, RF07 | Tela 1 — Dashboard |
| **UC02** | Gerenciar Aulas de um Curso | RF02, RF04 | Tela 2 — Tela do Curso |
| **UC03** | **Marcar / Desmarcar Aula como Concluída ⭐ *(Mais Fácil)*** | RF03, RF04 | Tela 2 — Tela do Curso |
| **UC04** | Ordenar Cursos (Nome ou Progresso) | RF05 | Tela 1 — Dashboard |
| **UC05** | Ordenar Aulas por Duração | RF05 | Tela 2 — Tela do Curso |
| **UC06** | Buscar Curso por Nome | RF06 | Tela 1 — Dashboard |

---

### Detalhamento dos Casos de Uso

#### UC01 — Gerenciar Cursos
- **Objetivo:** Cadastrar, visualizar e excluir cursos na tela inicial.
- **Pré-condições:** Sistema iniciado com a plataforma inicializada (e dados de exemplo carregados).
- **Fluxo Principal:**
  1. O sistema exibe a lista `#1..#N` de cursos com barra de progresso `[#######---]` e total de aulas.
  2. **Criar curso:** o estudante escolhe a opção de novo curso e digita o nome; o sistema aloca o nó e insere no final da lista.
  3. **Remover curso:** o estudante escolhe remover e informa o número `#`; o sistema libera todas as aulas do curso e remove o curso da lista.
- **Exceções:** Nome vazio ou número inválido → exibe aviso e volta ao menu sem quebrar.
- **Pós-condições:** `plat->inicio` e `plat->total_cursos` permanecem consistentes.

#### UC02 — Gerenciar Aulas de um Curso
- **Objetivo:** Abrir um curso, cadastrar novas aulas e remover aulas existentes.
- **Pré-condições:** Existir pelo menos um curso cadastrado.
- **Fluxo Principal:**
  1. No Dashboard, o estudante informa o número do curso que deseja abrir.
  2. O sistema exibe as aulas numeradas `1..N` com status `[x]`/`[ ]`, duração e anotação.
  3. **Nova aula:** informa título, duração em minutos (`>= 0`) e uma anotação opcional; o sistema insere no final de `curso->aulas`.
  4. **Remover aula:** informa o número da aula; o sistema remove o nó e chama `free()`.
- **Exceções:** Título vazio ou duração negativa → exibe aviso e não insere.
- **Pós-condições:** `curso->aulas`, `curso->total_aulas` e o progresso do curso são atualizados.

#### UC03 — Marcar / Desmarcar Aula como Concluída ⭐ *(Funcionalidade Mais Fácil)*
- **Objetivo:** Alternar o status de uma aula e atualizar imediatamente a porcentagem do curso.
- **Pré-condições:** Estar dentro de um curso que possua ao menos uma aula.
- **Fluxo Principal:**
  1. Na Tela do Curso, o estudante escolhe a opção de concluir/desmarcar e digita o número da aula.
  2. O sistema chama `aula_alternar_concluida(aula)`.
  3. A tela é exibida novamente mostrando `[x]` (ou `[ ]`) e a barra de progresso atualizada.
- **Exceções:** Número fora de `1..total_aulas` → exibe aviso.
- **Pós-condições:** `aula->concluida` invertido e progresso recalculado.

#### UC04 — Ordenar Cursos (Nome ou Progresso)
- **Objetivo:** Reorganizar a lista de cursos no Dashboard.
- **Pré-condições:** Estar no Dashboard.
- **Fluxo Principal:**
  1. O estudante escolhe **Ordenar por Nome (A–Z)** ou **Ordenar por Maior Progresso (%)**.
  2. O sistema executa o algoritmo de ordenação na lista encadeada `plat->inicio`.
  3. O Dashboard é exibido com a nova ordem.
- **Exceções:** Lista com 0 ou 1 curso → não faz alterações e continua normalmente.

#### UC05 — Ordenar Aulas por Duração
- **Objetivo:** Reorganizar as aulas de um curso da mais curta para a mais longa.
- **Pré-condições:** Estar na Tela do Curso.
- **Fluxo Principal:**
  1. O estudante escolhe **Ordenar aulas por menor duração**.
  2. O sistema executa `ordenar_aulas_por_duracao(curso)`.
  3. A Tela do Curso é exibida com as aulas em ordem crescente de minutos.

#### UC06 — Buscar Curso por Nome
- **Objetivo:** Localizar rapidamente um curso digitando parte do nome.
- **Pré-condições:** Estar no Dashboard.
- **Fluxo Principal:**
  1. O estudante escolhe **Buscar curso** e digita o termo (ex.: `dados` ou `SQL`).
  2. O sistema chama `buscar_curso_por_nome(&plat, termo)` (ignorando maiúsculas/minúsculas).
  3. Se encontrar, exibe o curso encontrado (ou já abre a Tela do Curso).
- **Exceções:** Nenhum curso encontrado → exibe mensagem `"Curso nao encontrado."`.

---

## Protótipo das Telas (Terminal)

### Tela 1 — Dashboard (Cursos)
```text
============================================================
   EAD-CLI | Gerenciador de Cursos             [3 cursos]
============================================================
  #1  Estruturas de Dados em C    [######----]  66% (2/3 aulas)
  #2  Banco de Dados SQL          [#####-----]  50% (1/2 aulas)
  #3  Algoritmos de Ordenacao     [----------]   0% (0/2 aulas)

  1. Abrir curso
  2. Novo curso
  3. Remover curso
  4. Ordenar por nome (A-Z)
  5. Ordenar por maior progresso
  6. Buscar curso por nome
  0. Sair
Opcao: _
```

### Tela 2 — Dentro de um Curso (Aulas)
```text
============================================================
  CURSO: Estruturas de Dados em C   [######----] 66% (2/3)
============================================================
  1. [x] Ponteiros em C (20 min)
     Nota: Sempre verificar se malloc retornou NULL
  2. [x] Listas Encadeadas (25 min)
     Nota: Usar ponteiro auxiliar para percorrer com while
  3. [ ] Ordenacao em Listas (30 min)
     Nota: Bubble Sort ou Selection Sort funciona bem

  1. Nova aula
  2. Marcar / desmarcar aula como concluida
  3. Remover aula
  4. Ordenar aulas por menor duracao
  0. Voltar
Opcao: _
```

---

## Divisão de Tarefas (4 Estudantes)

> **Regra de ouro:** cada integrante edita **apenas o seu arquivo `.c`**. Assim ninguém sobrescreve o código do colega no Git!

| Integrante | Responsabilidade | Arquivo(s) | Qtd. Funções | Guia Passo a Passo |
| :--- | :--- | :--- | :---: | :--- |
| **Estudante 1** | Lista de Cursos (`malloc`, inserção no fim, busca por número, remoção e `free` total) | `src/cursos.c` | 5 | [GUIA_ESTUDANTE_1.md](docs/GUIA_ESTUDANTE_1.md) |
| **Estudante 2** | Lista de Aulas, **concluir aula ⭐ *(Mais Fácil)***, cálculo de progresso (`%`) e `free` das aulas | `src/aulas.c` | 6 | [GUIA_ESTUDANTE_2.md](docs/GUIA_ESTUDANTE_2.md) |
| **Estudante 3** | Ordenação das listas (Bubble/Selection Sort) e busca de curso por nome | `src/ordenacao.c` | 4 | [GUIA_ESTUDANTE_3.md](docs/GUIA_ESTUDANTE_3.md) |
| **Estudante 4** | Leitura segura (`fgets`), telas no terminal, dados de exemplo e menu principal | `src/interface.c`<br>`src/main.c` | 6 + menu | [GUIA_ESTUDANTE_4.md](docs/GUIA_ESTUDANTE_4.md) |

---

## Estrutura do Repositório

```text
TG-AD1/
├── README.md                    # Este arquivo (Requisitos, Use Cases e divisao)
├── Makefile                     # Atalho de compilacao (opcional)
├── .gitignore                   # Ignora executaveis e arquivos temporarios
├── include/                     # CONTRATO COMPARTILHADO
│   ├── models.h                 # Structs Aula, Curso e Plataforma (lista simples *next)
│   ├── cursos.h                 # Assinaturas do Estudante 1
│   ├── aulas.h                  # Assinaturas do Estudante 2
│   ├── ordenacao.h              # Assinaturas do Estudante 3
│   └── interface.h              # Assinaturas do Estudante 4
├── src/                         # Implementacoes .c (esqueletos ja compilam!)
│   ├── cursos.c                 # -> Estudante 1
│   ├── aulas.c                  # -> Estudante 2
│   ├── ordenacao.c              # -> Estudante 3
│   ├── interface.c              # -> Estudante 4
│   └── main.c                   # -> Estudante 4 (menu principal)
├── tests/
│   └── testar_tudo.c            # Testes automaticos simples para validar os modulos
└── docs/                        # Guias curtos e diretos
    ├── CONTRATO.md              # Diagrama das listas e regras de memoria
    ├── GIT_E_CONVENCOES.md      # Fluxo de 4 branches no Git e dicas de C
    ├── GUIA_ESTUDANTE_1.md      # Passo a passo do Estudante 1
    ├── GUIA_ESTUDANTE_2.md      # Passo a passo do Estudante 2
    ├── GUIA_ESTUDANTE_3.md      # Passo a passo do Estudante 3
    ├── GUIA_ESTUDANTE_4.md      # Passo a passo do Estudante 4
    └── notas-iniciais/          # Rascunhos originais preservados
```

---

## Como Compilar e Executar

### Comando direto com `gcc` (Windows, Linux e macOS)

**Programa principal:**
```powershell
# Windows (PowerShell / CMD)
gcc -Wall -Wextra -std=c11 -Iinclude src/cursos.c src/aulas.c src/ordenacao.c src/interface.c src/main.c -o ead.exe
.\ead.exe
```

```bash
# Linux / macOS
gcc -Wall -Wextra -std=c11 -Iinclude src/*.c -o ead
./ead
```

**Testes automáticos (valida Estudantes 1, 2 e 3 sem precisar digitar no menu):**
```powershell
# Windows (PowerShell / CMD)
gcc -Wall -Wextra -std=c11 -Iinclude src/cursos.c src/aulas.c src/ordenacao.c tests/testar_tudo.c -o testar_tudo.exe
.\testar_tudo.exe
```

```bash
# Linux / macOS
gcc -Wall -Wextra -std=c11 -Iinclude src/cursos.c src/aulas.c src/ordenacao.c tests/testar_tudo.c -o testar_tudo
./testar_tudo
```

### Usando `Makefile` (se tiver `make` instalado)
```bash
make        # Compila o programa principal
make run    # Compila e executa
make test   # Compila e roda tests/testar_tudo.c
make clean  # Remove executaveis
```

---

## Documentação Completa

1. [**docs/CONTRATO.md**](docs/CONTRATO.md) — Como funcionam as structs, ponteiros `*next` e liberação de memória.
2. [**docs/GIT_E_CONVENCOES.md**](docs/GIT_E_CONVENCOES.md) — Como usar Git em 4 pessoas sem conflito + erros comuns em C.
3. [**docs/GUIA_ESTUDANTE_1.md**](docs/GUIA_ESTUDANTE_1.md) — Roteiro passo a passo do Estudante 1 (`src/cursos.c`).
4. [**docs/GUIA_ESTUDANTE_2.md**](docs/GUIA_ESTUDANTE_2.md) — Roteiro passo a passo do Estudante 2 (`src/aulas.c`).
5. [**docs/GUIA_ESTUDANTE_3.md**](docs/GUIA_ESTUDANTE_3.md) — Roteiro passo a passo do Estudante 3 (`src/ordenacao.c`).
6. [**docs/GUIA_ESTUDANTE_4.md**](docs/GUIA_ESTUDANTE_4.md) — Roteiro passo a passo do Estudante 4 (`src/interface.c` e `src/main.c`).
