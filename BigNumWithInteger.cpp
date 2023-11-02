#include "BigNumWithInteger.h"
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

BigNumWithInteger::BigNumWithInteger()
{
  digits.clear();
  sign = 1;
}

BigNumWithInteger::BigNumWithInteger(int s) {
  digits.clear();
  if(s < 0) {
    sign = -1;
    s = -s;
  } else {
    sign = 1;
  }

  if(s == 0) {
    digits.push_back(0);
    return;
  }

  while(s > 0) {
    digits.push_back(s % BASE);
    s /= BASE;
  }
}

BigNumWithInteger::BigNumWithInteger(string s) {
  digits.clear();
  sign = 1;
  if(s[0] == '-') {
    sign = -1;
    s = s.substr(1);
  }

  if(s.size() == 0) {
    digits.push_back(0);
    return;
  }
  reverse(s.begin(), s.end());
  int pos = 0;
  while(pos < s.size()) {
    uint value = 0;
    for(int i = pos; i < pos + BIT_PER_DIGIT && i < s.size(); i++) {
      value += (s[i] - '0') * (1 << (i - pos));
    }
    digits.push_back(value);
    pos += BIT_PER_DIGIT;
  } 
}

string BigNumWithInteger::to_string() const
{
  string binary = "";
  for(uint val : digits) {
    for(int i = 0; i < BIT_PER_DIGIT; i++) {
      binary += (val & (1 << i)) ? '1' : '0';
    }
  }

  while(binary.size() > 1 && binary.back() == '0') {
    binary.pop_back();
  }

  reverse(binary.begin(), binary.end());
  if(sign == -1) {
    binary = '-' + binary;
  }
  return binary;
}

BigNumWithInteger BigNumWithInteger::operator+(const BigNumWithInteger &a)
{
  int n = size();
  int m = a.size();
  BigNumWithInteger ans;
  ans.digits.resize(max(n, m) + 1);
  
  if(sign == a.sign) {
    ans.sign = sign;
    int carry = 0;
    ll sum = 0;
    for(int i = 0; i < max(n, m); i++) {
      sum = carry;
      if(i < n) sum += digits[i];
      if(i < m) sum += a.digits[i];
      ans.digits[i] = sum % BASE;
      carry = sum / BASE;
    }
    ans.digits[max(n, m)] = carry;
    ans.trim();
    return ans;
  }
  else {
    if(this->abs() > a.abs()) {
      ans.sign = sign;
      int carry = 0;
      ll sum = 0;
      for(int i = 0; i < n; i++) {
        sum = carry + digits[i];
        if(i < m) sum -= a.digits[i];
        if(sum < 0) {
          sum += BASE;
          carry = -1;
        } else {
          carry = 0;
        }
        ans.digits[i] = sum;
      }
      ans.trim();
      return ans;
    } else {
      ans.sign = a.sign;
      int carry = 0;
      ll sum = 0;
      for(int i = 0; i < m; i++) {
        sum = carry + a.digits[i];
        if(i < n) sum -= digits[i];
        if(sum < 0) {
          sum += BASE;
          carry = -1;
        } else {
          carry = 0;
        }
        ans.digits[i] = sum;
      }
      ans.trim();
      return ans;
    }
  }
}

BigNumWithInteger BigNumWithInteger::operator-(BigNumWithInteger a)
{
  a.sign = -a.sign;
  return *this + a;
}

bool BigNumWithInteger::operator<(const BigNumWithInteger &a)
{
  if(sign != a.sign) return sign < a.sign;

  bool is_negative = sign == -1;

  int n = size();
  int m = a.size();
  if(n < m) return true;
  if(n > m) return false;
  for(int i = n - 1; i >= 0; i--) {
    if(digits[i] < a.digits[i]) return true ^ is_negative;
    if(digits[i] > a.digits[i]) return false ^ is_negative;
  }
  return false;
}

bool BigNumWithInteger::operator<=(const BigNumWithInteger &a)
{
  if(sign != a.sign) return sign < a.sign;
  bool is_negative = sign == -1;

  int n = size();
  int m = a.size();
  if(n < m) return true;
  if(n > m) return false;
  for(int i = n - 1; i >= 0; i--) {
    if(digits[i] < a.digits[i]) return true ^ is_negative;
    if(digits[i] > a.digits[i]) return false ^ is_negative;
  }
  return true;
}

bool BigNumWithInteger::operator>(const BigNumWithInteger &a)
{
  return !(*this <= a);
}

bool BigNumWithInteger::operator>=(const BigNumWithInteger &a)
{
  return !(*this < a);
}

int& BigNumWithInteger::operator[](int i) 
{
  if(i < 0 || i > size()) {
    throw "Index out of range";
  }

  return digits[i];
}

BigNumWithInteger BigNumWithInteger::operator>>(int i)
{
  if(i < 0) {
    throw "Shift right must be positive";
  }

  if(i == 0) return *this;

  if(i > BASE) {
    throw "Shift right ammount is to big";
  }

  int value = 1 << (i);

  int n = size();
  BigNumWithInteger ans;
  ans.sign = sign;
  ans.digits.resize(n);
  
  int carry = 0;
  ll sum = 0;
  for(int j = n - 1; j >= 0 ; j--) {
    if(j == n - 1) {
      carry = digits[j] % value;
      ans.digits[j] = digits[j] >> i;
    }
    else{
      sum = digits[i];
      if(carry > 0) {
        sum += BASE * carry;
      }
      carry = sum % value;
      ans.digits[i] = sum >> i;
    }
  }
  ans.trim();
  return ans;
}

BigNumWithInteger BigNumWithInteger::operator<<(int i)
{
  if(i < 0) {
    throw "Shift left must be positive";
  }

  if(i == 0) return *this;

  if(i > BASE) {
    throw "Shift left ammount is to big";
  }

  ll carry = 0;
  ll sum = 0;
  int n = size();
  BigNumWithInteger ans;
  ans.sign = sign;
  ans.digits.resize(n+i);
  for(int j = 0; j < n; j++) {
    if(j == 0) {
      sum = digits[j] << i;
      ans.digits[j] = sum % BASE;
      carry = sum / BASE;
    } else {
      sum = digits[j] + carry;
      ans.digits[j] = sum % BASE;
      carry = sum / BASE;
    }
  }
  ans.digits[n] = carry;
  ans.trim();
  return ans;
}

BigNumWithInteger BigNumWithInteger::operator%(const BigNumWithInteger &a) const
{
  BigNumWithInteger ans(*this);
  if(ans < a) return ans;

  while(ans >= a) {
    ans = ans - a;
  }
  ans.trim();
  return ans;
}

BigNumWithInteger BigNumWithInteger::operator%=(const BigNumWithInteger &a)
{
  while(*this >= a) {
    *this = *this - a;
  }
  trim();
  return *this;
}

BigNumWithInteger BigNumWithInteger::operator*(const BigNumWithInteger &a) const
{
  int n = size();
  int m = a.size();
  BigNumWithInteger ans;
  ans.digits.resize(n + m);
  ans.sign = sign * a.sign;
  
  ll carry = 0;
  ll sum = 0;
  for(int i = 0; i < n; i++) {
    carry = 0;
    for(int j = 0; j < m; j++) {
      sum = ans.digits[i + j] + 1ll * digits[i] * a.digits[j] + carry;
      ans.digits[i + j] = sum % BASE;
      carry = sum / BASE;
    }
    ans.digits[i + m] = carry;
  }

  ans.trim();
  return ans;
}

BigNumWithInteger BigNumWithInteger::abs() const
{
  BigNumWithInteger ans(*this);
  ans.sign = 1;
  return ans;
}

BigNumWithInteger BigNumWithInteger::mulMod(BigNumWithInteger d, BigNumWithInteger n)
{
  BigNumWithInteger ans;
  
  return ans;
}

BigNumWithInteger BigNumWithInteger::powMod(BigNumWithInteger d, BigNumWithInteger n)
{
  return BigNumWithInteger();
}

ostream &operator<<(ostream & out, const BigNumWithInteger &a)
{
  out << a.to_string();
  return out;
}

void BigNumWithInteger::trim()
{
  while(digits.size() > 1 && !digits.back()) {
    digits.pop_back();
  }
}

int BigNumWithInteger::size() const
{
  return digits.size();
}

int BigNumWithInteger::bit_size() const
{
  return this->to_string().size();
}

bool BigNumWithInteger::is_zero() const
{
  return digits.empty() || (digits.size() == 1 && digits[0] == 0);
}
