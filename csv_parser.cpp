/*
Your code is to read in a single CSV and output a series of files that are run length encoded (https://en.wikipedia.org/wiki/Run-length_encoding), where each output file corresponds to an attribute (column) from the input file. The output files should be named [inputfilename]-[attributename]-[attributenumber].csv and written to the same directory as the input files. The output files will have two values per line. The first value will be the value from the original file, the second value is the number of times that value appears consecutively.  The values should be comma delimited. The number of lines per file will vary with the level of repetition.
*/

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>

using namespace std;


/*
CSV Parser using automata

1 : preparation phase
2 : value without quotes
3 : value with single quote
4 : value with double quote
6 : after phase
7 : final phase
*/

string int_to_string(int i) {
  ostringstream s;
  s << i;
  return s.str();
}

vector<string> parse_line_to_words(string line) {
  vector<string> words;
  int i = 0;
  int state = 1;
  string word = "";
  while (state != 7) {
    if (state == 1) {
      if (line[i] == '\'') {
        state = 3;
      }
      else if (line[i] == '\"') {
        state = 4;
      }
      else {
        state = 2;
        word += line[i];
      }
    }
    else if (state == 2 || 3 || 4) {
      if (line[i] == '\\') {
        i++;
        word += line[i];
      }
      else if (state == 3 && line[i] == '\'') {
        state = 6;
      }
      else if (state == 4 && line[i] == '\"') {
        state = 6;
      }
      else if (state == 2 && (line[i] == ',' || line[i] == '\0')) {
        if (line[i] == ',') {
          state = 1;
        }
        else if (line[i] == '\0') {
          state = 7;
        }
        words.push_back(word);
        word = "";
      }
      else if (line[i] == '\'' || line[i] == '\"') {
        throw "Malformed CSV. You need to escape character at "+int_to_string(i);
      }
      else {
        word += line[i];
      }
    }
    else if (state == 5) {
      word += line[i];
    }
    else if (state == 6) {
      if (line[i] == '\0') {
        state = 7;
      }
      else if (line[i] == ',') {
        state = 1;
      }
      else {
        throw "Malformed CSV";
      }
      words.push_back(word);
      word = "";
    }
    else {
      throw "Malformed CSV";
    }
    i++;
  }
  return words;
}

int main(int argc, char *argv[])
{
  string row;
  string str;
  vector<string> words;

  ifstream file(argv[1]);
  getline(file,str);
  try {
    vector<string> attributes = parse_line_to_words(str);
    while (getline(file, row)) {
      words = parse_line_to_words(row);
    }
    for (vector<string>::iterator it = attributes.begin(); it != attributes.end(); ++it) {
      cout << *it << '\n';
    }

    for (vector<string>::iterator it = words.begin(); it != words.end(); ++it) {
      cout << *it << '\n';
    }
  }
  catch (string s) {
    cout << s << '\n';
    return -1;
  }

  return 0;
}
