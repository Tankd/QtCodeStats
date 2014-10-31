#ifndef CABOUTDIALOG_H
#define CABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class CAboutDialog;
}

class CAboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CAboutDialog(QWidget *parent = 0);
    ~CAboutDialog();

private slots:
    void on_aboutQtButton_clicked();

    void on_okButton_clicked();

private:
    Ui::CAboutDialog *ui;
};

#endif // CABOUTDIALOG_H
