#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>
#include <QColorDialog>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QDesktopServices>
#include <QTranslator>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QLibraryInfo>
#include <QClipboard>
#include <QtNetwork>

#include "ui_mainwindow.h"
#include "about.h"
#include "donation.h"

#define APPNAME QObject::tr("SEDU Configurator")
#define APPVERS "1.0.0"
#define APPDATE "28.07.2015"

#define TIMEOUT 1000

#define SET_CFG 0xC0
#define GET_CFG	0xC1	// <FF> <program> <r> <g> <b> <brightness> <speed> <order> <leds> <00> <AC>
#define SET_EPM	0xC2
#define GET_EPM	0xC3

class MainWindow : public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT

public:

	explicit MainWindow(QWidget *parent = 0);
	int ports;

private:

	bool ignore;

	QTranslator sysTranslator, appTranslator;
	QSerialPortInfo spi;
	QSerialPort *sp;
	QByteArray ba;
	QColor rgb;

private slots:

	QString toFormatedHex(QByteArray str_i);
	void log(QString message);
	bool sendCMD(int cmd);

	void on_pushButton_connect_clicked(bool);
	void on_pushButton_test_clicked();
	void on_pushButton_save_clicked();
	void on_pushButton_run_clicked();
	void on_comboBox_program_activated(int);
	void on_actionExit_triggered();
	void on_actionAbout_triggered();

	void keyPressEvent(QKeyEvent*);
	void closeEvent(QCloseEvent*);
};

#endif
