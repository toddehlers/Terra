#include <qapplication.h>
#include <qstring.h>
#include <qfile.h>
#include <qtextstream.h>

#include <iostream>

#include "plotter.h"

#define MAX_ARGS 8

int main(int argc, char *argv[])
{
	int temp;
	double x_temp, y_temp;
	QString temp1, temp2;

	QApplication a(argc, argv);
	PlotSettings settings;
	Plotter p;
	
	p.setCaption("TERRA - 2D PERIODIC TOPOGRAPHY MODEL-ISOTHERMS & TOPOGRAPHY - UNIVERSITY OF MICHIGAN");
	p.setGeometry(300, 300, 1000, 600);
  
  a.setMainWidget(&p);
 	CurveData data[5];
	
  if(argc != MAX_ARGS)
  {
    std::cerr<<"Bad command line arguments - FIXME!!!\n";
    exit(EXIT_FAILURE);
  }
      
	x_temp = atoi(argv[1]);
	y_temp = atoi(argv[2]);

	settings.minY = -1 * y_temp;
	settings.maxY = 5000;
	temp = (int)y_temp + 5000;    // FIXME, converting to 'int' from 'double'
	
  while((temp % 10) == 0)
	{
		temp = temp / 10;
	}
  
	settings.numYTicks = temp * 2;
	
	settings.minX = -1 * x_temp / 2;
	settings.maxX = 1 * x_temp / 2;
	settings.numXTicks = 8;

	p.exhumation = argv[3];
	p.iso_1 = argv[4];
	p.iso_2 = argv[5];
	p.iso_3 = argv[6];
	p.iso_4 = argv[7];

	p.setPlotSettings(settings);
	
	QFile topoY("../output/periodic/isotherm/periodic_topoY.dat");
	QFile fin1("../output/periodic/isotherm/periodic_geo1.dat");
	QFile fin2("../output/periodic/isotherm/periodic_geo2.dat");
	QFile fin3("../output/periodic/isotherm/periodic_geo3.dat");
	QFile fin4("../output/periodic/isotherm/periodic_geo4.dat");

	topoY.open(IO_ReadOnly);
	QTextStream t(&topoY);
	
  while(!t.atEnd())
	{
		t >> temp1 >> temp2;	
		if(temp2 != "NaN")
		{
			data[0].push_back(atof(temp1));
			data[0].push_back(atof(temp2));
		}
	}
	topoY.close();

	fin1.open(IO_ReadOnly);
	QTextStream f1(&fin1);
  
	while(!f1.atEnd())
	{
		f1 >> temp1 >> temp2;	
		if(temp2 != "NaN")
		{
			data[1].push_back(atof(temp1));
			data[1].push_back(atof(temp2));
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
			data[2].push_back(atof(temp1));
			data[2].push_back(atof(temp2));
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
			data[3].push_back(atof(temp1));
			data[3].push_back(atof(temp2));
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
			data[4].push_back(atof(temp1));
			data[4].push_back(atof(temp2));
		}
	}
	fin4.close();

	p.setCurveData(0, data[0]);
	p.setCurveData(1, data[1]);
	p.setCurveData(2, data[2]);
	p.setCurveData(3, data[3]);
	p.setCurveData(4, data[4]);
	p.show();
  
  return a.exec();
}

