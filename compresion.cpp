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

    // If leaf → return character
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
        out << "# ";        // marker for NULL
        return;
    }
    out << root->ch << ":" << root->freq << " ";
    serializeTree(root->left, out);
    serializeTree(root->right, out);
}
Node* deserializeTree(ifstream &in) {
    string token;
    in >> token;

    if (token == "#")
        return nullptr;

    char ch;
    int freq;

    // Example token: A:12
    size_t pos = token.find(':');
    ch = token[0];
    freq = stoi(token.substr(pos + 1));

    Node* node = new Node(ch, freq, nullptr, nullptr);
    node->left = deserializeTree(in);
    node->right = deserializeTree(in);
    return node;
}
void encodeHuffman(string inputFile, string encodedFile, string treeFile)
{
    // 1. Read input file
    ifstream in(inputFile);
    if (!in.is_open()) {
        cout << "Error: cannot open input file\n";
        return;
    }
    
    string text((istreambuf_iterator<char>(in)),
                 istreambuf_iterator<char>());
    in.close();

    // 2. Count frequencies
    unordered_map<char, int> freq;
    for (char ch: text)
        freq[ch]++;

    // 3. Priority queue
    priority_queue<Node*, vector<Node*>, comp> pq;

    for (auto &p : freq)
        pq.push(makeNode(p.first, p.second, nullptr, nullptr));

    // 4. Build tree
    while (pq.size() != 1) {
        Node *left = pq.top(); pq.pop();
        Node *right = pq.top(); pq.pop();
        pq.push(makeNode(' ', left->freq + right->freq, left, right));
    }

    Node* root = pq.top();

    // 5. Generate Huffman codes
    unordered_map<char, string> huffmanCode;
    CompressEncode(root, "", huffmanCode);

    // 6. Encode text
    string encodedString = "";
    for (char ch : text)
        encodedString += huffmanCode[ch];

    // 7. Save encoded text
    ofstream out(encodedFile);
    out << encodedString;
    out.close();

    // 8. Save Huffman tree
    ofstream treeOut(treeFile);
    serializeTree(root, treeOut);
    treeOut.close();

    cout << "Encoding completed.\n";
    cout << "Encoded saved to: " << encodedFile << endl;
    cout << "Tree saved to: " << treeFile << endl;
}
void decodeHuffman(string encodedFile, string treeFile, string outputFile)
{
    // 1. Load encoded file
    ifstream in(encodedFile);
    if (!in.is_open()) {
        cout << "Error: cannot open encoded file\n";
        return;
    }
    string encoded((istreambuf_iterator<char>(in)),
                    istreambuf_iterator<char>());
    in.close();

    // 2. Load tree
    ifstream tin(treeFile);
    if (!tin.is_open()) {
        cout << "Error: cannot open tree file\n";
        return;
    }

    Node* root = deserializeTree(tin);
    tin.close();

    // 3. Decode
    string decoded = "";
    int index = -1;
    while (index < (int)encoded.size() - 2) {
        decoded += CompressDecode(root, index, encoded);
    }

    // 4. Save output
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
}
// int main(int argc, char *argv[])
// {

// 	buildHuffmanTree(argv[1]);

// 	return 0;


// }