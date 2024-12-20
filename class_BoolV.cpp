#include <cstddef>
#include <iostream>
#include <istream>
#include <ostream>
#include <string.h>
#include <string>
#include <fstream>

#include <vector>
using namespace std;


class BoolVec {
private:
    unsigned int* vec; // Булев вектор
    size_t bits;          // Кол-во бит вектора
    size_t sizeVector;    // Кол-во эл-тов массива vector
public:
    BoolVec(size_t n = 1);
    BoolVec(const char*);
    BoolVec(const BoolVec&);
    ~BoolVec();

    void Set1(size_t);
    void Set0(size_t);
    string ConvertVecToStr() const;
    BoolVec& operator= (const BoolVec&);
    int operator[](size_t);

    bool operator== (const BoolVec&);
    BoolVec operator| (const BoolVec&);
    BoolVec operator& (const BoolVec&);
    BoolVec operator~ ();
    int weight();

    friend ostream& operator<< (ostream&, const BoolVec&);
    friend istream& operator>> (istream&, BoolVec&);
};


string BoolVec::ConvertVecToStr() const 
{
    string stringVec = "";
    unsigned int mask = 1;

    if (vec == NULL || bits < 0)
        return stringVec;

    for (int i = 0; i < bits; i++) {
        mask = 1;
        if (vec[i / 32] & (mask << (i % 32)))
            stringVec += '1';
        else
            stringVec += '0';
    }

    return stringVec;
}


BoolVec::BoolVec(size_t n) 
{
    if (!n) 
    {
        bits = 0;
        sizeVector = 0;
        vec = NULL;
        return;
    }

    int bytes = ((n - 1) / 8) + 1;
    bits = n;
    sizeVector = ((bytes - 1) / 4) + 1;
    vec = new unsigned int[sizeVector];
    for (int i = 0; i < sizeVector; i++)
        vec[i] = 0;
}


BoolVec::BoolVec(const char* str) : BoolVec::BoolVec(strlen(str)) 
{
    unsigned int mask = 1;

    for (int i = 0; i < bits; i++) 
    {
        if (str[i] != '0')
            vec[i / 32] = vec[i / 32] | mask;

        mask <<= 1;

        if (mask == 0)
            mask = 1;
    }
}


BoolVec::BoolVec(const BoolVec& obj) : BoolVec::BoolVec(obj.bits) 
{
    for (int i = 0; i < obj.sizeVector; i++)
        vec[i] = obj.vec[i];
}


BoolVec::~BoolVec() 
{
    if (vec != NULL)
        delete[] vec;

    bits = 0;
    sizeVector = 0;
}


BoolVec& BoolVec::operator= (const BoolVec& obj) 
{
    if (this == &obj)
        return *this;

    delete[] vec;

    vec = new unsigned int[obj.sizeVector];
    bits = obj.bits;
    sizeVector = obj.sizeVector;

    for (int i = 0; i < sizeVector; i++)
        vec[i] = obj.vec[i];

    return *this;
}

int BoolVec::operator[](size_t index) 
{
    if (index >= bits) 
    {
        cout << "Error []: index out of range!\n";
        exit(1);
    }


    unsigned int mask = 1;
    mask <<= (index % 32);

    int r = bool(vec[index / 32] & mask);
    
    return r;
}


bool BoolVec::operator== (const BoolVec& obj) 
{
    if (obj.bits != bits)
        return false;

    for (int i = 0; i < sizeVector; i++) 
    {
        if (vec[i] != obj.vec[i])
            return false;
    }

    return true;
}


void BoolVec::Set1(size_t bit) 
{
    if (vec == NULL || bits < 0 || bits < bit)
        return;

    unsigned int mask = 1;
    int byte = bit / 32;
    int shiftBits = bit % 32;

    mask = mask << shiftBits;
    vec[byte] = vec[byte] | mask;
}


void BoolVec::Set0(size_t bit) 
{
    if (vec == NULL || bits < 0 || bits < bit)
        return;

    unsigned int mask = 1;
    int byte = bit / 32;
    int shiftBits = bit % 32;

    mask = mask << shiftBits;
    vec[byte] = vec[byte] | mask;
}


int BoolVec::weight() 
{
    int weight = 0;
    for (size_t i = 0; i < bits; i++) 
    {
        unsigned int mask = 1 << (i % 32);
        if (vec[i / 32] & mask) {
            weight++;
        }
    }
    return weight;
}

BoolVec BoolVec::operator~ () 
{
    BoolVec result(bits);

    unsigned int mask = 0xffffffff;

    for (size_t i = 0; i < sizeVector; i++) {
        result.vec[i] = vec[i] ^ mask;
    }

    int remainingBits = bits % 32;
    if (remainingBits > 0) {
        mask = (1 << remainingBits) - 1;
        result.vec[sizeVector - 1] &= mask;
    }

    return result;
}


BoolVec BoolVec::operator& (const BoolVec& robj) 
{
    BoolVec result(bits);

    if (vec == NULL || robj.vec == NULL ||
        bits != robj.bits) {
        return result;
    }

    for (int i = 0; i < result.sizeVector; i++)
        result.vec[i] = vec[i] & robj.vec[i];

    return result;
}


BoolVec BoolVec::operator| (const BoolVec& robj) 
{
    BoolVec result(bits);

    if (vec == NULL || robj.vec == NULL || bits != robj.bits) 
    {
        return result;
    }

    for (int i = 0; i < result.sizeVector; i++)
        result.vec[i] = vec[i] | robj.vec[i];

    return result;
}


ostream& operator<< (ostream& out, const BoolVec& vec) 
{
    out << vec.ConvertVecToStr();
    return out;
}


istream& operator>> (istream& in, BoolVec& vec) 
{
    string str;
    cout << "Input vector: ";
    in >> str;
    BoolVec result(str.c_str());

    vec = result;

    return in;
}


class BoolMatrix {
private:
    BoolVec* boolMatrix;
    size_t rows, cols;
public:
    BoolMatrix(size_t n = 1, size_t m = 1);
    BoolMatrix(const BoolMatrix&);
    ~BoolMatrix();
    BoolVec& operator[] (size_t);
    BoolMatrix operator= (const BoolMatrix&);
    void TopSort(BoolMatrix&, size_t, vector<int>&);
    friend ostream& operator<< (ostream&, BoolMatrix&);

    size_t getRows() { return rows; }
    size_t getCols() { return cols; }


};


void BoolMatrix::TopSort(BoolMatrix& matrix, size_t m, vector<int>& arr) 
{
    BoolVec vec0(m);// использованные вершины
    BoolVec vec1(m);// нет пути
    BoolVec vec2(m);// вершины без входящих ребер но ещё не обработаны
    BoolVec mask(m);// макса из нулей 

    while (vec0.weight() != m)//пока все вершины не будут использованы
    {
        vec1 = mask;//обнуляем при каждом проходе
        
        for (int i = 0; i < m; i++)// Вершины в которые есть путь
            vec1 = vec1 | matrix[i];


        vec1 = ~vec1;//вершины в которые не имеют входящих дуг
        
        vec2 = vec1 & (~vec0);// необработанные верщины без входящих ребер


        if (vec2 == mask)// проверка на цикл
        {
            cout << "Cycle!" << endl;
            exit(1);
        }

        for (int i = 0; i < m; i++)// цикл по вершинам
        {
            if (vec2[i] == 1)// если есть необработанная
            {
                arr.push_back(i + 1);// добавляем в конец вектора 
                matrix[i] = mask;   // Удаляем рёбра из найденных вершин
            }
        }

        vec0 = vec0 | vec2;// запоминаем обработанные вершины
    }
}


BoolMatrix ReadFromFile() 
{
    ifstream fileIn("graf.txt");// открываем файл для чтения
    if (!fileIn.is_open())// проверка  
    {
        cout << "File can't find\n";
        exit(1);
    }

    int a, b;// вершины
    int m;// количество вершин

    fileIn >> m;
    BoolMatrix boolM(m, m);
    while (!fileIn.eof())// идём до конца файла
    {
        fileIn >> a;
        fileIn >> b;
        boolM[a - 1].Set1(b - 1);//в столбец a и строчку b устанавляваем 1 что есть такой путь
    }
    fileIn.close();// закрываем файл

    return boolM;
}


BoolMatrix::BoolMatrix(size_t n, size_t m) 
{
    if (!n || !m) {
        boolMatrix = NULL;
        rows = 0;
        cols = 0;
        return;
    }

    rows = n;
    cols = m;
    boolMatrix = new BoolVec[rows];

    for (int i = 0; i < rows; i++)
        boolMatrix[i] = BoolVec(cols);
}


BoolMatrix::BoolMatrix(const BoolMatrix& obj) 
{
    rows = obj.rows;
    cols = obj.cols;
    boolMatrix = new BoolVec[rows];
    for (int i = 0; i < rows; i++)
        boolMatrix[i] = obj.boolMatrix[i];
}


BoolMatrix::~BoolMatrix() 
{
    if (boolMatrix)
        delete[] boolMatrix;
    rows = 0;
    cols = 0;
}

BoolMatrix BoolMatrix::operator= (const BoolMatrix& obj) 
{
    if (this == &obj)
        return *this;

    delete[] boolMatrix;

    rows = obj.rows;
    cols = obj.cols;

    boolMatrix = new BoolVec[rows];

    for (int i = 0; i < rows; i++)
        boolMatrix[i] = obj.boolMatrix[i];

    return *this;
}


ostream& operator<< (ostream& out, BoolMatrix& obj) 
{
    for (int i = 0; i < obj.rows; i++)
        out << obj.boolMatrix[i] << endl;

    return out;
}


BoolVec& BoolMatrix::operator[](size_t index) 
{
    if (index >= rows) 
    {
        cerr << "Error []: index out of range!\n";
        exit(1);
    }
    return boolMatrix[index];
}


int main() 
{
    BoolMatrix matrix;

    matrix = ReadFromFile();
    cout << matrix << endl;
    
    vector<int> arr;
    
    matrix.TopSort(matrix, matrix.getRows(), arr);

    for (auto p : arr)
        cout << p << " ";

    cout << endl;

    return 0;
}
