#include "gui.h"
#include "../Image2D/Image.h"

GUI::GUI(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    labelImageSetter.setLabel(ui.img2DLabel);
    Image image;
    image.loadImage("test.jpg");
    labelImageSetter.setImage(image.getImage());
}
