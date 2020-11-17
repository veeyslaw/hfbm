#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_gui.h"
#include "../Image2D/Image.h"
#include "../Image2D/LabelImageSetter.h"

class GUI : public QMainWindow
{
  Q_OBJECT

public:
  GUI(std::shared_ptr<Image> image, QWidget *parent = Q_NULLPTR);

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
    
  std::shared_ptr<Image> image;
  LabelImageSetter labelImageSetter;
};
