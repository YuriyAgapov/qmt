#include "json_testbed.h"
#include "delegate_testbed.h"
#include "type_editor.h"

#include <QApplication>
#include <QDebug>

#include <memory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //jsonTestbed();

    TypeEditor editor;
    editor.show();

    //DelegateTestbed delegate;

    return a.exec();
}

