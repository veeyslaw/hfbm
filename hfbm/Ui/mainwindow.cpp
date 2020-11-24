#include "mainwindow.h"
#include "../Image/Image.h"

#include <QFileDialog>
#include <QImageReader>
#include <QImageWriter>
#include <QStandardPaths>
#include <QUrl>

#include "../Mesh/Vertex.h"
#include "../Mesh/Mesh.h"

#include <string>

#define CONVERSION_PAGE 0
#define OPTIONS_PAGE 1

static void initializeImageFileDialog(QFileDialog& dialog, QFileDialog::AcceptMode acceptMode);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
  ui.setupUi(this);

  labelImageSetter.setLabel(ui.imgLabel);

  connectButtons();
  connectCheckBoxes();

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
  connect(ui.borderCheckBox, &QCheckBox::toggled, this, &MainWindow::onBorderChange);
  connect(ui.baseCheckBox, &QCheckBox::toggled, this, &MainWindow::onBaseChange);
}

void MainWindow::convert()
{
  // TODO replace testing code with real one
  std::vector<Vertex> vertices;
  vertices.push_back(Vertex(glm::fvec3(200, 0, 0)));
  vertices.push_back(Vertex(glm::fvec3(0, 0, 0)));
  vertices.push_back(Vertex(glm::fvec3(0, 200, 0)));

  vertices.push_back(Vertex(glm::fvec3(200, 0, 0)));
  vertices.push_back(Vertex(glm::fvec3(0, 200, 0)));
  vertices.push_back(Vertex(glm::fvec3(200, 200, 0)));

  vertices.push_back(Vertex(glm::fvec3(0, 0, 0)));
  vertices.push_back(Vertex(glm::fvec3(100, 100, 6)));
  vertices.push_back(Vertex(glm::fvec3(0, 100, 0)));

  vertices.push_back(Vertex(glm::fvec3(0, 0, 0)));
  vertices.push_back(Vertex(glm::fvec3(100, 0, 0)));
  vertices.push_back(Vertex(glm::fvec3(100, 100, 6)));

  vertices.push_back(Vertex(glm::fvec3(0, 100, 0)));
  vertices.push_back(Vertex(glm::fvec3(100, 200, 0)));
  vertices.push_back(Vertex(glm::fvec3(0, 200, 0)));

  vertices.push_back(Vertex(glm::fvec3(0, 100, 0)));
  vertices.push_back(Vertex(glm::fvec3(100, 100, 6)));
  vertices.push_back(Vertex(glm::fvec3(100, 200, 0)));

  vertices.push_back(Vertex(glm::fvec3(100, 0, 0)));
  vertices.push_back(Vertex(glm::fvec3(200, 100, 0)));
  vertices.push_back(Vertex(glm::fvec3(100, 100, 6)));

  vertices.push_back(Vertex(glm::fvec3(100, 0, 0)));
  vertices.push_back(Vertex(glm::fvec3(200, 0, 0)));
  vertices.push_back(Vertex(glm::fvec3(200, 100, 0)));

  vertices.push_back(Vertex(glm::fvec3(100, 100, 6)));
  vertices.push_back(Vertex(glm::fvec3(200, 200, 0)));
  vertices.push_back(Vertex(glm::fvec3(100, 200, 0)));

  vertices.push_back(Vertex(glm::fvec3(100, 100, 6)));
  vertices.push_back(Vertex(glm::fvec3(200, 100, 0)));
  vertices.push_back(Vertex(glm::fvec3(200, 200, 0)));

  Mesh mesh(vertices);

  mesh.saveToSTL("test.stl");

  std::string str(std::to_string(0));
  ui.imgLabel->setText(str.c_str());
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
  ui.imageHeightLabel->setEnabled(checked);
  ui.imageHeightSlider->setEnabled(checked);
  ui.imageHeightValueLabel->setEnabled(checked);
}

void MainWindow::onBorderChange(bool checked)
{
  ui.borderThicknessLabel->setEnabled(checked);
  ui.borderThicknessSlider->setEnabled(checked);
  ui.borderThicknessValueLabel->setEnabled(checked);
  ui.borderHeightLabel->setEnabled(checked);
  ui.borderHeightSlider->setEnabled(checked);
  ui.borderHeightValueLabel->setEnabled(checked);
}

void MainWindow::onBaseChange(bool checked)
{
  ui.baseHeightLabel->setEnabled(checked);
  ui.baseHeightSlider->setEnabled(checked);
  ui.baseHeightValueLabel->setEnabled(checked);
}

void MainWindow::updateMesh() {
  ui.meshWidget->update();
}

void MainWindow::loadImage()
{
  QFileDialog dialog(this, tr("Open File"));
  initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

  while (dialog.exec() == QDialog::Accepted && !image.loadImage(dialog.selectedFiles().first())) {}

  labelImageSetter.setImage(image.getImage());
}

static void initializeImageFileDialog(QFileDialog& dialog, QFileDialog::AcceptMode acceptMode)
{
  static bool firstDialog = true;

  if (firstDialog) {
    firstDialog = false;
    const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
  }

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