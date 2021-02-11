#pragma once

#include <type_build/qmt_macro.h>
#include <type_build/qmt_table_wrapper.h>
#include <widgets/qmt_table_edit_widget.h>
#include <json/qmt_json.h>
#include <itemviews/qmt_delegate_manager.h>
#include <itemviews/qmt_delegate_settings_model.h>
#include <itemviews/qmt_item_delegate.h>
#include <itemviews/qmt_string_delegate.h>
#include <core/qmt_itable.h>

#include <QApplication>
#include <QDebug>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QSplitter>
#include <QFileDialog>
#include <QJsonDocument>
#include <QCompleter>
#include <QStringListModel>
#include <QStyledItemDelegate>
#include <QItemSelectionModel>
#include <QDockWidget>
#include <QLabel>

struct Property
{
    QString type;
    QString name;
};

struct Column
{
    QString name;
    QString property;
    QString delegate;
};

struct View
{
    QString name;
    std::vector<Column> columns;
};

struct Type
{
    QString name;
    std::vector<Property> properties;
    std::vector<View> views;
};

struct Project
{
    std::vector<Type> types;
};

QMT_DECLARE_METATYPE(Column)
QMT_DECLARE_METATYPE(View)
QMT_DECLARE_METATYPE(Property)
QMT_DECLARE_METATYPE(Type)
QMT_DECLARE_METATYPE(Project)

inline Qmt::StaticTypeSP qmtMetaColumn()
{
    return std::make_shared<Qmt::StaticType>(
                qMetaTypeId<Column>(),
                Qmt::StaticType::PropertyHash
    {
        QMT_PROPERTY_MEMBER(Column, name),
        QMT_PROPERTY_MEMBER(Column, property),
        QMT_PROPERTY_MEMBER(Column, delegate)
    });
}

inline Qmt::StaticTypeSP qmtMetaView()
{
    return std::make_shared<Qmt::StaticType>(
                qMetaTypeId<View>(),
                Qmt::StaticType::PropertyHash
    {
        QMT_PROPERTY_MEMBER(View, name),
        QMT_PROPERTY_MEMBER(View, columns)
    });
}

inline Qmt::StaticTypeSP qmtMetaProperty()
{
    return std::make_shared<Qmt::StaticType>(
                qMetaTypeId<Property>(),
                Qmt::StaticType::PropertyHash
    {
        QMT_PROPERTY_MEMBER(Property, type),
        QMT_PROPERTY_MEMBER(Property, name)
    });
}

inline Qmt::StaticTypeSP qmtMetaType()
{
    return std::make_shared<Qmt::StaticType>(
                qMetaTypeId<Type>(),
                Qmt::StaticType::PropertyHash
    {
        QMT_PROPERTY_MEMBER(Type, name),
        QMT_PROPERTY_MEMBER(Type, properties),
        QMT_PROPERTY_MEMBER(Type, views)
    });
}

inline Qmt::StaticTypeSP qmtMetaProject()
{
    return std::make_shared<Qmt::StaticType>(
                qMetaTypeId<Project>(),
                Qmt::StaticType::PropertyHash
    {
        QMT_PROPERTY_MEMBER(Project, types)
    });
}

template <typename CollectionType>
void setCollection(Qmt::TableEditWidget& widget, CollectionType& collection)
{
    widget.setTable(std::make_shared<Qmt::TableWrapper<CollectionType>>(collection));
}

class TypeEditor : public QMainWindow
{
public:
    using ProjectSP = std::shared_ptr<Project>;

    TypeEditor() : QMainWindow()
        , _splitter(new QSplitter(Qt::Horizontal))
        , _columnEditWidget(new Qmt::TableEditWidget())
        , _viewEditWidget(new Qmt::TableEditWidget())
        , _typeEditWidget(new Qmt::TableEditWidget())
        , _projectEditWidget(new Qmt::TableEditWidget())
    {
        auto paths = QIcon::themeSearchPaths();
        paths.push_back(QDir::currentPath() + "/icons");
        QIcon::setThemeSearchPaths(paths);
        QIcon::setThemeName("Flatery");

        QMT_REGISTER_F(Column, qmtMetaColumn());
        QMT_REGISTER_F(View, qmtMetaView());
        QMT_REGISTER_F(Property, qmtMetaProperty());
        QMT_REGISTER_F(Type, qmtMetaType());
        QMT_REGISTER_F(Project, qmtMetaProject());

        setCentralWidget(_projectEditWidget);
        addDock(_typeEditWidget, tr("Type properties"), Qt::LeftDockWidgetArea);
        addDock(_viewEditWidget, tr("Views"), Qt::RightDockWidgetArea);
        addDock(_columnEditWidget, tr("View columns"), Qt::RightDockWidgetArea);

        connect(_projectEditWidget->selectionModel(),
                &QItemSelectionModel::selectionChanged, [this](const QItemSelection& selected,
                                                               const QItemSelection&)
        {
            const auto indexes = selected.indexes();
            if (indexes.empty())
            {
                _typeEditWidget->setTable(nullptr);
                _viewEditWidget->setTable(nullptr);
                return;
            }

            _currentType = indexes.first().row();
            auto& type = _project->types[_currentType];
            setCollection(*_typeEditWidget, type.properties);
            setCollection(*_viewEditWidget, type.views);
        });
        connect(_viewEditWidget->selectionModel(),
                &QItemSelectionModel::selectionChanged, [this](const QItemSelection& selected,
                                                               const QItemSelection&)
        {
            const auto indexes = selected.indexes();
            if (indexes.empty())
            {
                _columnEditWidget->setTable(nullptr);
                return;
            }

            const auto row = indexes.first().row();
            auto& type = _project->types[_currentType];
            auto& view = type.views[row];
            setCollection(*_columnEditWidget, view.columns);
        });

        createMenu();
        configureEdit();
        openProjectFromFile("tasks.qmt");
    }
    void setProject(ProjectSP project)
    {
        _project = project;
        if (_project)
            setCollection(*_projectEditWidget, project->types);
        else
            _projectEditWidget->setTable(nullptr);
        _typeEditWidget->setTable(nullptr);
    }
private:
    void openProjectFromFile(const QString& fileName)
    {
        QFile file(fileName);
        if (!file.open(QFile::ReadOnly))
            return;
        auto jsonObject = QJsonDocument::fromJson(file.readAll()).object();
        file.close();

        auto project = std::make_shared<Project>();
        Qmt::fromJsonValue(*project.get(), jsonObject);
        setProject(project);
    }
    Qmt::ItemDelegate* createProjectDelegate()
    {
        auto nameSettings = std::make_shared<Qmt::StringDelegateSettings>();
        nameSettings->placeholderText = "name";

        auto settingsModel = std::make_shared<Qmt::DelegateSettingsModel>();
        settingsModel->setSettings("name", nameSettings);

        return new Qmt::ItemDelegate(Qmt::DelegateManager::defaultInstance(), settingsModel);
    }
    Qmt::ItemDelegate* createTypeDelegate()
    {
        auto typeSettings = std::make_shared<Qmt::StringDelegateSettings>();
        typeSettings->placeholderText = "type";
        typeSettings->completer = createTypeCompleter();

        auto nameSettings = std::make_shared<Qmt::StringDelegateSettings>();
        nameSettings->placeholderText = "name";
        nameSettings->completer = createTypeCompleter();

        auto settingsModel = std::make_shared<Qmt::DelegateSettingsModel>();
        settingsModel->setSettings("name", nameSettings);
        settingsModel->setSettings("type", typeSettings);

        return new Qmt::ItemDelegate(Qmt::DelegateManager::defaultInstance(), settingsModel);
    }
    Qmt::ItemDelegate* createViewDelegate()
    {
        auto propertySettings = std::make_shared<Qmt::StringDelegateSettings>();
        propertySettings->placeholderText = "property name";

        auto nameSettings = std::make_shared<Qmt::StringDelegateSettings>();
        nameSettings->placeholderText = "column name";

        auto settingsModel = std::make_shared<Qmt::DelegateSettingsModel>();
        settingsModel->setSettings("property", propertySettings);
        settingsModel->setSettings("name", nameSettings);

        return new Qmt::ItemDelegate(Qmt::DelegateManager::defaultInstance(), settingsModel);
    }
    void configureEdit()
    {
        _projectEditWidget->setProperties({{"name", "Name"}});
        _projectEditWidget->setItemDelegate(createProjectDelegate());

        _typeEditWidget->setProperties({{"name", "Name"}, {"type", "Type"}});
        _typeEditWidget->setItemDelegate(createTypeDelegate());

        _viewEditWidget->setProperties({{"name", "Name"}});
        _viewEditWidget->setItemDelegate(createTypeDelegate());

        _columnEditWidget->setProperties({{"property", "Property"}, {"name", "Name"}});
        _columnEditWidget->setItemDelegate(createViewDelegate());
    }

    QCompleter* createTypeCompleter()
    {
        QStringList typeNames;
        int typeId = 1;
        while (1)
        {
            const QString typeName = QMetaType::typeName(typeId++);
            if (typeName.isEmpty())
            {
                if (typeId >= QMetaType::User)
                    break;
                typeId = QMetaType::User;
                continue;
            }
            typeNames.push_back(typeName);
        }

        QStringListModel* typeModel = new QStringListModel();
        typeModel->setStringList(typeNames);
        QCompleter* completer = new QCompleter(typeModel);
        return completer;
    }
    void createMenu()
    {
        QAction* newAction = new QAction(tr("New"));
        newAction->setIcon(QIcon::fromTheme("document-new"));

        connect(newAction, &QAction::triggered, this, [this]()
        {
            auto project = std::make_shared<Project>();
            project->types.push_back(Type{tr("NewType"), {{tr("QString"), tr("name")}}, {}});
            setProject(project);
        });
        QAction* openAction = new QAction(tr("Open"));
        openAction->setIcon(QIcon::fromTheme("document-open"));
        connect(openAction, &QAction::triggered, this, [this]()
        {
            const QString fileName = QFileDialog::getOpenFileName(this,
                                                                  tr("Meta data"),
                                                                  QString(),
                                                                  tr("Meta data (.qmt)"));
            if (fileName.isEmpty())
                return;
            openProjectFromFile(fileName);
        });
        QAction* saveAction = new QAction(tr("Save"));
        saveAction->setIcon(QIcon::fromTheme("document-save"));
        connect(saveAction, &QAction::triggered, this, [this]()
        {
            const QString fileName = QFileDialog::getSaveFileName(this,
                                                                  tr("Meta data"),
                                                                  QString(),
                                                                  tr("Meta data (.qmt)"));

            if (fileName.isEmpty())
                return;

            QJsonDocument document;
            QJsonObject object = Qmt::toJsonValue(*_project.get()).toObject();
            document.setObject(object);

            QFile file(fileName);
            if (!file.open(QFile::WriteOnly))
                return;
            file.write(document.toJson());
            file.close();
        });
        QAction* closeAction = new QAction(tr("Close"));
        closeAction->setIcon(QIcon::fromTheme("document-close"));
        connect(closeAction, &QAction::triggered, this, [this]()
        {
            setProject(nullptr);
        });
        QAction* quitAction = new QAction(tr("Quit"));
        quitAction->setIcon(QIcon::fromTheme("application-exit"));
        connect(quitAction, &QAction::triggered, this, []()
        {
            qApp->quit();
        });

        QMenu* fileMenu = new QMenu(tr("File"));
        fileMenu->addAction(newAction);
        fileMenu->addAction(openAction);
        fileMenu->addAction(saveAction);
        fileMenu->addAction(closeAction);
        fileMenu->addSeparator();
        fileMenu->addAction(quitAction);

        menuBar()->addMenu(fileMenu);
    }

    void addDock(QWidget* widget, const QString& title, Qt::DockWidgetArea area = Qt::LeftDockWidgetArea)
    {
        QDockWidget* dock = new QDockWidget(title);
        dock->setWidget(widget);
        addDockWidget(area, dock);
    }

    ProjectSP _project;
    QSplitter* _splitter;
    Qmt::TableEditWidget* _columnEditWidget;
    Qmt::TableEditWidget* _viewEditWidget;
    Qmt::TableEditWidget* _typeEditWidget;
    Qmt::TableEditWidget* _projectEditWidget;
    int _currentType = -1;
};

