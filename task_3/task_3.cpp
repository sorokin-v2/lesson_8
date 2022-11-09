#include <iostream>

template <typename T>
class My_unique_ptr {
public:
    
    My_unique_ptr(): ptr_(nullptr) {}                 //ptr_ инициализируется nullptr ( T* ptr_{ nullptr };), по-этому этот конструктор не обязателен?
    // Explicit constructor
    explicit My_unique_ptr(T* src) : ptr_(src) {}       //Указатель сознательно не константный
    // Constructor/Assignment that binds to nullptr
    // This makes usage with nullptr cleaner
    My_unique_ptr(std::nullptr_t) : ptr_(nullptr) {}  //ptr_ инициализируется nullptr ( T* ptr_{ nullptr };), по-этому этот конструктор не обязателен?
/*
    //Перемещающий конструктор от буржуев
    My_unique_ptr(My_unique_ptr&& moving) noexcept : ptr_(nullptr)
    {
        moving.swap(*this);
        // In the comments it was pointed out that this
        // does not match the implementation of std::unique_ptr
        // I am going to leave mine the same. But
        // the the standard provides some extra guarantees
        // and probably a more intuitive usage.
    }
*/
    //Перемещающий конструктор наш:
    My_unique_ptr(My_unique_ptr&& moving){
        *this(std::move(moving));
    }

    ~My_unique_ptr() {
        delete ptr_;
    }

    My_unique_ptr& operator= (std::nullptr_t)
    {
        reset();
        return *this;
    }
/*
    //Перемещающий оператор= (буржуйская версия)
    My_unique_ptr& operator= (My_unique_ptr&& moving) noexcept
    {
        moving.swap(*this);
        return *this;
        // See move constructor.
    }
*/

    //Перемещающий оператор = (наш вариант)
    My_unique_ptr& operator= (My_unique_ptr&& moving) {
        delete ptr_;
        ptr_ = moving.ptr_;
        moving.ptr_ = nullptr;
        return *this;
    }

    //Константная и не константная версия operator*
    const T& operator*() const { return *ptr_;}
    T& operator*() { return *ptr_; }

    T* operator->() const { return ptr_; }
    
    explicit operator bool() const { return ptr_; }
    
    T* release() noexcept
    {
        //T* result = nullptr;
        //std::swap(result, ptr_);
        //Или так:
        T* result = ptr_;
        ptr_ = nullptr;
        return result;
    }
  
    void reset() {
        T* tmp = release();
        delete tmp;
    }

    void swap(My_unique_ptr& src) noexcept
    {
        std::swap(ptr_, src.ptr);
    }


    //Запрещаем конструктор копирования и operator=
    My_unique_ptr(const My_unique_ptr&) = delete;
    My_unique_ptr& operator=(const My_unique_ptr&) = delete;

private:
    T* ptr_{ nullptr };

};

int main()
{
    My_unique_ptr<int> my_unique_ptr (new int(100));
    std::cout << *my_unique_ptr << "\n";
    int* val2 = my_unique_ptr.release();
    std::cout << *val2 << "\n";

    My_unique_ptr<int> i(new int(200));
    std::cout << *i << "\n";
    My_unique_ptr<int> j(new int(300));
    std::cout << *j << "\n";
    i = My_unique_ptr<int> (new int(500));
    std::cout << *i << "\n";
    //i = j;                    //НЕ прокатит т.к. запрещен оператор присваивания
    i = std::move(j);     //А вто такое прокатит
    std::cout << *i << "\n";
}
