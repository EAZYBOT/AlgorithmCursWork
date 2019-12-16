#include <iostream>
#include <chrono>

#include "PolyPhaseSort.h"

using namespace std;

int main() {
	print_file("f_3");
	generate_file(200000, "file_0");

	auto start = chrono::steady_clock::now();
	poly_phase_sort("file_0", 6);
	auto end = chrono::steady_clock::now();
	auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);

	cout << elapsed.count() / 1000.0 << endl;
	return 0;
}