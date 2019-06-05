/********************************************************************************
** Form generated from reading UI file 'launcher.ui'
**
** Created by: Qt User Interface Compiler version 5.11.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LAUNCHER_H
#define UI_LAUNCHER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Launcher
{
public:
    QWidget *centralWidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *title;
    QLabel *launchertext;
    QComboBox *modeselect;
    QPushButton *startbutton;
    QProgressBar *progressbar;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Launcher)
    {
        if (Launcher->objectName().isEmpty())
            Launcher->setObjectName(QStringLiteral("Launcher"));
        Launcher->resize(352, 301);
        centralWidget = new QWidget(Launcher);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(0, 0, 351, 231));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        title = new QLabel(verticalLayoutWidget);
        title->setObjectName(QStringLiteral("title"));
        title->setStyleSheet(QLatin1String("font: 87 28pt \"Verdana Pro Black\";\n"
"color: rgb(85, 170, 0);"));

        verticalLayout->addWidget(title);

        launchertext = new QLabel(verticalLayoutWidget);
        launchertext->setObjectName(QStringLiteral("launchertext"));

        verticalLayout->addWidget(launchertext);

        modeselect = new QComboBox(verticalLayoutWidget);
        modeselect->addItem(QString());
        modeselect->addItem(QString());
        modeselect->setObjectName(QStringLiteral("modeselect"));

        verticalLayout->addWidget(modeselect);

        startbutton = new QPushButton(verticalLayoutWidget);
        startbutton->setObjectName(QStringLiteral("startbutton"));

        verticalLayout->addWidget(startbutton);

        progressbar = new QProgressBar(verticalLayoutWidget);
        progressbar->setObjectName(QStringLiteral("progressbar"));
        progressbar->setValue(24);

        verticalLayout->addWidget(progressbar);

        Launcher->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Launcher);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 352, 26));
        Launcher->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Launcher);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        Launcher->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Launcher);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Launcher->setStatusBar(statusBar);

        retranslateUi(Launcher);

        QMetaObject::connectSlotsByName(Launcher);
    } // setupUi

    void retranslateUi(QMainWindow *Launcher)
    {
        Launcher->setWindowTitle(QApplication::translate("Launcher", "Launcher", nullptr));
        title->setText(QApplication::translate("Launcher", "NATURE ZEN", nullptr));
        launchertext->setText(QApplication::translate("Launcher", "Launcher", nullptr));
        modeselect->setItemText(0, QApplication::translate("Launcher", "Windowed", nullptr));
        modeselect->setItemText(1, QApplication::translate("Launcher", "Fullscreen", nullptr));

        startbutton->setText(QApplication::translate("Launcher", "START", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Launcher: public Ui_Launcher {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LAUNCHER_H
