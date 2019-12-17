#include "TestModule.h"


void print_file(const string& file_name) {
	fstream f;
	f.open(file_name, ios::in | ios::binary);

	if (!f.is_open()) {
		return;
	}

	Record i;
	while (true) {
		f.read((char*)&i, sizeof(Record));
		if (f.eof()) {
			break;
		}

		cout << i.key << endl;

	}
	f.close();
}

void generate_file(int n_records, const string& file_name)
{
	mt19937 generator(time(0));

	fstream f0;
	f0.open(file_name, ios::out | ios::binary | ios::trunc);

	for (int i = 0; i < n_records; i++) {
		Record out_rec;
		out_rec.key = generator() % 100000;

		for (int j = 0; j < DATA_SIZE - 1; j++) {
			out_rec.data[j] = generator() % ('z' - 'a') + 'a';
		}
		out_rec.data[DATA_SIZE - 1] = '\0';

		f0.write((const char*)&out_rec, sizeof(Record));
	}

	f0.close();
}

bool file_is_sorting(const string& file_name)
{
	bool result = true;

	fstream f;
	f.open(file_name, ios::in | ios::binary);

	if (!f.is_open()) {
		return false;
	}

	Record i, j;
	while (true) {
		f.read((char*)&i, sizeof(Record));
		if (f.eof()) {
			break;
		}

		if (j.key > i.key) {
			result = false;
			break;
		}

		j = i;
	}
	f.close();

	return result;
}
