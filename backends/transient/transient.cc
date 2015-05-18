// 1D Transient Sedimentation/Erosion
// Earth Surface Processes Research Group
// The University of Michigan
// Professor Todd Ehlers 
// Research Assistant Santosh Kumar
// Research Assistant Tehmasp Chaudhri

#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>

#include "getopt.h"

#undef __WIN32__

#define ERR_HEAP "ERROR: Heap Allocation Error"
#define ERR_FILE "ERROR: Could not open file(s) for output; do the directories exist?"

using namespace std;

const double secPerMilYears = 3.1536E7 * 1E6;
const char *versionString = "1D Transient Erosion Calculation - v1.0";

double interpl(const double arr1[], const int size, const double arr2[], const double num)
{
	int pos0 = 0,
      pos1 = 0;
  
	double slope = 0,
         ans = 0;  // 'ans' must be set to zero
  
	bool value = false; // 'value' must be set to false

	for(int i = 0; i < (size - 1); i++) {
    
		if((num >= arr1[i]) && (num <= arr1[i + 1])) {
			pos0 = i;
			pos1 = i + 1;
			i = size + 1;
			
    	value = true;
		}
	}

  if(value) {
    slope = (arr2[pos1] - arr2[pos0]) / (arr1[pos1] - arr1[pos0]);
		ans = (slope * (num - arr1[pos0])) + arr2[pos0];
  }
  
	return ans;
}

// This function prints the help menu to the
// command line. This function takes the name
// of the program 'argv[0]' as a parameter for
// 'filename'.
void printMenu(const char *fileName)
{
	cout<<"1D Transient Erosion Calculation\n";
	cout<<"Usage: "<<fileName<<" [OPTION]... [ARG]...\n\n";
	
	cout<<"OPTIONS:\n";
	cout<<"-T   surface temperature (celcius)\n";
	cout<<"-B   basal temperature gradient (basal flux/thermal cond.)\n";
	cout<<"-z   maximum depth (meters)\n";
	cout<<"-a   thermal diffusivity (k/rho*Cp)\n";
	cout<<"-r   erosion rate (mm/yr)\n";
	cout<<"-o   temperature history (million years output time)\n\n";
	
	cout<<"-d   geotherm 1 (million years)\n";
	cout<<"-e   geotherm 2 (million years)\n";
	cout<<"-f   geotherm 3 (million years)\n";
	cout<<"-g   geotherm 4 (million years)\n\n";
	
	cout<<"-i   isotherm 1 (celcius)\n";
	cout<<"-j   isotherm 2 (celcius)\n";
	cout<<"-k   isotherm 3 (celcius)\n";
	cout<<"-l   isotherm 4 (celcius)\n\n";
	
	cout<<"-h   display this help menu\n";
	cout<<"-v   display the version information\n";
}


int main(int argc, char *argv[])
{
  extern char *optarg;    // getopt() external variable
  int option;
  
  // program variables with default values. these variables are those
  // that can be set by the user via the commnad line.
	
  double zmax = 50000;		// maximum depth (meters)
  double To = 0;					// surface temperature (celcius)
	double Gb = 0.025; 			// basal temperature gradient (basal flux / thermal cond.)
	double a = 1.3636E-6;		// thermal diffusivity (k / rho * Cp)
	
	double rate = -1 * -0.5;      	// erosion rate
	double v = -0.5 / 3.1536E10;		// velocity used for solution
	
	double temp_hist = 10; 		// FIXME, make this option a spinBox in the GUI
	
	double t[4] = {0 * secPerMilYears, 5 * secPerMilYears, 10 * secPerMilYears, 15 * secPerMilYears};
  double isotherm[4] = {60, 120, 250, 350};

  // parse all the command line options. if there are any bad command line arguments
  // the program will exit after prompting the user.
  while((option = getopt(argc, argv, "hvz:T:B:a:d:e:f:g:r:i:j:k:l:o:")) != -1) {
    switch(option) {
      
      case 'z':
        zmax = atof(optarg);
        break;
      
      case 'T':
			  To = atof(optarg);
        break;
      
      case 'B':
        Gb = atof(optarg);
        break;
      
      case 'a':
      	a = atof(optarg);
        break;
      
     	case 'd':
				t[0] = atof(optarg) * secPerMilYears;
        break;
      
     	case 'e':
				t[1] = atof(optarg) * secPerMilYears;
        break;	
      
     	case 'f':
				t[2] = atof(optarg) * secPerMilYears;
				break;
      
     	case 'g':
				t[3] = atof(optarg) * secPerMilYears;
        break;
      
     	case 'r':
				rate = -1 * atof(optarg);
				v = atof(optarg) / 3.1536E10;
        break;
      
    	case 'i':
      	isotherm[0] = atof(optarg);
        break;
      
     	case 'j':
      	isotherm[1] = atof(optarg);
       	break;
      
     	case 'k':
				isotherm[2] = atof(optarg);
       	break;
      
     	case 'l':
      	isotherm[3] = atof(optarg);
        break;
      
     	case 'o':
      	temp_hist = round(atof(optarg));
        break;
			
     	case 'h':
				printMenu(argv[0]);
			  exit(EXIT_SUCCESS);
			
     	case 'v':
			  cout<<versionString<<endl;
				exit(EXIT_SUCCESS);

     	case ':':
     	case '?':
     		cerr<<"Try `"<<argv[0]<<" -h' for more information.\n";
		   	exit(EXIT_FAILURE);
		}
	}
	
  // program variables NOT set from the command line
  
  int size = 0;       // the depth of the solution

	double u = 0.0,			// movement of system - NOT ADVECTION VELOCITY (leave at 0.0)
	       time = 0.0,
	       increment = rate * 1000;
	
	double *z = NULL,
	       *ztemp = NULL,
         *T = NULL,
	       *temp_hist_arr = NULL,
	       *Ttherm = NULL,
	       *geo = NULL,
	       **Tnew = NULL;
	
  // temporaray variables
  int i, j, k;
  double temp;	
	
  string fileName;
  ofstream outFile;

  // set up file output format
  outFile<<setiosflags(ios::showpoint | ios::left);
  outFile.precision(4);
	
	// calculate the depth of the solution
  // and store in 'size'. 'size' will be used
  // to allocate the necessary arrays.
	temp = -1 * zmax;
	while(temp < increment) {
		temp += increment;
		size++;
	}
	
	// allocate all dynamic memory
  try {
  	z = new double[size];
    ztemp = new double[size];
    geo = new double[size];
    T = new double[size];
	  Ttherm = new double[size];
    
	  Tnew = new double*[size];
	  for(i = 0; i < size; i++) {
			Tnew[i] = new double[size];
   	}
	}
  catch(bad_alloc) {
  	cerr<<ERR_HEAP<<endl;
    exit(EXIT_FAILURE);
  }
	
	// FIXME COMMENTS!!!
  temp = -1 * zmax;
	for(i = 0; i < size; i++) {
		ztemp[i] = temp;
		temp += increment;
		
    if(i == (size - 2)) {
			if(temp > 0) {
				temp = 0;
			}
		}
	}
	
  // FIXME COMMENTS!!!
	j = size - 1;
	for(i = 0; i < size; i++) {
		z[i] = ztemp[j];
		j--;
	}

//	
// CALCULATE GEOTHERM OF TIME INTERVALS
//

	cerr<<"calculating geotherm of time intervals...\n";
   
	for(i = 0; i < 4; i++) {
  	if(i == 0) {
#ifdef __WIN32__      
      fileName = "..\\output\\transient\\geotherm\\transient_T1.dat";
#else
      fileName = "../output/transient/geotherm/transient_T1.dat";
#endif
    }
    else if(i == 1) {
#ifdef __WIN32__
      fileName = "..\\output\\transient\\geotherm\\transient_T2.dat";
#else
      fileName = "../output/transient/geotherm/transient_T2.dat";
#endif
    }
    else if(i == 2) {
#ifdef __WIN32__
      fileName = "..\\output\\transient\\geotherm\\transient_T3.dat";
#else
      fileName = "../output/transient/geotherm/transient_T3.dat";
#endif
    }
    else {
#ifdef __WIN32__
      fileName = "..\\output\\transient\\geotherm\\transient_T4.dat";
#else
      fileName = "../output/transient/geotherm/transient_T4.dat";
#endif
    }

		outFile.open(fileName.c_str());

	  // check for file open errors
    if(!outFile) {
      cerr<<ERR_FILE<<endl;
      exit(EXIT_FAILURE);
    }
    
    // calculate equation, FIXME need to double-check this formula
    for(j = 0; j < size; j++) {
			T[j] = To + Gb * ((-1 * z[j]) - (v * t[i])) + 0.5 * (Gb + ((Gb * u) / v)) * (((-1 * z[j]) + (v * t[i])) * 
             exp((v * (-1 * z[j])) / a) * erfc(((-1 * z[j]) + (v * t[i])) / (2 * sqrt(a * t[i]))) - ((-1 * z[j]) -
             (v * t[i])) * erfc(((-1 * z[j]) - (v * t[i])) / (2 * sqrt(a * t[i]))));
			
      outFile<<" "<<T[j]<<" "<<z[j];        
		}
    
    outFile.close();    
  }

//  
// DEPTH OF SPECIFIED TEMPERATURE THROUGH TIME
//

	cerr<<"calculating depth of specified temperature through time...\n";
   
	for(i = 0; i < size; i++) {
		time = i * secPerMilYears;
    
		// calculate equation, FIXME need to double check this formula
		for(j = 0; j < size; j++) {
			 Tnew[j][i] = To + Gb * ((-1 * z[j]) - (v * time)) + 0.5 * (Gb + ((Gb * u) / v)) * (((-1 * z[j]) + (v * time)) *
                    exp((v * (-1 * z[j])) / a) * erfc(((-1 * z[j]) + (v * time)) / (2 * sqrt(a * time))) - ((-1 * z[j]) -
                    (v * time)) * erfc(((-1 * z[j]) - (v * time)) / (2 * sqrt(a * time))));
		}
	}

#ifdef __WIN32__
	outFile.open("..\\output\\transient\\temp_time\\transient_Tnew.dat");
#else
  outFile.open("../output/transient/temp_time/transient_Tnew.dat");
#endif

	// check for file open errors
  if(!outFile) {
    cerr<<ERR_FILE<<endl;
    exit(EXIT_FAILURE);
  }
	
  // write 'Tnew.transient' out
  for(i = 0; i < size; i++) {
		for(j = 0; j < size; j++) {
			outFile<<" "<<Tnew[i][j];
		}		
    
    outFile<<endl;
  }	
  outFile.close();
	
//
// CALCULATE ISOTHERM DATA
//

  cerr<<"calculating geotherm of time intervals...\n";
   
  for(i = 0; i < 4; i++) {

    for(j = 0; j < size; j++) {
			for(k = 0; k < size; k++) {
				Ttherm[k] = Tnew[k][j];
      }
			
      // performing interpolations
			geo[j] = interpl(Ttherm, size, z, isotherm[i]);
		}
    
		if(i == 0) {
#ifdef __WIN32__
			outFile.open("..\\output\\transient\\temp_time\\transient_isotherm1.dat");
#else
			outFile.open("../output/transient/temp_time/transient_isotherm1.dat");
#endif
    }
		else if(i == 1) {
#ifdef __WIN32__
			outFile.open("..\\output\\transient\\temp_time\\transient_isotherm2.dat");
#else
			outFile.open("../output/transient/temp_time/transient_isotherm2.dat");
#endif
    }      
		else if(i == 2) {
#ifdef __WIN32__
			outFile.open("..\\output\\transient\\temp_time\\transient_isotherm3.dat");
#else
			outFile.open("../output/transient/temp_time/transient_isotherm3.dat");
#endif
    }
		else {
#ifdef __WIN32__
			outFile.open("..\\output\\transient\\temp_time\\transient_isotherm4.dat");
#else
			outFile.open("../output/transient/temp_time/transient_isotherm4.dat");
#endif
    }

	  // check for file open errors
    if(!outFile) {
      cerr<<ERR_FILE<<endl;
      exit(EXIT_FAILURE);
    }

    // write the output
		for(j = 0; j < size; j++) {
			if(geo[j] == 0.0) {
				outFile<<"NaN";
      }
			else {
				outFile<<" "<<j<<" "<<geo[j];
      }
		}

    outFile.close();
	}
	
//
// TEMPERATURE HISTORY OF ROCK EXPOSED AT SURFACE
//

	cerr<<"calculating temperature history of rock exposed at surface...\n";
	
	k = (int)temp_hist;    // FIXME, double to int cast, is this ok, what about the fractions???
  
	try {
    temp_hist_arr = new double[k + 1];
  }
  catch(bad_alloc) {
    cerr<<ERR_HEAP<<endl;
    exit(EXIT_FAILURE);
  }
  
	for(i = 0; i <= temp_hist; i++) {
		temp_hist_arr[i] = Tnew[k][i];
		k--;
	}
	
#ifdef __WIN32__
	outFile.open("..\\output\\transient\\temp_hist\\transient_tempHist.dat");
#else
	outFile.open("../output/transient/temp_hist/transient_tempHist.dat");
#endif
  
	// check for file open errors
  if(!outFile) {
    cerr<<ERR_FILE<<endl;
    exit(EXIT_FAILURE);
  }
  
  for(i = 0; i <= temp_hist; i++) {
		outFile<<" "<<i<<" "<<temp_hist_arr[i];
  }	
  outFile.close();

#ifdef __WIN32__
  outFile.open("..\\output\\default_output\\transient_timeHist.dat");
#else
  outFile.open("../output/default_output/transient_timeHist.dat");
#endif

	// check for file open errors
  if(!outFile) {
    cerr<<ERR_FILE<<endl;
    exit(EXIT_FAILURE);
  }

  // create the timeHist.transient file for age prediction
  // calculation in the default output directory
  for(i = 0; i <= temp_hist; i++) {
    outFile<<(float)i<<endl;        // output values as float for age_prediction compatability
  }
  outFile.close();

#ifdef __WIN32__
  outFile.open("..\\output\\default_output\\transient_tempHist.dat");
#else
  outFile.open("../output/default_output/transient_tempHist.dat");
#endif

	// check for file open errors
  if(!outFile) {
    cerr<<ERR_FILE<<endl;
    exit(EXIT_FAILURE);
  }

  // create the tempHist.transient file for age prediction
  // calculation in the default output directory
  for(i = 0; i <= temp_hist; i++) {
    outFile<<temp_hist_arr[i]<<endl;
  }
  outFile.close();
  
  // de-allocate all dynamic memory
	for(i = 0; i < size; i++)
		delete [] Tnew[i];
	delete [] Tnew;
  
  delete [] T;
	delete [] Ttherm;
	delete [] temp_hist_arr;
	delete [] geo;
	delete [] z;
	delete [] ztemp;
	
	return 0;
}

