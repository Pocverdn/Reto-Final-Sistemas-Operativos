#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <string>

using namespace std;
//Los nodos del arbol
struct Node
{
    char ch;
    int freq;
    Node *left, *right;

    // Default constructor
    Node() : ch('\0'), freq(0), left(nullptr), right(nullptr) {}

    // Custom constructor
    Node(char c, int f, Node* l = nullptr, Node* r = nullptr)
        : ch(c), freq(f), left(l), right(r) {}
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

char CompressDecode(Node* root, int &index, const string &str)
{
    if (root == nullptr)
        return '\0';

    if (!root->left && !root->right)
        return root->ch;

    index++;

    if (str[index] == '0')
        return CompressDecode(root->left, index, str);
    else
        return CompressDecode(root->right, index, str);
}

void serializeTree(Node* root, ofstream &out) {
    if (!root) {
        out << "# ";
        return;
    }

    unsigned int code = static_cast<unsigned char>(root->ch);

    out << code << ":" << root->freq << " ";
    serializeTree(root->left, out);
    serializeTree(root->right, out);
}

Node* deserializeTree(ifstream &in) {
    string token;
    in >> token;

    if (token == "#")
        return nullptr;

    size_t pos = token.find(':');

    int code = stoi(token.substr(0, pos));
    int freq = stoi(token.substr(pos + 1));

    char ch = static_cast<char>(code);

    Node* node = new Node(ch, freq, nullptr, nullptr);

    node->left = deserializeTree(in);
    node->right = deserializeTree(in);

    return node;
}

void encodeHuffman(string inputFile, string encodedFile, string treeFile)
{
    // leer archivo
    ifstream in(inputFile);
    if (!in.is_open()) {
        cout << "Error: cannot open input file\n";
        return;
    }
    
    string text((istreambuf_iterator<char>(in)),
                 istreambuf_iterator<char>());
    in.close();

    // contar frequencia
    unordered_map<char, int> freq;
    for (char ch: text)
        freq[ch]++;

    // cola
    priority_queue<Node*, vector<Node*>, comp> pq;

    for (auto &p : freq)
        pq.push(makeNode(p.first, p.second, nullptr, nullptr));

    // construir arbol
    while (pq.size() != 1) {
        Node *left = pq.top(); pq.pop();
        Node *right = pq.top(); pq.pop();
        pq.push(makeNode(' ', left->freq + right->freq, left, right));
    }

    Node* root = pq.top();

    // Generar codigos de huffman
    unordered_map<char, string> huffmanCode;
    CompressEncode(root, "", huffmanCode);

    // comprimir
    string encodedString = "";
    for (char ch : text)
        encodedString += huffmanCode[ch];

    // Salvar compression
    ofstream out(encodedFile);
    out << encodedString;
    out.close();

    // Salvar llave o arbol
    ofstream treeOut(treeFile);
    serializeTree(root, treeOut);
    treeOut.close();

    cout << "Encoding completed.\n";
    cout << "Encoded saved to: " << encodedFile << endl;
    cout << "Tree saved to: " << treeFile << endl;
}
void decodeHuffman(string encodedFile, string treeFile, string outputFile)
{
    ifstream in(encodedFile);
    if (!in.is_open()) {
        cout << "Error: cannot open encoded file\n";
        return;
    }
    string encoded((istreambuf_iterator<char>(in)),
                    istreambuf_iterator<char>());
    in.close();

    ifstream tin(treeFile);
    if (!tin.is_open()) {
        cout << "Error: cannot open tree file\n";
        return;
    }

    Node* root = deserializeTree(tin);
    tin.close();

    string decoded = "";
    int index = -1;
    while (index < (int)encoded.size() - 2) {
        decoded += CompressDecode(root, index, encoded);
    }
    ofstream out(outputFile);
    out << decoded;
    out.close();

    cout << "Decoding completed.\n";
    cout << "Decoded saved to: " << outputFile << endl;
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
		Node *left = pq.top(); pq.pop();
		Node *right = pq.top();	pq.pop();

		int sum = left->freq + right->freq;
		pq.push(makeNode(' ', sum, left, right));
	}

	Node* root = pq.top();

	unordered_map<char, string> huffmanCode;
	CompressEncode(root, "", huffmanCode);

	cout << "Huffman Codes are :n" << 'n';
	for (auto pair: huffmanCode) {
		cout << pair.first << " " << pair.second << 'n';
	}

	cout << "nOriginal string was :n" << text << 'n';

	string str = "";
	for (char ch: text) {
		str += huffmanCode[ch];
	}

	cout << "nEncoded string is :n" << str << 'n';

	int index = -1;
	cout << "nDecoded string is: n";
	while (index < (int)str.size() - 2) {
		CompressDecode(root, index, str);
	}
}
// int main(int argc, char *argv[])
// {

// 	buildHuffmanTree(argv[1]);

// 	return 0;


// }