#pragma once
#include <vector>

template <typename T>
class DataList
{
private:
	std::vector<T*> datalist;

public:
	DataList();
	~DataList();

	int AddData(T* data);
	T& GetData(int index) const;
	int GetLength() const;
};


template <typename T>
DataList<T>::DataList()
{

}

template <typename T>
DataList<T>::~DataList()
{
	for (T* ele : datalist)
	{
		delete ele;
	}
}

template<typename T>
int DataList<T>::AddData(T* data)
{
	datalist.push_back(data);
	return GetLength() - 1;
}

template<typename T>
T& DataList<T>::GetData(int index) const
{
	return *datalist.at(index);
}

template<typename T>
int DataList<T>::GetLength() const
{
	return datalist.size();
}
