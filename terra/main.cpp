#include <qapplication.h>
#include "frmmain.h"

int main(int argc, char ** argv)
{
    QApplication a( argc, argv );
    frmMain w;
    w.show();
    
    a.setMainWidget(&w);
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
