#pragma once

#include <QDialog>
#include "ui_pinedit.h"
#include "twinCATParam.h"
class PinsettingDialog :
	public QDialog
{
	Q_OBJECT
public:
	PinsettingDialog(twinCATParam* pinparam,QWidget *parent = 0);
	~PinsettingDialog();
	enum showmode
	{
		ADD,
		EDIT
	};
	void setshowmode(showmode smode);
	void setdata(twinCATParam::Pininfo pindata);
	twinCATParam::Pininfo * getdata();
	void delpinname(QString pinname);
	void updatepinname(QString oldpiname, QString newpiname);
private slots:
	void sureclicked();
	void cancelclicked();
	void rwtypechanged(int index);
	void datatypechanged(int index);


private:
	Ui::PinsettingDialog *ui;
	twinCATParam* m_pinparam;
	QList<QString> m_pinnames;
	showmode m_showmode;
	QString m_oldname;
};

