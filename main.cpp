#include "model.h"
#include "view.h"

#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    View w;
    //Model m;

    //m.setView(&w);
    //w.setModel(&m);
    w.show();

    return a.exec();

}
