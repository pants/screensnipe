#ifndef SCREEN_SNIPE_IMAGE_UPLOADER_H
#define SCREEN_SNIPE_IMAGE_UPLOADER_H

#include "../main/screen_snipe.h"
#include "../utils/string_utils.h"
#include <QString>
#include <QList>
#include <QByteArray>
#include <QBuffer>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QHttpMultiPart>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

class ImageUploader : public QObject {
Q_OBJECT
public:
    struct UploadConfig {
        QString request_url; //https://imgur.com/upload
        QString request_method; //POST, GET, PUT

        QList<QString> request_headers; //user-agent: ....
        QList<QString> request_args; //file=%file%

        bool multipart = false;

        QString response_json_key; //used if the destination returns json that needs to be parsed
        //When we get the response, from either json or regular, add this before and after
        QString response_format;
    };

public:
    explicit ImageUploader(UploadConfig *t_uploadConfig, bool t_copyToClipboard);
    void uploadImage(QBuffer &t_fileBuffer);

private:
    static void setupHeaders(QNetworkRequest &t_request, QList<QString> &headers);

    void setupRequestBody(QHttpMultiPart &t_multipart, QByteArray &t_post_data, QBuffer &t_file_buffer);

    void replaceStringJsonKeys(QString *t_string, QStringList &t_keys, QString &t_responseData);

    UploadConfig* m_uploadConfig;
    bool m_copyToClipboard;
private slots:
    void onNetworkReply(QNetworkReply *t_reply);
};


#endif //SCREEN_SNIPE_IMAGE_UPLOADER_H
