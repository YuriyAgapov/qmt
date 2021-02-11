#pragma once

#include <QAbstractItemModel>

#include <memory>

namespace Qmt
{
    class IDecorationModel;
    class IEntity;
    class IType;

    class EntityModel : public QAbstractItemModel
    {
    public:
        using IEntitySP = std::shared_ptr<IEntity>;
        using IDecorationModelSP = std::shared_ptr<IDecorationModel>;
        using NameVector = std::vector<std::pair<QString, QString>>;

        explicit EntityModel(QObject* parent = nullptr);
        virtual ~EntityModel();

        Qt::ItemFlags flags(const QModelIndex &index) const override;

        const NameVector& properties() const;
        void setProperties(const QStringList& properties);
        void setProperties(const NameVector& properties);

        QString propertyName(const int index) const;
        QString propertyDisplayName(const int index) const;
        virtual QString propertyName(const QModelIndex& index) const;
        virtual int indexType(const QModelIndex& index) const;
        int propertyCount() const;
        virtual int propertyIndex(const QModelIndex& index) const = 0;
        int propertyIndex(const QString& name) const;

        IDecorationModelSP decorationModel() const;
        void setDecorationModel(IDecorationModelSP decorationModel);

    protected:
        void setEntity(IEntitySP entity);

        virtual bool toView(QVariant& value, const QModelIndex& modelIndex, int role) const;
        virtual bool fromView(QVariant& value, const QModelIndex& modelIndex, int role) const;

        void updateTypes();

    private:
        IEntitySP _entity;
        IDecorationModelSP _decorationModel;
        NameVector _properties;
        std::vector<int> _types;
    };
}//namespace Qmt

