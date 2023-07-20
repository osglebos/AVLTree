#include <iostream>
#include <time.h>
#include <vector>

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
ElementType Retrieve(Position P);
void printTree(AvlTree T, int l = 0);

AvlTree RandomTree(int num)
{
	AvlTree T = NULL;
	printf("Generating random AVLTree: \n");
	srand((unsigned int)time(NULL) * 1000);
	for (int i = 0; i < num; i++)
		T = Insert(rand() % 100, T);
	printTree(T);
	return T;
}

int FullBinaryTrees(AvlTree T, vector<Position> *full_roots)
{
	if (T == NULL) return -1;
	if (T->Height == 0) return 0;
	if (T->Height > 0)
	{
		int right = FullBinaryTrees(T->Right, full_roots);
		int left = FullBinaryTrees(T->Left, full_roots);
	
		if ((left == -2) || (right == -2)) return -2;

		if((left == -1) && (right == -1)) return 0;
		else if ((left == 0) && (right == -1)) return -2;
		else if ((left == -1) && (right == 0)) return -2;
		else if ((left == 0) && (right == 0))
		{
			(*full_roots).push_back(T);
			return 1;
		}
		else if ((left > 0) || (right > 0))
		{
			if (left == right)
			{
				(*full_roots).push_back(T);
				return T->Height;
			}
			else return -2;
		}	
	}
}

void FindMaxFullBinaryTree(AvlTree T)
{
	vector<Position> roots_list;
	FullBinaryTrees(T, &roots_list);
	int max = roots_list.front()->Height;
	Position res = roots_list.front();
	for (auto i : roots_list)
	{
		if (max < i->Height)
		{
			max = i->Height;
			res = i;
		}
	}

	printf("\n\nLargest full binary tree:");
	printTree(res);
}

#define SIZE 32
AvlTree GenerateTree(void)
{
	AvlTree T = NULL;
	printf("Generating AVLTree: \n");
	int a[SIZE] = { 20, 15, 9, 18, 6, 40, 34, 51, 19, 17, 70, 25, 31, 29, 27, 38, 36, 10 , 55, 75, 72, 56, 53, 45, 39, 37, 35, 33, 30, 28, 26, 24};
	for (int i = 0; i < SIZE; i++)
		T = Insert(a[i], T);
	printTree(T);
	return T;
}

int main(void)
{
	//AvlTree head = RandomTree(100);
	AvlTree head = GenerateTree();
	FindMaxFullBinaryTree(head);

	// удаление АВЛ-дерева
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
