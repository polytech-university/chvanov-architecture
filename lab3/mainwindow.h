#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QDebug>

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
    void on_getLLM_clicked();

    void on_getLLMs_clicked();

    void on_createLLM_clicked();

    void on_updateLLM_clicked();

    void on_deleteLLM_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
