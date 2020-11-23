

#include <iostream>

#include "tutorial_helib.h"

using namespace std;

int main(int argc, char** argv)
{

  string arg = "";
  if (argc > 1) {
    arg = argv[1];
  }

  if (arg == "basics")
    tut_1_basics();
  else if (arg == "plaintexts")
    tut_2_plaintexts();
  else if (arg == "io")
    tut_3_io();
  else {
    cout << "Usage: tutorial_helib <tutName>" << endl
         << "\t<tutName> can be:" << endl
         << "\t\tbasics" << endl
         << "\t\tplaintexts" << endl
         << "\t\tio" << endl;
    exit(1);
  }
}