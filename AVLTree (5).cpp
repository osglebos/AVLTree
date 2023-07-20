#include <iostream>
#include <time.h>

using namespace std;
typedef int ElementType;
typedef struct AvlNode *Position;
typedef struct AvlNode *AvlTree;
struct AvlNode {
	ElementType Element;
	AvlTree Left;
	AvlTree Right;
	int Height;
};

AvlTree MakeEmpty(AvlTree T);
AvlTree Insert(ElementType X, AvlTree T);
AvlTree Delete(AvlTree T, ElementType X);
ElementType Retrieve(Position P);
void printTree(AvlTree T, int l = 0);

#define SIZE 14
AvlTree GenerateTree(void)
{
	AvlTree T = NULL;
	printf("Создаем АВЛ дерево: \n");
	int a[SIZE] = { 25, 30, 10, 1, 15, 17, 22, 3, 27, 19, 47, 50, 23, 21 };
	for (int i = 0; i < SIZE; i++)
		T = Insert(a[i], T);
	printTree(T);
	return T;
}

int main(void)
{
	setlocale(LC_ALL, "Russian");
	AvlTree head = GenerateTree();
	printf("\n-----------------------------\nУдаляю число число 27\n");
	head = Delete(head, 27);
	printTree(head);
	printf("\n-----------------------------\n");
	head = MakeEmpty(head);
	system("pause");
	return 0;
}

//функция удаления вершины и его поддеревьев
AvlTree MakeEmpty(AvlTree T) {
	if (T != NULL) {
		MakeEmpty(T->Left);
		MakeEmpty(T->Right);
		free(T);
	}
	return NULL;
}

//функция возвращает вес вершины
static int Height(Position P) {
	if (P == NULL) return -1;
	else return P->Height;
}

//функция возвращает максимальное из двух чисел
static int Max(int Lhs, int Rhs) {
	return Lhs > Rhs ? Lhs : Rhs;
}

/*функция выполняет поворот между вершинами K2 и его левым потомком*/
static Position SingleRotateWithLeft(Position K2) {
	Position K1;
	K1 = K2->Left;
	K2->Left = K1->Right;
	K1->Right = K2;
	K2->Height = Max(Height(K2->Left), Height(K2->Right)) + 1;
	K1->Height = Max(Height(K1->Left), K2->Height) + 1;
	return K1;  //Новый корень
}

//функция выполняет поворот между вершинами K1 и его правым потомком
static Position SingleRotateWithRight(Position K1) {
	Position K2;
	K2 = K1->Right;
	K1->Right = K2->Left;
	K2->Left = K1;
	K1->Height = Max(Height(K1->Left), Height(K1->Right)) + 1;
	K2->Height = Max(Height(K2->Right), K1->Height) + 1;
	return K2;  //новый корень
}

//функция выполняет двойной левый-правый поворот
static Position DoubleRotateWithLeft(Position K3) {
	// поворот между K1 и K2/
	K3->Left = SingleRotateWithRight(K3->Left);
	// поворот между K3 и K2
	return SingleRotateWithLeft(K3);
}

//функция выполняет двойной правый-левый поворот
static Position DoubleRotateWithRight(Position K1) {
	// поворот между K3 и K2
	K1->Right = SingleRotateWithLeft(K1->Right);
	// поворот между K1 и K2
	return SingleRotateWithRight(K1);
}

//функция вставки вершины в АВЛ-дерево
AvlTree Insert(ElementType X, AvlTree T) {
	if (T == NULL) {
		T = new AvlNode();
		if (T == NULL)
			fprintf(stderr, "Недостаточно памяти!!!\n");
		else {
			T->Element = X; T->Height = 0;
			T->Left = T->Right = NULL;
		}
	}
	else if (X < T->Element) {
		T->Left = Insert(X, T->Left);
		if (Height(T->Left) - Height(T->Right) == 2)
			if (X < T->Left->Element)
				T = SingleRotateWithLeft(T);
			else
				T = DoubleRotateWithLeft(T);
	}
	else if (X > T->Element) {
		T->Right = Insert(X, T->Right);
		if (Height(T->Right) - Height(T->Left) == 2)
			if (X > T->Right->Element)
				T = SingleRotateWithRight(T);
			else
				T = DoubleRotateWithRight(T);
	}
	T->Height = Max(Height(T->Left), Height(T->Right)) + 1;
	return T;
}

//функция возвращает значение, хранящееся в вершине
ElementType Retrieve(Position P) {
	return P->Element;
}

//функция вывода АВЛ-дерева на печать
void printTree(AvlTree T, int l) {
	int i;
	if (T != NULL) {
		printTree(T->Right, l + 1);
		for (i = 0; i < l; i++) cout << "    ";
		printf("%4ld(%i)", Retrieve(T), l);
		printTree(T->Left, l + 1);
	}
	else cout << endl;
}

/*----------------------------------------------------------------*/

int Height1(AvlTree p)
{
	if (p == NULL) return -1;
	else return p->Height;
}

int bfactor(AvlTree p)
{
	return Height1(p->Right) - Height1(p->Left);
}

void fixHeight(AvlTree p)
{
	int hl = Height1(p->Left);
	int hr = Height1(p->Right);
	p->Height = Max(hl, hr) + 1;
}

AvlTree rotateRight(AvlTree p) // правый поворот вокруг p
{
	AvlTree q = p->Left;
	p->Left = q->Right;
	q->Right = p;
	fixHeight(p);
	fixHeight(q);
	return q;
}

AvlTree rotateLeft(AvlTree q) // левый поворот вокруг q
{
	AvlTree p = q->Right;
	q->Right = p->Left;
	p->Left = q;
	fixHeight(q);
	fixHeight(p);
	return p;
}

AvlTree balance(AvlTree p) // балансировка узла p
{
	fixHeight(p);

	if (bfactor(p) == 2)
	{
		if (bfactor(p->Right) < 0)
			p->Right = rotateRight(p->Right);
		return rotateLeft(p);
	}
	if (bfactor(p) == -2)
	{
		if (bfactor(p->Left) > 0)
			p->Left = rotateLeft(p->Left);
		return rotateRight(p);
	}
	return p; // балансировка не нужна
}

AvlTree findmin(AvlTree p) // поиск узла с минимальным ключом в дереве p 
{
	return p->Left ? findmin(p->Left) : p;
}

AvlTree removemin(AvlTree p) // удаление узла с минимальным ключом из дерева p
{
	if (p->Left == 0)
		return p->Right;
	p->Left = removemin(p->Left);
	return balance(p);
}

AvlTree Delete(AvlTree p, ElementType k)
{
	if (!p) return NULL;
	if (k < p->Element)
		p->Left = Delete(p->Left, k);
	else if (k > p->Element)
		p->Right = Delete(p->Right, k);
	else
	{
		AvlTree q = p->Left;
		AvlTree r = p->Right;
		delete p;
		if (!r) return q;
		AvlTree min = findmin(r);
		min->Right = removemin(r);
		min->Left = q;
		return balance(min);
	}
	return balance(p);
}
