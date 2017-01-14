#ifndef EDITCLASS_H
#define EDITCLASS_H

#include <QHeaderView>
#include <QWidget>
#include <QLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QMap>
#include <QPointer>
#include "visuconfiguration.h"
#include "visumisc.h"

class EditConfiguration : public QWidget
{
    Q_OBJECT

public:
    EditConfiguration(QPointer<VisuConfiguration> configuration);

signals:
    void configParamsUpdated();

private slots:
    void updateConfigParams();
    void cellUpdated(int row, int col);

private:
    QPointer<VisuConfiguration> mConfiguration;
    QMap<QString, QString> mProperties;
    QPointer<QTableWidget> mTable;
};

#endif // EDITCLASS_H
