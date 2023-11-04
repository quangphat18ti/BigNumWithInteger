#include<iostream>
#include<vector>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include <algorithm>
#include <chrono>
#include <map>
#include <string>
#include<bitset>
#include<cmath>
using namespace std;

typedef long long ll;
typedef unsigned int uint;

// const int MAX_BIT = 1024 + 5;
const int BIT_PER_DIGIT = 30;
const  ll BASE = 1ll << BIT_PER_DIGIT;
// const int MAX_SIZE = MAX_BIT / BIT_PER_DIGIT + 1;

string hex_to_bin(string a_hex) {
    string a_bin;
    for (int i = 0; i < int(a_hex.size()); i++) {
        if (a_hex[i] >= '0' && a_hex[i] <= '9') {
            a_bin += bitset<4>(a_hex[i] - '0').to_string();
        } else {
            a_bin += bitset<4>(a_hex[i] - 'A' + 10).to_string();
        }
    }
    return a_bin;
}

string bin_to_hex(string a_bin) {
    
    string a_hex;
    for (int i = 0; i < int(a_bin.size()); i += 4) {
        int num = 0;
        for (int j = 0; j < 4; j++) {
            num += (a_bin[i + j] - '0') * pow(2, 3 - j);
        }
        if (num >= 0 && num <= 9) {
            a_hex += to_string(num);
        } else {
            a_hex += (char)(num - 10 + 'A');
        }
    }
    return a_hex;
}

string int_to_binary(int x) {
    string a_bin;
    string temp = "";
    if(x < 0) {
        temp = "-";
        x = -x;
    }
    if(x == 0) {
        return "0";
    }
    while (x > 0) {
        a_bin = to_string(x % 2) + a_bin;
        x /= 2;
    }
    return temp + a_bin;
}

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
  BigNumWithInteger operator %    (BigNumWithInteger a) const;
  BigNumWithInteger operator %=   (const BigNumWithInteger a);
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
  bool             is_one() const;
};

BigNumWithInteger::BigNumWithInteger()
{
  digits.clear();
  sign = 1;
}

BigNumWithInteger::BigNumWithInteger(ll s) {
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

  this->trim();
}

string BigNumWithInteger::to_string() const
{
  if(is_zero()) return "0";
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
    if(this->abs() >= a.abs()) {
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

  bool is_negative = (sign == -1);

  int n = size();
  int m = a.size();
  if(n < m) return true ^ is_negative;
  if(n > m) return false ^ is_negative;

  for(int i = n - 1; i >= 0; i--) {
    if(digits[i] < a.digits[i]) return true ^ is_negative;
    if(digits[i] > a.digits[i]) return false ^ is_negative;
  }
  return false;
}

bool BigNumWithInteger::operator<=(const BigNumWithInteger &a)
{
  if(sign != a.sign) return sign < a.sign;
  bool is_negative = (sign == -1);

  int n = size();
  int m = a.size();
  if(n < m) return true ^ is_negative;
  if(n > m) return false ^ is_negative;
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

BigNumWithInteger BigNumWithInteger::operator%(BigNumWithInteger a) const
{
  BigNumWithInteger ans(*this);
  a = a.abs();
  ans = ans.abs();
  if(ans < a) {
    ans.sign = sign;
    return ans;
  }

  while(ans >= a) {
    ans = ans - a;
  }
  ans.trim();
  ans.sign = sign;
  return ans;
}

BigNumWithInteger BigNumWithInteger::operator%=(BigNumWithInteger a)
{
  *this = *this % a;
  return *this;
}

BigNumWithInteger BigNumWithInteger::operator*(const BigNumWithInteger& a) const
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
  if(d.is_zero() || this->is_zero()) {
    return ans;
  }

  ans.sign = this->sign * d.sign;
  d = d.abs();

  string binary = d.to_string();
  reverse(binary.begin(), binary.end());
  
  BigNumWithInteger temp = (*this);
  temp = temp.abs();
  temp%=n;

  for(int i = 0; i < binary.size(); i++) {
    if(binary[i] == '1') {
      ans = (ans + temp) % n;
    }
    temp = (temp + temp) % n;
  }

  return ans%n;
}

BigNumWithInteger BigNumWithInteger::powMod(BigNumWithInteger d, BigNumWithInteger n)
{
  BigNumWithInteger ans(1);
  if(this->is_zero()) return BigNumWithInteger(0);
  if(d.is_zero())  return ans;
  if(d.is_one()) return *this;

  if(d.sign == -1) {
    throw "Exponent must be positive";
  }

  string binary = d.to_string();
  reverse(binary.begin(), binary.end());

  BigNumWithInteger x(*this); 
  x = x.abs();
  x %= n;
  for(int i = binary.size() - 1; i >= 0; i--) {
    if(! (ans.is_zero() || ans.is_one()))
    {
      ans = ans.mulMod(ans, n);
    }     

    if(binary[i] == '1') {
      ans = ans.mulMod(x, n);
    }

    if(ans >= n) {
      cerr << "ans >= n\n";
    }
  }

  ans %= n;

  // cout << "ans = " << ans << endl;
  // cout << "n   = " << n << endl;
  // cout << "ans >= n: " << (ans >= n) << endl;
  // cout << ans.size() << ' ' << n.size() << endl;
  // cout << ans.bit_size() << ' ' << n.bit_size() << endl;
  // cout << ans.digits.back() << ' ' << n.digits.back() << endl;

  if(sign == 1) {
    ans.sign = 1;
  } else {
    ans.sign = d[0] & 1 ? -1 : 1;
  }

  return ans;
}

ostream &operator<<(ostream & out, const BigNumWithInteger &a)
{
  out << a.to_string();
  return out;
}

void BigNumWithInteger::trim()
{
  while(digits.size() > 0 && !digits.back()) {
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

bool BigNumWithInteger::is_one() const
{
  return digits.size() == 1 && digits[0] == 1;
}

bool test(BigNumWithInteger &a, BigNumWithInteger &n) {
    BigNumWithInteger n_minus_1 = n - BigNumWithInteger("1");
    BigNumWithInteger d = n_minus_1;
    int s = 0;
    while(d[0] == 0) {
        d >> 1;
        s++;
    }
    /// n - 1 = 2^s * d

    // cout << "d = " << d << endl;
    // cout << "s = " << s << endl;        
    // cout << "a = " << a << endl;
    BigNumWithInteger x = a.powMod(d, n);
    // cout << "x = " << x << endl;
    if(x == BigNumWithInteger("1") || x == n_minus_1) {
        return true;
    }

    bool check = false;
    for(int r = 1; r <= s; r++) {
        x = x.mulMod(x, n);
        if(x == BigNumWithInteger("1")) {
            check = true;
            break;
        }
        if(x == n_minus_1) {
            check = true;
            break;
        }
        // cerr << "x = " << x << endl;
    }

    if(!check) {
        return false;
    }
    return true;
}

bool MillerRabin(BigNumWithInteger n, vector<int> primes, int k = 4) {
    int max_bit = n.max_bit();

    if(max_bit < 64) {
        k = 240;
    }
    else if(max_bit < 128) {
        k = 120;
    }
    else if(max_bit < 256) {
        k = 60;
    }
    else {
        k = 30;
    }

    if(k > primes.size()) {
        k = primes.size();
    }

    // cerr << "max_bit = " << max_bit << endl;
    // cerr << "k = " << k << endl;m
    
    BigNumWithInteger n_minus_1 = n - BigNumWithInteger("1");

    while(k--) { 
        int pos = rand() % primes.size(); 
        int prime = primes[pos]; 
        primes.erase(primes.begin() + pos); 
 
        string prime_binary = int_to_binary(prime); 
        BigNumWithInteger a(prime_binary); 
        if(a >= n) { 
            continue; 
        } 
 
        if(!test(a, n)) { 
            return false; 
        } 
    } 
    return true;
}

vector<int> sangnt(int n) {
  vector<int> primes;
  vector<bool> isPrime(n + 1, true);
  isPrime[0] = isPrime[1] = false;
  for (int i = 2; i <= sqrt(n); i++) {
    if (isPrime[i]) {
      for (int j = i * i; j <= n; j += i) {
        isPrime[j] = false;
      }
    }
  }

    for (int i = 2; i <= n; i++) {
        if (isPrime[i]) {
        primes.push_back(i);
        }
    }
  return primes;
}

int main(int argc, char** argv) {
    if(argc != 3) {
        cout << "Usage: ./main <input_file> <output_file>" << endl;
        return 0;
    }

    freopen(argv[1], "r", stdin);
    freopen(argv[2], "w", stdout);

    srand(time(NULL));
    vector<int> primes = sangnt(10000);

    string number_hex;
    cin >> number_hex;
    reverse(number_hex.begin(), number_hex.end());
    
    string number_binary = hex_to_bin(number_hex);
    BigNumWithInteger number(number_binary);
    cout << MillerRabin(number, primes) << endl;
}