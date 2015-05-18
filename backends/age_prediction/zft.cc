#include <iostream>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

// function prototypes
int get_zft_age(double** timein,double** tempin,const int m,const int n,double zftAgesArray[]);
int pass_close(double ctemp,double** temp,double** time,int m,int currSample,double* age);
int rtsafe(double tdot,double* ctemp);
int fun_eval(double tc,double* fn,double* df,double tdot);

// FUNCTION: Determine The ZFT Age
int get_zft_age(double** timein,double** tempin,const int m,const int n,double zftAgesArray[])
{
  extern double MILYEARS;

  int i = 0;
  int iabove = 0;
  int icatch = 0;
  int out = 0;
  int imax = m;
  double tmax = 0.0;
  double tdot = 0.0;
  double ctemp = 0.0;
  double age = 0.0;

  // convert temperature values to Kelvin
  for(int i = 0;i < m;i++)
  {
    for(int j = 0; j < n;j++)
    {
      tempin[i][j] = tempin[i][j] + 273.0;
    }
  }

  for(int currSample = 0; currSample < n; currSample++)
  {
    // reset values
    iabove = 0;	
    icatch = 0;
    out = 0;
    imax = m;
    
    tmax = 0.0;
    tdot = 0.0;
    ctemp = 0.0;
    age = 0.0;

    // determine the closure temp by assuming a linear cooling rate between 200 and 300 deg C
    for(i = m;i > -1;i--)
    {
      if(tempin[i][currSample] > 473.0)
      {
        iabove = i;
        icatch = 1;
        break;
      }
    }
    
    // if temp never goes past 200 C, set age as unreset - i.e. grain will have age of MILYEARS	
    if(icatch == 0)
    {
      zftAgesArray[currSample] = MILYEARS + timein[m][0];
      continue;
    }	
    else
    {
      for(i = iabove;i > -1;i--)
      {	
        // look for time when temp goes above 300 C
        if(tempin[i][currSample] > 573.0)
        {
          imax = i;
          break;
        }
      }
      
      // if temp doesn't pass 300 C, look to find the maximum temp
      if(imax == m)
      {
        tmax = tempin[iabove][currSample];
        for(i = iabove;i > -1;i--)
        {
          if(tempin[i][currSample] > tmax)
          {
            tmax = tempin[i][currSample];
            imax = i;
          }	
        }
        
        // if there is only one temp value past 200 C, set age as unreset
        if(imax == m)
        {
          zftAgesArray[currSample] = MILYEARS + timein[m][0];
          continue;
        }	
      }	
    }
    
    // define cooling rate between 200 C and value determined above (either 300 C or max temp under 300 C).
    // multiply by -1 so that the value should be positive (time is model time)
    tdot = -1.0 * (tempin[imax][currSample] - tempin[iabove][currSample]) / (timein[imax][0] - timein[iabove][0]);	
    
    if(tdot < 0.0)
    { 
      fprintf(stderr,"\n#########\n########  ERROR: Negative Cooling Rate \n#########");
    }	

    // determine the closure temp from cooling rate     
    out = rtsafe(tdot,&ctemp);                  

    // determine the age from closure temp
    pass_close(ctemp,tempin,timein,m,currSample,&age);

    cout << "age: " << age << ", out: " << out << endl;
    
    zftAgesArray[currSample] = age;
  }
  
  return(0);
}


//  FUNCTION: Determine The Time Since A Particle Has Passed Through Closure Temp For Zircon
int pass_close(double ctemp,double** temp,double** time,int m,int currSample,double* age)
{
  extern double MILYEARS;

  int i = 0;
  int reset = 0;

  for(i = m;i > -1;i--)
  {
    if(temp[i][currSample] >= ctemp)
    {
      reset = 1;
      *age = time[m][0] - time[i][0];
      break;
    }
  }

  cout << "reset: " << reset << ", ctemp: " << ctemp << endl;
    
  if(reset == 0)
  {
    *age = MILYEARS + time[m][0];
  }	

  return(0);
}


// FUNCTION: Use A Hybrid Newton-Ralphson Method To Find Root Of Function fun_eval As Per Numerical Recipies In C
int rtsafe(double tdot,double* ctemp)
{
  int j = 0;
  int out = 0;
  int MAXIT = 400;
  double df = 0.0;
  double dx = 0.0;
  double dxold = 0.0;
  double f = 0.0;
  double fh = 0.0;
  double fl = 0.0;
  double temp = 0.0;
  double xh = 0.0;
  double xl = 0.0;
  double rts = 0.0;
  double x1 = 473.00;
  double x2 = 600.00;
  double xacc = 0.5;

  out = fun_eval(x1,&fl,&df,tdot);
  out = fun_eval(x2,&fh,&df,tdot);

  if (fl == 0.0)
  {
    *ctemp = x1;
    return(0); 
  }	

  if (fh == 0.0)
  {
    *ctemp = x2;
    return(0); 
  }	

  if (fl < 0.0)
  {
    xl = x1; 
    xh = x2; 
  } 
  else
  { 
    xh = x1; 
    xl = x2; 
  } 

  rts = 0.5 * (x1 + x2); 
  dxold = fabs(x2 - x1); 
  dx = dxold; 
  out = fun_eval(rts,&f,&df,tdot);

  for (j = 1;j <= MAXIT;j++)
  {
    if ((((rts - xh) * df - f) * ((rts - xl) * df - f) > 0.0) || (fabs(2.0 * f) > fabs(dxold * df)))
    { 
      dxold = dx; 
      dx = 0.5 * (xh - xl); 
      rts = xl + dx; 
      if (xl == rts)
      {
        *ctemp = rts;
        return(0);
      }	
    } 
    else
    { 
      dxold = dx; 
      dx = f / df; 
      temp = rts; 
      rts -= dx; 
      if (temp == rts)
      {
        *ctemp = rts;
        return(0); 
      }	
    } 
    
    if (fabs(dx) < xacc)
    {
      *ctemp = rts;
      return(0);
    }	
    
    out = fun_eval(rts,&f,&df,tdot);
    
    if (f < 0.0)
    {
      xl = rts;
    }
    else
    {
      xh = rts; 
    }

  }
    
  fprintf(stderr,"\n\n#################  ERROR: Maximum Number Of Iterations Exceeded In Rtsafe  #################\n\n"); 
  *ctemp = 1000.0;
    
  return(0);
}



// FUNCTION: Evaluate The Closure Temperature And The Derivative Of The Eqn At The Given Point For Use In The Root Finding Algorithm
int fun_eval(double tc,double* fn,double* df,double tdot)
{
  // define function constants (B, EBAR, & R) as from Batt and Brandon (2001)

  extern double B;
  extern double EBAR;
  extern double R;

  // closure temp function as defined by Dodson (1979) (note: +tdot on RHS since cooling rate is defined as positive)
  *fn = (-R * pow(tc,2)) / (EBAR * B * exp(EBAR / (R * tc))) + tdot;
  // derivative of above function w/rt tc
  *df = -exp(-EBAR / (R * tc)) / B - (2 * exp(-EBAR / (R * tc)) * R * tc) / (B * EBAR);

  return(0);
}	
