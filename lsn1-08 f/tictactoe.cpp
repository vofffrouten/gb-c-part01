#include <iostream>
#include <random> 
#include <stdlib.h> 
#include <chrono>
#include <fstream>

using namespace std;

//=====================================


#define SCORES "scores" // имя файля со счетом

//цвета консоли делаю через дефайн не дал сделать enum string (
#ifdef __linux__
#define CRESET "\e[0m" //сброс 
#define CEMPTY "\e[0;32m" //зелень - пустые клетки
#define CZERO "\e[1;33m" //желтый жирный
#define CCROSS "\e[1;34m" //синий жирный
#define CTABLE "\e[0;30m" //номера ячеек
#define CTABLEU "\e[4;30m" //номера ячеек underline
#elif  _WIN64               
#define CRESET "\x1B[0m" //сброс 
#define CEMPTY "\x1B[0;32m" //зелень - пустые клетки
#define CZERO "\x1B[1;33m" //желтый жирный
#define CCROSS "\x1B[1;36m" //синий жирный
#define CTABLE "\x1B[0;32m" //номера ячеек
#define CTABLEU "\x1B[4;32m" //номера ячеек underline   
#endif

void locate()
{
#ifdef _WIN64
    setlocale(LC_ALL, "Russian");
#endif
}

void clearScr()  /// очистка экрана
{
#ifdef __linux__
    system("clear");
#elif  (_WIN64 || _WIN32) 
    system("cls");
    //    std::cout << "\y1B[2J\y1B[H";
#endif
}

//=====================================

int32_t getRandomNum(int32_t min, int32_t may)
{
    const static auto seed = chrono::system_clock::now().time_since_epoch().count();
    static mt19937_64 generator(seed);
    uniform_int_distribution<int32_t> dis(min, may);
    return dis(generator);
}

enum TCell : char
{
    CROSS = 'X',
    ZERO = '0',
    EMPTY = '-'
};


enum TProgress { HUMAN_WINS, AI_WINS, DRAW, IN_PROGRESS };

struct TCoord
{
    size_t y{ 0U };
    size_t x{ 0U };
};

struct TGame
{
    TCell** ppField{ nullptr };
    size_t size{ 3 };
    TProgress progress;
    TCell humanFig = CROSS;
    TCell aiFig = ZERO;
    size_t turn{ 0U };
};

//=====================================

size_t takeSize() //запрос размера поля
{
    size_t size{ 3U };
    clearScr();
    cout << "Введите размер стороны поля (больше 2 но меньше 10): ";
    do
    {
        cin >> size;
    } while (size <= 2 || size > 10);
    clearScr();
    return (size);
}

void initGame(TGame& g)  //заполняем структуру
{
    if (getRandomNum(0, 500) > 250)
    {
        g.aiFig = CROSS;
        g.humanFig = ZERO;
    }

    g.size = takeSize();

    g.ppField = new TCell * [g.size];
    for (size_t i = 0; i < g.size; i++)
    {
        g.ppField[i] = new TCell[g.size];
    }

    for (size_t y = 0; y < g.size; y++)
    {
        for (size_t x = 0; x < g.size; x++)
        {
            g.ppField[y][x] = EMPTY;
        }
    }

    g.progress = { IN_PROGRESS };
}

void deinitGame(TGame& g)  //освобождаем память
{
    for (size_t i = 0; i < g.size; i++)
    {
        delete[] g.ppField[i];
    }
    delete[] g.ppField;
    g.ppField = nullptr;
}

void printGame(TGame& g)   //печатаем поле
{
    cout << "     ";
    for (size_t y = 1; y <= g.size; y++) //первая строка
    {
        cout << CTABLEU << y << " ";
    }
    cout << endl;

    for (size_t y = 0; y < g.size; y++)
    {
        for (size_t x = 0; x < g.size; x++)
        {
            if (x == 0)
                cout << CTABLE << "   " << y + 1 << "|";
            switch (g.ppField[y][x]) {  //выбор цвета
            case CROSS:
                cout << CCROSS;
                break;
            case ZERO:
                cout << CZERO;
                break;
            case EMPTY:
                cout << CEMPTY;
                break;
            default:
                break;
            }
            cout << g.ppField[y][x] << " ";
            cout << CRESET;             //reset color
        }
        cout << endl;
    }
    cout << endl;
}

void waitForEnter() // ожидание любого ввода перед обнулением экрана
{
    cout << endl << "введите любое число для продолжения "; // system() не хочет фризить
    string a{ "1" };
    cin >> a;
    clearScr();
}

void writeScoreInFile(const TGame& g)    // записать счет в файл со счетом
{
    const size_t size{ 3 };
    int buf[size] = { 0 };
    fstream fin(SCORES);   //открываем файл с очками для чтения

    if (fin.is_open())
    {
        for (size_t i = 0; i < size; i++)
        {
            fin >> buf[i]; // читаем из файла в массив
        }
        fin.close();
    }

    if (g.progress == HUMAN_WINS)       //добавляем в зависимости от прогресса
        buf[0] ++;
    else if (g.progress == AI_WINS)
        buf[1] ++;
    else if (g.progress == IN_PROGRESS)
        buf[2] ++;

    ofstream fout(SCORES);   //открываем файл с очками для записи

    if (fout.is_open())
    {
        for (size_t i = 0; i < size; i++)
        {
            fout << buf[i] << " "; // записываем массив
        }
        fin.close();
    }
}

void congrats(const TGame& g) // проверяем выйграл ли кто-нибудь
{
    clearScr();
    cout << endl << endl << "=====================" << endl;

    if (g.progress == HUMAN_WINS)
        cout << CCROSS << "-- Человек выйграл!--" << endl << CRESET;
    else if (g.progress == AI_WINS)
        cout << CCROSS << "- Выйграл компьютер!-" << endl << CRESET;
    else if (g.progress == DRAW)
        cout << CCROSS << "------ Ничья :( -----" << endl << CRESET;
    cout << "=====================" << endl;

    writeScoreInFile(g);
    waitForEnter();
}

TProgress getWon(const TGame& g)  //Выйграл ли кто-то
{
    for (size_t y = 0; y < g.size; y++)     //проверяем вертикали
    {
        for (size_t x = 1; x < g.size; x++)
        {
            if ((g.ppField[y][x] != g.ppField[y][x - 1]) ||
                g.ppField[y][x] == EMPTY || g.ppField[y][x - 1] == EMPTY) // еcли предыдущий символ не равен последующему, то выход
            {
                break;
            }
            if (x == g.size - 1)    // если цикл отработал до конца и не вышел
            {
                if (g.ppField[y][x] == g.aiFig)
                {
                    return AI_WINS;
                }
                else if (g.ppField[y][x] == g.humanFig)
                {
                    return HUMAN_WINS;
                }
            }
        }
    }

    for (size_t x = 0; x < g.size; x++)     //проверяем горизонтали
    {
        for (size_t y = 1; y < g.size; y++)
        {
            if ((g.ppField[y][x] != g.ppField[y - 1][x]) ||
                g.ppField[y][x] == EMPTY || g.ppField[y - 1][x] == EMPTY) // еcли предыдущий символ не равен последующему, то выход
            {
                break;
            }
            if (y == g.size - 1)    // если цикл отработал до конца и не вышел
            {
                if (g.ppField[y][x] == g.aiFig)
                {
                    return AI_WINS;
                }
                else if (g.ppField[y][x] == g.humanFig)
                {
                    return HUMAN_WINS;
                }
            }
        }
    }

    for (size_t i = 1; i < g.size; i++)  // проверяем диагональ справа на лево
    {
        if ((g.ppField[i][i] != g.ppField[i - 1][i - 1])
            || g.ppField[i][i] == EMPTY || g.ppField[i - 1][i - 1] == EMPTY) // еcли предыдущий символ не равен последующему, то выход
        {
            break;
        }
        if (i == g.size - 1)    // если цикл отработал до конца и не вышел
        {
            if (g.ppField[i][i] == g.aiFig)
            {
                return AI_WINS;
            }
            else if (g.ppField[i][i] == g.humanFig)
            {
                return HUMAN_WINS;
            }
        }
    }

    for (size_t y = g.size - 1; y > 0; y--)  // проверяем диагональ справа налево 
    {
        size_t x{ g.size - y - 1 };
        if ((g.ppField[y][x] != g.ppField[y - 1][x + 1])
            || g.ppField[y][x] == EMPTY || g.ppField[y - 1][x + 1] == EMPTY) // еcли предыдущий символ не равен последующему, то выход
        {
            break;
        }
        if (y - 1 == 0)    // если цикл отработал до конца и не вышел
        {
            if (g.ppField[y][x] == g.aiFig)
            {
                return AI_WINS;
            }
            else if (g.ppField[y][x] == g.humanFig)
            {
                return HUMAN_WINS;
            }
        }
    }
    
    if (g.turn == g.size * g.size)
        return DRAW;

    return IN_PROGRESS;
}

TCoord getHumanCoord(const TGame& g) //ход человека
{
    TCoord c;
    do
    {
        cout << "Введите координаты X и Y:";
        cin >> c.x;
        cin >> c.y;

        c.x--;
        c.y--;

    } while (c.y > g.size - 1 || c.x > g.size - 1 || g.ppField[c.y][c.x] != EMPTY);

    return c;
}

TCoord getAICoord(TGame& g)    //ход ИИ
{
    TCoord cnt; //центр поля (для четных ячеек сомнительно нужен рандом центра)

    cnt.x = g.size / 2;
    cnt.y = cnt.x;
    // просматриваем возможные вырианты выйгрыша

    for (size_t y = 0; y < g.size; y++)
    {
        for (size_t x = 0; x < g.size; x++)
        {
            if (g.ppField[y][x] == EMPTY)
            {
                g.ppField[y][x] = g.aiFig;
                if (getWon(g) == AI_WINS)
                {
                    g.ppField[y][x] = EMPTY;
                    return { y, x };
                }
                else
                {
                    g.ppField[y][x] = EMPTY;
                }

                g.ppField[y][x] = g.humanFig;
                if (getWon(g) == HUMAN_WINS)
                {
                    g.ppField[y][x] = EMPTY;
                    return { y, x };
                }
                else
                {
                    g.ppField[y][x] = EMPTY;
                }
            }
        }
    }

    // приоритеты
    if (g.ppField[cnt.y][cnt.x] == EMPTY)
    {
        return cnt;
    }

    const size_t sz { 100 };
    
    TCoord buffer[sz];
    size_t counter{ 0 };

    // просматриваем диагонали
    for (size_t y = 0; y < g.size; y++)
    {
        for (size_t x = 0; x < g.size; x++)
        {
            if (x == cnt.x && y == cnt.y)
            {
                continue;
            }
            if (g.ppField[y][x] == EMPTY)
            {
                buffer[counter] = { y, x };
                counter++;
            }
        }
    }
    if (counter > 0)
    {
        size_t ind = getRandomNum(0, 1000) % counter;
        return (buffer[ind]);
    }

    // просматриваем все кроме диагоналей
    counter = 0U;

    for (size_t y = 0; y < g.size; y++)
    {
        for (size_t x = 0; x < g.size; x++)
        {
            if (x == y || y == g.size - x)
                continue;
            if (g.ppField[y][x] == EMPTY)
            {
                buffer[counter] = { y, x };
                counter++;
            }
        }
    }
    if (counter > 0)
    {
        size_t ind = getRandomNum(0, 1000) % counter;
        return (buffer[ind]);
    }
}

void humanTurn(TGame& g)  ///ход человека
{
    const TCoord c = getHumanCoord(g);
    g.ppField[c.y][c.x] = g.humanFig;
}

void AITurn(TGame& g)     ///ход ИИ
{
    const TCoord c = getAICoord(g);
    g.ppField[c.y][c.x] = g.aiFig;
}

void newGame()          //тело игры
{
    TGame g;

    initGame(g);
    printGame(g);
    bool turnCheck{ g.humanFig == CROSS ? true : false };
    do
    {
        if (turnCheck)
        {
            humanTurn(g);
        }
        else
        {
            AITurn(g);
        }
        clearScr();
        printGame(g);
        g.turn++;
        g.progress = getWon(g);
        turnCheck = !turnCheck;

    } while (g.progress == IN_PROGRESS);
    congrats(g);
    deinitGame(g);
}

void startScreen()  // отрисовывает стартовый экран
{
    #ifdef __linux__
    cout << CCROSS 
        << "   ───╔═╗╔═╗───╔═══╗────" << endl
        << "   ───╚╗╚╝╔╝───║╔═╗║────" << endl
        << "   ────╚╗╔╝────║║─║║────" << endl
        << "   ────╔╝╚╗────║║─║║────" << endl
        << "   ───╔╝╔╗╚╗───║╚═╝║────" << endl
        << "   ───╚═╝╚═╝───╚═══╝────" << endl << endl;
    #elif _WIN64
    cout << CCROSS
        << "   ---ХХ--ХХ--000000----" << endl
        << "   ----ХХХХ---00--00----" << endl
        << "   -----ХХ----00--00----" << endl
        << "   -----ХХ----00--00----" << endl
        << "   ----ХХХХ---00--00----" << endl
        << "   ---ХХ--ХХ--000000----" << endl << endl;
    #endif
    cout << CEMPTY << "введите 1 чтобы начать игру" << endl;;
    cout << CEMPTY << " введите 2 посмотреть счёт" << endl;;
    cout << CEMPTY << "   введите 3 чтобы выйти" << endl;;
    cout << CRESET << endl;
}

void showScore()   //показать таблицу очков
{
    const size_t size{ 3 };
    const string str[size] = { "победы человека ----- ",
                               "победы компьютера --- ",
                               "ничьи --------------- " };

    clearScr();

    cout << CCROSS << "======================" << endl //шапка
        << "-------- СЧЁТ --------" << endl
        << "======================" << endl << endl;

    cout << CRESET;

    ifstream fin(SCORES);   //открываем файл с очками
    if (fin.is_open())
    {
        int buf[size];
        for (size_t i = 0; i < size; i++)
        {
            fin >> buf[i];
            cout << CEMPTY << str[i] << buf[i] << endl;; // печать таблицы счета
        }
        fin.close();
    }
    else
    {
        cout << CEMPTY << " компьютер и человек" << endl
            << "еще не сражались здесь!" << endl;
    }
    cout << CRESET;
    waitForEnter();
}

int main()
{
    locate();
    size_t menuChoice{ 0U };
    do
    {
        startScreen();
        cin >> menuChoice;
        if (menuChoice == 1)
            newGame();
        else if (menuChoice == 2)
            showScore();
    } while (menuChoice != 3);

    cout << endl << endl;
    return 0;
}
