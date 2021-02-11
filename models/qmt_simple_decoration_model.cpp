#include "qmt_simple_decoration_model.h"

#include "qmt_entity_model.h"

#include <QPainter>

namespace Qmt
{

    class StyleHelper
    {
    public:
        QPixmap pixmap(const QString& name)
        {
            const int hash = qHash(QStringRef(&name, 0, std::min(_hashStringSize, name.size())));
            QPixmap& pixmap = _iconsCache[hash];
            if (pixmap.isNull())
            {
                pixmap = createIcon(name.mid(0,1), getShape(hash), getColor(hash));
            }
            return pixmap;
        }
        static StyleHelper& instance()
        {
            static StyleHelper keeper;
            return keeper;
        }
    private:
        enum class Shape
        {
            Rect,
            Ellipse
        };
        inline Shape getShape(const int hash)
        {
            return hash & 0x1 ? Shape::Rect : Shape::Ellipse;
        }
        inline QColor getColor(const int hash)
        {
            return _colors.empty() ? QColor()
                                   : _colors[hash % _colors.size()];
        }
        inline QColor inverseColor(const QColor& color)
        {
            return QColor(255 - color.red(),
                          255 - color.green(),
                          255 - color.blue());
        }
        inline QPixmap createIcon(const QString& title, const Shape shape, const QColor& bgColor)
        {
            QPixmap pixmap(side, side);
            pixmap.fill(Qt::transparent);
            QPainter painter(&pixmap);
            //painter.setPen(QPen(inverseColor(bgColor), 0));
            painter.setPen(QPen(_contourColor));
            painter.setBrush(QBrush(bgColor));
            switch (shape)
            {
            case Shape::Rect:
                painter.drawRect(0, 0, side - 1, side - 1);
                break;
            case Shape::Ellipse:
                painter.setRenderHint(QPainter::Antialiasing, true);
                painter.drawEllipse(0, 0, side - 1, side - 1);
                break;
            }
            QFont font = painter.font();
            font.setPixelSize(side-2);
            painter.setPen(QPen(_titleColor));
            painter.drawText(QRect(1, 1, side - 2, side - 2), Qt::AlignCenter, title);
            return pixmap;
        }
        void makePallete(const QString& mid, const QString& dark)
        {
            const QString midName = "cc";
            const QString darkName = "99";
            const QStringList baseColors = {
                "#ffcccc",
                "#ffcc99",
                "#ffffcc",
                "#ccffcc",
                "#99ffcc",
                "#ccffff",
                "#99ccff",
                "#ccccff",
                "#ffccff"
            };
            for (auto color : baseColors)
            {
                _colors.push_back(QColor(color.replace(midName, mid)
                                              .replace(darkName, dark)));
            }
        }

        StyleHelper()
        {
            makePallete("66", "33");
        }

        QHash<int, QPixmap> _iconsCache;
        std::vector<QColor> _colors;
        const int _hashStringSize = 5;
        const int side = 16;
        QColor _contourColor{"#000000"};
        QColor _titleColor{"#000000"};
    };
    SimpleDecorationModel::SimpleDecorationModel()
    {
    }

    bool SimpleDecorationModel::toView(QVariant& value, const int role,
                                       const QModelIndex& index, const EntityModel& model) const
    {
        if (role == Qt::DecorationRole)
        {
            if (model.propertyIndex(index) == _iconIndex)
            {
                const QString name = model.data(index, Qt::DisplayRole).toString();
                value = StyleHelper::instance().pixmap(name);
                return true;
            }
        }
        return false;
    }
}
