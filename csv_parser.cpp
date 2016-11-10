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

vector<string> parse_attributes(string s) {
  vector<string> output;
  std::stringstream ss;
  ss.str(s);
  string token;
  while (std::getline(ss, token, ',')) {
    output.push_back(token);
  }
  return output;
}

vector< vector<string> > parse_values(string line) {
  vector< vector<string> > rows;
  string word = "";


  int i = 0;

  while (line[i] != '\0') {
    int state = PREPARATION_PHASE;
    vector<string> row;
    while (state != FINAL_STATE) {
      if (state == PREPARATION_PHASE) {
        if (line[i] == '\'') {
          state = READ_WITH_SINGLE_QUOTE;
        }
        else if (line[i] == '\"') {
          state = READ_WITH_DOUBLE_QUOTE;
        }
        else if (line[i] == ',') {
          row.push_back(word);
          word = "";
        }
        else if (line[i] == '\n') {
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
        else if ((state == READ_WITH_DOUBLE_QUOTE || state == READ_WITH_SINGLE_QUOTE) && line[i] == '\n') {
          word += ' ';
        }
        else if (state == READ_WITHOUT_QUOTES && (line[i] == ',' || line[i] == '\n')) {
          row.push_back(word);
          word = "";
          if (line[i] == ',') {
            state = PREPARATION_PHASE;
          }
          else if (line[i] == '\n') {
            state = FINAL_STATE;
          }
        }
        else if (state == READ_WITHOUT_QUOTES && (line[i] == '\'' || line[i] == '\"')) {
          cout << line[i-1] << " " << line[i] << endl;
          throw "Malformed CSV. You need to escape character at "+int_to_string(rows.size());
        }
        else {
          word += line[i];
        }
      }
      else if (state == AFTER_PHASE) {
        if (line[i] == '\n') {
          row.push_back(word);
          word = "";
          state = FINAL_STATE;
        }
        else if (line[i] == ',') {
          row.push_back(word);
          word = "";
          state = PREPARATION_PHASE;
        }
        else {
          throw string("Malformed CSV");
        }
      }
      else {
        throw string("Malformed CSV");
      }
      i++;
    }
    rows.push_back(row);
  }

  return rows;
}

string read_from_file(ifstream & file) {
  string output = "";
  char c;
  while (file.get(c)) {
    output += c;
  }
  return output;
}

int main(int argc, char *argv[])
{
  string str;
  string file_name(argv[1]);
  vector< vector<string> > contents;
  vector<string> attributes;
  double elapsed_secs;

  ifstream file(argv[1]);

  getline(file,str);
  try {
    vector<string> attributes = parse_attributes(str);
    str = read_from_file(file);
    clock_t begin = clock();
    contents = parse_values(str);
    clock_t end = clock();
    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    for (unsigned int i=0;i<attributes.size();i++) {
      ofstream output_file((file_name+"-"+attributes.at(i)+"-"+int_to_string(i)+".csv").c_str());
      for (vector< vector<string> >::iterator vec_it = contents.begin() ; vec_it != contents.end() ; ++vec_it) {
        output_file << (*vec_it).at(i) << endl;
      }
      output_file.close();
    }
  }
  catch (string s) {
    cout << s << '\n';
    return -1;
  }
  cout << "Processed "+int_to_string(contents.size())+" rows" << endl;
  cout << "Time elapsed :";
  cout << elapsed_secs;
  cout << " s\n";
  return 0;
}
