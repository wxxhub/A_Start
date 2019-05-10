#include "qt/include/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    dimesion_(3)
{
    n_digital_ = new NDigital();

    open_table_model_ = new QStandardItemModel();
    close_table_model_ = new QStandardItemModel();

    // init model
    open_table_model_->setColumnCount(3);
    open_table_model_->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("编号"));
    open_table_model_->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("层数"));
    open_table_model_->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("估价值"));

    close_table_model_->setColumnCount(3);
    close_table_model_->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("编号"));
    close_table_model_->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("层数"));
    close_table_model_->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("估价值"));

    ui->setupUi(this);

    ui->OrigionNode->setSpacing(0);
    ui->GoalNode->setSpacing(0);
    ui->OpenResult->setSpacing(0);
    ui->MinOpenResult->setSpacing(0);
    ui->CloseResult->setSpacing(0);

    ui->SelectDimesion->setSuffix("数码难题");
    ui->SelectDimesion->setSingleStep(7);
    ui->SelectDimesion->setRange(8, 15);

    // setTable
    ui->openTable->setModel(open_table_model_);
    ui->openTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->openTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->openTable->setColumnWidth(0,40);
    ui->openTable->setColumnWidth(1,40);
    ui->openTable->setColumnWidth(2,55);
    ui->openTable->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->openTable->setAlternatingRowColors(true);
    ui->openTable->verticalHeader()->hide();
    ui->openTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->openTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->closeTable->setModel(close_table_model_);
    ui->closeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->closeTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->closeTable->setColumnWidth(0,40);
    ui->closeTable->setColumnWidth(1,40);
    ui->closeTable->setColumnWidth(2,55);
    ui->closeTable->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->closeTable->setAlternatingRowColors(true);
    ui->closeTable->verticalHeader()->hide();
    ui->closeTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->closeTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_SelectDimesion_valueChanged(int arg1)
{
    dimesion_ = sqrt(arg1 + 1);
    ui->TestLabel->setText(QString::number(arg1)+"__"+QString::number(dimesion_));
}

void MainWindow::cleanNode(QGridLayout *g_layout)
{
    QLayoutItem *child;
    while(child=g_layout->layout()->takeAt(0))
    {
        // 防止删除后不消失
        if(child->widget())
        {
            child->widget()->setParent(NULL);
        }

        delete child;
    }
}

void MainWindow::on_CreateTable_clicked()
{
    cleanNode(ui->OrigionNode);
    cleanNode(ui->GoalNode);

    n_digital_->getDefault3Map(origion_, goal_);
    n_digital_->setMap3(origion_, goal_);

    for (int i = 0; i < dimesion_; i++)
    {
        for (int j = 0; j < dimesion_; j++)
        {
            ui->GoalNode->addWidget(newLable(goal_[i][j]), i, j, 1, 1);
            ui->OrigionNode->addWidget(newLable(origion_[i][j]), i, j, 1, 1);
        }
    }
}

QLabel* MainWindow::newLable(int num)
{
    QLabel *new_label;
    if (num == 0)
    {
        new_label = new QLabel(" ");
    }
    else
    {
        new_label = new QLabel(QString::number(num));
    }

    new_label->setFrameShape (QFrame::Box);
    return new_label;
}

void MainWindow::on_ProcessOnce_clicked()
{
    int result = n_digital_->findNext();
    updateTable();
    updateMap();
    if (result == 0)
    {
        QMessageBox::information(this,"提示","查找完成");
    }
}

void MainWindow::updateTable()
{
    // update open table
    std::vector<TableTips> open_tips = n_digital_->getOpenTableTips();
    open_table_model_->removeRows(0, open_table_model_->rowCount());
    for (int i = 0; i < open_tips.size(); i++)
    {
        open_table_model_->setItem(i, 0, new QStandardItem(QString::number(open_tips[i].id)));
        open_table_model_->setItem(i, 1, new QStandardItem(QString::number(open_tips[i].layer)));
        open_table_model_->setItem(i, 2, new QStandardItem(QString::number(open_tips[i].value)));
    }

    // update close table
    std::vector<TableTips> close_tips = n_digital_->getCloseTableTips();
    close_table_model_->removeRows(0, close_table_model_->rowCount());
    for (int i = 0; i < close_tips.size(); i++)
    {
        close_table_model_->setItem(i, 0, new QStandardItem(QString::number(close_tips[i].id)));
        close_table_model_->setItem(i, 1, new QStandardItem(QString::number(close_tips[i].layer)));
        close_table_model_->setItem(i, 2, new QStandardItem(QString::number(close_tips[i].value)));
    }
}

void MainWindow::updateMap()
{
    cleanNode(ui->CloseResult);

    // update close_map
    std::vector<std::vector<int>> close_map = n_digital_->getEndCloseMap();
    for (int i = 0; i < dimesion_; i++)
    {
        for (int j = 0; j < dimesion_; j++)
        {
            ui->CloseResult->addWidget(newLable(close_map[i][j]), i, j, 1, 1);
        }
    }
}