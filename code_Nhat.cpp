#include <iostream>
#include <fstream>
#include <string.h>
#include <bitset>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace std;

const int MAX_BIT = 512;
typedef bitset<MAX_BIT> nBit;

std::chrono::time_point<std::chrono::high_resolution_clock> start;
std::chrono::duration<double> duration;

int resPrime = 0;

string hexToBin(string a)
{
    string res;
    for (char c : a)
    {
        int decimal;
        if (c >= '0' && c <= '9')
            decimal = c - '0';
        else
            decimal = c - 'A' + 10;

        res = bitset<4>(decimal).to_string() + res;
    }
    return res;
}
vector<nBit> primeBitset()
{
    int pr[168] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691, 701, 709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809, 811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997};
    vector<nBit> res;
    for (int i = 0; i < 168; i++)
        res.push_back(nBit(pr[i]));
    return res;
}
bool checkTime()
{
    auto end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    return duration.count() > 59;
}
bool checkAns()
{
    return resPrime >= 3;
}
bitset<MAX_BIT + 1> addBs(nBit &x, nBit &y)
{
    bool carry = 0;
    int x_1 = x.count();
    int y_1 = y.count();
    bitset<MAX_BIT + 1> res;
    for (int i = 0; i < MAX_BIT; i++)
    {
        if(checkTime())
            return 0;

        res[i] = (x[i] ^ y[i]) ^ carry;
        carry = (x[i] & y[i]) | (carry & (x[i] ^ y[i]));

        x_1 -= x[i];
        y_1 -= y[i];

        if (carry == 0 && x_1 == 0 && y_1 == 0)
            break;
    }
    res[MAX_BIT] = carry;
    return res;
}
bool subBit(bool b1, bool b2, bool &borrow)
{
    bool diff;
    if (borrow)
    {
        diff = !(b1 ^ b2);
        borrow = !b1 || (b1 && b2);
    }
    else
    {
        diff = b1 ^ b2;
        borrow = !b1 && b2;
    }
    return diff;
}
bitset<MAX_BIT + 1> subBs(bitset<MAX_BIT + 1> &x, nBit &mod)
{
    bool borrow = 0;
    bitset<MAX_BIT + 1> res;
    for (int i = 0; i < MAX_BIT; i++)
    {
        if(checkTime())
            return 0;

        res[i] = subBit(x[i], mod[i], borrow);
    }

    if (borrow)
        res[MAX_BIT] = 0;
    else
        res[MAX_BIT] = x[MAX_BIT];
    return res;
}
// int compBit(bitset<MAX_BIT + 1> &x, nBit &y)
// {
//     if(x[MAX_BIT] == 1) return 1;
//     for(int i = MAX_BIT - 1; i>=0;i--)
//         if(x[i] > y[i]) return 1;
//         else if(x[i] < y[i]) return -1;
//     return 0;
// }
nBit modBs(bitset<MAX_BIT + 1> x, nBit &mod)
{
    while (x[MAX_BIT] == 1 || x.to_string().substr(1, MAX_BIT) >= mod.to_string())
    {
        if(checkTime())
            return 0;
        x = subBs(x, mod);
    }

    string temp = x.to_string().substr(1, MAX_BIT);
    nBit res = nBit(temp);
    return res;
}
nBit addMod(nBit &x, nBit &y, nBit &mod)
{
    if (x == 0)
        return modBs(bitset<MAX_BIT + 1>(y.to_string()), mod);
    if (y == 0)
        return modBs(bitset<MAX_BIT + 1>(x.to_string()), mod);

    bitset<MAX_BIT + 1> res = addBs(x, y);
    return modBs(res, mod);
}
nBit mulMod(nBit &x, nBit &y, nBit &mod)
{
    nBit res = nBit(0);
    if (y == 0 || x == 0)
        return res;
    if (x == 1)
        return modBs(bitset<MAX_BIT + 1>(y.to_string()), mod);
    if (y == 1)
        return modBs(bitset<MAX_BIT + 1>(x.to_string()), mod);

    nBit temp = x;
    int y_1 = y.count();
    for (int i = 0; i < MAX_BIT; i++)
    {
        if(checkTime())
            return 0;

        if (y[i] == 1)
            res = addMod(res, temp, mod), y_1--;
        if (y_1 == 0)
            break;
        
        if(checkTime())
            return 0;

        bitset<MAX_BIT + 1> tt = bitset<MAX_BIT + 1>(temp.to_string());
        tt <<= 1;
        temp = modBs(tt, mod);
    }
    return res;
}
nBit powerMod(nBit &x, nBit &y, nBit &mod)
{
    nBit res = nBit(1);
    if (y == 0)
        return res;
    if (y == 1)
        return modBs(bitset<MAX_BIT + 1>(x.to_string()), mod);
    for (int i = MAX_BIT - 1; i >= 0; i--)
    {
        if(checkTime())
            return 0;

        if (res != nBit(1))
            res = mulMod(res, res, mod);

        if(checkTime())
            return 0;
        
        if (y[i] == 1)
            res = mulMod(res, x, mod);
    }
    return res;
}
bool checkPrime(string &s, vector<nBit> &p)
{
    nBit sbs = nBit(s);
    nBit fbs = (sbs ^ nBit(1));
    if (sbs == nBit(1))
        return 0;
    if (sbs == nBit(2))
        return 1;

    int kt[200];
    for (int i = 0; i < 168; i++)
    {
        kt[i] = 0;
        if (p[i] == sbs)
            return 1;
    }

    for (int i = 0; i < 4; i++)
    {
        int randPrime = std::rand() % p.size();
        while (kt[randPrime] == 1)
            randPrime = std::rand() % p.size();

        // int randPrime = std::rand() % 50;
        // while (kt[randPrime] == 1)
        //     randPrime = std::rand() % 50;

        nBit p_i = p[randPrime];
        kt[randPrime] = 1;

        nBit temp = (sbs ^ nBit(1));

        nBit m = temp;
        int r = 0;
        while (m[0] == 0)
            m >>= 1, r++;

        if(checkTime())
            return checkAns();

        nBit ans = powerMod(p_i, m, sbs);

        if(checkTime())
            return checkAns();

        bool check = 0;
        for (int i = 1; i <= r; i++)
        {
            nBit pre = ans;
            ans = mulMod(ans, ans, sbs);
            if (ans == nBit(1) || (ans == fbs && i != r))
            {
                if (ans == nBit(1))
                    resPrime++;
                else
                    resPrime += 2;

                if(ans == nBit(1) && pre != nBit(1) && pre != fbs)
                    return 0;

                check = 1;
                break;
            }

            if(checkTime())
                return checkAns();
        }
        if (!check)
            return 0;
    }
    return 1;
}
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cerr << "Usage: " << argv[0] << " input_file output_file" << endl;
        return 1;
    }

    ifstream inp(argv[1]);
    if (!inp.is_open())
    {
        cerr << "Could not open input file: " << argv[1] << endl;
        return 1;
    }
    ofstream out(argv[2]);
    if (!out.is_open())
    {
        cerr << "Could not open output file: " << argv[2] << endl;
        return 1;
    }
    std::srand(std::time(0));

    start = std::chrono::high_resolution_clock::now();
    string s;
    string sbn;
    vector<nBit> p_bs;

    inp >> s;

    sbn = hexToBin(s);
    p_bs = primeBitset();

    out << checkPrime(sbn, p_bs);

    inp.close();
    out.close();
    return 0;
}