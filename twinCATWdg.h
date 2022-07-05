#pragma once
#include <moduleClass.h>
#include <qmenu.h>
#include "PinsettingDialog.h"
#include "EventsettingDialog.h"
#include <QTableWidget>

namespace Ui { class twinCATWdg; }
class twinCATParam;
class twinCATMod;
using namespace qzcv;

class twinCATWdg : public ModSetWidget
{
	Q_OBJECT

public:
	twinCATWdg(int i=0);
	~twinCATWdg();
	virtual void setModule(UnitModule* moudle);
	private slots:
	void edit_pin_setting();
	void edit_event_setting();
	void addpin();
	void delpin();
	void editpin();
	void addeventname();
	void deleventname();
	void editeventname();
	void showpineditmenu();
	void showeventeditmenu();
	void updatedata50ms();
	void on_connect_clicked();
private:
	void connectSlots(bool link);
	void iniUi();
	void setUiValue();
	void updatecurentrow(QTableWidget *tablewidget, twinCATParam::Pininfo pininfo);
	void updateeventinfos();
private:
	Ui::twinCATWdg *ui;
	bool m_hasConnect;
	QMenu *m_pin_editmenu;
	QMenu *m_event_editmenu;
	QAction *m_addpin;
	QAction *m_delpin;
	QAction *m_editpin;
	QAction *m_addevent;
	QAction *m_delevent;
	QAction *m_editevent;
	PinsettingDialog *m_pinsettingdialog;
	EventsettingDialog *m_eventsettingdialog;
	twinCATParam* m_param;
	twinCATMod* m_module;
	int m_widgetType;
	int m_selectedpin;
	QStringList m_datatypes;// = { "bool","int","double","qstring","XY","XYA" };
	QStringList m_rwtypes;// = { "READ","WRITE","RW" };
	QStringList m_triggertypes;// = { "Raising","Falling" };

};

