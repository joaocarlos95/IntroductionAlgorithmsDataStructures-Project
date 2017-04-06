/************************************/
/* 	Grupo 20					    */
/*								    */
/* 	N 81988 - Pedro Miguel Campos  */
/* 	N 82528 - Joao Carlos Costa    */
/* 	N 82547 - Joao Bernardo Alves  */
/*								    */
/************************************/

#include <stdio.h>			 												
#include <stdlib.h>				

#define tamanho_nome 42													
#define numero_bancos 1000													
																			
void registaNovoBanco(); 													/* Cria um novo banco quer na estrutura, quer na matriz que regista os movimentos. */
void mudaClassificacao(int classificacao, int referencia); 					/* Promove ou desclassifica um banco 'bom' ou 'mau'. */
void emprestimo(int giver, int receiver, int valor); 						/* Faz um empréstimo de um banco A ao banco B. */
void amortizacao(int giver, int receiver, int valor); 						/* Faz uma amortização de um banco A ao banco B. */
void escolheTipo(int tipo);													/* Escolhe um dos três diferentes tipos de listagens. */
void listagemTipo0();														/* Dá-nos uma listagem simples da rede de bancos. */
void listagemTipo1();														/* Dá-nos uma listagem completa da rede de bancos. */
void listagemTipo2();														/* Dá-nos um histograma do número de parceiros. */
void final();																/* Sai do programa mostrando o número total de bancos e o número total de bancos bons. */
void despromoveBanco();														/* Despromove o banco em maiores dificuldades. */
int listagemTipo2_aux(int num_banco);										/* Função auxiliar para listagemTipo2. */	
void Reset1();																/* Reinicia a estrutura para uma nova actualizacao dos dados dos bancos. */
void Reset2();																/* Reinicia o vector auxiliar para despromover os bancos. */

typedef struct {
	char nome[tamanho_nome];
	int classificacao;
	int referencia;
	int inP;
	int outP;
	int outV;
	int outVM;
	int inV;
	int inVM;
	} estrutura;

estrutura banco[numero_bancos];
int new_bank, giver, receiver, valor, tipo;
int matriz[numero_bancos+1][numero_bancos+1];								
int vectordespromove[numero_bancos] = {0,};

/* Escolha do tipo de comando a utilizar com os bancos, pedindo apenas um de cada vez. */
int main() {
	
	int comando, referencia;

	new_bank = 0;

	while (1) {
		comando = getchar();
		switch (comando) {	
				case 'a':
					scanf("%s %d %d", banco[new_bank].nome, &banco[new_bank].classificacao, &banco[new_bank].referencia);
					registaNovoBanco();
					break;
				case 'k':
					scanf("%d", &referencia);
					mudaClassificacao(0, referencia);	
					break;
				case 'r':
					scanf("%d", &referencia);
					mudaClassificacao(1, referencia);
					break;
				case 'e':
					scanf("%d %d %d", &giver, &receiver, &valor);
					emprestimo(giver, receiver, valor);
					break;
				case 'p':
					scanf("%d %d %d", &giver, &receiver, &valor);
					amortizacao(giver, receiver, valor);
					break;
				case 'l':
					scanf("%d", &tipo);
					escolheTipo(tipo);
					break;
				case 'K':
					despromoveBanco();
					break;
				case 'x':
					final();
					return EXIT_SUCCESS;
		}
		getchar();
	}
return 0;
}

/************************/
/*  Funções Principais  */
/************************/

/* Comando 'a' */
/* Cria um novo banco na estrutura e na matriz (à qual faz corresponder a sua referência). */
void registaNovoBanco() {
	
	if (new_bank <= numero_bancos) {
		matriz[new_bank+1][0] = (matriz[0][new_bank+1] = banco[new_bank].referencia);
	}
	new_bank++;
}

/* Comando 'k' e 'r' */
/* Percorre a estrutura e caso 'classificacao' seja 0, o banco passará a ter uma classificação má, caso contrário */
/* passará a ter uma classificação boa (caso 'classificacao' seja 1). */
void mudaClassificacao(int classificacao, int referencia) {					

	int i, j;

	for (i = j = 0; (i < new_bank) || (j < new_bank); i++, j++) {
		if (classificacao != 0 && referencia == banco[i].referencia) {
			banco[i].classificacao = 1;
		}
		else if (referencia == banco[j].referencia) {
			banco[j].classificacao = 0;
		}
	}
}

/* Comando 'e' */
/* Percorre a primeira coluna da matriz, procurando o banco que vai emprestar o dinheiro, seguido da primeira linha */
/* da matriz para encontrar o banco que vai receber o empréstimo. Após descobrirem os bancos, irá ser actualizado */
/* o valor do empréstimo na respectiva coordenada. */
void emprestimo(int giver, int receiver, int valor) {						

	int i, j;																

	for (i = 1; i <= new_bank; i++) {
		if (matriz[i][0] == giver) {
			for (j = 1; j <= new_bank; j++) {
				if (matriz[0][j] == receiver) {
					matriz[i][j] += valor;
				}
			}
		}
	}
}

/* Comando 'p' */
/* Percorre a primeira coluna da matriz, procurando o banco que vai receber o dinheiro da amortização, seguido da primeira */
/* linha da matriz para procurar o banco que vai amortizar a sua dívida. Após descobrirem os bancos, irá ser actualizado */
/* o valor da amortização na respectiva coordenada. */
void amortizacao(int giver, int receiver, int valor) {	

	int i, j;																 

	for (i = 1; i <= new_bank; i++) {
		if (matriz[0][i] == receiver) {
			for (j = 1; j <= new_bank; j++) {
				if (matriz[j][0] == giver) {
					matriz[i][j] -= valor;
				}
			}
		}
	}
}

/* Comando 'l' */
/* Escolhe um dos três tipos que o comando 'l' pode tomar. */
void escolheTipo(int tipo) {												

	int f;

	if (tipo == 0) {
		listagemTipo0();
	}
	else if (tipo == 1) {
		listagemTipo1();
		for (f = 0; f < new_bank; f++) {
			printf("%d %s %d %d %d %d %d %d %d\n", banco[f].referencia, banco[f].nome, banco[f].classificacao, banco[f].inP, banco[f].outP, banco[f].outV, banco[f].outVM, banco[f].inV, banco[f].inVM);
		}
	}
	else if (tipo == 2) {
		listagemTipo2();
	}
}

/* Imprime a referência e o nome do banco acompanhados da classificação do mesmo. */
void listagemTipo0() {														

	int i;

		for (i = 0; i < new_bank; i++) {
			printf("%d %s %d\n", banco[i].referencia, banco[i].nome, banco[i].classificacao);
		}
}

/* Percorre a matriz para preencher as variáveis inP, inV, outP, outV, inVM, outVM com os devidos valores, */
/* fazendo Reset a esses valores sempre que a matriz seja alterada. Faz o print na função 'escolheTipo'. */
void listagemTipo1() {

	int i, j;

	Reset1();
	for (i = 0; i < new_bank; i++) {
		for (j = 0; j < new_bank; j++) {
			if (matriz[i+1][j+1] != 0) {
				banco[j].inP += 1;
				banco[j].inV += matriz[i+1][j+1];
				banco[i].outP += 1;
				banco[i].outV += matriz[i+1][j+1];
				if(banco[i].classificacao != 1) {
					banco[j].inVM += matriz[i+1][j+1];
				}
				if(banco[j].classificacao != 1) {
					banco[i].outVM += matriz[i+1][j+1];
				}
			}		
		}
	}
}	

/* Faz print do número de bancos com parceiros, isto é, com aqueles com quem tem um compromisso monetário. */
void listagemTipo2() {

	int i, j;
	int vectorbanco[numero_bancos] = {0,};

	for (i = 1; i <= new_bank; i++) {
		vectorbanco[listagemTipo2_aux(i)] += 1;
	}
	for (j = 0; j < new_bank; j++) {
		if (vectorbanco[j] != 0)
			printf("%d %d\n", j, vectorbanco[j]);
	}
}

/* Comando 'K' */
/* No primeiro ciclo, vai ser criado um vector que tomará os valores do outVM dos diferentes bancos. Se seguida, */
/* irá ser escolhido o valor mais alto (caso haja dois iguais, o do banco mais recente). */
/* Deverá devolver um print com o banco que foi desclassficado (actualizdo) e o número de bancos totais e bancos bons. */
void despromoveBanco() {

	int i, j, y = 0, x = 0, max = 0;
	
	Reset2();
	listagemTipo1();
	for (i = 0; i < new_bank; i++) {
		if (banco[i].classificacao != 0) {
			vectordespromove[i] = banco[i].outVM;
			if (vectordespromove[i] != 0) {
				x += 1;
			}
		}
	}
	if (x > 0) {
		for (j =0; j<new_bank;j++) {
    		if (vectordespromove[j] >= max) {
    			max = vectordespromove[j];
    			y = j;
    		}
    	}
    	mudaClassificacao(0, banco[y].referencia);
    	printf("*%d %s %d %d %d %d %d %d %d\n", banco[y].referencia, banco[y].nome, banco[y].classificacao, banco[y].inP, banco[y].outP, banco[y].outV, banco[y].outVM, banco[y].inV, banco[y].inVM);
   		final();
    }
    else {
    	final();
    }
 }

/* Comando 'X' */
/* Devolve o número de bancos totais e o númer de bancos bons, saindo de seguida do programa. */
void final() {

	int i, total_bom = 0;

	for (i = 0; i < new_bank; i++) {
		if (banco[i].classificacao != 0) {
			total_bom += 1;
		}
	}
	printf("%d %d\n", new_bank, total_bom);
}

/************************/
/*  Funções Auxiliares  */
/************************/

/* Devolve à função principal 'listagemTipo2' o número de bancos parceiros referente a um determinado banco. */
int listagemTipo2_aux(int num_banco) {

	int i, num_parceiros = 0;

	for (i = 1; i <= new_bank; i++) {
		if ((matriz[i][num_banco] != 0) || (matriz[num_banco][i] != 0 )) {
			num_parceiros += 1;
		}
	}
	return num_parceiros;
}

/* Coloca todos os parâmetros em questão a 0. */
void Reset1() {

	int i;

	for (i = 0; i < new_bank; i++) {
		banco[i].inP = 0;
		banco[i].inV = 0;
		banco[i].outP = 0;
		banco[i].outV = 0;
		banco[i].inVM = 0;
		banco[i].outVM = 0;	
	}
}

/* Coloca o vector auxiliar, que contém os outVM referentes aos bancos bons, todo a 0. */
void Reset2 () {

	int i;

 	for (i = 0; i < new_bank; i++) {
		vectordespromove[i] = 0;
	}
}