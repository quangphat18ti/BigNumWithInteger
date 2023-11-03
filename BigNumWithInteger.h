#pragma once
#include <iostream>
#include<vector>
#include<algorithm>

typedef long long ll;

using namespace std;

const int MAX_BIT = 515;
const int BIT_PER_DIGIT = 30;
const ll BASE = 1ll << BIT_PER_DIGIT;
const int MAX_SIZE = MAX_BIT / BIT_PER_DIGIT + 1;

typedef long long ll;
typedef unsigned int uint;

class BigNumWithInteger{
  using TYPE = int;

  vector<TYPE>     digits;
  int sign;  // sign == 1 is positive, sign == -1 is negative
public:
  BigNumWithInteger();
  BigNumWithInteger(ll);
  BigNumWithInteger(string);
  BigNumWithInteger(const BigNumWithInteger& a) : digits(a.digits), sign(a.sign) { }

  BigNumWithInteger& operator=(const BigNumWithInteger& a) { digits = a.digits; sign = a.sign; return *this;}

public:
  string to_string() const;
  friend ostream& operator<<(ostream&, const BigNumWithInteger&);

public: 
  BigNumWithInteger operator+ (const BigNumWithInteger& a);
  BigNumWithInteger operator- (BigNumWithInteger a);
  BigNumWithInteger operator ++ (int i) { BigNumWithInteger temp = *this; *this = *this + BigNumWithInteger("1"); return temp; }
  BigNumWithInteger operator ++ () { *this = *this + BigNumWithInteger("1"); return *this; }
  bool              operator == (const BigNumWithInteger& a) const { return digits == a.digits && sign == a.sign; }
  bool              operator != (const BigNumWithInteger& a) { return !(*this == a); }
  bool              operator <  (const BigNumWithInteger& a);
  bool              operator <=  (const BigNumWithInteger& a);
  bool              operator >  (const BigNumWithInteger& a);
  bool              operator >=  (const BigNumWithInteger& a);
  TYPE&             operator [](int i) ;
  void set(int i, TYPE value) { digits[i] = value; }
  BigNumWithInteger operator >> (int i);
  BigNumWithInteger operator << (int i);
  BigNumWithInteger operator %    (const BigNumWithInteger& a) const;
  BigNumWithInteger operator %=   (const BigNumWithInteger& a);
  BigNumWithInteger operator *    (const BigNumWithInteger& a) const;
  BigNumWithInteger abs           () const;

public:
  BigNumWithInteger mulMod(BigNumWithInteger d, BigNumWithInteger n);
  BigNumWithInteger powMod(BigNumWithInteger d, BigNumWithInteger n);
  int              max_bit() {return bit_size(); }
    void             trim();
    int              size() const;
    int              bit_size() const;
    bool             is_zero() const;
};