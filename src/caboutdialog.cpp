#include "caboutdialog.h"
#include "ui_caboutdialog.h"

#include <QMessageBox>

CAboutDialog::CAboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CAboutDialog)
{
    ui->setupUi(this);
}

CAboutDialog::~CAboutDialog()
{
    delete ui;
}

void CAboutDialog::on_aboutQtButton_clicked()
{
    QMessageBox::aboutQt(this);
}

void CAboutDialog::on_okButton_clicked()
{
    close();
}
