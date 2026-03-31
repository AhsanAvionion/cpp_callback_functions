#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "bc_client_api.h"

#include <iostream>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;


    bc_client_api client;   // ✅ your library object

    // all handler functions to be set as call back to the functions in bc_client_api.h
    static void handleCallbackA(const std::string& data);  // must be static
    static void handleCallbackB(const std::string& data);  // must be static

};
#endif // MAINWINDOW_H
