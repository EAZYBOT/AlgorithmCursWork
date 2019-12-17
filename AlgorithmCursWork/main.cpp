#include <iostream>
#include <chrono>
#include <cstdlib>
#include <conio.h>

#include "PolyPhaseSort.h"
#include "TestModule.h"

using namespace std;

void print_menu() {
	cout << "Main Menu" << endl << endl;
	cout << "1) Generate file" << endl;
	cout << "2) Sort file" << endl;
	cout << "3) Check file" << endl;
	cout << "4) Print file" << endl << endl;
	cout << "0) Exit" << endl << endl;

}

int main() {
	bool is_running = true;

	char choice;
	string file_name;
	int n_records;
	int n_files;
	while (is_running)
	{
		system("cls");

		print_menu();

		choice = _getch();

		system("cls");

		switch (choice)
		{
		case '1':
			cout << "Enter a file name and number of records: " << endl;
			cin >> file_name >> n_records;

			system("cls");

			generate_file(n_records, file_name);

			cout << "Successful" << endl;
			system("pause");
			break;

		case '2':
			cout << "Enter file name and number of ways:" << endl;
			cin >> file_name >> n_files;

			system("cls");
			{
				string sort_file_name;
				auto start = chrono::steady_clock::now();
				poly_phase_sort(file_name, ++n_files, sort_file_name);
				auto end = chrono::steady_clock::now();
				auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);

				cout << "File: " + sort_file_name << endl;
				cout << "Time: " << elapsed.count() / 1000.0 << " sec." << endl;
			}
			
			system("pause");
			break;

		case '3':
			cout << "Enter file name:" << endl;
			cin >> file_name;

			system("cls");

			if (file_is_sorting(file_name)) {
				cout << "Correct. File is sorted." << endl;
			}
			else {
				cout << "Incorrect. File is not sorted." << endl;
			}

			system("pause");
			break;

		case '4':
			cout << "Enter file name:" << endl;
			cin >> file_name;

			system("cls");

			print_file(file_name);

			system("pause");
			break;

		case '0':
			cout << "Do you want to exit? (y/n)" << endl;
			choice = _getch();
			if (choice == 'y') {
				is_running = false;
			}
			break;

		default:
			cout << "Incorrect input!" << endl;
			system("pause");
			break;
		}

	}

	return 0;
}