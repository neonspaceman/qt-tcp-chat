#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  ui->statusBar->showMessage("Disconnected");

  ui->buttonDisconnect->setVisible(false);

  QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
  QRegExp ipRegex ("^" + ipRange + "\\." + ipRange + "\\." + ipRange + "\\." + ipRange + "$");
  QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
  ui->editIP->setValidator(ipValidator);

  connect(&socket, &socket.connected, this, &onSocketConnected);
  connect(&socket, &socket.readyRead, this, &onSocketReadyRead);
  connect(&socket, &socket.disconnected, this, &onSocketDisconected);
  connect(ui->buttonClose, &ui->buttonClose->clicked, this, &on_buttonClose_clicked);
  connect(ui->buttonClose, &ui->buttonClose->clicked, this, &on_buttonClose_clicked);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_buttonConnect_clicked()
{
  socket.connectToHost(ui->editIP->text(), PORT);
}

void MainWindow::onSocketReadyRead()
{
  qDebug() << "Request message";
  QDataStream in(&socket);
  if (!requestSize)
  {
    if (socket.bytesAvailable() < sizeof(requestSize))
      return;
    in >> requestSize;
  }
  if (socket.bytesAvailable() < requestSize)
    return;
  requestSize = 0;
  QString message;
  in >> message;
  ui->textMessages->append("> " + message);
  qDebug() << "Server send message: " + message;
}

void MainWindow::onSocketConnected()
{
  ui->statusBar->showMessage("Connected");
  ui->editIP->setEnabled(false);
  ui->buttonConnect->setVisible(false);
  ui->buttonDisconnect->setVisible(true);
  ui->editMessage->setEnabled(true);
  ui->buttonSend->setEnabled(true);
  qDebug() << "Connect";
}

void MainWindow::onSocketDisconected()
{
  ui->statusBar->showMessage("Disconnected");
  ui->editIP->setEnabled(true);
  ui->buttonConnect->setVisible(true);
  ui->buttonDisconnect->setVisible(false);
  ui->editMessage->setEnabled(false);
  ui->buttonSend->setEnabled(false);
  qDebug() << "Disconnect";
}

void MainWindow::on_buttonSend_clicked()
{
  QString message = ui->editMessage->text();
  if (message.isEmpty())
    return;
  ui->editMessage->clear();
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);
  out << (quint16)0 << message;
  out.device()->seek(0);
  out << (quint16)(block.size() - sizeof(quint16));
  socket.write(block);
  qDebug() << "Send message";
}

void MainWindow::on_buttonClose_clicked()
{
  close();
}

void MainWindow::on_buttonDisconnect_clicked()
{
  socket.disconnectFromHost();
}
