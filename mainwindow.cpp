#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	MainWindow mw;

	if(!mw.ports)
	{
		return 1;
	}

	mw.move(QApplication::desktop()->screen()->rect().center() - mw.rect().center());

	mw.show();

	return app.exec();
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	if(!QLocale::system().name().startsWith("en_"))
	{
		if(appTranslator.load("seducfg_" + QLocale::system().name(), QApplication::applicationDirPath() + "/lng"))
		{
			QApplication::installTranslator(&appTranslator);

			if(sysTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
			{
				QApplication::installTranslator(&sysTranslator);
			}
			else if(sysTranslator.load("qt_" + QLocale::system().name(), QApplication::applicationDirPath() + "/lng"))
			{
				QApplication::installTranslator(&sysTranslator);
			}
		}
	}

	setupUi(this);

	log(QString("%1 V%2 - (C) 2014-2015 Thomas \"LazyT\" Löwe").arg(APPNAME, APPVERS));

	spi = QSerialPortInfo();

	if(!(ports = spi.availablePorts().count()))
	{
		QMessageBox::critical(this, APPNAME, tr("No serial port detected!\n\nPlease check usb connection and try again..."));

		return;
	}

	for(int i = 0; i < ports; i++)
	{
		comboBox_port->addItem(spi.availablePorts().at(i).portName(), spi.availablePorts().at(i).systemLocation());

		if(spi.availablePorts().at(i).manufacturer() == "FTDI")
		{
			comboBox_port->setCurrentText(spi.availablePorts().at(i).portName());
		}
	}

	comboBox_port->model()->sort(0);

	ignore = false;
}

QString MainWindow::toFormatedHex(QByteArray str_i)
{
	QString str_o;

	for(int i = 0; i < str_i.toHex().length(); i += 2)
	{
		str_o.append(str_i.toHex().toUpper().mid(i, 2) + " ");
	}

	return str_o.left(str_o.length() - 1);
}

void MainWindow::log(QString message)
{
	listWidget->addItem(message);

	listWidget->scrollToBottom();
}

bool MainWindow::sendCMD(int cmd)
{
	ba.clear();
	ba.append("\xA5\x5A\xFF");
	ba.append(cmd);

	if(cmd == SET_CFG)
	{
		ba.append(comboBox_program->currentIndex());
		ba.append(rgb.red());
		ba.append(rgb.green());
		ba.append(rgb.blue());
		ba.append(spinBox_brightness->value());
		ba.append(spinBox_speed->value());
		ba.append(comboBox_color->currentIndex());
		ba.append(spinBox_led->value());
	}

	log(QString("\n> %1").arg(toFormatedHex(ba)));

	QApplication::processEvents();

	sp->write(ba);

	if(sp->waitForBytesWritten(TIMEOUT))
	{
		if(sp->waitForReadyRead(TIMEOUT))
		{
			ba = sp->readAll();

			while(sp->waitForReadyRead(TIMEOUT/10))
			{
				ba += sp->readAll();
			}

			log(QString("< %1").arg(toFormatedHex(ba)));

			if(cmd == GET_CFG)
			{
				ignore = true;
				comboBox_program->setCurrentIndex(ba.at(1) & 0xFF);
				ignore = false;
				rgb.setRed(ba.at(2) & 0xFF);
				rgb.setGreen(ba.at(3) & 0xFF);
				rgb.setBlue(ba.at(4) & 0xFF);
				spinBox_brightness->setValue(ba.at(5) & 0xFF);
				spinBox_speed->setValue(ba.at(6) & 0xFF);
				comboBox_color->setCurrentIndex(ba.at(7) & 0xFF);
				spinBox_led->setValue(ba.at(8) & 0xFF);
			}
		}
		else
		{
			log(tr("Timeout reading data!"));

			return false;
		}
	}
	else
	{
			log(tr("Timeout writing data!"));

			return false;
	}

	return true;
}

void MainWindow::on_pushButton_connect_clicked(bool checked)
{
	if(checked)
	{
		sp = new QSerialPort(comboBox_port->currentText(), this);

		if(sp->open(QIODevice::ReadWrite))
		{
			sp->clear();

			sp->setBaudRate(comboBox_speed->currentText().toInt());
			sp->setDataBits(QSerialPort::Data8);
			sp->setParity(QSerialPort::NoParity);
			sp->setStopBits(QSerialPort::OneStop);
			sp->setFlowControl(QSerialPort::NoFlowControl);

			log(tr("\nConnected to SEDU @ %1 with %2 baud.").arg(comboBox_port->currentData().toString()).arg(sp->baudRate()));

			comboBox_port->setEnabled(false);
			comboBox_speed->setEnabled(false);
			comboBox_program->setEnabled(true);
			comboBox_color->setEnabled(true);
			spinBox_brightness->setEnabled(true);
			spinBox_speed->setEnabled(true);
			spinBox_led->setEnabled(true);
			pushButton_connect->setText(tr("Disconnect"));
			pushButton_test->setEnabled(true);
			pushButton_save->setEnabled(true);
			pushButton_run->setEnabled(true);

			if(!sendCMD(GET_CFG))
			{
				QMessageBox::warning(this, APPNAME, tr("SEDU-Command \"GET_CFG\" failed!"));
			}
		}
		else
		{
			log(tr("\nCould not connect to SEDU @ %1: %2!").arg(comboBox_port->currentData().toString()).arg(sp->errorString()));

			pushButton_connect->setChecked(false);
		}
	}
	else
	{
		log(tr("\nDisconnected from SEDU."));

		comboBox_port->setEnabled(true);
		comboBox_speed->setEnabled(true);
		comboBox_program->setEnabled(false);
		comboBox_color->setEnabled(false);
		spinBox_brightness->setEnabled(false);
		spinBox_speed->setEnabled(false);
		spinBox_led->setEnabled(false);
		pushButton_connect->setText(tr("Connect"));
		pushButton_test->setEnabled(false);
		pushButton_save->setEnabled(false);
		pushButton_run->setEnabled(false);

		sp->close();
	}
}

void MainWindow::on_pushButton_test_clicked()
{
	if(!sendCMD(SET_CFG))
	{
		QMessageBox::warning(this, APPNAME, tr("SEDU-Command \"SET_CFG\" failed!"));
	}
}

void MainWindow::on_pushButton_save_clicked()
{
	if(!sendCMD(SET_EPM))
	{
		QMessageBox::warning(this, APPNAME, tr("SEDU-Command \"SET_EPM\" failed!"));
	}
}

void MainWindow::on_pushButton_run_clicked()
{
	if(!sendCMD(GET_EPM))
	{
		QMessageBox::warning(this, APPNAME, tr("SEDU-Command \"GET_EPM\" failed!"));
	}
}

void MainWindow::on_comboBox_program_activated(int index)
{
	if(index == 1 && !ignore)
	{
		rgb = QColorDialog::getColor(rgb, this);
	}
}

void MainWindow::on_actionExit_triggered()
{
	close();
}


void MainWindow::on_actionAbout_triggered()
{
    aboutDialog *dlg = new aboutDialog(this);
	dlg->exec();
}

void MainWindow::keyPressEvent(QKeyEvent *ke)
{
	if(ke->key() == Qt::Key_F1)
	{
		QMessageBox::information(this, APPNAME, tr("Help is not yet implemented..."));
	}
	else if(ke->key() == Qt::Key_Escape)
	{
		close();
	}

	QMainWindow::keyPressEvent(ke);
}

void MainWindow::closeEvent(QCloseEvent *ce)
{
	if(QMessageBox::question(this, APPNAME, tr("Really exit program?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
	{
		if(pushButton_connect->isChecked())
		{
			on_pushButton_connect_clicked(false);
		}

		ce->accept();
	}
	else
	{
		ce->ignore();
	}
}
