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
    
    My_unique_ptr(My_unique_ptr&& moving) noexcept : ptr_(nullptr)
    {
        moving.swap(*this);
        // In the comments it was pointed out that this
        // does not match the implementation of std::unique_ptr
        // I am going to leave mine the same. But
        // the the standard provides some extra guarantees
        // and probably a more intuitive usage.
    }
    
    ~My_unique_ptr() {
        delete ptr_;
    }

    My_unique_ptr& operator= (std::nullptr_t)
    {
        reset();
        return *this;
    }

    My_unique_ptr& operator= (My_unique_ptr&& moving) noexcept
    {
        moving.swap(*this);
        return *this;
        // See move constructor.
    }

    T& operator*() const { return *ptr_;}
    T* operator->() const { return ptr_; }
    explicit operator bool() const { return ptr_; }
    
    T* release() noexcept
    {
        T* result = nullptr;
        std::swap(result, ptr_);
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
}
