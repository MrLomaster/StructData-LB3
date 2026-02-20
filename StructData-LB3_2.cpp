#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

const int K = 8; // Используются степени двойки (1, 2, 4, 8, 16 итд). Является размером квадрата

struct Node
{
    int data;
    Node* link;
    int x, y;
};
class List
{
    Node* h;
    int K;

    Node* find(int x, int y)
    {
        Node* current = h;
        while (current)
        {
            if (current->x == x && current->y == y) return current;
            current = current->link;
        }
        return NULL;
    }



public:  
    void pushBack(int x, int y, int val)
    {
        Node* n = new Node();
        n->data = val;
        n->link = NULL;
        n->x = x;
        n->y = y;

        if (!h)
        {
            h = n;
            return;
        }

        Node* c = h;
        while (c->link)
            c = c->link;
        c->link = n;
    }

    List(int s) : K(s), h(NULL)
    {
        for (int i = 0; i < K; ++i)
            for (int j = 0; j < K; ++j)
                pushBack(i, j, 0);
    }

    int& at(int x, int y)
    {
        static int bbuf;

        Node* n = find(x, y);
        if (n) return n->data;
        else return bbuf;
    }

    ~List()
    {
        while (h)
        {
            Node* n = h;
            h = h->link;
            delete n;
        }
    }
};

void divMas(int mas, int* c, int s)
{
    /*
        функция работает по принципу того, что мы, условно, знаем количество чисел и их значения, которые находятся под mas, 
        благодаря размерности текущего "массива" (s) и арифметической прогрессии +1. Так же известно, что если число нечётно, 
        то порядок чисел в этой клетке прямой и напротив, если число чётно, то порядок в этой клетке обратный и уже отталкиваясь
        от порядка можно создать нужное число в клетке.

        mas - число, от которого считаем
        *c - адрес в массиве для записи числа
        s - размер "делимого" массива
    */
    if (mas % 2 != 0) // из прямого в обратный
    {
        *c = mas - 1 + s;
    }
    else if (mas % 2 == 0) // из обратного в прямой
    {
        *c = mas - s + 1;
    }
}

void divMasBig(int mas[K][K], int x11, int y11, int x12, int y12, int x21, int y21, int s, int mode)
{
    /*
        функция работает по принципу "отзеркаленного" копирования, т.е. мы не тупо копируем элементы из клеток в клетки,
        находящиеся на n-ом расстоянии, а делаем так, что дальние клетки копируются в дальние, а ближние клетки в ближние

        mas[][][] - основной массив
        x11 - позиция начала развёрнутого массива по X
        y11 - позиция начала развёрнутого массива по Y
        x12 - позиция окончания развёрнутого массива по X
        y12 - позиция окончания развёрнутого массива по Y
        x21 - позиция начала новой части массива по X
        y21 - позиция начала новой части массива по Y
        s - размер "делимого" массива
        mode - режим работы. При 1 - развёртка сверху-вниз, при 2 - развёртка слева-направо
    */

    if (mode == 1)
    {
        for (int i = 0; i <= x12 - x11; ++i)
            for (int j = 0; j <= y12 - y11; ++j)
                divMas(mas[i][j], &mas[x21 - i][y21 + j], s);
    }
    else
    {
        for (int i = 0; i <= x12 - x11; ++i)
            for (int j = 0; j <= y12 - y11; ++j)
                divMas(mas[i][j], &mas[x21 + i][y21 - j], s);
    }
}
void divMasBig(List* mas, int x11, int y11, int x12, int y12, int x21, int y21, int s, int mode)
{
    if (mode == 1)
    {
        for (int i = 0; i <= x12 - x11; ++i)
            for (int j = 0; j <= y12 - y11; ++j)
                divMas(mas->at(i, j), &mas->at(x21 - i, y21 + j), s);
    }
    else
    {
        for (int i = 0; i <= x12 - x11; ++i)
            for (int j = 0; j <= y12 - y11; ++j)
                divMas(mas->at(i, j), &mas->at(x21 + i, y21 - j), s);
    }
}
void divMasBig(vector<vector<int>>& mas, int x11, int y11, int x12, int y12, int x21, int y21, int s, int mode)
{
    if (mode == 1)
    {
        for (int i = 0; i <= x12 - x11; ++i)
            for (int j = 0; j <= y12 - y11; ++j)
                divMas(mas[i][j], &mas[x21 - i][y21 + j], s);
    }
    else
    {
        for (int i = 0; i <= x12 - x11; ++i)
            for (int j = 0; j <= y12 - y11; ++j)
                divMas(mas[i][j], &mas[x21 + i][y21 - j], s);
    }
}

void f(int mas[K][K])
{
    /*
        mas[K][K][K * K] - Основной массив
      ================================================================================================================
        size - количество чисел, которые "скрываются" под mas[i][k][0] (на самом деле их не существует,
               но со знанием их количества и +1 прогрессии, мы можем это рассчитать, поэтому size так важен).
               После каждой итерации количество чисел в каждой клетке уменьшается в 4 раза.

        gsize - текущая размерность развёрнутого поля. Изначально равен 1 (есть только поле 00)
                но после каждой итерации увеличивается по геометрической прогрессии *2
    */
    int size = K * K;

    int gsize = 1;
    while (size != 1)
    {
        divMasBig(mas, 0, 0, gsize - 1, gsize - 1, gsize * 2 - 1, 0, size, 1);
        divMasBig(mas, 0, 0, gsize * 2 - 1, gsize - 1, 0, gsize * 2 - 1, size / 2, 2);

        gsize = gsize * 2;
        size /= 4;
    }
}
void f(List* mas)
{
    int size = K * K;

    int gsize = 1;
    while (size != 1)
    {
        divMasBig(mas, 0, 0, gsize - 1, gsize - 1, gsize * 2 - 1, 0, size, 1);
        divMasBig(mas, 0, 0, gsize * 2 - 1, gsize - 1, 0, gsize * 2 - 1, size / 2, 2);

        gsize = gsize * 2;
        size /= 4;
    }
}
void f(vector<vector<int>>& mas)
{
    int size = K * K;

    int gsize = 1;
    while (size != 1)
    {
        divMasBig(mas, 0, 0, gsize - 1, gsize - 1, gsize * 2 - 1, 0, size, 1);
        divMasBig(mas, 0, 0, gsize * 2 - 1, gsize - 1, 0, gsize * 2 - 1, size / 2, 2);

        gsize = gsize * 2;
        size /= 4;
    }
}

void outputMas(int a[K][K])
{
    for (int i = 0; i < K; ++i)
    {
        cout << endl;
        for (int j = 0; j < K; ++j)
        {
            cout << " ";
            printf("%4d ", a[i][j]);
        }
    }
}
void outputMas(List* a)
{
    for (int i = 0; i < K; ++i)
    {
        cout << endl;
        for (int j = 0; j < K; ++j)
        {
            cout << " ";
            printf("%4d ", a->at(i, j));
        }
    }
}
void outputMas(vector<vector<int>>& a)
{
    for (int i = 0; i < K; ++i)
    {
        cout << endl;
        for (int j = 0; j < K; ++j)
        {
            cout << " ";
            printf("%4d ", a[i][j]);
        }
    }
}

void initMas(int mas[K][K])
{
    mas[0][0] = 1;
}
void initMas(List* mas)
{
    mas->at(0, 0) = 1;
}
void initMas(vector<vector<int>>& mas)
{
    mas[0][0] = 1;
}

int main()
{
    cout << "Created by Kalinin Timofei Nikolaevich\n" << "Code of Group: 090304-RPIa-o25\n\n";

    List* ls = new List(K);
    int res[K][K];
    vector<vector<int>> vec(K, vector<int>(K, 0));
    
    auto start = chrono::high_resolution_clock::now();
    initMas(res);
    f(res);
    auto end = chrono::high_resolution_clock::now();
    auto time_res = chrono::duration_cast<chrono::microseconds>(end - start).count();
    cout << "\nMassive worked for " << time_res << " mcs";

    start = chrono::high_resolution_clock::now();
    initMas(ls);
    f(ls);
    end = chrono::high_resolution_clock::now();
    auto time_ls = chrono::duration_cast<chrono::microseconds>(end - start).count();
    cout << "\nLink list worked for " << time_ls << " mcs";

    start = chrono::high_resolution_clock::now();
    initMas(vec);
    f(vec);
    end = chrono::high_resolution_clock::now();
    auto time_vec = chrono::duration_cast<chrono::microseconds>(end - start).count();
    cout << "\nVector worked for " << time_vec << " mcs";

    cout << "\n\nMassive:";
    outputMas(res);
    cout << "\nLink List:";
    outputMas(ls);
    cout << "\nVector:";
    outputMas(vec);
}