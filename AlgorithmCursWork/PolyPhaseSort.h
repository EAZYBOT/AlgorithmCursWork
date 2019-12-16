#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <random>
#include <ctime>
#include <vector>

#include "datatype.h"

using namespace std;

void print_file(const string& file_name);

void generate_file(int n_recods, const string& file_name);

void select(vector<int>& a, vector<int>& d, const int& n_files, int& j, int& level);

void poly_phase_sort(const string& file_name, const unsigned int& n_files);