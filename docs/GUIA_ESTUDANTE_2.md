# Guia do Estudante 2 — Módulo de Aulas e Progresso (`src/aulas.c`)

**Seu único arquivo para editar:** `src/aulas.c`  
**Contrato de referência:** [include/aulas.h](../include/aulas.h)  
**Total de funções:** **6 funções**.

---

## Passo a Passo

### 1. `aula_alternar_concluida(Aula *aula)`
Comece por esta para ganhar confiança:
```c
if (aula == NULL) {
    return false;
}
aula->concluida = !aula->concluida;
return true;
```

### 2. `aula_calcular_progresso(const Curso *curso)`
- Se `curso == NULL || curso->total_aulas == 0`, retorne `0.0` (evita divisão por zero!).
- Percorra `curso->aulas` contando quantas aulas têm `atual->concluida == true`.
- Retorne `(100.0 * concluidas) / curso->total_aulas;`.

### 3. `aula_buscar_por_indice(const Curso *curso, int indice)`
- Se `curso == NULL || indice < 1 || indice > curso->total_aulas`, retorne `NULL`.
- Percorra `curso->aulas` avançando `indice - 1` vezes e retorne o ponteiro.

### 4. `aula_criar(Curso *curso, const char *titulo, int duracao_min, const char *anotacao)`
1. Valide: se `curso == NULL || titulo == NULL || titulo[0] == '\0' || duracao_min < 0`, retorne `NULL`.
2. Aloque `Aula *nova = malloc(sizeof(Aula));` e cheque `if (nova == NULL) return NULL;`.
3. Copie `titulo` com segurança (`strncpy` + `'\0'`).
4. Copie `anotacao`: se `anotacao == NULL`, faça `nova->anotacao[0] = '\0';`; senão use `strncpy` + `'\0'`.
5. Defina `nova->duracao_min = duracao_min;`, `nova->concluida = false;`, `nova->next = NULL;`.
6. Insira no **final** da lista `curso->aulas` (se vazia, `curso->aulas = nova`; senão caminhe até `atual->next == NULL`).
7. Incremente `curso->total_aulas++` e retorne `nova`.

### 5. `aula_remover(Curso *curso, int indice)`
1. Valide: se `curso == NULL || indice < 1 || indice > curso->total_aulas`, retorne `false`.
2. Percorra guardando `anterior` e `atual` até a posição `indice`.
3. Se `anterior == NULL`, `curso->aulas = atual->next;`; senão `anterior->next = atual->next;`.
4. Chame `free(atual);`, decremente `curso->total_aulas--` e retorne `true`.

### 6. `aula_liberar_todas(Curso *curso)`
- Se `curso == NULL`, retorne.
- Percorra salvando `Aula *proximo = atual->next;` antes de dar `free(atual)`.
- Ao final: `curso->aulas = NULL; curso->total_aulas = 0;`.

---

## Checklist do Estudante 2
- [ ] Compila sem warnings.
- [ ] `aula_calcular_progresso` retorna `0.0` quando o curso não tem aulas (sem dar `NaN`).
- [ ] `aula_alternar_concluida` muda `false → true → false` e o progresso atualiza na hora.
- [ ] Remover a 1ª aula, a do meio e a última funciona sem crash.
- [ ] `aula_liberar_todas` deixa `curso->aulas = NULL` e `curso->total_aulas = 0`.
