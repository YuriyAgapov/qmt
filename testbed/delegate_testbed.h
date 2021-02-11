#pragma once

#include <QApplication>
#include <QDebug>
#include <QMap>
#include <QTableView>
#include <QTimer>
#include <QTreeView>

#include <algorithm>
#include <memory>
#include <set>

#include <qmt.h>

struct Task
{
    QUuid id;
    QString assagnee;
    QString reporter;
    QDateTime estimate = QDateTime::currentDateTime();
    int logged = 0;
    double coast = 0.0;
    bool checked = true;
    QVariantHash attributes;

    void setReporter(const QString& str)
    {
        reporter = str;
    }
    QString getReporter() const
    {
        return reporter;
    }
};

QMT_DECLARE_METATYPE(Task)

inline Qmt::StaticTypeSP qmtMetaTask()
{
    return std::make_shared<Qmt::StaticType>(
                qMetaTypeId<Task>(),
                Qmt::StaticType::PropertyHash
    {
        QMT_PROPERTY_MEMBER(Task, assagnee),
        QMT_PROPERTY_METHOD(Task, reporter, QString, getReporter, setReporter),
        QMT_PROPERTY_MEMBER(Task, estimate),
        QMT_PROPERTY_MEMBER(Task, id),
        QMT_PROPERTY_MEMBER(Task, logged),
        QMT_PROPERTY_MEMBER(Task, coast),
        QMT_PROPERTY_MEMBER(Task, checked),
        QMT_PROPERTY_MEMBER(Task, estimate),
        QMT_PROPERTY_MEMBER(Task, estimate)
    });
}

Qmt::DelegateSettingsCSP makeStringSettings(const QString& placeholder)
{
    auto settings = std::make_shared<Qmt::StringDelegateSettings>();
    settings->placeholderText = placeholder;
    return settings;
}
Qmt::DelegateSettingsCSP makeNumberSettings(const int precision, const QString& suffix)
{
    auto settings = std::make_shared<Qmt::NumberDelegateSettings>();
    settings->precision = precision;
    settings->suffix = suffix;
    settings->min = -900;
    settings->max = 100000;
    settings->step = 0.01;
    return settings;
}

Qmt::DelegateSettingsCSP makeDateTimeSettings()
{
    auto settings = std::make_shared<Qmt::DateTimeDelegateSettings>();
    settings->format = "hh:mm:ss dd.MM.yyyy";
    return settings;
}

std::shared_ptr<Qmt::IDelegateSettingsModel> createTaskSettingsModel()
{
    auto settingsModel = std::make_shared<Qmt::DelegateSettingsModel>();
    settingsModel->setSettings("assagnee", makeStringSettings("assagnee"));
    settingsModel->setSettings("reporter", makeStringSettings("reporter"));
    settingsModel->setSettings("coast", makeNumberSettings(2, " rub"));
    settingsModel->setSettings("estimate", makeDateTimeSettings());
    return settingsModel;
}

void createView(QAbstractItemModel* model, std::shared_ptr<Qmt::IDelegateSettingsModel> settingsModel)
{
    auto delegateManager = Qmt::DelegateManager::defaultInstance();


    QTableView* tableView = new QTableView();
    tableView->setModel(model);
    tableView->setItemDelegate(new Qmt::ItemDelegate(delegateManager, settingsModel));

    QTreeView* treeView = new QTreeView();
    treeView->setModel(model);
    treeView->setItemDelegate(new Qmt::ItemDelegate(delegateManager, settingsModel));

    QTabWidget* tabWidget = new QTabWidget();
    tabWidget->addTab(tableView, "Table");
    tabWidget->addTab(treeView, "Tree");
    tabWidget->show();
    tabWidget->resize(1024, 768);
}


struct DelegateTestbed
{
    using TaskObject = Qmt::ObjectWrapper<Task>;
    using TaskTable = Qmt::TableWrapper<std::vector<Task>>;

    DelegateTestbed()
    {
        QMT_REGISTER(Task);

        const QStringList taskViewProperties = {"assagnee", "reporter", "coast", "checked", "estimate", "logged"};
        taskSettingsModel = createTaskSettingsModel();

        Task task;
        task.id = QUuid::createUuid();
        task.attributes = QVariantHash{{"1", 1}};

        tasks.push_back(task);
        tasks.push_back(task);

        taskTable = std::make_shared<TaskTable>(tasks);
        taskTable->insert(1, QVariantHash{{"reporter", "Petr"}});

        tableModel = std::make_shared<Qmt::TableModel>();
        tableModel->setProperties(taskViewProperties);
        tableModel->setTable(taskTable);
        createView(tableModel.get(), taskSettingsModel);

        taskObject = std::make_shared<TaskObject>(tasks[0]);

        objectModel = std::make_shared<Qmt::ObjectModel>();
        objectModel->setObject(taskObject);
        objectModel->setProperties(taskViewProperties);
        createView(objectModel.get(), taskSettingsModel);

        QTimer::singleShot(500, Qt::PreciseTimer, [task, this]()
        {
            Task newTask = task;
            newTask.assagnee = "Welcome!";
            taskObject->setValue(newTask);
        });
        QTimer::singleShot(1500, Qt::PreciseTimer, [this]()
        {
            taskObject->setProperty("reporter", "Welcome2!");
        });
    }

    std::vector<Task> tasks;
    std::shared_ptr<Qmt::IDelegateSettingsModel> taskSettingsModel;
    std::shared_ptr<TaskTable> taskTable;
    std::shared_ptr<TaskObject> taskObject;
    std::shared_ptr<Qmt::TableModel> tableModel;
    std::shared_ptr<Qmt::ObjectModel> objectModel;
};
