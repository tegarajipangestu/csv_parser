/*
Your code is to read in a single CSV and output a series of files that are run length encoded (https://
en.wikipedia.org/wiki/Run-length_encoding), where each output file corresponds to an attribute (column)
from the input file. The output files should be named [inputfilename]-[attributename]-[
attributenumber].csv and written to the same directory as the input files. The output files will have two
values per line. The first value will be the value from the original file, the second value is the number
of times that value appears consecutively.  The values should be comma delimited. The number of lines per
file will vary with the level of repetition.
*/

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <algorithm>

#define PREPARATION_PHASE 1
#define READ_WITHOUT_QUOTES 2
#define READ_WITH_SINGLE_QUOTE 3
#define READ_WITH_DOUBLE_QUOTE 4
#define AFTER_PHASE 5
#define FINAL_STATE 6


using namespace std;


/*
CSV Parser using automata

current state             | input | next state
--------------------------------------
PREPARATION_PHASE         | "     | READ_WITH_DOUBLE_QUOTE
PREPARATION_PHASE         | '     | READ_WITH_SINGLE_QUOTE
PREPARATION_PHASE         | ,     | READ_WITHOUT_QUOTES
READ_WITH_SINGLE_QUOTE    | '     | AFTER_PHASE
READ_WITH_DOUBLE_QUOTE    | "     | AFTER_PHASE
READ_WITHOUT_QUOTES       | ,     | PREPARATION_PHASE
READ_WITH_DOUBLE_QUOTE    | .     | READ_WITH_DOUBLE_QUOTE
READ_WITH_SINGLE_QUOTE    | .     | READ_WITH_SINGLE_QUOTE
READ_WITHOUT_QUOTES       | .     | READ_WITHOUT_QUOTES
AFTER_PHASE               | ,     | PREPARATION_PHASE
*/

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss

string get_current_date_time() {
    time_t t = time(0);
    struct tm * now = localtime(&t);
    ostringstream result;

    result << (now->tm_year + 1900) << '-' << (now->tm_mon+1) << '-' << (now->tm_mday);
    return result.str();
}

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
  while (getline(ss, token, ',')) {
    token.erase(remove(token.begin(), token.end(), '\r'), token.end());
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
          word.erase(remove(word.begin(), word.end(), '\r'), word.end());
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
          throw "Malformed CSV";
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
      int counter = 1;
      for (unsigned int j = 0 ; j<contents.size()-1 ; j++) {
        string current_value = contents[j][i];
        string next_value = contents[j+1][i];

        if (current_value == next_value) {
          counter++;
        }
        else {
          output_file << contents[j][i] +","+ int_to_string(counter) << endl;
          counter = 1;
        }
      }
      if (contents[contents.size()][i] != contents[contents.size()-1][i]) {
        counter = 1;
      }
      output_file << contents[contents.size()][i] +","+ int_to_string(counter) << endl;
      output_file.close();
    }
  }
  catch (string s) {
    cout << s << '\n';
    return -1;
  }

  ofstream log ((get_current_date_time()+".log").c_str());
  log << "Running program at "+file_name << endl;
  log << "Processed "+int_to_string(contents.size())+" rows" << endl;
  log << "Time elapsed :";
  log << elapsed_secs;
  log << " s" << endl;
  return 0;
}
