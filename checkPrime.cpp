#include<iostream>
#include<vector>
#include <cstdlib> // for rand() and srand()
#include <ctime> // for time()
#include <algorithm>
#include "hex_binary.h"
#include "BigIntegerBinary.h"	
using namespace std;

bool test(BigIntegerBinary &a, BigIntegerBinary &n) {
    BigIntegerBinary n_minus_1 = n - BigIntegerBinary("1");
    BigIntegerBinary d = n_minus_1;
    int s = 0;
    while(d[0] == 0) {
        d >> 1;
        s++;
    }
    /// n - 1 = 2^s * d

    // cout << "d = " << d << endl;
    // cout << "s = " << s << endl;        
    // cout << "a = " << a << endl;
    BigIntegerBinary x = a.powMod(d, n);
    // cout << "x = " << x << endl;
    if(x == BigIntegerBinary("1") || x == n_minus_1) {
        return true;
    }

    bool check = false;
    for(int r = 1; r <= s; r++) {
        x = x.mulMod(x, n);
        if(x == BigIntegerBinary("1")) {
            check = true;
            break;
        }
        if(x == n_minus_1) {
            check = true;
            break;
        }
        // cout << "x = " << x << endl;

    }

    if(!check) {
        return false;
    }
    return true;
}

bool MillerRabin(BigIntegerBinary n, vector<int> primes, int k = 5) {
    BigIntegerBinary n_minus_1 = n - BigIntegerBinary("1");
    for(int i = 0; i <= 15; i++) {
        cerr << primes[i] << endl;
        BigIntegerBinary a = BigIntegerBinary(int_to_binary(primes[i]));
        if(a >= n) {
            return true;
        }
        if(!test(a, n)) {
            return false;
        }
    }

    while(k--) {
        int prime = primes[rand() % primes.size()];
        cerr << prime << endl;
        string prime_binary = int_to_binary(prime);
        BigIntegerBinary a(prime_binary);
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
    vector<int> primes = sangnt(1000);

    string number_hex;
    cin >> number_hex;
    reverse(number_hex.begin(), number_hex.end());
    
    string number_binary = hex_to_bin(number_hex);
    // cout << number_binary << endl;
    BigIntegerBinary number(number_binary);
    cout << MillerRabin(number, primes) << endl;
}