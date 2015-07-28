#include "donation.h"

donationDialog::donationDialog(QWidget *parent) : QDialog(parent)
{
	setupUi(this);

	setWindowFlags(Qt::Tool);
	layout()->setSizeConstraint(QLayout::SetFixedSize);

	lineEdit_payment3->hide();
	lineEdit_payment4->hide();

	BTCexchangeRate = -1;

	show();
	activateWindow();
	move(parent->mapToGlobal(parent->rect().center()) - rect().center());
}

void donationDialog::on_horizontalSlider_valueChanged(int value)
{
	pushButton->setText(QString(tr("donate %1€ via %2")).arg(value).arg(comboBox->currentText()));

	if(comboBox->currentIndex() == 1)
	{
		lineEdit_payment2->setText(QString("%1 EUR = %2 BTC").arg(value).arg(BTCexchangeRate ? QString::number(horizontalSlider->value() * BTCexchangeRate, 'f', 8) : "?"));
	}
}

void donationDialog::on_comboBox_activated(const QString &text)
{
	pushButton->setText(QString(tr("donate %1€ via %2")).arg(horizontalSlider->value()).arg(text));

	lineEdit_payment3->hide();
	lineEdit_payment4->hide();

	switch(comboBox->currentIndex())
	{
		case 0:
		{
			lineEdit_payment1->setText(QByteArray::fromBase64("TGF6eVRAZ214Lm5ldA=="));
			lineEdit_payment1->setToolTip(tr("beneficiary email address"));
			lineEdit_payment2->setText("https://www.paypal.com");
			lineEdit_payment2->setToolTip(tr("transaction website"));

			label_qrcode->setPixmap(QPixmap(":/png/png/qr_pp.png"));

			break;
		}
		case 1:
		{
			if(BTCexchangeRate == -1)
			{
				BTCexchangeRate = EUR2BTC();
			}

			lineEdit_payment1->setText(QByteArray::fromBase64("MUVqRDhOa2RCUDFBeUd5OWdod3VQcGc4Z1FFb29qM1E4aw=="));
			lineEdit_payment1->setToolTip(tr("beneficiary BitCoin address"));
			lineEdit_payment2->setText(QString("%1 EUR = %2 BTC").arg(horizontalSlider->value()).arg(BTCexchangeRate ? QString::number(horizontalSlider->value() * BTCexchangeRate, 'f', 8) : "?"));
			lineEdit_payment2->setToolTip(tr("current exchange rate"));

			label_qrcode->setPixmap(QPixmap(":/png/png/qr_bc.png"));

			break;
		}
		case 2:
		{
			lineEdit_payment1->setText(QByteArray::fromBase64("VGhvbWFzIEzDtndl"));
			lineEdit_payment1->setToolTip(tr("beneficiary name"));
			lineEdit_payment2->setText(QByteArray::fromBase64("REU2NyA4NjA1NTU5MiAxODAwMDAwOTQw"));
			lineEdit_payment2->setToolTip("IBAN");

			label_qrcode->setPixmap(QPixmap(":/png/png/qr_bt.png"));

			lineEdit_payment3->show();
			lineEdit_payment4->show();

			break;
		}
	}
}

void donationDialog::on_pushButton_clicked()
{
	if(comboBox->currentIndex() == 0)
	{
        QDesktopServices::openUrl(QString(QByteArray::fromBase64("aHR0cHM6Ly93d3cucGF5cGFsLmNvbS9jZ2ktYmluL3dlYnNjcj9jbWQ9X3hjbGljayZidXNpbmVzcz1MYXp5VEBnbXgubmV0Jml0ZW1fbmFtZT0lMSZhbW91bnQ9JTImY3VycmVuY3lfY29kZT1FVVI=")).arg("SEDUCFG").arg(horizontalSlider->value()));

		QMessageBox::information(this, APPNAME, tr("The default web browser will be started now with the donation website.\n\nIf this fails please donate manually with the shown information."));
	}
	else if(comboBox->currentIndex() == 1)
	{
		QDesktopServices::openUrl(QString(QByteArray::fromBase64("Yml0Y29pbjolMT9hbW91bnQ9JTImbGFiZWw9TGF6eVQmbWVzc2FnZT0lMw==")).arg(lineEdit_payment1->text()).arg(horizontalSlider->value() * BTCexchangeRate).arg(APPNAME));

		QMessageBox::information(this, APPNAME, tr("The default Bitcoin client will be started now with the payment information.\n\nIf this fails please donate manually with the shown information."));
	}
	else
	{
		QApplication::clipboard()->setText(tr("Beneficiary\t: %1\nIBAN\t\t: %2\nBIC\t\t: %3\nBank\t\t: %4").arg(lineEdit_payment1->text()).arg(lineEdit_payment2->text()).arg(lineEdit_payment3->text()).arg(lineEdit_payment4->text()));

		QMessageBox::information(this, APPNAME, tr("The payment information will be copied to the clipboard.\n\nPlease perform the transfer using this data."));
	}

	close();
}

double donationDialog::EUR2BTC()
{
	QNetworkAccessManager *mgr = new QNetworkAccessManager();
	QElapsedTimer timeout;

	QNetworkReply *rep = mgr->get(QNetworkRequest(QUrl("https://blockchain.info/de/tobtc?currency=EUR&value=1")));

	rep->ignoreSslErrors();

	lineEdit_payment2->setText(tr("getting current exchange rate..."));
	comboBox->setEnabled(false);
	horizontalSlider->setEnabled(false);
	pushButton->setEnabled(false);

	timeout.start();

	while(rep->isRunning())
	{
		QCoreApplication::processEvents();

		if(timeout.hasExpired(3000))
		{
			rep->abort();
		}
	}

	comboBox->setEnabled(true);
	horizontalSlider->setEnabled(true);
	pushButton->setEnabled(true);

	if(rep->error())
	{
		QMessageBox::warning(this, APPNAME, tr("Current exchange rate could not be determined!\n\nPlease specify the desired amount manually..."));

		return 0.0;
	}

	QByteArray raw = rep->readAll();

	rep->deleteLater();

	return raw.toDouble();
}

void donationDialog::keyPressEvent(QKeyEvent *ke)
{
	if(ke->key() == Qt::Key_F1)
	{
		QMessageBox::information(this, APPNAME, tr("Help is not yet implemented..."));
	}

	QDialog::keyPressEvent(ke);
}
