#include <iostream>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <string>

using namespace std;

/////Задание 1

int inputInt()
{
    int n{ 0 };
    cout << "Введи число:" << endl;
    do
    {
        cin >> n;
    } while (n <= 0);
    return n;
}

void fillArray(int* ptrArr, int n)
{
    for (size_t i = 0; i < n; i++)
        ptrArr[i] = pow(2, i);
}

void printArrayT1(int* ptrArr, int n)
{
    for (size_t i = 0; i < n; i++)
        cout << ptrArr[i] << " ";
    cout << endl;
}

void firstTask()
{
    int arrLenght{ 0 };
    int* ptrArr;

    cout << "Задание 1" << endl;

    arrLenght = inputInt();
    ptrArr = new int[arrLenght];
    fillArray(ptrArr, arrLenght);
    printArrayT1(ptrArr, arrLenght);

    delete[] ptrArr;
    ptrArr = nullptr;
}

/////Задание 2

void fillArray(int** ptrArr, int n)
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

void memoryFill(int** ptrArr, int SIZE)
{
    for (size_t i = 0; i < SIZE; i++)
    {
        ptrArr[i] = new int[SIZE];
    }
}


void memoryFree(int** ptrArr, int SIZE)
{
    for (size_t i = 0; i < SIZE; i++)
    {
        delete[] ptrArr[i];
    }
    delete[] ptrArr;
}

void secondTask()
{
    int** ptrArr;
    const int SIZE{ 4 };

    cout << "Задание 1" << endl;

    ptrArr = new int* [SIZE];

    memoryFill(ptrArr, SIZE);
    fillArray(ptrArr, SIZE);
    memoryFree(ptrArr, SIZE);
    ptrArr = nullptr;
}

/////Задание 3
void thirdTask(string fileName1, string fileName2)
{
    int const len{ 50 };
    char ch{ 0 };

    ofstream fout1(fileName1);
    for (size_t i = 0; i < len; i++)
    {
        ch = rand() % 25 + 65;
        fout1 << ch;
    }
    fout1.close();

    ofstream fout2(fileName2);
    for (size_t i = 0; i < len; i++)
    {
        ch = rand() % 25 + 97;
        fout2 << ch;
    }
    fout2.close();
}

/////Задание 4
void fourthTask(string fileName1, string fileName2)
{
    string fileOutName{ " " };
    
    cout << "Задание 4" << endl;
    cout << "Введи название сборного файла:" << endl;
    cin >> fileOutName;
    ifstream fin1(fileName1);
    ifstream fin2(fileName2);
    ofstream fout(fileOutName + ".txt");

    if (fin1.is_open() && fin2.is_open())
    {
        const size_t SZ{ 255 };
        char buff[SZ];
        while (!fin1.eof())
        {
            fin1.getline(buff, SZ);
            fout << buff;
        }
        fin1.close();

        while (!fin2.eof())
        {
            fin2.getline(buff, SZ);
            fout << buff;
        }
        fin2.close();
    }
    else
        cout << "не могу открыть файлы";
    fout.close();
}

/////Задание 5
void fifthTask()
{
    string fileName{ "lorem.txt" };
    string word{ " " };
    bool findCheck{ 0 };

    cout << "Задание 4" << endl;
    cout << "Какое слово найти?" << endl;
    cin >> word;

    ifstream fin(fileName);
    if (fin.is_open())
    {
        string buff;
        while (!fin.eof())
        {
            getline(fin, buff);
            if (buff.find(word) != string::npos)
            {
                findCheck = 1;
                break;
            }
        }
    }
    else
    {
        cout << "не могу открыть файлы";
    }
    fin.close();
    
    //cout << findCheck ? "Есть такое слово (y)" : "Нет такого слова (n)";
    if (findCheck)
        cout << "Есть такое слово (y)";
    else
        cout << "Нет такого слова (n)";
}

int main()
{
    srand(time(NULL));
    string fileName1{ " " };
    string fileName2{ " " };
    
    firstTask();
    secondTask();

    cout << "Задание 3" << endl;
    cout << "Введи название первого файла:" << endl;
    cin >> fileName1;
    cout << "Введи название второго файла:" << endl;
    cin >> fileName2;
    fileName1 += ".txt";
    fileName2 += ".txt";

    thirdTask(fileName1, fileName2);
    fourthTask(fileName1, fileName2);
    fifthTask();
    return 0;
}