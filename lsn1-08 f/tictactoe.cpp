#include <iostream>
#include <random> 
#include <stdlib.h> 
#include <chrono> 

using namespace std;

//цвета консоли делаю через дефайн не дал сделать enum string (
#define CRESET "\e[0m" //сброс 
#define CEMPTY "\e[0;32m" //зелень - пустые клетки
#define CZERO "\e[1;33m" //желтый жирный
#define CCROSS "\e[1;34m" //синий жирный
#define CTABLE "\e[0;30m" //номера ячеек
#define CTABLEU "\e[4;30m" //номера ячеек

void clearScr()  /// очистка экрана
{
    #ifdef __linux__
         system("clear");
    #else  (_WIN64 || _WIN32)
        std::cout << "\y1B[2J\y1B[H";
    #endif
}

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

//============================

struct TCoord
{
    size_t y {0U};
    size_t x {0U};
};

struct TGame
{
    TCell** ppField { nullptr };
    size_t size { 3 };
    TProgress progress;
    TCell humanFig = CROSS;
    TCell aiFig = ZERO;
    size_t turn {0U};
};

int takeSize ()
{
    size_t size { 4U };
    return (size);
}

void initGame(TGame& g)  //заполняем структуру
{
    if(getRandomNum(0, 500) > 250) 
    {
        g.aiFig = CROSS;
        g.humanFig = ZERO;
    }

    g.size = takeSize();
    
    g.ppField = new TCell * [g.size];  
    for (size_t i = 0; i < g.size; i++)
    {
        g.ppField[i] = new TCell [g.size];
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

void deinitGame (TGame& g)  //освобождаем память
{
    for (size_t i = 0; i < g.size; i++)
    {
        delete [] g.ppField[i];
    }
    delete [] g.ppField;
    g.ppField = nullptr;
}

void printGame (TGame& g)   //печатаем поле
{
    cout << "  ";
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
                cout << CTABLE << y+1 << "|";            
            switch (g.ppField[y][x]){  //выбор цвета
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
}

void congrats (const TGame& g) // проверяем выйграл ли кто-нибудь
{
    if (g.progress == HUMAN_WINS)
        cout << "Человек выйграл!";
    else if (g.progress == AI_WINS)
        cout << "Выйграл компьютер!";
    else if (g.progress == DRAW)
        cout << "Ничья :(";
}

TProgress getWon (const TGame& g)
{
    for (size_t y = 0; y < g.size; y++)     //проверяем вертикали
    {
        for (size_t x = 1; x < g.size; x++)
        {
            if ((g.ppField[y][x] != g.ppField[y][x-1]) || 
            g.ppField[y][x] == EMPTY || g.ppField[y][x-1] == EMPTY) // еcли предыдущий символ не равен последующему, то выход
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
            if ((g.ppField[y][x] != g.ppField[y-1][x]) || 
                g.ppField[y][x] == EMPTY || g.ppField[y-1][x] == EMPTY) // еcли предыдущий символ не равен последующему, то выход
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
        if ((g.ppField[i][i] != g.ppField[i-1][i-1]) 
            || g.ppField[i][i] == EMPTY || g.ppField[i-1][i-1] == EMPTY) // еcли предыдущий символ не равен последующему, то выход
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
        size_t x {g.size - y -1};
        if ((g.ppField[y][x] != g.ppField[y-1][x+1]) 
            || g.ppField[y][x] == EMPTY || g.ppField[y-1][x+1] == EMPTY) // еcли предыдущий символ не равен последующему, то выход
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

    if (g.size*g.size == g.turn) ///проверяем ничью
        return DRAW;    
    
    return IN_PROGRESS;
}

TCoord getHumanCoord(const TGame& g) //ход человека
{
    TCoord c;
    do
    {
        cout << "Введите воординаты y и x:";
        cin >> c.x; 
        cin >> c.y;

        c.x--;
        c.y--;

    } while (c.y > g.size - 1 || c.x > g.size - 1 || g.ppField[c.y][c.x] != EMPTY);
    
    return c;
}

TCoord getAICoord( TGame& g) //ход человека
{
    TCoord cnt; //центр поля

    cnt.x = g.size / 2;
    cnt.y = cnt.x;
    // просматриваем возможные вырианты выйгрыша

    for (size_t y = 0; y < g.size; y++)         
    {
        for (size_t x = 0; x < g.size; x++)
        {    
            if (g.ppField [y][x] == EMPTY)
            {
                g.ppField [y][x] = g.aiFig;
                if (getWon(g) == AI_WINS)
                {
                    g.ppField [y][x] = EMPTY;
                    return {y, x};
                }
                else 
                {
                    g.ppField [y][x] = EMPTY;
                }

                g.ppField [y][x] = g.humanFig;
                if (getWon(g) == HUMAN_WINS)
                {
                    g.ppField [y][x] = EMPTY;
                    return {y, x};
                }
                else 
                {
                    g.ppField [y][x] = EMPTY;
                }
            }
        }
    }

    // приоритеты
    if (g.ppField[cnt.y][cnt.x] == EMPTY)       
    {
        return cnt;
    }

    TCoord buffer [g.size * g.size];
    size_t counter { 0 };
    
    // просматриваем диагонали
    for (size_t y = 0; y < g.size; y++)         
    {
        for (size_t x = 0; x < g.size; x++)
        {
            if (x == cnt.x && y == cnt.y)
            {
                continue;
            }    
            if (g.ppField [y][x] == EMPTY)
            {
                buffer [counter] = {y, x};
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
            if (g.ppField [y][x] == EMPTY)
            {
                buffer [counter] = {y, x};
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

void humanTurn (TGame& g)  ///ход человека
{
    const TCoord c = getHumanCoord(g);
    g.ppField[c.y][c.x] = g.humanFig;
}

void AITurn (TGame& g)     ///ход ИИ
{
    const TCoord c = getAICoord(g);
    g.ppField[c.y][c.x] = g.aiFig;
}

void newGame()
{
    TGame g; 
    
    initGame(g);
    printGame(g);
    bool turnCheck { g.humanFig == CROSS ? true : false };
    do
    {
        if (turnCheck) 
        {
            humanTurn (g);
        }
        else
        {
            AITurn (g);
        }

        clearScr();
        printGame(g);        
        g.progress = getWon(g);
        g.turn++;
        turnCheck = !turnCheck;

    } while (g.progress == IN_PROGRESS);
    congrats(g);
    deinitGame(g);
}

int main()
{
    newGame();
    return 0;
}