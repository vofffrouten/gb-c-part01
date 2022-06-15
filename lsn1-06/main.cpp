#include <iostream>
#include <cstdlib>
#include <cmath>
#include <fstream>

using namespace std;

/////Задание 1

int inputInt ()
{
    int n {0};
    cout << "Введи число:" << endl;
    do
    {
        cin >> n;
    } while (n <= 0);
    return n;
}

void fillArray (int* ptrArr, int n)
{
    for (size_t i = 0; i < n; i++)
        ptrArr[i] = pow (2, i);
}

void printArrayT1 (int* ptrArr, int n)
{
    for (size_t i = 0; i < n; i++)
        cout << ptrArr[i] << " ";
    cout << endl;
}

void firstTask()
{
    int arrLenght { 0 };
    int* ptrArr;

    cout << "Задание 1" << endl;
    
    arrLenght = inputInt();
    ptrArr = new int [arrLenght];
    fillArray (ptrArr, arrLenght);
    printArrayT1(ptrArr, arrLenght);

    delete [] ptrArr;
    ptrArr = nullptr;
}

/////Задание 2

void fillArray (int** ptrArr, int n)
{
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)    
        {
            ptrArr[i][j] = rand() % 10;
            cout << ptrArr[i][j] << " ";
        }
        cout << endl;
    }        
}

void memoryFree (int** ptrArr, int SIZE)
{
    for (size_t i = 0; i < SIZE; i++)
    {
        delete [] ptrArr[i];
    }
    delete [] ptrArr;
}

void secondTask()
{
    int** ptrArr;
    const int SIZE { 4 };

    cout << "Задание 1" << endl;    
    
    ptrArr = new int* [SIZE];
    for (size_t i = 0; i < SIZE; i++)
    {
        ptrArr[i] = new int [SIZE];
    }

    fillArray(ptrArr, SIZE);
    memoryFree(ptrArr, SIZE);
    ptrArr = nullptr;
}

/////Задание 3
void thirdTask ()
{
    char ch { 0 };
    string fileName1 {" "};
    string fileName2 {" "};
    int const len { 50 };

    cout << "Задание 3" << endl; 

    cout << "Введи название первого файла:" << endl;
    cin >> fileName1;
    cout << "Введи название второго файла:" << endl;
    cin >> fileName2;
    
    ofstream fout1 (fileName1 + ".txt");
    
    for (size_t i = 0; i < len; i++)
    {   
        ch = rand () % 25 + 65;
        fout1 << ch;
    }
    

}

int main ()
{
    srand(time(NULL));
  
    firstTask();
    secondTask();
    thirdTask();

    return 0;
}