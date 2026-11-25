# Guia do Estudante 1 — Módulo de Cursos (`src/cursos.c`)

**Seu único arquivo para editar:** `src/cursos.c`  
**Contrato de referência:** [include/cursos.h](../include/cursos.h)  
**Total de funções:** apenas **5 funções**.

---

## Passo a Passo

### 1. `plataforma_inicializar(Plataforma *plat)` *(Comece por aqui — 2 linhas!)*
- Se `plat == NULL`, apenas retorne (`return;`).
- Defina:
  ```c
  plat->inicio = NULL;
  plat->total_cursos = 0;
  ```

### 2. `curso_buscar_por_indice(const Plataforma *plat, int indice)`
- Se `plat == NULL || indice < 1 || indice > plat->total_cursos`, retorne `NULL`.
- Crie `Curso *atual = plat->inicio;` e um contador `int pos = 1;`.
- Faça um `while (atual != NULL && pos < indice)`: avance `atual = atual->next; pos++;`.
- Retorne `atual`.

### 3. `curso_criar(Plataforma *plat, const char *nome)`
1. Valide: se `plat == NULL || nome == NULL || nome[0] == '\0'`, retorne `NULL`.
2. Aloque memória:
   ```c
   Curso *novo = malloc(sizeof(Curso));
   if (novo == NULL) {
       return NULL;
   }
   ```
3. Preencha os campos:
   ```c
   strncpy(novo->nome, nome, TAM_TEXTO - 1);
   novo->nome[TAM_TEXTO - 1] = '\0';
   novo->aulas = NULL;
   novo->total_aulas = 0;
   novo->next = NULL;
   ```
4. Insira no **final** da lista:
   - Se `plat->inicio == NULL`: `plat->inicio = novo;`
   - Senão: percorra com `Curso *atual = plat->inicio;` até `atual->next == NULL`, e então faça `atual->next = novo;`.
5. Incremente `plat->total_cursos++` e retorne `novo`.

### 4. `curso_remover(Plataforma *plat, int indice)`
1. Valide: se `plat == NULL || indice < 1 || indice > plat->total_cursos`, retorne `false`.
2. Percorra guardando `anterior` e `atual`:
   ```c
   Curso *anterior = NULL;
   Curso *atual = plat->inicio;
   for (int pos = 1; pos < indice; pos++) {
       anterior = atual;
       atual = atual->next;
   }
   ```
3. Desvincule `atual` da lista:
   - Se `anterior == NULL` (estava no início): `plat->inicio = atual->next;`
   - Senão: `anterior->next = atual->next;`
4. Libere a memória **na ordem certa**:
   ```c
   aula_liberar_todas(atual);
   free(atual);
   plat->total_cursos--;
   return true;
   ```

### 5. `plataforma_liberar(Plataforma *plat)`
- Se `plat == NULL`, retorne.
- Percorra todos os cursos salvando o próximo antes do `free`:
  ```c
  Curso *atual = plat->inicio;
  while (atual != NULL) {
      Curso *proximo = atual->next;
      aula_liberar_todas(atual);
      free(atual);
      atual = proximo;
  }
  plat->inicio = NULL;
  plat->total_cursos = 0;
  ```

---

## Checklist do Estudante 1
- [ ] Compila sem warnings (`gcc -Wall -Wextra -std=c11 -Iinclude ...`).
- [ ] Criar 3 cursos mantém a ordem `#1`, `#2`, `#3`.
- [ ] Remover o primeiro curso (`#1`), o do meio (`#2`) e o último funciona sem crash.
- [ ] `plataforma_liberar` zera `inicio` e `total_cursos`.
