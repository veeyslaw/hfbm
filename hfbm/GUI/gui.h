#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_gui.h"
#include "../Image2D/LabelImageSetter.h"

class GUI : public QMainWindow
{
    Q_OBJECT

public:
    GUI(QWidget *parent = Q_NULLPTR);

private:
    Ui::GUIClass ui;
    
    LabelImageSetter labelImageSetter;
};
