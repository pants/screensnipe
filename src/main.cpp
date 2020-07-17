#include <QApplication>
#include "main/ScreenSnipe.h"

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);

    auto* screenSnipe = new ScreenSnipe(&application);
    screenSnipe->setupHotkeys();

    return QApplication::exec();
}
