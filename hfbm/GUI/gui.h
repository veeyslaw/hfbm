#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_gui.h"
#include "../Image/Image.h"
#include "../Image/LabelImageSetter.h"

class GUI : public QMainWindow
{
  Q_OBJECT

public:
  GUI(QWidget *parent = Q_NULLPTR);

private:
  void connectButtons();
  void connectCheckBoxes();

private slots:
  void loadImage();
  void convert();
  void goToOptions();
  void goToConversion();
  void onAutoLevelChange(bool checked);
  void onBorderChange(bool checked);
  void onBaseChange(bool checked);

private:
  Ui::GUIClass ui;
    
  Image image;
  
  LabelImageSetter labelImageSetter;
};
