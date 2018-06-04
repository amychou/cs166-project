#include "HashTable.h"
#include "PerfectlyBalancedTree.h"
#include "SplayTree.h"
#include "StdSetTree.h"
#include "WeightBalancedTree.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <stdexcept>
using namespace std;

namespace {
  /* Prompts the user for a string, returning whatever they typed in. */
  string getLine(const string& prompt) {
    cout << prompt;
    
    string result;
    getline(cin, result);
    return result;
  }
  
  /* Parses a string into a list of doubles, reporting an error if unble to do
   * so.
   */
  vector<double> parseLine(const string& line) {
    istringstream converter(line);
    
    /* Read the values, and confirm they're all positive. */
    vector<double> result;
    for (double elem; converter >> elem; ) {
      if (elem <= 0) throw runtime_error("All weights must be positive.");
      result.push_back(elem);
    }
    
    /* Make sure that there isn't anything left over. */
    converter.clear();
    for (char ch; converter >> ch; ) throw runtime_error("Couldn't parse all values.");
    return result;
  }
  
  /* Prompts the user for a list of weights to give to the keys. */
  vector<double> readWeights() {
    cout << "Enter a list of weights to use in the tree. The weights will be associated" << endl;
    cout << "with the keys 0, 1, 2, ..., n - 1." << endl;
    while (true) {
      try {
        string line = getLine("");
        return parseLine(line);
      } catch (const exception& e) {
        cout << e.what() << endl;
      }
    }
  }
  
  /* Converts a string to an integer, reporting an error if the conversion
   * can't be done.
   */
  int toInt(const string& str) {
    istringstream converter(str);
    
    int result;
    if (converter >> result, !converter) {
      throw runtime_error("Couldn't interpret that as an integer.");
    }
    
    /* Make sure there's nothing left. */
    for (char ch; converter >> ch; ) throw runtime_error("Couldn't interpret that as an integer.");
    
    return result;
  }
  
  template <typename BST> void exploreTree() {
    auto weights = readWeights();
    cout << endl;
    
    cout << "Building tree... " << flush;
    BST tree{weights};
    cout << "done!" << endl << endl;
    
    /* Let the student make some queries! */
    while (true) {
      string input = getLine("Enter a key to search for (or ENTER if you're done): ");
      if (input == "") break;
      
      /* Treat the input as an integer. */
      try {
        bool present = tree.contains(toInt(input));
        cout << "  Is that key present? " << (present? "Yes" : "No") << endl;
      } catch (const exception& e) {
        cout << e.what() << endl;
      }
    }
    
    cout << endl;
  }
}

int main() {
  while (true) {
    cout << "Please select the tree you'd like to explore." << endl;
    cout << "  (W)eight-Balanced Tree" << endl;
    cout << "  (S)play Tree" << endl;
    cout << "Or just hit ENTER to quit." << endl;
    string tree = getLine("Your choice: ");
    
    if      (tree == "W" || tree == "w") exploreTree<WeightBalancedTree>();
    else if (tree == "S" || tree == "s") exploreTree<SplayTree>();
    else if (tree == "Q" || tree == "q" || tree == "") break;
    else cout << "Sorry, I didn't understand that." << endl;
  }
}
