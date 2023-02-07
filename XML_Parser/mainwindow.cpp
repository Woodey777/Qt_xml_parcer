#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    auto menu = new QMenu(tr("Файл"));
    auto open = new QAction(tr("Открыть"), this);
    auto closeAll = new QAction(tr("Закрыть все"), this);
    auto exit = new QAction(tr("Выход"), this);

    menuBar()->addMenu(menu);
    menu->addAction(open);
    menu->addAction(closeAll);
    menu->addAction(exit);

    reader = new Reader{};
    tree = new QTreeView{};
    tree->setModel(reader);
    tree->setContextMenuPolicy(Qt::CustomContextMenu);
    setCentralWidget(tree);

    connect(tree,     SIGNAL(activeMenu(QPoint)), SLOT(activeMenu(QPoint)));
    connect(open,     SIGNAL(triggered()), this, SLOT(open()));
    connect(closeAll, SIGNAL(triggered()), this, SLOT(closeAll()));
    connect(exit,     SIGNAL(triggered()), this, SLOT(close()));
}

MainWindow::~MainWindow() {
    delete reader;
    delete tree;
}

void MainWindow::activeMenu(QPoint pos){
    if (tree->indexAt(pos).isValid() && tree->currentIndex().parent() == QModelIndex()) {
        QMenu *menu = new QMenu(this);
        auto active = new QAction(tr("Сделать активным"), this);
        menu->addAction(active);
        connect(active, &QAction::triggered, this, &MainWindow::active);
        menu->popup(tree->viewport()->mapToGlobal(pos));
    }
}

void MainWindow::active(){
    auto index = tree->currentIndex();
    QFont font;
    font.setBold(true);
    reader->setData(index, font, Qt::FontRole);
}

void MainWindow::open(){
    QString file = QFileDialog::getOpenFileName(this, tr("Выберите файл"), QDir::homePath(), tr("*.xml"));
    reader->loadData(file);
    tree->reset();
    QFont font;
    font.setBold(true);
    reader->setData(reader->index(0, 0), font, Qt::FontRole);
}

void MainWindow::closeAll(){
    reader->delModelRows();
    tree->reset();
}

