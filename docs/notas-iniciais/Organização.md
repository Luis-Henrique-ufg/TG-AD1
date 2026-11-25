Para um grupo de **3 pessoas em C**, o segredo para não dar conflito de código (nem ninguém ficar travado esperando o outro) é **combinar primeiro um único arquivo `models.h`** com as `struct`s. Com esse contrato definido, cada pessoa trabalha nos seus próprios `.h`/`.c` em paralelo.

Aqui vai uma divisão equilibrada onde **os três mexem com ponteiros e listas encadeadas**, mas cada um tem um domínio claro:

---

### Contrato Compartilhado (os 3 fecham juntos no Dia 1)
- **`models.h`**: definição das structs `Nota`, `Aula`, `Curso` e `Plataforma` (que guarda o ponteiro `Curso *inicio` e `Aula *ultima_aula`), além das constantes de tamanho de string.

---

### Pessoa 1 — Listas Encadeadas e Regras de Negócio (CRUD)
**Arquivos:** `curso.h/.c`, `aula.h/.c`, `anotacao.h/.c`

- **Cursos (lista duplamente encadeada):** criar, editar, remover, buscar por ID/índice e **mover curso para cima/baixo** (trocando ponteiros `prev`/`next`, estilo reordenação do Receitas).
- **Aulas (lista duplamente encadeada dentro de cada curso):** adicionar, editar, remover, marcar/desmarcar como concluída e calcular o progresso do curso (`concluidas / total` e duração total).
- **Anotações (lista encadeada dentro de cada aula):** adicionar com timestamp (`MM:SS`) já mantendo inserção ordenada, listar e remover.
- **Gestão de memória:** função `liberar_plataforma()` para dar `free()` em cascata (notas → aulas → cursos) sem vazamento.

---

### Pessoa 2 — Ordenação, Busca e Persistência em Arquivo
**Arquivos:** `ordenacao.h/.c`, `busca.h/.c`, `persistencia.h/.c`

- **Ordenação em Lista Encadeada (Merge Sort):**
  - Implementar Merge Sort ajustando `next` e `prev`, recebendo um **ponteiro de função comparadora** (`int (*cmp)(...)`) para não duplicar o algoritmo.
  - Comparadores de **Curso**: nome (A–Z), maior/menor progresso (`%`), duração total e ordem manual.
  - Comparadores de **Aula**: módulo/ordem, menor duração e pendentes primeiro.
- **Busca textual:** pesquisar termo (sem diferenciar maiúsculas/minúsculas) nos títulos de cursos, títulos de aulas e textos de anotações.
- **Persistência (`dados.txt`):**
  - Salvar toda a estrutura em arquivo texto estruturado (ex.: linhas `CURSO|...`, `AULA|...`, `NOTA|...`).
  - Carregar o arquivo na inicialização reconstruindo todos os ponteiros e a `ultima_aula`.
- **Exportação:** gerar um arquivo `resumo_<curso>.md` com todas as aulas e anotações ordenadas.

---

### Pessoa 3 — Interface de Terminal, Abridor de Mídia e Integração
**Arquivos:** `ui.h/.c`, `launcher.h/.c`, `main.c`

- **Telas no terminal (`ui`):**
  - Dashboard (barra de progresso ASCII `[██████░░░░]`, atalho *"Continuar de onde parou"*, exibição do critério de ordenação atual).
  - Tela do curso (tabela de aulas com `[x]`/`[ ]`, duração e contagem de notas).
  - Tela da aula (detalhes, anotações e navegação `p` anterior / `n` próxima usando `aula->prev` e `aula->next`).
- **Entrada segura e formatação:** helpers usando `fgets` (evitando bugs de `scanf`), formatação de segundos para `MM:SS` / `HH:MM` e cores ANSI leves.
- **Abridor real de aulas (`launcher`):** função que abre o link (YouTube/web) ou caminho local (`.mp4`/`.pdf`) no programa padrão do sistema (`start` / `ShellExecute` no Windows, `xdg-open` no Linux, `open` no macOS).
- **Integração (`main.c`) + Massa de teste:** conectar os menus às funções das Pessoas 1 e 2 e montar um `dados.txt` inicial com 3–4 cursos prontos para a apresentação.

---

### Resumo visual da divisão

| Integrante | Foco principal | Arquivos |
| :--- | :--- | :--- |
| **Pessoa 1** | Estruturas de dados (listas encadeadas, CRUD, memória) | `curso.*`, `aula.*`, `anotacao.*` |
| **Pessoa 2** | Algoritmos (Merge Sort, busca) e Arquivos (save/load/export) | `ordenacao.*`, `busca.*`, `persistencia.*` |
| **Pessoa 3** | UX no terminal, abrir vídeo/link real e integração geral | `ui.*`, `launcher.*`, `main.c` |

### Ordem recomendada de trabalho
1. **Dia 1 (30 min juntos):** fechar `models.h` e os protótipos das funções nos `.h`.
2. **Desenvolvimento paralelo:** cada um implementa seus `.c` (a Pessoa 1 e a Pessoa 2 podem testar suas funções com um `main_teste.c` simples antes mesmo da UI ficar pronta).
3. **Integração final:** compilar tudo junto (`gcc src/*.c -o ead`), testar o fluxo completo e checar memory leaks.

Se quiser, posso gerar agora o **`models.h`** e todos os **arquivos `.h` (assinaturas das funções)** já prontos para vocês dividirem e cada um só precisar preencher o seu `.c`. Quer que eu monte isso?