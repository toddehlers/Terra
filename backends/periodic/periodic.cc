// 2D Periodic Topography Model
// Earth Surface Processes Research Group
// The University of Michigan
// Professor Todd Ehlers 
// Research Assistant Santosh Kumar
// Research Assistant Tehmasp Chaudhri

// This program will analyze the geotherm at varying depths
// and see how both topography and denudation on that
// topography will affect it.

#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>

#include "getopt.h"

#undef __WIN32__

#define ERR_HEAP "ERROR: Heap Allocation Error"
#define ERR_FILE "ERROR: Could not open file(s) for output; do the directories exist?"

using namespace std;

const double pi = 3.14159265;
const char *versionString = "2D Periodic Topography Model - v1.0";

// This function searches 'arr' for the largest value.
// Note: This is a slow linear search algorithm.
double findMax(double *inArray, int size)
{
	double currentMax = inArray[0];
	
	for(int i = 1; i < size; i++) {
		if(inArray[i] > currentMax) {
			currentMax = inArray[i];
		}
	}
	
	return currentMax;
}


// FIXME, COMMENTS!!!
double interpl(const double Ttherm[], const int endY, const double YY[], const double num)
{
	int pos0 = 0,
      pos1 = 0;
  
	double slope = 0, 
         ans = 0;   // 'ans' must be set to zero
  
	bool value = false; // 'value' must be set to false 

	for(int i = 0; i < (endY - 1); i++) {
		
    if((num <= Ttherm[i]) && (num >= Ttherm[i + 1])) {
			pos0 = i;
			pos1 = i + 1;
			i = endY + 1;
			
			value = true;
		}
	}

	if(value) {
		slope = (YY[pos1] - YY[pos0]) / (Ttherm[pos1] - Ttherm[pos0]);
		ans = (slope * (num - Ttherm[pos0])) + YY[pos0];
	}
	
	return ans;
}


// FIXME, COMMENTS!!!
double interpl1(const double YY[], const int sizeYY, const double Ttherm[], const double num)
{
	int pos0 = 0, 
      pos1 = 0;
  
	double slope = 0,
         ans = 0;   // 'ans' must be set to zero
  
	bool value = false;   // 'value' must be set to false

	for(int i = 0; i < (sizeYY - 1); i++) {
    
		if(num == YY[i]) {
			ans = Ttherm[i];
			i = sizeYY + 1;
		}
		else if((num >= YY[i]) && (num <= YY[i + 1])) {
			pos0 = i;
			pos1 = i + 1;
			i = sizeYY + 1;
			
			value = true;
		}
	}

	if(value) {
		slope = (Ttherm[pos1] - Ttherm[pos0]) / (YY[pos1] - YY[pos0]);
		ans = (slope * (num - YY[pos0])) + Ttherm[pos0];
	}
	
	return ans;
}

// This function prints the help menu to the
// command line. This function takes the name
// of the program 'argv[0]' as a parameter for
// 'filename'.
void printMenu(const char *filename)
{
	cout<<"2D Periodic Topography Model\n";
	cout<<"Usage: "<<filename<<" [OPTION]... [ARG]...\n\n";
	
	cout<<"OPTIONS:\n";
	cout<<"-T   surface temperature (celcius)\n";
	cout<<"-B   lithosphere base temperature (celcius)\n";
	cout<<"-D   lithosphere base depth (meters)\n";
	cout<<"-l   surface temperature lapse rate (C/m)\n";
	cout<<"-U   erosion rate (mm/yr)\n\n";
	
	cout<<"-w   topography wavelength (meters)\n";
	cout<<"-a   topography amplitude (meters)\n";
	cout<<"-x   horizontal width (meters)\n";
	cout<<"-y   maximum depth of calculation (meters)\n\n";
	
	cout<<"-k   diffusivity (m2/s)\n";
	cout<<"-c   density of crust (kg/m3)\n";
	cout<<"-S   specific heat (J/kg/K)\n";
	cout<<"-s   surface volumetric heat production (W/m3)\n";
	cout<<"-d   heat production characteristic depth (meters)\n\n";
	
	cout<<"-n   isotherm 1 (celcius)\n";
	cout<<"-o   isotherm 2 (celcius)\n";
	cout<<"-p   isotherm 3 (celcius)\n";
	cout<<"-q   isotherm 4 (celcius)\n\n";
	
	cout<<"-h   display this help menu\n";
	cout<<"-v   display the version information\n";
}


int main(int argc, char *argv[])
{
	int option;
	extern char *optarg;
	
	// program variables (with default values) which are set via command line arguments
	
	double Ts = 10,					// surface temperature (celcius)
				 U = 0.5,					// erosion rate (mm / yr), converted later
		     k = 0.000001,		// diffusivity constant (m2 / s)
		     h = 10000,		    // depth to heat production of 1/e (meters)
		     lam = 20000,	    // wavelength of topography (meters)
		     As = 0.000001,		// volumetric heat production at surface (W / m3)
		     p = 2700,		    // density of material (kg / m3)
		     c = 800,		      // heat capacity / specific heat (J / kg / K)
		     Tl = 1000,		    // temperature at base of lithosphere (celcius)
		     L = 100000,		  // depth to base of lithosphere (meters)
		     phi = 0.007,	    // lapse rate of surface temperature at altitude (C / m)
		     Ho = 1500;       // amplitude of topography (meters)
		
	int	x = 40000,					// horizontal width - starting point (meters)
		  y = 10000;					// maximum depth of calculation - starting point (meters)
			
	int isotherm[5] = {0, 60, 120, 180, 250};
	
  // parse the command line options using getopt()
  // exit the program if there are any bad command line arguments.
	while((option = getopt(argc, argv, "hvT:U:k:d:w:s:c:S:B:D:l:a:x:y:n:o:p:q:")) != -1) {
		switch(option) {
		
			case 'T':
						Ts = atof(optarg);
						break;
						
			case 'U':
						U = atof(optarg);
						break;
						
			case 'k':
						k = atof(optarg);
						break;
						
			case 'd':
						h = atol(optarg);
						break;
						
			case 'w':
						lam = atol(optarg);
						break;
						
			case 's':
						As = atof(optarg);
						break;
						
			case 'c':
						p = atol(optarg);
						break;
						
			case 'S':
						c = atol(optarg);
						break;
						
			case 'B':
						Tl = atol(optarg);
						break;
						
			case 'D':
						L = atol(optarg);
						break;
						
			case 'l':
						phi = atof(optarg);
						break;
						
			case 'a':
						Ho = atol(optarg);
						break;
						
			case 'x':
						x = atol(optarg);
						break;
				
			case 'y':
						y = atol(optarg);
						break;
						
			case 'n':
						isotherm[1] = atoi(optarg);
						break;
						
			case 'o':
						isotherm[2] = atoi(optarg);
						break;
						
			case 'p':
						isotherm[3] = atoi(optarg);
						break;
						
			case 'q':
						isotherm[4] = atoi(optarg);
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
	
	// program variables not set from the command line

	int i, j, m, temp, tempY;		// some temporary variables
	
  int x_count = 0,
      y_count = 0;
  
  double hilltop = 0;
	
	double *topoY = NULL,
			   *topoYBottom = NULL,
		     *XX = NULL,
		     *YY = NULL,
		     *Ttherm = NULL,
		     *geo = NULL,
		     *geo_temp = NULL,
				 **T = NULL,
		     **Tts = NULL,
				 **ZHist = NULL,
				 **TempHist = NULL;

  ofstream outFile;
  ofstream zHistOutFile;

  // set up file output format
  outFile<<setiosflags(ios::showpoint | ios::left);
  outFile.precision(4);

  zHistOutFile<<setiosflags(ios::showpoint | ios::left);
  zHistOutFile.precision(4);
  
	// defining the equations
	const double u = U / (3.1536E10);	// this will convert the erosion/denudation rate from mm/yr to m/s
								                    // which will make it easier to change values of u
	
  const double uk = u / k;          // defining u/k as uk

	const double B = (As * h * h) / (p * c * (k - (u * h))); // solving for beta
	
	const double G = ((Tl - Ts) - B * (1 - exp(-L / h))) / (1 - exp(-uk * L)); // solve for gamma
	const double m2 = 0.5 * (-uk - sqrt((uk * uk) + ((4 * pi / lam) * (4 * pi / lam)))); // solution to the quadratic equation
	const double D = Ho * (uk * G + (B / h) - phi);  // and delta (note: this is an approximation)

	// this program will solve the temperature at each (x,y) point, 
	// resulting in the entire field being solved for every 100m.

  // calculate x_count
	i = -1 * x / 2;
	while(i <= (x / 2)) {
		i += 100;           // FIXME, what does '100' represent???
		x_count++;
	}

  // calculate the different valley positions
	temp = ((x_count - 1) / 2);
  
	const int ridgePos = 0;                                 // ridge
	const int valleyPos = ((temp - 1) / 2);                 // valley
	const int midvalleyPos = ((valleyPos - 1) / 2);		      // mid-valley
	const int quarterPos = ((midvalleyPos - 1) / 2);	      // 1/4 position (between ridge & mid-valley)
	const int threequarterPos = (valleyPos - quarterPos);   // 3/4 position	(between mid-valley & valley)

  try {
    topoY = new double[x_count];
    topoYBottom = new double[x_count];
  }
  catch(bad_alloc) {
    cerr<<ERR_HEAP<<endl;
    exit(EXIT_FAILURE);
  }

  // define 'topoY' and 'topoYBottom'
  i = -1 * x / 2;
	for(j = 0; j < x_count; j++) {
		topoY[j] = Ho * cos(2 * pi * i / lam);
		topoYBottom[j] = Ho * cos(2 * pi * i / lam) - y + Ho;
		
    i += 100;
	}

	// same as 'Ho', but take the max of 'topoY' and 'x_count' as a double check.
  // find the max value in 'topoY' array.
  hilltop = findMax(topoY, x_count);
  
  // calculate y_count
  i = -1 * y;
	while(i <= hilltop) {
		i += 100;
		y_count++;
	}

  // dynamically allocate space based on 'x_count' and 'y_count'
	try {
    XX = new double[x_count];
    geo = new double[x_count];
    
    YY = new double[y_count];
    Ttherm = new double[y_count];
    
    T = new double*[y_count];
    for(i = 0; i < y_count; i++) {
      T[i] = new double[x_count];
    }
    
    Tts = new double*[y_count];
    for(i = 0; i < y_count; i++) {
      Tts[i] = new double[x_count];
    }
  }
  catch(bad_alloc) {
    cerr<<ERR_HEAP<<endl;
    exit(EXIT_FAILURE);
  }
			
//
// SOLVE FOR ISOTHERM TOPOGRAPHY
// 

  // DEBUG
  cerr<<"solving for isotherm topography...\n";  
  cerr<<"writing 'periodic_YY.dat' output file...\n";
  
#ifdef __WIN32__
	outFile.open("..\\output\\periodic\\isotherm\\periodic_YY.dat");
#else
	outFile.open("../output/periodic/isotherm/periodic_YY.dat");
#endif
  
  // check for file open errors
  if(!outFile) {
    cerr<<ERR_FILE<<endl;
    exit(EXIT_FAILURE);
  }
  
	i = -1 * y;
	for(j = 0; j < y_count; j++) {
		YY[j] = i;
		outFile<<i<<endl;
    
		i += 100;
	}
	outFile.close();

  // DEBUG
  cerr<<"writing 'periodic_XX.dat' output file...\n";
  
#ifdef __WIN32__                               
	outFile.open("..\\output\\periodic\\isotherm\\periodic_XX.dat");
#else
	outFile.open("../output/periodic/isotherm/periodic_XX.dat");
#endif

  // check for file open errors
  if(!outFile) {
    cerr<<ERR_FILE<<endl;
    exit(EXIT_FAILURE);
  }
  
	i = -1 * x / 2;		// needs to be reset from the previous loop
	for(j = 0; j < x_count; j++) {
		XX[j] = i;
		outFile<<i<<endl;
		i += 100;
	}
	outFile.close();

	temp = -1 * x / 2;
	for(m = 0; m < x_count; m++) {
    
		tempY = y;
    
		for(i = 0; i < y_count; i++) {
      
      // calculating the values of T, FIXME need to double check these formulas
			T[i][m] = Ts + B * (1 - exp(-1 * tempY / h)) + G * (1 - exp(-1 * uk * tempY)) + 
                (D * cos(2 * pi * temp / lam) * exp(m2 * tempY));
      
			Tts[i][m] = Ts + (0.03 * tempY / 3.0) + ((As * h * h / 3.0) * (1 - exp(-1 * tempY / h))) + 
                  ((phi - (0.03 / 3.0) - (As * h / 3.0)) * Ho * cos(2 * pi * temp / lam) *
                  exp(-1 * 2 * pi * tempY / lam));
      
			tempY -= 100;
		}
		
    temp += 100;
  }

//
// GEOTHERM
// 
  
  // DEBUG
  cerr<<"writing 'periodic_geoX.dat' output files...\n";
  
	for(temp = 0; temp < 5; temp++) {
    
		for(m = 0; m < x_count; m++) {
      
			for(i = 0; i < y_count; i++) {
			  Ttherm[i] = T[i][m];
      }
      
      // perform interpolation
			geo[m] = interpl(Ttherm, y_count, YY, isotherm[temp]);
		}

		if(temp == 0) {
#ifdef __WIN32__
			outFile.open("..\\output\\periodic\\isotherm\\periodic_geo0.dat");
#else
			outFile.open("../output/periodic/isotherm/periodic_geo0.dat");
#endif
    }
		else if(temp == 1) {
#ifdef __WIN32__
			outFile.open("..\\output\\periodic\\isotherm\\periodic_geo1.dat");
#else
			outFile.open("../output/periodic/isotherm/periodic_geo1.dat");
#endif
    }
		else if(temp == 2) {
#ifdef __WIN32__
			outFile.open("..\\output\\periodic\\isotherm\\periodic_geo2.dat");
#else
			outFile.open("../output/periodic/isotherm/periodic_geo2.dat");
#endif
    }
		else if(temp == 3) {
#ifdef __WIN32__
			outFile.open("..\\output\\periodic\\isotherm\\periodic_geo3.dat");
#else
			outFile.open("../output/periodic/isotherm/periodic_geo3.dat");
#endif
    } 
		else {
#ifdef __WIN32__
			outFile.open("..\\output\\periodic\\isotherm\\periodic_geo4.dat");
#else
			outFile.open("../output/periodic/isotherm/periodic_geo4.dat");
#endif
    }

    // check for file open errors
    if(!outFile) {
      cerr<<ERR_FILE<<endl;
      exit(EXIT_FAILURE);
    }

		for(j = 0; j < x_count; j++) {
      
			outFile<<" "<<XX[j]<<" ";

			if(geo[j] == 0.0) {
				outFile<<"NaN";
      }
			else {
				outFile<<geo[j];
      }
		}
		outFile.close();
		
		for(m = 0; m < x_count; m++) {
      
			for(i = 0; i < y_count; i++) {
				Ttherm[i] = Tts[i][m];
      }
      
      // perform interpolation
			geo[m] = interpl(Ttherm, y_count, YY, isotherm[temp]);
		}
  }
	
  // change all values below 0.0 in T[][] to be 0.0.
	for(i = 0; i < x_count; i++) {
    
		for(j = 0; j < y_count; j++) {
			
      if(T[j][i] < 0.0) {   // modify T[][] so that it can be plotted
				T[j][i] = 0.0;
      }
		}
		
		for(j = 0; j < y_count; j++) {
      
			if(Tts[j][i] < 0.0) {   // modify T[][] so that it can be plotted
				Tts[j][i] = 0.0;
      }
		}
	}

  // DEBUG
  cerr<<"writing 'periodic_T.dat' output file...\n";
	
	// writing the new values of T[][] into the file T.dat
#ifdef __WIN32__
	outFile.open("..\\output\\periodic\\isotherm\\periodic_T.dat");
#else
	outFile.open("../output/periodic/isotherm/periodic_T.dat");
#endif

  // check for file open errors
  if(!outFile) {
    cerr<<ERR_FILE<<endl;
    exit(EXIT_FAILURE);
  }

	for(i = 0; i < y_count; i++) {	
		for(j = 0; j < x_count; j++) {
			outFile<<T[i][j]<<" ";
		}
		outFile<<endl;
	}
	outFile.close();

  // DEBUG
  cerr<<"writing 'periodic_topoY.dat' output file...\n";
	
	// writing the values of topoY[] into the file "topoY.periodic"
#ifdef __WIN32__
	outFile.open("..\\output\\periodic\\isotherm\\periodic_topoY.dat");
#else
	outFile.open("../output/periodic/isotherm/periodic_topoY.dat");
#endif

  // check for file open errors
  if(!outFile) {
    cerr<<ERR_FILE<<endl;
    exit(EXIT_FAILURE);
  }
  
	for(i = 0; i < x_count; i++) {
		outFile<<" "<<XX[i]<<" "<<topoY[i];
	}
	outFile.close();

  // DEBUG
  cerr<<"writing 'periodic_topoYBottom.dat' output file...\n";
	
  // writing the values of topoYBottom[] into the file "topoYBottom.periodic"
#ifdef __WIN32__
	outFile.open("..\\output\\periodic\\isotherm\\periodic_topoYBottom.dat");
#else
	outFile.open("../output/periodic/isotherm/periodic_topoYBottom.dat");
#endif

  // check for file open errors
  if(!outFile) {
    cerr<<ERR_FILE<<endl;
    exit(EXIT_FAILURE);
  }
  
	for(i = 0; i < x_count; i++) {
		outFile<<" "<<XX[i]<<" "<<topoYBottom[i];
	}
	outFile.close();

//
// ZHIST
//
	
	int count = 0,
      test = 0;
      
	double dt = 0.1E6,
         dz = U * 0.001 * dt;
  
	double slope, x0, x1, x2, t0, t1, t2;

  count = (int)((y - Ho) / dz);  // FIXME, converting to 'int' from 'double'
	
	try {
    geo_temp = new double[x_count];
    
    ZHist = new double*[count];
	  for(i = 0; i < count; i++) {
		  ZHist[i] = new double[x_count];
    }
  }
  catch(bad_alloc) {
    cerr<<ERR_HEAP<<endl;
    exit(EXIT_FAILURE);
  }

	for(i = 0; i < x_count; i++) {
    
		temp = (int)topoYBottom[i];  // FIXME, converting to 'int' from 'double'
    
		for(j = 0; j < count; j++) {
			test = temp;
			ZHist[j][i] = test;
      
			temp += (int)dz;   // FIXME, converting to 'int' from 'double'
		}
  }

  // DEBUG
  cerr<<"writing 'periodic_zHist.dat' output file...\n";

#ifdef __WIN32__
	outFile.open("..\\output\\periodic\\temp_hist\\periodic_zHist.dat");
#else
	outFile.open("../output/periodic/temp_hist/periodic_zHist.dat");
#endif

#ifdef __WIN32__
	zHistOutFile.open("..\\output\\default_output\\periodic_zHist.dat");
#else
	zHistOutFile.open("../output/default_output/periodic_zHist.dat");
#endif


  // check for file open errors
  if(!outFile || !zHistOutFile) {
    cerr<<ERR_FILE<<endl;
    exit(EXIT_FAILURE);
  }
  
  for(i = 0; i < count; i++) {
    outFile<<(ZHist[i][ridgePos] / 1000)<<"   "
           <<(ZHist[i][quarterPos] / 1000)<<"   "
           <<(ZHist[i][midvalleyPos] / 1000)<<"   "
           <<(ZHist[i][threequarterPos] / 1000)<<"   "
           <<(ZHist[i][valleyPos] / 1000)<<endl;

    zHistOutFile<<(ZHist[i][ridgePos] / 1000)<<"   "
                <<(ZHist[i][quarterPos] / 1000)<<"   "
                <<(ZHist[i][midvalleyPos] / 1000)<<"   "
                <<(ZHist[i][threequarterPos] / 1000)<<"   "
                <<(ZHist[i][valleyPos] / 1000)<<endl;
  }
  outFile.close();
  zHistOutFile.close();
  
  // DEBUG
  cerr<<"writing 'periodic_topoYBottomGeo.dat' output file...\n";

#ifdef __WIN32__
	outFile.open("..\\output\\periodic\\isotherm\\periodic_topoYBottomGeo.dat");
#else
  outFile.open("../output/periodic/isotherm/periodic_topoYBottomGeo.dat");
#endif

  // check for file open errors
  if(!outFile) {
    cerr<<ERR_FILE<<endl;
    exit(EXIT_FAILURE);
  }
  
	for(i = 0; i < x_count; i++) {
		for(j = 0; j < y_count; j++) {
			Ttherm[j] = T[j][i];
    }
    
		geo[i] = interpl1(YY, y_count, Ttherm, topoYBottom[i]);  // performing interpolations for topoYBottom
		outFile<<geo[i]<<endl;
	}
	outFile.close();

  // DEBUG
  cerr<<"writing 'periodic_topoYGeo.dat' output file...\n";

#ifdef __WIN32__  
	outFile.open("..\\output\\periodic\\isotherm\\periodic_topoYGeo.dat");
#else
	outFile.open("../output/periodic/isotherm/periodic_topoYGeo.dat");
#endif
  
  // check for file open errors
  if(!outFile) {
    cerr<<ERR_FILE<<endl;
    exit(EXIT_FAILURE);
  }

  // performing interpolations for topoY
	for(i = 0; i < x_count; i++) {
		
    if(topoY[i] > 0) {
			geo_temp[i] = Ts - (topoY[i] * phi);
    }
		else if(topoY[i] == 0) {
			geo_temp[i] = Ts;
    }
		else {
			geo_temp[i] = Ts - (topoY[i] * phi);
    }
		
		outFile<<geo_temp[i]<<endl;
	}
	outFile.close();

	try {
    TempHist = new double*[count];
	  for(i = 0; i < count; i++) {
		  TempHist[i] = new double[x_count];
    }
  }
  catch(bad_alloc) {
    cerr<<"ERR_HEAP"<<endl;
    exit(EXIT_FAILURE);
  }
		
	for(i = 0; i < x_count; i++) {
    
		for(m = 0; m < count; m++) {
      
			for(temp = 0; temp < y_count; temp++) {
				Ttherm[temp] = T[temp][i];
      }
      
			if(interpl1(YY, y_count, Ttherm, ZHist[m][i]) >= 40.0) {
				TempHist[m][i] = interpl1(YY, y_count, Ttherm, ZHist[m][i]);
			}
			else {
				x1 = topoY[i]; t1 = geo_temp[i];
				t2 = interpl1(YY, y_count, Ttherm, ZHist[m - 1][i]);
				
        for(j = m; j < count; j++) {
					x0 = ZHist[j][i];
					x2 = ZHist[j - 1][i]; 
					slope = (t2 - t1) / (x2 - x1);
					t0 = (slope * (x0 - x2)) + t2;
					TempHist[j][i] = t0;
					t2 = t0;
				}
				
        break;
			}
		}
	}
	
	// modification requested by Todd
	for(i = 0; i < x_count; i++) {
		TempHist[count - 1][i] = geo_temp[i];
  }

  // DEBUG
  cerr<<"writing 'periodic_tempHist.dat' output file...\n";
  
#ifdef __WIN32__
	outFile.open("..\\output\\periodic\\temp_hist\\periodic_tempHist.dat");
#else
  outFile.open("../output/periodic/temp_hist/periodic_tempHist.dat");
#endif

  // check for file open errors 
  if(!outFile) {
    cerr<<"ERR_FILE"<<endl;
    exit(EXIT_FAILURE);
  }
  
	for(i = 0; i < count; i++) {
		for(j = 0; j < x_count; j++) {
			outFile<<TempHist[i][j]<<" ";
		}
	  outFile<<endl;
	}
	outFile.close();

  //////////////////////////////////////////////////////////////
  
	double time_increment = (((y + hilltop) / (U * 0.001)) / count);
	double *time = NULL;

  try {
	  time = new double[count];
  }
  catch(bad_alloc) {
    cerr<<"ERR_HEAP"<<endl;
    exit(EXIT_FAILURE);
  }

  // FIXME, coverting to 'int' from 'double' in for loop below!!!
	for(i = -1 * (int)((y + hilltop) / (U * 0.001)), temp = 0; temp < count; i += (int)time_increment, temp++) {
		time[temp] = i / 1E6;
	}

	// modification requested by Todd
	time[count - 1] = 0;

//
// TIME_HIST
// 
 
  // DEBUG
  cerr<<"writing 'periodic_timeHist.dat' output file...\n";
  
#ifdef __WIN32__
	outFile.open("..\\output\\default_output\\periodic_timeHist.dat");
#else
	outFile.open("../output/default_output/periodic_timeHist.dat");
#endif

  // check for file open errors 
  if(!outFile) {
    cerr<<"ERR_FILE"<<endl;
    exit(EXIT_FAILURE);
  }
  
	for(i = (count - 1); i >= 0; i--) {
		if(i == (count - 1)) {
			outFile<<"0.0"<<endl;
     }
		else {
			outFile<<(-1 * time[i])<<endl;
    }
	}
	outFile.close();

//
// RIDGE_NEW
//

  // DEBUG
  cerr<<"writing 'periodic_ridge.dat' output file...\n";

#ifdef __WIN32__
	outFile.open("..\\output\\periodic\\temp_hist\\periodic_ridge.dat");
#else
	outFile.open("../output/periodic/temp_hist/periodic_ridge.dat");
#endif

  // check for file open errors 
  if(!outFile) {
    cerr<<"ERR_FILE"<<endl;
    exit(EXIT_FAILURE);
  }
  
	for(i = 0; i < count; i++) {
		outFile<<" "<<1 * time[i]<<" "<<TempHist[i][ridgePos];
  }
	outFile.close();

//
// VALLEY_NEW
//

  // DEBUG
  cerr<<"writing 'periodic_valley.dat' output file...\n";
  
#ifdef __WIN32__
	outFile.open("..\\output\\periodic\\temp_hist\\periodic_valley.dat");
#else
	outFile.open("../output/periodic/temp_hist/periodic_valley.dat");
#endif

  // check for file open errors 
  if(!outFile) {
    cerr<<"ERR_FILE"<<endl;
    exit(EXIT_FAILURE);
  }
  
	for(i = 0; i < count; i++) {
		outFile<<" "<<1 * time[i]<<" "<<TempHist[i][valleyPos];
	}
	outFile.close();
		
//
// MID-VALLEY_NEW
//

  // DEBUG
  cerr<<"writing 'periodic_midValley.dat' output file...\n";

#ifdef __WIN32__
	outFile.open("..\\output\\periodic\\temp_hist\\periodic_midValley.dat");
#else
	outFile.open("../output/periodic/temp_hist/periodic_midValley.dat");
#endif

  // check for file open errors 
  if(!outFile) {
    cerr<<"ERR_FILE"<<endl;
    exit(EXIT_FAILURE);
  }
  
	for(i = 0; i < count; i++) {
		outFile<<" "<<1 * time[i]<<" "<<TempHist[i][midvalleyPos];
	}
	outFile.close();

//
// TEMPHIST FOR TAP
//

  // DEBUG
  cerr<<"writing 'periodic_tempHist.dat' output file...\n";

#ifdef __WIN32__
	outFile.open("..\\output\\default_output\\periodic_tempHist.dat");
#else
	outFile.open("../output/default_output/periodic_tempHist.dat");
#endif

  // check for file open errors 
  if(!outFile) {
    cerr<<"ERR_FILE"<<endl;
    exit(EXIT_FAILURE);
  }
  
	for(i = 0; i < count; i++) {
		outFile<<TempHist[i][ridgePos]<<"   "<<TempHist[i][quarterPos]<<"   "<<TempHist[i][midvalleyPos]<<"   "
           <<TempHist[i][threequarterPos]<<"   "<<TempHist[i][valleyPos]<<endl;
  }
	outFile.close();

//
// XHIST FOR TAP
//

  // DEBUG
  cerr<<"writing 'periodic_xHist.dat' output file...\n";
 
#ifdef __WIN32__
	outFile.open("..\\output\\default_output\\periodic_xHist.dat");
#else
	outFile.open("../output/default_output/periodic_xHist.dat");
#endif

  // check for file open errors 
  if(!outFile) {
    cerr<<"ERR_FILE"<<endl;
    exit(EXIT_FAILURE);
  }
  
	for(i = 0; i < count; i++) {
    outFile<<(XX[ridgePos] / 1000)<<"   "
           <<(XX[quarterPos] / 1000)<<"   "
           <<(XX[midvalleyPos] / 1000)<<"   "
           <<(XX[threequarterPos] / 1000)<<"   "
           <<(XX[valleyPos] / 1000)<<endl;
  }
  outFile.close();
  
//
// YHIST FOR TAP
//

  // DEBUG
  cerr<<"writing 'periodic_yHist.dat' output file...\n";

#ifdef __WIN32__
	outFile.open("..\\output\\default_output\\periodic_yHist.dat");
#else
	outFile.open("../output/default_output/periodic_yHist.dat");
#endif

  // check for file open errors 
  if(!outFile) {
    cerr<<"ERR_FILE"<<endl;
    exit(EXIT_FAILURE);
  }
  
  for(i = 0; i < count; i++) {
    outFile<<"0.000"<<"   "<<"0.000"<<"   "
           <<"0.000"<<"   "<<"0.000"<<"   "
           <<"0.000"<<endl;
  }
  outFile.close();
  
  // de-allocate all dynamic data
	delete [] topoY;
	delete [] topoYBottom;
	delete [] XX;	
	delete [] YY;
	delete [] Ttherm;
	delete [] geo;
	delete [] geo_temp;
	delete [] time;

	for(temp = 0; temp < y_count; temp++)
		delete [] T[temp];
	delete [] T;
	
	for(temp = 0; temp < y_count; temp++)
		delete [] Tts[temp];
	delete [] Tts;
	
	for(temp = 0; temp < count; temp++)
		delete [] TempHist[temp];
	delete [] TempHist;
	
	for(temp = 0; temp < count; temp++)
		delete [] ZHist[temp];
	delete [] ZHist;
	
	return 0;
}
