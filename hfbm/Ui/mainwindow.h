#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include "../Image/Image.h"
#include "../Image/LabelImageSetter.h"
#include "../Mesh/Mesh.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget * parent = Q_NULLPTR);

private:
  void connectButtons();
  void connectCheckBoxes();
  void connectSliders();

signals:
  void logSent(const std::string&);

private slots:
  void updateMesh();
  void loadImage();
  void convert();
  void goToOptions();
  void goToConversion();
  void onAutoLevelChange(bool);
  void onHeightChange(int);
  void log(const std::string&);

private:
  Ui::MainWindowClass ui;
  Image image;
  LabelImageSetter labelImageSetter;
};
