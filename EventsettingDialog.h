#pragma once
#include <QDialog>
#include "ui_eventedit.h"
#include "twinCATParam.h"
class EventsettingDialog :
	public QDialog
{
	Q_OBJECT
public:
	EventsettingDialog(twinCATParam* pinparam, QWidget *parent = 0);
	~EventsettingDialog();
	void setdata(twinCATParam::Eventinfo pinfo);
	twinCATParam::Eventinfo* getdata();
	enum showmode
	{
		ADD,EDIT
	};
	void setshowmode(int sm);
	void updatepins();
private slots:
	void sureclicked();
	void cancelclicked();
	void pinnamechanged(int index);
	void triggertypechanged(int index);
private:
	Ui::eventeditdialog *ui;
	twinCATParam* m_param;
	QList<QString> m_eventnames;
	QList<int> m_pindexs;
	twinCATParam::Eventinfo m_oldeventinfo;
	int m_showmode;
	bool m_hasConnect;
	void connectSlots(bool link);
	void connectslot(bool link, const QObject * sender, const char * signal, const char * method);
};

