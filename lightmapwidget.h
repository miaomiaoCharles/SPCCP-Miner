#ifndef LIGHTMAPWIDGET_H
#define LIGHTMAPWIDGET_H

#include <QMainWindow>

namespace Ui {
class lightmapwidget;
}

class lightmapwidget : public QMainWindow
{
    Q_OBJECT

public:
    explicit lightmapwidget(QWidget *parent = nullptr);
    ~lightmapwidget();

private:
    Ui::lightmapwidget *ui;
};

#endif // LIGHTMAPWIDGET_H
