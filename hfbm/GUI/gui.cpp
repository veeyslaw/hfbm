#include "gui.h"
#include "../Image2D/Image.h"

#include <QFileDialog>
#include <QImageReader>
#include <QImageWriter>
#include <QStandardPaths>

#include <QMessageBox> // TODELETE

static void initializeImageFileDialog(QFileDialog& dialog, QFileDialog::AcceptMode acceptMode);

GUI::GUI(std::shared_ptr<Image> image, QWidget *parent)
    : QMainWindow(parent), image(image)
{
  ui.setupUi(this);

  labelImageSetter.setLabel(ui.img2DLabel);

  connectButtons();
}

void GUI::connectButtons()
{
  connect(ui.uploadButton, SIGNAL(clicked()), this, SLOT(loadImage()));
}

void GUI::loadImage()
{
  QFileDialog dialog(this, tr("Open File"));
  initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

  while (dialog.exec() == QDialog::Accepted && !image->loadImage(dialog.selectedFiles().first())) {}

  labelImageSetter.setImage(image->getImage());
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