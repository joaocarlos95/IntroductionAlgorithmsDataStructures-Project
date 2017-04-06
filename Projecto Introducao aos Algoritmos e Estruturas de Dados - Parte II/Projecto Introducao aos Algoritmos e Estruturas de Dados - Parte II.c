/************************************/
/* 	Grupo 20					    */
/*								    */
/* 	N 81988 - Pedro Miguel Campos   */
/* 	N 82528 - Joao Carlos Costa     */
/* 	N 82547 - Joao Bernardo Alves   */
/*								    */
/************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define key(A) (A) 
#define less(A, B) (key(A) < key(B)) 



typedef struct cheque {
	int valor;
	long int refe;
	long int refb;
	long int refc;
	} *Item;

typedef struct Pool *Link;

struct Pool {
	Item item;
	Link next;
	};

static Link Head;
static Link Tail;

typedef struct cliente {
	long int ref;
	int nche;
	int vche;
	int nchb;
	int vchb;
} *ItemCliente;

typedef struct AVLCliente *LinkCliente;
struct AVLCliente {
	ItemCliente itemCliente;
	LinkCliente Left;
	LinkCliente Right;
	int Profundidade;
	};

static LinkCliente HeadCliente;



void inicializa();
Item criaCheque(int valor, long int refe, long int refb, long int refc);
Link novoCheque(Item item, Link next);
void insereCheque(Item item);
Link procuraCheque(Link Head, long int ref);
void removeCheque (long int ref);
void criaCliente(long int ref, int nche, int vche, int nchb, int vchb);
LinkCliente novoCliente(ItemCliente itemCliente, LinkCliente Left, LinkCliente Right);
LinkCliente procuraCliente(LinkCliente foundCliente, long int ref);
LinkCliente insereCliente(LinkCliente foundCliente, ItemCliente cliente);
LinkCliente removeCliente(LinkCliente foundCliente, ItemCliente itemCliente);
int numeroClientes(LinkCliente foundCliente);
void printClientes(LinkCliente foundCliente);
void verifica(long int ref);
void actualiza(long int ref, int EmissBenef, int AddRemov, int valor);
LinkCliente AVLMax(LinkCliente foundCliente);
int profundidadeAVL(LinkCliente foundCliente);
LinkCliente rotacaoEsq(LinkCliente foundCliente);
LinkCliente rotacaoDir(LinkCliente foundCliente);
LinkCliente rotacaoEsqDir(LinkCliente foundCliente);
LinkCliente rotacaoDirEsq(LinkCliente foundCliente);
int balance(LinkCliente foundCliente);
LinkCliente AVLbalance(LinkCliente foundCliente);
void freeArvore(LinkCliente HeadCliente);
void freeLista();




int main() {
	
	char comando[15];
	int valor, nch = 0, vch = 0;
	long int refe, refb, refc, ref;
	LinkCliente cliente;

	inicializa();
	scanf("%s", comando);
	
	while (strcmp(comando, "sair") != 0) {
		
		if (strcmp(comando, "cheque") == 0)	{
			Item cheque;
			scanf ("%d %ld %ld %ld", &valor, &refe, &refb, &refc);
			
			cheque = criaCheque (valor, refe, refb, refc);
			insereCheque (cheque);
			
			if (procuraCliente(HeadCliente, refe) == NULL)
				criaCliente (refe, 1, valor, 0, 0);
			else
				actualiza(refe, 0, 1, valor);
			if (procuraCliente(HeadCliente, refb) == NULL)
				criaCliente (refb, 0, 0, 1, valor);
			else
				actualiza(refb, 1, 1, valor);
		}

		else if (strcmp (comando, "processa") == 0) {
			if (Head == NULL)
				printf("Nothing to process\n");
			else {
				refc = Head -> item -> refc;
				refb = Head -> item -> refb;
				refe = Head -> item -> refe;
				valor = Head -> item -> valor;
				
				removeCheque (refc);
				actualiza(refe, 0, 0, valor);
				actualiza(refb, 1, 0, valor);
				verifica(refe);verifica(refb);
			}
		}

		else if(strcmp(comando, "processaR") == 0) {
			Link cheque;
			scanf("%ld", &ref);
			
			cheque = procuraCheque(Head, ref);
			
			if (cheque != NULL) { 
				refb = cheque -> item -> refb;
				refe = cheque -> item -> refe;
				valor = cheque -> item -> valor;
				
				removeCheque(ref);
				actualiza(refe, 0, 0, valor);
				actualiza(refb, 1, 0, valor);
				verifica(refe);verifica(refb);
			}
			else
				printf("Cheque %ld does not exist\n", ref);
		}

		else if (strcmp(comando, "infocheque") == 0) {
			Link foundCheque;
			scanf ("%ld", &ref);

			foundCheque = procuraCheque (Head, ref);
			printf ("Cheque-info: %ld %d %ld --> %ld\n", foundCheque -> item -> refc,
				foundCheque -> item -> valor, foundCheque -> item -> refe,
				foundCheque -> item -> refb);
		}

		else if(strcmp(comando, "infocliente") == 0) {
			scanf("%ld", &ref);
		
			cliente = procuraCliente(HeadCliente, ref);
		
			if (cliente != NULL)
					printf("Cliente-info: %ld %d %d %d %d\n", cliente -> itemCliente -> ref,
						cliente -> itemCliente -> nche, cliente -> itemCliente -> vche,
						cliente -> itemCliente -> nchb, cliente -> itemCliente -> vchb);
		}

		else if(strcmp(comando, "info") == 0) {
			if (numeroClientes(HeadCliente) == 0)
				printf("No active clients\n");
			else
				printClientes(HeadCliente);
        }

		else {
			printf(" O comando introduzido e desconhecido. Introduza novamente.\n");
		}
	
		scanf("%s", comando);
	}

	while (Head != NULL) {
		nch++;
		vch += Head->item->valor;
		Head = Head->next;
	}
	printf("%d %d %d\n", numeroClientes(HeadCliente), nch, vch);
	freeLista(Head);
	freeArvore(HeadCliente);
	
	return 0;
}




/* Funcao que vai inicializar a Head e Tail da lista de cheques, */
/* e a Head da arvore de clientes */
void inicializa() {

	Head = NULL;
	Tail = NULL;
	HeadCliente = NULL;
}


/************************************/
/*  Funcoes referentes aos Cheques  */
/************************************/

/* Comando 'cheque' */
/* Funcao que vai criar um cheque (adicionar as suas novas referencias e valor) */
Item criaCheque(int valor, long int refe, long int refb, long int refc) {

	Item newCheque = (Item) malloc(sizeof(struct cheque));

	newCheque -> valor = valor;
	newCheque -> refe = refe;
	newCheque -> refb = refb;
	newCheque -> refc = refc;

	return newCheque;
}

/* Comando 'cheque' */
/* Funcao que vai adicionar o novo cheque a lista ligada, colocando um apontador */
/* para a estrutura que contem as referencias e valor, e um apontador para o proximo */
/* cheque na lista. */
Link novoCheque(Item item, Link next) {

	Link newCheque = (Link) malloc(sizeof(struct Pool));
	
	newCheque -> item = item;
	newCheque -> next = next;

	return newCheque;
}

/* Comando 'cheque' */
/* Funcao que vai adicionar um novo cheque a lista de cheques por processar, tendo em */
/* atencao se a lista esta vazia ou nao. Caso esteja, a Head vai ser igual a Tail, caso */
/* contrario muda a Tail para o novo cheque. */
void insereCheque(Item item) {

	if (Head == NULL)
		Tail = (Head = novoCheque(item, Head));
	else {
		Tail -> next = novoCheque(item,Tail -> next);
		Tail = Tail -> next;
	}
}

/* Funcao que tem como funcao procurar um cheque na lista de cheques por processar tendo */
/* como ponto de partida a referencia do mesmo. */
Link procuraCheque(Link Head, long int ref) {

	Link foundCheque;
	
	for (foundCheque = Head; foundCheque != NULL; foundCheque = foundCheque -> next) {
		if (ref == foundCheque -> item -> refc)
			return foundCheque;
	}
	return NULL;
}

/* Comando 'processa' e 'processaR' */
/* Funcao que vai remover um cheque da lista de cheques por processar, recebendo apenas */
/* a referencia do mesmo. Depois actualiza a Head, caso o cheque retirado tenha sido */
/* o primeiro, actualiza a Tail, caso o cheque retirado tenha sido o ultimo, e coloca */
/* o apontador do cheque anterior ao removido a apontar para o cheque a seguir ao removido. */
void removeCheque (long int ref) {
	
	Link foundCheque, previousCheque;

	for (foundCheque = Head, previousCheque = NULL; foundCheque != NULL && foundCheque -> item -> refc != ref;
			previousCheque = foundCheque, foundCheque = foundCheque -> next);
	
	if(foundCheque == NULL) {
		printf("Cheque %ld does not exist\n", ref);
		return;
	}
	
	if(Head == foundCheque)
		Head = foundCheque -> next;
	else 
		previousCheque -> next = foundCheque -> next;
	if(foundCheque -> next == NULL)
		Tail = previousCheque; 
	
	free(foundCheque -> item);
	free(foundCheque);
}


/*************************************/
/*  Funcoes referentes aos Clientes  */
/*************************************/

/* Funcao que vai criar um cliente (adicionar a sua referencia, o valor total recebido */
/* e emitido dos cheques e o numero de cheques emitidos e recebidos que ainda se */
/* encontram na pilha para serem processados. */
void criaCliente(long int ref, int nche, int vche, int nchb, int vchb) {
	
	ItemCliente cliente = (ItemCliente) malloc(sizeof(struct cliente));
	
	cliente -> ref = ref;
	cliente -> vche = vche;
	cliente -> nche = nche;
	cliente -> vchb = vchb;
	cliente -> nchb = nchb;
	
	HeadCliente = insereCliente(HeadCliente, cliente);
}

/* Funcao que vai adicionar o novo cliente a arvore, colocando um apontador para */
/* a estrutura que contem o numero de cheques emitidos e recebidos, a referencia */
/* do cliente, e o valor de cheques emitidos e recebidos que ainda se encontram */
/* na lista por processar. Tem tambem dois apontadores, um para o seu filho direito */
/* e outro para o seu filho esquerdo. */
LinkCliente novoCliente(ItemCliente itemCliente, LinkCliente Left, LinkCliente Right) {

	LinkCliente newCliente = (LinkCliente) malloc(sizeof(struct AVLCliente));
	
	newCliente -> itemCliente = itemCliente;
	newCliente -> Left = Left;
	newCliente -> Right = Right;
	newCliente -> Profundidade = 0;

	return newCliente; 
}

/* Funcao que tem como funcao procurar um cliente na arvore de clientes, tendo como */
/* como ponto de partida a referencia do mesmo. */
LinkCliente procuraCliente(LinkCliente foundCliente, long int ref) {

	if (foundCliente == NULL)
		return NULL;
	if (ref == foundCliente -> itemCliente -> ref)
		return foundCliente;
	if (less(ref, foundCliente -> itemCliente -> ref))
		return procuraCliente (foundCliente -> Left, ref);
	else
		return procuraCliente (foundCliente -> Right, ref);
}

/* Funcao que vai adicionar um cliente a arvore, adicionando ou a esquerda ou a direita, */
/* dependendo do valor da referencia. Se for inferior a raiz, adiciona a esquerda, caso */
/* contrario adiciona a direita. No final equilibra a arvore. */
LinkCliente insereCliente(LinkCliente foundCliente, ItemCliente cliente) {	

	ItemCliente x = cliente;

	if (foundCliente == NULL)
		return novoCliente(cliente, NULL, NULL);
	if (less(x -> ref, foundCliente -> itemCliente -> ref))
		foundCliente -> Left = insereCliente (foundCliente -> Left, cliente);
	else
		foundCliente -> Right = insereCliente (foundCliente -> Right, cliente);
	
	foundCliente = AVLbalance(foundCliente);
	return foundCliente;
}

/* Funcao que vai remover um cliente da arvore, removendo primeiro o cliente em si e depois*/
/* o nodo do mesmo. De seguida, actualiza os ponteiros para os nodos e equlibra a arvore. */
LinkCliente removeCliente(LinkCliente foundCliente, ItemCliente itemCliente) {

	LinkCliente aux;
	ItemCliente x;

	if (foundCliente == NULL)
		return foundCliente;
	else if (less(itemCliente -> ref, foundCliente -> itemCliente -> ref))
		foundCliente -> Left = removeCliente (foundCliente -> Left, itemCliente);
	else if (less(foundCliente -> itemCliente -> ref, itemCliente -> ref))
		foundCliente -> Right = removeCliente (foundCliente -> Right, itemCliente);
	else {
		if (foundCliente -> Left != NULL && foundCliente -> Right != NULL) { 
			aux = AVLMax (foundCliente -> Left);
			{x = foundCliente -> itemCliente; foundCliente -> itemCliente = aux -> itemCliente;
				aux -> itemCliente = x;}
			foundCliente -> Left = removeCliente (foundCliente -> Left, aux -> itemCliente);
		}
		else {
			aux = foundCliente;
			if (foundCliente -> Left == NULL && foundCliente -> Right == NULL)
				foundCliente = NULL;
			else if (foundCliente -> Left == NULL)
				foundCliente = foundCliente -> Right;
			else
				foundCliente = foundCliente -> Left;

			free(aux -> itemCliente);
			free(aux);
		}
	}

	foundCliente = AVLbalance (foundCliente);
	return foundCliente;
}


/* Funcao que devolve o numero de clientes que contem cheques por processar. */
int numeroClientes(LinkCliente foundCliente) {

	if (foundCliente == NULL)
		return 0;
	else {
		return numeroClientes (foundCliente -> Right) + 
			numeroClientes (foundCliente -> Left) + 1;
	}
}

/* Funcao que imprime para o output os clientes por ordem crescente de referencia. */
void printClientes(LinkCliente foundCliente) {

	if (foundCliente == NULL)
		return;

	printClientes (foundCliente -> Left);
	printf("*%ld %d %d %d %d\n", foundCliente -> itemCliente -> ref,
		foundCliente -> itemCliente -> nche, foundCliente -> itemCliente -> vche,
		foundCliente -> itemCliente -> nchb, foundCliente -> itemCliente -> vchb);
	printClientes (foundCliente -> Right); 
}

/* Verifica se pode apagar o cliente da arvore ou nao - caso em que ja nao tem cheques */
/* por processar. */
void verifica(long int ref) {

	LinkCliente foundCliente = procuraCliente(HeadCliente,ref);

	if (foundCliente == NULL)
		return;
	if (foundCliente -> itemCliente -> nchb == 0 && foundCliente -> itemCliente -> nche == 0)
		 HeadCliente = removeCliente (HeadCliente, foundCliente -> itemCliente);
}

/* Actualiza o cliente de acordo com o cheque que foi inserido na Pool, ou com */
/* o cheque que foi removido da Pool. */
void actualiza(long int ref, int EmissBenef, int AddRemov, int valor) {
	
	LinkCliente foundCliente = procuraCliente (HeadCliente, ref);
	
	if (AddRemov == 0) {
		if (EmissBenef == 0) {
			foundCliente -> itemCliente -> nche --;
			foundCliente -> itemCliente -> vche -= valor; 
		}
		else {
			foundCliente -> itemCliente -> nchb --;
			foundCliente -> itemCliente -> vchb -= valor;
		}
	} 
	else {
		if (EmissBenef == 0) {
			foundCliente -> itemCliente -> nche ++;
			foundCliente -> itemCliente -> vche += valor;
		}
		else {
			foundCliente -> itemCliente -> nchb ++;
			foundCliente -> itemCliente -> vchb += valor;
		}
	}
}

/* Funcao que vai buscar o cliente com a referencia mais elevada. */
LinkCliente AVLMax(LinkCliente foundCliente) {

	if (foundCliente == NULL || foundCliente -> Right == NULL)
		return foundCliente;
	else
		return AVLMax (foundCliente -> Right);
}

/* Funcao que vai devolve a profundidade da arvore. */
int profundidadeAVL(LinkCliente foundCliente) {

	if (foundCliente == NULL) {
		return -1;
	}
	return foundCliente -> Profundidade;
}

/* Funcao que vai fazer a rotacao da arvore a esquerda. */
LinkCliente rotacaoEsq(LinkCliente foundCliente) {

	int profundidadeEsq, profundidadeDir;
	LinkCliente x;

	x = foundCliente -> Right;
	foundCliente -> Right = x -> Left;
	x -> Left = foundCliente;

	profundidadeEsq = profundidadeAVL (foundCliente -> Left);
	profundidadeDir = profundidadeAVL (foundCliente -> Right);
	foundCliente -> Profundidade = profundidadeEsq > profundidadeDir ?
		profundidadeEsq + 1: profundidadeDir + 1;
	
	profundidadeEsq = profundidadeAVL (x -> Left);
	profundidadeDir = profundidadeAVL (x -> Right);
	x -> Profundidade = profundidadeEsq > profundidadeDir ?
		profundidadeEsq + 1: profundidadeDir + 1;

	return x;
}

/* Funcao que vai fazer a rotacao da arovre a direita. */
LinkCliente rotacaoDir(LinkCliente foundCliente) {

	int profundidadeEsq, profundidadeDir;
	LinkCliente x;

	x = foundCliente -> Left;
	foundCliente -> Left = x -> Right;
	x -> Right = foundCliente;

	profundidadeEsq = profundidadeAVL (foundCliente -> Left);
	profundidadeDir = profundidadeAVL (foundCliente -> Right);
	foundCliente -> Profundidade = profundidadeEsq > profundidadeDir ?
		profundidadeEsq + 1: profundidadeDir + 1;
	
	profundidadeEsq = profundidadeAVL (x -> Left);
	profundidadeDir = profundidadeAVL (x -> Right);
	x -> Profundidade = profundidadeEsq > profundidadeDir ?
		profundidadeEsq + 1: profundidadeDir + 1;

	return x;
}

/* Funcao que vai fazer a rotacao a esquerda e posterior a direita da arvore. */
LinkCliente rotacaoEsqDir(LinkCliente foundCliente) {

	if (foundCliente == NULL)
		return foundCliente;
	
	foundCliente -> Left = rotacaoEsq (foundCliente -> Left);
	return rotacaoDir(foundCliente);
}

/* Funcao que vai fazer a rotacao a direita e posterior a esquerda da arvore. */
LinkCliente rotacaoDirEsq(LinkCliente foundCliente) {

	if (foundCliente == NULL)
		return foundCliente;
	
	foundCliente -> Right = rotacaoDir (foundCliente -> Right);
	return rotacaoEsq(foundCliente);
}

/* Funcao que calcula o factor balance para equilibrar a arvore na funcao AVLbalance */
int balance(LinkCliente foundCliente) {

	if (foundCliente == NULL)
		return 0;

	return profundidadeAVL(foundCliente -> Left) - profundidadeAVL(foundCliente -> Right);
}

/* Funcao que equilibra a arvore para uma pesquisa mais eficiente. */
LinkCliente AVLbalance(LinkCliente foundCliente) {

	int factorBalance, profundidadeDir, profundidadeEsq;

	if (foundCliente == NULL)
		return foundCliente;

	factorBalance = balance(foundCliente);
	if (factorBalance > 1) {
		if (balance (foundCliente -> Left) > 0)
			foundCliente = rotacaoDir (foundCliente);
		else
			foundCliente = rotacaoEsqDir(foundCliente);
	}
	else if (factorBalance < -1) {
		if (balance(foundCliente -> Right) < 0)
			foundCliente = rotacaoEsq(foundCliente);
		else
			foundCliente = rotacaoDirEsq(foundCliente);
	}
	else {
		profundidadeEsq = profundidadeAVL(foundCliente -> Left);
		profundidadeDir = profundidadeAVL(foundCliente -> Right);
		foundCliente -> Profundidade = profundidadeEsq > profundidadeDir ?
			profundidadeEsq + 1: profundidadeDir + 1;
	}

	return foundCliente;
}

/* Funcao que, ao ser usada na main, vai libertar toda a memoria dedicada aos clientes da arvore. */
void freeArvore(LinkCliente HeadCliente) {

	LinkCliente auxLeft, auxRight;

	if (HeadCliente == NULL)
		return;

	auxLeft = HeadCliente -> Left;
	auxRight = HeadCliente -> Right;
	free(HeadCliente -> itemCliente);
	free(HeadCliente);
	if (auxLeft != NULL)
		freeArvore(auxLeft);
	if (auxRight != NULL)
		freeArvore(auxRight);
}

/* Funcao que, ao ser usada na main, vai libertar toda a memoria dedicada aos cheques da lista. */
void freeLista(Link Head) {
	
	while (Head != NULL) {
		removeCheque(Head -> item -> refc);
		Head = Head->next;
	}
}