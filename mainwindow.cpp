#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    randomModel = new MongoListModel();
    ui->tableView->setModel(randomModel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_search_editingFinished()
{
   ui->tableView->scrollToTop();
   emit randomModel->searchBy(ui->search->text());
}
