#define HE_NODES 202
#define HE_STEPS 700002
#define PAD_STEPS 20				// number of timesteps in which to do the padding

#include <iostream>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>

using namespace std;


// function prototypes
int get_he_age(double **thist, double **temp, const int m, const int n, double a, double D0, double Ea, double U, double Th, double aheAgesArray[]);							
int calc_he_age(double* HeProd, double* HeFin, double He[], double r[], double L[], double A, double syr, double HeAge[], int t);											
int assem_gs(double rhs[], double Gstiff[][4], double P[][4], double Estiff[][3], double L[], double dt[], double D[], int t);			 
int interp_Thist(double* temp, double* thist, int m_pad, double T[], double time[], double dt[], double deltat, double syr);	 		 
int solve(double Gstiff[][4], double rhs[], double He[], double gam[]);
int apply_bc(double Gstiff[][4], double rhs[], double r[]);		 
int assem_es(int n1, double L[], double dt[], double D[], double Estiff[][3], int t);					 
void init_variables(double r[], double L[], double *temp, double He[], int m_pad);
int assem_rhs(double L[], double P[][4], double Fn1, double Fn2, double* A_temp, double r[],
              double rhs[], double He[], double lamda238, double lamda232, double U, double Th,
							double dt[], int t);


// FUNCTION: Predict The (U-Th) / He age
int get_he_age(double** thist, double** temp, const int m, const int n, double a, double D0,
               double Ea, double U, double Th, double aheAgesArray[])
{
  // all arrays will be dynamically set w/ last value in (time_hist file / deltat).

  // extern variables
  extern double MILYEARS;
  extern double TIME_STEP;

  // physical constants
  const double R = 0.0019872;       		// universal gas constant, units = 0.001987 kcal mol-1 k-1.
  const double lamda238 = 4.95e-18;   	// decay rate constant of U238, units= s-1.
  const double lamda232 = 1.56e-18;   	// decay rate constant of Th232, units = s-1.
  const double syr = 3.1557e7;       		// number of seconds per year.

  int t = 1;				// indexing variable for timesteping through algorithm.
  int ndt = 0;      // number of timestep intervals stored in dt[] array.
  int ntT;

  double A = 0.0;          		  // helium produced from radioactive decay, units = ppm s-1.
  double Fn1 = 0.0;       	    // node i force vector contribution from heat production
  double Fn2 = 0.0;   		      // node j force vector contribution from heat production
  double deltat = TIME_STEP;		// time step size in FEM calculation, units = years. Converted to sec in program.
  // double oldest = 0.0;   	  // number of timesteps and temperatures in new, interpolated thermal history arrays.
  double HeProd = 0.0;					// total helium produced in sphere from radioactive decay of U and Th.
  double HeFin = 0.0;						// final Helium age (M.y.) at end of simulations (same as HeAge[ndt]).
  double time_int = 0.0;

  double He[HE_NODES];
  double r[HE_NODES];
  double L[HE_NODES - 1];
  double rhs[HE_NODES];
  double gam[HE_NODES];

  double Estiff[3][3];
  double Gstiff[HE_NODES][4];
  double P[HE_NODES][4];

  double* temp_pad;
  double* time_pad;

  static double HeAge[HE_STEPS];
  static double T[HE_STEPS];
  static double dt[HE_STEPS];
  static double time[HE_STEPS];
  static double D[HE_STEPS];

  /*
  ARRAY DEFINITIONS

  Values Related To The PDE:   
  - thist = Input coarsely defined time history, read from file in My, and converted to seconds within this program.
  - temp = Input coarsely defined temperature history, file units = C. This variable is converted to Kelvin within this program.
  - He = 1D Nodal helium concentration. Note that this is actually the transformed variable U in the pde and U = r * He, He units = ppm.
  - HeAge = Sample age at each timestep, units = M.y. 
  - r = Dimensionless nodal position across diffusion domain,units = dimensionless
  - T = Refined (interpolated) temperature history used in FEM calculation, units = Kelvin.  
  - dt = Length of time between each sequential items in time[] array.
  - time = Refined (interpolated) time history used in FEM calculation, units = seconds
  - D = Transient apatite grain diffusivity normalized by the square of the domain size, units= sec-1
  - L = Dimensionless distance across each element. Current implementation of this program uses uniform element lengths.

  Values Related To Indexing Or FEM Solution:
  - Estiff = Element stiffness matrix 
  - Gstiff = Global stiffness matrix stored as 3 column matrix where each column represents a diagonal in the tri-diagonal matrix.
  - rhs = RHS side of equation solved (transient and source terms)
  - C = Global capacitance matrix, used for transient solution.
  - P = Composite term on RHS
  - gam = Parameter used in tridiagonal solution algorithm.

  SCALAR DEFINITIONS

  Values Related To The PDE:
  - m = Number of temperatures (rows) in input file
  - n = Number of samples (cols) in input file
  - a = Diffusion domain (apatite grain) size, units = um (0.01 cm = 100 um) (Passed from Terra.GUI).
  - D0 = Initial apatite diffusivity at infinite temperature (Farley, 2000, p. 2910), units = um^2/sec (Passed from Terra.GUI).
  - Ea = Activation energy for apatite, See Farley 2000, p. 2910, units = kcal/mol (Passed from Terra.GUI).
  - U = Concentration of uranium in apatite, units = ppm (Passed from Terra.GUI).
  - Th = Concentration of Thorium in apatite, units = ppm (Passed from Terra.GUI).
  */

  // time stepping interval
  time_int = MILYEARS / PAD_STEPS;

  // pad the temp_history to MILYEARS in PAD_STEPS in order to see unreset ages

  time_pad = new double[m + PAD_STEPS + 2];
  temp_pad = new double[m + PAD_STEPS + 2];

  // set initial time_pad index to '0'
  time_pad[0] = 0.00;

  // determine time_pad values
  for(t = 1; t < PAD_STEPS + 1; t++) {
    time_pad[t] = thist[1][0] + time_int * (double)(t - 1);
  }

  for(t = 0; t < m; t++) {
    time_pad[1 + t + PAD_STEPS] = thist[t + 1][0] + MILYEARS;
  }

  // initialize variables and time_pad
  int m_pad = m + PAD_STEPS;
  deltat = deltat * syr;

  for(t = 1; t <= m_pad; t++) {
      time_pad[t] = time_pad[t] * syr * 1000000.0;
  }

  // loop for each sample ('n' times) to determine the sample's age
  for(int currSample = 0; currSample < n; currSample++)
  {
    // set initial time_pad index to '0'
    temp_pad[0] = 0.00;

    for(t = 1; t <= PAD_STEPS; t++) {
      temp_pad[t] = temp[1][currSample];
    }

    for(t = 0; t < m; t++) {
      temp_pad[1 + t + PAD_STEPS] = temp[t + 1][currSample];
    }

    // initialize variables
    init_variables(r,L,temp_pad,He,m_pad);

    // calculate timestep size and interpolate a new temperature history.
    // if the user defined (input file) timestep is too large
    ntT = interp_Thist(temp_pad,time_pad,m_pad,T,time,dt,deltat,syr);
    ndt = ntT - 1;

    // loop over timesteps and calculate FEM solution for He concentration
    for(t = 1; t <= ndt; t++)
    {
      // update material properties
      D[t] = D0 / (pow(a,2.0)) * exp(-1.0 * (Ea / (R * ((T[t] + T[t + 1]) / 2.0))));
      assem_gs(rhs, Gstiff, P, Estiff, L, dt, D, t);
      assem_rhs(L,P,Fn1,Fn2,&A,r,rhs,He,lamda238,lamda232,U,Th,dt,t);
      apply_bc(Gstiff,rhs,r);
      solve(Gstiff,rhs,He,gam);
      calc_he_age(&HeProd,&HeFin,He,r,L,A,syr,HeAge,t);
    }

    // append calculated age to sample_ages_array
    aheAgesArray[currSample] = HeFin;

  }

  delete [] time_pad;
  delete [] temp_pad;

  return(0);
}



// FUNCTION: Calculate He Age
int calc_he_age(double *HeProd, double *HeFin, double He[], double r[], double L[], double A,
                double syr, double HeAge[], int t)
{
  double total = 0.0;

  *HeProd = 0.0;

  for(int i = 2; i <= HE_NODES - 2; i++)
  {
    total = total + He[i] * r[i] * L[i];
  }
      
  total = 4.0 * 3.1415926 * total;
  *HeProd = (4.0 / 3.0) * 3.1415926 * A;

  // Convert to My 
  HeAge[t] = ((total / ((4.0 / 3.0) * 3.1415926 * A)) / syr) / 1e6;

  *HeFin = HeAge[t];

  // Debug Code
  // fprintf(stderr,"\n%10.5e %10.5e %10.5e",total,A,syr);
  return(0);
}



// FUNCTION: Solve Tridiagonal System 
int solve(double Gstiff[][4], double rhs[], double He[], double gam[])
{
  int j = 0;
  double bet = 0.0;

  bet = Gstiff[1][2];
  He[1] = rhs[1] / bet;

  for(j = 2; j <= HE_NODES - 1; j++)
  {
    gam[j] = Gstiff[j - 1][3] / bet;
    bet = Gstiff[j][2] - Gstiff[j][1] * gam[j];
    He[j] = (rhs[j] - Gstiff[j][1] * He[j - 1]) / bet;
  }

  for(j = HE_NODES - 2; j >= 1; j--) {
    He[j] = He[j] - gam[j + 1] * He[j + 1];
  }

  return(0);
}



// FUNCTION: Apply Boundary Conditions
int apply_bc(double Gstiff[][4], double rhs[], double r[])
{
  // Apply zero He concentration to outer edge of domain
  Gstiff[HE_NODES - 1][2] = 1.0;
  Gstiff[HE_NODES - 1][1] = 0.0;
  rhs[HE_NODES - 1] = r[HE_NODES - 1] * 0.0;

  // Assign zero flux at center node, note that zero flux = zero scalar in transformed coordinate system.
  Gstiff[1][2] = 1.0;
  Gstiff[1][3] = 0.0;
  rhs[1] = 0.0;

  return(0);
}



// FUNCTION: Assemble The RHS
int assem_rhs(double L[], double P[][4], double Fn1, double Fn2, double *A_temp, double r[],
              double rhs[], double He[], double lamda238, double lamda232, double U, double Th,
							double dt[], int t)
{
  int n1 = 0;
  int n2 = 0;
  int nn = 0;
  int ie = 0;
  int n = 0;
  double c11 = 0.0;
  double c22 = 0.0;
  double A = 0.0;

  // Assemble global capacitence matrix
  for(ie = 1;ie <= HE_NODES - 2;ie++)
  {
    n1 = ie;
    n2 = ie + 1;

    // Lumped formulation
    c11 = L[ie] / 2.0;
    c22 = c11;
    P[n1][2] = c11 + P[n1][2];
    P[n1][3] = P[n1][3];
    P[n2][1] = P[n2][1];
    P[n2][2] = c22 + P[n2][2];

    // Add Helium Term To RHS

    // TEHMASP - Note this A term is where variable Helium production across the grain can be implemented
    // (recall I drew a picture of this for you).  To implement variable heat production A would need to be
    // A[he_nodes] and and U and Th would need to be U[he_nodes] and Th[he_nodes].  We dont need to implement
    // this now, but please keep this note in here and we'll add this on once we have this basic version working.

    A = (8.0 * lamda238 * U + 6.0 * lamda232 * Th);
    Fn1 = dt[t] * (A / L[n1]) * ((pow(r[n2], 3.0) / 6.0) - (r[n2] * pow(r[n1], 2.0) / 2.0) + (pow(r[n1], 3.0) / 3.0));
    Fn2 = dt[t] * (A / L[n1]) * ((pow(r[n1], 3.0) / 6.0) - (r[n1] * pow(r[n2], 2.0) / 2.0) + (pow(r[n2], 3.0) / 3.0));
    rhs[n1] = rhs[n1] + Fn1;
    rhs[n2] = rhs[n2] + Fn2;
  }	

  // Multiple capacitance matrix with previous (current) He concentration and complete assembly of RHS
  rhs[1] = rhs[1] + P[1][2] * He[1] + P[1][3] * He[2];

  for(n = 2;n <= HE_NODES - 2; n++)
  {
    rhs[n] = rhs[n] + P[n][1] * He[n - 1] + P[n][2] * He[n] + P[n][3] * He[n + 1];
  }

  nn = HE_NODES - 1;
  rhs[nn] = rhs[nn] + P[nn][1] * He[nn - 1] + P[nn][2] * He[nn];
  *A_temp = A;

  return(0);
}	



// FUNCTION: Assemble The Element Stiffness
int assem_es(int n1, double L[], double dt[], double D[], double Estiff[][3], int t)
{
  double c = 0.0;
  double k = 0.0;

  // Element capacity matrix
  c = L[n1] / 2.0;

  // Element conductance matrix
  k = dt[t] * (D[t] / L[n1]);

  // Lumped stiffness matrix
  Estiff[1][1] = c + k;
  Estiff[1][2] = -1.0 * k;
  Estiff[2][1] = Estiff[1][2];
  Estiff[2][2] = Estiff[1][1];

  return(0);
}



// FUNCTION: Assemble Global Stiffness Matrix
int assem_gs(double rhs[], double Gstiff[][4], double P[][4], double Estiff[][3], double L[],
             double dt[], double D[], int t)
{
  int i = 0;
  int j = 0;
  int ie = 0;
  int n1 = 0;
  int n2 = 0;

  // Clear old values
  for(i = 1; i <= HE_NODES - 1; i++)
  {
      rhs[i]=0.0;
      for(j = 1; j <= 3; j++)
      {
      	Gstiff[i][j] = 0.0;
      	P[i][j] = 0.0;
      }
  }	

  // Assemble new G stiffness matrix
  for(ie = 1; ie <= HE_NODES - 2;ie++)
  {
      n1 = ie;
      n2 = ie + 1;
      assem_es(n1, L, dt, D, Estiff, t);
      
      // Add element stiff to global
      Gstiff[n1][2] = Estiff[1][1] + Gstiff[n1][2];
      Gstiff[n1][3] = Estiff[1][2] + Gstiff[n1][3];
      Gstiff[n2][1] = Estiff[2][1] + Gstiff[n2][1];
      Gstiff[n2][2] = Estiff[2][2] + Gstiff[n2][2];		
  }

  return(0);
}	



// FUNCTION: Interpolate The Temp History
int interp_Thist(double* temp_pad, double* thist_pad, int m_pad, double T[], double time[], double dt[],
                 double deltat, double syr)
{
  int i = 1;
  int j = 1;
  int k = 0;
  int numsteps = 0;
  int max = 0;
  double tinterval = 0.0;
  double slope = 0.0;
  double b = 0.0;

  // Loop Through The Iput Time And Temps
  for (i = 1;i <= m_pad - 1;i++)
  {  		
    // Slope and intercept for linear interp of temperature
    slope = (temp_pad[i + 1] - temp_pad[i]) / (thist_pad[i + 1] - thist_pad[i]);
    b = temp_pad[i] - slope * thist_pad[i];
    
    // Determine number of sub timesteps until next input time
    tinterval = thist_pad[i + 1] - thist_pad[i];
      
    if(tinterval < deltat)
    {
      fprintf(stderr,"\n ########   ERROR   ###########");
      fprintf(stderr,"\n Timestep size in input file is less than defined for soln");
      fprintf(stderr,"\n Program Defined = %f    Plasti input = %f\n\n\n", deltat / 1000000.0 / syr, tinterval / 1000000.0 / syr);
      exit(EXIT_FAILURE);
    }

    numsteps = (int)ceil(tinterval / deltat);

    for (j = 1;j <= numsteps;j++)
    {
      if(j == 1) 
      {
        k = k + 1;
        T[k] = temp_pad[i];
        time[k] = thist_pad[i];
      }	
      else if (j > 1 && j < numsteps)
      {
        k = k + 1;
        time[k] = time[k - 1] + deltat;
        T[k] = slope * time[k] + b;
      }
      else
      {
        if (thist_pad[i + 1] == time[k - 1] + deltat && i != (m_pad - 1))
        {
          break;
        }
        else if (thist_pad[i + 1] == (time[k - 1] + deltat) && i == (m_pad - 1))
        {
          k = k + 1;
          time[k] = time[k - 1] + deltat;
          T[k] = slope * time[k] + b; 
        }
        else
        {
          k = k + 1;
          time[k] = time[k - 1] + deltat;
          T[k] = slope * time[k] + b;
        }	
      }	
    }	
  }	
      
  // Make sure final/last value in input and interpolated temp history match
  if(time[k] != thist_pad[m_pad]) {
    k = k + 1;
    time[k] = thist_pad[m_pad];
    T[k] = temp_pad[m_pad];
  }	

  max = k;
      
  // Generate vector dt[] for size of each timestep interval in time[]
  for (k = 2;k <= max;k++) {
    dt[k - 1] = time[k] - time[k - 1];
  }
      
  return(max);
}



// FUNCTION: Initialize variables
void init_variables(double r[], double L[], double* temp_pad, double He[], int m_pad)
{
  int i = 0;
  double dr = 0.0;
	
  for(i = 1; i <= m_pad; i++)
  {
    temp_pad[i] = temp_pad[i] + 273.0;
  }

  dr = 1.0 / (HE_NODES - 2);
  r[1] = 0.0;

  for (i = 2; i <= HE_NODES - 1; i++)
  {
    r[i] = r[i - 1] + dr;
  }

  r[HE_NODES - 1] = 1.0;

  // Initialize He Arrays And Other Stuff
  for(i = 1; i <= HE_NODES - 2; i++)
  {
    L[i] = r[i + 1] - r[i];       // Dimensionless Distance Across Each Element 
    He[i] = 0.0;
  }

  He[HE_NODES - 1] = 0.0;
  
}
