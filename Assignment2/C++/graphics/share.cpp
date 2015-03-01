#include "share.h"
#include "ui_share.h"
#include "file.h"
#include <string>
#include <QMessageBox>

bool readonly;
std::string getting_username;
std::string existingusername;

share::share(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::share)
{
    ui->setupUi(this);
    this->setObjectName("share");
    this->setStyleSheet("#share {background-color:sandybrown;}");
    this->setFixedSize(508,342);
    this->setWindowIcon(QIcon("/home/faran/Desktop/COP290/Assignment2/DesignDocument/ddlogo2.png")) ;

    ui->radioButton->setChecked(true);
    ui->pushButton->setFocusPolicy(Qt::NoFocus);
}

share::~share()
{
    delete ui;
}


void share::on_pushButton_clicked()
{
    QString username1 = ui->lineEdit->text();
    getting_username = username1.toUtf8().constData();
    if(ui->radioButton->isChecked() == true)
    {
        readonly = true;
    }
    else
    {
        readonly = false;
    }

    if(getting_username == existingusername)
    {
        ui->lineEdit->setText("");
        QMessageBox::information(this,tr("successfully shared"),tr("your file is successfully shared"));
        this->hide();
    }
    else
    {
        ui->lineEdit->setText("");
        QMessageBox::information(this,tr("Please Re-Enter"),tr("Username doesnot exist"));
    }


}

void share::on_pushButton_2_clicked()
{
    this->hide();
    file share1;
    share1.setModal(true);
    share1.exec();

}
