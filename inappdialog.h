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
    void dialogClose(const DialogResult &result);
    void dialogResize(const int &w, const int &y);
    void resized();

protected:
    void closeDialog(const DialogResult &result);
    void resizeEvent(QResizeEvent *);
};

#endif // INAPPDIALOG_H
