#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // ✅ Set callback direct
    //client.setCallbackTypeA([](const std::string& data){
    //    std::cout << "Callback A: " << data << std::endl;
    //});
    client.setCallbackTypeA(MainWindow::handleCallbackA);
    client.setCallbackTypeB(MainWindow::handleCallbackB);


    // ✅ Connect to server
    bool ok = client.connect("127.0.0.1", 1234);

    if (!ok)
        std::cout << "Connection failed!" << std::endl;
    else
        std::cout << "Connected!" << std::endl;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui->test_LB->setText("abc");
    char buffer[1024]="Mehmood";
    client.send(buffer);

    //client->bc_set_callback_typeA(client, callBackA_called);
}




void MainWindow::handleCallbackA(const std::string& data)
{
    std::cout << "Callback A: 1 " << data << std::endl;
}
void MainWindow::handleCallbackB(const std::string& data)
{
    std::cout << "Callback B: 1 " << data << std::endl;
}
