// Age Prediction
// Earth Surface Processes Research Group
// The University of Michigan
// Professor Todd Ehlers
// Research Assistant Tehmasp Chaudhri

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

#include "utils.h"
#include "getopt.h"

#undef __WIN32__

#define FILE_SUFFIX ".dat"
#define TECPLOT_FILE_SUFFIX "_tecplot.dat"
#define ERR_HEAP "ERROR: Heap Allocation Error"
#define ERR_FILE "ERROR: Could not open file(s) for output; do the directories exist?"

using namespace std;

const char *versionString = "Age Prediction - v1.0";

extern int get_he_age(double **thist, double **temp, const int m, const int n, double a, double D0,
                      double Ea, double U, double Th, double aheAgesArray[]);
extern int get_aft_age(double **timein, double **tempin, const int m, const int n, double *aftAgesArray);
extern int get_zft_age(double **timein, double **tempin, const int m, const int n, double zftAgesArray[]);

int NSTEPS = 20;
double MILYEARS = 50.0; 			// time padding for thermochronometer work
double TIME_STEP = 20000.0;		// time step size in FEM calculation, units = years. converted to sec in program.
double B = 3.160e-22;					// material constant (My)
double EBAR = 49.77;					// acivation energy (kcal / mol)
double R = 0.0019872;					// gas constant (kcal / (mol * deg))
double DPAR = 1.8;
double ALPHA = -0.12327;
double BETA = -11.988;
double MIN_DETECT = 0.13;
double C[4] = {-19.844, 0.38951, -51.253, -7.6423};

// this function writes out the data from the input arguments
// in a tablular format.
void writeOutput(double **xHistArray, double **yHistArray, double **zHistArray, double *aheAgesArray,
								 double *zheAgesArray, double *aftAgesArray, double *zftAgesArray, int tempRows,
								 int tempCols, const string &fileName)
{
	int i;
	ofstream outFile;

  // write output to 'fileName', without header
  outFile.open( string(fileName + FILE_SUFFIX).c_str() );

  // check for file open errors
  if(!outFile) {
    cerr<<ERR_FILE<<endl;
    exit(EXIT_FAILURE);
  }
  
  // set up file output format
  outFile<<setiosflags(ios::showpoint | ios::left);
  outFile.precision(4);
 
  cout << "tempCols: " << tempCols << endl;
 
  for(i = 0; i < tempCols; i++) {
    outFile<<"  "<<xHistArray[tempRows - 1][i]
           <<"  "<<yHistArray[tempRows - 1][i]
           <<"  "<<zHistArray[tempRows - 1][i]
           <<"  "<<aheAgesArray[i]
           <<"  "<<zheAgesArray[i]
           <<"  "<<aftAgesArray[i]
           <<"  "<<zftAgesArray[i];
    
    outFile<<endl;
  }

  outFile.close();
}


// this function writes out the data from the input arguments
// in a tablular format and also appends a header at the top
// of the file to allow for plots to be created with 'Tecplot'
void writeTecPlot(double **xHistArray, double **yHistArray, double **zHistArray, double *aheAgesArray,
								  double *zheAgesArray, double *aftAgesArray, double *zftAgesArray, int tempRows,
								  int tempCols, const string &fileName)
{
	int i;
	ofstream outFile;
	
	// write output to 'fileName' + TECPLOT_FILE_SUFFIX, with header 
  outFile.open( string(fileName + TECPLOT_FILE_SUFFIX).c_str() );

  // check for file open errors
  if(!outFile) {
    cerr<<ERR_FILE<<endl;
    exit(EXIT_FAILURE);
  }
	
  // set up file output format
	outFile<<setiosflags(ios::showpoint | ios::left);
  outFile.precision(4);

  // output the 'Tecplot' header
  outFile<<"TITLE= TERRA"<<endl;
  outFile<<"VARIABLES= X(km), Y(km), Z(km), AHe(Ma), ZHe(Ma), AFT(Ma), ZFT(Ma)"<<endl;
  outFile<<"ZONE I="<<tempCols<<", F=POINT"<<endl;

  cout << "tempCols: " << tempCols << endl;
 
  for(i = 0; i < tempCols; i++) {
    outFile<<"  "<<xHistArray[tempRows - 1][i]
           <<"  "<<yHistArray[tempRows - 1][i]
           <<"  "<<zHistArray[tempRows - 1][i]
           <<"  "<<aheAgesArray[i]
           <<"  "<<zheAgesArray[i]
           <<"  "<<aftAgesArray[i]
           <<"  "<<zftAgesArray[i];
    
    outFile<<endl;
  }

  outFile.close();
}


int main(int argc, char *argv[])
{	
  extern char *optarg;    // getopt() external variable
  int option;
  
  // program variables with default values. these variables are those
  // that can be set by the user via the commnad line.

  // default AHe calculation variables
  double aa = 42.50;
  double ad = 50.0e8;
  double ae = 33.0;
  int au = 13;
  int at = 27;

  // default ZHe calculation variables
  double za = 40.00;
  double zd = 4.6e7;
  double ze = 40.4;
  int zu = 200;
  int zt = 100;
  
  int flgXCoord = 0, flgYCoord = 0, flgZCoord = 0;      // value of '0' disables that coordinate file
  int flgAHe = 1, flgZHe = 1, flgAft = 1, flgZft = 1;   // value of '1' enables that calculation
 
  // temporary strings to hold the various filenames
  string timeHist, tempHist, xHist, yHist, zHist, tmpStr;

#ifdef __WIN32__
  string outputFile = "..\\output\\age_prediction\\point_age_hists"; 
#else
  string outputFile = "../output/age_prediction/point_age_hists";
#endif

  // parse all the command line options. if there are any bad command line arguments
  // the program will exit after prompting the user.
  while((option = getopt(argc, argv, "hv1234t:T:x:y:z:o:Y:S:j:k:l:m:n:J:K:L:M:N:A:B:C:D:E:F:G:H:I:U:V:W:")) != -1) {
    switch(option) {

      case 't':
        timeHist = optarg;
        break;

      case 'T':
        tempHist = optarg;
        break;

      case 'x':
        xHist = optarg;
        flgXCoord = 1;
        break;

      case 'y':
        yHist = optarg;
        flgYCoord = 1;
        break;

      case 'z':
        zHist = optarg;
        flgZCoord = 1;
        break;

      case 'o':
        outputFile = optarg;
        break;

      case '1':
        flgAHe = 0;
        break;

      case '2':
        flgZHe = 0;
        break;

      case '3':
        flgAft = 0;
        break;

      case '4':
        flgZft = 0;
        break;

      case 'Y':
        MILYEARS = atof(optarg);
        break;

      case 'S':
        TIME_STEP = atof(optarg);
        break;

      case 'j':
        ad = atof(optarg);
        break;

      case 'k':
        ae = atof(optarg);
        break;

      case 'l':
        aa = atof(optarg);
        break;

      case 'm':
        au = atoi(optarg);
        break;

      case 'n':
        at = atoi(optarg);
        break;

      case 'J':
        zd = atof(optarg);
        break;

      case 'K':
        ze = atof(optarg);
        break;

      case 'L':
        za = atof(optarg);
        break;

      case 'M':
        zu = atoi(optarg);
        break;

      case 'N':
        zt = atoi(optarg);
        break;

      case 'A':
        ALPHA = atof(optarg);
        break;

      case 'B':
        BETA = atof(optarg);
        break;

      case 'C':
        NSTEPS = atoi(optarg);
        break;

      case 'D':
        DPAR = atof(optarg);
        break;

      case 'E':
        MIN_DETECT = atof(optarg);
        break;

      case 'F':
        C[0] = atof(optarg);
        break;

      case 'G':
        C[1] = atof(optarg);
        break;

      case 'H':
        C[2] = atof(optarg);
        break;

      case 'I':
        C[3] = atof(optarg);
        break;

      case 'U':
        B = atof(optarg);
        break;

      case 'V':
        EBAR = atof(optarg);
        break;

      case 'W':
        R = atof(optarg);
        break;
        
      case 'h':
        printMenu(argv[0]);
        printOptionsMenu();
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

  // check that a timeHist and a tempHist file have been provided.
  if(timeHist == "" || tempHist == "") {
    printMenu(argv[0]);
    exit(EXIT_FAILURE);
  }
  
  // program variables NOT set from the command line

  // local variables for array dimensioning
  int timeRows = 0, timeCols = 0, tempRows = 0, tempCols = 0;
  
  double **timeHistArray = NULL,
         **tempHistArray = NULL, 
         **xHistArray = NULL,
         **yHistArray = NULL, 
         **zHistArray = NULL;
				     
	// arrays for calculated age values of ahe, zhe, aft, & zft
  double *aheAgesArray = NULL, 
         *zheAgesArray = NULL, 
         *aftAgesArray = NULL,
         *zftAgesArray = NULL;
  
  // call countRows() & countCols() and allocate dynamic arrays accordingly.
  // count timeHist, tempHist, xHist, yHist, & zHist
  timeRows = countRows(timeHist);
  timeCols = countCols(timeHist);
  tempRows = countRows(tempHist);
  tempCols = countCols(tempHist);

  cout << "timeRows: " << timeRows << ", timeCols: " << timeCols <<
    ", tempRows: " << tempRows << ", tempCols: " << tempCols << endl;

  // initialize the 2d arrays for all the data
  // that is to be read from the input files.
  init2dArray(xHistArray, tempRows, tempCols);
  init2dArray(yHistArray, tempRows, tempCols);
  init2dArray(zHistArray, tempRows, tempCols);
  init2dArray(timeHistArray, timeRows, timeCols);
  init2dArray(tempHistArray, tempRows, tempCols);
	
  // initialize the 1d arrays for storing the output
  // from each thermochronometer calculation.
  init1dArray(aheAgesArray, tempCols);
  init1dArray(zheAgesArray, tempCols);
  init1dArray(aftAgesArray, tempCols);
  init1dArray(zftAgesArray, tempCols);

  // read the coordinate files if the proper flags have
  // already been set.
  if(flgXCoord) {
    read2D(xHist, tempRows, tempCols, xHistArray);
  }
  if(flgYCoord) {
    read2D(yHist, tempRows, tempCols, yHistArray);
  }
  if(flgZCoord) {	
    read2D(zHist, tempRows, tempCols, zHistArray);
  }

  // calculate ages depending on the flags: flgAHe, flgZHe, flgZft, and flgAft
  if(flgAHe) {
    cout << "calculating AHe" << endl;
    read2D(timeHist, timeRows, timeCols, timeHistArray);
    read2D(tempHist, tempRows, tempCols, tempHistArray);
    
    get_he_age(timeHistArray, tempHistArray, (tempRows - 1), tempCols, aa, ad, ae, au, at, aheAgesArray);
  }

  if(flgZHe) {
    cout << "calculating ZHe" << endl;
    read2D(timeHist, timeRows, timeCols, timeHistArray);
    read2D(tempHist, tempRows, tempCols, tempHistArray);
    
    get_he_age(timeHistArray, tempHistArray, (tempRows - 1), tempCols, za, zd, ze, zu, zt, zheAgesArray);
  }

  if(flgAft) {
    cout << "calculating AFt" << endl;
    read2D(timeHist, timeRows, timeCols, timeHistArray);
    read2D(tempHist, tempRows, tempCols, tempHistArray);
    
    get_aft_age(timeHistArray, tempHistArray, timeRows, tempCols, aftAgesArray);
  }

  if(flgZft) {
    cout << "calculating ZFt" << endl;
    read2D(timeHist, timeRows, timeCols, timeHistArray);
    read2D(tempHist, tempRows, tempCols, tempHistArray);
    
    get_zft_age(timeHistArray, tempHistArray, (timeRows - 1), tempCols, zftAgesArray);
  }

  
  // write the data from the ahe, zhe, aft, and zft 1D arrays to output files.
  // two output files will be created in the default output directory, one with
  // a formatted header and the other with just raw data.
	writeOutput(xHistArray, yHistArray, zHistArray, aheAgesArray, zheAgesArray, aftAgesArray,
	            zftAgesArray, tempRows, tempCols, outputFile);
								 
	writeTecPlot(xHistArray, yHistArray, zHistArray, aheAgesArray, zheAgesArray, aftAgesArray,
	             zftAgesArray, tempRows, tempCols, outputFile);
	   
  return(0);
}
