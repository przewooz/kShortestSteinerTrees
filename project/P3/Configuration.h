// Brian Goldman

// The Configuration class is used to
// store configurable options needed by the program.

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <unordered_map>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <algorithm>

using std::unordered_map;
using std::string;

#include "../IslandGA/BinaryCoding.h"
#include "../IslandGA/Evaluation.h"

class Configuration {
 public:

  Configuration() = default;
  virtual ~Configuration() = default;

  // Parses command line arguments to set up the configuration
  void parse(int argc, char * argv[]);
  // Parses a file, loading in key-value pairs
  void parse(char filename[]);
  // writes out the complete configuration in the same format as
  // input files
  void dump(std::ostream& out);

  // Template call to get a value using a key.  Uses template
  // specialization to automatically cast to different types.
  template<class T>
  T get(const string key);

  // Explicitly add some key value pairing to the configuraiton
  template<class T>
  void set(const string key, const T value) {
    data[key] = std::to_string(value);
  }

  void set(const string key, const string value) {
	  data[key] = value;
  }

  CEvaluation<CBinaryCoding>* get_injected_evaluation() {
	  return injected_evaluation;
  }

  void set_injected_evaluation(CEvaluation<CBinaryCoding>* injected_evaluation) {
	  Configuration::injected_evaluation = injected_evaluation;
  }

 private:
  unordered_map<string, string> data;
  CEvaluation<CBinaryCoding>* injected_evaluation;
};

#endif /* CONFIGURAITON_H_ */
