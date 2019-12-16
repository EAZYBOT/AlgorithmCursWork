#include "PolyPhaseSort.h"

void print_file(const string& file_name) {
	fstream f;
	f.open(file_name, ios::in | ios::binary);

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

void select(vector<int>& a, vector<int>& d, const int& n_files, int& j, int& level)
{
	if (d[j] < d[j + 1]) {
		j++;
	}
	else {
		if (d[j] == 0) {
			level++;
			int z = a[0];
			for (int i = 0; i < n_files - 1; i++) {
				d[i] = z + a[i + 1] - a[i];
				a[i] = z + a[i + 1];
			}
		}
		j = 0;
	}
	d[j]--;
}

void poly_phase_sort(const string& file_name, const unsigned int& n_files)
{
	if (n_files < 3) {
		return;
	}

	fstream f0;
	vector<fstream> f(n_files);
	vector<int> t(n_files);
	vector<int> ta(n_files);
	vector<Record> buffer(n_files);

	vector<int> a(n_files, 0), d(n_files, 0);
	for (unsigned int i = 0; i < n_files  - 1; i++) {
		a[i] = 1; d[i] = 1;
	}

	int level = 1, j = 0;

	f0.open(file_name, ios::in | ios::binary);
	for (int i = 0; i < n_files - 1; i++) {
		f[i].open("f_" + to_string(i + 1), ios::out | ios::binary | ios::trunc);
	}

	Record buf_0;
	f0.read((char*)&buf_0, sizeof(Record));
	do {
		select(a, d, n_files, j, level);

		while (!f0.eof() && buffer[j].key <= buf_0.key) {
			buffer[j] = buf_0;

			f0.read((char*)&buf_0, sizeof(Record));

			f[j].write((char*)&buffer[j], sizeof(Record));
		}
	} while (j < (n_files - 2) && !f0.eof());

	while (!f0.eof()) {
		select(a, d, n_files, j, level);

		if (buffer[j].key <= buf_0.key) {
			do {
				buffer[j] = buf_0;

				f0.read((char*)&buf_0, sizeof(Record));

				f[j].write((char*)&buffer[j], sizeof(Record));
			} while (!f0.eof() && buffer[j].key <= buf_0.key);

			if (f0.eof()) {
				d[j]++;
			}
			else {
				do {
					buffer[j] = buf_0;

					f0.read((char*)&buf_0, sizeof(Record));

					f[j].write((char*)&buffer[j], sizeof(Record));
				} while (!f0.eof() && buffer[j].key <= buf_0.key);
			}
		}
		else {
			do {
				buffer[j] = buf_0;

				f0.read((char*)&buf_0, sizeof(Record));

				f[j].write((char*)&buffer[j], sizeof(Record));
			} while (!f0.eof() && buffer[j].key <= buf_0.key);
		}
	} 

	f0.close();
	for (int i = 0; i < n_files - 1; i++) {
		f[i].close();
		f[i].open("f_" + to_string(i + 1), ios::in | ios::binary);
		f[i].read((char*)&buffer[i], sizeof(Record));
	}
	f[n_files - 1].open("f_" + to_string(n_files), ios::out | ios::binary | ios::trunc);

	for (int i = 0; i < n_files; i++) {
		t[i] = i;
	}

	do {
		int z = a[n_files - 2];
		d[n_files - 1] = 0;

		do {
			int k = 0;
			for (int i = 0; i < n_files - 1; i++) {
				if (d[i] > 0) {
					d[i]--;
				}
				else {
					k++;
					ta[k - 1] = t[i];
				}
			}

			if (k == 0) {
				d[n_files - 1]++;
			}
			else {
				int af= k;
				int ao = af;

				do {
					Record min = buffer[ta[0]];
					int save_index = 0;
					
					for (int i = 1; i < ao; i++) {
						if (min.key > buffer[ta[i]].key) {
							min = buffer[ta[i]];
							save_index = i;
						}
					}

					f[ta[save_index]].read((char*)&buffer[ta[save_index]], sizeof(Record));

					f[t[n_files - 1]].write((char*)&min, sizeof(Record));

					if (f[ta[save_index]].eof()) {
						af--; ao--;

						swap(ta[save_index], ta[ao]);
						swap(ta[ao], ta[af]);
					}
					else if (min.key > buffer[ta[save_index]].key) {
						ao--;
						swap(ta[save_index], ta[ao]);
					}

				} while (ao > 0);
			}

			z--;
		} while (z > 0);

		f[t[n_files - 1]].close();
		f[t[n_files - 1]].open("f_" + to_string(t[n_files - 1] + 1), ios::in | ios::binary);
		f[t[n_files - 1]].read((char*)&buffer[t[n_files - 1]], sizeof(Record));

		int tn = t[n_files - 1];
		int dn = d[n_files - 1];
		z = a[n_files - 2];

		for (int i = n_files - 1; i > 0; i--) {
			t[i] = t[i - 1];
			d[i] = d[i - 1];
			a[i] = a[i - 1] - z;
		}

		t[0] = tn;
		d[0] = dn;
		a[0] = z;

		f[t[n_files - 1]].close();
		f[t[n_files - 1]].open("f_" + to_string(t[n_files - 1] + 1), ios::out | ios::binary | ios::trunc);

		level--;
	} while (level > 0);

	for (int i = 0; i < n_files; i++) {
		f[i].close();
	}

	cout << "f_" + to_string(t[0] + 1) << endl;
}
