# Fluxo de Git (4 Estudantes) e Dicas de C

---

## 1. Como Trabalhar em 4 Pessoas sem Conflito no Git

Cada estudante cria sua própria branch e edita **somente o seu arquivo `.c`**:

- **Estudante 1:** branch `estudante1/cursos` → edita `src/cursos.c`
- **Estudante 2:** branch `estudante2/aulas` → edita `src/aulas.c`
- **Estudante 3:** branch `estudante3/ordenacao` → edita `src/ordenacao.c`
- **Estudante 4:** branch `estudante4/interface` → edita `src/interface.c` e `src/main.c`

### Comandos do dia a dia
```bash
# 1. Atualizar a main antes de começar
git checkout main
git pull origin main

# 2. Criar sua branch (apenas na primeira vez)
git checkout -b estudante1/cursos

# 3. Salvar seu progresso
git add src/cursos.c
git commit -m "Implementa criar e remover cursos"

# 4. Enviar para o GitHub e abrir Pull Request para a main
git push -u origin estudante1/cursos
```

> **Atenção:** não altere arquivos da pasta `include/` sem combinar com os outros 3 colegas primeiro.

---

## 2. Erros Mais Comuns em C (e Como Evitar)

| Erro comum | Como fazer do jeito certo |
| :--- | :--- |
| Usar `scanf("%d", &x)` e o programa pular a próxima pergunta | Use sempre `ler_texto()` e `ler_inteiro()` (com `fgets`) |
| Dar `free(atual)` antes de pegar `atual->next` | Salve antes: `No *prox = atual->next; free(atual); atual = prox;` |
| Esquecer de checar `malloc` | Sempre escreva `if (novo == NULL) return NULL;` |
| Dividir por zero quando `total_aulas == 0` | Sempre cheque `if (curso->total_aulas == 0) return 0.0;` |
| Usar `strcasestr` (não funciona no Windows!) | Compare caractere por caractere usando `tolower((unsigned char)c)` |
| Esquecer `\0` ao usar `strncpy` | Sempre coloque `destino[TAM - 1] = '\0';` logo após o `strncpy` |
