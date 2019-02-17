#include <iostream>
#include <map>
#include <list>
#include <fstream>
#include <vector>

using namespace std;

vector<bool> code;
map<char, vector<bool> > table;


class Node
{
	public:
		int freq;
		char sym;
		Node *left, *right;

		Node()
		{
			right = left = NULL;
		}

		Node(Node *L, Node *R)
		{
			left = L;
			right = R;

			freq = L->freq + R->freq;
		}
};


struct NodeCompare
{
	bool operator() (const Node *L, const Node *R) const { return L->freq < R->freq; }
};


void BuildTable(Node *root)
{
	if (root->left != NULL)
	{
		code.push_back(0);
		BuildTable(root->left);
	}

	if (root->right != NULL)
	{
		code.push_back(1);
		BuildTable(root->right);
	}

	if (root->left == NULL && root->right == NULL)
	{
		table[root->sym] = code;
	}

	code.pop_back();
}



int main()
{
	map<char, int> m;
	list<Node*> li;

	ifstream input("input.txt", ios::out | ios::binary);

	while (!input.eof())
	{
		char c = input.get();
		m[c]++;
	}

	for (map<char, int>::iterator itr = m.begin(); itr != m.end(); ++itr)
	{
		Node *p = new Node;
		p->freq = itr->second;
		p->sym = itr->first;
		li.push_back(p);
	}

	while (li.size() != 1)
	{
		li.sort(NodeCompare());

		Node *lson = li.front();
		li.pop_front();
		Node *rson = li.front();
		li.pop_front();

		Node *parent = new Node(lson, rson);
		li.push_back(parent);
	}

	Node *root = li.front();

	BuildTable(root);


	input.clear();
	input.seekg(0);

	ofstream output("output.txt", ios::out | ios::binary);

	int count = 0;
	char buf = 0;
	while (!input.eof())
	{
		char c = input.get();
		vector<bool> x = table[c];
		for (int n = 0; n < x.size(); n++)
		{
			buf  = buf | (x[n] << (7 - count));
			count++;
			if (count == 8)
			{
				count = 0;
				output << buf;
				buf = 0;
			}
		}

	}

	input.close();
	output.close();


	ifstream F("output.txt", ios::in | ios::binary);

	Node *p = root;
	count = 0;
	char byte;
	byte  = F.get();

	while (!F.eof())
	{
		bool b = byte & (1 << (7 - count));
		if (b) p = p->right; else p = p->left;
		if (p->left == NULL && p->right == NULL)
		{
			cout << p->sym;
			p = root;
		}
		count++;
		if (count == 8)
		{
			count = 0;
			byte = F.get();
		}
	}

	cout << endl;
	F.close();

	return 0;

}
