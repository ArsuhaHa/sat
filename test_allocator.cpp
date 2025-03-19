#include <iostream>
#include "Allocator.h"
#include "DataTypes.h"
#include <new>
#include <ctime>

using namespace std;

void my_out_of_memory()
{
    throw std::bad_alloc();
}

class A {
public:
    A(unsigned int size) {
        this->size = size;
        this->arr = (int *)operator new[](this->size * sizeof(int));
    }

    int * Index(unsigned int index) {
        return this->arr + index;
    }

    ~A() {
        operator delete[](arr);
    }
private:
    DECLARE_ALLOCATOR
    int *arr = nullptr;
    unsigned int size = 0;
};

IMPLEMENT_ALLOCATOR(A, 0, 0);

int main()
{
    std::set_new_handler(my_out_of_memory);
    srand(time(NULL));

    int mode = 0;
    cin >> mode;

    int len_arr = 10;

    switch (mode)
    {
    case 0:

        try
        {
            Allocator Heap(40);

            int *arr = (int *)Heap.Allocate(len_arr * sizeof(int));
            if (arr != nullptr)
            {

                for (int i = 0; i < len_arr; i++)
                {
                    arr[i] = rand() % 10;
                    std::cout << arr[i] << " ";
                }
                cout << endl;
            }
            else
            {
                std::cout << "Ne vidililas pamiti";
            }

            Heap.Deallocate(arr);

            int *arr_tmp = (int *)Heap.Allocate(len_arr * sizeof(int));
            if (arr_tmp != nullptr)
            {

                for (int i = 0; i < len_arr; i++)
                {
                    arr_tmp[i] = 10 + rand() % 10;
                    std::cout << arr_tmp[i] << " ";
                }
                cout << endl;
            }
            else
            {
                std::cout << "Ne vidililas pamiti";
            }

            Heap.Deallocate(arr_tmp);

            int *arr_tmp_error = (int *)Heap.Allocate((len_arr + 1) * sizeof(int));
            if (arr_tmp_error != nullptr)
            {

                for (int i = 0; i < 11; i++)
                {
                    arr_tmp_error[i] = 10 + rand() % 10;
                    std::cout << arr_tmp_error[i] << " ";
                }
                cout << endl;
            }
            else
            {
                std::cout << "Ne vidililas pamiti";
            }

            Heap.Deallocate(arr_tmp_error);


        }
        catch (const std::bad_alloc &e)
        {
            std::cout << "catch Heap: " << e.what() << std::endl;
            return 1;
        }
        break;

    case 1:
        try
        {
            Allocator Pool(40, 2);
            int * arr1 = (int *)Pool.Allocate(len_arr * sizeof(int));
            
            if (arr1 != nullptr) {
                for(int i = 0; i < len_arr; i++) {
                    arr1[i] = rand() % 10;
                    cout << arr1[i] << " ";
                }
                cout << endl;
            } else {
                std::cout << "Ne vidililas pamiti";
            }

            int * arr2 = (int *)Pool.Allocate((len_arr - 2) * sizeof(int));
            
            if (arr2 != nullptr) {
                for(int i = 0; i < len_arr - 2; i++) {
                    arr2[i] = rand() % 10;
                    cout << arr2[i] << " ";
                }
                cout << endl;
            } else {
                std::cout << "Ne vidililas pamiti";
            }

            int * arr_error = (int *)Pool.Allocate((len_arr - 2) * sizeof(int));
            
            if (arr_error != nullptr) {
                for(int i = 0; i < len_arr - 2; i++) {
                    arr_error[i] = rand() % 10;
                    cout << arr_error[i] << " ";
                }
                cout << endl;
            } else {
                std::cout << "Ne vidililas pamiti";
            }
        }
        catch (const std::bad_alloc &e)
        {
            std::cout << "catch: " << e.what() << std::endl;
            return 1;
        }
        break;

    case 2:
        try {
            char st[80];
            Allocator staticPool(40, 2, st);

            char * arr1 = (char *)staticPool.Allocate(len_arr * sizeof(char));
            if (arr1 != nullptr) {
                for (int i = 0; i < len_arr; i++) {
                    arr1[i] = 48 + rand() % 57;
                    std::cout << arr1[i] << " ";
                }
                std::cout << endl;
            } else {
                std::cout << "Ne vidililas pamiti";
            }

            int * arr2 = (int *)staticPool.Allocate(len_arr * sizeof(char));
            if (arr2 != nullptr) {
                for (int i = 0; i < len_arr; i++) {
                    arr2[i] = 10 + rand() % 10;
                    cout << arr2[i] << " ";
                }
                std::cout << endl;
            } else {
                std::cout << "Ne vidililas pamiti";
            }
        }
        catch (const std::bad_alloc &e)
        {
            std::cout << "catch: " << e.what() << std::endl;
            return 1;
        }
        break;

    case 3:
        try {
            unsigned int sizeArr = 1000;
            A * obj = new A(sizeArr);

            for(int i = 0; i < sizeArr; i++) {
                *(obj->Index(i)) = rand() % 10;
            }
            
            delete obj;
        }
        catch (const std::bad_alloc &e)
        {
            std::cout << "catch: " << e.what() << std::endl;
            return 1;
        }
        break;
    }

    return 0;
}