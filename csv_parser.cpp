/*
Your code is to read in a single CSV and output a series of files that are run length encoded (https://en.wikipedia.org/wiki/Run-length_encoding), where each output file corresponds to an attribute (column) from the input file. The output files should be named [inputfilename]-[attributename]-[attributenumber].csv and written to the same directory as the input files. The output files will have two values per line. The first value will be the value from the original file, the second value is the number of times that value appears consecutively.  The values should be comma delimited. The number of lines per file will vary with the level of repetition.
*/

#include <fstream>
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
  ifstream file(argv[1]);
  string str;
  getline(file,str);
  while (getline(file, str)) {
    cout << str << '\n';
  }

  return 0;
}
