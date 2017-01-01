#ifndef FILENOTFOUNDEXCEPTION_H
#define FILENOTFOUNDEXCEPTION_H

#include <exception>
#include <QString>
#include <QMap>

class ConfigLoadException : public std::exception
{
private:
    static QString context;
    QString message;

public:

    ConfigLoadException(QString desc, QString arg = "")
    {
        message = QString(desc).arg(arg);

        if (!ConfigLoadException::context.isEmpty())
        {
            message += ", when " + ConfigLoadException::context + ".";
        }
    }

    virtual const char* what() const throw()
    {
        return message.toLatin1().data();
    }

    static void setContext(QString context)
    {
        ConfigLoadException::context = context;
    }

    static void setInstrumentLoadContext(QMap<QString, QString> properties)
    {
        int id = properties.contains("id") ? properties["id"].toInt() : -1;
        QString name = properties.contains("name") ? properties["name"] : QString("Unknown");

        ConfigLoadException::context = QString("loading %1 instrument (id: %2)")
                .arg(name).arg(id);
    }

    ~ConfigLoadException() throw()
    {

    }
};

#endif // FILENOTFOUNDEXCEPTION_H
