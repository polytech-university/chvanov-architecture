#include "clientinterface.h"

ClientInterface * ClientInterface::existing_object = nullptr;

LLM ClientInterface::GetLLM(const unsigned int id)
{
    QNetworkRequest request;

    QUrl url("http://localhost:80/api/llm/" + QString::number(id));
    qDebug() << "\nRequesting URL:" << url.toString();

    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply * reply = m_client->get(request);


    QEventLoop evtLoop;
    QObject::connect(reply, &QNetworkReply::readyRead,     &evtLoop, &QEventLoop::quit);
    QObject::connect(reply, &QNetworkReply::errorOccurred, &evtLoop, &QEventLoop::quit);
    QObject::connect(reply, &QNetworkReply::sslErrors,     &evtLoop, &QEventLoop::quit);
    evtLoop.exec();

    LLM result;

    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Error:" << reply->errorString();
        return result;
    }

    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll(), &error);
    
    if (error.error != QJsonParseError::NoError) {
        qDebug() << "Ошибка при парсинге JSON:" << error.errorString();
        return result;
    }

    if (!jsonDoc.isObject()) {
        qDebug() << "Error: Expected JSON object for LLM";
        return result;
    }

    QJsonObject rootObj = jsonDoc.object();

    result.id = rootObj["id"].toInt();
    
    result.model_name = rootObj["model_name"].toString();
    result.opensource = rootObj["opensource"].toBool();
    result.reasoning = rootObj["reasoning"].toBool();

    result.param_number_billions = rootObj["param_number_billions"].toInt();
    result.context_window = rootObj["context_window"].toInt();
    result.cost = rootObj["cost"].toDouble();

    result.MMLU = rootObj["MMLU"].toDouble();
    result.TruthfulQA = rootObj["TruthfulQA"].toDouble();
    result.Perplexity = rootObj["Perplexity"].toDouble();

    qDebug() << "—————OUTPUT——————"
             << "\nModel name:" << result.model_name << "\nOpensource:" << result.opensource
             << "\nReasoning:" << result.reasoning << "\nNumber of parameters:" << result.param_number_billions
             << "\nContext window length:" << result.context_window << "\nCost:" << result.cost
             << "\nMMLU:" << result.MMLU << "\nTruthfulQA:" << result.TruthfulQA << "\nPerplexity:" << result.Perplexity
             << "\n—————————————————";

    return result;
}

void ClientInterface::GetLLMs() {
    QNetworkRequest request;

    QUrl url("http://localhost:80/api/llm");

    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply * reply = m_client->get(request);

    QEventLoop evtLoop;
    QObject::connect(reply, &QNetworkReply::readyRead,     &evtLoop, &QEventLoop::quit);
    QObject::connect(reply, &QNetworkReply::errorOccurred, &evtLoop, &QEventLoop::quit);
    QObject::connect(reply, &QNetworkReply::sslErrors,     &evtLoop, &QEventLoop::quit);
    evtLoop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Error:" << reply->errorString();
        return;
    }

    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll(), &error);
    
    if (error.error != QJsonParseError::NoError) {
        qDebug() << "JSON Parse Error:" << error.errorString();
        return;
    }

    QList<LLM> LLMs;

    if (jsonDoc.isArray()) {
        QJsonArray jsonArray = jsonDoc.array();
        for (const QJsonValue &value : jsonArray) {
            if (value.isObject()) {
                LLM llm = LLM::fromJson(value.toObject());
                if (llm.id != 0) {
                    LLMs.append(llm);
                }
            }
        }
    } else {
        qDebug() << "Ожидается массив LLM";
        return;
    }

    for (const LLM &llm : LLMs) {
        qDebug() << "——————— LLM ID:" << llm.id << "———————"
                 << "\nModel name:" << llm.model_name 
                 << "\nOpen source:" << (llm.opensource ? "Yes" : "No")
                 << "\nReasoning:" << (llm.reasoning ? "Yes" : "No")
                 << "\nParameters:" << llm.param_number_billions << "B"
                 << "\nContext window:" << llm.context_window << "tokens"
                 << "\nCost:" << llm.cost << "USD"
                 << "\nBenchmarks:"
                 << "\n  - MMLU:" << llm.MMLU << "%"
                 << "\n  - TruthfulQA:" << llm.TruthfulQA << "%"
                 << "\n  - Perplexity:" << llm.Perplexity
                 << "\n———————————————————————\n";
    }

    if (LLMs.isEmpty()) {
        qDebug() << "В базе данных нет LLM\n";
    }

    m_LLMs = LLMs;
}

void ClientInterface::CreateLLM() {
    QJsonObject json;

    json["model_name"] = "Orion";
    json["opensource"] = true;
    json["reasoning"] = true;
    json["param_number_billions"] = 10000;
    json["context_window"] = 128000;
    json["cost"] = 9.2;
    json["MMLU"] = 87;
    json["TruthfulQA"] = 90;
    json["Perplexity"] = 89;

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    QNetworkRequest request;

    QUrl url("http://localhost:80/api/llm");
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply * reply = m_client->post(request, data);

    QEventLoop eventLoop;
    QObject::connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "LLM успешно добавлена";
        qDebug() << "Ответ сервера:" << reply->readAll();
    } else {
        qDebug() << "Ошибка при создании LLM:" << reply->errorString();
    }
}

void ClientInterface::UpdateLLM(const unsigned int id) {
    QJsonObject json;

    json["model_name"] = "Orion 1";
    json["context_window"] = 156000;

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    QNetworkRequest request;

    QUrl url("http://localhost:80/api/llm/" + QString::number(id));
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply * reply = m_client->sendCustomRequest(request, "PATCH", data);

    QEventLoop eventLoop;
    QObject::connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "LLM" << id << "успешно обновлена";
        qDebug() << "Ответ сервера:" << reply->readAll();
    } else {
        qDebug() << "Ошибка при редактировании LLM" << id << ":" << reply->errorString();
    }
}

void ClientInterface::DeleteLLM(const unsigned int id) {
    QNetworkRequest request;

    QUrl url("http://localhost:80/api/llm/" + QString::number(id));
    qDebug() << "\nRequesting URL:" << url.toString();

    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply * reply = m_client->deleteResource(request);

    QEventLoop evtLoop;

    QObject::connect(reply, &QNetworkReply::finished, &evtLoop, &QEventLoop::quit);
    evtLoop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "LLM" << id << "успешно удалена";
    } else {
        qDebug() << "Ошибка при удалении LLM" << id << ":" << reply->errorString();
    }
}
