/********************************************************************************
** Form generated from reading UI file 'gpiocontrol.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GPIOCONTROL_H
#define UI_GPIOCONTROL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GpioControl
{
public:
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton_3;
    QPushButton *pushButton_2;
    QPushButton *pushButton;

    void setupUi(QScrollArea *GpioControl)
    {
        if (GpioControl->objectName().isEmpty())
            GpioControl->setObjectName(QStringLiteral("GpioControl"));
        GpioControl->setWindowModality(Qt::NonModal);
        GpioControl->setEnabled(true);
        GpioControl->resize(1300, 757);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(GpioControl->sizePolicy().hasHeightForWidth());
        GpioControl->setSizePolicy(sizePolicy);
        GpioControl->setMinimumSize(QSize(1250, 0));
        GpioControl->setAutoFillBackground(true);
        GpioControl->setFrameShape(QFrame::StyledPanel);
        GpioControl->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        GpioControl->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        GpioControl->setWidgetResizable(true);
        GpioControl->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 1298, 755));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(scrollAreaWidgetContents->sizePolicy().hasHeightForWidth());
        scrollAreaWidgetContents->setSizePolicy(sizePolicy1);
        gridLayout_3 = new QGridLayout(scrollAreaWidgetContents);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        pushButton_3 = new QPushButton(scrollAreaWidgetContents);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pushButton_3->sizePolicy().hasHeightForWidth());
        pushButton_3->setSizePolicy(sizePolicy2);

        verticalLayout->addWidget(pushButton_3);


        horizontalLayout->addLayout(verticalLayout);

        pushButton_2 = new QPushButton(scrollAreaWidgetContents);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        sizePolicy2.setHeightForWidth(pushButton_2->sizePolicy().hasHeightForWidth());
        pushButton_2->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(pushButton_2);

        pushButton = new QPushButton(scrollAreaWidgetContents);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        sizePolicy2.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(pushButton);


        gridLayout_3->addLayout(horizontalLayout, 0, 0, 1, 1);

        GpioControl->setWidget(scrollAreaWidgetContents);

        retranslateUi(GpioControl);

        QMetaObject::connectSlotsByName(GpioControl);
    } // setupUi

    void retranslateUi(QScrollArea *GpioControl)
    {
        GpioControl->setWindowTitle(QApplication::translate("GpioControl", "Upgrader", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("GpioControl", "USER_LED_0", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("GpioControl", "USER_LED_1", Q_NULLPTR));
        pushButton->setText(QApplication::translate("GpioControl", "USER_LED_2", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class GpioControl: public Ui_GpioControl {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GPIOCONTROL_H
