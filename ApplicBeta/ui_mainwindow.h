/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QToolBox *toolBox;
    QWidget *page;
    QLabel *setComPort;
    QComboBox *comboBoxCom;
    QLabel *setBaudRate;
    QComboBox *comboBoxBaudRate;
    QLabel *setStopBits;
    QComboBox *comboBoxStopBits;
    QComboBox *comboBoxParity;
    QLabel *setParityBits;
    QPushButton *pushButton_4;
    QComboBox *comboBoxFlowControl;
    QLabel *setFlowControl;
    QComboBox *comboBoxDataBits;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QLabel *setDataBits;
    QWidget *page_2;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPlainTextEdit *plainTextEdit;
    QLineEdit *lineEdit;
    QWidget *page_3;
    QLabel *label;
    QLabel *label_2;
    QProgressBar *progressBar;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(450, 684);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(450, 684));
        MainWindow->setMaximumSize(QSize(450, 16777215));
        MainWindow->setStyleSheet(QStringLiteral("background-color:rgb(34, 32, 34)"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        toolBox = new QToolBox(centralWidget);
        toolBox->setObjectName(QStringLiteral("toolBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(toolBox->sizePolicy().hasHeightForWidth());
        toolBox->setSizePolicy(sizePolicy1);
        toolBox->setLayoutDirection(Qt::LeftToRight);
        toolBox->setStyleSheet(QLatin1String("background-color:rgb(49, 47, 49);\n"
"color:white;\n"
"font: 14pt \"Liberation Serif\";\n"
""));
        toolBox->setFrameShape(QFrame::NoFrame);
        toolBox->setFrameShadow(QFrame::Plain);
        toolBox->setMidLineWidth(42);
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        page->setGeometry(QRect(0, 0, 432, 517));
        setComPort = new QLabel(page);
        setComPort->setObjectName(QStringLiteral("setComPort"));
        setComPort->setGeometry(QRect(10, 40, 141, 16));
        QFont font;
        font.setFamily(QStringLiteral("Liberation Serif"));
        font.setPointSize(9);
        font.setBold(false);
        font.setItalic(false);
        font.setWeight(50);
        setComPort->setFont(font);
        setComPort->setStyleSheet(QStringLiteral("font: 9pt \"Liberation Serif\";"));
        comboBoxCom = new QComboBox(page);
        comboBoxCom->setObjectName(QStringLiteral("comboBoxCom"));
        comboBoxCom->setGeometry(QRect(10, 60, 191, 41));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(comboBoxCom->sizePolicy().hasHeightForWidth());
        comboBoxCom->setSizePolicy(sizePolicy2);
        comboBoxCom->setStyleSheet(QStringLiteral("font: 13pt \"Liberation Serif\";"));
        setBaudRate = new QLabel(page);
        setBaudRate->setObjectName(QStringLiteral("setBaudRate"));
        setBaudRate->setGeometry(QRect(10, 120, 111, 16));
        setBaudRate->setStyleSheet(QStringLiteral("font: 9pt \"Liberation Serif\";"));
        comboBoxBaudRate = new QComboBox(page);
        comboBoxBaudRate->setObjectName(QStringLiteral("comboBoxBaudRate"));
        comboBoxBaudRate->setGeometry(QRect(10, 140, 191, 41));
        comboBoxBaudRate->setStyleSheet(QStringLiteral("font: 13pt \"Liberation Serif\";"));
        setStopBits = new QLabel(page);
        setStopBits->setObjectName(QStringLiteral("setStopBits"));
        setStopBits->setGeometry(QRect(10, 260, 111, 16));
        setStopBits->setStyleSheet(QStringLiteral("font: 9pt \"Liberation Serif\";"));
        comboBoxStopBits = new QComboBox(page);
        comboBoxStopBits->setObjectName(QStringLiteral("comboBoxStopBits"));
        comboBoxStopBits->setGeometry(QRect(10, 280, 191, 41));
        comboBoxStopBits->setStyleSheet(QStringLiteral("font: 13pt \"Liberation Serif\";"));
        comboBoxParity = new QComboBox(page);
        comboBoxParity->setObjectName(QStringLiteral("comboBoxParity"));
        comboBoxParity->setGeometry(QRect(10, 210, 191, 41));
        comboBoxParity->setStyleSheet(QStringLiteral("font: 13pt \"Liberation Serif\";"));
        setParityBits = new QLabel(page);
        setParityBits->setObjectName(QStringLiteral("setParityBits"));
        setParityBits->setGeometry(QRect(10, 190, 101, 16));
        setParityBits->setStyleSheet(QStringLiteral("font: 9pt \"Liberation Serif\";"));
        pushButton_4 = new QPushButton(page);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(230, 60, 191, 41));
        pushButton_4->setStyleSheet(QStringLiteral("font: 13pt \"Liberation Serif\";"));
        comboBoxFlowControl = new QComboBox(page);
        comboBoxFlowControl->setObjectName(QStringLiteral("comboBoxFlowControl"));
        comboBoxFlowControl->setGeometry(QRect(10, 360, 191, 41));
        comboBoxFlowControl->setStyleSheet(QStringLiteral("font: 13pt \"Liberation Serif\";"));
        setFlowControl = new QLabel(page);
        setFlowControl->setObjectName(QStringLiteral("setFlowControl"));
        setFlowControl->setGeometry(QRect(10, 340, 181, 16));
        setFlowControl->setStyleSheet(QStringLiteral("font: 9pt \"Liberation Serif\";"));
        comboBoxDataBits = new QComboBox(page);
        comboBoxDataBits->setObjectName(QStringLiteral("comboBoxDataBits"));
        comboBoxDataBits->setGeometry(QRect(10, 430, 191, 41));
        comboBoxDataBits->setStyleSheet(QStringLiteral("font: 13pt \"Liberation Serif\";"));
        pushButton_5 = new QPushButton(page);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setGeometry(QRect(230, 140, 191, 41));
        pushButton_5->setStyleSheet(QLatin1String("background-color:rgb(49, 47, 49);\n"
"font: 13pt \"Liberation Serif\";"));
        pushButton_6 = new QPushButton(page);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));
        pushButton_6->setGeometry(QRect(230, 430, 191, 41));
        pushButton_6->setStyleSheet(QLatin1String("background-color:rgb(89, 89, 89);\n"
"font: 13pt \"Liberation Serif\";"));
        setDataBits = new QLabel(page);
        setDataBits->setObjectName(QStringLiteral("setDataBits"));
        setDataBits->setGeometry(QRect(10, 410, 181, 16));
        setDataBits->setStyleSheet(QStringLiteral("font: 9pt \"Liberation Serif\";"));
        toolBox->addItem(page, QString::fromUtf8("                             \320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\260 COM-\320\277\320\276\321\200\321\202\320\260"));
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        page_2->setGeometry(QRect(0, 0, 432, 517));
        pushButton = new QPushButton(page_2);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(10, 10, 201, 41));
        QFont font1;
        font1.setFamily(QStringLiteral("Liberation Serif"));
        font1.setPointSize(13);
        font1.setBold(false);
        font1.setItalic(false);
        font1.setWeight(50);
        pushButton->setFont(font1);
        pushButton->setStyleSheet(QLatin1String("background-color:rgb(89, 89, 89);\n"
"font: 13pt \"Liberation Serif\";"));
        pushButton_2 = new QPushButton(page_2);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(10, 70, 201, 41));
        pushButton_2->setStyleSheet(QLatin1String("background-color:rgb(89, 89, 89);\n"
"font: 13pt \"Liberation Serif\";"));
        pushButton_3 = new QPushButton(page_2);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(10, 130, 201, 41));
        pushButton_3->setStyleSheet(QLatin1String("background-color:rgb(89, 89, 89);\n"
"font: 13pt \"Liberation Serif\";"));
        plainTextEdit = new QPlainTextEdit(page_2);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(10, 270, 411, 251));
        lineEdit = new QLineEdit(page_2);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(10, 190, 411, 41));
        lineEdit->setStyleSheet(QStringLiteral("background-color:rgb(0, 0, 0)"));
        toolBox->addItem(page_2, QString::fromUtf8("                             \320\232\320\273\321\216\321\207\320\270 \320\270 \320\250\320\270\321\204\321\200\320\276\320\262\320\260\320\275\320\270\320\265"));
        page_3 = new QWidget();
        page_3->setObjectName(QStringLiteral("page_3"));
        page_3->setGeometry(QRect(0, 0, 432, 517));
        label = new QLabel(page_3);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 10, 411, 21));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy3);
        label->setFont(font1);
        label->setStyleSheet(QStringLiteral("font: 13pt \"Liberation Serif\";"));
        label_2 = new QLabel(page_3);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 70, 211, 18));
        label_2->setStyleSheet(QStringLiteral("font: 13pt \"Liberation Serif\";"));
        progressBar = new QProgressBar(page_3);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setGeometry(QRect(10, 100, 181, 23));
        progressBar->setValue(24);
        toolBox->addItem(page_3, QString::fromUtf8("                             \320\237\320\260\321\200\320\260\320\274\320\265\321\202\321\200\321\213 \320\263\320\260\321\200\320\275\320\270\321\202\321\203\321\200\321\213"));

        verticalLayout->addWidget(toolBox);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 450, 22));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        toolBox->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "HFTA", nullptr));
        setComPort->setText(QApplication::translate("MainWindow", "\320\222\321\213\320\261\320\276\321\200 COM-\320\277\320\276\321\200\321\202\320\260", nullptr));
        setBaudRate->setText(QApplication::translate("MainWindow", "\320\222\321\213\320\261\320\276\321\200 \321\201\320\272\320\276\321\200\320\276\321\201\321\202\320\270", nullptr));
        setStopBits->setText(QApplication::translate("MainWindow", "\320\241\321\202\320\276\320\277-\320\261\320\270\321\202", nullptr));
        setParityBits->setText(QApplication::translate("MainWindow", "\320\221\320\270\321\202 \321\207\320\265\321\202\320\275\320\276\321\201\321\202\320\270", nullptr));
        pushButton_4->setText(QApplication::translate("MainWindow", "\320\236\320\261\320\275\320\276\320\262\320\270\321\202\321\214", nullptr));
        setFlowControl->setText(QApplication::translate("MainWindow", "\320\232\320\276\320\275\321\202\321\200\320\276\320\273\321\214 \320\277\320\276\321\202\320\276\320\272\320\260", nullptr));
        pushButton_5->setText(QApplication::translate("MainWindow", "\320\237\320\276\320\264\320\272\320\273\321\216\321\207\320\270\321\202\321\214 \320\277\320\276\321\200\321\202", nullptr));
        pushButton_6->setText(QApplication::translate("MainWindow", "\320\236\321\202\320\272\320\273\321\216\321\207\320\270\321\202\321\214 \320\277\320\276\321\200\321\202", nullptr));
        setDataBits->setText(QApplication::translate("MainWindow", "\320\224\320\273\320\270\320\275\320\260 \320\277\320\276\321\202\320\276\320\272\320\260", nullptr));
        toolBox->setItemText(toolBox->indexOf(page), QApplication::translate("MainWindow", "                             \320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\260 COM-\320\277\320\276\321\200\321\202\320\260", nullptr));
        pushButton->setText(QApplication::translate("MainWindow", "\320\241\320\223\320\225\320\235\320\225\320\240\320\230\320\240\320\236\320\222\320\220\320\242\320\254 \320\232\320\233\320\256\320\247", nullptr));
        pushButton_2->setText(QApplication::translate("MainWindow", "\320\227\320\220\320\223\320\240\320\243\320\227\320\230\320\242\320\254 \320\232\320\233\320\256\320\247", nullptr));
        pushButton_3->setText(QApplication::translate("MainWindow", "\320\237\320\240\320\236\320\247\320\230\320\242\320\220\320\242\320\254 \320\232\320\233\320\256\320\247", nullptr));
        toolBox->setItemText(toolBox->indexOf(page_2), QApplication::translate("MainWindow", "                             \320\232\320\273\321\216\321\207\320\270 \320\270 \320\250\320\270\321\204\321\200\320\276\320\262\320\260\320\275\320\270\320\265", nullptr));
        label->setText(QApplication::translate("MainWindow", "MAC \320\260\320\264\321\200\320\265\321\201 \320\263\320\260\321\200\320\275\320\270\321\202\321\203\321\200\321\213:", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "\320\243\321\200\320\276\320\262\320\265\320\275\321\214 \320\267\320\260\321\200\321\217\320\264\320\260 \320\261\320\260\321\202\320\260\321\200\320\265\320\270", nullptr));
        toolBox->setItemText(toolBox->indexOf(page_3), QApplication::translate("MainWindow", "                             \320\237\320\260\321\200\320\260\320\274\320\265\321\202\321\200\321\213 \320\263\320\260\321\200\320\275\320\270\321\202\321\203\321\200\321\213", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
