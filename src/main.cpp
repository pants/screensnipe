#include <QApplication>
#include "main/screen_snipe.h"

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);

    auto* screenSnipe = new ScreenSnipe(&application);
    screenSnipe->setupHotkeys();

    return QApplication::exec();
}
