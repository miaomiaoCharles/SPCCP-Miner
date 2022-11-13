#include "lightmapwidget.h"
#include "ui_lightmapwidget.h"

lightmapwidget::lightmapwidget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::lightmapwidget)
{
    ui->setupUi(this);
}

lightmapwidget::~lightmapwidget()
{
    delete ui;
}
