#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

class BoolV
{
    unsigned int *vec;
    int nbit;// nbit – количество бит вектора
    int size_v;//размер вектора или количество элементов массива vec (bytes)
public:
    BoolV(int nn = 1)
    {
        nbit = nn;
        size_v = (nn - 1) / 32 + 1;
        vec = new unsigned int[size_v];
        for(int i = 0; i < size_v; i++)
            vec[i] = 0;
    }

    BoolV(const char *str) : BoolV:: BoolV(strlen(str))
    {
        unsigned int mask = 1;

        for(int i = 0; i < nbit; i++)
        {
            if(mask == 0)
                mask = 1;

            if(str[i] != '0')
                vec[i/32] = vec[i/32] | mask;

            mask = mask << 1;


        }
    }

    BoolV(const BoolV &other)
    {
        nbit = other.nbit;
        size_v = other.size_v;
        vec = new unsigned int[size_v];
        for(int i = 0; i < size_v; i++)
        {
            vec[i] = other.vec[i];
        }
    }

    ~BoolV()
    {
        if(vec != NULL)
            delete[] vec;

        nbit = 0;
        size_v = 0;
    }

    int weight()
    {
        int count = 0;
        unsigned mask = 1;
        for(int i = 0; i < nbit; i++)
        {
            if(vec[i/32] & mask)
                count++;

            mask = mask << 1;

            if(mask == 0)
                mask = 1;
        }
        return count;
    }

    void set1(int pos)//5
    {
        if(pos > nbit || pos < 0)
        {
            cout << "Index out of range" << endl;
            exit(-1);
        }

        unsigned int mask = 1;
        int byte_v = pos / 32;
        mask = mask << (pos % 32);
        vec[byte_v] = vec[byte_v] | mask;
    }

    void set0(int pos)
    {
        if(pos > nbit || pos < 0)
        {
            cout << "INdex out of range" << endl;
            exit(-2);
        }

        unsigned int mask = 1;
        int byte_v = pos / 32;
        mask = mask << (pos % 32);
        vec[byte_v] = vec[byte_v] & ~(mask);
    }

    int operator[](int pos)
    {
        if(pos > nbit || pos < 0)
        {
            cout << "Index out of range" << endl;
            exit(-3);
        }

        unsigned int mask = 1;
        int byte_v = pos / 32;
        mask = mask << (pos % 32);
        bool res = vec[byte_v] & mask;

        return res;
    }

    BoolV operator= (const BoolV &other)
    {
        if(this == &other)
            return *this;

        delete[] vec;

        vec = new unsigned int[other.size_v];
        nbit = other.nbit;
        size_v = other.size_v;
        for(int i = 0; i < size_v; i++)
        {
            vec[i] = other.vec[i];
        }

        return *this;
    }

    bool operator==(const BoolV &other)
    {
        if(nbit != other.nbit)
            return false;
        for(int i = 0; i < size_v; i++)
        {
            if(vec[i] != other.vec[i])
                return false;
        }

        return true;
    }

    BoolV operator|(const BoolV &other)//логическое или
    {
        int  size = nbit;

        BoolV res(size);

        for(int i = 0; i < size_v; i++)
        {
            res.vec[i] = vec[i] | other.vec[i];
        }
        return res;
    }

    BoolV operator& (const BoolV &other)
    {
        int size = nbit;

        BoolV res(size);

        for(int i = 0; i < size_v; i++)
        {
            res.vec[i] = vec[i] & other.vec[i];
        }
        return res;
    }

    BoolV operator~()//????????
    {
        BoolV res(*this);//конструктор копирования
        
        unsigned int mask = 65535;
        for(int i = 0; i < size_v; i++)
        {
            res.vec[i] = ~vec[i];
        }
        unsigned int  mask = (1 << (nbit % 32)) - 1;
        res.vec[size_v - 1] = res.vec[size - 1] & mask;
        
        return res;
    }

    friend ostream& operator<< (ostream &os, const BoolV &boolV)
    {
        char *str = new char[boolV.nbit + 1];
        str[boolV.nbit] = '\0';
        for(int i = 0; i < boolV.nbit; i++)
        {
            unsigned int mask = 1;
            if(boolV.vec[i/32] & (mask << (i % 32)))
                str[i] = '1';
            else
                str[i] = '0';
        }
        os << str << endl;

        return os;
    }

    friend istream& operator>> (istream &in, BoolV &boolV)
    {
        string str;
        in >> str;

        BoolV res(str.c_str());//Чтобы сделать указатель на строку в C++, можно использовать функцию-член c_str() для std::string.

        boolV = res;

        return in;
    }

};

class BoolM
{
    BoolV *bm;
    int m, n;// m – количество строк, n – количество столбцов
public:
    BoolM(int k = 1, int l = 1) : m(k), n(l)
    {
        if(k <= 0 || l <= 0)
        {
            cout << "Error index" << endl;
            exit(-4);
        }

        bm = new BoolV[m];
        for(int i = 0; i < m; i++)
        {
            bm[i] = BoolV(n);
        }
    }

    BoolM(const BoolM &other) : m(other.m), n(other.n)
    {
        bm = new BoolV[m];
        for(int i = 0; i < m; i++)
        {
            bm[i] = other.bm[i];
        }
    }

    ~BoolM()
    {
        delete[] bm;
    }

    BoolV & operator[] (int pos)
    {
        return bm[pos];
    }

    BoolM operator= (const BoolM &other)
    {
        if(this == &other)
            return *this;

        delete[] bm;
        m = other.m;
        n = other.n;
        bm = new BoolV[m];
        for(int i = 0; i < m; i++)
        {
            bm[i] = other.bm[i];
        }
        return *this;
    }

    void TopSort(BoolM &M, int m, int *a)
    {

    }

    BoolM CreateMx_byGraf()
    {
        ifstream file("graf.txt");
        if(!file.is_open())
        {
            cout << "file not open" << endl;
            exit(-4);
        }

        int a, b;//a - начало b - конец
        int m;//количество вершин

        file >> m;
        BoolM Mx(m, m);

        while(file >> a >> b)
        {
            Mx[a - 1].set1(b - 1);
        }

        file.close();

        return Mx;
    }

    friend ostream& operator<< (ostream& os, BoolM &boolM)
    {
        for(int i = 0; i < boolM.m; i++)
        {
            os << boolM[i] << endl;
        }
        return os;
    }
};



int main()
{
    BoolV vec1;
    cin >> vec1;

    cout << vec1;

    BoolV vec2("apa00011212la0");
    //          11100011111110

    cout << "vec2 - " << vec2;

    BoolV vec3(vec2);

    cout << "vec3 - " << vec3 << endl;

    vec3.set0(10);

    cout << "vec3.set0(10) - " << vec3;

    vec3.set1(5);

    cout << "vec3.set1(5) - " << vec3;
    vec3 = ~vec3;
    cout << "~vec3 - " << vec3 << endl;

    BoolV vec4;

    vec4 = vec3 & vec2;

    cout << "vec4 = vec3 & vec2 = " << vec4 << endl;

    BoolV vec5;

    vec5 = vec3  | vec2;
    cout << "vec5 = vec3 | vec2 = " << vec5 << endl;


    int weight_vec5 = vec5.weight();
    cout << weight_vec5 << endl;

    return 0;
}
