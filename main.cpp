#include "commoditylogindlg.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CommodityLoginDlg w;
    w.show();
    return a.exec();
}
