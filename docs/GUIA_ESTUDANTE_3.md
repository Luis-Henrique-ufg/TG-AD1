# Guia do Estudante 3 — Ordenação e Busca (`src/ordenacao.c`)

**Seu único arquivo para editar:** `src/ordenacao.c`  
**Contrato de referência:** [include/ordenacao.h](../include/ordenacao.h)  
**Total de funções:** apenas **4 funções**.

> **Boa notícia:** como o professor aceita qualquer algoritmo de ordenação, você pode usar **Bubble Sort** ou **Selection Sort** na lista encadeada! São fáceis de entender, curtos e muito tranquilos de explicar na apresentação.

---

## Como Ordenar uma Lista Simplesmente Encadeada do Jeito Mais Fácil

Na nossa lista simples (sem ponteiros externos apontando para nós individuais), o jeito mais fácil de implementar **Bubble Sort** é trocar os **dados** de dois nós adjacentes (`atual` e `atual->next`), mantendo o ponteiro `next` intacto!

### Função auxiliar para trocar os dados de 2 Cursos:
```c
static void trocar_dados_cursos(Curso *a, Curso *b) {
    char temp_nome[TAM_TEXTO];
    Aula *temp_aulas = a->aulas;
    int temp_total = a->total_aulas;

    strcpy(temp_nome, a->nome);
    strcpy(a->nome, b->nome);
    strcpy(b->nome, temp_nome);

    a->aulas = b->aulas;
    b->aulas = temp_aulas;

    a->total_aulas = b->total_aulas;
    b->total_aulas = temp_total;
}
```
*(Repare que `a->next` e `b->next` NÃO mudam — por isso a lista nunca quebra!)*

### Função auxiliar para trocar os dados de 2 Aulas:
Troque apenas `titulo`, `anotacao`, `duracao_min` e `concluida` entre `a` e `b` (sem mexer em `next`).

---

## Passo a Passo (do mais fácil para o mais trabalhoso)

### 1. Helper `static int comparar_texto_ci(const char *a, const char *b)`
Compara duas strings de A–Z ignorando maiúsculas/minúsculas (portável para Windows e Linux):
```c
static int comparar_texto_ci(const char *a, const char *b) {
    while (*a != '\0' && *b != '\0') {
        int ca = tolower((unsigned char)*a);
        int cb = tolower((unsigned char)*b);
        if (ca != cb) {
            return ca - cb;
        }
        a++;
        b++;
    }
    return tolower((unsigned char)*a) - tolower((unsigned char)*b);
}
```

### 2. `buscar_curso_por_nome(const Plataforma *plat, const char *termo)` *(Comece por aqui!)*
- Se `plat == NULL || termo == NULL || termo[0] == '\0'`, retorne `NULL`.
- Crie um helper `static bool contem_texto_ci(const char *texto, const char *termo)` que testa se `termo` aparece em qualquer posição de `texto` usando `tolower((unsigned char)...)`.
- Percorra `plat->inicio` com `while (atual != NULL)` e retorne o primeiro curso onde `contem_texto_ci(atual->nome, termo)` for `true`.
- Se acabar a lista, retorne `NULL`.

### 3. `ordenar_aulas_por_duracao(Curso *curso)` (Bubble Sort — menor duração primeiro)
- Se `curso == NULL || curso->aulas == NULL || curso->aulas->next == NULL`, retorne.
- Estrutura do Bubble Sort em lista:
  ```c
  bool trocou;
  do {
      trocou = false;
      Aula *atual = curso->aulas;
      while (atual->next != NULL) {
          if (atual->duracao_min > atual->next->duracao_min) {
              trocar_dados_aulas(atual, atual->next);
              trocou = true;
          }
          atual = atual->next;
      }
  } while (trocou);
  ```

### 4. `ordenar_cursos_por_nome(Plataforma *plat)` (Bubble Sort — A até Z)
- Mesma estrutura de `do { ... } while (trocou);` percorrendo `plat->inicio`.
- Troca quando `comparar_texto_ci(atual->nome, atual->next->nome) > 0`.

### 5. `ordenar_cursos_por_progresso(Plataforma *plat)` (Bubble Sort — maior progresso primeiro)
- Mesma estrutura, chamando `aula_calcular_progresso(atual)` e `aula_calcular_progresso(atual->next)` (do Estudante 2!).
- Troca quando `prog_atual < prog_proximo` (porque queremos do **maior** para o **menor**!).

> **Opcional (se quiser um desafio extra depois que tudo funcionar):** você pode trocar a ordenação por **Insertion Sort**, **Selection Sort** ou **Merge Sort** — a assinatura no `ordenacao.h` é exatamente a mesma!

---

## Checklist do Estudante 3
- [ ] Compila sem warnings.
- [ ] Ordenar listas com 0 ou 1 elemento não causa crash.
- [ ] `ordenar_cursos_por_nome` ordena de A–Z ignorando maiúsculas/minúsculas.
- [ ] `ordenar_cursos_por_progresso` coloca o curso com maior `%` no topo (`#1`).
- [ ] `ordenar_aulas_por_duracao` coloca a aula mais curta primeiro.
- [ ] `buscar_curso_por_nome` encontra `"DADOS"` dentro de `"Banco de Dados SQL"`.
