#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{
}

void MainWindow::showDialog(InAppDialog *d)
{
    this->dialog = d;

    dialog->setParent(this);
    dialog->setWindowOpacity(0);
    connect(dialog, SIGNAL(resized()), this, SLOT(dialogResized()));
    connect(dialog, SIGNAL(dialogClose(DialogResult)), this, SLOT(dialogClose(InAppDialog::DialogResult)));
}

void MainWindow::dialogCloseNeeded(const InAppDialog::DialogResult &result)
{

}

void MainWindow::dialogResized()
{
    dialog->move(this->width() / 2 - dialog->width() / 2, this->height() / 2 - dialog->height() / 2);
}
