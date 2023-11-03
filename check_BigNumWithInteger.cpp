#include "BigNumWithInteger.h"
#include "hex_binary.h"
#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <bitset>
#include <cassert>
#include <cmath>
using namespace std;

const int MOD = 1e9 + 7;
const int MAX = 1e9;

typedef long long ll;

void check_to_string() {
  for(int test = 0; test < 10; test++) {
    int x = rand()%(MAX/10); x = rand()%2 ? x : -x;
    BigNumWithInteger x_num(x);
    BigNumWithInteger x_binary(int_to_binary(x));
    BigNumWithInteger x_copy(x_num);
    assert(x_num.to_string() == int_to_binary(x));
    assert(x_binary.to_string() == int_to_binary(x));
    assert(x_num.to_string() == x_copy.to_string());
  }
  cout << "check_to_string() is OK\n";
}

void check_mulMod() {
  for(int test = 0; test < 10; test++) {
    int x = rand() % (MAX);
    int y = rand() % (MAX);
    int z = 1ll * x * y % MOD;
    BigNumWithInteger x_binary(int_to_binary(x));
    BigNumWithInteger y_binary(int_to_binary(y));
    BigNumWithInteger z_binary = x_binary.mulMod(y_binary, BigNumWithInteger(int_to_binary(MOD)));
    // cout << x_binary << endl;
    // cout << y_binary << endl;
    // cout << z_binary << endl;
    // cout << int_to_binary(z) << endl;
    // cout << "-------\n";
    assert(z_binary.to_string() == int_to_binary(z));
  }
  cout << "check_mulMod() is OK\n";
}

int Pow(int x, int y, int mod) {
  if(y == 0) {
    return 1;
  }
  int z = Pow(x, y / 2, mod);
  z = 1ll * z * z % mod;
  if(y % 2 == 1) {
    z = 1ll * z * x % mod;
  }
  return z;
}

void check_powMod() {
  for(int test = 0; test < 10; test++) {
    int x = rand() % (MAX);
    int y = rand() % (MAX);
    int z = Pow(x, y, MOD);
    BigNumWithInteger x_binary(int_to_binary(x));
    BigNumWithInteger y_binary(int_to_binary(y));
    BigNumWithInteger z_binary = x_binary.powMod(y_binary, BigNumWithInteger(int_to_binary(MOD)));

    // cout << x_binary << " " << y_binary << " " << z_binary << endl;
    assert(z_binary.to_string() == int_to_binary(z));
  }
  cout << "check_powMod() is OK\n";
}

void check_mod() {
  int x = 16;
  int y = 3;
  BigNumWithInteger x_binary = BigNumWithInteger(int_to_binary(x));
  BigNumWithInteger y_binary = BigNumWithInteger(int_to_binary(y));
  assert(x_binary%y_binary == BigNumWithInteger(int_to_binary(x%y)));
  cout << "check_mod() is OK\n";
}

void check_plus() {
  BigNumWithInteger a;
  BigNumWithInteger b(5);
  BigNumWithInteger c = a + b;
  assert(c.to_string() == int_to_binary(5));

  for(int test = 0; test < 10; test++) {
    int x = rand()%(MAX/10); x = rand()%2 ? x : -x;
    int y = rand()%(MAX/10); y = rand()%2 ? y : -y;
    int z = x + y;

    BigNumWithInteger x_binary = BigNumWithInteger(int_to_binary(x));
    BigNumWithInteger y_binary = BigNumWithInteger(int_to_binary(y));
    BigNumWithInteger z_binary = x_binary + y_binary;

    assert(int_to_binary(z) == z_binary.to_string());
  }

  cout << "check_plus() is OK\n";
}

void check_subtract() {
  for(int test = 0; test < 10; test++) {
    int x = rand()%(MAX/10); 
    int y = rand()%(MAX/10);
    BigNumWithInteger x_binary = BigNumWithInteger(int_to_binary(x));
    BigNumWithInteger y_binary = BigNumWithInteger(int_to_binary(y));
    BigNumWithInteger z_binary = x_binary - y_binary;
    assert(int_to_binary(x-y) == z_binary.to_string());
  }

  cout << "check_subtract() is OK\n";
}

void check_mul() {
  for(int test = 0; test < 10; test++) {
    int x = rand()%((int)sqrt(MAX)); x = rand()%2 == 1 ? x : -x; 
    int y = rand()%((int)sqrt(MAX)); y = rand()%2 == 1 ? y : -y; 
    BigNumWithInteger x_binary = BigNumWithInteger(int_to_binary(x));
    BigNumWithInteger y_binary = BigNumWithInteger(int_to_binary(y));
    BigNumWithInteger z_binary = x_binary * y_binary;
    assert(int_to_binary(x*y) == z_binary.to_string());
  }

  cout << "check_mul() is OK\n";
}

void check_compare() {
  for(int test = 0; test < 10; test++) {
    int x = rand()%(MAX/10); x = rand()%2 ? x : -x;
    int y = rand()%(MAX/10); y = rand()%2 ? y : -y;


    BigNumWithInteger x_binary = BigNumWithInteger(int_to_binary(x));
    BigNumWithInteger y_binary = BigNumWithInteger(int_to_binary(y));

    if(x < y) assert(x_binary < y_binary);
    if(x > y) assert(x_binary > y_binary);
    if(x == y) assert(x_binary == y_binary);
  }
  cout << "check_compare() is OK\n";
}

void shift_left() {
  for(int test = 0; test < 10; test++) {
    int x = rand()%(MAX/100); x = rand()%2 ? x : -x;
    int y = rand()%(6); 
    int z = x << y;

    BigNumWithInteger x_binary = BigNumWithInteger(int_to_binary(x));
    BigNumWithInteger z_binary = x_binary << y;
  
    assert(int_to_binary(z) == z_binary.to_string());
  }
  cout << "shift_left() is OK\n";
}

void shift_right() {
  for(int test = 0; test < 10; test++) {
    int x = rand()%(MAX);
    int y = rand()%(10); 
    int z = x >> y;

    BigNumWithInteger x_binary = BigNumWithInteger(int_to_binary(x));
    BigNumWithInteger z_binary =  x_binary >> y;

    assert(int_to_binary(z) == z_binary.to_string());
  }
  cout << "shift_right() is OK\n";
}

int main() {
  srand(time(NULL));
  check_to_string();
  check_compare();
  check_plus();
  check_subtract();
  check_mod();
  check_mul();
  shift_left();
  shift_right();
  check_mulMod();
  check_powMod();
} 