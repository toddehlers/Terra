/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/

#include <qdir.h>
#include <qmessagebox.h>

#include "topotherm.h"
#include "mainform.h"
#include "helpwindow.h"

void frmMain::showTopo()
{
 topotherm *t = new topotherm;
 t->show();
}

void frmMain::showTap()
{
 mainform *m = new mainform;
 m->show();
}

void frmMain::helpContents()
{ 
 QString home = QDir( "./help/index.html").absPath();

 HelpWindow *help = new HelpWindow(home, ".", 0, "help viewer");
 help->setCaption("TERRA - Help Contents");
 help->show();
}

void frmMain::helpAbout()
{
    
 QMessageBox::about(this, "TERRA - ABOUT", "TERRA\nThermochronometer Exhumation Record Recovery Analysis\nVersion 1.0 (July 1, 2005)\n\nCopyright Todd Ehlers 2005 (tehlers@umich.edu)\n\nThis program, and all associated subprograms, are free for use for non-profit applications.\n\nOn-line help documentation is available via the \"Help->Contents\" menu.\n\nProgramming Team:\nTodd Ehlers, Tehmasp Chaudhri, Santosh Kumar, Chris Fuller.\n\nSend all bug reports to tehlers@umich.edu\nUse this program at your own risk.\nAlthough all programs have been checked for accuracy results are not guaranteed.\n\nThese programs were written in C++ and with the Qt GUI interface (www. trolltech.com)\nSource code for these programs is available upon request. Command line driven versions\nof the software are also available for batch processing age prediction.");

}
