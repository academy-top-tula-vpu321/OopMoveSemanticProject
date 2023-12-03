#include <iostream>
#include <chrono>
#include <vector>
#include <string>

#include <memory>


class Timer
{
    using Clock = std::chrono::high_resolution_clock;
    using Second = std::chrono::duration<double, std::ratio<1>>;
    std::chrono::time_point<Clock> t_begin{ Clock::now() };

public:
    void Reset() { t_begin = Clock::now(); }
    double Elapsed() const
    {
        return std::chrono::duration_cast<Second>(Clock::now() - t_begin).count();
    }
};

template <typename T>
class Vector
{
    T* array{};
    int size{};
public:
    Vector(int size) : array{ new T[size] }, 
                       size{ size } {}
    ~Vector() { delete[] array; }

    Vector(const Vector& v)
     : size{v.size}
    {
        std::cout << "Copy construct\n";
        array = new T[size];
        for (int i = 0; i < size; i++)
            array[i] = v.array[i];
    }

    Vector(Vector&& v) : size{ v.size },
                        array{ v.array }
    {
        std::cout << "Move construct\n";
        v.size = 0;
        v.array = nullptr;
    }

    Vector& operator=(const Vector& v) = delete;
    /*{
        if (&v == this)
            return *this;
        
        if (array)
            delete[] array;
        
        array = new T[size];
        for (int i = 0; i < size; i++)
            array[i] = v.array[i];

        return *this;
    }*/

    Vector& operator=(Vector&& v)
    {
        if (&v == this)
            return *this;

        if (array)
            delete[] array;

        size = v.size;
        array = v.array;
        
        v.size = 0;
        v.array = nullptr;

        return *this;
    }

    int Size() { return size; }
    T& operator[](int index) { return array[index]; }
    //const T& operator[](int index) { return array[index]; }
};

Vector<int> CloneArrayDouble(Vector<int> v)
{
    Vector<int> res(v.Size());
    for (int i = 0; i < v.Size(); i++)
        res[i] = v[i] * 2;
    return res;
}



template <typename T>
class SmartPointer
{
    T* pointer;
public:
    SmartPointer(T* pointer = nullptr) : pointer{ pointer } {}
    ~SmartPointer() { delete pointer; }

    SmartPointer(const SmartPointer& obj)
    {
        pointer = new T();
        *pointer = *obj.pointer;
    }
    
    SmartPointer(SmartPointer&& robj) : pointer{ robj.pointer }
    {
        robj.pointer = nullptr;
    }


    SmartPointer operator=(const SmartPointer& obj)
    {
        if (&obj == this)
            return *this;

        if (pointer)
            delete pointer;

        pointer = new T();
        *pointer = *obj.pointer;

        return *this;
    }

    SmartPointer operator=(SmartPointer&& robj)
    {
        if (&robj == this)
            return *this;

        if (pointer)
            delete pointer;
        pointer = robj.pointer;
        robj.pointer = nullptr;

        return *this;
    }

    T& operator*() const { return *pointer; }
    T* operator->() const { return pointer; }

    bool IsNull() { return pointer == nullptr; }
};

class Resource
{
    int value;
public:
    Resource(){ std::cout << "Resource create\n"; }
    ~Resource(){ std::cout << "Resource delete\n"; }
    int& Value() { return value; }
};

void ResInfo(SmartPointer<Resource> res)
{

}

SmartPointer<Resource> GenRes()
{
    SmartPointer<Resource> res(new Resource());
    return res;
}

int Five()
{
    int x = 5;
    return x;
}

void func(int& value)
{
    std::cout << "l value object: " << value << "\n";
}

void func(int&& value)
{
    std::cout << "r value object: " << value << "\n";
}

int main()
{
    /*Timer timer;
    
    Vector<int> vint(1000000);
    
    for (int i = 0; i < vint.Size(); i++)
        vint[i] = i + 1;

    vint = CloneArrayDouble(std::move(vint));

    std::cout << timer.Elapsed() << "\n";*/


    //SmartPointer<Resource> smartPtr;
    //smartPtr = GenRes();
    //ResInfo(smartPtr);
    

    /*smartPtr->Value() = 100;
    std::cout << smartPtr.IsNull() << "\n";

    SmartPointer<Resource> smartPtr2(smartPtr);
    std::cout << smartPtr.IsNull() << "\n";
    std::cout << smartPtr2.IsNull() << "\n";*/

    /*int number;
    std::cout << "Input number: ";
    std::cin >> number;

    if (number == 0) return -1;*/

    /*SmartPointer<Resource> smartPtr(new Resource());

    smartPtr->Value() = 100;*/

    /*int x;
    int& xref{ x };
    int&& yref{ 5 };
    int y{ 5 };
    func(y);
    func(5);


    yref = 20;
    std::cout << &yref << "\n";
    std::cout << &xref << "\n";
    std::cout << &x << "\n";

    int z = Five();*/

    /*std::vector<std::string> v;
    std::string str = "Hello";
    v.push_back(std::move(str));
    std::cout << "str: " << str << "\n";
    std::cout << "v[0]: " << v[0] << "\n";

    auto ptr1 = std::make_unique<Resource>();
    ptr1.get()->Value() = 100;*/
    {
        auto resource = new Resource();
        std::shared_ptr<Resource> shptr1(resource);
        std::cout << shptr1.use_count() 
                  << " one shared pointers for resource\n";

        {
            std::shared_ptr<Resource> shptr2(shptr1);
            std::cout << shptr1.use_count() << " " 
                    << shptr2.use_count() 
                    << " two shared pointers for resource\n";
        }

        std::cout << shptr1.use_count()
            << " one shared pointers for resource\n";
    }
    
    std::cout << "zero shared pointers for resource\n";
    
    

    return 0;
}
