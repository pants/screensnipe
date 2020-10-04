//
// Created by andrew on 9/6/20.
//

#ifndef SCREEN_SNIPE_UPLOAD_CONFIG_H
#define SCREEN_SNIPE_UPLOAD_CONFIG_H

#include <QString>
#include <QList>

struct UploadConfig {
    QString destination_url; //https://imgur.com/upload
    QString upload_type; //POST, GET, PUT
    QList<QString> request_headers;
    QList<QString> post_upload_args;
    QList<QString> get_upload_args;
};

#endif //SCREEN_SNIPE_UPLOAD_CONFIG_H
