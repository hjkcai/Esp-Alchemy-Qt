#ifndef INAPPDIALOG_H
#define INAPPDIALOG_H

#include <QWidget>
#include <QGraphicsEffect>

class InAppDialog : public QWidget
{
    Q_OBJECT
public:
    enum DialogResult
    {
        None,
        OK,
        Cancel
    };

    explicit InAppDialog(QWidget *parent = 0);

private:

#ifndef Q_OS_MAC
    QGraphicsDropShadowEffect *_shadow;
#endif

signals:
    void DialogClosed(const DialogResult &result);

protected:
    void closeDialog(const DialogResult &result);
};

#endif // INAPPDIALOG_H
