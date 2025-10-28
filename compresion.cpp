#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <queue>
#include <unordered_map>

//Los nodos del arbol
struct Node
{
	char ch;
	int freq;
	Node *left, *right;
};

Node* makeNode(char ch, int freq, Node* left, Node* right)
{
	Node* node = new Node();

	node->ch = ch;
	node->freq = freq;
	node->left = left;
	node->right = right;

	return node;
}

//Functor para hacer la comparasion más facil.
struct comp
{
	bool operator()(Node* l, Node* r)
	{
		return l->freq > r->freq;
	}
};


void CompressEncode(Node* root, string str,
			unordered_map<char, string> &huffmanCode)
{
	if (root == nullptr)
		return;

	// found a leaf node
	if (!root->left && !root->right) {
		huffmanCode[root->ch] = str;
	}

	CompressEncode(root->left, str + "0", huffmanCode);
	CompressEncode(root->right, str + "1", huffmanCode);
}

void CompressDecode(Node* root, int &index, string str)
{
	if (root == nullptr) {
		return;
	}


	if (!root->left && !root->right)
	{
		cout << root->ch << " " << root->freq;

		return;
	}

	index++;

	if (str[index] =='0')
		CompressDecode(root->left, index, str);
	else
		CompressDecode(root->right, index, str);
}

void buildHuffmanTree(string text)
{
	// Guardamos las frequencias en el hash.
	unordered_map<char, int> freq;
	for (char ch: text) {
		freq[ch]++;
	}

	// Usamos una Queue
	priority_queue<Node*, vector<Node*>, comp> pq;

	// Create a leaf node for each character and add it
	// to the priority queue.
	for (auto pair: freq) {
		pq.push(makeNode(pair.first, pair.second, nullptr, nullptr));
	}

	// do till there is more than one node in the queue
	while (pq.size() != 1)
	{
		// Remove the two nodes of highest priority
		// (lowest frequency) from the queue
		Node *left = pq.top(); pq.pop();
		Node *right = pq.top();	pq.pop();

		// Create a new internal node with these two nodes
		// as children and with frequency equal to the sum
		// of the two nodes' frequencies. Add the new node
		// to the priority queue.
		int sum = left->freq + right->freq;
		pq.push(makeNode(' ', sum, left, right));
	}

	// root stores pointer to root of Huffman Tree
	Node* root = pq.top();

	// traverse the Huffman Tree and store Huffman Codes
	// in a map. Also prints them
	unordered_map<char, string> huffmanCode;
	CompressEncode(root, "", huffmanCode);

	cout << "Huffman Codes are :n" << 'n';
	for (auto pair: huffmanCode) {
		cout << pair.first << " " << pair.second << 'n';
	}

	cout << "nOriginal string was :n" << text << 'n';

	// print encoded string
	string str = "";
	for (char ch: text) {
		str += huffmanCode[ch];
	}

	cout << "nEncoded string is :n" << str << 'n';

	// traverse the Huffman Tree again and this time
	// decode the encoded string
	int index = -1;
	cout << "nDecoded string is: n";
	while (index < (int)str.size() - 2) {
		CompressDecode(root, index, str);
	}

    int main()
{
	string text = "Huffman coding is a data compression algorithm.";

	buildHuffmanTree(text);

	return 0;
}
}