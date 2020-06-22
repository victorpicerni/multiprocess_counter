/* Contador de palavras
 *
 * Este programa recebera uma serie de caracteres representando palavras em sua
 * entrada. Ao receber um caractere fim de linha ('\n'), deve imprimir na tela o
 * numero de palavras separadas que recebeu e, apos, encerrar.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> //define o tipo pid_t
#include <unistd.h> //define fork
#include <sys/wait.h> //define o waitpid
#include <sys/mman.h>

#define MAX_PROCESSOS 4


/*função para definir se o numero é primo ou nao*/
//definindo numeros primos. Se o retorno for 0 o numero não é primo, se for 1 o numero é primo

unsigned long int primo(int num){
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
  pid_t pid;

/* Definir flags de protecao e visibilidade de memoria */
  int protecao = PROT_READ | PROT_WRITE;
  int visibilidade = MAP_SHARED | MAP_ANON;
  
/*definindo variaveis auxiliares*/
  int num;
  char c;
  int qte_palavras = 0;
  int total_processos = 0;
  int status;

/*criando duas memorias compartilhadas, uma para o vetor de numeros e outra para os valores de reultados e informações de leitura*/
  mem_compartilhada = (int*)mmap(NULL, sizeof(int)*1000, protecao,visibilidade,0,0);
  mem_auxiliar = (int*)mmap(NULL, sizeof(int)*1000, protecao,visibilidade,0,0);
  resultado = &(mem_auxiliar[0]);
  palavras_processadas = &(mem_auxiliar[1]);
  (*palavras_processadas) = 0;
  
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
  for (int i = 0; i<qte_palavras; i++){
  	total_processos++;
	for (;total_processos>=MAX_PROCESSOS;total_processos--)
	{
	wait(&status);	
	waitpid(pid,NULL,0);
	}

	pid = fork();/*inicia novos processos*/

	if (pid==0) {/*para os processos filhos, temos pid = 0*/
			
	(*resultado)+=(primo(mem_compartilhada[(*palavras_processadas)]));/*irá verificar se o numero cujo processo buscou na memoria compartilhada é primo*/
	(*palavras_processadas)++;/*avança na memória compartilhada*/
	exit(0);/*termina o processo*/
	}
}

while ((wait(&status) > 0));/*espera todos os processos terminarem*/

printf("%d\n", (*resultado));/*printa o resultado*/
  return 0;/*finaliza o programa*/
}


