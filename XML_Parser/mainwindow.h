#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "reader.h"
#include <QMainWindow>
#include <QTreeView>
#include <QMenu>
#include <QMenuBar>
#include <QLineEdit>
#include <QFileDialog>
#include <QGridLayout>
#include <QDir>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void active();
    void activeMenu(QPoint pos);
    void open();
    void closeAll();

private:
    Reader* reader;
    QTreeView* tree;
};
#endif // MAINWINDOW_H
