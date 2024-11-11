# INF1018 - Software Básico (2024.2)

## Segundo Trabalho: Gerador Dinâmico de Funções

### Objetivo

O objetivo deste trabalho é implementar em C uma função `cria_func`, que recebe o endereço de uma função `f` e a descrição de um conjunto de parâmetros. A função `cria_func` deverá gerar, dinamicamente, o código de uma "nova versão" de `f` e gravar na região de memória especificada.

---

### Instruções Gerais

- Leia com atenção o enunciado do trabalho e as instruções para a entrega.
- Em caso de dúvidas, não invente. Pergunte!
- O trabalho deve ser entregue até a data marcada no EaD.
- Trabalhos entregues com atraso perderão um ponto por dia (ou fração) de atraso.
- Trabalhos que não compilem no ambiente Linux não serão considerados (ou seja, receberão grau zero).
- Os trabalhos podem ser feitos em grupos de no máximo dois alunos.
- Alguns grupos poderão ser chamados para apresentações orais / demonstrações dos trabalhos entregues.

### Amarrando Parâmetros

O propósito de gerarmos dinamicamente uma "nova versão" de uma função `f` é podermos "amarrar" valores pré-determinados a um ou mais dos parâmetros de `f`. Dessa forma, não precisaremos passar esses valores como argumentos quando chamarmos a nova versão gerada.

Exemplo: Considere uma função que retorna o produto de seus dois parâmetros:

```c
int mult(int x, int y);
```

A função `cria_func` permite criar dinamicamente uma nova função, baseada em `mult`, que sempre retorna o valor de seu parâmetro multiplicado por 10. Para criar essa nova função, `cria_func` amarra o segundo parâmetro de `mult` a um valor fixo (10). Ou seja, `cria_func` constrói, em tempo de execução, o código de uma nova função que chama `mult`, passando dois argumentos: o primeiro é o argumento recebido por essa nova função, e o segundo é o valor constante 10.

---

### Especificação das Funções

A função `cria_func` deve ter o protótipo:

```c
void cria_func(void* f, DescParam params[], int n, unsigned char codigo[]);
```

- `f` é o endereço da função original a ser chamada pelo código gerado.
- `params` é um array que contém a descrição dos parâmetros para chamar essa função.
- `n` é o número de parâmetros descritos por `params`.
- `codigo` é um vetor onde deverá ser gravado o código gerado.

O número mínimo de parâmetros é 1 e o máximo é 3.

### Definição do Tipo DescParam

```c
typedef enum {INT_PAR, PTR_PAR} TipoValor;
typedef enum {PARAM, FIX, IND} OrigemValor;

typedef struct {
   TipoValor    tipo_val;  /* indica o tipo do parametro (inteiro ou ponteiro) */
   OrigemValor  orig_val;  /* indica a origem do valor do parametro */
   union {
     int v_int;
     void* v_ptr;
   } valor;         /* define o valor ou endereço do valor do parametro (quando amarrado/indireto) */
} DescParam;
```

- O campo `orig_val` indica se o parâmetro deve ser "amarrado" ou não:
  - `PARAM`: o parâmetro não é amarrado, ou seja, deve ser recebido pela nova função e repassado à função original.
  - `FIX`: o parâmetro deve ser amarrado a um valor constante, fornecido no campo `valor`.
  - `IND`: o parâmetro deve ser amarrado a uma variável, cujo endereço é fornecido no campo `valor`.

O arquivo `cria_func.h` contém as definições acima e pode ser obtido AQUI. O trabalho deve seguir estritamente as definições constantes nesse arquivo.

---

### Implementação

A função `cria_func` deve ser implementada em C, gerando no espaço de memória do vetor `codigo` um trecho de código em linguagem de máquina que corresponda à nova função. Para facilitar, use a instrução `call` indireto, onde o endereço da função a ser chamada está em um registrador, por exemplo:

```assembly
call *%rax
```

De forma geral, `cria_func` irá percorrer o array com a descrição dos parâmetros e gerar um código em linguagem de máquina que:

- alinhe a pilha (usando o prólogo); isto é necessário porque poderemos chamar funções de biblioteca!
- coloque os valores a serem passados para a função original nos registradores correspondentes, respeitando os tipos e eventuais valores amarrados especificados no array params;

#### Atenção: lembre-se que a localização (registradores) dos parâmetros não amarrados, recebidos pela função gerada dinamicamente, pode não ser a mesma para a chamada à função original. Cuidado para não perder o valor desses parâmetros!

- chame a função original usando a instrução `call` (indireto);
  desfaça o registro de ativação `leave`
- retorne o controle para seu chamador, mantendo inalterado qualquer valor de retorno da função original.
- Você deve criar um arquivo fonte chamado `cria_func.c` contendo as função `cria_func` e funções auxiliares, se for o caso. Esse arquivo não deve conter uma função main nem depender de arquivos de cabeçalho além de `cria_func.h` e dos cabeçalhos das bibliotecas padrão!

Para testar o seu programa, crie um outro arquivo, por exemplo `teste.c`, contendo a função main. Crie seu programa executável, por exemplo teste, com a linha

```
gcc -Wall -Wa,--execstack -o teste cria_func.c teste.c
```

(sem a opção execstack , o sistema operacional abortará o seu programa, por tentar executar um código armazenado na área de dados).

Para testar seu programa, use técnica de TDD (Test Driven Design), na qual testes são escritos antes do código. O propósito é garantir ao desenvolvedor (você) ter um bom entendimento dos requisitos do trabalho antes de implementar o programa. Com isto a automação de testes é praticada desde o início do desenvolvimento, permitindo a elaboração e execução contínua de testes de regressão.

Desta forma fortalecemos a criação de um código que nasce simples, testável e próximo aos requisitos do trabalho. Os passos gerais para seguir tal técnica:

- Escrever/codificar um novo teste
- Executar todos os testes criados até o momento para ver se algum falha
- Escrever o código responsável por passar no novo teste inserido
- Executar todos os testes criados até o momento e atestar execução com sucesso
- Refatorar código testado

---

### Dicas de Implementação

Recomendamos fortemente uma implementação gradual, desenvolvendo e testando passo-a-passo cada nova funcionalidade implementada.

Comece, por exemplo, com um esqueleto que declare como variável local da main um vetor unsigned char de tamanho suficientemente grande, coloca um código bem conhecido nessa região. Teste a chamada a essa função gerada dinamicamente. Para obter um código "bem conhecido" você pode compilar um arquivo assembly contendo uma função bem simples (que, por exemplo, retorna o valor do seu parâmetro) usando:

```
minhamaquina > gcc -c code.s
```

A opção -c é usada para compilar e não gerar o executável. Depois de compilar, veja o código de máquina gerado usando:

```
minhamaquina > objdump -d code.o
```

A seguir, implemente a geração dinamica do código, e comece a testá-la. Por exemplo, comece usando cria_func para gerar um código que chama uma função que retorna o valor de seu único parâmetro inteiro. Teste primeiro sem amarrar o parâmetro, e depois o amarrando a um valor fixo e ao valor de uma variável.

Vá então aumentando gradualmente o número e os tipos de parâmetros tratados, testando combinações diferentes. Você pode usar os exemplos dados no enunciado do trabalho, mas faça também outros testes!

---

### Entrega

Leia com atenção as instruções para entrega a seguir e siga-as estritamente. Atenção para os nomes e formatos dos arquivos!

Devem ser entregues via EAD dois arquivos:

1. o arquivo fonte `cria_func.c`

Coloque no início do arquivo fonte, como comentário, os nomes dos integrantes do grupo da seguinte forma:

```
/* Nome_do_Aluno1 Matricula Turma */
/* Nome_do_Aluno2 Matricula Turma */
```

Lembre-se que esse arquivo não deve conter a função main!

2. um arquivo texto (não envie um .doc ou .docx), chamado relatorio.txt, contendo um pequeno relatório que descreva os testes realizados.
   Esse relatório deve explicar o que está funcionando e o que não está funcionando. Mostre exemplos de testes executados com sucesso e os que resultaram em erros (se for o caso).

Coloque também no relatório os nomes dos integrantes do grupo.

Coloque na área de texto da tarefa do EAD os nomes e turmas dos integrantes do grupo.
