#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QScrollBar>

#include "mongolistmodel.h"

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
    void on_search_editingFinished();

private:
    Ui::MainWindow *ui;
    MongoListModel *randomModel;

};

#endif // MAINWINDOW_H
