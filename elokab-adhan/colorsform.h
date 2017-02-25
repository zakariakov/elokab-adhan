#ifndef COLORSFORM_H
#define COLORSFORM_H

#include <QWidget>
#include "buttoncolor/buttoncolor.h"
namespace Ui {
class ColorsForm;
}

class ColorsForm : public QWidget
{
    Q_OBJECT

public:
    explicit ColorsForm(QWidget *parent = 0);
    ~ColorsForm();
void saveSettings();
private:
    Ui::ColorsForm *ui;

    //----colors----------
    ButtonColor *btnColorBackground;
ButtonColor *btnColorText;
 ButtonColor *btnColorNext;
   ButtonColor *btnColorPrev;
};

#endif // COLORSFORM_H
