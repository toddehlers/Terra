#ifndef __UTILS_H__  
#define __UTILS_H__

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>

#define ERR_HEAP "ERROR: Heap Allocation Error"
#define ERR_READ "ERROR: Reading file: "

using namespace std;

// FUNCTION: Read fileName Into A 2D Array
bool read2D(string fileName, int rows, int cols, double **inArray)
{
  ifstream file;
     
  // open file & check for errors 
  file.open(fileName.c_str());
      
  if(!file) {
    file.clear();
      
    cerr<<ERR_READ<<fileName<<endl;
    exit (EXIT_FAILURE);
  }
  else {
    for(int i = 0; i < rows; i++) {
      
      for(int j = 0; j < cols; j++) {
        file>>inArray[i][j];
      }
    }
  }
    
  file.close();    
  return(EXIT_SUCCESS);    
}


// FUNCTION: Count The Number Of Rows From fileName
int countRows(string fileName)
{
  int rowCount = 0;
  
  string tmpStr;
  ifstream file;
      
  file.open(fileName.c_str());
   
  if(!file) {
    file.clear();
      
    cerr<<ERR_READ<<fileName<<endl;
    exit(EXIT_FAILURE);
  }
  else { 
     while(!file.eof())
     { 
        getline(file, tmpStr);
       
        if(tmpStr != "")
           rowCount++;
     }
  }

  file.close();     
  return(rowCount);
}



// FUNCTION: Count The Number Of Cols From fileName
int countCols(string fileName)
{
  int colCount = 0;
  char tmpChr = '0';
  
  ifstream file;

  file.open(fileName.c_str());

  if(!file) {
    file.clear();
    
    cerr<<ERR_READ<<fileName<<endl;
    exit(EXIT_FAILURE);
  }
  else {
    // count assuming all floats have a decimal point
    while(tmpChr != '\n') {
      file.get(tmpChr);
           
      if(tmpChr == '.')
      {
        colCount++;
      }
    }
  }

  file.close();     
  return(colCount);
}


// FUNCTION: Allocate 1d Double Array
void init1dArray(double *&inArray, int size)
{
  int i;
  
  try {
    inArray = new double[size];
  }
  catch(bad_alloc) {
    cerr<<ERR_HEAP<<endl;
    exit(EXIT_FAILURE);
  }

  for(i = 0; i < size; i++) {
    inArray[i] = 0;
  }
}


// FUNCTION: Allocate 2d Double Array
void init2dArray(double **&inArray,int size)
{
  int i, j;
  
  try {
    inArray = new double*[size];
    for(i = 0; i < size; i++) {
      inArray[i] = new double[size];
    }
  }
  catch(bad_alloc) {
    cerr<<ERR_HEAP<<endl;
    exit(EXIT_FAILURE);
  }

  // initialize array elements to zero
  for(i = 0; i < size; i++) {
    for(j = 0; j < size; j++)
      inArray[i][j] = 0;
  }
}


// FUNCTION: Allocate 2d Double Array Of Mixed Size
void init2dArray(double **&inArray,int rows, int cols)
{
  int i, j;

  try {
    inArray = new double*[rows];  
    for(i = 0; i < rows; i++) {
      inArray[i] = new double[cols];
    }
  }
  catch(bad_alloc) {
    cerr<<ERR_HEAP<<endl;
    exit(EXIT_FAILURE);
  }

  // initialize array elements to zero
  for(i = 0; i < rows; i++) {
    for(j = 0; j < cols; j++)
      inArray[i][j] = 0;
  }
}


// FUNCTION: Print command line options list
void printMenu(const char *fileName)
{
  cout<<"Age Prediction\n";
  cout<<"Usage: "<<fileName<<" [OPTION] [FILE]... [OPTION] [ARG]...\n\n";

  cout<<"INPUT FILES:\n";
	cout<<"-t   time history (required)\n";
  cout<<"-T   temperature history (required)\n";
  cout<<"-x   x-coordinate history\n";
  cout<<"-y   y-coordinate history\n";
  cout<<"-z   z-coordinate history\n";
  cout<<"-o   output file\n\n";
	
	cout<<"-h   display the options menu\n";
  cout<<"-v   display the version information\n";
}


// FUNCTION: Print command line options list
void printOptionsMenu()
{
  cout<<"\nOPTIONS:\n";
  cout<<"-1   disable ahe age prediction\n";
  cout<<"-2   disable zhe age prediction\n";
  cout<<"-3   disable aft age prediction\n";
  cout<<"-4   disable zft age prediction\n\n";

  cout<<"-Y   time padding for thermochronometer work (My)\n";
  cout<<"-S   time step size in FEM calculation (years)\n\n";

  cout<<"-j   ahe initial diffusivity (um^2/sec)\n";
  cout<<"-k   ahe activation energy (kcal/mol)\n";
  cout<<"-l   ahe grain radius (nm)\n";
  cout<<"-m   ahe uranium concentration (ppm)\n";
  cout<<"-n   ahe thorium concentration (ppm)\n\n";

  cout<<"-J   zhe initial diffusivity (um^2/sec)\n";
  cout<<"-K   zhe activation energy (kcal/mol)\n";
  cout<<"-L   zhe grain radius (nm)\n";
  cout<<"-M   zhe uranium concentration (ppm)\n";
  cout<<"-N   zhe thorium concentration (ppm)\n\n";

  cout<<"-A   aft alpha value\n";
  cout<<"-B   aft beta value\n";
  cout<<"-C   aft time step value\n";
  cout<<"-D   aft dpar value\n";
  cout<<"-E   aft minimum detectable normalized track length\n";
  cout<<"-F   first aft coefficient\n";
  cout<<"-G   second aft coefficient\n";
  cout<<"-H   third aft coefficient\n";
  cout<<"-I   fourth aft coefficient\n\n";

  cout<<"-U   zft closure material constant (My)\n";
  cout<<"-V   zft closure activation energy (kcal/mol)\n";
  cout<<"-W   zft closure gas constant (kcal/(mol*deg))\n";
}

#endif /* __UTILS_H__ */
