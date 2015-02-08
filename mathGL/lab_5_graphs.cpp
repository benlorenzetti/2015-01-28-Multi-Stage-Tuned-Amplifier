// lab_5_graphs.cpp
// $ g++ lab_5_graphs.cpp -std=c++0x -lmgl -o generate_graphs
// $ generate_graphs

#include <iostream>
#include "csv_import.h"
#include <math.h>
#include <mgl2/mgl.h>
#include <mgl2/data.h>
using namespace std;

int main () {
	csv_import import_1 ("default-parameters.csv"); // the initial SPICE simulation
	csv_import import_2 ("updated-parameters.csv"); // the improved SPICE simulation
	csv_import import_3 ("frequency-response-data.csv"); // the experimental data

	// Create double arrays for the SPICE data
	int spice_size = import_1.get_number_of_data_lines () -1;
	double *spice_freq = new double[spice_size];
	double *default_mag = new double[spice_size];
	double *default_phase = new double[spice_size];
	double *improved_mag = new double[spice_size];
	double *improved_phase = new double[spice_size];

	// Create double arrays for the experimental data
	int experimental_size = import_3.get_number_of_data_lines () -1;
	double *exp_freq = new double [experimental_size];
	double *exp_mag = new double [experimental_size];
	double *exp_phase = new double [experimental_size];

	// Import data into the double arrays
	import_1.get_vector_data (0, spice_freq, spice_size);
	import_1.get_vector_data (1, default_mag, spice_size);
	import_1.get_vector_data (2, default_phase, spice_size);
	import_2.get_vector_data (1, improved_mag, spice_size);
	import_2.get_vector_data (2, improved_phase, spice_size);
	import_3.get_vector_data (0, exp_freq, experimental_size);
	import_3.get_vector_data (1, exp_mag, experimental_size);
	import_3.get_vector_data (2, exp_phase, experimental_size);

	// Put the data into mathGL data objects
	mglData f1, m1, p1, m2, p2, f3, m3, p3;
	f1.Set (spice_freq, spice_size);
	m1.Set (default_mag, spice_size);
	p1.Set (default_phase, spice_size);
	m2.Set (improved_mag, spice_size);
	p2.Set (improved_phase, spice_size);
	f3.Set (exp_freq, experimental_size);
	m3.Set (exp_mag, experimental_size);
	p3.Set (exp_phase, experimental_size);

	// Set some graphing option values
	const int MIN_FREQ = 10;
	const int MAX_FREQ = 100000000;
	const char GRAPH_FILENAME[] = "comparison_graph.eps";
	const char PLOT_TITLE[] = "BJT Amplifier Circuit Frequency Response";

	// Generate Plots and Save as EPS vector file
	mglGraph gr;
	gr.SubPlot (1,2,0);
		gr.Title (PLOT_TITLE);
		gr.Label('y', "Magnitude (dB)", 0);
		gr.SetRanges (MIN_FREQ, MAX_FREQ, -20, 50);
		gr.SetFunc ("lg(x)","");	// use logarithmic scale
		gr.Grid ("xy","k"); 		// k for black colored grid
		gr.Label('x', "Frequency (Hz)", 0);
		gr.Axis ();
		gr.AddLegend ("Default SPICE Simulation","b2");
		gr.AddLegend ("Improved SPICE Simulation","g2");
		gr.AddLegend ("Experimental Data", "rs");
		gr.Legend (0, 1);
		gr.Plot (f1, m1, "b2");		// b for blue, 2 for thickness
		gr.Plot (f1, m2, "g2");
		gr.Plot (f3, m3, "rs");
		
	gr.SubPlot (1,2,1);
		gr.Label('y', "Phase (radians)", 0);
		gr.SetRanges (MIN_FREQ, MAX_FREQ, -4, 6);
		gr.SetFunc ("lg(x)","");	// use logarithmic scale
		gr.Grid ("xy","k"); 		// k for black colored grid
		gr.Label('x', "Frequency (Hz)", 0);
		gr.Axis ();
		gr.Legend(1,0.3);	// positioning
		gr.Plot (f1, p1, "b2");
		gr.Plot (f1, p2, "g2");
		gr.Plot (f3, p3, "rs");
	gr.WriteEPS (GRAPH_FILENAME);

	cout << "Graphing complete. Output written to " << GRAPH_FILENAME << endl;
	return 0;
}
