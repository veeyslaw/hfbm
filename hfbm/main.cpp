#include "GUI/gui.h"
#include "Image2D/Image.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto image = std::make_shared<Image>();

    GUI w(image);
    w.show();
    return a.exec();
}
