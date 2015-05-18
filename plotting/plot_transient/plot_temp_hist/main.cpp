#include <qapplication.h>
#include <qstring.h>
#include <qfile.h>
#include <qtextstream.h>

#include <iostream>

#include "plotter.h"

#define MAX_ARGS 3

int main(int argc, char *argv[])
{
	QString temp1, temp2;

	QApplication a(argc, argv);
	PlotSettings settings;
	Plotter p;

  if(argc != MAX_ARGS)
  {
    std::cerr<<"Bad command line arguments - FIXME!!!\n";
    exit(EXIT_FAILURE);
  }
	
	p.setCaption("TERRA - 1D TRANSIENT EROSION-TEMPERATURE HISTORY - UNIVERSITY OF MICHIGAN");
	p.setGeometry(300, 300, 1000, 600);
  
  a.setMainWidget(&p);
 	CurveData data[1];
	
	p.exhumation = argv[1];
	p.iso_1 = argv[1];
	p.iso_2 = " ";
	p.iso_3 = " ";
	p.iso_4 = " ";

	QFile fin1("../output/transient/temp_hist/transient_tempHist.dat");

	fin1.open(IO_ReadOnly);
	QTextStream f(&fin1);
	f >> temp1 >> temp2;
	fin1.close();

	settings.minX = 0;
	settings.maxX = atoi(argv[1]);
	
	settings.minY = atoi(argv[2]);
	settings.maxY = atoi(temp2) + 10;

	p.setPlotSettings(settings);

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

	p.setCurveData(0, data[0]);
	p.show();
  return a.exec();
}

