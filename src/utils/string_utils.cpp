#include "string_utils.h"

void StringUtils::getKeysFromString(QString t_string, QStringList *t_list) {
    QString key;
    bool inKey = false;
    for (const auto &c : t_string) {
        if (inKey) {
            key += QString(c);
        }

        if (c == '%' && inKey) {
            t_list->append(key);
            key = "";
            inKey = false;
        } else if (c == '%') {
            key += QString(c);
            inKey = true;
        }
    }
}
