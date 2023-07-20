#include <iostream>
#include <time.h>
#include <conio.h>
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
Position Find(ElementType X, AvlTree T);
Position FindMin(AvlTree T);
Position FindMax(AvlTree T);
AvlTree Insert(ElementType X, AvlTree T);
ElementType Retrieve(Position P);
void printTree(AvlTree T, int l = 0);

//функция удаления вершины и его поддеревьев
AvlTree MakeEmpty(AvlTree T) {
	if (T != NULL) {
		MakeEmpty(T->Left);
		MakeEmpty(T->Right);
		free(T);
	}
	return NULL;
}

// поиск вершины со значением X
AvlTree FindElement(ElementType X, AvlTree T) {
	if (T == NULL)
	{
		printf("There is no such element.\n");
		return NULL;
	}

	if (X < T->Element)	return FindElement(X, T->Left);
	else if (X > T->Element) return FindElement(X, T->Right);
	else if (X == T->Element) return T;
}


// поиск вершины со значением X
Position Find(ElementType X, AvlTree T) {
	if (T == NULL)
		return NULL;
	if (X < T->Element)
		return Find(X, T->Left);
	else
		if (X > T->Element)
			return Find(X, T->Right);
		else
			return T;
}

//функция поиска вершины с минимальным значением
Position FindMin(AvlTree T) {
	if (T == NULL)
		return NULL;
	else
		if (T->Left == NULL)
			return T;
		else
			return FindMin(T->Left);
}

//функция поиска вершины с максимальным значением
Position FindMax(AvlTree T) {
	if (T == NULL)
		return NULL;
	else
		if (T->Right == NULL)
			return T;
		else
			return FindMax(T->Right);
}

//функция возвращает вес вершины
static int Height(Position P) {
	if (P == NULL)
		return -1;
	else
		return P->Height;
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
			T->Element = X;
			T->Height = 0;
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

//функция удаления вершины в АВЛ-дереве
AvlTree Delete(ElementType X, AvlTree T) {
	if (T == NULL) {
		cout << "Такого элемента в дереве нет." << endl;
	}
	else if (X < T->Element) {
		T->Left = Delete(X, T->Left);
		if (Height(T->Right) - Height(T->Left) == 2)
		{
			if (X > T->Right->Element)
				T = SingleRotateWithRight(T);
			else
				T = DoubleRotateWithRight(T);
		}


	}
	else if (X > T->Element) {
		T->Right = Delete(X, T->Right);
		if (Height(T->Left) - Height(T->Right) == 2)
		{
			if (X < T->Left->Element)
				T = SingleRotateWithLeft(T);
			else
				T = DoubleRotateWithLeft(T);
		}

	}
	else if (X == T->Element) {
		if (T->Right == NULL)
		{
			Position tmp = T;
			T = T->Left;
			free(tmp);
		}
		else if (T->Right != NULL)
		{
			Position tmp = T;

			T = T->Right;
			while (T->Left != NULL)
				T = T->Left;

			T->Left = tmp->Left;
			T = tmp->Right;
			free(tmp);
		}
	}
	if (T != NULL) T->Height = Max(Height(T->Left), Height(T->Right)) + 1;
	return T;
}

AvlTree Delete_new(AvlTree *T) {
	if (*T == NULL) return NULL;
	AvlTree A = *T;
	*T = (*T)->Right;
	while (*T != NULL)
		T = &(*T)->Left;
	*T = A->Left;
	free(A);
	/*
	else if ((*T)->Right == NULL)
	{
		AvlTree tmp = (*T);
		T = T->Left;
		free(tmp);
	}
	else if (T->Right != NULL)
	{
		Position tmp = T;

		T = T->Right;
		while (T->Left != NULL)
			T = T->Left;

		T->Left = tmp->Left;
		T = tmp->Right;
		free(tmp);
	}*/
}


//функция возвращает значение, хранящееся в вершине
ElementType Retrieve(Position P) {
	if (P == NULL) return NULL;
	return P->Element;
}

//функция вывода АВЛ-дерева на печать
void printTree(AvlTree T, int l) {
	int i;
	if (T != NULL) {
		printTree(T->Right, l + 1);
		for (i = 0; i < l; i++) cout << "    ";
		printf("%4ld(%i)", Retrieve(T), T->Height);
		printTree(T->Left, l + 1);
	}
	else cout << endl;
}

int main(void)
{
	AvlTree T = NULL;
	int i, maxnum;
	bool exit = false;
	char key;

	printf("\nЙНННЛНННННННННННННННННННННННННН»\n");
	printf("єescєExit                      є\n");
	printf("є1  єGenerate   tree           є\n");
	printf("є2  єInsert new element        є\n");
	printf("є3  єDelete element            є\n");
	printf("є4  єPrint tree                є\n");
	printf("є5  єFind element              є\n");
	printf("ИНННКННННННННННННННННННННННННННј\n");

	while (!exit)
	{


		key = _getch();
		switch (key)
		{
		case 27:
			printf("Exit.\n");
			exit = true;
			break;

		case '1':
		{
			T = MakeEmpty(NULL);
			cout << "Input number of elements to create a tree: ";
			cin >> maxnum;
			cout << endl;
			srand((unsigned int)time(NULL) * 1000);

			for (i = 0; i < maxnum; i++)
				T = Insert(rand() % 100, T);
			cout << "Print AVL-tree" << endl;
			printTree(T);
			cout <<  endl;
			cout << "Min = " << Retrieve(FindMin(T)) << ", Max = " << Retrieve(FindMax(T)) << endl;
		}
		break;

		case '2':
		{
			int tmp = 0;
			cout << "Input new number to insert: ";
			cin >> tmp;
			cout << endl;
			T = Insert(tmp, T);
			cout << "Print AVL-tree" << endl;
			printTree(T);
			cout << endl;
			cout << "Min = " << Retrieve(FindMin(T)) << ", Max = " << Retrieve(FindMax(T)) << endl;
		}
		break;

		case '3':
		{
			AvlTree a = NULL;
			int tmp = 0;
			cout << "Input number to remove it: ";
			cin >> tmp;
			cout << endl;
			
			a = FindElement(tmp, T);
			Delete_new(&a);
			cout << "Print AVL-tree" << endl;
			printTree(T);
			cout << endl;
			cout << "Min = " << Retrieve(FindMin(T)) << ", Max = " << Retrieve(FindMax(T)) << endl;
		}
		break;

		case '4':
		{
			cout << "Print AVL-tree" << endl;
			printTree(T);
			cout << endl;
			cout << "Min = " << Retrieve(FindMin(T)) << ", Max = " << Retrieve(FindMax(T)) << endl;
		}
		break;

		case '5':
		{
			int tmp = 0;
			AvlTree a = NULL;
			cout << "Input number to find it: ";
			cin >> tmp;
			cout << endl;
			a = FindElement(tmp, T);
			if (a != NULL) printf("Element is %i with hight %i.\n", a->Element, a->Height);
		}
		break;

		default:
			printf("Wrong symbol\n");
			break;
		}
	}
	T = MakeEmpty(T);
}
