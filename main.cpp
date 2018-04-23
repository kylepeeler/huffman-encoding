#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <string>

using namespace std;

vector<char> letters;
vector<int> frequencies;
vector<struct HuffmanEncoding*> encodings;

struct HuffmanNode {
    HuffmanNode(char data, int freq) : left(nullptr), right(nullptr), data(data), freq(freq) {}
    char data;
    int freq;
    HuffmanNode *left, *right;
};

struct HuffmanComparitor {
    bool operator() (HuffmanNode* l, HuffmanNode* r) {
        return (l->freq > r->freq);
    }
};

struct HuffmanEncoding {
    HuffmanEncoding(char data, string encoding) : data(data), encoding(encoding) {}
    char data;
    string encoding;
};

struct ascii_sort
{
    inline bool operator() (const HuffmanEncoding* he1, const HuffmanEncoding* he2)
    {
        return (he1->data < he2->data);
    }
};

void readInput(string fileName) {
    ifstream input(fileName);
    string line;
    if (input.fail()) {
        cout << "Unable to open file" << endl;
        exit(1);
    }
    while (!input.eof()) {
        getline(input, line);
        string letter = line.substr(0, line.find(' '));
        string frequency = line.substr(line.find(' ') + 1);
        if (letter != "LF") {
            letters.push_back(letter[0]);
        }
        else {
            letters.push_back('\n');
        }
        frequencies.push_back(stoi(frequency));
    }
}

struct HuffmanNode* buildHuffmanTree() {
    struct HuffmanNode *left, *right, *top = nullptr;
    //Create a min heap to store Huffman nodes
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, HuffmanComparitor> minHeap;
    //Push the values from input onto the min heap
    for (int i = 0; i < frequencies.size(); i++) {
        minHeap.push(new HuffmanNode(letters[i], frequencies[i]));
    }
    //Iterate until we have one single node in our min heap (our Huffman Tree)
    while (minHeap.size() != 1) {
        //Get the first two values from the min heap
        left = minHeap.top();
        minHeap.pop();
        right = minHeap.top();
        minHeap.pop();

        //Create a new node, summing the two frequencies and set the data to a value marking it as internal ('*')
        top = new HuffmanNode('*', left->freq + right->freq);
        top->left = left;
        top->right = right;

        //Push the new internal node onto the min heap
        minHeap.push(top);
    }
    //return the HuffmanTree
    return top;
}

void getHuffmanCodes(struct HuffmanNode* root, string str) {
    //base case for recursion
    if (!root) return;
    //only store non-internal nodes (* = internal)
    if (root->data != '*') {
        encodings.push_back(new HuffmanEncoding(root->data, str));
    }
    //recursively traverse the tree:

    //if we go left we add a 0
    getHuffmanCodes(root->left, str + "0");

    //if we go right we add a 1
    getHuffmanCodes(root->right, str + "1");
}

void outputResults(string fileName) {
    //sort the encodingss by ASCII value of data
    sort(encodings.begin(), encodings.end(), ascii_sort());

    ofstream output;
    output.open(fileName);
    for (int i = 0; i < encodings.size(); i++) {
        if (encodings[i]->data != '\n') {
            cout << encodings[i]->data << ":" << encodings[i]->encoding << endl;
            output << encodings[i]->data << ":" << encodings[i]->encoding << endl;
        }
        else {
            cout << "LF:" << encodings[i]->encoding << endl;
            output << "LF:" << encodings[i]->encoding << endl;
        }

    }
    output.close();
    cout << "\nOutput written to " << fileName << endl;

}

int main() {
    readInput("freq.txt");
    getHuffmanCodes(buildHuffmanTree(), "");
    outputResults("codetable.txt");
    cin.get();
    return 0;
}