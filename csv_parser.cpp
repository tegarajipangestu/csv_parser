/*
Your code is to read in a single CSV and output a series of files that are run length encoded (https://en.wikipedia.org/wiki/Run-length_encoding), where each output file corresponds to an attribute (column) from the input file. The output files should be named [inputfilename]-[attributename]-[attributenumber].csv and written to the same directory as the input files. The output files will have two values per line. The first value will be the value from the original file, the second value is the number of times that value appears consecutively.  The values should be comma delimited. The number of lines per file will vary with the level of repetition.
*/

#include <fstream>
#include <string>
#include <vector>
#include <iostream>

using namespace std;


vector<string> parse_line_to_words(string line) {
  vector<string> words;
  int i = 0;
  bool escaped = false;
  string word = "";

  while (line[i] != '\0') {
    if (line[i] == ',' && !escaped) {
      escaped = false;
      words.push_back(word);
      // n_words++;
      word = "";
    }
    else if (line[i] == '\\'){
      escaped = true;
    }
    else {
      word += line[i];
    }
    i++;
  }
  words.push_back(word);
  return words;
}

int main(int argc, char *argv[])
{
  string row;
  string str;
  vector<string> words;

  ifstream file(argv[1]);
  getline(file,str);
  vector<string> attributes = parse_line_to_words(str);
  while (getline(file, row)) {
    words = parse_line_to_words(row);
    // cout << words.to_s;
  }

  for (vector<string>::iterator it = attributes.begin(); it != attributes.end(); ++it) {
    cout << *it << '\n';
  }

  for (vector<string>::iterator it = words.begin(); it != words.end(); ++it) {
    cout << *it << '\n';
  }
  return 0;
}
