#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "inappdialog.h"

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    
private:
    InAppDialog *dialog;

public slots:
    void showDialog(InAppDialog *d);

private slots:
    void dialogResized();
    void dialogCloseNeeded(const InAppDialog::DialogResult &result);
};

#endif // MAINWINDOW_H
