#include <iostream>
using namespace std;

template <typename T>
class vector {
    private:
    T* container;
    int length;
    int capacity;

    protected:
    void expansion();
    void compression();

    public:
    vector()
    {
        container = new T[2];
        length = 0;
        capacity = 2;
    }
    
    void push_back(T val);
    void pop_back();
    int size();
    bool empty();
};

template <typename T>
void vector<T>::expansion()
{
    if(size==(capacity/2))
    {
        T* temp = new T[2*capacity];
        for(int i=0; i<size; i++)
        {
            temp[i] = container[i];
        }
        delete[] container;
        container = temp;
        capacity = 2*capacity;
    }
}

template <typename T>
void vector<T>::compression()
{
    if(length<(capacity/4) && capacity>2)
    {
        T* temp = new T[capacity/2];
        for(int i=0; i<length; i++)
        {
            temp[i] = container[i];
        }
        delete[] container;
        container = temp;
        capacity = capacity/2;
    }
}

template <typename T>
void vector<T>::push_back(T val)
{
    expansion();
    container[length] = item;
    length++;
}

template <typename T>
void vector<T>::pop_back()
{
    length--;
    compression();
}

template <typename T>
int vector<T>:: size()
{
    return length;
}

template <typename T>
bool vector<T>:: empty()
{
    if(length==0)return true;
    return false;
}
