#include "stdafx.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class Node;
class student;

//GLOBAL VARIABLES//
Node* root;
int nodes;
int splite_count;

class group {
public:
	std::string name;
	std::vector<student*> groupList;

	group(std::string a) {
		name = a;
	}
};

class student {
public:
	std::string name;
	std::string homeCountry;
	std::string homeCity;
	double rating;
	group* specialization;

	student(std::string n, std::string c, std::string c_, double r, group* s) : name(n), homeCountry(c), homeCity(c_), rating(r), specialization(s) {}

	void show() const {
		std::cout << "Student Name: " << name << "\nStudent Country: " << homeCountry << "\nStudent City: " << homeCity << "\nStudent Grade: " << rating << std::endl;
	}
};

class Node {
	bool leaf;
	bool root;
	int key_num;
	std::vector<student*> keys; //21
	std::vector<Node*>children; //22
	Node* father;
public:
	Node() : leaf(0), root(0), key_num(0), keys(21), children(22), father(nullptr) {
		nodes++;
	}
	void setRoot(bool r) { root = r; }
	void setLeaf(bool l) { leaf = l; }
	void setKeynum(int k) { key_num = k; }
	void setKey(int index, student* value) { keys[index] = value; }
	void setChild(int index, Node* c) { children[index] = c; }
	void setFather(Node* f) { father = f; }

	bool getRoot() { return root; }
	bool getLeaf() { return leaf; }
	int getKeynum() { return key_num; }
	student* getKey(int index) { return keys[index]; }
	Node* getChild(int index) { return children[index]; }
	Node* getFather() { return father;}
};

void initBPT() {
	root = new Node();
	root->setRoot(1);
	root->setLeaf(1);
	nodes = 0;
	splite_count = 0;
}

Node* findLeaf(student* key) {
	Node* current = root;
	while (!current->getLeaf())	{
		for (int i = 0; i <= current->getKeynum(); i++) {
			if (i == current->getKeynum() || key->name < current->getKey(i)->name) {
				current = current->getChild(i);
				break;
			}
		}
	}
	return current;
}

Node* findLeaf(std::string key) {
	Node* current = root;
	while (!current->getLeaf()) {
		for (int i = 0; i <= current->getKeynum(); i++) {
			if (i == current->getKeynum() || key < current->getKey(i)->name) {
				current = current->getChild(i);
				break;
			}
		}
	}
	return current;
}

void insertToNode(Node*, student*, Node*);

void split(Node* node) {
	splite_count++;
	Node* temp = new Node();
	student* middle = node->getKey(10);

	temp->setKeynum(10);
	for (int i = 0; i < temp->getKeynum(); i++)
	{
		temp->setKey(i, node->getKey(i + 11));
		temp->setChild(i, node->getChild(i + 11));
	}
	temp->setChild(temp->getKeynum(), node->getChild(21));
	node->setKeynum(10);

	if (node->getLeaf()) {
		node->setKeynum(node->getKeynum()+1);
		temp->setChild(0, node->getChild(0));
		node->setChild(0, temp);
		temp->setLeaf(1);
		middle = node->getKey(11);
	}

	if (node->getRoot()) {
		node->setRoot(0);
		root = new Node();
		root->setRoot(1);
		root->setKey(0, middle);
		root->setChild(0, node);
		root->setChild(1, temp);
		root->setKeynum(1);

		node->setFather(root);
		temp->setFather(root);
	}
	else {
		temp->setFather(node->getFather());
		insertToNode(node->getFather(), middle, temp);
	}
}

void insertToNode(Node* node, student* key, Node* value) {
	int x = 0;
	while (x < node->getKeynum() && node->getKey(x)->name < key->name) x++;
	for (int i = node->getKeynum(); i > x; i--) {
		node->setKey(i, node->getKey(i - 1));
	}
	for (int i = node->getKeynum() + 1; i > x + 1; i--) {
		node->setChild(i, node->getChild(i - 1));
	}
	node->setKey(x, key);
	node->setChild(x + 1, value);
	node->setKeynum(node->getKeynum() + 1);

	if (node->getKeynum() == 21) // need to split
		split(node);
}

void deleteFromNode(Node* node, student* key) {
	int x = 0;
	while (key != node->getKey(x)) {
		x++;
	}
	for (int i = x; i < node->getKeynum() - 1; i++) {
		node->setKey(i, node->getKey(i + 1));
	}
	for (int i = x + 1; i < node->getKeynum(); i++) {
		node->setChild(i, node->getChild(i + 1));
	}
	node->setKeynum(node->getKeynum() - 1);
}

bool insert(student* key, Node* value) {
	Node* leaf = findLeaf(key);
	for (int i = 0; i < leaf->getKeynum(); i++) {
		if (key == leaf->getKey(i)) {
			return false;
		}
	}
	insertToNode(leaf, key, value);
	return true;
}

bool deleteNode(std::string key) {
	Node* leaf = findLeaf(key);
	for (int i = 0; i < leaf->getKeynum(); i++)
		if (key == leaf->getKey(i)->name) {
			deleteFromNode(leaf, leaf->getKey(i));
			return true;
		}
	return false;
}

void search(std::string key) {
	Node* leaf = findLeaf(key);
	for (int i = 0; i < leaf->getKeynum(); i++) {
		if (key == leaf->getKey(i)->name) {
			leaf->getKey(i)->show();
			return;
		}
	}
	std::cout << "NOT FOUND!" << std::endl;
}


int main() {
	std::vector<group*> Groups(10);
	std::vector<student*> Students(100);

	initBPT();

	std::ifstream in("base.txt");
	std::string temp, tempCountry, tempCity, tempSpec, tempR;
	double tempRating;

	for (int i = 0; i < 10; i++) {
		getline(in, temp);
		Groups[i] = new group(temp);
	}

	getline(in, temp);

	for (int i = 0; i < 100; i++) {
		getline(in, temp);
		getline(in, tempR);
		getline(in, tempCity);
		getline(in, tempCountry);
		getline(in, tempSpec);
		tempRating = atof(tempR.c_str());
		Students[i] = new student(temp, tempCountry, tempCity, tempRating, new group(tempSpec));
	
		insert(Students[i], nullptr);

		for (int j = 0; j < 10; j++) {
			if (Groups[j]->name == temp) {
				Groups[j]->groupList.push_back(Students[i]);
			}
		}
		getline(in, temp);
	}

	search("lolka");
	search("Evonne Puterman");
	deleteNode("Evonne Puterman") ? std::cout << "DELETED SUCCSESSFULLY!" : std::cout << "ERROR, CANT FIND STUDENT!"; std::cout << std::endl;
	search("Evonne Puterman");

	system("pause");
    return 0;
}