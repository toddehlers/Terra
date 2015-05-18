/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

#include<qprocess.h>
#include<qstring.h>
#include<qfiledialog.h>
#include<qfile.h>
#include<qtextstream.h>
#include<qmessagebox.h>
#include<qsplashscreen.h>
#include<qpixmap.h>
#include<qsound.h>
#include<qprogressdialog.h>

void topotherm::fileNew()
{
  lineEdit1->setText("");
  lineEdit2->setText("");
  lineEdit3->setText("");
  lineEdit4->setText("");
  lineEdit5->setText("");
  lineEdit6->setText("");
  lineEdit7->setText("");
  lineEdit8->setText("");
  lineEdit9->setText("");
  lineEdit10->setText("");
  lineEdit11->setText("");
  lineEdit12->setText("");
  lineEdit13->setText("");
  lineEdit14->setText("");
}

void topotherm::fileOpen()
{
  QString temp1, temp2;
  QString filename = QFileDialog::getOpenFileName("./input_config/", "TERRA - THERMAL CALCULATION Session file (*.dat)", this, "Load Session", "Select file to open" );  

  if(filename != "")
  {
    QFile file(filename);
    file.open(IO_ReadOnly);
    QTextStream f(&file);
    
    QProgressDialog progress("Getting values...", "Cancel", 0, this, "progress", TRUE);
  
    f >> temp1 >> temp2;
    lineEdit1->setText(temp2);
    progress.setProgress(1);
    f >> temp1 >> temp2;
    lineEdit2->setText(temp2);
    progress.setProgress(2);
    f >> temp1 >> temp2;
    lineEdit3->setText(temp2);
    progress.setProgress(3);
    f >> temp1 >> temp2;
    lineEdit4->setText(temp2);
    progress.setProgress(4);
    f >> temp1 >> temp2;
    lineEdit5->setText(temp2);
    progress.setProgress(5);
    f >> temp1 >> temp2;
    lineEdit6->setText(temp2);
    progress.setProgress(6);
    f >> temp1 >> temp2;
    lineEdit7->setText(temp2);
    progress.setProgress(7);
    f >> temp1 >> temp2;
    lineEdit8->setText(temp2);
    progress.setProgress(8);
    f >> temp1 >> temp2;
    lineEdit9->setText(temp2);
    progress.setProgress(9);
    f >> temp1 >> temp2;
    lineEdit10->setText(temp2);
    progress.setProgress(10);
    f >> temp1 >> temp2;
    lineEdit11->setText(temp2);
    progress.setProgress(11);
    f >> temp1 >> temp2;
    lineEdit12->setText(temp2);
    progress.setProgress(12);
    f >> temp1 >> temp2;
    lineEdit13->setText(temp2);
    progress.setProgress(13);
    f >> temp1 >> temp2;
    lineEdit14->setText(temp2);
    progress.setProgress(14);

    file.close();
  }
}


void topotherm::fileSave()
{
  QString filename = QFileDialog::getSaveFileName("./input_config/", "TERRA - THERMAL CALCULATION Session file (*.dat)",
                                                  this, "Save Session",  "Select file to save" );

  if(filename != "")
  {
    QFile file(filename);
    file.open(IO_WriteOnly);
 
    QTextStream f(&file); 
      
    f << "Surface_Temperature " << lineEdit1->text() << endl;
    f << "Lithosphere_Base_Temperature " << lineEdit2->text() << endl;
    f << "Lithosphere_Base_Depth " << lineEdit3->text() << endl;
    f << "Surface_Temperature_Lapse_Rate " << lineEdit4->text() << endl;

    f << "Horizontal_Distance " << lineEdit5->text() << endl;
    f << "Vertical_Depth " << lineEdit6->text() << endl;

    f << "Topography_Wavelength " << lineEdit7->text() << endl;
    f << "Topography_Amplitude " << lineEdit8->text() << endl;

    f << "Diffusivity " << lineEdit9->text() << endl;
    f << "Density_of_Crust " << lineEdit10->text() << endl;
    f << "Specific_Heat " << lineEdit11->text() << endl;
    f << "Surface_Volumetric_Heat_Production " << lineEdit12->text() << endl;
    f << "Heat_production_charactersitic_Depth " << lineEdit13->text() << endl;

    f << "Exhumation " << lineEdit14->text() << endl;

    file.close();
  }    
}

void topotherm::editCut()
{
  if(lineEdit1->hasFocus())
    lineEdit1->cut();
  else if(lineEdit2->hasFocus())
    lineEdit2->cut();
  else if(lineEdit3->hasFocus())
    lineEdit3->cut();
  else if(lineEdit4->hasFocus())
    lineEdit4->cut();
  else if(lineEdit6->hasFocus())
    lineEdit6->cut();
  else if(lineEdit7->hasFocus())
    lineEdit7->cut();
  else if(lineEdit8->hasFocus())
    lineEdit8->cut();
  else if(lineEdit9->hasFocus())
    lineEdit9->cut();
  else if(lineEdit10->hasFocus())
    lineEdit10->cut();
  else if(lineEdit11->hasFocus())
    lineEdit11->cut();
  else if(lineEdit12->hasFocus())
    lineEdit12->cut();
  else if(lineEdit13->hasFocus())
    lineEdit13->cut();
  else if(lineEdit14->hasFocus())
    lineEdit14->cut();
  else if(lineEdit15->hasFocus())
    lineEdit15->cut();
  else if(lineEdit16->hasFocus())
    lineEdit16->cut();
  else if(lineEdit17->hasFocus())
    lineEdit17->cut();
  else if(lineEdit18->hasFocus())
    lineEdit18->cut();
}


void topotherm::editCopy()
{
  if(lineEdit1->hasFocus())
    lineEdit1->copy();
  else if(lineEdit2->hasFocus())
    lineEdit2->copy();
  else if(lineEdit3->hasFocus())
    lineEdit3->copy();
  else if(lineEdit4->hasFocus())
    lineEdit4->copy();
  else if(lineEdit6->hasFocus())
    lineEdit6->copy();
  else if(lineEdit7->hasFocus())
    lineEdit7->copy();
  else if(lineEdit8->hasFocus())
    lineEdit8->copy();
  else if(lineEdit9->hasFocus())
    lineEdit9->copy();
  else if(lineEdit10->hasFocus())
    lineEdit10->copy();
  else if(lineEdit11->hasFocus())
    lineEdit11->copy();
  else if(lineEdit12->hasFocus())
    lineEdit12->copy();
  else if(lineEdit13->hasFocus())
    lineEdit13->copy();
  else if(lineEdit14->hasFocus())
    lineEdit14->copy();
  else if(lineEdit15->hasFocus())
    lineEdit15->copy();
  else if(lineEdit16->hasFocus())
    lineEdit16->copy();
  else if(lineEdit17->hasFocus())
    lineEdit17->copy();
  else if(lineEdit18->hasFocus())
    lineEdit18->copy();
}


void topotherm::editPaste()
{
  if(lineEdit1->hasFocus())
    lineEdit1->paste();
  else if(lineEdit2->hasFocus())
    lineEdit2->paste();
  else if(lineEdit3->hasFocus())
    lineEdit3->paste();
  else if(lineEdit4->hasFocus())
    lineEdit4->paste();
  else if(lineEdit6->hasFocus())
    lineEdit6->paste();
  else if(lineEdit7->hasFocus())
    lineEdit7->paste();
  else if(lineEdit8->hasFocus())
    lineEdit8->paste();
  else if(lineEdit9->hasFocus())
    lineEdit9->paste();
  else if(lineEdit10->hasFocus())
    lineEdit10->paste();
  else if(lineEdit11->hasFocus())
    lineEdit11->paste();
  else if(lineEdit12->hasFocus())
    lineEdit12->paste();
  else if(lineEdit13->hasFocus())
    lineEdit13->paste();
  else if(lineEdit14->hasFocus())
    lineEdit14->paste();
  else if(lineEdit15->hasFocus())
    lineEdit15->paste();
  else if(lineEdit16->hasFocus())
    lineEdit16->paste();
  else if(lineEdit17->hasFocus())
    lineEdit17->paste();
  else if(lineEdit18->hasFocus())
    lineEdit18->paste();   
}

void topotherm::send_back()
{
  int count = 0;
  int steps = 0;
    
  if( (lineEdit15->text().toDouble() <= 35.0) || (lineEdit16->text().toDouble() <= 35.0) ||
      (lineEdit17->text().toDouble() <= 35.0) || (lineEdit18->text().toDouble() <= 35.0) )
  {
    QMessageBox::warning(this, tr("TERRA - THERMAL CALCULATION"),
                         tr("<p> One or more of the Isotherms is <b>LESS</b> than or equal to 35.0! Please input values greater than 35." ));
  }
  else
  {
    QProcess *proc;
    proc = new QProcess(this);
 
    proc->setWorkingDirectory(QDir("./bin")); 
 
    proc->addArgument("./periodic");
    proc->addArgument("-T");
    proc->addArgument(lineEdit1->text());
    proc->addArgument("-U");
    proc->addArgument(lineEdit14->text());
    proc->addArgument("-k");
    proc->addArgument(lineEdit9->text());
    proc->addArgument("-d");
    proc->addArgument(lineEdit13->text());
    proc->addArgument("-w");
    proc->addArgument(lineEdit7->text());
    proc->addArgument("-s");
    proc->addArgument(lineEdit12->text());
    proc->addArgument("-c");
    proc->addArgument(lineEdit10->text());
    proc->addArgument("-S");
    proc->addArgument(lineEdit11->text());
    proc->addArgument("-B");
    proc->addArgument(lineEdit2->text());
    proc->addArgument("-D");
    proc->addArgument(lineEdit3->text());
    proc->addArgument("-l");
    proc->addArgument(lineEdit4->text());
    proc->addArgument("-a");
    proc->addArgument(lineEdit8->text());
    proc->addArgument("-x");
    proc->addArgument(lineEdit5->text());
    proc->addArgument("-y");
    proc->addArgument(lineEdit6->text());
   
    proc->addArgument("-n");
    proc->addArgument(lineEdit15->text());
    proc->addArgument("-o");
    proc->addArgument(lineEdit16->text());
    proc->addArgument("-p");
    proc->addArgument(lineEdit17->text());
    proc->addArgument("-q");
    proc->addArgument(lineEdit18->text());
   
    // start program
    proc->start();
   
    QProgressDialog progress("Calculating...", "Cancel", 0, this, "progress", TRUE);
    progress.setCaption("Please Wait...");
    progress.setMinimumDuration(0);
   
    while(proc->isRunning())
    {
      count++;
      if(count % 5000 == 0)
      {
        progress.setProgress(++steps);
      }
    }
    delete proc;
   
    if(checkBox2->isChecked() == true)
    {
      QProcess *proc;
      proc = new QProcess(this);
      
      proc->setWorkingDirectory(QDir("./bin"));
    
      proc->addArgument("./plot_periodic_isotherm");
      proc->addArgument(lineEdit5->text());
      proc->addArgument(lineEdit6->text());
      proc->addArgument(lineEdit14->text());
    
      proc->addArgument(lineEdit15->text());
      proc->addArgument(lineEdit16->text());
      proc->addArgument(lineEdit17->text());
      proc->addArgument(lineEdit18->text());
    
      proc->start();
      delete proc;
    }
             
    if(checkBox1->isChecked() == true)
    {
      QProcess *proc;  
      proc = new QProcess(this);
       
      proc->setWorkingDirectory(QDir("./bin"));
    
      proc->addArgument("./plot_periodic_temphist");
      proc->addArgument(lineEdit14->text());
    
      proc->start();
      delete proc;
    }
   
  }
  
}

void topotherm::send_back_transient()
{
    int count = 0;
    int steps = 0;
    
    QString temp;
    temp = lineEdit25->text();
    
    if(temp.toFloat() > 0.0) { 
 QMessageBox::warning(this, tr("TERRA - THERMAL CALCULATION"), tr("<p> Value of Rate entered is positive! Please enter <b>NEGATIVE</b> values only." ));
    }
    else
    {
 QProcess *proc;
 proc = new QProcess(this);
 proc->setWorkingDirectory(QDir("./bin"));
 
 proc->addArgument( "./transient" );
 proc->addArgument("-z");
 proc->addArgument(lineEdit23->text());
 proc->addArgument("-T");
 proc->addArgument(lineEdit21->text());
 proc->addArgument("-B");
 proc->addArgument(lineEdit22->text());
 proc->addArgument("-a");
 proc->addArgument(lineEdit24->text());
 
 proc->addArgument("-d");
 proc->addArgument(lineEdit27->text());
 proc->addArgument("-e");
 proc->addArgument(lineEdit28->text());
 proc->addArgument("-f");
 proc->addArgument(lineEdit29->text());
 proc->addArgument("-g");
 proc->addArgument(lineEdit210->text());
 
 proc->addArgument("-r");
 proc->addArgument(lineEdit25->text());
 
 proc->addArgument("-i");
 proc->addArgument(lineEdit211->text());
 proc->addArgument("-j");
 proc->addArgument(lineEdit212->text());
 proc->addArgument("-k");
 proc->addArgument(lineEdit213->text());
 proc->addArgument("-l");
 proc->addArgument(lineEdit214->text());
 
 proc->addArgument("-o");
 proc->addArgument(lineEdit26->text());
 
 proc->start();
 
  QProgressDialog progress("Calculating...", "Cancel", 0, this, "progress", TRUE);
  progress.setCaption("Please Wait...");
  progress.setMinimumDuration(0);
 
 while(proc->isRunning()) {
     count++;
     if(count % 5000 == 0) {
  progress.setProgress(++steps);
     }
 }
 delete proc;
 
 if(checkBox5->isChecked() == true)
 {
     QProcess *proc;
     proc = new QProcess(this);
     proc->setWorkingDirectory(QDir("./bin"));
       
     proc->addArgument("./plot_transient_temphist");
     proc->addArgument(lineEdit26->text());
     proc->addArgument(lineEdit21->text());
       
     proc->start();
     delete proc;
   }
    
    
    if(checkBox6->isChecked() == true)
    {
 QProcess *proc;
 proc = new QProcess(this);
 proc->setWorkingDirectory(QDir("./bin"));
 
 // 2. Geotherm    
 proc->addArgument("./plot_transient_geotherm");
 proc->addArgument( lineEdit23->text() );
 proc->addArgument( lineEdit25->text() );
 
 proc->addArgument( lineEdit27->text() );
 proc->addArgument( lineEdit28->text() );
 proc->addArgument( lineEdit29->text() );
 proc->addArgument( lineEdit210->text() );
 
 proc->addArgument( lineEdit21->text() );
     
 proc->start();
 delete proc;
    }
    
    
    if(checkBox7->isChecked() == true)
    {
 QProcess *proc;
 proc = new QProcess(this);
 proc->setWorkingDirectory(QDir("./bin"));
 
 // 3. Temp_time     
 proc->addArgument("./plot_transient_temptime");
 proc->addArgument( lineEdit23->text() );
 proc->addArgument( lineEdit25->text() );
 
 proc->addArgument( lineEdit211->text() );
 proc->addArgument( lineEdit212->text() );
 proc->addArgument( lineEdit213->text() );
 proc->addArgument( lineEdit214->text() );
     
 proc->start();
 delete proc;
    }
    
}
}

void topotherm::isotherms()
{
    if((checkBox3->isChecked()) == true)
    {
 lineEdit15->setEnabled(true);
 iso_1->setEnabled(true);
 
 lineEdit16->setEnabled(true);
 iso_2->setEnabled(true);
 
 lineEdit17->setEnabled(true);
 iso_3->setEnabled(true);
 
 lineEdit18->setEnabled(true);
 iso_4->setEnabled(true);
    }
    else
    {
 if((lineEdit15->text() == "") || (lineEdit16->text() == "") || (lineEdit17->text() == "") || (lineEdit18->text() == ""))
 {
     QMessageBox::warning(this, tr("TERRA - THERMAL CALCULATION"), tr("<p> One of the Isotherms is <b>NOT</b> provided! Please complete it for accurate plots." ));
     checkBox3->setChecked(true);
 }
 else
 {
     lineEdit15->setEnabled(false);
     iso_1->setEnabled(false); 
     
     lineEdit16->setEnabled(false);
     iso_2->setEnabled(false); 
     
     lineEdit17->setEnabled(false);
     iso_3->setEnabled(false); 
     
     lineEdit18->setEnabled(false);
     iso_4->setEnabled(false); 
 }
    }
}

void topotherm::ckBox4()
{
    QMessageBox::warning(this, tr("TERRA - THERMAL CALCULATION"),
          tr("<p> After Thermal Histories are calculated, Ages can be calculated through main menu \"AGE PREDICTION\" button." ));
}

void topotherm::ckBox8()
{
    bool value;
    
    if(checkBox8->isChecked() == true)
 value = true;
    else
 value = false;

}

void topotherm::pop_up()
{
    QMessageBox::warning(this, tr("TERRA - THERMAL CALCULATION"),
          tr("<p> After Thermal Histories are calculated, Ages can be calculated through main menu \"AGE PREDICTION\" button." ));
}

void topotherm::horiz_width()
{
    QString temp;
    temp = lineEdit7->text();
    lineEdit5->setText(QString::number(2 * temp.toLong()));    
}
