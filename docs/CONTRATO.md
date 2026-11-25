# Contrato do Projeto — EAD-CLI (Versão Enxuta)

Este documento resume as regras simples que os **4 estudantes** devem respeitar para que todos os módulos se encaixem perfeitamente na hora de juntar o código.

---

## 1. Desenho das Estruturas de Dados

Usamos apenas **listas simplesmente encadeadas** (cada nó aponta só para o próximo via `next`):

```text
Plataforma
 └── inicio ──► [Curso #1] ──► [Curso #2] ──► NULL
                    │
                    └── aulas ──► [Aula 1] ──► [Aula 2] ──► NULL
```

- Quando não há cursos: `plat->inicio == NULL` e `plat->total_cursos == 0`.
- Quando um curso não tem aulas: `curso->aulas == NULL` e `curso->total_aulas == 0`.
- O último elemento de qualquer lista sempre tem `next == NULL`.

---

## 2. Regras de Memória (`malloc` e `free`)

| O que é alocado | Quem dá `malloc` | Quem dá `free` |
| :--- | :--- | :--- |
| `Curso` | `curso_criar()` (Estudante 1) | `curso_remover()` e `plataforma_liberar()` (Estudante 1) |
| `Aula` | `aula_criar()` (Estudante 2) | `aula_remover()` e `aula_liberar_todas()` (Estudante 2) |

**Regra de ouro ao remover um Curso:**
Antes de chamar `free(curso)`, o Estudante 1 **sempre** chama `aula_liberar_todas(curso)`. Assim nenhuma aula fica perdida na memória!

---

## 3. Convenções Combinadas

1. **Índices sempre começam em `1`** (`#1`, `#2`, `#3`...), igual ao que o usuário vê na tela.
   - Se o usuário passar `indice < 1` ou `indice > total`, a função retorna `NULL` (ou `false`).
2. **Retornos padronizados:**
   - Funções `bool`: `true` = deu certo, `false` = erro ou não encontrado.
   - Funções de ponteiro (`Curso *`, `Aula *`): endereço válido = deu certo, `NULL` = erro ou não encontrado.
3. **Cópia segura de strings:**
   Sempre use:
   ```c
   strncpy(destino, origem, tamanho - 1);
   destino[tamanho - 1] = '\0';
   ```
4. **Sem `printf` fora da interface:**
   `cursos.c`, `aulas.c` e `ordenacao.c` **não** imprimem nada na tela. Apenas `interface.c` e `main.c` usam `printf` e `fgets`.

---

## 4. Quem Depende de Quem

```text
main.c (Estudante 4)
 ├── interface.h (Estudante 4) ──► cursos.h, aulas.h
 ├── ordenacao.h (Estudante 3) ──► aulas.h (apenas aula_calcular_progresso)
 ├── cursos.h    (Estudante 1) ──► aulas.h (apenas aula_liberar_todas)
 └── aulas.h     (Estudante 2) ──► apenas models.h
```

Todos os 4 conseguem programar **ao mesmo tempo** porque os cabeçalhos em `include/` já existem e já compilam!
