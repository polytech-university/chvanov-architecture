#ifndef CLIENTINTERFACE_H
#define CLIENTINTERFACE_H

#include <QEventLoop>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QString>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QList>
#include <QNetworkAccessManager>

class LLM {
public:
    unsigned int id;

    QString model_name;
    bool opensource;
    bool reasoning;

    unsigned int param_number_billions;
    unsigned int context_window;
    double cost;

    double MMLU;
    double TruthfulQA;
    double Perplexity;

    LLM() : id(0) {}

    LLM(QString model_name, bool opensource, bool reasoning, 
        unsigned int param_number_billions, unsigned int context_window, 
        double cost, double MMLU, double TruthfulQA, double Perplexity) 
        : id(0), model_name(model_name), opensource(opensource), reasoning(reasoning),
          param_number_billions(param_number_billions), context_window(context_window),
          cost(cost), MMLU(MMLU), TruthfulQA(TruthfulQA), Perplexity(Perplexity) {}

    static LLM fromJson(const QJsonObject &json)
    {
        LLM llm;
        llm.id = json["id"].toInt();
        llm.model_name = json["model_name"].toString();
        llm.opensource = json["opensource"].toBool();
        llm.reasoning = json["reasoning"].toBool();
        llm.param_number_billions = json["param_number_billions"].toInt();
        llm.context_window = json["context_window"].toInt();
        llm.cost = json["cost"].toDouble();
        llm.MMLU = json["MMLU"].toDouble();
        llm.TruthfulQA = json["TruthfulQA"].toDouble();
        llm.Perplexity = json["Perplexity"].toDouble();
        return llm;
    }

    QString name() const { return model_name; }
};

class ClientInterface {
public:
    static ClientInterface * getIntstance() {
        if (existing_object == nullptr) {
            existing_object = new ClientInterface();
        }
        return existing_object;
    }

    static void removeInstance() {
        if (existing_object) {
            delete existing_object;
        }
        existing_object = nullptr;
    }

    LLM GetLLM(const unsigned int id);
    void GetLLMs();
    void CreateLLM();
    void UpdateLLM(const unsigned int id);
    void DeleteLLM(const unsigned int id);

    const QList<LLM>& getLLMs() const { return m_LLMs; }

    ~ClientInterface() {
        delete m_client;
    }

private:
    static ClientInterface * existing_object;
    ClientInterface() : m_client(new QNetworkAccessManager()) {}
    
    QNetworkAccessManager * m_client;
    QList<LLM> m_LLMs;
};

#endif // CLIENTINTERFACE_H
