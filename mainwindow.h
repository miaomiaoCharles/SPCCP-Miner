#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "spationode.h"
#include "write.h"
#include "road.h"
#include "widget.h"
#include <QIcon>
#include <QFont>
#include <QTextBrowser>
#include <QMenu>
#include <QFileDialog>
#include <iostream>
#include<QPushButton>
#include <QMainWindow>
#include <QFileDialog>
#include <QFont>
#include <set>
#include <map>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    static widget* mymap;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H