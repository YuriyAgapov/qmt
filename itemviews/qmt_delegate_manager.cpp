#include "qmt_datetime_delegate.h"
#include "qmt_delegate_manager.h"
#include "qmt_number_delegate.h"
#include "qmt_string_delegate.h"

namespace Qmt
{
    DelegateManager::DelegateManager()
    {
    }

    bool DelegateManager::paint(QPainter* painter,
                                       const QStyleOptionViewItem& option,
                                       DelegateSettingsCSP settings,
                                       const QVariant& data) const
    {
        auto iter = _displays.find(settings->viewType);
        if (iter == _displays.end())
            return false;
        if (iter.value())
            iter.value()->paint(painter, option, settings, data);
        return true;
    }

    QString DelegateManager::displayText(const QStyleOptionViewItem& option,
                                         DelegateSettingsCSP settings,
                                         const QVariant& data) const
    {
        auto iter = _displays.find(settings->viewType);
        if (iter == _displays.end())
            return data.toString();

        return iter.value() ? iter.value()->displayText(option, settings, data)
                            : data.toString();
    }

    QWidget* DelegateManager::createEditor(QWidget* parent,
                                           const QStyleOptionViewItem& option,
                                           DelegateSettingsCSP settings,
                                           const QVariant& data) const
    {
        auto iter = _editors.find(settings->editorType);
        if (iter == _editors.end())
            return nullptr;

        return iter.value() ? iter.value()->createEditor(parent, option, settings, data)
                            : nullptr;
    }

    void DelegateManager::setEditorData(QWidget* editor, DelegateSettingsCSP settings, const QVariant& data) const
    {
        auto iter = _editors.find(settings->editorType);
        if (iter == _editors.end())
            return;

        iter.value()->setEditorData(editor, settings, data);
    }

    QVariant DelegateManager::editorData(QWidget* editor, DelegateSettingsCSP settings) const
    {
        auto iter = _editors.find(settings->editorType);
        if (iter == _editors.end())
            return QVariant();

        return iter.value() ? iter.value()->editorData(editor, settings)
                            : QVariant();
    }

    void DelegateManager::addDisplay(const QString& viewType, DelegateDisplaySP display)
    {
        _displays[viewType] = display;
    }

    void DelegateManager::addEditor(const QString& editorType, DelegateEditorSP editor)
    {
        _editors[editorType] = editor;
    }

    std::shared_ptr<DelegateManager> DelegateManager::defaultInstance()
    {
        static std::weak_ptr<DelegateManager> delegateManagerWeak;
        std::shared_ptr<DelegateManager> delegateManager = delegateManagerWeak.lock();
        if (!delegateManager)
        {
            delegateManager = std::make_shared<DelegateManager>();
            delegateManager->addDisplay(StringDelegateSettings::typeName(), std::make_shared<StringDelegateDisplay>());
            delegateManager->addEditor(StringDelegateSettings::typeName(), std::make_shared<StringDelegateEditor>());
            delegateManager->addDisplay(NumberDelegateSettings::typeName(), std::make_shared<NumberDelegateDisplay>());
            delegateManager->addEditor(NumberDelegateSettings::typeName(), std::make_shared<NumberDelegateEditor>());
            delegateManager->addDisplay(DateTimeDelegateSettings::typeName(), std::make_shared<DateTimeDelegateDisplay>());
            delegateManager->addEditor(DateTimeDelegateSettings::typeName(), std::make_shared<DateTimeDelegateEditor>());
            delegateManagerWeak = delegateManager;
        }
        return delegateManager;
    }
}//namespace Qmt
