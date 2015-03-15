#include "sharedwithothers.h"
#include "ui_sharedwithothers.h"
#include <vector>
#include <string>
#include "SyncManager.h"

//to display the list of all the files shared by the user

extern SyncManager MergedSyncManager;

std::vector<std::string> sharedfiles;

sharedwithothers::sharedwithothers(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sharedwithothers)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon("ddlogo2.png")) ;
    this->setFixedSize(500,695);
    this->setWindowTitle("Shared Files");
    std::vector<SharingGiver> v1=  MergedSyncManager.GetGivingFiles().GetGivingList();
    std::string basic= "/home/faran/Desktop/DeadDropServer/" + MergedSyncManager.GetUsername();
    for (unsigned int i = 0;i<v1.size();i++)//to display the list of files shared user with whom its shared and permissions
    {
        ui->sharedfileslist->addItem(("Filename:\t\t" + v1[i].FilePath.substr(basic.size())).c_str());
        ui->sharedfileslist->addItem((("User shared:\t" + v1[i].UserName).c_str()));
        if (v1[i].Permissions == 0)
        {
            ui->sharedfileslist->addItem("Permissions\t Read and Write");
            ui->sharedfileslist->addItem("");
        }
        else
        {
            ui->sharedfileslist->addItem("Permissions\t Read Only");            
            ui->sharedfileslist->addItem("");
        }
    }

}

//destructor
sharedwithothers::~sharedwithothers()
{
    delete ui;
}

//back button
void sharedwithothers::on_backbutton_clicked()
{
    this->hide();
}