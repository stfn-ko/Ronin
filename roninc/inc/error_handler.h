#pragma once

#include <iostream>
#include <string>

#define COLOR_RESET "\e[0m"
#define BOLD_RED "\e[1;31m"
#define BOLD_GREEN "\e[1;32m"
#define BOLD_YELLOW "\e[1;33m"

typedef enum {
  UNDEF_ERR = 0,
  SYNTAX_ERR,
  LOGICAL_ERR,
  RUNTIME_ERR,
  TLE_ERR // Time Limit Exceeded
} errc;

typedef struct {
  size_t ln;
  size_t col;
} position;

typedef struct {
  std::string context;
  std::string msg;
  errc code;
  position pos;
} error_t;

auto mkerr(const char *context, const char *message, errc error_code,
           position position) -> error_t;
void error(bool condition, std::string msg);
void warning(bool condition, std::string msg);
void report(bool condition, error_t err);