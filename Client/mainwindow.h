#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define PORT 6000

#include <QMainWindow>
#include <QTcpSocket>
#include <QDataStream>
#include <QByteArray>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void on_buttonConnect_clicked();
  void onSocketReadyRead();
  void onSocketConnected();
  void onSocketDisconected();

  void on_buttonSend_clicked();

  void on_buttonClose_clicked();

  void on_buttonDisconnect_clicked();

private:
  Ui::MainWindow *ui;
  QTcpSocket socket;
  quint16 requestSize = 0;
};

#endif // MAINWINDOW_H
