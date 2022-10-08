QT       += core gui
QT       += sql
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Controller.cpp \
    logic.cpp \
    bloom.cpp \
    data_base.cpp \
    data_layer.cpp \
    main.cpp \
    windows/add_arrival_window.cpp \
    windows/add_client_window.cpp \
    windows/add_employee_window.cpp \
    windows/administrator_window.cpp \
    windows/authorization_window.cpp \
    windows/employee_window.cpp \
    windows/main_window.cpp \
    windows/add_book_window.cpp \
    windows/add_order_window.cpp \
    windows/order_window.cpp \
    windows/sales_three_months.cpp \
    windows/top_client.cpp \
    windows/top_selling_books.cpp \
    windows/top_selling_books_month.cpp

HEADERS += \
    logic.h \
    bloom.h \
    controller.h \
    data_base.h \
    data_layer.h \
    entities/models.h \
    windows/add_arrival_window.h \
    windows/add_client_window.h \
    windows/add_employee_window.h \
    windows/administrator_window.h \
    windows/authorization_window.h \
    windows/employee_window.h \
    windows/main_window.h \
    windows/add_book_window.h \
    windows/add_order_window.h \
    windows/order_window.h \
    windows/sales_three_months.h \
    windows/top_client.h \
    windows/top_selling_books.h \
    windows/top_selling_books_month.h

FORMS += \
    add_arrival_window.ui \
    add_book_window.ui \
    add_client_window.ui \
    add_employee_window.ui \
    add_order_window.ui \
    administrator_window.ui \
    authorization_window.ui \
    employee_window.ui \
    main_window.ui \
    order_window.ui \
    sales_three_months.ui \
    top_client.ui \
    top_selling_books.ui \
    top_selling_books_month.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
