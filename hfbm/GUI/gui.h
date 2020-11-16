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

private slots:
  void loadImage();

private:
  Ui::GUIClass ui;
    
  std::shared_ptr<Image> image;
  LabelImageSetter labelImageSetter;
};
