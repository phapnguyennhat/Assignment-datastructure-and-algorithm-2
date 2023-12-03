#include "main.h"
int MAXSIZE;

// setup Huffman tree
class HuffNode{
	public:
		char val;
		int wgt;
		HuffNode* pleft;
		HuffNode* pright;
	public:
		~HuffNode(){
			delete pleft;
			delete pright;
		}
		int weight(){return wgt;}
		char letter(){return val;}
		HuffNode(char letter,int wgt)
		:val(letter), wgt(wgt){
			pleft=NULL;
			pright=NULL;
		}
		HuffNode(HuffNode*l,HuffNode* r){
			wgt=l->weight()+r->weight();
			val ='0';
			pleft=l;
			pright=r;
		}
};
int height(HuffNode *root)
{
	if (root == NULL)
		return 0;
	if (root->pleft == NULL && root->pright == NULL)
		return 1;
	return 1 + max(height(root->pleft), height(root->pright));
}

class HuffTree{
	private:
		HuffNode* root;
		int order;
	public:
		HuffTree(char let,int wgt,int stt){
			root=new HuffNode(let,wgt);
			this->order=stt;
		}
		HuffTree(HuffTree* l,HuffTree* r,int stt){
			// thực hiện nối 2 cây và cân bằng giá trị weight cần được giữ lại
			this->order=stt;
			root=new HuffNode(l->Root(),r->Root());
			if(height(root->pleft)-height(root->pright)>1){
				// can bang trai
				left_balance(root);
			}
			else if(height(root->pright)-height(root->pleft)>1){
				// can bang phai
				right_balance(root);
			}
			// int wgt=root->weight();
			// if(height(root->pleft)-height(root->pright)>1){
			// 	// lech trai
			// 	// l-l
			// 	HuffNode* left_tree=root->pleft;
			// 	if(height(left_tree->pleft)>height(left_tree->pright)){
			// 		rotateRight(root);
			// 	}
			// 	// l-r	
			// 	else if(height(left_tree->pleft)<height(left_tree->pright)){
			// 		rotateLeft(root->pleft);
			// 		rotateRight(root);
			// 	}

			// }
			// else if(height(root->pright)-height(root->pleft)>1){
			// 	//lech phai
			// 	HuffNode* right_tree=root->pright;
			// 	// r-r
			// 	if(height(right_tree->pright)>height(right_tree->pleft)){
			// 		rotateLeft(root);
			// 	}
			// 	// r-l
			// 	else if(height(right_tree->pright)<height(right_tree->pleft)){
			// 		rotateRight(root->pright);
			// 		rotateLeft(root);
			// 	}
			// }
			// root->wgt=wgt;
		}
		HuffNode* Root(){return root;}
		void printInOrderRecur(HuffNode* root){
			if(root==NULL) return;
			printInOrderRecur(root->pleft);
			if(root->letter()!='0') cout<<root->letter()<<endl;
			else cout<<root->weight()<<endl;
			printInOrderRecur(root->pright);
		}
		void printInOrder(){
			printInOrderRecur(root);
		}
		int weight(){
			return root->weight();
		}
		char letter(){return root->letter();}
		int stt(){return order;}
		void rotateLeft(HuffNode*& root){
			HuffNode* right_tree=root->pright;
			root->pright=right_tree->pleft;
			right_tree->pleft=root;
			root=right_tree;
		}
		void rotateRight(HuffNode*& root){
			HuffNode* left_tree=root->pleft;
			root->pleft=left_tree->pright;
			left_tree->pright=root;
			root=left_tree;
		}
		void left_balance(HuffNode*& root){
			HuffNode* left_tree=root->pleft;
			if(height(left_tree->pleft)<height(left_tree->pright)){
				// l-r
				rotateLeft(root->pleft);
				rotateRight(root);
			}
			else{
				// l-l
				// l-e
				rotateRight(root);
			}
			HuffNode* right_tree=root->pright;
			if(height(right_tree->pleft)-height(right_tree->pright)>1){
				left_balance(root->pright);
			}
		}
		void right_balance(HuffNode*& root){
			HuffNode* right_tree=root->pright;
			if(height(right_tree->pleft)>height(right_tree->pright)){
				// r-l
				rotateRight(root->pright);
				rotateLeft(root);
			}
			else{
				// r-r
				// r-e
				rotateLeft(root);
			}
			HuffNode* left_tree=root->pleft;
			if(height(left_tree->pright)-height(left_tree->pleft)>1){
				right_balance(root->pleft);
			}
		}
		map<char,string> encodeChar(){
			//trả về một bảng chuyển đổi từ char qua bits
			map<char,string> res;
			stack<HuffNode*> st;
			stack<string> code;
			code.push("");
			st.push(root);
			HuffNode* sttop=NULL;
			string codetop;
			while(!st.empty()){
				codetop=code.top();
				sttop=st.top();
				code.pop();
				st.pop();
				if(sttop->pleft!=NULL){
					st.push(sttop->pleft);
					code.push(codetop+'0');
				}
				if(sttop->pright!=NULL){
					st.push(sttop->pright);
					code.push(codetop+'1');
				}
				if(sttop->pleft==NULL && sttop->pright==NULL){
					res[sttop->val]=codetop;
				}
			}
			return res;
		}

};

// insertion sort
// void insertionSort(vector<pair<char,int>> &v){
// 	int n=v.size();
// 	for(int i=1;i<n;i++){
// 		pair<char,int> temp=v[i];
// 		int walker=i-1;
// 		while(walker>=0 && temp.second<v[walker].second){
// 			v[walker+1]=v[walker];
// 			walker--;
// 		}
// 		v[walker+1]=temp;
// 	}
// }
map<char,int> mapfreq(string name){
	map<char,int> res;
	for(char c: name){
		res[c]++;
	}
	return res;
}


map<char,int>  mergeFreq(map<char,int> freq, map<char,char>& tableEncode){
	// trả về map tra cứu từ kí tự qua kí tự được mã hóa thành ceasar
	map<char,int> res;
	char key;
	char keyencode;
	for (pair<char,int> x : freq)
	{
		key=x.first;
		if (x.first >= 'a' && x.first <= 'z')
		{

			keyencode = (x.first - 'a' + x.second) % 26 + 'a';
		}
		else if (x.first >= 'A' && x.first <= 'Z')
		{
			keyencode = (x.first - 'A' + x.second) % 26 + 'A';
		}
		res[keyencode]+=x.second;
		tableEncode[key]=keyencode;
	}
	return res;
}
bool prior(const pair<char,int>& a,const pair<char,int>& b){
	if(a.second!=b.second) return a.second<b.second;
	// if(a.first>='a'&&a.first<='z' &&b.first>='a'&&b.first<='z'){
	// 	return a.first<b.first;
	// }
	// if(a.first>='A'&&a.first<='Z'&& b.first>='A'&&b.second<='Z'){
	// 	return a.first<b.first;
	// }
	return a.first<b.first;
}
string encodeNameCeasar(map<char,char> tableEncodeCearse, string nameCustomer){
	string res="";
	for(char c:nameCustomer){
		res+=tableEncodeCearse[c];
	}
	return res;
}
string encodeNameBit(map<char,string>& hashcode, string& nameCeasar ){
	string res="";
	for(char c:nameCeasar){
		res+=hashcode[c];
	}
	return res;
}
int bin2Dec(string bits){
	int res=0;
	//cout<<bits<<endl;
	int n=bits.size();
	bits=(n>10)? bits.substr(0,10): bits;
	n=bits.size();
	for(unsigned int i=0;i<n;i++){
		res=res+(bits[i]-'0')*pow(2,n-1-i);
	}
	return res;
}
// heap for hufftree
class HeapHuffTree{
	private:
		vector<HuffTree*> forest;
		int order;
	public:
		// ~HeapHuffTree(){
		// 	for(unsigned int i=0;i<forest.size();i++){
		// 		delete forest[i];
		// 	}
		// }
		HeapHuffTree(vector<pair<char,int>>& v){
			this->order=0;
			for(pair x:v){
				insert(new HuffTree(x.first,x.second,order++));
			}
		}
		int size(){return forest.size();}
		void insert(HuffTree* item){
			forest.push_back(item);
			reheapUp(forest.size()-1);
		}
		HuffTree* top(){
			return forest[0];
		}
		void popMin(){
			forest[0]=forest[forest.size()-1];
			forest.pop_back();
			reheapDown(0);
		}
		void reheapUp(int pos){
			if(pos!=0){
				int parent=(pos-1)/2;
				if(forest[pos]->weight()<forest[parent]->weight()){
					swap(forest[pos],forest[parent]);
					reheapUp(parent);
				}
				else if(forest[pos]->weight()==forest[parent]->weight() && forest[pos]->stt()<forest[parent]->stt()){
					swap(forest[pos],forest[parent]);
					reheapUp(parent);
				}
			}
		}
		void reheapDown(int pos){
			int smallest=pos;
			int leftChild=pos*2+1;
			int rightChild=pos*2+2;
			int n=forest.size();

			if(leftChild<n && ((forest[leftChild]->weight()<forest[smallest]->weight()) || (forest[leftChild]->weight()==forest[smallest]->weight() && forest[leftChild]->stt()<forest[smallest]->stt()))){
				smallest=leftChild;
			}
			
			if(rightChild<n && ((forest[rightChild]->weight()< forest[smallest]->weight()) || (forest[rightChild]->weight()==forest[smallest]->weight() && forest[rightChild]->stt()<forest[smallest]->stt()))){
				smallest=rightChild;
			}
			if(smallest!=pos){
				swap(forest[pos],forest[smallest]);
				reheapDown(smallest);
			}
		}
		HuffTree* buildHuff(){
			HuffTree* temp1,* temp2,*temp3=NULL;
			while(size()>1){
				temp1=top();
				popMin();
				temp2=top();
				popMin();
				 //cout<<"temp1: "<<temp1->letter()<<" "<<temp1->weight()<<endl;
				 //cout<<"temp2: "<<temp2->letter()<<" "<<temp2->weight()<<endl;
				temp3=new HuffTree(temp1,temp2,order++);
				delete temp1;
				delete temp2;
				insert(temp3);
				 //for(int i=0;i<forest.size();i++) cout<<"("<<forest[i]->letter()<<" "<<forest[i]->weight()<<")"<<" ";
			}
			return temp3;
		}

};
class Restaurant{
	private:
		// cần lưu thông tin của khách hàng gần đây nhất
		// table decode ceasar
		// map<char,char> decodeCeasar;
		// hufftree 
		HuffTree* tree;
		
		
	public:
		class NhaG{
			public:
				class Node{
					public:
						int val;
						Node* pLeft;
						Node* pRight;
					public:
						~Node(){
							delete pLeft;
							delete pRight;
						}
						Node(int val){
							this->val=val;
							pLeft=NULL;
							pRight=NULL;
						}

				};
				class BST{
					private:
						Node* root;
						queue<int> orderPush;
					public:
						~BST(){
							delete root;
						}
						BST(){
							root=NULL;
						}
						Node* insertRecur(Node* root,int val){
							
							if(root==NULL) return new Node(val);
							if(val<root->val) root->pLeft=insertRecur(root->pLeft,val);
							else root->pRight=insertRecur(root->pRight,val);
							return root;
						}
						void insert(int val){
							orderPush.push(val);
							root=insertRecur(root,val);
						}

						Node* deleteNodeRecur(Node* root,int val){
							if(root==NULL) return root;
							if(val<root->val){
								root->pLeft=deleteNodeRecur(root->pLeft,val);
								return root;
							}
							else if(val>root->val){
								root->pRight=deleteNodeRecur(root->pRight,val);
								return root;
							}
							if(root->pLeft==NULL){
								Node* tmp=root->pRight;
								root->pRight=NULL;
								delete root;
								return tmp;
							}
							else if(root->pRight==NULL){
								Node* tmp=root->pLeft;
								root->pLeft=NULL;
								delete root;
								return tmp;
							}
							else{
								Node* succParent=root;
								Node* succ=root->pRight;
								while(succ->pLeft!=NULL){
									succParent=succ;
									succ=succ->pLeft;
								}
								if(succParent!=root){
									succParent->pLeft=succ->pRight;
								}
								else{
									succParent->pRight=succ->pRight;
								}
								root->val=succ->val;
								succ->pLeft=NULL;
								succ->pRight=NULL;
								delete succ;
								return root;
							}
						}
						void deleteNode(int val){
							root=deleteNodeRecur(root,val);
						}
						void kickCus(int num){
							// đuổi num khách hàng từ hàng đợi;
							int sizeTree=orderPush.size();
							if(num>=sizeTree){
								clear();
								return;
							}
							for(int i=0;i<num;i++){
								deleteNode(orderPush.front());
								orderPush.pop();
							}
						}
						void printfInOrderRecur(Node* root){
							if(root==NULL) return;
							printfInOrderRecur(root->pLeft);
							cout<<root->val<<endl;
							printfInOrderRecur(root->pRight);
						}
						void printfInOrder(){
							printfInOrderRecur(root);
						}
						void clear(){
							delete root;
							while(!orderPush.empty()){
								orderPush.pop();
							}
							root=NULL;
						}
						int sizeTree(){
							int n=orderPush.size();
							return n;
						}
						vector<int> arrTree()
						{
							queue<int> copy = orderPush;
							vector<int> res;
							while (!copy.empty())
							{
								res.push_back(copy.front());
								copy.pop();
							}
							return res;
						}
						vector<long long> arrFactorial(int n)
						{
							vector<long long> res(n + 1, 0);
							res[0] = 1;
							res[1] = 1;
							for (int i = 2; i < n; i++)
							{
								res[i] = res[i - 1] * i;
							}
							return res;
						}
						long long tohop(int k, int n, vector<long long> &arrFactorial)
						{
							if (k > n) return -1; //error
							return arrFactorial[n] / (arrFactorial[k] * arrFactorial[n - k]);
						}
						long long countWay(vector<int> arrTree, vector<long long> &arrFactorial)
						{
							int n = arrTree.size();
							if (n <= 2)
								return 1;
							int root = arrTree[0];
							vector<int> left_tree;
							vector<int> right_tree;
							for (int i = 1; i < n; i++)
							{
								if (arrTree[i] < root)
									left_tree.push_back(arrTree[i]);
								else
									right_tree.push_back(arrTree[i]);
							}
							int sizeL = left_tree.size();
							int sizeR = right_tree.size();
							return countWay(left_tree, arrFactorial) * countWay(right_tree, arrFactorial) * tohop(sizeL, n - 1, arrFactorial);
						}
						long long y(){
							vector<long long> fact=arrFactorial(orderPush.size());
							return countWay(arrTree(),fact);
						}

				};
				void selectRegion(int result){
					hash[result%MAXSIZE+1]->insert(result);
				}
				void kokusen(){
					for(int i=1;i<=MAXSIZE;i++){
						hash[i]->kickCus(hash[i]->y()%MAXSIZE);
					}
				}
				void limitless(int num){
					if(num<1 || num>MAXSIZE) return;
					hash[num]->printfInOrder();
				}
				NhaG(){
					for(int i=1;i<=MAXSIZE;i++){
						hash[i]=new BST();
					}
				}

			private:
				map<int,BST*> hash;
		};
		class NhaS{
			public:
				NhaS(){
					timeLine=0;
				}
				~NhaS(){
					for(int i=0;i<minHeap.size();i++){
						delete minHeap[i];
					}
				}
				class Region{
					// khu vuc
					private:
						queue<int> orderPush;
						int id;
						int timeLineChange;
					public:
						Region(int id,int res,int timeLineChange){
							this->id=id;
							orderPush.push(res);
							this->timeLineChange=timeLineChange;
						}
						void insert(int res,int timeChange){
							this->timeLineChange=timeChange;
							orderPush.push(res);
						}
						queue<int> copyQueue(){return orderPush;}
						int ID(){return id;}
						int sizeRegion(){return orderPush.size();}
						int timeChange(){return timeLineChange;}
						void removeNum(int num,int timeChange){
							num = (num > sizeRegion()) ? sizeRegion() : num;
							for (int i = 0; i < num; i++)
							{
								timeLineChange=timeChange;
								cout<<orderPush.front()<<"-"<<ID()<<endl;
								orderPush.pop();
							}
						}
				};
				void popMin(){
					Region* del=top();
					minHeap[0]=minHeap[minHeap.size()-1];
					minHeap.pop_back();
					delete del;
					reheapDown(0);
				}
				Region* top(){
					if(minHeap.size()==0) return NULL;
					return minHeap[0];
				}
				void reheapUp(int pos)
				{
					if (pos != 0)
					{
						int parent = (pos - 1) / 2;
						if (minHeap[pos]->sizeRegion() < minHeap[parent]->sizeRegion())
						{
							swap(minHeap[pos], minHeap[parent]);
							reheapUp(parent);
						}
						else if (minHeap[pos]->sizeRegion() == minHeap[parent]->sizeRegion() && minHeap[pos]->timeChange() < minHeap[parent]->timeChange())
						{
							swap(minHeap[pos], minHeap[parent]);
							reheapUp(parent);
						}
					}
				}
				void reheapDown(int pos)
				{
					int smallest = pos;
					int leftChild = pos * 2 + 1;
					int rightChild = pos * 2 + 2;
					int n = minHeap.size();

					if (leftChild < n && ((minHeap[leftChild]->sizeRegion() < minHeap[smallest]->sizeRegion()) || (minHeap[leftChild]->sizeRegion() == minHeap[smallest]->sizeRegion() && minHeap[leftChild]->timeChange() < minHeap[smallest]->timeChange())))
					{
						smallest = leftChild;
					}

					if (rightChild < n && ((minHeap[rightChild]->sizeRegion() < minHeap[smallest]->sizeRegion()) || (minHeap[rightChild]->sizeRegion() == minHeap[smallest]->sizeRegion() && minHeap[rightChild]->timeChange() < minHeap[smallest]->timeChange())))
					{
						smallest = rightChild;
					}
					if (smallest != pos)
					{
						swap(minHeap[pos], minHeap[smallest]);
						reheapDown(smallest);
					}
				}
				int searchRegion(int id){
					for(int i=0;i<minHeap.size();i++){
						if(id==minHeap[i]->ID()) return i;
					}
					return -1;
				}
				void selectRegion(int result){
					// tìm khu vực đó trong heap
					// thực hiện chèn vào
					int id=result%MAXSIZE+1;
					int pos=searchRegion(id);
					if(pos==-1){
						//not found
						minHeap.push_back(new Region(id,result,timeLine++));
						reheapUp(minHeap.size()-1);
					}
					else{
						minHeap[pos]->insert(result,timeLine++);
						reheapDown(pos);
					}
				}
				void keiteiken(int num){
					// xóa num các khách hàng ở num region có số lượng ít nhất theo FIFO // ở đầu luôn
					for(int i=0;i<num;i++){
						if(minHeap.size()==0) return;
						top()->removeNum(num,timeLine++);
						if(top()->sizeRegion()==0) popMin();
					}
				}
				void traversalPreOrder(int root,int num){
					if(root<minHeap.size()){
						// in num khách hàng LIFO
						queue<int> copy=minHeap[root]->copyQueue();
						int id=minHeap[root]->ID();
						stack<int> st;
						while(!copy.empty()){
							st.push(copy.front());
							copy.pop();
						}
						num=(num>st.size())? st.size():num;
						for(int i=0;i<num;i++){
							cout<<id<<"-"<<st.top()<<endl;
							st.pop();
						}
						traversalPreOrder(2*root+1,num);
						traversalPreOrder(2*root+2,num);
					}
				}
				void cleave(int num)
				{
					if(minHeap.size()==0) return;
					traversalPreOrder(0,num);
					// duyệt thứ tự pre order trong minheap
					// in num khách hàng trong node theo LIFO ID-result <<endl;
					
				}

			private:
				vector<Region*> minHeap;
				int timeLine;
				// minHeap để region có size nhỏ nhất và chưa đc sử dụng lâu nhất ở trên;
		};
		void lapse(string name)
		{
			// name="abaaabbbDd";
			map<char,int> freq=mapfreq(name);
			if(freq.size()<3) return;
			// freq xuất hiên của kí tự vì yêu cầu tính ổn định nên sẽ dùng insertion sort
			//insertionSort(freq);
			map<char,char> tableEncodeCeasar ;//= codeCeasar(freq);
			map<char,int> merge=mergeFreq(freq,tableEncodeCeasar);
			
			// gop cac ki tu trung nhau
			vector<pair<char,int>> encode(merge.begin(),merge.end());
			stable_sort(encode.begin(),encode.end(),prior);
			// for(auto x: encode) cout<<"("<<x.first<<" "<<x.second<<" "<<")";

			HeapHuffTree* newHeaphuff=new HeapHuffTree(encode);
			if(tree!=NULL)
			{
				delete tree->Root();
				delete tree;
			}
			tree=newHeaphuff->buildHuff();
			map<char,string> hashcode=tree->encodeChar();
			string nameEncodeCeasar=encodeNameCeasar(tableEncodeCeasar,name);
			string nameBits=encodeNameBit(hashcode,nameEncodeCeasar);
			int result=bin2Dec(nameBits);
			//le gojo chan thi sukuna
			if(result%2==1) gojo->selectRegion(result);
			else sukuna->selectRegion(result);
			delete newHeaphuff;
			// tim result;
		}
		void kokusen(){
			gojo->kokusen();
		}
		void keiteiken(int num){
			sukuna->keiteiken(num);
		}
		void hand(){
			// nếu k có gì thay đổi thì sẽ tạo một huffmanTree với char và freq của vector freqfreq
			tree->printInOrder();
		};
		void limitless(int num){
			// in in-order của cây ở khu vực num
			gojo->limitless(num);
		}
		void cleave(int num){
			sukuna->cleave(num);
		}
		Restaurant(){
			gojo=new NhaG();
			sukuna=new NhaS();
			tree=NULL;
		}
	private:
		NhaG* gojo;
		NhaS* sukuna;

};

void simulate(string filename)
{
	ifstream ss(filename);
	string str,maxsize,name,num;
	ss>>str;
	if(str=="MAXSIZE"){
		ss>>maxsize;
		MAXSIZE=stoi(maxsize);
	}
	Restaurant* r =new Restaurant();
	while (ss >> str)
	{
		if (str == "MAXSIZE")
		{
			ss >> maxsize;
			MAXSIZE = stoi(maxsize);
		}
		else if (str == "LAPSE")
		{
			ss >> name;
			r->lapse(name);
		}
		else if (str == "KOKUSEN")
		{
			r->kokusen();
		}
		else if (str == "KEITEIKEN")
		{
			ss >> num;
			r->keiteiken(stoi(num));
		}
		else if (str == "HAND")
		{
			r->hand();
		}
		else if (str == "LIMITLESS")
		{
			ss >> num;
			r->limitless(stoi(num));
		}
		else
		{
			// CLEAVE
			ss >> num;
			r->cleave(stoi(num));
		}
	}
	//cout<<"good luck";
	delete r;
	return;
}