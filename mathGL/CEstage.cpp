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
	csv_import import_1 ("CEstage.csv"); // the SPICE simulation
	csv_import import_2 ("ceFrequencyResponse.csv"); // experimental data

	// Create double arrays for the SPICE data
	int spice_size = import_1.get_number_of_data_lines () -1;
	double *spice_freq = new double[spice_size];
	double *spice_mag = new double[spice_size];
	double *spice_phase = new double[spice_size];

	// Create double arrays for the experimental data
	int experimental_size = import_2.get_number_of_data_lines () -1;
	double *exp_freq = new double [experimental_size];
	double *exp_mag = new double [experimental_size];
	double *exp_phase = new double [experimental_size];

	// Import data into the double arrays
	import_1.get_vector_data (0, spice_freq, spice_size);
	import_1.get_vector_data (1, spice_mag, spice_size);
	import_1.get_vector_data (2, spice_phase, spice_size);
	import_2.get_vector_data (0, exp_freq, experimental_size);
	import_2.get_vector_data (5, exp_mag, experimental_size);
	import_2.get_vector_data (6, exp_phase, experimental_size);

	// Put the data into mathGL data objects
	mglData f1, m1, p1, f2, m2, p2;
	f1.Set (spice_freq, spice_size);
	m1.Set (spice_mag, spice_size);
	p1.Set (spice_phase, spice_size);
	f2.Set (exp_freq, experimental_size);
	m2.Set (exp_mag, experimental_size);
	p2.Set (exp_phase, experimental_size);

	// Set some graphing option values
	const int MIN_FREQ = 100000;
	const int MAX_FREQ = 100000000;
	const char GRAPH_FILENAME[] = "ceStage.eps";
	const char PLOT_TITLE[] = "Common-Emitter Stage Frequency Response";

	// Generate Plots and Save as EPS vector file
	mglGraph gr;
	gr.SubPlot (1,2,0);
		gr.Title (PLOT_TITLE);
		gr.Label('y', "Magnitude (dB)", 0);
		gr.SetRanges (MIN_FREQ, MAX_FREQ, -40, 40);
		gr.SetFunc ("lg(x)","");	// use logarithmic scale
		gr.Grid ("xy","k"); 		// k for black colored grid
		gr.Label('x', "Frequency (Hz)", 0);
		gr.Axis ();
		gr.AddLegend ("SPICE Simulation","b2");
		gr.AddLegend ("Experimental Data", "rs");
		gr.Legend (0, .5);
		gr.Plot (f1, m1, "b2");		// b for blue, 2 for thickness
		gr.Plot (f2, m2, "rs");
		
	gr.SubPlot (1,2,1);
		gr.Label('y', "Phase (radians)", 0);
		gr.SetRanges (MIN_FREQ, MAX_FREQ, -3, 3);
		gr.SetFunc ("lg(x)","");	// use logarithmic scale
		gr.Grid ("xy","k"); 		// k for black colored grid
		gr.Label('x', "Frequency (Hz)", 0);
		gr.Axis ();
		gr.Legend(0,0.25);	// positioning
		gr.Plot (f1, p1, "b2");
		gr.Plot (f2, p2, "rs");
	gr.WriteEPS (GRAPH_FILENAME);

	cout << "Graphing complete. Output written to " << GRAPH_FILENAME << endl;
	return 0;
}
