#include "EventsettingDialog.h"
#include <qmessagebox.h>



EventsettingDialog::EventsettingDialog(twinCATParam* pinparam, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::eventeditdialog)
{
	ui->setupUi(this);
	m_param = pinparam;
	for (int i = 0; i < m_param->pinsize; i++)
	{
		if (m_param->pininfos[i]->pintype == twinCATParam::BOOL_TYPE&&m_param->pininfos[i]->rwtype!=twinCATParam::WRITE_ONLY) {
			ui->comboBox->addItem(m_param->pininfos[i]->Pinname);
			m_pindexs.append(i);
		}
	}
	if(m_pindexs.size()>0)
		ui->lineEdit->setText(m_param->pininfos[m_pindexs[0]]->eventname[0]);
	connect(ui->okButton, SIGNAL(clicked()), SLOT(sureclicked()));
	connect(ui->cancelButton, SIGNAL(clicked()), SLOT(cancelclicked()));
	connectSlots(true);
}


EventsettingDialog::~EventsettingDialog()
{
}

void EventsettingDialog::setdata(twinCATParam::Eventinfo pinfo)
{
	connectSlots(false);
	ui->comboBox->setCurrentText(pinfo.pinname);
	ui->comboBox_2->setCurrentIndex(pinfo.triggertype);
	ui->lineEdit->setText(pinfo.eventname);
	m_oldeventinfo = pinfo;
	connectSlots(true);

}

twinCATParam::Eventinfo* EventsettingDialog::getdata()
{
	twinCATParam::Eventinfo* tmp= new twinCATParam::Eventinfo();
	tmp->pinname = m_param->pininfos[m_pindexs[ui->comboBox->currentIndex()]]->Pinname;
	tmp->triggertype = ui->comboBox_2->currentIndex();
	tmp->eventname = ui->lineEdit->text();
	return tmp;
}

void EventsettingDialog::setshowmode(int sm)
{
	m_showmode = sm;
}

void EventsettingDialog::updatepins()
{
	connectSlots(false);
	ui->comboBox->clear();
	m_pindexs.clear();
	for (int i = 0; i < m_param->pinsize; i++)
	{
		if (m_param->pininfos[i]->pintype == twinCATParam::BOOL_TYPE&&m_param->pininfos[i]->rwtype != twinCATParam::WRITE_ONLY) {
			ui->comboBox->addItem(m_param->pininfos[i]->Pinname);
			m_pindexs.append(i);
		}
	}
	if (m_pindexs.size()>0)
		ui->lineEdit->setText(m_param->pininfos[m_pindexs[0]]->eventname[0]);
	connectSlots(true);
}

void EventsettingDialog::sureclicked()
{
	twinCATParam::Eventinfo tmp;
	tmp.pinname = m_param->pininfos[m_pindexs[ui->comboBox->currentIndex()]]->Pinname;
	tmp.eventname = ui->lineEdit->text();
	tmp.triggertype = ui->comboBox_2->currentIndex();
	switch (m_showmode)
	{
	case 0:
		if (m_param->eventinfos.contains(tmp)) {
			{
				QMessageBox::warning(this, "Warning", QString::fromLocal8Bit("触发信号已存在对应事件"), QMessageBox::Button::Ok);
				return;
			}
		}
		m_param->pininfos[m_pindexs[ui->comboBox->currentIndex()]]->eventname[ui->comboBox_2->currentIndex()] = tmp.eventname;
		break;
	case 1:
		if (m_param->eventinfos.contains(tmp)&&(!(tmp== m_oldeventinfo))) {
			{
				QMessageBox::warning(this, "Warning", QString::fromLocal8Bit("已存在相同管脚名"), QMessageBox::Button::Ok);
				return;
			}
		}
		m_param->pininfos[m_pindexs[ui->comboBox->findText(m_oldeventinfo.pinname)]]->eventname[m_oldeventinfo.triggertype] = "";
		m_param->pininfos[m_pindexs[ui->comboBox->currentIndex()]]->eventname[ui->comboBox_2->currentIndex()] = tmp.eventname;

	default:
		break;
	}
	QDialog::accept();
}

void EventsettingDialog::cancelclicked()
{
	QDialog::reject();
}

void EventsettingDialog::pinnamechanged(int index)
{
	ui->lineEdit->setText(m_param->pininfos[m_pindexs[index]]->eventname[ui->comboBox_2->currentIndex()]);
}

void EventsettingDialog::triggertypechanged(int index)
{
	//ui->lineEdit->setText(m_param->pininfos[m_pindexs[ui->comboBox->currentIndex()]]->eventname[index]);
}

void EventsettingDialog::connectSlots(bool link)
{
	if (m_hasConnect == link)
		return;
	connectslot(link, ui->comboBox, SIGNAL(currentIndexChanged(int)), SLOT(pinnamechanged(int)));
	connectslot(link, ui->comboBox_2, SIGNAL(currentIndexChanged(int)), SLOT(triggertypechanged(int)));
	m_hasConnect = link;
}

void EventsettingDialog::connectslot(bool link, const QObject * sender, const char * signal, const char * method)
{
	if (link)
		connect(sender, signal, this, method);
	else
		disconnect(sender, signal, this, method);
}
