#include "mainwindow.h"
#include "../Image/Image.h"

#include <string>
#include <sstream>
#include <chrono>
#include <QFileDialog>
#include <QImageReader>
#include <QImageWriter>
#include <QStandardPaths>
#include <QUrl>
#include "../Mesh/Vertex.h"
#include "../Mesh/Mesh.h"
#include "../Triangulation/Naive.h"
#include "../Triangulation/Greedy.h"
#include "../Triangulation/Delaunay.h"

#define CONVERSION_PAGE 0
#define OPTIONS_PAGE 1

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
  ui.setupUi(this);

  labelImageSetter.setLabel(ui.imgLabel);

  connectButtons();
  connectCheckBoxes();
  connectSliders();
  connect(this, &MainWindow::logSent, this, &MainWindow::log);
}

void MainWindow::connectButtons()
{
  connect(ui.uploadButton, &QPushButton::clicked, this, &MainWindow::loadImage);
  connect(ui.convertButton, &QPushButton::clicked, this, &MainWindow::convert);
  connect(ui.toOptionsButton, &QPushButton::clicked, this, &MainWindow::goToConversion);
  connect(ui.toConversionButton, &QPushButton::clicked, this, &MainWindow::goToOptions);
}

void MainWindow::connectCheckBoxes()
{
  connect(ui.autoLevelCheckBox, &QCheckBox::toggled, this, &MainWindow::onAutoLevelChange);
}

void MainWindow::connectSliders()
{
  connect(ui.imageHeightSlider, &QSlider::valueChanged, this, &MainWindow::onHeightChange);
}

void MainWindow::convert()
{
  auto qimage = image.getImage();
  auto heightMap = std::make_unique<HeightMap>(qimage);
  auto invert = ui.invertCheckBox->isChecked();
  if (invert) {
    heightMap->invert();
  }
  auto meshHeight = 1.;
  if (!ui.autoLevelCheckBox->isChecked()) {
    meshHeight = ui.imageHeightSlider->value() / 256.;
    heightMap->scale(meshHeight);
  }
  auto error = ui.maxErrorSpinBox->value() / 100;

  auto algorithm = ui.algorithmComboBox->currentIndex();
  std::unique_ptr<Triangulator> triangulator;

  switch (algorithm) {
  case 0:
    triangulator = std::make_unique<Naive>(std::move(heightMap), meshHeight);
    break;
  case 1:
    triangulator = std::make_unique<Greedy>(std::move(heightMap), meshHeight, error);
    break;
  case 2:
    triangulator = std::make_unique<Delaunay>(std::move(heightMap), meshHeight, error);
    break;
  }
  auto start = std::chrono::steady_clock::now();
  triangulator->run();
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;

  auto mesh = triangulator->getMesh(ui.meshWidget->getContext());
  auto fileSize = mesh->saveToSTL("test.stl");
  std::stringstream timeSize;
  timeSize << elapsed_seconds.count() << " s,   " << fileSize / 1024. << " KB ";
  ui.timeSizeLabel->setText(QString::fromStdString(timeSize.str()));
  ui.meshWidget->setMesh(std::move(mesh));
  ui.meshWidget->update();
}

void MainWindow::goToOptions()
{
  ui.stackedWidget->setCurrentIndex(CONVERSION_PAGE);
}

void MainWindow::goToConversion()
{
  ui.stackedWidget->setCurrentIndex(OPTIONS_PAGE);
}

void MainWindow::onAutoLevelChange(bool checked)
{
  ui.imageHeightLabel->setEnabled(!checked);
  ui.imageHeightSlider->setEnabled(!checked);
  ui.imageHeightValueLabel->setEnabled(!checked);
}

void MainWindow::onHeightChange(int value)
{
    ui.imageHeightValueLabel->setText(QString::fromStdString(std::to_string(value)));
}

void MainWindow::updateMesh() {
  ui.meshWidget->update();
}


static void initializeImageFileDialog(QFileDialog& dialog, QFileDialog::AcceptMode acceptMode)
{
  static bool firstDialog = true;

  if (firstDialog) {
    firstDialog = false;}

  QStringList mimeTypeFilters;
  const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
    ? QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();
  for (const QByteArray& mimeTypeName : supportedMimeTypes)
    mimeTypeFilters.append(mimeTypeName);
  mimeTypeFilters.sort();
  dialog.setMimeTypeFilters(mimeTypeFilters);
  dialog.selectMimeTypeFilter("image/jpeg");
  if (acceptMode == QFileDialog::AcceptSave)
    dialog.setDefaultSuffix("jpg");
}

void MainWindow::loadImage()
{
  QFileDialog dialog(this, tr("Open File"));
  initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

  while (dialog.exec() == QDialog::Accepted && !image.loadImage(dialog.selectedFiles().first())) {}

  labelImageSetter.setImage(image.getImage());
}

void MainWindow::log(const std::string& errorLog) {
  ui.imgLabel->clear();
  ui.imgLabel->setText(errorLog.c_str());
}
