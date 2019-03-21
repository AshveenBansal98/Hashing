#include <bits/stdc++.h>
using namespace std;

class Bucket{
	vector <int> m;
	int localdepth;
	int size;
	public:
	Bucket(int depth, int size){
		this->size = size;
		this->localdepth = depth;
	}
	bool isFull();
	bool isEmpty();
	int insert(int key);
	int search(int key);
	void copy(vector <int> &temp);
	int getdepth();
	void display();
	void increasedepth();
	void del(int key);
};

void Bucket :: del(int key){
	for(int i = 0; i < m.size(); i++)
		if (m[i] == key){
			m.erase(m.begin() + i);
			return;
		}
	cout << "Key not found" << endl;
}
void Bucket :: display(){
    cout << this->localdepth << "   ";
    for(int i = 0; i < m.size(); i++)
        cout << m[i] << " ";
    cout << endl;
}

int Bucket :: getdepth(){
    return this->localdepth;
}

void Bucket :: increasedepth(){
    this->localdepth++;

}
bool Bucket :: isFull(){
	if (m.size() == size)
		return true;
	else
		return false;
}

int Bucket :: insert(int key){
	if (isFull())
		return -1;
	for(int i = 0; i < m.size(); i++)
		if (m[i] == key)
			return 0;
	m.push_back(key);
	return 1;
}

int Bucket :: search(int key){
	for(int i = 0; i < m.size(); i++)
		if (m[i] == key)
			return 1;
	return 0;
}

void Bucket :: copy(vector <int> &temp){
    for(int i = 0; i < m.size(); i++)
        temp.push_back(m[i]);
    m.clear();
}

class Directory{
	int globaldepth;
	int bucketsize;
	vector <Bucket *> buckets;
	int hash (int n);
	void split(int bucket_num);
	void reinsert(int bucket_num);
	void Doubledirectory();
	public:
	Directory(int globaldepth, int bucket_size){
		this->globaldepth = globaldepth;
		this->bucketsize = bucket_size;
		for(int i = 0; i <  (1 << globaldepth); i++)
			buckets.push_back(new Bucket(globaldepth, bucket_size));
	}
	void insert(int key);
	void del(int key);
	void search(int key);
	void display();
};

void Directory :: del(int key){
	int bucket_num = hash(key);
	buckets[bucket_num]->del(key);
}
int Directory :: hash (int n){
	return n&( (1 << globaldepth) - 1);
}

void Directory :: Doubledirectory(){
	for(int i = 0; i < (1 << globaldepth); i++)
		buckets.push_back(buckets[i]);
	this->globaldepth++;
	//cout << "line 94 " << this->globaldepth << endl;;
}

void Directory :: reinsert(int bucket_num){
	vector <int> temp;
	buckets[bucket_num]->copy(temp);
	for(int i = 0; i < temp.size(); i++){
		int key = temp[i];
		int bucket_num  = hash(key);
		//cout << "new bucket_num" << bucket_num << endl;
		int flag = buckets[bucket_num]->insert(key);
		if (flag == -1){
			split(bucket_num);
			buckets[bucket_num]->insert(key);
		}
	}
}

void Directory :: split(int bucket_num){
	int localdepth = buckets[bucket_num]->getdepth();
	//cout << globaldepth << " " << localdepth << endl;
	if (globaldepth == localdepth)
		Doubledirectory();
	int mirrorindex = bucket_num ^ (1 << localdepth);
	buckets[bucket_num]->increasedepth();
	localdepth++;
	//cout << buckets[bucket_num]->getdepth() << endl;
	buckets[mirrorindex] = new Bucket(localdepth, bucketsize);
	int num = 1 << localdepth;
	for(int i = mirrorindex + num; i < (1 << globaldepth); i += num)
		buckets[i] = buckets[mirrorindex];
	for(int i = mirrorindex - num; i >=0 ; i -= num)
		buckets[i] = buckets[mirrorindex];
	reinsert(bucket_num);
}

void Directory :: insert(int key){
	int bucket_num = hash(key);
	int flag = buckets[bucket_num]->insert(key);
	if (flag == -1){
        cout << "splitting bucketnum " << bucket_num << endl;
		split(bucket_num);
		insert(key);
	}
	else if (flag == 0){
		cout << "Key already exists" << endl;
	}
	else{
		cout << key << " inserted in bucket having number " << bucket_num << endl;
	}
}

void Directory :: search (int key){
	int bucket_num = hash(key);
	int flag = buckets[bucket_num]->search(key);
	if (flag == 1){
		cout << key << " exists in bucket number " << bucket_num <<  endl;
	}
	else{
		cout << key << " doesnot exist " << endl;
	}
}

void Directory :: display(){
    set <Bucket*> s;
	for(int i = 0; i < (1 << globaldepth); i++){
        if (s.find(buckets[i]) != s.end())
            continue;
		cout << i << ": ";
		buckets[i]->display();
        s.insert(buckets[i]);
	}
}

int main(){
	int globaldepth, bucket_size;
	cin >> globaldepth >> bucket_size;
	Directory d(globaldepth, bucket_size);
	int inp;
	cin >> inp;
	while(inp != -1){
		if (inp == 2){
			int key;
			cin >> key;
			d.insert(key);
		}
		else if (inp == 3){
			int key;
			cin >> key;
			d.search(key);
		}
		else if (inp == 3){
			int key;
			cin >> key;
			d.del(key);
		}
		else if (inp = 5){
            d.display();
		}
		cin >> inp;
	}

}
