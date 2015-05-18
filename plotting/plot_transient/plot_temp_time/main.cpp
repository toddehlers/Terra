#include <qapplication.h>
#include <qstring.h>
#include <qfile.h>
#include <qtextstream.h>

#include <iostream>
#include <cmath>

#include "plotter.h"

#define MAX_ARGS 7 


int main(int argc, char *argv[])
{
	float temp;
	int count=0;
	double y_temp;
	QString temp1, temp2;

	QApplication a(argc, argv);
	PlotSettings settings;
	Plotter p;

  if(argc != MAX_ARGS)
  {
    std::cerr<<"Bad command line arguments - FIXME!!!\n";
    exit(EXIT_FAILURE);
  }
	
	p.setCaption("TERRA - 1D TRANSIENT EROSION-ISOTHERM DEPTH HISTORY - UNIVERSITY OF MICHIGAN");
	p.setGeometry(300, 300, 1000, 600);
  
  a.setMainWidget(&p);
 	CurveData data[4];
	
	y_temp = atoi(argv[1]);
	p.exhumation = argv[2];
  
	p.iso_1 = argv[3];
	p.iso_2 = argv[4];
	p.iso_3 = argv[5];
	p.iso_4 = argv[6];

	QFile fin1("../output/transient/temp_time/transient_isotherm1.dat");
	QFile fin2("../output/transient/temp_time/transient_isotherm2.dat");
	QFile fin3("../output/transient/temp_time/transient_isotherm3.dat");
	QFile fin4("../output/transient/temp_time/transient_isotherm4.dat");

	fin1.open(IO_ReadOnly);
	QTextStream f1(&fin1);
	while(!f1.atEnd())
	{
		f1 >> temp1 >> temp2;	
		if(temp2 != "NaN")
		{
			data[0].push_back(atof(temp1));
			data[0].push_back(atof(temp2));
		}
	}
	fin1.close();

	fin2.open(IO_ReadOnly);
	QTextStream f2(&fin2);
	while(!f2.atEnd())
	{
		f2 >> temp1 >> temp2;	
		if(temp2 != "NaN")
		{
			data[1].push_back(atof(temp1));
			data[1].push_back(atof(temp2));
		}
	}
	fin2.close();

	fin3.open(IO_ReadOnly);
	QTextStream f3(&fin3);
	while(!f3.atEnd())
	{
		f3 >> temp1 >> temp2;	
		if(temp2 != "NaN")
		{
			data[2].push_back(atof(temp1));
			data[2].push_back(atof(temp2));
		}
	}
	fin3.close();
	
	fin4.open(IO_ReadOnly);
	QTextStream f4(&fin4);
	while(!f4.atEnd())
	{
		f4 >> temp1 >> temp2;	
		if(temp2 != "NaN")
		{
			data[3].push_back(atof(temp1));
			data[3].push_back(atof(temp2));
		}
		count++;
	}
	fin4.close();
	
	fin4.open(IO_ReadOnly);
	QTextStream f(&fin4);
	f >> temp1 >> temp2;
	fin4.close();

	settings.minX = 0;
	settings.maxX = static_cast<int>(count - 1);

	temp = floor(atof(temp2) / 1000) - 2;

	settings.minY = static_cast<int>(temp * 1000);
	settings.maxY = 0;

	p.setPlotSettings(settings);

	p.setCurveData(0, data[0]);
	p.setCurveData(1, data[1]);
	p.setCurveData(2, data[2]);
	p.setCurveData(3, data[3]);
	p.show();
  return a.exec();
}

