#pragma once

#include <fstream>
#include <memory>

#include "error_handler.h"

auto valid_extension(const std::string &file_name) -> bool;
auto read_file(const std::string &file_path) -> std::unique_ptr<std::string>;