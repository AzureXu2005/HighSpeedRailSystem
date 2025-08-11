#pragma once
#include "iostream"
#include "fstream"
#include "string"
#include "deque"

using namespace std;

template<typename T>
class TemplateSet :
	public deque<T>
{
	public:
		TemplateSet(void);
		virtual ~TemplateSet(void);
		// Overloaded deque::push_back for adding a new Person object at the end of the collection.
		bool push_back(const T& t);

		// Overloaded deque::push_front for adding a new Person object at the beginning of the collection.
		void push_front(const T& t);

		// Overloaded deque::insert for adding a new Person object at the specified position in the collection.
		typename deque<T>::iterator insert(typename deque<T>::iterator i, const T& t = T());


		// Reads sequentially from the specified file, constructs Person class objects, and adds them to the collection.
		int ReadFromFile(const char filename[]);

		// Writes the Person class objects in the collection sequentially into the specified file.
		int WriteToFile(const char filename[]);
};

template<typename T>
TemplateSet<T>::TemplateSet(void)
{

}

template<typename T>
TemplateSet<T>::~TemplateSet(void)
{

}

// Overloaded deque::push_back for adding a new Person object at the end of the collection.
template<typename T>
bool TemplateSet<T>::push_back(const T& t)
{
	if (find(deque<T>::begin(), deque<T>::end(), t) == deque<T>::end()) {
		deque<T>::push_back(t);
		return true;
	}
	return false;
}

// Overloaded deque::push_front for adding a new Person object at the beginning of the collection.
template<typename T>
void TemplateSet<T>::push_front(const T& t)
{
	deque<T>::push_front(t);
}

// Overloaded deque::insert for adding a new Person object at the specified position in the collection.
template<typename T>
typename deque<T>::iterator TemplateSet<T>::insert(typename deque<T>::iterator i, const T& t)
{
	return deque<T>::insert(i, t);
}


// Reads sequentially from the specified file, constructs Person class objects, and adds them to the collection.
template<typename T>
int TemplateSet<T>::ReadFromFile(const char filename[])
{
	ifstream fin;
	int count = 0;
	fin.open(filename, ios::in);
	if (!fin) {
		count = -1;
		return -1;
	}

	// 读取第一行但不存储（跳过）
	string firstLine;
	getline(fin, firstLine);

	T t;
	while (!fin.eof()) {
		fin >> t;
		this->push_back(t);
		count++;
	}
	fin.close();
	return count;
}

// Writes the Person class objects in the collection sequentially into the specified file.
template<typename T>
int TemplateSet<T>::WriteToFile(const char filename[]) {
	ifstream fin(filename);
	if (!fin.is_open()) {
		return -1;  // 文件打开失败
	}

	// 读取第一行并保存
	string firstLine;
	getline(fin, firstLine);
	fin.close();

	// 重新打开文件，写入第一行 + 新数据（从第二行开始）
	ofstream fout(filename);
	if (!fout.is_open()) {
		return -1;
	}

	// 写入原始第一行
	fout << firstLine << endl;

	// 从第二行开始写入新数据
	int count = 0;
	for (auto it = this->begin(); it != this->end(); ++it) {
		fout << *it << endl;
		count++;
	}

	fout.close();
	return count;
}