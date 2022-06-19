#include <iostream>
#include <fstream>
#include <string>
#include "mylib.h"

#define NUM_CHECK (num) std::cout << ()

/*////// Задание 1
void firstTask()
{
    const int sizeTask1 { 8 };
    float arrayTask1 [sizeTask1]; 
    std::cout << "Задание 1" << std::endl;
    
    MyLib::initArray (arrayTask1, sizeTask1);
    MyLib::printArray (arrayTask1, sizeTask1);
    MyLib::searchArray (arrayTask1, sizeTask1);    
}*/

///// Задание 2 
/*
#define tCheck (n, l)

int getNum ()
{
    while (true)
    {
        int num { 0 };
        std::cin >> num;

        if (std::cin.fail())
        {
            std::cout << "Число не целое, либо меньше ноля" << std::endl;
            std::cin.clear();
            std::cin.ignore(32767, '\n');
        }
        else
        {
            return num;
        }
    }
}

void secondTask()
{
    unsigned int inputNum { 0 };
    unsigned int inputLimit { 0 };
    std::string boolStr;
    
    std::cout << "Задание 2" << std::endl;
    std::cout << "Введите целое число >= 0"  << std::endl;
    inputNum  = getNum();
    std::cout << "Введите лимит > 0"  << std::endl;
    inputLimit =  getNum();
    boolStr = tCheck (inputNum, inputLimit);
    // boolStr = ((inputNum >= 0) && (inputNum < inputLimit)) ? "TRUE" : "FALSE";
    std::cout << boolStr << std::endl;
}
*/

///// Задание 3
#define ARRAY_SIZE 10
#define SwapINT(a, b) a += b;\
b = a - b;\
a = a - b;

void printArray (const int* array, const int size)
{
    for (size_t i = 0; i < size; i++)
    {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}

void thirdTask ()
{
    int array [ARRAY_SIZE] { 0 };
    std::cout << "Задание 3" << std::endl;
    for (size_t i = 0; i < ARRAY_SIZE; i++)
    {
        std::cin >> array[i];
    }
    for (size_t i = 0; i < ARRAY_SIZE; i++)
    {
        for (size_t j = 0; j < ARRAY_SIZE - 1 - i; j++)
        {
            if (array[j] > array [j+1])
            {
                SwapINT(array[j], array[j+1]);
            }
        }
    }
    printArray(array, ARRAY_SIZE);
}

///// Задание 4
#pragma pack(push, 1)

struct Employee
{
    unsigned short ID;
    unsigned short age;
    char sex;
    unsigned short exper;
};
#pragma pack(pop)

void printStruct(Employee* empl)
{
    std::cout << "id: " << empl -> ID << std::endl; 
    std::cout << "age: " << empl -> age << std::endl;
    std::cout << "sex: " << empl -> sex << std::endl;
    std::cout << "experience: " << empl -> exper << std::endl;
}

void fileOutStruct(Employee* empl, std::string fName = "out.txt")
{
    std::ofstream fout(fName);
    
    fout << "id: " << empl -> ID << std::endl; 
    fout << "age: " << empl -> age << std::endl;
    fout << "sex: " << empl -> sex << std::endl;
    fout << "experience: " << empl -> exper << std::endl;

    fout.close();
} 

void fourthTask ()
{
    std::string outFileName { "struct.txt" };
    Employee* empl1 = new Employee; 
    empl1 -> ID = 005446;
    empl1 -> age = 21;
    empl1 -> sex  = 'M';
    empl1 -> exper = 4;

    printStruct(empl1);
    std::cout << "Размер: "<< sizeof (empl1) << "byte" << std::endl; 
    // при динамическом выделении прагма не сработала: 8 байт
    // при обычном объявлении прагма работает: 7 байт

    fileOutStruct(empl1, outFileName);    
    delete empl1;
}

int main ()
{
    srand(time(NULL));
    //firstTask();
    //secondTask();
    thirdTask();
    fourthTask();
    
    return 0;
}