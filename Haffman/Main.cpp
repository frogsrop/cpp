#include<bits/stdc++.h>
#include<Haffman.h>
using namespace std;

void encode(char sin[256], char sout[256])
{
    ifstream in(sin,ios_base::in | ios_base::binary);
    ofstream out(sout,ios_base::out |ios_base::binary);
    haffman h;
    
    char ch;
    while(in.get(ch))
    {
        unsigned char chh = (unsigned char) ch;
        h.incSymbol(chh);
    }
    h.build();
    
    unsigned long long * inf = h.getinf();
    unsigned long long ls = 0;
    for(int i = 0; i < 512; i++)
    {
        unsigned int res = (unsigned int) inf[i];
        if(res > 0)
        {
            unsigned long long num = ((h.getCode(i).num_().size() - 1) * 8  +   h.getCode(i).used_()) * res;
            ls+= num;
        }
        for(int j = 7;j >= 0; j--)
        {
            unsigned char temp = (unsigned char) (res >> (4 * j));
            out.put(temp);
        }
    }
    for(int j = 7;j >= 0; j--)
    {
        unsigned char temp = (unsigned char) (ls >> (4 * j));
        out.put(temp);
    }
    byte b(0, 0);
    in.clear();
    in.seekg(0);
    while(in.get(ch))
    {
        unsigned char chh = (unsigned char) ch;
        b.sum(h.getCode(chh));
        if(b.num_().size() > 1 || b.used_() == 8)
        {
            if (b.used_() == 8)
            {
                for (int i = 0; i < b.num_().size(); i++)
                {
                    out.put(b.num_()[i]);
                }
                b = byte(0, 0);
            }
            else
            {
                for (int i = 0; i < b.num_().size() - 1; i++)
                {
                    out.put(b.num_()[i]);
                }
                b = byte({b.num_().back()}, b.used_());
            }
        }
    }
    if(b.used_()!=0)
    {
        uint_fast8_t temp = b.num_().back();
        temp <<= (8-b.used_());
        out.put(temp);
    }
    in.close();
    out.flush();
    out.close();
}
bool checker(unsigned long long* x, unsigned long long* y)
{
    for(int i = 0; i < 512; i++)
    {
        if(x[i]!=y[i])
        {
            return 0;
        }
    }
    return 1;
}
void decode(char sin[256], char sout[256])
{
    ifstream in(sin, ios_base::binary);
    ofstream out(sout, ios_base::binary);
    haffman h;
    in >> noskipws;
    unsigned long long arrayForCheck[512];
    for(int i = 0; i < 512;i++)
    {
        arrayForCheck[i] = 0;
        unsigned long long var = 0;
        for(int j = 0; j < 8; j++)
        {
            char temp = 0;
            if(!in.get(temp))
            {
                throw std::runtime_error("Can't decode file");
            }
             unsigned char ttemp = (unsigned char) temp;
            var <<= 4;
            var |= ttemp;
        }
        h.incSymbol(i, var);
    }
    unsigned long long am = 0;
    for(int j = 0; j < 8; j++)
    {
        char temp = 0;
        if(!in.get(temp))
        {
            throw std::runtime_error("Can't decode file");
        }
        unsigned char ttemp = (unsigned char) temp;
        am<<=4;
        am |= ttemp;
    }

    h.build();
    char ch = 0;
    unsigned char prev = 0;
    pair<node*, unsigned char> temp = {nullptr, 0};
    while(in.get(ch))
    {
        unsigned char chh = (unsigned char) ch;

        for(int i = 0; i < 8; i++)
        {
            am--;
            uint_fast8_t cur = (uint_fast8_t) ((chh >> 7) & 1);
            temp = h.shiftDown(cur, temp.first);
            if (temp.first == nullptr)
            {
                out.put(temp.second);
                arrayForCheck[temp.second]++;
                temp = {nullptr, 0};
            }
            chh <<= 1;
            if (am == 0)
            {
                if(!checker(h.getinf(), arrayForCheck))
                {

                    throw std::runtime_error("Can't decode file");
                }
                in.close();
                out.close();
                return;
            }
        }
    }
    if(!checker(h.getinf(), arrayForCheck))
    {
        throw std::runtime_error("Can't decode file");
    }
    in.close();
    out.close();
}
int main(int argc, char* argv[])
{
    char in[256];
    char out[256];
    cout<<"Programm started"<<endl;
    try{
        if(argc != 4)
        {
            cout<<"wrong format\n";
            cout<<"for encode \n -en input_file_name output_fine_name\n";
            cout<<"for decode \n -de input_file_name output_fine_name\n";
            return 0;
        }
        if(strcmp(argv[1], "-en") == 0)
        {
            strcpy(in, argv[2]);
            strcpy(out, argv[3]);
            encode(in, out);
            cout<<"encoding finished"<<endl;
        }
        else
        if(strcmp(argv[1], "-de") == 0)
        {
            strcpy(in, argv[2]);
            strcpy(out, argv[3]);
            decode(in, out);
            cout<<"decoding finished"<<endl;
        }
        else
        {
            cout<<"wrong args. nothing done"<<endl;
        }
    }
    catch(std::runtime_error& e)
    {
        cout<<e.what()<<endl;
    }

/*
    char in[256];
    string temp = "../Haffman/Haffman.in";
    

    strcpy(in, temp.c_str());

    char out[256];
    temp = "../Haffman/Haffman.out";

    strcpy(out, temp.c_str());
    encode(in, out);
    
    cout<< "encoding finished"<<endl;

    temp = "../Haffman/Haffman.out";
    strcpy(in, temp.c_str());
    temp = "../Haffman/Haffman1.out";
    strcpy(out, temp.c_str());
    decode(in, out);
*/
    return 0;
}
