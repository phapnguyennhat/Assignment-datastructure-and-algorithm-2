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
			int wgt=root->weight();
			if(height(root->pleft)-height(root->pright)>1){
				// lech trai
				// l-l
				HuffNode* left_tree=root->pleft;
				if(height(left_tree->pleft)>height(left_tree->pright)){
					rotateRight(root);
				}
				// l-r
				else if(height(left_tree->pleft)<height(left_tree->pright)){
					rotateLeft(root->pleft);
					rotateRight(root);
				}

			}
			else if(height(root->pright)-height(root->pleft)>1){
				//lech phai
				HuffNode* right_tree=root->pright;
				// r-r
				if(height(right_tree->pright)>height(right_tree->pleft)){
					rotateLeft(root);
				}
				// r-l
				else if(height(right_tree->pright)<height(right_tree->pleft)){
					rotateRight(root->pright);
					rotateLeft(root);
				}
			}
			root->wgt=wgt;
		}
		HuffNode* Root(){return root;}
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
		map<char,string> encodeChar(){
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
void insertionSort(vector<pair<char,int>> &v){
	int n=v.size();
	for(int i=1;i<n;i++){
		pair<char,int> temp=v[i];
		int walker=i-1;
		while(walker>=0 && temp.second<v[walker].second){
			v[walker+1]=v[walker];
			walker--;
		}
		v[walker+1]=temp;
	}
}

void freqInsert(char key,vector<pair<char,int>> &v){
	for(unsigned int i=0;i<v.size();i++){
		if(v[i].first==key){
			v[i].second++;
			return;
		}
	}
	v.push_back(make_pair(key,1));
}

void mergeChar(vector<pair<char,int>>& freq,vector<pair<char,int>> & encode){
	bool check=false;
	for(pair<char,int> x:freq){
		check=false;
		for(pair<char,int>& y: encode){
			if(x.first==y.first){
				y.second+=x.second;
				check=true;
				break;
			}
		}
		if(check==false){
			encode.push_back(make_pair(x.first,x.second));
		}
	}
}

void codeCeasar(vector<pair<char,int>> &v){
	for (auto &x : v)
	{
		if (x.first >= 'a' && x.first <= 'z')
		{
			x.first = (x.first - 'a' + x.second) % 26 + 'a';
		}
		else if (x.first >= 'A' && x.first <= 'Z')
		{
			x.first = (x.first - 'A' + x.second) % 26 + 'A';
		}
	}
}
bool prior(const pair<char,int>& a,const pair<char,int>& b){
	if(a.second!=b.second) return a.second<b.second;
	if(a.first>='a'&&a.first<='z' &&b.first>='a'&&b.first<='z'){
		return a.first<b.first;
	}
	if(a.first>='A'&&a.first<='Z'&& b.first>='A'&&b.second<='Z'){
		return a.first<b.first;
	}
	return a.first>b.first;
}

// heap for hufftree
class HeapHuffTree{
	private:
		vector<HuffTree*> forest;
		int order;
	public:
		~HeapHuffTree(){
			for(unsigned int i=0;i<forest.size();i++){
				delete forest[i];
			}
		}
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
				// cout<<"temp1: "<<temp1->letter()<<" "<<temp1->weight()<<endl;
				// cout<<"temp2: "<<temp2->letter()<<" "<<temp2->weight()<<endl;
				temp3=new HuffTree(temp1,temp2,order++);
				delete temp1;
				delete temp2;
				insert(temp3);
				// for(int i=0;i<forest.size();i++) cout<<"("<<forest[i]->letter()<<" "<<forest[i]->weight()<<")"<<" ";
			}
			return temp3;
		}

};
class Restaurant{
	
	public:
		class NhaG{
			
		};
		class NhaS{

		};
		void lapse(string name)
		{
			name="abcdefghi";
			vector<pair<char,int>> freq;
			for(char c:name){
				freqInsert(c,freq);
			}
			if(freq.size()<3) return;
			// freq xuất hiên của kí tự vì yêu cầu tính ổn định nên sẽ dùng insertion sort
			insertionSort(freq);
			codeCeasar(freq);
			
			// gop cac ki tu trung nhau
			vector<pair<char,int>> encode;
			mergeChar(freq,encode);
			stable_sort(encode.begin(),encode.end(),prior);
			// for(auto x: encode) cout<<"("<<x.first<<" "<<x.second<<" "<<")";

			HeapHuffTree* newHeaphuff=new HeapHuffTree(encode);
			HuffTree* tree=newHeaphuff->buildHuff();
			map<char,string> hashcode=tree->encodeChar();
			for(auto x:hashcode){
				cout<<x.first<<": "<<x.second<<" ";
			}


			delete tree->Root();
			delete newHeaphuff;
			// tim result;
		}
		void kokusen(){

		}
		void keiteiken(int num){

		}
		void hand(){

		};
		void limitless(int num){

		}
		void cleave(int num){

		}

	private:
		NhaG* gojo;
		NhaS* sukuna;

};

void simulate(string filename)
{
	Restaurant* r =new Restaurant();
	ifstream ss(filename);
	string str,maxsize,name,num;
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
	cout<<"good luck";
	delete r;
	return;
}