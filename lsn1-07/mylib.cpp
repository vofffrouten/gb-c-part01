#include <iostream>

namespace MyLib
{
    void initArray (float* array, const int size)
    {
        float randLimit { 4.0 };
        float randStep { 50 };
        int randSize = randLimit * randStep * 2;

        for (size_t i = 0; i < size; i++)
        {
            array[i] = (rand() % randSize) / randStep - randLimit;            
        }
    }

    void printArray (const float* array, const int size)
    {
        for (size_t i = 0; i < size; i++)
        {
            std::cout << array[i] << " ";
        }
        std::cout << std::endl;
    }

    void searchArray (const float* array, const int size)
    {
        int positiveCount { 0 };
        int negativeCount { 0 };
        int zeroCount { 0 };
        for (size_t i = 0; i < size; i++)
        {
            if (array[i] > 0)
                positiveCount++;
            else if (array[i] < 0)
                negativeCount++;  
        }
        zeroCount = size - positiveCount - negativeCount;
        std::cout << "Number of Positive = " << positiveCount << std::endl;
        std::cout << "Number of Negotive = " << negativeCount << std::endl;
        std::cout << "Number of Zero = " << zeroCount << std::endl;
    }
}
