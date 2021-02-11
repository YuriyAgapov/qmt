QT += core gui widgets

CONFIG += c++14

HEADERS += \
    core/qmt_declare_ptr.h \
    core/qmt_ientity.h \
    core/qmt_ientity_listener.h \
    core/qmt_iobject.h \
    core/qmt_iobject_listener.h \
    core/qmt_iproperty.h \
    core/qmt_itable.h \
    core/qmt_itable_listener.h \
    core/qmt_itype.h \
    core/qmt_notifier.h \
    core/qmt_ptr.h \
    core/qmt_type_storage.h \
    itemviews/qmt_datetime_delegate.h \
    itemviews/qmt_delegate_display.h \
    itemviews/qmt_delegate_editor.h \
    itemviews/qmt_delegate_manager.h \
    itemviews/qmt_delegate_settings.h \
    itemviews/qmt_delegate_settings_model.h \
    itemviews/qmt_idelegate_settings_model.h \
    itemviews/qmt_item_delegate.h \
    itemviews/qmt_number_delegate.h \
    itemviews/qmt_string_delegate.h \
    json/qmt_json.h \
    json/qmt_json_codec_type.h \
    json/qmt_json_decode.h \
    json/qmt_json_encode.h \
    json/qmt_string_tools.h \
    mem/qmt_mem_entity.h \
    mem/qmt_mem_type.h \
    models/qmt_entity_model.h \
    models/qmt_idecoration_model.h \
    models/qmt_model_settings.h \
    models/qmt_object_model.h \
    models/qmt_simple_decoration_model.h \
    models/qmt_table_model.h \
    qmt.h \
    testbed/charts_testbed.h \
    testbed/delegate_testbed.h \
    testbed/json_testbed.h \
    testbed/type_editor.h \
    type_build/qmt_debug.h \
    type_build/qmt_iproperty_wrapper.h \
    type_build/qmt_macro.h \
    type_build/qmt_member_wrapper.h \
    type_build/qmt_method_wrapper.h \
    type_build/qmt_object_wrapper.h \
    type_build/qmt_static_type.h \
    type_build/qmt_table_wrapper.h \
    type_build/qmt_type_build.h \
    type_build/qmt_type_build_ptr.h \
    type_build/qmt_value_wrapper.h \
    widgets/qmt_table_edit_widget.h

SOURCES += \
    core/qmt_type_storage.cpp \
    itemviews/qmt_datetime_delegate.cpp \
    itemviews/qmt_delegate_display.cpp \
    itemviews/qmt_delegate_manager.cpp \
    itemviews/qmt_delegate_settings_model.cpp \
    itemviews/qmt_item_delegate.cpp \
    itemviews/qmt_number_delegate.cpp \
    itemviews/qmt_string_delegate.cpp \
    json/qmt_json_decode.cpp \
    json/qmt_json_encode.cpp \
    mem/qmt_mem_entity.cpp \
    mem/qmt_mem_type.cpp \
    models/qmt_entity_model.cpp \
    models/qmt_model_settings.cpp \
    models/qmt_object_model.cpp \
    models/qmt_simple_decoration_model.cpp \
    models/qmt_table_model.cpp \
    type_build/qmt_static_type.cpp \
    widgets/qmt_table_edit_widget.cpp

INCLUDEPATH = $$PWD

HEADERS += \

SOURCES += \
    testbed/main.cpp \

win32 {
    QMAKE_EXTRA_TARGETS += before_build makefilehook

    makefilehook.target = $(MAKEFILE)
    makefilehook.depends = .beforebuild

    PRE_TARGETDEPS += .beforebuild

    before_build.target = .beforebuild
    before_build.depends = FORCE
    before_build.commands = chcp 1251
}
