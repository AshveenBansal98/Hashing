#include <bits/stdc++.h>
using namespace std;

class Bucket{
	vector <int> values;
	Bucket* overflow;
	int size;
	public:
	Bucket(int size){
		this->size = size;
		overflow = NULL;
	}
	void insert(int key);
	void copy(vector <int> &temp);
	int search(int key);
	void display();
	int del(int key);
	bool isEmpty();
	bool isOverflow();
};

bool Bucket :: isEmpty(){
    return (values.size() == 0);
}

bool Bucket :: isOverflow(){
   return (overflow != NULL);
}

int Bucket :: del(int key){
	for(int i = 0; i < values.size(); i++){
		if (values[i] == key){
			values.erase(values.begin() + i);
			if (overflow){
				values.push_back(overflow->values[0]);
				int status = overflow->del(overflow->values[0]);
				if (overflow->isEmpty()){
					delete(overflow);
					overflow = NULL;
				}
			}
			return 1;
		}
	}
	if (overflow){
		int status = overflow->del(key);
		if (overflow->isEmpty()){
			delete(overflow);
			overflow = NULL;
		}
		return status;
	}
	else
		return 0;
}

void Bucket :: display(){
	for(int i = 0; i < values.size(); i++)
		cout << values[i] << " ";
	if (overflow)
		overflow->display();
	else
		cout << endl;
}

void Bucket :: insert(int key){
	if (values.size() < size)
		values.push_back(key);
	else{
		if (overflow == NULL)
			overflow = new Bucket(size);
		overflow->insert(key);
	}

}

void Bucket :: copy(vector <int> &temp){
	for(int i = 0; i < values.size(); i++)
		temp.push_back(values[i]);
	values.clear();
	if (overflow){
		overflow->copy(temp);
		delete(overflow);
		overflow = NULL;
	}
}

int Bucket :: search(int key){
	for(int i = 0; i < values.size(); i++)
		if (values[i] == key)
			return 1;
	else if (overflow)
		return overflow->search(key);
    return 0;
}

class Hashmap{
	int numrecords;
	int next;
	int hash (int n);
	void reinsert(int bucket_num);
	int numbits;
	int bucketsize;
	vector <Bucket *> buckets;
	public:
	Hashmap(int numbits, int bucketsize){
		this->numbits = numbits;
		this->bucketsize = bucketsize;
		next = 0;
		numrecords = 1 << numbits;
		for(int i = 0; i < numrecords; i++)
			buckets.push_back(new Bucket(bucketsize));
	}
	void insert(int key);
	void reinsert(vector <int> &temp);
	void del(int key);
	void search(int key);
	void display();
};

int Hashmap :: hash(int n){
	return n&( (1 << numbits) - 1);
}

void Hashmap :: reinsert(vector <int> &temp){
	for(int i = 0; i < temp.size(); i++){
        int key = temp[i];
		int num = hash(key);
		buckets[num]->insert(key);
	}
}

void Hashmap :: insert(int key){
	int num = hash(key);
	if (num >= buckets.size()){
		num = num - (1 << (numbits - 1));
	}
	buckets[num]->insert(key);
	if (!buckets[num]->isOverflow())
		return;
    buckets.push_back(new Bucket(bucketsize));
	numbits = ceil(log2((double)buckets.size()));
	vector <int> temp;
	buckets[next]->copy(temp);
	reinsert(temp);
	next++;
	if (next == 1 << (numbits-1)){
		next = 0;
	}
}

void Hashmap :: display(){
	cout << "INDEX OF NEXT BUCKET TO BE SPLIT " << next << endl;
	for(int i = 0; i < buckets.size(); i++){
		cout << i << " ";
		buckets[i]->display();
	}
}

void Hashmap :: search(int key){
	int num = hash(key);
	if (num < buckets.size()){
		int status = buckets[num]->search(key);
		if (status)
			cout << key << " found at bucket index " << num << endl;
		else
			cout << key << " not found" << endl;
	}
	else{
		num -= 1 << (numbits - 1);
		int status = buckets[num]->search(key);
		if (status)
			cout << key << " found at bucket index " << num << endl;
		else
			cout << key << " not found" << endl;
	}
}

void Hashmap :: del(int key){
	int num = hash(key);
	if (num < buckets.size()){
		int status = buckets[num]->del(key);
		if (status)
			cout << key << " deleted from bucket index " << num << endl;
		else
			cout << key << " not found" << endl;
	}
	else{
		num -= 1 << (numbits - 1);
		int status = buckets[num]->del(key);
		if (status)
			cout << key << " deleted from bucket index " << num << endl;
		else
			cout << key << " not found" << endl;
	}
}

int main(){
	int numbits, bucket_size;
	cin >> numbits >> bucket_size;
	Hashmap h(numbits, bucket_size);
	int inp;
	cin >> inp;
	while(inp != -1){
		if (inp == 2){
			int key;
			cin >> key;
			h.insert(key);
		}
		else if (inp == 3){
			int key;
			cin >> key;
			h.search(key);
		}
		else if (inp == 4){
			int key;
			cin >> key;
			h.del(key);
		}
		else if (inp == 5){
            h.display();
		}
		cin >> inp;
	}

}
