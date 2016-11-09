/*
Your code is to read in a single CSV and output a series of files that are run length encoded (https://en.wikipedia.org/wiki/Run-length_encoding), where each output file corresponds to an attribute (column) from the input file. The output files should be named [inputfilename]-[attributename]-[attributenumber].csv and written to the same directory as the input files. The output files will have two values per line. The first value will be the value from the original file, the second value is the number of times that value appears consecutively.  The values should be comma delimited. The number of lines per file will vary with the level of repetition.
*/

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>

#define PREPARATION_PHASE 1
#define READ_WITHOUT_QUOTES 2
#define READ_WITH_SINGLE_QUOTE 3
#define READ_WITH_DOUBLE_QUOTE 4
#define AFTER_PHASE 5
#define FINAL_STATE 6


using namespace std;


/*
CSV Parser using automata

1 : preparation phase
2 : value without quotes
3 : value with single quote
4 : value with double quote
5 : after phase
6 : final phase
*/

string int_to_string(int i) {
  ostringstream s;
  s << i;
  return s.str();
}

vector<string> parse_line_to_words(string line) {
  vector<string> words;
  int i = 0;
  int state = PREPARATION_PHASE;
  string word = "";
  while (state != FINAL_STATE) {
    if (state == PREPARATION_PHASE) {
      if (line[i] == '\'') {
        state = READ_WITH_SINGLE_QUOTE;
      }
      else if (line[i] == '\"') {
        state = READ_WITH_DOUBLE_QUOTE;
      }
      else if (line[i] == ',') {
        words.push_back(word);
        word = "";
      }
      else if (line[i] == '\0') {
        state = FINAL_STATE;
      }
      else {
        state = READ_WITHOUT_QUOTES;
        word += line[i];
      }
    }
    else if (state == READ_WITHOUT_QUOTES || state == READ_WITH_SINGLE_QUOTE || state == READ_WITH_DOUBLE_QUOTE) {
      if (line[i] == '\\') {
        i++;
        word += line[i];
      }
      else if (state == READ_WITH_SINGLE_QUOTE && line[i] == '\'') {
        state = AFTER_PHASE;
      }
      else if (state == READ_WITH_DOUBLE_QUOTE && line[i] == '\"') {
        state = AFTER_PHASE;
      }
      else if (state == READ_WITHOUT_QUOTES && (line[i] == ',' || line[i] == '\0')) {
        if (line[i] == ',') {
          state = PREPARATION_PHASE;
        }
        else if (line[i] == '\0') {
          state = FINAL_STATE;
        }
        words.push_back(word);
        word = "";
      }
      else if (state == READ_WITHOUT_QUOTES && (line[i] == '\'' || line[i] == '\"')) {
        throw "Malformed CSV. You need to escape character at "+int_to_string(i);
      }
      else {
        word += line[i];
      }
    }
    else if (state == AFTER_PHASE) {
      if (line[i] == '\0') {
        state = FINAL_STATE;
      }
      else if (line[i] == ',') {
        state = PREPARATION_PHASE;
      }
      else {
        throw string("Malformed CSV");
      }
      words.push_back(word);
      word = "";
    }
    else {
      throw string("Malformed CSV");
    }
    i++;
  }
  return words;
}

int main(int argc, char *argv[])
{
  string row;
  string str;
  vector< vector<string> > output;
  vector<string> words;
  int i = 1;

  ifstream file(argv[1]);
  getline(file,str);
  try {
    vector<string> attributes = parse_line_to_words(str);
    while (getline(file, row)) {
      words = parse_line_to_words(row);
      output.push_back(words);
      i++;
    }
    for (vector<string>::iterator it = attributes.begin(); it != attributes.end(); ++it) {
      cout << *it << '\n';
    }

    for (vector<string>::iterator it = words.begin(); it != words.end(); ++it) {
      cout << *it << '\n';
    }
  }
  catch (string s) {
    cout << "Error at line : "+int_to_string(i) << endl;
    cout << s << '\n';
    return -1;
  }
  cout << "Processed "+int_to_string(i) << endl;
  return 0;
}
