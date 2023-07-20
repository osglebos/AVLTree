#include <iostream>
#include <time.h>
#include <math.h>
#include <list>

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

AvlTree GeneratePerfectTree(int num)
{
	list<int> numbers;
	numbers.empty();
	AvlTree T = NULL;
	num = (int)pow(2, num) - 1;
	srand((unsigned int)time(NULL) * 1000);
	for (int i = 0; i < num;)
	{
		numbers.push_front(rand()%100);
		numbers.sort();
		numbers.unique();
		if (i < (int) numbers.size()) i++;
	}
		
	for (auto i : numbers)
		T = Insert(i, T);		
	return T;
}

int main(void)
{
	AvlTree head;
	int power = 0;
	cout << "Insert power of 2 to create perfect AVL-tree : ";
	cin >> power;
	cout << "Generating perfect AVL-tree" << endl;

	head = GeneratePerfectTree(power);
	printTree(head);
	head = MakeEmpty(head);
	system("pause");
	return 0;
}

//������� �������� ������� � ��� �����������
AvlTree MakeEmpty(AvlTree T) {
	if (T != NULL) {
		MakeEmpty(T->Left);
		MakeEmpty(T->Right);
		free(T);
	}
	return NULL;
}

//������� ���������� ��� �������
static int Height(Position P) {
	if (P == NULL) return -1;
	else return P->Height;
}

//������� ���������� ������������ �� ���� �����
static int Max(int Lhs, int Rhs) {
	return Lhs > Rhs ? Lhs : Rhs;
}

/*������� ��������� ������� ����� ��������� K2 � ��� ����� ��������*/
static Position SingleRotateWithLeft(Position K2) {
	Position K1;
	K1 = K2->Left;
	K2->Left = K1->Right;
	K1->Right = K2;
	K2->Height = Max(Height(K2->Left), Height(K2->Right)) + 1;
	K1->Height = Max(Height(K1->Left), K2->Height) + 1;
	return K1;  //����� ������
}

//������� ��������� ������� ����� ��������� K1 � ��� ������ ��������
static Position SingleRotateWithRight(Position K1) {
	Position K2;
	K2 = K1->Right;
	K1->Right = K2->Left;
	K2->Left = K1;
	K1->Height = Max(Height(K1->Left), Height(K1->Right)) + 1;
	K2->Height = Max(Height(K2->Right), K1->Height) + 1;
	return K2;  //����� ������
}

//������� ��������� ������� �����-������ �������
static Position DoubleRotateWithLeft(Position K3) {
	// ������� ����� K1 � K2/
	K3->Left = SingleRotateWithRight(K3->Left);
	// ������� ����� K3 � K2
	return SingleRotateWithLeft(K3);
}

//������� ��������� ������� ������-����� �������
static Position DoubleRotateWithRight(Position K1) {
	// ������� ����� K3 � K2
	K1->Right = SingleRotateWithLeft(K1->Right);
	// ������� ����� K1 � K2
	return SingleRotateWithRight(K1);
}

//������� ������� ������� � ���-������
AvlTree Insert(ElementType X, AvlTree T) {
	if (T == NULL) {
		T = new AvlNode();
		if (T == NULL)
			fprintf(stderr, "������������ ������!!!\n");
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

//������� ���������� ��������, ���������� � �������
ElementType Retrieve(Position P) {
	return P->Element;
}

//������� ������ ���-������ �� ������
void printTree(AvlTree T, int l) {
	int i;
	if (T != NULL) {
		printTree(T->Right, l + 1);
		for (i = 0; i < l; i++) cout << "    ";
		printf("%4ld", Retrieve(T));
		printTree(T->Left, l + 1);
	}
	else cout << endl;
}
