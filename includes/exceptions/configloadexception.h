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
        message = arg == "" ? QString(desc) : QString(desc).arg(arg);

        if (!ConfigLoadException::context.isEmpty())
        {
            message += ", when " + ConfigLoadException::context + ".";
        }
    }

    QString getMessage()
    {
        return message;
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

        ConfigLoadException::context = QString("loading %1 widget (id: %2)")
                .arg(name).arg(id);
    }

    static QString getInstrumentLoadContext()
    {
        return ConfigLoadException::context;
    }

    ~ConfigLoadException() throw()
    {

    }
};

#endif // FILENOTFOUNDEXCEPTION_H
