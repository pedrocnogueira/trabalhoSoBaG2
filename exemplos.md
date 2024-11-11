# Exemplos de Uso

## Exemplo 1: Multiplicação com Parâmetro Fixado

```c
#include <stdio.h>
#include "cria_func.h"

typedef int (*func_ptr) (int x);

int mult(int x, int y) {
  return x * y;
}

int main (void) {
  DescParam params[2];
  func_ptr f_mult;
  int i;
  unsigned char codigo[500];

  params[0].tipo_val = INT_PAR; /* o primeiro parâmetro de mult é int */
  params[0].orig_val = PARAM;   /* a nova função repassa seu parâmetro */

  params[1].tipo_val = INT_PAR; /* o segundo parâmetro de mult é int */
  params[1].orig_val = FIX;     /* a nova função passa para mult a constante 10 */
  params[1].valor.v_int = 10;

  cria_func(mult, params, 2, codigo);
  f_mult = (func_ptr) codigo;

  for (i = 1; i <= 10; i++) {
    printf("%d\n", f_mult(i)); /* a nova função só recebe um argumento */
  }

  return 0;
}
```

---

## Exemplo 2: Multiplicação com Parâmetros Amarrados

```c
#include <stdio.h>
#include "cria_func.h"

typedef int (*func_ptr) ();

int mult(int x, int y) {
  return x * y;
}

int main (void) {
  DescParam params[2];
  func_ptr f_mult;
  int i;
  unsigned char codigo[500];

  params[0].tipo_val = INT_PAR; /* a nova função passa para mult um valor inteiro */
  params[0].orig_val = IND;     /* que é o valor corrente da variável i */
  params[0].valor.v_ptr = &i;

  params[1].tipo_val = INT_PAR; /* o segundo argumento passado para mult é a constante 10 */
  params[1].orig_val = FIX;
  params[1].valor.v_int = 10;

  cria_func(mult, params, 2, codigo);
  f_mult = (func_ptr) codigo;

  for (i = 1; i <= 10; i++) {
    printf("%d\n", f_mult()); /* a nova função não recebe argumentos */
  }

  return 0;
}
```

---

## Exemplo 3: Comparação de Prefixos com memcmp

```c
#include <stdio.h>
#include <string.h>
#include "cria_func.h"

typedef int (*func_ptr) (void* candidata, size_t n);

char fixa[] = "quero saber se a outra string é um prefixo dessa";

int main (void) {
  DescParam params[3];
  func_ptr mesmo_prefixo;
  char s[] = "quero saber tudo";
  int tam;
  unsigned char codigo[500];

  params[0].tipo_val = PTR_PAR; /* o primeiro parâmetro de memcmp é um ponteiro para char */
  params[0].orig_val = FIX;     /* a nova função passa para memcmp o endereço da string "fixa" */
  params[0].valor.v_ptr = fixa;

  params[1].tipo_val = PTR_PAR; /* o segundo parâmetro de memcmp é também um ponteiro para char */
  params[1].orig_val = PARAM;   /* a nova função recebe esse ponteiro e repassa para memcmp */

  params[2].tipo_val = INT_PAR; /* o terceiro parâmetro de memcmp é um inteiro */
  params[2].orig_val = PARAM;   /* a nova função recebe esse inteiro e repassa para memcmp */

  cria_func(memcmp, params, 3, codigo);
  mesmo_prefixo = (func_ptr) codigo;

  tam = 12;
  printf("'%s' tem mesmo prefixo-%d de '%s'? %s\n", s, tam, fixa, mesmo_prefixo(s, tam) ? "NAO" : "SIM");
  tam = strlen(s);
  printf("'%s' tem mesmo prefixo-%d de '%s'? %s\n", s, tam, fixa, mesmo_prefixo(s, tam) ? "NAO" : "SIM");

  return 0;
}
```
