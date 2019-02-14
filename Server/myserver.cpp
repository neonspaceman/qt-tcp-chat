#include "myserver.h"

MyServer::MyServer(QObject *parent)
  : QObject(parent)
{
  connect(&server, &server.newConnection, this, &onConnected);
  if (server.listen(QHostAddress::Any, PORT))
    qDebug() << "Server listen port " + QString::number(PORT);
  else
    throw QString("Server don't listen port " + QString::number(PORT));
}

void MyServer::broadCast(const QString &message)
{
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);
  out << (quint16)0 << message;
  out.device()->seek(0);
  out << (quint16)(block.size() - sizeof(quint16));
  foreach(QSharedPointer<QTcpSocket> client, clients)
    client->write(block);
}

void MyServer::onConnected()
{
  QSharedPointer<QTcpSocket> clientSocket(server.nextPendingConnection(), &QTcpSocket::deleteLater);
  connect(clientSocket.data(), &clientSocket->readyRead, this, &onReadyRead);
  connect(clientSocket.data(), &clientSocket->disconnected, this, &onDisconnected);
  clients.push_back(clientSocket);
  qDebug() << "Client connected";
  qDebug() << "Now " + QString::number(clients.size()) + " clients";
}

void MyServer::onDisconnected()
{
  QTcpSocket *clientSocket = (QTcpSocket *)sender();
  for (auto it = clients.begin(); it != clients.end();)
  {
    if (it->data() == clientSocket)
      it = clients.erase(it);
    else
      ++it;
  }
  qDebug() << "Client disconnected";
  qDebug() << "Now " + QString::number(clients.size()) + " clients";
}

void MyServer::onReadyRead()
{
  QTcpSocket *clientSocket = (QTcpSocket *)sender();
  QDataStream in(clientSocket);
  if (!requestSize)
  {
    if (clientSocket->bytesAvailable() < sizeof(requestSize))
      return;
    in >> requestSize;
  }
  if (clientSocket->bytesAvailable() < requestSize)
    return;
  requestSize = 0;
  QString message;
  in >> message;
  broadCast(message);
  qDebug() << "Client send message: " + message;
}
