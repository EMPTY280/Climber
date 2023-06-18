#pragma once
#include <vector>

template <typename T>
class DataList
{
private:
	std::vector<T> datalist;

public:
	~DataList() { }

	int AddData(T data)
	{
		datalist.push_back(data);
		return GetSize() - 1;
	}

	void DeleteData(int index)
	{
		datalist.erase(datalist.begin() + index);
	}

	T& GetData(int index) const
	{
		return datalist.at(index);
	}

	int GetSize() const
	{
		return datalist.size();
	}

	T& operator[](int index)
	{
		return *datalist.at(index);
	}
};

template <typename T>
class DataList<T*>
{
private:
	std::vector<T*> datalist;

public:
	~DataList()
	{
		for (T* ele : datalist)
		{
			delete ele;
		}
	}

	int AddData(T* data)
	{
		datalist.push_back(data);
		return GetSize() - 1;
	}
	
	void DeleteData(int index)
	{
		T* data = datalist.at(index);
		datalist.erase(datalist.begin() + index);
		delete data;
	}

	T& GetData(int index) const
	{
		return *datalist.at(index);
	}

	int GetSize() const
	{
		return datalist.size();
	}

	T& operator[](int index)
	{
		return *datalist.at(index);
	}
};