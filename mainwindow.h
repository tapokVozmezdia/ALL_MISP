#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include "studentService.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

public slots:
    void changePrice();
    std::string getCity();
    std::string getAdress();
    std::string getInstitute();
    std::string getCaffe();
    std::string getCinema();
    int getAge();
};
#endif // MAINWINDOW_H
