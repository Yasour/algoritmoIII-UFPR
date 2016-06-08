#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dicionario.h"

void *mallocc (size_t nbytes){          /* Função que avisa caso malloc devolva NULL */
  void *ptr;
  ptr = malloc (nbytes);
  if (ptr == NULL) {
    printf ("Socorro! malloc devolveu NULL!\n");
    exit (EXIT_FAILURE);
  }
  return ptr;
}

FILE* abreDicio (){                     /* Abre FILE do Dicionario corretamente */
  FILE *entrada;
  entrada = fopen ("dicionario.txt", "r");
  if (entrada == NULL) {
      printf("\nNão encontrei o arquivo dicionario.txt.\n");
      exit (EXIT_FAILURE);
  }
  return entrada;
}

int* maiuscMinusc(int* palavra){       /* Troca Maiúsculas -> Minúsculas */
  int i;
  for (i = 0; palavra [i] != 10; i++){


    if (palavra [i] >=65 && palavra [i] <= 90 ){
      palavra [i] += 32 ;
    }
  }
  return palavra;
}

int conferePalavra(int* palavra){
  int i, flag;
  palavra = maiuscMinusc (palavra);

  flag = 1;
  for (i = 0; palavra[i] != 10 ; i++){
    if (palavra [i] < 97 || palavra [i] > 123){  /* Caso seja diferente de letras minusculas e pula linha */
      flag = 0;
    }
  }

  if (flag == 1){
    return 1; /* CERTO */
  } else {
    return 0; /* ERRADO */
  }

}

int *copiaFile(FILE *entrada, int* palavra){         /* .txt -> vetor */

  int i, j;
  i = 0;

  while (i < 30){
    palavra [i] = fgetc (entrada);  /* copie do .txt 1 char */

    if (palavra [i] == EOF){
      palavra[0] = 10;
      return palavra;
    }
    if (palavra [i] == 10) {         /* fim FILE ou fim palavra */

      j = conferePalavra(palavra);
      if (j == 0){
        i = 0;
        palavra [i] = fgetc (entrada);
        j = conferePalavra(palavra);
      }else
      if (j == 1){
        return palavra;         /* Aqui fará com que saia do laço WHILE */
      }
    }
    i++;
  }

  return palavra;                     /* Vetor com o trecho */
}


nodo* criaNo(){           /* somente cria */
  int i ;
  nodo* no;

  no = mallocc (sizeof(nodo));

  for (i = 0; i < 27; i++){
    no -> letra [i] = NULL ;             /* valor inicial do vetor 0 */
  }

  no -> filhos = 0;
  no -> pai = NULL;

  return no;                        /* retornará o endereço raiz */
}

raiz *criaTrie(){                     /* Função cria Raíz Trie */
  raiz *root;
  nodo* no;
  root = mallocc (sizeof(raiz));
  no = criaNo();
  root -> primeiro = no;

  return root;
}


int inserePalavra(nodo* N, int* palavra){    /* Recebe vetor com lista de palavras*/
  int i, pos;
  nodo *auxNodo;   /* vou inserir o primeiro nodo indicado pela RAIZ */
  nodo *novoNodo;

  auxNodo = N;

  for (i = 0; palavra[i] != 10; i++){
    pos = INDICE_CHAR (palavra [i]);    /* posição 00~26*/

    if (auxNodo -> letra [pos]){        /* se existir */
      auxNodo = auxNodo -> letra [pos];
    } else {
      novoNodo = criaNo();
      auxNodo -> letra [pos] = novoNodo;
      novoNodo -> pai = auxNodo;
      auxNodo -> filhos++;        /* agora tem 1 filho */
      auxNodo = novoNodo;
    }
  }

  if (!auxNodo -> letra [26]){    /* função que indica se há filhos ou não */
    auxNodo -> letra [26] = (nodo*) 1;
    auxNodo -> filhos++;
  }

  return 1;
}

/* Função auxiliar que acha o final de palavra */
nodo *get(nodo *N, int *palavra){
  int i, pos;
  nodo *pointer;

  i = 0;

  pointer = N;
  if (palavra [i] == 10){
    return NULL;
  } else {
    while ((palavra [i] != 10) && (pointer)){
      pos = INDICE_CHAR (palavra [i]);
      pointer = pointer -> letra [pos];
      i++;
    }
  }
  return pointer;
}

int procura(nodo *N, int *palavra){
  nodo *aux;

  aux = get(N,palavra);

  if ((aux) && (aux -> letra [26])){
    return 1; /* tem */
  }
  return 0; /* não tem */
}

int encheTrie(){
    int i, fim,*palavra;   /* Vetor aonde será copiado um trecho do arq */
    raiz *root;
    nodo *no;
    FILE *entrada;

    root = criaTrie ();
    no = root -> primeiro;

    entrada = abreDicio();
    palavra = mallocc (30 * sizeof (int));

    palavra[0] = 1;

    for(i = 0; palavra[0] != 10 ; i++){

      palavra = copiaFile(entrada,palavra); /* extrai 1 palavra de FILE */
      inserePalavra (no,palavra);   /* Insere a palavra na TRIE */

      /* IMPRIME PALAVRA POR PALAVRA */
    /*  for (fim = 0; palavra [fim] != 10; fim++){
        printf("%c",palavra[fim]);
      };
      printf ("\n");*/
    }
    fclose (entrada);
    free (palavra);
    return i;
}
