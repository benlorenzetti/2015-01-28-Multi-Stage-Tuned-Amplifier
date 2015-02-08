// ngspice_txt_to_csv.cpp
/*
 *	Use this program to convert NGSPICE output data
 *	to Comma Separated Value (CSV) format. Enter the
 *	.txt filename to be converted as a command line
 *	arguement:
 *
 *	$ ngspice_txt_to_csv text_file_from_ngspice.txt
 *
 *	To compile, use C++ version 11:
 *
 *	$ g++ ngspice_ascii_write_to_csv.cpp -std=c++0x -o ngspice_txt_to_csv
 */

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
using namespace std;

int main(int argc, char** argv) {
	if (argc < 2) {
        	cerr << "Usage: " << argv[0] << " filename.txt" << std::endl;
		return 1;
	}

	// Open ngspice data file
	fstream in_file, out_file;
	in_file.open (argv[1], ios::in);
	if ( !in_file.is_open() ) {
		cerr << "Error: filename " << argv[1] << " could not be opened.\n";
		return -1;
	}

	// Open output data file
	char output_filename[1000];
	strcpy (output_filename, argv[1]);
	strcpy (output_filename + strlen(argv[1])-3, "csv");
	out_file.open (output_filename, ios::out | ios::trunc);
	if ( !out_file.is_open() ) {
		cerr << "Error: filename " << output_filename << " could not be created.\n";
		return -1;
	}

	// Skip the "Title", "Date", "Plotname", and "Flags" lines
	char char_1000[1000];
	for (int i=0; i<4; i++) {
		in_file.getline (char_1000, 1000);
		cout << char_1000 << endl;
	}

	// Get the "No. of Variables" Line
	in_file.getline (char_1000, 1000);
	string no_dot_variables_line = char_1000;
	size_t space_pos = no_dot_variables_line.rfind (' ');
	if (space_pos == string::npos) {
		cerr << "Error: no space was found in No. variables line\n";
		return -1;
	}
	int number_of_variables = stoi ( no_dot_variables_line.substr (space_pos));
	cout << "Number of variables: " << number_of_variables << endl;

	// Get the "No. of Points" Line
	in_file.getline (char_1000, 1000);
	string no_dot_points_line = char_1000;
	space_pos = no_dot_points_line.rfind (' ');
	if (space_pos == string::npos) {
		cerr << "Error: no space was found in No. variables line\n";
		return -1;
	}
	int number_of_points = stoi ( no_dot_points_line.substr (space_pos));
	cout << "Number of points: " << number_of_points << endl;

	// Skip the "Variables" Line
	in_file.getline (char_1000, 1000);
	cout << char_1000 << endl;

	// Get the variables names
	vector<string> variables;
	for (int i=0; i< number_of_variables; i++) {
		in_file.getline (char_1000, 1000);
		string current_line = char_1000;
		size_t first_tab = current_line.find ('\t', 0);
		size_t second_tab = current_line.find ('\t', first_tab+1);
		size_t third_tab = current_line.find ('\t', second_tab+1);
		variables.push_back (current_line.substr (second_tab+1, third_tab-second_tab-1));
		cout << "\t" << i << "\t" << variables.at (i) << endl;
	}

	// Skip the "Values" Line
	in_file.getline (char_1000, 1000);
	cout << char_1000 << endl;

	// Get the data
	vector<double> data;
	for (int i=0; i<number_of_points; i++) {
		cout << i;
		for (int j=0; j<number_of_variables; j++) {
			in_file.getline (char_1000, 1000);
			string current_line = char_1000;
			size_t first_tab = current_line.find ('\t', 0);
			size_t first_comma = current_line.find (',', first_tab+1);
			data.push_back (stod (current_line.substr (first_tab+1, first_comma-first_tab-1)));
			cout << '\t' << data.back();
		}
		in_file.getline (char_1000, 1000); // skip the blank line
		cout << endl;
	}

	// At this point all important information has been put into the vectors
	// Time to write the comma separated variables (csv) output
	for (int i=0; i<number_of_variables-1; i++) {
		out_file << variables.at (i) << ',';
	}	out_file << variables.at (number_of_variables-1) << endl;
	for (int j=0; j<number_of_points; j++) {
		for (int i=0; i<number_of_variables-1; i++) {
			out_file << data.at (j*number_of_variables + i) << ',';
		}	out_file << data.at (j*number_of_variables + number_of_variables-1) << endl;
	}
	
	// Close both files and return
	in_file.close ();
	out_file.close ();
	cout << "Program complete. Output written to " << output_filename << endl;
}


