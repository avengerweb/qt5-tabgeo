#include "country.h"

#include <QCoreApplication>
#include <QTime>

using namespace TabGeo;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Country *country = new Country(a.applicationDirPath() + "/tabgeo_country_v4.dat");

    //Examples
    QTime t;
    t.start();

    qDebug() << country->get("202.43.147.226"); //China
    qDebug() << country->get("154.72.195.206"); //Uganda
    qDebug() << country->get("208.53.164.114"); //USA
    qDebug() << country->get("5.135.159.162");  //France

    qDebug("Time elapsed for check: %d ms", t.elapsed());

    return a.exec();
}
