#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <queue>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    current_power = -1;
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString str = ui->textEdit->toPlainText();
    QStringList str_list = str.split(" ",QString::SkipEmptyParts);
    std::queue<unsigned int> array;

    foreach(QString num, str_list)
        array.push(num.toInt());

    if (ui->comboBox->currentIndex()  == 0 ){
        if (current_power == -1){
            current_power = 1;
        }

    }else{
        if (current_power == -1){
            foreach(QString num, str_list)
                if (num.length() > current_power){
                    current_power = num.length();
                }
        }
    }

    std::queue<unsigned int> temp[10];

    while (!array.empty()){
        unsigned int x = array.front();
        x %= int(pow(10, current_power));
        if (current_power > 0){
            x /= int(pow(10, current_power-1));
        }
        temp[x].push(array.front());
        array.pop();
    }

    QString result;
    for (unsigned int i = 0; i<10; ++i){
        QString tmp;
        while(!temp[i].empty()){
            tmp.append(QString::number(temp[i].front()));
            tmp.append(' ');
            temp[i].pop();
        }
        //SET TMP TO TABLE
        QTableWidgetItem *setdes = new QTableWidgetItem;

        setdes->setText(tmp);
        ui->tableWidget->setItem(i,0,setdes);
        result.append(tmp);
    }

    ui->textEdit->setText(result);
    if (ui->comboBox->currentIndex()  == 0 ){
        current_power += 1;
    }else{

    }
}

void MainWindow::on_tableWidget_clicked(const QModelIndex &index)
{
    int i = index.row();
    if (ui->comboBox->currentIndex() != 0){
        MainWindow *temp = new MainWindow();
        temp->ui->textEdit->setText(ui->tableWidget->item(i, 0)->text());
        temp->current_power = current_power - 1;
        temp->ui->comboBox->setCurrentIndex(1);
        temp->show();

        //LIKE WHILE FOR FORM
        QEventLoop loop;
        connect(temp, SIGNAL(destroyed()), & loop, SLOT(quit()));
        ///RECORD
        QTableWidgetItem *setdes = new QTableWidgetItem;
        setdes->setText(temp->ui->textEdit->toPlainText());
        ui->tableWidget->setItem(i, 0, setdes);

        loop.exec();
        //END OF WHILE
    }
}
