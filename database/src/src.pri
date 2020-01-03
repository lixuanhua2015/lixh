INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

include($$PWD/databasemanager/databasemanager.pri)
include($$PWD/IoTObject/IoTObject.pri)
include($$PWD/qextserialport-1.2beta1/qextserialport.pri)
include($$PWD/3GDial/dial3g.pri)

HEADERS += \
    $$PWD/myArithmetic.h \
    $$PWD/parambase.h

