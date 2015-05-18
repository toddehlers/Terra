/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include <qapplication.h>
#include <qcheckbox.h>
#include <qclipboard.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qfocusdata.h>
#include <qlineedit.h>
#include <qmessagebox.h>
#include <qprocess.h>
#include <qprogressdialog.h>
#include <qpushbutton.h>
#include <qsettings.h>
#include <qspinbox.h>
#include <qstring.h>
#include <qtextstream.h>
#include <qtimer.h>
#include <qwidget.h>

#include "outputform.h"

void mainform::editCut()
{ 
    if(lineEditLoadOne->hasFocus())
 lineEditLoadOne->cut();
    else if(lineEditLoadTwo->hasFocus())
 lineEditLoadTwo->cut();
    else if(lineEditLoadThree->hasFocus())
 lineEditLoadThree->cut();
    else if(lineEditLoadFour->hasFocus())
 lineEditLoadFour->cut();
    else if(lineEditLoadFive->hasFocus())
 lineEditLoadFive->cut();
    else if(lineEditOutputOne->hasFocus())
 lineEditOutputOne->cut();
    else if(lineEditOutputTwo->hasFocus())
 lineEditOutputTwo->cut();
    else if(lineEditOutputThree->hasFocus())
 lineEditOutputThree->cut();
    else if(MilyearsInput->hasFocus())
 MilyearsInput->cut();
    else if(AHeInitialDiffuseInput->hasFocus())
 AHeInitialDiffuseInput->cut();
    else if(AHeActivationEnergyInput->hasFocus())
 AHeActivationEnergyInput->cut();
    else if(AHeGrainSizeInput->hasFocus())
 AHeGrainSizeInput->cut();
    else if(ZHeInitialDiffuseInput->hasFocus())
 ZHeInitialDiffuseInput->cut();
    else if(ZHeActivationEnergyInput->hasFocus())
 ZHeActivationEnergyInput->cut();
    else if(ZHeGrainSizeInput->hasFocus())
 ZHeGrainSizeInput->cut();
    else if(AFTAlphaInput->hasFocus())
 AFTAlphaInput->cut();
    else if(AFTBetaInput->hasFocus())
 AFTBetaInput->cut();
    else if(AFTMinTrackLengthInput->hasFocus())
 AFTMinTrackLengthInput->cut();
    else if(AFTC0Input->hasFocus())
 AFTC0Input->cut();
    else if(AFTC1Input->hasFocus())
 AFTC1Input->cut();
    else if(AFTC2Input->hasFocus())
 AFTC2Input->cut();
    else if(AFTC3Input->hasFocus())
 AFTC3Input->cut();
    else if(ZFTBInput->hasFocus())
 ZFTBInput->cut();
    else if(ZFTRInput->hasFocus())
 ZFTRInput->cut();
    else if(ZFTEbarInput->hasFocus())
 ZFTEbarInput->cut();
}

void mainform::editCopy()
{
    
    if(lineEditLoadOne->hasFocus())
 lineEditLoadOne->copy();
    else if(lineEditLoadTwo->hasFocus())
 lineEditLoadTwo->copy();
    else if(lineEditLoadThree->hasFocus())
 lineEditLoadThree->copy();
    else if(lineEditLoadFour->hasFocus())
 lineEditLoadFour->copy();
    else if(lineEditLoadFive->hasFocus())
 lineEditLoadFive->copy();
    else if(lineEditOutputOne->hasFocus())
 lineEditOutputOne->copy();
    else if(lineEditOutputTwo->hasFocus())
 lineEditOutputTwo->copy();
    else if(lineEditOutputThree->hasFocus())
 lineEditOutputThree->copy();
    else if(MilyearsInput->hasFocus())
 MilyearsInput->copy();
    else if(AHeInitialDiffuseInput->hasFocus())
 AHeInitialDiffuseInput->copy();
    else if(AHeActivationEnergyInput->hasFocus())
 AHeActivationEnergyInput->copy();
    else if(AHeGrainSizeInput->hasFocus())
 AHeGrainSizeInput->copy();
    else if(ZHeInitialDiffuseInput->hasFocus())
 ZHeInitialDiffuseInput->copy();
    else if(ZHeActivationEnergyInput->hasFocus())
 ZHeActivationEnergyInput->copy();
    else if(ZHeGrainSizeInput->hasFocus())
 ZHeGrainSizeInput->copy();
    else if(AFTAlphaInput->hasFocus())
 AFTAlphaInput->copy();
    else if(AFTBetaInput->hasFocus())
 AFTBetaInput->copy();
    else if(AFTMinTrackLengthInput->hasFocus())
 AFTMinTrackLengthInput->copy();
    else if(AFTC0Input->hasFocus())
 AFTC0Input->copy();
    else if(AFTC1Input->hasFocus())
 AFTC1Input->copy();
    else if(AFTC2Input->hasFocus())
 AFTC2Input->copy();
    else if(AFTC3Input->hasFocus())
 AFTC3Input->copy();
    else if(ZFTBInput->hasFocus())
 ZFTBInput->copy();
    else if(ZFTRInput->hasFocus())
 ZFTRInput->copy();
    else if(ZFTEbarInput->hasFocus())
 ZFTEbarInput->copy();
}

void mainform::editPaste()
{
    if(lineEditLoadOne->hasFocus())
 lineEditLoadOne->paste();
    else if(lineEditLoadTwo->hasFocus())
 lineEditLoadTwo->paste();
    else if(lineEditLoadThree->hasFocus())
 lineEditLoadThree->paste();
    else if(lineEditLoadFour->hasFocus())
 lineEditLoadFour->paste();
    else if(lineEditLoadFive->hasFocus())
 lineEditLoadFive->paste();
    else if(lineEditOutputOne->hasFocus())
 lineEditOutputOne->paste();
    else if(lineEditOutputTwo->hasFocus())
 lineEditOutputTwo->paste();
    else if(lineEditOutputThree->hasFocus())
 lineEditOutputThree->paste();
    else if(MilyearsInput->hasFocus())
 MilyearsInput->paste();
    else if(AHeInitialDiffuseInput->hasFocus())
 AHeInitialDiffuseInput->paste();
    else if(AHeActivationEnergyInput->hasFocus())
 AHeActivationEnergyInput->paste();
    else if(AHeGrainSizeInput->hasFocus())
 AHeGrainSizeInput->paste();
    else if(ZHeInitialDiffuseInput->hasFocus())
 ZHeInitialDiffuseInput->paste();
    else if(ZHeActivationEnergyInput->hasFocus())
 ZHeActivationEnergyInput->paste();
    else if(ZHeGrainSizeInput->hasFocus())
 ZHeGrainSizeInput->paste();
    else if(AFTAlphaInput->hasFocus())
 AFTAlphaInput->paste();
    else if(AFTBetaInput->hasFocus())
 AFTBetaInput->paste();
    else if(AFTMinTrackLengthInput->hasFocus())
 AFTMinTrackLengthInput->paste();
    else if(AFTC0Input->hasFocus())
 AFTC0Input->paste();
    else if(AFTC1Input->hasFocus())
 AFTC1Input->paste();
    else if(AFTC2Input->hasFocus())
 AFTC2Input->paste();
    else if(AFTC3Input->hasFocus())
 AFTC3Input->paste();
    else if(ZFTBInput->hasFocus())
 ZFTBInput->paste();
    else if(ZFTRInput->hasFocus())
 ZFTRInput->paste();
    else if(ZFTEbarInput->hasFocus())
 ZFTEbarInput->paste();
    
}

void mainform::showOutput()
{
    outputForm* output = new outputForm(this);
    output->show();
}

void mainform::loadInputFileOne()
{
    QString fileName = QFileDialog::getOpenFileName("./output/default_output/","Any file (*)",this,"Load input file","Select file to open");
    
    if(fileName != "") {
 lineEditLoadOne->setText(fileName);
    }
}


void mainform::loadInputFileTwo()
{
    QString fileName = QFileDialog::getOpenFileName("./output/default_output/","Any file (*)",this,"Load input file","Select file to open");
    
    if(fileName != "") {
 lineEditLoadTwo->setText(fileName);
    }
}

void mainform::loadInputFileThree()
{
    QString fileName = QFileDialog::getOpenFileName("./output/default_output/","Any file (*)",this,"Load input file","Select file to open");
    
    if(fileName != "") {
 lineEditLoadThree->setText(fileName);
    }
}

void mainform::loadInputFileFour()
{
    QString fileName = QFileDialog::getOpenFileName("./output/default_output/","Any file (*)",this,"Load input file","Select file to open");
    
    if(fileName != "") {
 lineEditLoadFour->setText(fileName);
    }
}

void mainform::loadInputFileFive()
{
    QString fileName = QFileDialog::getOpenFileName("./output/default_output/","Any file (*)",this,"Load input file","Select file to open");
    
    if(fileName != "") {
 lineEditLoadFive->setText(fileName);
    }
}

void mainform::saveOutputFileOne()
{
    QString fileName = QFileDialog::getSaveFileName("./output/age_prediction/","Any file (*)",this,"Save output file","Select file to save");
    if(fileName != "") {
 lineEditOutputOne->setText(fileName);
    }
}

void mainform::saveOutputFileTwo()
{    
    QString fileName = QFileDialog::getSaveFileName("./output/age_prediction/","Any file (*)",this,"Save output file","Select file to save");
    if(fileName != "") {
 lineEditOutputTwo->setText(fileName);
    }
}


void mainform::saveOutputFileThree()
{
    QString fileName = QFileDialog::getSaveFileName("./output/age_prediction/","Any file (*)",this,"Save output file","Select file to save");
    if(fileName != "") {
 lineEditOutputThree->setText(fileName);
    }
}

void mainform::processAll()
{ 
    QProcess* proc = new QProcess(this);
    int steps = 0;
    int count = 0;
    
    proc->setWorkingDirectory( QDir("./bin") );
    
    if(lineEditLoadOne->text() != "" && lineEditLoadTwo->text() != "" && lineEditOutputOne->text() != "") {     
 // run external program
 proc->addArgument("./age_prediction");
 proc->addArgument("-t");
 proc->addArgument(lineEditLoadOne->text());
 proc->addArgument("-T");
 proc->addArgument(lineEditLoadTwo->text());
 proc->addArgument("-o");
 proc->addArgument(lineEditOutputOne->text());
 proc->addArgument("-Y");
 proc->addArgument(MilyearsInput->text());
 proc->addArgument("-S");
 proc->addArgument(TimestepInput->text());
 proc->addArgument("-j");
 proc->addArgument(AHeInitialDiffuseInput->text());
 proc->addArgument("-k");
 proc->addArgument(AHeActivationEnergyInput->text());
 proc->addArgument("-l");
 proc->addArgument(AHeGrainSizeInput->text());
 proc->addArgument("-m");
 proc->addArgument(AHeUConcInput->text());
 proc->addArgument("-n");
 proc->addArgument(AHeThConcInput->text());
 proc->addArgument("-J");
 proc->addArgument(ZHeInitialDiffuseInput->text());
 proc->addArgument("-K");
 proc->addArgument(ZHeActivationEnergyInput->text());
 proc->addArgument("-L");
 proc->addArgument(ZHeGrainSizeInput->text());
 proc->addArgument("-M");
 proc->addArgument(ZHeUConcInput->text());
 proc->addArgument("-N");
 proc->addArgument(ZHeThConcInput->text());
 proc->addArgument("-A");
 proc->addArgument(AFTAlphaInput->text());
 proc->addArgument("-B");
 proc->addArgument(AFTBetaInput->text());
 proc->addArgument("-C");
 proc->addArgument(AFTNstepsInput->text());
 proc->addArgument("-D");
 proc->addArgument(AFTDparInput->text());
 proc->addArgument("-E");
 proc->addArgument(AFTMinTrackLengthInput->text());
 proc->addArgument("-F");
 proc->addArgument(AFTC0Input->text());
 proc->addArgument("-G");
 proc->addArgument(AFTC1Input->text());
 proc->addArgument("-H");
 proc->addArgument(AFTC2Input->text());
 proc->addArgument("-I");
 proc->addArgument(AFTC3Input->text());
 proc->addArgument("-U");
 proc->addArgument(ZFTBInput->text());
 proc->addArgument("-V");
 proc->addArgument(ZFTEbarInput->text());
 proc->addArgument("-W");
 proc->addArgument(ZFTRInput->text());
 
 // determine which coord. files to pass
 if(lineEditLoadThree->text() != "") {
     proc->addArgument("-x");
     proc->addArgument(lineEditLoadThree->text());
             }
 if(lineEditLoadFour->text() != "") {
     proc->addArgument("-y");
     proc->addArgument(lineEditLoadFour->text());
 }
 if(lineEditLoadFive->text() != "") { 
     proc->addArgument("-z");
     proc->addArgument(lineEditLoadFive->text());
 }
 
 if(proc->start()) { 
     QProgressDialog progress("Calculating all ages...", "Cancel", 0, this, "progress", TRUE);
     progress.setCaption("Please Wait...");
     progress.setMinimumDuration(0);
     
     while(proc->isRunning()) {
  count++;
  if(count % 5000 == 0) {
      progress.setProgress(++steps);
  }
     }
 }
 delete proc;
    }
    else
    {
 QMessageBox::about(this,"TERRA - AGE PREDICTION","Please provide all (Forward Model) input and output filenames.\n"
      "X, Y, and Z coordinate history files are not always required.\n");
    }
    
}

void mainform::processSel()
{
    QProcess* proc = new QProcess(this);
    int steps = 0;
    int count = 0;
    
    proc->setWorkingDirectory( QDir("./bin") );
    
    if(!AHeRunTestBox->isChecked() && !ZHeRunTestBox->isChecked() && !AFTRunTestBox->isChecked() && !ZFTRunTestBox->isChecked()) {
 QMessageBox::about(this,"TERRA - AGE PREDICTION","Please select at least one age to calculate.\n");
    }
    else {
 if(lineEditLoadOne->text() != "" && lineEditLoadTwo->text() != "" && lineEditOutputOne->text() != "") {     
     // run external program
     proc->addArgument("./age_prediction");
     proc->addArgument("-t");
     proc->addArgument(lineEditLoadOne->text());
     proc->addArgument("-T");
     proc->addArgument(lineEditLoadTwo->text());
     proc->addArgument("-o");
     proc->addArgument(lineEditOutputOne->text());
     proc->addArgument("-Y");
     proc->addArgument(MilyearsInput->text());
     proc->addArgument("-S");
     proc->addArgument(TimestepInput->text());
     proc->addArgument("-j");
     proc->addArgument(AHeInitialDiffuseInput->text());
     proc->addArgument("-k");
     proc->addArgument(AHeActivationEnergyInput->text());
     proc->addArgument("-l");
     proc->addArgument(AHeGrainSizeInput->text());
     proc->addArgument("-m");
     proc->addArgument(AHeUConcInput->text());
     proc->addArgument("-n");
     proc->addArgument(AHeThConcInput->text());
     proc->addArgument("-J");
     proc->addArgument(ZHeInitialDiffuseInput->text());
     proc->addArgument("-K");
     proc->addArgument(ZHeActivationEnergyInput->text());
     proc->addArgument("-L");
     proc->addArgument(ZHeGrainSizeInput->text());
     proc->addArgument("-M");
     proc->addArgument(ZHeUConcInput->text());
     proc->addArgument("-N");
     proc->addArgument(ZHeThConcInput->text());
     proc->addArgument("-A");
     proc->addArgument(AFTAlphaInput->text());
     proc->addArgument("-B");
     proc->addArgument(AFTBetaInput->text());
     proc->addArgument("-C");
     proc->addArgument(AFTNstepsInput->text());
     proc->addArgument("-D");
     proc->addArgument(AFTDparInput->text());
     proc->addArgument("-E");
     proc->addArgument(AFTMinTrackLengthInput->text());
     proc->addArgument("-F");
     proc->addArgument(AFTC0Input->text());
     proc->addArgument("-G");
     proc->addArgument(AFTC1Input->text());
     proc->addArgument("-H");
     proc->addArgument(AFTC2Input->text());
     proc->addArgument("-I");
     proc->addArgument(AFTC3Input->text());
     proc->addArgument("-U");
     proc->addArgument(ZFTBInput->text());
     proc->addArgument("-V");
     proc->addArgument(ZFTEbarInput->text());
     proc->addArgument("-W");
     proc->addArgument(ZFTRInput->text());
     
     // determine which ages to calculate
     if(!AHeRunTestBox->isChecked()) {
  proc->addArgument("-1");
  proc->addArgument("0");
     }
     if(!ZHeRunTestBox->isChecked()) {
  proc->addArgument("-2");
  proc->addArgument("0");
     }
     if(!AFTRunTestBox->isChecked()) {
  proc->addArgument("-3");
  proc->addArgument("0");
     }
     if(!ZFTRunTestBox->isChecked()) {
  proc->addArgument("-4");
  proc->addArgument("0");
     }
     
     // determine which coord. files to pass
     if(lineEditLoadThree->text() != "") {
  proc->addArgument("-x");
  proc->addArgument(lineEditLoadThree->text());
     }
     if(lineEditLoadFour->text() != "") {
  proc->addArgument("-y");
  proc->addArgument(lineEditLoadFour->text());
     }
     if(lineEditLoadFive->text() != "") {
  proc->addArgument("-z");
  proc->addArgument(lineEditLoadFive->text());
     }
     
     if(proc->start()) {
  QProgressDialog progress("Calculating selected ages...", "Cancel", 0, this, "progress", TRUE);
  progress.setCaption("Please Wait...");
  progress.setMinimumDuration(0);
  
  while(proc->isRunning()) {
      count++;
      if(count % 5000 == 0) {
   progress.setProgress(++steps);
      }
              }
     }
     delete proc;
     
 }
 else {
     QMessageBox::about(this,"TERRA - AGE PREDICTION","Please provide all (Forward Model) input and output filenames.\n" "X, Y, and Z coordinate history files are not always required.\n");
 }
 
    }
    
}
    
    
    
