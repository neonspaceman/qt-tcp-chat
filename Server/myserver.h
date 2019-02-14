#ifndef MYSERVER_H
#define MYSERVER_H

#include <QVector>
#include <QSharedPointer>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QDataStream>
#include <QByteArray>

#define PORT 6000

class MyServer : public QObject
{
  Q_OBJECT
public:
  explicit MyServer(QObject *parent = 0);
  void broadCast(const QString &message);
public slots:
  void onConnected();
  void onDisconnected();
  void onReadyRead();
private:
  QTcpServer server;
  QVector<QSharedPointer<QTcpSocket>> clients;
  quint16 requestSize = 0;
};

#endif // MYSERVER_H
