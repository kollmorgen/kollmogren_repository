#ifndef KOLL_H
#define KOLL_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class koll; }
QT_END_NAMESPACE

class koll : public QMainWindow
{
    Q_OBJECT

public:
    koll(QWidget *parent = nullptr);
    ~koll();

private slots:

private:
    Ui::koll *ui;
};
#endif // KOLL_H
