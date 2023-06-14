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
	int GetSize() const;

	T& operator[](int index)
	{
		return *datalist.at(index);
	}
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
	return GetSize() - 1;
}

template<typename T>
T& DataList<T>::GetData(int index) const
{
	return *datalist.at(index);
}

template<typename T>
int DataList<T>::GetSize() const
{
	return datalist.size();
}
