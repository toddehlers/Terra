#include <qapplication.h>
#include <qstring.h>
#include <qfile.h>
#include <qtextstream.h>

#include <iostream>

#include "plotter.h"

#define MAX_ARGS 2

int main(int argc, char *argv[])
{
	QString temp1, temp2;

	QApplication a( argc, argv );
	PlotSettings settings;
	Plotter p;
	
	p.setCaption("TERRA - TEMPERATURE HISTORIES: 2D PERIODIC-TEMPERATURE HISTORY - UNIVERSITY OF MICHIGAN");
	p.setGeometry(300, 300, 1000, 600);
  
  a.setMainWidget(&p);
 	CurveData data[3];
	
  if(argc != MAX_ARGS)
  {
    std::cerr<<"Bad command line arguments - FIXME!!!\n";
    exit(EXIT_FAILURE);
  }
  
	p.exhumation = argv[1];
	p.iso_1 = "Ridge";
	p.iso_2 = "Mid-Valley";
	p.iso_3 = "Valley";
	p.iso_4 = " ";

	QFile fin1("../output/periodic/temp_hist/periodic_ridge.dat");
	QFile fin2("../output/periodic/temp_hist/periodic_midValley.dat");
	QFile fin3("../output/periodic/temp_hist/periodic_valley.dat");

	fin1.open(IO_ReadOnly);
	QTextStream f(&fin1);
	f >> temp1 >> temp2;
	fin1.close();

	settings.minX = atoi(temp1);
	settings.maxX = 0;
	
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

	settings.minY = atoi(temp2);

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
	
	fin3.open(IO_ReadOnly);
	QTextStream f4(&fin3);
	f4 >> temp1 >> temp2;
	fin3.close();

	settings.maxY = atoi(temp2);

	p.setPlotSettings(settings);

	p.setCurveData(0, data[0]);
	p.setCurveData(1, data[1]);
	p.setCurveData(2, data[2]);
	p.show();
  
  return a.exec();
}

