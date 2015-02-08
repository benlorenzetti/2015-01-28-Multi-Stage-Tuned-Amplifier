// csv_import.h
// compile for C++11: -std=c++0x

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstring>
using namespace std;

const int MAX_CSV_COLUMNS = 10;
const int MAX_CSV_LINES = 1000;
const int MAX_LINE_LENGTH = 2000;

class csv_import {
public:
	csv_import (const char *);
		// @param: filename.csv
	int get_number_of_columns ();
	int get_number_of_data_lines ();
	int get_vector_title (int, char *, int);
		// @param: column number
		// @param: character array, to be filled with title
		// @param: max array size
		// return: filled array size
	int get_vector_data (int, double *, int);
		// @param: column number
		// @param: double array, to be filled with data
		// @param: max array size
		// return: filled array size
	void print ();
private:
	int number_of_lines, number_of_columns;
	vector<string> titles;
	vector<double> data;
};

csv_import::csv_import (const char *filename) {
	// Initialize Memebers
	number_of_lines = 0;
	number_of_columns = 0;

	// Open File
	fstream fs;
	fs.open (filename, ios::in);
	if (!fs.is_open ()) {
		cerr << "Error: could not open " << filename << endl;
	}

	// Read file line-by-line
	char current_line[MAX_LINE_LENGTH];
	string line;
	while (fs.good ()) {
		fs.getline (current_line, MAX_LINE_LENGTH);
		line = current_line;
		if (line.size() == 0) {
			// Protection against empty lines
			continue;
		}
		number_of_lines++;	
		if (number_of_lines == 1) {
			// The first line are column names
			int prev_pos = 0;
			int pos = 0;
			for (pos=0; pos< line.size (); pos++) {
				if (line.at (pos) == ',' || line.at (pos) == '\t') {
					number_of_columns++;
					titles.push_back (line.substr (prev_pos, pos-prev_pos));
					prev_pos = pos + 1;
				}
			} 
			titles.push_back (line.substr (prev_pos));
			number_of_columns++;
		} else {
			// The remaining lines are data
			int prev_pos = 0;
			int pos = 0;
			for (pos=0; pos< line.size (); pos++) {
				if (line.at (pos) == ',' || line.at (pos) == '\t') {
					if (pos-prev_pos <= 1) {
						data.push_back (0);
					} else {
						data.push_back ( stod (line.substr (prev_pos, pos-prev_pos)));
					}
					prev_pos = pos + 1;
				}
			}
			if (pos-prev_pos <= 1) {
				data.push_back (0);
			} else {
				data.push_back ( stod (line.substr (prev_pos)));
			}
		}//end if-else
	}// end line-by-line loop

	// Close File
	fs.close ();
}// end constructor

int csv_import::get_number_of_columns () {
	return number_of_columns;
}

int csv_import::get_number_of_data_lines () {
	return number_of_lines;
}

int csv_import::get_vector_title (int vector, char *title, int max_size) {
	if (vector > number_of_columns - 1) {
		cout << "Error in get_vector_title(): vector " << vector << " requested, only "
			<< number_of_columns << " available.\n";
		*title = 0;
		return 0;
	}
	strncpy (title, titles.at(vector).c_str (), max_size);
	*(title + max_size -1) = 0;
	return (max_size > titles.at(vector).size())? titles.at(vector).size() : max_size;
}

int csv_import::get_vector_data (int vector, double *vector_data, int max_size) {
	if (vector > number_of_columns - 1) {
		cout << "Error in get_vector_data(): vector " << vector << " requested, only "
			<< number_of_columns << " available.\n";
		return 0;
	}
	int size = (number_of_lines - 1 < max_size)? number_of_lines - 1 : max_size;
	for (int i=0; i<size; i++) {
		*(vector_data + i) = data.at (vector + i * number_of_columns);
	}
	return size;
}

void csv_import::print () {
	cout << "number of columns = " << number_of_columns << " = " << titles.size() << endl;
	vector<int> col_widths;
	for (int i=0; i< titles.size(); i++) {
		col_widths.push_back (2 + titles.at (i).size ());
		cout << setw (col_widths.at (i)) << titles.at (i);
	} cout << endl;
	for (int j=0; j< number_of_lines-1; j++) { // # of data rows = # of lines - 1 line of titles
		for (int i=0; i< number_of_columns; i++) {
			cout << setw (col_widths.at (i)) << data.at (i + j * number_of_columns);
		} cout << endl;
	}
}
