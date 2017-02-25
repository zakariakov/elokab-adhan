#include "colorsform.h"
#include "ui_colorsform.h"
#include<QSettings>

ColorsForm::ColorsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorsForm)
{
    ui->setupUi(this);
    QSettings cg("elokab","adhan");
    cg.beginGroup("Adhan");



   //----------colors-----------------
    btnColorBackground->setColor(cg.value("ColorBackground",QColor(250, 255, 212, 255)).value<QColor>());
    btnColorText->setColor(cg.value("ColorText",QColor(0, 0, 0, 255)).value<QColor>());
    btnColorNext->setColor(cg.value("ColorNext",QColor(0, 0, 255, 255)).value<QColor>());
    btnColorPrev->setColor(cg.value("ColorPrev",QColor(255, 0, 0, 255)).value<QColor>());
    cg.endGroup();

}

ColorsForm::~ColorsForm()
{
    delete ui;
}
void ColorsForm::saveSettings()
{
    QSettings cg("elokab","adhan");
    cg.beginGroup("Adhan");

//----------colors-----------------
    cg.setValue("ColorBackground", btnColorBackground->color());
    cg.setValue("ColorText", btnColorText->color());
    cg.setValue("ColorNext", btnColorNext->color());
    cg.setValue("ColorPrev", btnColorPrev->color());
    cg.endGroup();

}
