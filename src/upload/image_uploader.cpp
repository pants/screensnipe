#include "image_uploader.h"

ImageUploader::ImageUploader(ImageUploader::UploadConfig *t_uploadConfig, bool t_copyToClipboard)
        : m_uploadConfig(t_uploadConfig),
          m_copyToClipboard(t_copyToClipboard) {
}

void ImageUploader::uploadImage(QBuffer &t_fileBuffer) {
    QUrl url = QUrl(m_uploadConfig->request_url);
    QNetworkRequest request(url);

    qDebug() << "Setting up headers..";
    setupHeaders(request, m_uploadConfig->request_headers);

    auto multipart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QByteArray post_data;
    qDebug() << "Setting up request body..";
    setupRequestBody(*multipart, post_data, t_fileBuffer);

    auto *netManager = new QNetworkAccessManager(this);
    connect(netManager, SIGNAL(finished(QNetworkReply * )), this,
            SLOT(onNetworkReply(QNetworkReply * )));

    QNetworkReply *reply;
    qDebug() << "Uploading image...";
    if (m_uploadConfig->multipart) {
        reply = netManager->post(request, multipart);
    } else {
        reply = netManager->post(request, post_data);
    }

    //clean up memory
    connect(reply, SIGNAL(finished()), reply, SLOT(deleteLater()));
    connect(reply, SIGNAL(destroyed()), netManager, SLOT(deleteLater()));
}

void ImageUploader::onNetworkReply(QNetworkReply *t_reply) {
    QByteArray responseData = t_reply->readAll();

    QString responseString = QString::fromUtf8(responseData);
    QString constructedResponse = m_uploadConfig->response_format;

    auto contentType = t_reply->header(QNetworkRequest::ContentTypeHeader);
    if (contentType.toString().toLower().contains("application/json")) {
        QStringList keys;
        StringUtils::getKeysFromString(m_uploadConfig->response_format, &keys);

        replaceStringJsonKeys(&constructedResponse, keys, responseString);
    } else {
        constructedResponse += responseString;
    }

    if (m_copyToClipboard) {
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(constructedResponse);
    }

    qDebug() << constructedResponse;
}

void ImageUploader::setupHeaders(QNetworkRequest &t_request, QList<QString> &t_headers) {
    t_request.setRawHeader(QString("User-Agent").toUtf8(), QString("Screensnipe").toUtf8());

    for (const QString &header : t_headers) {
        QStringList headerSplit = header.split("=");
        QString headerName = headerSplit.first();
        QString headerValue = headerSplit.last();

        t_request.setRawHeader(headerName.toUtf8(), headerValue.toUtf8());
    }
}

void ImageUploader::setupRequestBody(
        QHttpMultiPart &t_multipart, QByteArray &t_postData,
        QBuffer &t_fileBuffer
) {
    for (auto i = 0; i < m_uploadConfig->request_args.size(); i++) {
        const auto &arg = m_uploadConfig->request_args.at(i);

        QStringList argument = arg.split("=");
        QString arg_name = argument.first();
        QString arg_value = argument.last();

        QByteArray value_bytes;
        if (arg_value == "%file%") {
            value_bytes = t_fileBuffer.buffer();
        } else {
            value_bytes = arg_value.toUtf8();
        }

        if (m_uploadConfig->multipart) {
            QHttpPart part;

            if (arg_value == "%file%") {
                //todo: have actually fill in the correct image formats..
                part.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));
                auto form_header_value = QVariant("form-data; name=\"" + arg_name + "\"; filename=\"image.jpg\"");
                part.setHeader(QNetworkRequest::ContentDispositionHeader, form_header_value);
            } else {
                part.setHeader(QNetworkRequest::ContentDispositionHeader,
                               QVariant("form-data; name=\"" + arg_name + "\""));
            }

            part.setBody(value_bytes);
            t_multipart.append(part);
        } else {
            t_postData.append(arg_name.append('=').toUtf8());
            t_postData.append(value_bytes);
            if (i != m_uploadConfig->request_args.size() - 1) {
                t_postData.append(QString('&').toUtf8());
            }
        }
    }
}

void ImageUploader::replaceStringJsonKeys(QString *t_string, QStringList &t_keys, QString &t_responseData) {
    auto document = QJsonDocument::fromJson(t_responseData.toUtf8());
    auto jsonObject = document.object();

    for (const auto &variable : t_keys) {
        QString jsonKey = variable.mid(1, variable.length() - 2);
        auto jsonObj = jsonObject[jsonKey];

        if (!jsonObj.isNull()) {
            QString jsonEntry = jsonObj.toString();
            qDebug() << "Variable: " << jsonKey << " var " << jsonEntry << t_string;
            t_string->replace(variable, jsonEntry);
        } else {
            qCritical() << "Expected json key '" << jsonKey << "' but it wasn't found!";
            qCritical() << "Problem String: " << m_uploadConfig->response_format;
            qCritical() << "Problem Response: " << t_responseData;
        }
    }
}
