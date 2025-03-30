#include "main.h"

class Restaurant {

public:
	Restaurant() {};
	virtual ~Restaurant() {};
	virtual void LAPSE(string name)=0;
	virtual void insert(int ID, int Result) = 0;

public:
	class customer {
	public:
		int ID;
		int result;
		customer*left;
		customer* right;
	public:
		customer() {
			left = NULL;
			right = NULL;
			result = 0;
			ID = 0;
		};
		customer(int ID, int re, customer* l, customer* r) : ID(ID), result(re), left(l), right(r) {}
		~customer() {}
	};
	class area {
	public:
		int ID;
		int NUM;
		area* left;
		area* right;
	public:
		area() {
			left = NULL;
			right = NULL;
			NUM = 0;
			ID = 0;
		};
		area(int ID, int n, area* l, area* r) : ID(ID), NUM(n), left(l), right(r) {}
		~area() {}
	};
};

class RestaurantG : public Restaurant
{
	int size = 0;
	vector<customer*> listRoot;// luu root cua tung id
	vector<queue<customer*>> idQueue;// luu queue cua tung id
public:
	void LAPSE(string name) {}
	RestaurantG(int MAXSIZE) :size(MAXSIZE), listRoot(size, NULL), idQueue(size) {};
	~RestaurantG(){
		for (auto& x : idQueue)
		{
			while (!x.empty()) {
				customer* tmp = x.front();
				x.pop();
				delete tmp;
				tmp = NULL;
			}
		}
		listRoot.clear();
		idQueue.clear();
	};
	void insertBST(customer*& root, customer*& newCustomer) {
		if (root == NULL) {
			root = newCustomer;
		}
		else if (newCustomer->result < root->result) {
			insertBST(root->left, newCustomer);
		}
		else if (newCustomer->result >= root->result) {
			insertBST(root->right, newCustomer);
		}
	}
	void insert(int ID, int Result) {
		int index = ID-1;// ID chay tu 1->maxsize con index chay tu 0->(maxsize-1)
		customer* newCustomer = new customer(ID, Result, NULL, NULL);
		if (listRoot[index] == NULL) {
			listRoot[index] = newCustomer;
		}
		else {
			insertBST(listRoot[index], newCustomer);
		}
		idQueue[index].push(newCustomer);
	};
	int factorial(int x) {
		return (x <= 1) ? 1 : x * factorial(x - 1);
	}

	int f(int a, int b) {
		int x = (factorial(a) * factorial(b));
		if(x!=0) return factorial(a + b) / x;// to hop chap a cua tong so la n=a+b
		return factorial(a + b);
	}

	int permutation(vector<int>& P) {// tinh hoan vi thu tu topological; 
		if (P.size() <= 2) return 1;// voi nut la thi so luong thu tu la 1 vi la diem cuoi -> chi co mot thu tu
		vector<int> L, R;// P.size()<=2 khong biet dung khong vi co noi de <=1
		for (unsigned int i = 1; i < P.size(); i++) {
			if (P[i] < P[0])
				L.push_back(P[i]);
			else
				R.push_back(P[i]);
		}
		int l = permutation(L);
		int r = permutation(R);
		//chia de tri
		return (l * r * f(L.size(), R.size()))%size;// tinh so luong thu tu có the xep cho node cha tu viec chon vi tri node con trai va phai
	}

	void bstToPostOrderArray(customer* root, vector<int>& result) {
		if (root) {
			bstToPostOrderArray(root->left, result);
			bstToPostOrderArray(root->right, result);
			result.push_back(root->result);
		}
	}

	customer* findMinOrderNode(customer* Customer) {
		customer* tmp = Customer;
		while (Customer->left != NULL) {
			tmp = Customer;
			Customer = Customer->left;
		}
		if (tmp != Customer) tmp->left = Customer->right;
		return Customer;
	}

	customer* deleteCustomer(customer*& root, customer*& deletedCustomer) {
		if (root == NULL) {
			return root;
		}
		if (deletedCustomer->result < root->result) {
			root->left = deleteCustomer(root->left, deletedCustomer);
		}
		else if (deletedCustomer->result > root->result) {
			root->right = deleteCustomer(root->right, deletedCustomer);
		}
		else {
			if (root->left == NULL && root->right == NULL) {
				root = NULL;
				delete root;
			}
			else if (root->left == NULL) {
				customer* temp = root->right;
				root = NULL;
				delete root;
				return temp;
			}
			else if (root->right == NULL) {
				customer* temp = root->left;
				root = NULL;
				delete root;
				return temp;
			}
			else {
				customer* temp = findMinOrderNode(root->right);
				temp->left = root->left;
				if (root->right != temp)temp->right = root->right;
				root = NULL;
				delete root;
				return temp;
			}
		}
		return root;
	}
	void KOKUSEN() {
		for (int i = 0; i < size; i++)
		{
			if (idQueue[i].empty()) continue;
			vector<int> resultArray;
			bstToPostOrderArray(listRoot[i], resultArray);
			reverse(resultArray.begin(), resultArray.end());
			int Y = permutation(resultArray);
			//cout << "num: "<<Y<<endl;
			for (int j = 0; j < Y && !idQueue[i].empty(); j++)
			{
				listRoot[i] = deleteCustomer(listRoot[i], idQueue[i].front());
				idQueue[i].pop();
			}

		}
	}
	void bstToInOrderArray(customer* root) {
		if (root) {
			bstToInOrderArray(root->left);
			cout << root->result << "\n";
			bstToInOrderArray(root->right);
		}
	}
	void LIMITLESS(int NUM) {
		int tmp = NUM - 1;
		bstToInOrderArray(listRoot[tmp]);
	}
	
};



class RestaurantS : public Restaurant
{
	unsigned int size = 0;
	vector<area*> minheap;
	vector<deque<customer*>> idDQueue;
	list<area*> order;
public:
	void LAPSE(string name){}
	RestaurantS(int MAXSIZE) :size(MAXSIZE), idDQueue(size) {};
	~RestaurantS(){
		for (unsigned int i = 0; i < minheap.size(); i++)
		{
			delete minheap[i];
			minheap[i] = NULL;
		}
		for (auto& x : idDQueue) {
			while (!x.empty()) {
				customer* tmp = x.front();
				x.pop_front();
				delete tmp;
				tmp = NULL;
			}
		}
		minheap.clear();
		order.clear();
		idDQueue.clear();
	};
	void reheap_Down(int index) {
		unsigned int leftChild = 2 * index + 1;
		unsigned int rightChild = 2 * index + 2;
		int smallest = index;
		if (leftChild < minheap.size() && rightChild < minheap.size() && minheap[leftChild]->NUM <= minheap[smallest]->NUM && minheap[rightChild]->NUM <= minheap[smallest]->NUM) {
			if (minheap[leftChild]->NUM == minheap[rightChild]->NUM) {
				// xet viec cap nhat bang list
				for (auto& x : order) {
					if (x == minheap[leftChild]) {
						smallest = leftChild;
						break;
					}
					else if (x == minheap[rightChild]) {
						smallest = rightChild;
						break;
					}
					else if (minheap[smallest]->NUM == minheap[leftChild]->NUM && x == minheap[smallest])
					{
						smallest = smallest;
						break;
					}
				}
			}
			else {
				if (minheap[leftChild]->NUM > minheap[rightChild]->NUM) {
					if (minheap[smallest]->NUM == minheap[rightChild]->NUM) {
						for (auto& x : order) {
							if (x == minheap[rightChild]) {
								smallest = rightChild;
								break;
							}
							else if (x == minheap[smallest])
							{
								smallest = smallest;
								break;
							}
						}
					}
					else if(minheap[smallest]->NUM > minheap[rightChild]->NUM) {
						smallest = rightChild;
					}
				}
				else {
					if (minheap[smallest]->NUM == minheap[leftChild]->NUM) {
						for (auto& x : order) {
							if (x == minheap[leftChild]) {
								smallest = leftChild;
								break;
							}
							else if (x == minheap[smallest])
							{
								smallest = smallest;
								break;
							}
						}
					}
					else if (minheap[smallest]->NUM > minheap[leftChild]->NUM) {
						smallest = leftChild;
					}
				}
			}
		}
		else if (leftChild < minheap.size() && minheap[leftChild]->NUM <= minheap[smallest]->NUM) {
			if (minheap[smallest]->NUM == minheap[leftChild]->NUM) {
				for (auto& x : order) {
					if (x == minheap[leftChild]) {
						smallest = leftChild;
						break;
					}
					else if (x == minheap[smallest])
					{
						smallest = smallest;
						break;
					}
				}
			}
			else if (minheap[smallest]->NUM > minheap[leftChild]->NUM) {
				smallest = leftChild;
			}
		}

		else if (rightChild < minheap.size() && minheap[rightChild]->NUM <= minheap[smallest]->NUM) {
			if (minheap[smallest]->NUM == minheap[rightChild]->NUM) {
				for (auto& x : order) {
					if (x == minheap[rightChild]) {
						smallest = rightChild;
						break;
					}
					else if (x == minheap[smallest])
					{
						smallest = smallest;
						break;
					}
				}
			}
			else if (minheap[smallest]->NUM > minheap[rightChild]->NUM) {
				smallest = rightChild;
			}
		}
		if (smallest != index) {
			swap(minheap[index], minheap[smallest]);
			reheap_Down(smallest);
		}
	}
	void reheap_Up(int index) {
		while (index > 0) {
			int parent = (index - 1) / 2;
			if (minheap[index]->NUM < minheap[parent]->NUM) {
				swap(minheap[index], minheap[parent]);
				index = parent;
			}
			
			else {
				break;
			}
		}
	}
	void insert(int ID, int Result) {
		int index = ID - 1;
		bool check = 0;
		customer* newCustomer = new customer(ID, Result, NULL, NULL);
		for (auto& node : minheap) {
			if (node->ID == ID) {
				node->NUM++;
				check = 1;
				order.remove(node);
				order.push_back(node);
				reheap_Down(&node - &minheap[0]);
				break;//tranh sau khi reheapdown gap lai ID cu
			}
		}

		if (check == 0 && minheap.size() < size) {
			area* newArea = new area(ID, 1, NULL, NULL);
			minheap.push_back(newArea);
			order.push_back(newArea);
			reheap_Up(minheap.size()-1);
		}
		idDQueue[index].push_back(newCustomer);
	};
	void removeMinheap(area* Area) {
		unsigned int index = 0;
		for (unsigned int i = 0; i < minheap.size(); i++)
		{
			if (minheap[i] == Area) {
				index = i;
				break;
			}
		}
		swap(minheap[index], minheap[minheap.size() - 1]);
		minheap.pop_back();
		order.remove(Area);
		delete Area;
		if (index != minheap.size()) {//index khong o cuoi
			reheap_Down(index);
			reheap_Up(index);
		}
	}

	void KEITEIKEN(int NUM) {
		int num = NUM;
		while(NUM > 0 && minheap.size()>0) {
			int index = 0;
			area* tmp = minheap[0];
			for (auto& x : order) {
				if (x->NUM == tmp->NUM) {
					tmp = x;
					index = x->ID-1;
					break;
				}
			}
			if (tmp->NUM > num) {
				tmp->NUM -= NUM;
				order.remove(tmp);
				order.push_back(tmp);
			}
			else
			{
				removeMinheap(tmp);
			}
			for (int j = 0; j < num && !idDQueue[index].empty(); j++)
			{
				cout << idDQueue[index].front()->result << "-" << idDQueue[index].front()->ID << '\n';
				delete idDQueue[index].front();
				idDQueue[index].pop_front();
			}
			NUM--;
		}
	}
	void traverse(vector<area*> minheap, int NUM, unsigned int index) {
		if (index < minheap.size()) {
			int count = 0;
			int id = minheap[index]->ID - 1;
			for (auto x = idDQueue[id].rbegin(); x != idDQueue[id].rend() && count < NUM; ++x)
			{
				cout << (*x)->ID<<"-"<<(*x)->result << '\n';
				count++;
			}

			traverse(minheap, NUM, 2 * index + 1);

			traverse(minheap, NUM, 2 * index + 2);
		}
	}
	void CLEAVE(int NUM) {
		if(minheap.size()>0) traverse(minheap, NUM, 0);
	}
};


class HuffNode {

public:
	int height = 1;
	virtual ~HuffNode() {} // Base destructor
	virtual int weight() = 0; // Return frequency
	virtual bool isLeaf() = 0; // Determine type
};

class LeafNode : public HuffNode {
private:
	char it; // Value
	int wgt; // Weight
public:
	LeafNode(const char& val, int freq) // Constructor
	{
		it = val; wgt = freq;
	}
	int weight() { return wgt; }
	char val() { return it; }
	bool isLeaf() { return true; }
};

class IntlNode : public HuffNode {
private:
	HuffNode* lc; // Left child
	HuffNode* rc; // Right child
	int wgt; // Subtree weight
public:
	IntlNode(HuffNode* l, HuffNode* r)
	{
		wgt = l->weight() + r->weight(); lc = l; rc = r;
	}
	int weight() { return wgt; }
	bool isLeaf() { return false; }
	HuffNode* left() const { return lc; }
	void setLeft(HuffNode* b)
	{
		lc = (HuffNode*)b;
	}
	HuffNode* right() const { return rc; }
	void setRight(HuffNode* b)
	{
		rc = (HuffNode*)b;
	}
};


class HuffTree {
private:
	HuffNode* Root; // Tree root
	int order = 0;
public:
	HuffTree(const char& val, int freq) // Leaf constructor
	{
		Root = new LeafNode(val, freq);
	}
	// Internal node constructor
	HuffTree(HuffTree* l, HuffTree* r)
	{
		Root = new IntlNode(l->root(), r->root());
	}
	~HuffTree() { delete Root; } // Destructor
	HuffNode* root() { return Root; } // Get root
	int weight() { return Root->weight(); } // Root weight
	void setRoot(HuffNode* root) {
		Root = root;
	}
	void settime(int o) {
		order = o;
	}
	int gettime() {
		return order;
	}
};

void inOrder(HuffNode* root)
{
	if (root->isLeaf()) {
		LeafNode* Root = dynamic_cast<LeafNode*>(root);
		cout << Root->val() << '\n';
		return;
	}
	IntlNode* Root = dynamic_cast<IntlNode*>(root);
	inOrder(Root->left());
	cout << Root->weight() <<'\n';
	inOrder(Root->right());
}

int height(HuffNode* node) {
	if (!node) return 0;
	return node->height;
}

HuffNode* leftRotate(HuffNode* x)
{
	if (x->isLeaf()) return x;
	IntlNode* root = dynamic_cast<IntlNode*>(x);
	HuffNode* y = root->right();
	IntlNode* subroot = dynamic_cast<IntlNode*>(y);
	HuffNode* T2 = subroot->left();
	subroot->setLeft(x);
	root->setRight(T2);
	x->height = max(height(root->left()),height(root->right())) + 1;
	y->height = max(height(subroot->left()),height(subroot->right())) + 1;
	return y;
	
}
HuffNode* rightRotate(HuffNode* x)
{
	if (x->isLeaf()) return x;
	IntlNode* root = dynamic_cast<IntlNode*>(x);
	HuffNode* y = root->left();
	IntlNode* subroot = dynamic_cast<IntlNode*>(y);
	HuffNode* T2 = subroot->right();
	subroot->setRight(x);
	root->setLeft(T2);
	x->height = max(height(root->left()),height(root->right())) + 1;
	y->height = max(height(subroot->left()),height(subroot->right())) + 1;
	return y;

}

int getBalance(HuffNode* N)
{
	if (N == NULL|| N->isLeaf()) return 0;
	else {
		IntlNode* root = dynamic_cast<IntlNode*>(N);
		return height(root->left()) - height(root->right());
	}
}


HuffNode* balanceHelp(HuffNode*& node,int& time,bool& check) {
	if (node->isLeaf()||time==0||check==1) {
		return node;
	}
	IntlNode* intlNode = dynamic_cast<IntlNode*>(node);
	int balance = getBalance(node);
	if (balance > 1 || balance < -1) {
		time--;
		check = 1;
		if (balance > 1 && getBalance(intlNode->left()) >= 0) {
			return rightRotate(intlNode);
		}

		// Right Right Case  
		if (balance < -1 && getBalance(intlNode->right()) <= 0) {
			return leftRotate(intlNode);
		}

		// Left Right Case  
		if (balance > 1 && getBalance(intlNode->left()) < 0)
		{
			intlNode->setLeft(leftRotate(intlNode->left()));
			return rightRotate(intlNode);
		}

		// Right Left Case  
		if (balance < -1 && getBalance(intlNode->right()) > 0)
		{
			intlNode->setRight(rightRotate(intlNode->right()));
			return leftRotate(intlNode);
		}
	}
	HuffNode* left = intlNode->left();
	HuffNode* right = intlNode->right();
	intlNode->setLeft(balanceHelp(left, time,check));
	intlNode->setRight(balanceHelp(right, time,check));
	intlNode->height = max(intlNode->left()->height, intlNode->right()->height) + 1;
	return node;
}
void balance(HuffTree*& tree) {
	int time = 3;
	bool check = 0;
	HuffNode* root = tree->root();
	if(root->isLeaf()) return;
	while (time>0)
	{
		root = tree->root();
		tree->setRoot(balanceHelp(root, time, check));
		if (check == 0) break;
		else {check = 0; }
	}
}
struct minTreeComp {
	bool operator()( HuffTree* l,  HuffTree* r)  {
		if (l->root()->weight() == r->root()->weight()) {
			return l->gettime() > r->gettime();
		}
		return l->weight() > r->weight();
	}
};


HuffTree* buildHuff(vector<HuffTree*> tmp,int order) {
	priority_queue<HuffTree*, vector<HuffTree*>, minTreeComp> forest(tmp.begin(),tmp.end());
	HuffTree* temp1, * temp2, * temp3 = NULL;
	while (forest.size() > 1) {
		temp1 = forest.top(); // Pull first two trees
		forest.pop();
		temp2 = forest.top(); // off the list
		forest.pop();
		temp3 = new HuffTree(temp1, temp2);
		temp3->root()->height = max(temp1->root()->height, temp2->root()->height) + 1;
		balance(temp3);
		if (temp3->root()->isLeaf()) return NULL;
		temp3->settime(order);
		order++;
		forest.push(temp3); // Put the new tree back on list
		temp1 = NULL;
		temp2 = NULL;
		delete temp1;
		delete temp2;
	}
	return temp3;
}

void deleteHuffmanTree(HuffNode* root) {
	if (!root->isLeaf()) {
		IntlNode* node = dynamic_cast<IntlNode*>(root);
		deleteHuffmanTree(node->left());
		deleteHuffmanTree(node->right());
	}
	delete root;
}

void buildHuffmancode(HuffNode* root,string code,unordered_map<char, string>& huffmancodes) {
	if (root->isLeaf()) {
		LeafNode* Root = dynamic_cast<LeafNode*>(root);
		huffmancodes[Root->val()] = code;
	}
	else {
		IntlNode* Root = dynamic_cast<IntlNode*>(root);
		buildHuffmancode(Root->left(), code + "0", huffmancodes);
		buildHuffmancode(Root->right(), code + "1", huffmancodes);
	}
}
void CaesarString(string& name,map<char,int> tanso) {
	for (auto& x : name) {
		if (isupper(x)) {
			x = (tanso[x] + x - 'A') % 26 + 'A';
		}
		else x = (tanso[x] + x - 'a') % 26 + 'a';
	}
}
void simulate(string filename)
{
    ifstream ss(filename);
    string str, maxsize, name;
	int MAXSIZE = 0, num = 0;
	RestaurantG* G = NULL;
	RestaurantS* S = NULL;
	HuffTree* root = NULL;
	while (ss >> str)
	{
		if (str == "MAXSIZE")
		{
			ss >> maxsize;
			MAXSIZE = stoi(maxsize);
			if (MAXSIZE <= 0) break;
			G = new RestaurantG(MAXSIZE);
			S = new RestaurantS(MAXSIZE);
		}
		else if (str == "LAPSE" && MAXSIZE > 0)
		{
			ss >> name;
			map<char, int> tanSo;
			map<char, int> tanSoCaesar;
			for (char kytu : name) {
				tanSo[kytu]++;
			}
			if (tanSo.size() < 3) continue;
			if (root != NULL) {
				if (!root->root()->isLeaf()) {
					IntlNode* node = dynamic_cast<IntlNode*>(root->root());
					deleteHuffmanTree(node->left());
					deleteHuffmanTree(node->right());
				}
				HuffTree* tmp = root;
				delete tmp;
				root = NULL;
			}
			for (const auto& entry : tanSo) {
				if (isupper(entry.first)) {
					char c = ((entry.first + entry.second - 'A') % 26 + 'A');
					tanSoCaesar[c] += tanSo[entry.first];
				}
				else {
					char c = ((entry.first + entry.second - 'a') % 26 + 'a');
					tanSoCaesar[c] += tanSo[entry.first];
				}
			}
			vector <pair<char, int>> newtanSo(tanSoCaesar.begin(), tanSoCaesar.end());
			sort(newtanSo.begin(), newtanSo.end(),
				[](const auto& a, const auto& b) {
					if (a.second == b.second) {
						if (isupper(a.first) && islower(b.first)) {//neu a hoa va b thuong thi tra false
							return false;
						}
						else if (islower(a.first) && isupper(b.first)) {// nguoc lai
							return true;
						}
						return tolower(a.first) < tolower(b.first);// neu a nho hon thi tra true->a dung truoc
					}
					return a.second < b.second;
				});
			tanSoCaesar.clear();
			vector<HuffTree* > listTree;
			int i = 0;
			for (auto& x : newtanSo) {
				HuffTree* tmp = new HuffTree(x.first, x.second);
				tmp->settime(i);
				listTree.push_back(tmp);
				i++;
			}
			newtanSo.clear();
			HuffTree* huffmanTree = buildHuff(listTree, i);
			root = huffmanTree;
			string bit = "";
			if (huffmanTree == NULL) {

				bit = "0";
			}
			else
			{
				unordered_map<char, string> huffmancodes;
				buildHuffmancode(huffmanTree->root(), "", huffmancodes);
				CaesarString(name, tanSo);
				tanSo.clear();
				for (auto& c : name) {
					bit += huffmancodes[c];
				}
			}
			if (bit.size() >= 10) {
				bit = bit.substr(bit.size() - 10, 10);
			}
			reverse(bit.begin(), bit.end());
			bitset<10> bits(bit);
			int result = bits.to_ulong();
			//cout << result << endl;
			if (result % 2) G->insert(result % MAXSIZE + 1, result);
			else S->insert(result % MAXSIZE + 1, result);
		}
		else if (str == "KOKUSEN" && MAXSIZE > 0) {
			G->KOKUSEN();
		}
		else if (str == "KEITEIKEN" && MAXSIZE > 0) {
			ss >> num;
			S->KEITEIKEN(num);
		}
		else if (str == "LIMITLESS" && MAXSIZE > 0) {
			ss >> num;
			G->LIMITLESS(num);
		}
		else if (str == "CLEAVE" && MAXSIZE > 0) {
			ss >> num;
			S->CLEAVE(num);
		}
		else if (str == "HAND" && MAXSIZE > 0) {
			if (root) inOrder(root->root());
			else continue;
		}
	}
	delete G;
	delete S;
	delete root;
	return;
}
