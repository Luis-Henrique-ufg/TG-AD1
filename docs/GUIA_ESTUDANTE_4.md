# Guia do Estudante 4 — Interface e Menu Principal (`src/interface.c` e `src/main.c`)

**Seus arquivos para editar:** `src/interface.c` e `src/main.c`  
**Contrato de referência:** [include/interface.h](../include/interface.h)  
**Total:** **6 funções curtas** em `interface.c` + loop do menu em `main.c`.

> **Foco:** apenas `printf` e `fgets` padrão! Não precisa de cores ANSI, não precisa de `<windows.h>` e não precisa de `scanf`.

---

## Passo a Passo em `src/interface.c`

### 1. `mostrar_barra_progresso(double progresso)` *(Comece por aqui!)*
- Calcule quantos `#` preencher (de 0 a 10):
  ```c
  int cheios = (int)(progresso / 10.0);
  if (cheios < 0) cheios = 0;
  if (cheios > 10) cheios = 10;

  printf("[");
  for (int i = 0; i < 10; i++) {
      printf("%c", (i < cheios) ? '#' : '-');
  }
  printf("]");
  ```

### 2. `ler_texto(const char *prompt, char *buffer, int tamanho)`
- Imprima `prompt` (se não `NULL`).
- Leia com `fgets(buffer, tamanho, stdin)`.
- Procure o `'\n'` com `strchr(buffer, '\n')`:
  - Se encontrou, troque por `'\0'`.
  - Se **não** encontrou (usuário digitou além do limite), limpe o resto da linha com:
    ```c
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}
    ```

### 3. `ler_inteiro(const char *prompt, int min, int max)`
- Faça um `while (1)` que chama `ler_texto(prompt, buf, sizeof(buf))` e converte com `strtol(buf, &fim, 10)`.
- Só retorne o número se `fim != buf && *fim == '\0' && valor >= min && valor <= max`.
- Caso contrário, imprima `"Valor invalido. Digite um numero entre %d e %d.\n"` e repita!

### 4. `exibir_dashboard(const Plataforma *plat)`
- Imprima o cabeçalho e o total de cursos (`plat->total_cursos`).
- Se `plat->inicio == NULL`, imprima `"  (Nenhum curso cadastrado ainda)\n"`.
- Senão, percorra `plat->inicio` numerando `#1`, `#2`, ..., imprimindo:
  - `curso->nome`
  - `mostrar_barra_progresso(aula_calcular_progresso(curso))`
  - porcentagem e `(%d aulas)`
- Imprima o menu de opções `1..6` e `0. Sair` (igual ao mockup do [README.md](../README.md)).

### 5. `exibir_curso(const Curso *curso)`
- Imprima o nome do curso, a barra de progresso e o percentual.
- Percorra `curso->aulas` numerando `1.`, `2.`, ..., exibindo `[x]` se `aula->concluida` ou `[ ]` se não, o título, a duração em minutos e a anotação (se `aula->anotacao[0] != '\0'`).
- Imprima o menu `1..4` e `0. Voltar`.

### 6. `carregar_dados_exemplo(Plataforma *plat)`
- Cadastre 3 cursos prontos usando `curso_criar`, `aula_criar` e `aula_alternar_concluida` (ex.: *"Estruturas de Dados em C"*, *"Banco de Dados SQL"* e *"Algoritmos de Ordenacao"*).
- Assim, assim que o professor rodar o programa, já haverá dados reais para testar todas as opções!

---

## Passo a Passo em `src/main.c`

1. Crie uma função auxiliar `static void menu_curso(Curso *curso)`:
   - Loop `while (1)`:
     - Chama `exibir_curso(curso);`
     - Lê `opcao = ler_inteiro("Opcao: ", 0, 4);`
     - `case 1`: lê título, duração (`ler_inteiro("Duracao (min): ", 0, 1000)`) e anotação → chama `aula_criar`.
     - `case 2`: se `curso->total_aulas > 0`, pede o número da aula → `aula_buscar_por_indice` → `aula_alternar_concluida`.
     - `case 3`: se `curso->total_aulas > 0`, pede o número da aula → `aula_remover`.
     - `case 4`: chama `ordenar_aulas_por_duracao(curso)`.
     - `case 0`: `return;` (volta ao Dashboard).
2. No `main(void)`:
   - Chama `plataforma_inicializar(&plat);` e `carregar_dados_exemplo(&plat);`.
   - Loop `while (1)`:
     - Chama `exibir_dashboard(&plat);`
     - Lê `opcao = ler_inteiro("Opcao: ", 0, 6);`
     - `case 1`: pede o número `#` do curso → `curso_buscar_por_indice` → `menu_curso(curso)`.
     - `case 2`: lê nome do curso → `curso_criar(&plat, nome)`.
     - `case 3`: pede o número `#` → `curso_remover(&plat, num)`.
     - `case 4`: `ordenar_cursos_por_nome(&plat)`.
     - `case 5`: `ordenar_cursos_por_progresso(&plat)`.
     - `case 6`: lê termo → `buscar_curso_por_nome(&plat, termo)` → se encontrou, exibe o nome (ou abre `menu_curso`).
     - `case 0`: sai do loop (`break;`).
   - Chama `plataforma_liberar(&plat);` antes do `return 0;`.

---

## Checklist do Estudante 4
- [ ] Compila sem warnings.
- [ ] Digitar letras (ex.: `"abc"`) quando pede número não trava o programa em loop infinito.
- [ ] Todas as opções do Dashboard (`0..6`) e da Tela do Curso (`0..4`) funcionam corretamente.
- [ ] Ao escolher `0. Sair`, `plataforma_liberar(&plat)` é chamado antes de encerrar.
