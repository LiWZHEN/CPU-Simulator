#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <iostream>

class InvalidFunction : public std::exception {
  const char *what() const throw() {
    return "Invalid function!";
  }
};

class InvalidAddress : public std::exception {
  const char *what() const throw() {
    return "Invalid address!";
  }
};

class InvalidInstruction : public std::exception {
  const char *what() const throw() {
    return "Invalid instruction!";
  }
};

class Nodata : public std::exception {
  const char *what() const throw() {
    return "No data here!";
  }
};

class NoMatchedType : public std::exception {
  const char *what() const throw() {
    return "No type matches!";
  }
};

class InvalidHexString : public std::exception {
  const char *what() const throw() {
    return "Invalid string of hex number!";
  }
};

#endif