// auto run main.exe test/test_00.inp test/test_00.ans
// to       main.exe test/test_19.inp test/test_19.ans

// Path: checkPrime.cpp

//code
#include <bits/stdc++.h>
#include "BigIntegerBinary.h"
using namespace std;

int main() {
  string Path = "test/test_";
  string inp = ".inp";
  string ans = ".ans";
  string out = ".out";
  int nTest = 20;
  for(int test = 0; test < nTest; test++) {
    stringstream inp, out;
    inp << Path << setw(2) << setfill('0') << test << ".inp";
    out << Path << setw(2) << setfill('0') << test << ".ans";

    int exitCode = system(("main.exe " + inp.str() + " " + out.str()).c_str());
    if(exitCode == 0) {
      cout << "Test " << test << " passed." << endl;
    }
    else {
      cout << "Test " << test << " failed." << endl;
    }
    cout << "------------------------------------" << endl;

  }
}