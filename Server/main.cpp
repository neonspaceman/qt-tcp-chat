#include <QCoreApplication>
#include <QDebug>
#include <memory>
#include "myserver.h"

int main(int argc, char *argv[])
{
  try
  {
    QCoreApplication a(argc, argv);

    MyServer server;

    return a.exec();
  }
  catch(const QString &err)
  {
    return -1;
  }
}
