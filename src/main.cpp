#include <QApplication>
#include "../headers/WebBrowser.hpp"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    WebBrowser browser;
    browser.show();

    return QApplication::exec();
}
