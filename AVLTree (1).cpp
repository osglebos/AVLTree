#include <iostream>
#include <Windows.h>
using namespace std;

struct AVLtree
{
	int key;
	AVLtree* leftNode;
	AVLtree* rightNode;
};

void addKnot(AVLtree*, AVLtree*);
void print_Tree(AVLtree*, int);
void findOrderlyTree(AVLtree*);
void checkNode(AVLtree*, int, int*, int, int*);
int findCountNodes(AVLtree*, int);
void fillArray(AVLtree*, int*, int, int);

int main()
{
	setlocale(LC_ALL, "RU");

	AVLtree* treeRoot = nullptr;
	int Nodes[7]={ 15,1,8,13,24,25,30 };
	AVLtree* newKnot = nullptr;

	//ñîçäàíèå êîðíÿ
	cout << "==ÑÎÇÄÀÍÈÅ ÊÎÐÍß ÄÅÐÅÂÀ==\n";
	treeRoot = new AVLtree();
	treeRoot->leftNode = nullptr;
	treeRoot->rightNode = nullptr;
	treeRoot->key = *(Nodes);
	for (int i = 1; i < 7; i++)
	{
		newKnot = new AVLtree();
		newKnot->key = Nodes[i];
		addKnot(treeRoot, newKnot);
	}
	//

	cout << "==ÎÁÙÅÅ ÄÅÐÅÂÎ==" << endl;
	print_Tree(treeRoot, 0);
	cout << endl << endl;

	findOrderlyTree(treeRoot);

	system("pause");
	return 0;
}

void addKnot(AVLtree* ptreeRoot, AVLtree* pnewKnot)
{
	if (ptreeRoot)
	{
		if (pnewKnot->key <= ptreeRoot->key)
		{
			addKnot(ptreeRoot->leftNode, pnewKnot);
			if (ptreeRoot->leftNode == nullptr) ptreeRoot->leftNode = pnewKnot;
		}
		else if (pnewKnot->key > ptreeRoot->key)
		{
			addKnot(ptreeRoot->rightNode, pnewKnot);
			if (ptreeRoot->rightNode == nullptr) ptreeRoot->rightNode = pnewKnot;
		}
	}
}
void print_Tree(AVLtree* ptreeRoot, int level)
{
	if (ptreeRoot)
	{
		print_Tree(ptreeRoot->rightNode, level + 1);
		for (size_t i = 0; i < level; i++) cout << "  ";
		cout << '|' << ptreeRoot->key << '|' << endl;
		print_Tree(ptreeRoot->leftNode, level + 1);
	}
}
void findOrderlyTree(AVLtree* ptreeRoot)
{
	if (ptreeRoot)
	{
		findOrderlyTree(ptreeRoot->leftNode);
		if (ptreeRoot->leftNode != nullptr || ptreeRoot->rightNode != nullptr)
		{
			int sizeArray = findCountNodes(ptreeRoot, 0);
			int* countNode = new int[sizeArray];
			int* countRepeat = new int[sizeArray];
			for (size_t i = 0; i < sizeArray; i++)
			{
				countRepeat[i] = 0;
			}

			fillArray(ptreeRoot, countNode, sizeArray, 0);

			checkNode(ptreeRoot, 0, countNode, sizeArray, countRepeat);
		}
		findOrderlyTree(ptreeRoot->rightNode);
	}
}
void checkNode(AVLtree* ptreeRoot, int recursionDepth, int* pcountNode, int psizeArray, int* pcountRepeat)
{
	if (ptreeRoot)
	{
		checkNode(ptreeRoot->leftNode, recursionDepth + 1, pcountNode, psizeArray, pcountRepeat);

		for (size_t i = 0; i < psizeArray; i++)
		{
			if (ptreeRoot->key == pcountNode[i]) pcountRepeat[i]++;
		}

		checkNode(ptreeRoot->rightNode, recursionDepth + 1, pcountNode, psizeArray, pcountRepeat);

		if (recursionDepth == 0)
		{
			bool flag = false;
			for (size_t i = 0; i < psizeArray; i++)
			{
				if (pcountRepeat[i] > 1)
				{
					flag = true;
					break;
				}
			}
			if (flag == false)
			{
				cout << "Äåðåâî: " << endl;
				print_Tree(ptreeRoot, 0);
				cout << endl << endl;
			}
		}
	}

}
int findCountNodes(AVLtree* ptreeRoot, int recursionDepth)
{
	static int count = 0;
	if (ptreeRoot)
	{
		count++;
		findCountNodes(ptreeRoot->leftNode, recursionDepth + 1);
		findCountNodes(ptreeRoot->rightNode, recursionDepth + 1);

		if (recursionDepth == 0)return count;
	}
}
void fillArray(AVLtree* ptreeRoot, int* pcountNode, int psizeArray, int rec)
{
	static int j = 0;
	bool flag = false;
	if (ptreeRoot)
	{
		fillArray(ptreeRoot->leftNode, pcountNode, psizeArray, rec + 1);

		for (size_t i = 0; i < psizeArray; i++)
		{
			if (ptreeRoot->key == pcountNode[i])
			{
				flag = true;
				break;
			}
		}

		if (flag == false) pcountNode[j++] = ptreeRoot->key;
		else pcountNode[j++] = 0;

		fillArray(ptreeRoot->rightNode, pcountNode, psizeArray, rec + 1);

		if (rec == 0) j = 0;
	}
}
