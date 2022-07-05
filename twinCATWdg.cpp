#include "twinCATWdg.h"
#include "twinCATMod.h"
#include "twinCATParam.h"
#include "ui_twinCATWdg.h"


 twinCATWdg::twinCATWdg(int i):
	 ModSetWidget(), ui(new Ui::twinCATWdg), m_widgetType(i)
 {
	 m_hasConnect = false;
	 ui->setupUi(this);
	 iniUi();
	 m_param = nullptr;
	 m_module = nullptr;
	 m_pinsettingdialog = NULL;
	m_eventsettingdialog = NULL;

	 m_datatypes<<"bool"<<"int" << "double" << "qstring" << "XY" << "XYA" ;
	 m_rwtypes << "READ" << "WRITE" << "RW" ;
	 m_triggertypes << "Raising" << "Falling";
 }

twinCATWdg::~twinCATWdg()
{
	delete ui;
}


void twinCATWdg::setModule(UnitModule *module)
{
	m_module = dynamic_cast<twinCATMod*>(module);
	m_param = m_module->m_param;
	if (m_pinsettingdialog == NULL)
		m_pinsettingdialog = new PinsettingDialog(m_param, this);
	if (!m_eventsettingdialog)
		m_eventsettingdialog = new EventsettingDialog(m_param);
	connectSlots(false);
	setUiValue();
	connectSlots(true);
	updateeventinfos();
}

void twinCATWdg::edit_pin_setting()
{
}

void twinCATWdg::edit_event_setting()
{
}

void twinCATWdg::addpin()
{
	if (m_pinsettingdialog == NULL)
		m_pinsettingdialog = new PinsettingDialog(m_param, this);
	twinCATParam::Pininfo pindata;
	pindata.Pinname = "newpin";
	pindata.pintype = twinCATParam::DOUBLE_TYPE;
	pindata.rwtype = twinCATParam::READ_ONLY;
	pindata.varname[0] = "tesfloat";
	m_pinsettingdialog->setshowmode(PinsettingDialog::ADD);
	m_pinsettingdialog->setdata(pindata);

	int result = m_pinsettingdialog->exec();
	if (result == QDialog::Accepted) {
		m_param->pininfos[m_param->pinsize] = m_pinsettingdialog->getdata();
		switch (m_param->pininfos[m_param->pinsize]->rwtype)
		{
		case twinCATParam::READ_ONLY:
			m_module->addoutputpin(*m_param->pininfos[m_param->pinsize]);
			break;
		case twinCATParam::WRITE_ONLY:
			m_module->addinputpin(*m_param->pininfos[m_param->pinsize]);
			break;
		case twinCATParam::RW:
			m_module->addinputpin(*m_param->pininfos[m_param->pinsize]);
			m_module->addoutputpin(*m_param->pininfos[m_param->pinsize]);
			break;
		default:
			break;
		}
		ui->tableWidget_2->insertRow(m_param->pinsize);
		QTableWidgetItem *item = new QTableWidgetItem(m_param->pininfos[m_param->pinsize]->Pinname);
		ui->tableWidget_2->setItem(m_param->pinsize, 0, item);
		item = new QTableWidgetItem(m_datatypes[m_param->pininfos[m_param->pinsize]->pintype]);
		ui->tableWidget_2->setItem(m_param->pinsize, 1, item);
		item = new QTableWidgetItem(m_rwtypes[m_param->pininfos[m_param->pinsize]->rwtype]);
		ui->tableWidget_2->setItem(m_param->pinsize, 2, item);
		m_param->pinsize++;
		m_eventsettingdialog->updatepins();
	}
}

void twinCATWdg::delpin()
{
	m_module->delpin(*(m_param->pininfos[m_selectedpin]));
	m_pinsettingdialog->delpinname(m_param->pininfos[m_selectedpin]->Pinname);
	delete m_param->pininfos[m_selectedpin];
	for (int i = m_selectedpin; i < m_param->pinsize - 1; i++)
	{
		m_param->pininfos[i] = m_param->pininfos[i + 1];
	}
	ui->tableWidget_2->removeRow(m_selectedpin);
	m_param->pinsize--;
	m_eventsettingdialog->updatepins();
	updateeventinfos();
}

void twinCATWdg::editpin()
{
	if (m_pinsettingdialog == NULL)
		m_pinsettingdialog = new PinsettingDialog(m_param, this);
	m_pinsettingdialog->setdata(*m_param->pininfos[m_selectedpin]);
	m_pinsettingdialog->setshowmode(PinsettingDialog::EDIT);
	int result = m_pinsettingdialog->exec();
	if (result == QDialog::Accepted) {
		m_module->delpin(*m_param->pininfos[m_selectedpin]);
		delete m_param->pininfos[m_selectedpin];
		m_param->pininfos[m_selectedpin] = m_pinsettingdialog->getdata();
		m_module->addpin(*m_param->pininfos[m_selectedpin]);
		updatecurentrow(ui->tableWidget_2, *m_param->pininfos[m_selectedpin]);
	}
	m_eventsettingdialog->updatepins();
	updateeventinfos();
}

void twinCATWdg::addeventname()
{
	if (!m_eventsettingdialog)
		m_eventsettingdialog = new EventsettingDialog(m_param);
	int index = ui->tableWidget->currentRow();
	//if(index>=0)
	//	m_eventsettingdialog->setdata(m_module->m_eventinfos[index]);
	m_eventsettingdialog->setshowmode(0);
	int res = m_eventsettingdialog->exec();
	if (res == QDialog::Accepted) {
		twinCATParam::Eventinfo * newpin = m_eventsettingdialog->getdata();
		//if(m_module->m_eventinfos.contains(*newpin)){
		//	int index = m_module->m_eventinfos.indexOf(*newpin);
		//	m_module->m_eventinfos[index].eventname;
		//	m_module->removeEvent(m_module->m_eventinfos[index].eventname);
		//	m_module->m_eventinfos[index].eventname = newpin->eventname;
		//	m_module->addEvent(newpin->eventname);
		//}
		//else
		{
			if (!m_module->eventNames().contains(newpin->eventname))
				m_module->addEvent(newpin->eventname);
			m_module->m_eventinfos.append(*newpin);
			m_param->eventinfos.append(*newpin);
			m_param->triggerssize++;
		}
		updateeventinfos();
	}
}

void twinCATWdg::deleventname()
{
	int index = ui->tableWidget->currentRow();
	for (int i = 0; i < m_param->pinsize; i++)
	{
		if (m_param->pininfos[i]->Pinname == m_param->eventinfos[index].pinname) {
			m_param->pininfos[i]->eventname[m_param->eventinfos[index].triggertype] = "";
		}
	}
	QString eventname = m_module->m_eventinfos.at(index).eventname;
	bool removeevent = true;
	m_module->m_eventinfos.removeAt(index);
	m_param->eventinfos.removeAt(index);
	for (int i = 0; i < m_module->m_eventinfos.size(); i++)
	{
		if (m_module->m_eventinfos.at(i).eventname == eventname) {
			removeevent = false;
		}
	}
	if (removeevent)
		m_module->removeEvent(eventname);
	updateeventinfos();
	m_param->triggerssize--;

}

void twinCATWdg::editeventname()
{
	if (!m_eventsettingdialog)
		m_eventsettingdialog = new EventsettingDialog(m_param);
	int index = ui->tableWidget->currentRow();
	if (index >= 0)
		m_eventsettingdialog->setdata(m_module->m_eventinfos[index]);
	m_eventsettingdialog->setshowmode(1);
	int res = m_eventsettingdialog->exec();
	if (res == QDialog::Accepted) {
		//for (int i = 0; i < m_param->pinsize; i++)
		//{
		//	if (m_param->pininfos[i]->Pinname == m_param->eventinfos[index].pinname) {
		//		m_param->pininfos[i]->eventname[m_param->eventinfos[index].triggertype] = "";
		//	}
		//}
		twinCATParam::Eventinfo * newpin = m_eventsettingdialog->getdata();
		QString eventname = m_module->m_eventinfos.at(index).eventname;
		bool removeevent = true;
		m_module->m_eventinfos[index] = *newpin;
		m_param->eventinfos[index] = *newpin;
		//for (int i = 0; i < m_param->pinsize; i++)
		//{
		//	if (m_param->pininfos[i]->Pinname == m_param->eventinfos[index].pinname) {
		//		m_param->pininfos[i]->eventname[m_param->eventinfos[index].triggertype] = newpin->eventname;
		//	}
		//}
		for (int i = 0; i < m_module->m_eventinfos.size(); i++)
		{
			if (m_module->m_eventinfos.at(i).eventname == eventname) {
				removeevent = false;
			}
		}
		if (removeevent && (!m_module->eventNames().contains(newpin->eventname)))
			m_module->renameEvent(eventname, newpin->eventname);
		else if (removeevent && (m_module->eventNames().contains(newpin->eventname)))
			m_module->removeEvent(eventname);
		else if ((!removeevent) && (!m_module->eventNames().contains(newpin->eventname)))
		{
			m_module->addEvent(newpin->eventname);
		}
		updateeventinfos();
	}
}

void twinCATWdg::showpineditmenu()
{
	//Q_UNUSED
	switch (m_param->pinsize)
	{
	case 0:
		m_delpin->setVisible(false);
		m_editpin->setVisible(false);
		break;
	default:
		m_delpin->setVisible(true);
		m_editpin->setVisible(true);
		break;
	}
	m_selectedpin = ui->tableWidget_2->currentRow();
	m_pin_editmenu->exec(QCursor::pos());
}

void twinCATWdg::showeventeditmenu()
{
	m_event_editmenu->exec(QCursor::pos());
}

void twinCATWdg::updatedata50ms()
{
	m_module->update50ms();
}

void twinCATWdg::on_connect_clicked()
{
	m_param->port = ui->port->value();
	m_param->netid[0] = ui->id1->text().toInt();
	m_param->netid[1] = ui->id2->text().toInt();
	m_param->netid[2] = ui->id3->text().toInt();
	m_param->netid[3] = ui->id4->text().toInt();
	m_param->netid[4] = ui->id5->text().toInt();
	m_param->netid[5] = ui->id6->text().toInt();
	m_module->connectplc();
}

void twinCATWdg::connectSlots(bool link)
{
	if (m_hasConnect == link)
		return;
	m_hasConnect = link;

}

void twinCATWdg::iniUi()
{
	ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	ui->tableWidget_2->setContextMenuPolicy(Qt::CustomContextMenu);
	m_pin_editmenu = new QMenu(this);
	m_event_editmenu = new QMenu(this);
	m_addpin = new QAction("add");
	m_delpin = new QAction("delete");
	m_editpin = new QAction("edit");
	m_addevent = new QAction("add");
	m_delevent = new QAction("delete");
	m_editevent = new QAction("edit");
	m_pin_editmenu->addAction(m_addpin);
	m_pin_editmenu->addAction(m_delpin);
	m_pin_editmenu->addAction(m_editpin);
	m_event_editmenu->addAction(m_addevent);
	m_event_editmenu->addAction(m_delevent);
	m_event_editmenu->addAction(m_editevent);
	connect(m_addpin, SIGNAL(triggered()), SLOT(addpin()));
	connect(m_delpin, SIGNAL(triggered()), SLOT(delpin()));
	connect(m_editpin, SIGNAL(triggered()), SLOT(editpin()));
	connect(m_addevent, SIGNAL(triggered()), SLOT(addeventname()));
	connect(m_delevent, SIGNAL(triggered()), SLOT(deleventname()));
	connect(m_editevent, SIGNAL(triggered()), SLOT(editeventname()));
	connect(ui->tableWidget, SIGNAL(customContextMenuRequested(const QPoint &)), SLOT(showeventeditmenu()));
	connect(ui->tableWidget_2, SIGNAL(customContextMenuRequested(const QPoint &)), SLOT(showpineditmenu()));
}

void twinCATWdg::setUiValue()
{
	if (!m_param)
		return;
	ui->tableWidget_2->setRowCount(m_param->pinsize);
	for (int i = 0; i < m_param->pinsize; i++)
	{
		QTableWidgetItem *item = new QTableWidgetItem(m_param->pininfos[i]->Pinname);
		ui->tableWidget_2->setItem(i, 0, item);
		item = new QTableWidgetItem(m_datatypes[m_param->pininfos[i]->pintype]);
		ui->tableWidget_2->setItem(i, 1, item);
		item = new QTableWidgetItem(m_rwtypes[m_param->pininfos[i]->rwtype]);
		ui->tableWidget_2->setItem(i, 2, item);
	}
	ui->port->setValue(m_param->port);
	QString idtext[6];
	for (int i = 0; i < 6; i++)
	{
		char text[256];
		sprintf(text, "%d", m_param->netid[i]);
		idtext[i] = text;
	}
	ui->id1->setText(idtext[0]);
	ui->id2->setText(idtext[1]);
	ui->id3->setText(idtext[2]);
	ui->id4->setText(idtext[3]);
	ui->id5->setText(idtext[4]);
	ui->id6->setText(idtext[5]);
}

void twinCATWdg::updatecurentrow(QTableWidget *tablewidget, twinCATParam::Pininfo pininfo)
{
	tablewidget->item(m_selectedpin, 0)->setText(pininfo.Pinname);
	tablewidget->item(m_selectedpin, 1)->setText(m_datatypes[pininfo.pintype]);
	tablewidget->item(m_selectedpin, 2)->setText(m_rwtypes[pininfo.rwtype]);

}

void twinCATWdg::updateeventinfos()
{
	m_module->m_eventinfos.clear();
	QList<QString> eventnamestmp;
	QStringList eventnamesnow = m_module->eventNames();
	for (int i = 0; i < m_param->pinsize; i++)
	{
		twinCATParam::Pininfo *pininfo = m_param->pininfos[i];
		if (pininfo->rwtype==twinCATParam::BOOL_TYPE&&pininfo->rwtype != twinCATParam::WRITE_ONLY)
		{
			for (int j = 0; j < 2; j++)
			{
				if (pininfo->eventname[j].size() > 0) {
					twinCATParam::Eventinfo e;
					e.eventname = pininfo->eventname[j];
					e.pinname = pininfo->Pinname;
					e.triggertype = j;
					m_module->m_eventinfos.append(e);
					if (!eventnamestmp.contains(pininfo->eventname[j])) {
						eventnamestmp.append(pininfo->eventname[j]);
					}
				}
			}
		}
	}
	for (int i = 0; i < eventnamesnow.size(); i++)
	{
		if (!eventnamestmp.contains(eventnamesnow[i])) {
			m_module->removeEvent(eventnamesnow[i]);
		}
	}
	m_param->triggerssize = m_module->m_eventinfos.size();
	m_param->eventinfos = m_module->m_eventinfos;
	QList<twinCATParam::Eventinfo> tmp = m_module->m_eventinfos;
	//ui->tableWidget->clear();
	ui->tableWidget->setRowCount(tmp.size());
	//m_module->clearEvents();
	//m_module->eventNames();
	QTableWidgetItem *item;
	for (int i = 0; i < tmp.size(); i++)
	{
		item = new QTableWidgetItem(tmp.at(i).pinname);
		ui->tableWidget->setItem(i, 0, item);
		item = new QTableWidgetItem(tmp.at(i).eventname);
		ui->tableWidget->setItem(i, 1, item);
		item = new QTableWidgetItem(m_triggertypes[tmp.at(i).triggertype]);
		ui->tableWidget->setItem(i, 2, item);
		//if (!m_module->eventNames().contains(tmp.at(i).eventname))
		//	m_module->addEvent(tmp.at(i).eventname);
	}
}

