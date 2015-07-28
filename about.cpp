#include "about.h"

aboutDialog::aboutDialog(QWidget *parent) : QDialog(parent)
{
	setupUi(this);

	setWindowFlags(Qt::Tool);
	layout()->setSizeConstraint(QLayout::SetFixedSize);

	label_Version->setText(tr("Version %1 - %2").arg(APPVERS, APPDATE));
	label_Update->setStyleSheet(ICO_UPDATE);

	textEdit->setAlignment(Qt::AlignCenter);
	textEdit->textCursor().insertText(tr("This program is Freeware and may be installed and used free of charge for non-commercial use on as many computers as you like without limitations.\n\nA liability for any damages resulting from the use is excluded. Use at your own risk!"));
	textEdit->moveCursor(QTextCursor::Start);
	textEdit->setFixedHeight(4 * textEdit->fontMetrics().height() + textEdit->document()->documentMargin() + 2);

	show();
	activateWindow();
	move(parent->mapToGlobal(parent->rect().center()) - rect().center());
}

void aboutDialog::mouseReleaseEvent(QMouseEvent *me)
{
	QWidget *child = QWidget::childAt(me->pos());

	if(child)
	{
		QString name = child->objectName();

		if(name == "label_Mail")
		{
            QDesktopServices::openUrl(QUrl(QByteArray::fromBase64("bWFpbHRvOkxhenlUQGdteC5uZXQ/c3ViamVjdD1TRURVQ0ZHJmJvZHk9V3JpdGUgaW4gRW5nbGlzaCBvciBHZXJtYW4gcGxlYXNlLi4u")));
		}
		else if(name == "label_Forum")
		{
            QDesktopServices::openUrl(QUrl(QByteArray::fromBase64("aHR0cDovL3d3dy5sZWRzdHlsZXMuZGUvaW5kZXgucGhwL0JvYXJkLzM3LUxFRC1Ub29scy1UdXRvcmlhbHM=")));
		}
		else if(name == "label_Donation")
		{
			if(QMessageBox::warning(this, APPNAME, tr("Please note the following points:\n\n* The payment is made voluntarily without the acquisition of claims.\n* You receive no rights to the offered software.\n* Because this is not a donation in juridical sense no certificate can be issued.\n\nWould you like to support the further development of this project nevertheless?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
			{
				new donationDialog((QWidget*)parent());
			}
		}
		else if(name == "label_Update")
		{
            QDesktopServices::openUrl(QUrl(QByteArray::fromBase64("aHR0cHM6Ly9naXRodWIuY29tL0xhenlUL3NlZHVjZmcvcmVsZWFzZXM=")));
		}
		else if(name == "label_Language")
		{
            QDesktopServices::openUrl(QUrl(QByteArray::fromBase64("aHR0cHM6Ly9naXRodWIuY29tL0xhenlUL3NlZHVjZmcvdHJlZS9tYXN0ZXIvbG5n")));
		}
	}
}

void aboutDialog::keyPressEvent(QKeyEvent *ke)
{
	if(ke->key() == Qt::Key_F1)
	{
		QMessageBox::information(this, APPNAME, tr("Help is not yet implemented..."));
	}

	QDialog::keyPressEvent(ke);
}
