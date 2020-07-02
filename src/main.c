/* Contador de palavras
 *
 * Este programa recebera uma serie de caracteres representando palavras em sua
 * entrada. Ao receber um caractere fim de linha ('\n'), deve imprimir na tela o
 * numero de palavras separadas que recebeu e, apos, encerrar.
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h> //define o tipo pid_t
#include <unistd.h> //define fork
#include <sys/wait.h> //define o waitpid
#include <sys/mman.h>

#define MAX_PROCESSOS 4


/*função para definir se o numero é primo ou nao*/
//definindo numeros primos. Se o retorno for 0 o numero não é primo, se for 1 o numero é primo

int primo(int num){
  if (num == 1 || num == 0)
    return 0;
  for (int i = 2 ; i < num ; i++){
    if (num%i == 0){
    //printf("nao é primo %d\n", num);
    return 0;
  }
 }
return 1;
}

int main() {
/*definindo as variáveis para a memória compartilhada e pid*/  
  int *mem_compartilhada;
  int *mem_auxiliar;
  int *resultado;
  int *palavras_processadas;
  pid_t pid[100];

/* Definir flags de protecao e visibilidade de memoria */
  int protecao = PROT_READ | PROT_WRITE;
  int visibilidade = MAP_SHARED | MAP_ANON;
  
/*definindo variaveis auxiliares*/
  int num;

  char c;

  int qte_palavras = 0;


/*criando duas memorias compartilhadas, uma para o vetor de numeros e outra para os valores de reultados e informações de leitura*/

  mem_compartilhada = (int*)mmap(NULL, sizeof(int)*1000, protecao,visibilidade,0,0);

  mem_auxiliar = (int*)mmap(NULL, sizeof(int)*1000, protecao,visibilidade,0,0);

  resultado = &(mem_auxiliar[0]);

  palavras_processadas = &(mem_auxiliar[1]);

  
/*criando o vetor de numeros*/
  do{
  scanf("%d",&num);
  mem_compartilhada[qte_palavras] = num;
  qte_palavras++;
  }
  while (c = getchar() != '\n'); /*quando o scan f encontrar uma quebra de linha, ele para o loop*/

/*iniciando os processos*/

/*haverá um processo para cada palavra, de forma que ao chegar no maximo de processos, será necessário
esperar até que um processo tenha sido terminado*/
  int pai = getpid();

  for (int i = 0; i < qte_palavras; i++) {
	
	pid[i] = fork();


	waitpid(pid[i],0,0);
	(*resultado)+= primo(mem_compartilhada[(*palavras_processadas)]); /*identifica e soma os primos*/
	(*palavras_processadas)++;
	
	/*sair se nao for o processo pai*/		
	if (getpid() != pai){
		exit(0);  
	}
	
  }

	
  
  

  
  printf("%d\n", (*resultado));/*printa o resultado*/


  return 0;/*finaliza o programa*/
}


