#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "clientinterface.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ClientInterface * interface1 = ClientInterface::getIntstance();
}

MainWindow::~MainWindow()
{
    delete ui;
}


unsigned int working_llm_id = 5;
void MainWindow::on_getLLM_clicked()
{
    ClientInterface * interface1 = ClientInterface::getIntstance();
    interface1->GetLLM(working_llm_id);
}

void MainWindow::on_getLLMs_clicked()
{
    ClientInterface * interface1 = ClientInterface::getIntstance();
    interface1->GetLLMs();
}

void MainWindow::on_createLLM_clicked()
{
    ClientInterface * interface1 = ClientInterface::getIntstance();
    interface1->CreateLLM();
}

void MainWindow::on_updateLLM_clicked()
{
    ClientInterface * interface1 = ClientInterface::getIntstance();
    interface1->UpdateLLM(working_llm_id);
}

void MainWindow::on_deleteLLM_clicked()
{
    ClientInterface * interface1 = ClientInterface::getIntstance();
    interface1->DeleteLLM(working_llm_id);
}
