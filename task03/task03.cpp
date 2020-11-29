#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <queue>

using namespace std;

const int stockSize = 10;
queue<int> stock;
sem_t stock_items;

const int lorrySize = stockSize;
queue<int> lorry;
sem_t lorry_items;

unsigned long long sum = 0;

void *Ivanov(void* param)
{
    for (int i = 0; i < stockSize; ++i)
    {
        int temp = rand();
        stock.push(temp);
        sem_post(&stock_items);

        printf("Ivanov took box #%d from the stock.\n", temp);
    }
    return nullptr;
}

void *Petrov(void* param)
{
    for (int i = 0; i < stockSize; ++i)
    {
        sem_wait(&stock_items);
        int temp = stock.front();
        stock.pop();

        lorry.push(temp);
        sem_post(&lorry_items);

        printf("Petrov put box #%d in the lorry.\n", temp);
    }
    return nullptr;
}

void *Necheporchuk(void *param)
{
    for (int i = 0; i < lorrySize; ++i)
    {
        sem_wait(&lorry_items);
        int temp = lorry.front();
        lorry.pop();

        sum += temp;

        printf("Necheporchuk calculated box #%d and got sum %d.\n", temp, sum);
    }
    return nullptr;
}



int main()
{
    stock = queue<int>();
    lorry = queue<int>();

    srand((unsigned)time(NULL));

    sem_init(&stock_items, 0, 0);
    sem_init(&lorry_items, 0, 0);

    pthread_t ivanov;
    pthread_create(&ivanov, NULL, Ivanov, NULL);
    pthread_t petrov;
    pthread_create(&petrov, NULL, Petrov, NULL);
    pthread_t necheporchuk;
    pthread_create(&necheporchuk, NULL, Necheporchuk, NULL);

    pthread_join(ivanov, NULL);
    pthread_join(petrov, NULL);
    pthread_join(necheporchuk, NULL);

    printf("Total sum of loot is equal to %d", sum);

    return 0;
}
