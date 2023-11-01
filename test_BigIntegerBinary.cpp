#include "BigIntegerBinary.h"
#include "hex_binary.h"
#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <bitset>
#include <cassert>
using namespace std;

const int MOD = 1e9 + 7;
const int MAX = 1e9;

typedef long long ll;

void check_mulMod() {
  for(int test = 0; test < 10; test++) {
    int x = rand() % (MAX);
    int y = rand() % (MAX);
    int z = 1ll * x * y % MOD;
    BigIntegerBinary x_binary(int_to_binary(x));
    BigIntegerBinary y_binary(int_to_binary(y));
    BigIntegerBinary z_binary = x_binary.mulMod(y_binary, BigIntegerBinary(int_to_binary(MOD)));

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
    BigIntegerBinary x_binary(int_to_binary(x));
    BigIntegerBinary y_binary(int_to_binary(y));
    BigIntegerBinary z_binary = x_binary.powMod(y_binary, BigIntegerBinary(int_to_binary(MOD)));

    // cout << x_binary << " " << y_binary << " " << z_binary << endl;
    assert(z_binary.to_string() == int_to_binary(z));
  }
  cout << "check_powMod() is OK\n";
}

void check_mod() {
  int x = 16;
  int y = 3;
  BigIntegerBinary x_binary = BigIntegerBinary(int_to_binary(x));
  BigIntegerBinary y_binary = BigIntegerBinary(int_to_binary(y));
  assert(x_binary%y_binary == BigIntegerBinary(int_to_binary(x%y)));
  cout << "check_mod() is OK\n";
}

void check_plus() {
  for(int test = 0; test < 10; test++) {
    int x = rand()%(MAX/10); x = rand()%2 ? x : -x;
    int y = rand()%(MAX/10); y = rand()%2 ? y : -y;
    int z = x + y;
    BigIntegerBinary x_binary = BigIntegerBinary(int_to_binary(x));
    BigIntegerBinary y_binary = BigIntegerBinary(int_to_binary(y));
    BigIntegerBinary z_binary = x_binary + y_binary;

    assert(int_to_binary(z) == z_binary.to_string());
  }

  cout << "check_plus() is OK\n";
}

void check_subtract() {
  for(int test = 0; test < 10; test++) {
    int x = rand()%(MAX/10); 
    int y = rand()%(MAX/10);
    BigIntegerBinary x_binary = BigIntegerBinary(int_to_binary(x));
    BigIntegerBinary y_binary = BigIntegerBinary(int_to_binary(y));
    BigIntegerBinary z_binary = x_binary - y_binary;
    assert(int_to_binary(x-y) == z_binary.to_string());
  }

  cout << "check_subtract() is OK\n";
}

void check_compare() {
  int x = 5;
  int y = 3;
  BigIntegerBinary x_binary = BigIntegerBinary(int_to_binary(x));
  BigIntegerBinary y_binary = BigIntegerBinary(int_to_binary(y));
  assert(x_binary > y_binary);
  cout << "check_compare() is OK\n";
}

int main() {
  srand(time(NULL));
  check_compare();
  check_plus();
  check_subtract();
  check_mod();
  check_mulMod();
  check_powMod();
} 