#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <ctime>

#include "datatype.h"

using namespace std;

void print_file(const string& file_name);

void generate_file(int n_recods, const string& file_name);

bool file_is_sorting(const string& file_name);