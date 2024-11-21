/* Guilherme Ferreira de Ávila - 2311153 */
/* Pedro Carneiro Nogueira - 2310540 */

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

/* Protótipos */
void escreveValorRDI(unsigned char codigo[], int* atual, char* valor, TipoValor tipo);
void escreveValorRSI(unsigned char codigo[], int* atual, char* valor, TipoValor tipo);
void escreve_f_rax(unsigned char codigo[], int* atual, unsigned char f[]);

/* Função Principal */
void cria_func(void* f, DescParam params[], int n, unsigned char codigo[]) {

    int atual = 0; // setando um contador para a escrita das instruções

    /* Escrevendo prologo */
    /* pushq %rbp */
    codigo[atual++] = 0x55; 

    /* movq %rsp, %rbp */
    codigo[atual++] = 0x48; 
    codigo[atual++] = 0x89;
    codigo[atual++] = 0xe5;

    escreve_f_rax(codigo, &atual, f);

    if (n == 1) {

        DescParam param1 = params[0];

        if (param1.orig_val == FIX) {
            /* movq $valor, %rdi OU movl $valor, %edi */
            if (param1.tipo_val == INT_PAR) {
                escreveValorRDI(codigo, &atual, &param1.valor.v_int, INT_PAR);
            } else {
                escreveValorRDI(codigo, &atual, &param1.valor.v_ptr, PTR_PAR);
            }

        } else if (param1.orig_val == IND) {

            int* ind_ptr = (int*)param1.valor.v_ptr;
            int ind = *ind_ptr;
            escreveValorRDI(codigo, &atual, &ind, INT_PAR);

        } 

    } else if (n == 2) {

        DescParam param1 = params[0];
        DescParam param2 = params[1];

        if ( param1.orig_val != PARAM && param2.orig_val == PARAM) {

            /* 
            a nova função terá apenas 1 parametro 
            assim, temos que pegar esse primeiro parametro 
            e botar no segundo parametro para chamar a função original 
            */

            /* movq %rdi, %rsi */
            codigo[atual++] = 0x48;
            codigo[atual++] = 0x89;
            codigo[atual++] = 0xfe;

            if (param1.orig_val == FIX) {
                /* movq $valor, %rdi OU movl $valor, %edi */
                if (param1.tipo_val == INT_PAR) {
                    escreveValorRDI(codigo, &atual, &param1.valor.v_int, INT_PAR);
                } else {
                    escreveValorRDI(codigo, &atual, &param1.valor.v_ptr, PTR_PAR);
                }

            } else if (param1.orig_val == IND) {

                int* ind_ptr = (int*)param1.valor.v_ptr;
                int ind = *ind_ptr;
                escreveValorRDI(codigo, &atual, &ind, INT_PAR);

            }

        } else if (param1.orig_val == PARAM && param2.orig_val != PARAM) {

            if (param2.orig_val == FIX) {
                /* movq $valor, %rdi OU movl $valor, %edi */
                if (param2.tipo_val == INT_PAR) {
                    escreveValorRDI(codigo, &atual, &param2.valor.v_int, INT_PAR);
                } else {
                    escreveValorRDI(codigo, &atual, &param2.valor.v_ptr, PTR_PAR);
                }

            } else if (param2.orig_val == IND) {

                int* ind_ptr = (int*)param2.valor.v_ptr;
                int ind = *ind_ptr;
                escreveValorRDI(codigo, &atual, &ind, INT_PAR);

            }
        }
    }

    /* call *%rax */
    codigo[atual] = 0xff;
    codigo[atual++] = 0xd0;

    codigo[atual++] = 0xc9; /* leave */
    codigo[atual++] = 0xc3; /* ret */

    return;
}

/* Funções Auxiliares */
void escreve_f_rax(unsigned char codigo[], int* atual, unsigned char f[]) {

    codigo[(*atual)++] = 0x48;
    codigo[(*atual)++] = 0xc7;
    codigo[(*atual)++] = 0xc0;

    // Copie byte a byte para o array de código na posição atual
    for (int i = 0; i < 8; i++) {
        codigo[(*atual)++] = f[i]; // Extrai o byte correspondente
    }
}

void escreveValorRDI(unsigned char codigo[], int* atual, char* valor, TipoValor tipo) {

    

    if (tipo == INT_PAR) {

        codigo[(*atual)++] = 0xbf;
    
        for (int i = 0; i < 4; i++) {
            codigo[(*atual)++] = valor[i];
        }

    } else {

        codigo[(*atual)++] = 0x48;
        codigo[(*atual)++] = 0xc7;
        codigo[(*atual)++] = 0xc7;
    
        for (int i = 0; i < 8; i++) {
            codigo[(*atual)++] = valor[i];
        }

    }

    return;
}

void escreveValorRSI(unsigned char codigo[], int* atual, char* valor, TipoValor tipo) {

    if (tipo == INT_PAR) {

        codigo[(*atual)++] = 0xbe;
    
        for (int i = 0; i < 4; i++) {
            codigo[(*atual)++] = valor[i];
        }

    } else {

        codigo[(*atual)++] = 0x48;
        codigo[(*atual)++] = 0xc7;
        codigo[(*atual)++] = 0xc6;
    
        for (int i = 0; i < 8; i++) {
            codigo[(*atual)++] = valor[i];
        }

    }

    return;
}