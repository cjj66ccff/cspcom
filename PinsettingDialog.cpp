#include "PinsettingDialog.h"
#include <qmessagebox.h>



PinsettingDialog::PinsettingDialog(twinCATParam* pinparam, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PinsettingDialog)
{
	ui->setupUi(this);
	m_pinparam = pinparam;
	for (int i = 0; i < m_pinparam->pinsize; i++)
	{
		m_pinnames.append(m_pinparam->pininfos[i]->Pinname);
	}
	connect(ui->okButton, SIGNAL(clicked()), SLOT(sureclicked()));
	connect(ui->cancelButton, SIGNAL(clicked()), SLOT(cancelclicked()));
	connect(ui->rwtype, SIGNAL(currentIndexChanged(int)), SLOT(rwtypechanged(int)));
	connect(ui->var_type, SIGNAL(currentIndexChanged(int)), SLOT(datatypechanged(int)));
}


PinsettingDialog::~PinsettingDialog()
{
	delete ui;
}

void PinsettingDialog::setshowmode(showmode smode)
{
	m_showmode = smode;
	
}

void PinsettingDialog::setdata(twinCATParam::Pininfo pindata)
{
	m_oldname = pindata.Pinname;
	ui->var_name->setText(pindata.Pinname);
	ui->var_type->setCurrentIndex(pindata.pintype);
	ui->rwtype->setCurrentIndex(pindata.rwtype);
	switch (pindata.pintype)
	{
	case twinCATParam::PinType::BOOL_TYPE:
	case twinCATParam::PinType::DOUBLE_TYPE:
	case twinCATParam::PinType::INT_TYPE:
	case twinCATParam::PinType::QSTRING_TYPE:
		ui->x_name->setText(pindata.varname[0]);
		ui->x_label->setVisible(false);
		ui->y_label->setVisible(false);
		ui->a_label->setVisible(false);
		ui->y_name->setVisible(false);
		ui->a_name->setVisible(false);
		break;
	case twinCATParam::PinType::XY_TYPE:
		ui->x_name->setText(pindata.varname[0]);
		ui->y_name->setText(pindata.varname[1]);
		ui->x_label->setVisible(true);
		ui->y_label->setVisible(true);
		ui->a_label->setVisible(false);
		ui->y_name->setVisible(true);
		ui->a_name->setVisible(false);
		break;
	case twinCATParam::PinType::XYA_TYPE:
		ui->x_name->setText(pindata.varname[0]);
		ui->y_name->setText(pindata.varname[1]);
		ui->a_name->setText(pindata.varname[2]);
		ui->x_label->setVisible(true);
		ui->y_label->setVisible(true);
		ui->a_label->setVisible(true);
		ui->y_name->setVisible(true);
		ui->a_name->setVisible(true);
		break;
	default:
		break;
	}
}

twinCATParam::Pininfo * PinsettingDialog::getdata()
{
	twinCATParam::Pininfo *pindata = new twinCATParam::Pininfo;
	pindata->Pinname = ui->var_name->text();
	pindata->pintype = twinCATParam::PinType(ui->var_type->currentIndex());
	pindata->rwtype = twinCATParam::RWType(ui->rwtype->currentIndex());
	switch (ui->var_type->currentIndex())
	{
	case twinCATParam::PinType::BOOL_TYPE:
	case twinCATParam::PinType::DOUBLE_TYPE:
	case twinCATParam::PinType::INT_TYPE:
	case twinCATParam::PinType::QSTRING_TYPE:
		pindata->varname[0] = ui->x_name->text();
		break;
	case twinCATParam::PinType::XY_TYPE:
		pindata->varname[0] = ui->x_name->text();
		pindata->varname[1] = ui->y_name->text();
		break;
	case twinCATParam::PinType::XYA_TYPE:
		pindata->varname[0] = ui->x_name->text();
		pindata->varname[1] = ui->y_name->text();
		pindata->varname[2] = ui->a_name->text();
		break;
	default:
		break;
	}
	return pindata;
}

void PinsettingDialog::delpinname(QString pinname)
{
	m_pinnames.removeOne(pinname);
}

void PinsettingDialog::updatepinname(QString oldpiname,QString newpiname)
{
	m_pinnames.replace(m_pinnames.indexOf(oldpiname, 0), newpiname);
}

void PinsettingDialog::sureclicked()
{
	QString pinname = ui->var_name->text();
	if (m_pinnames.contains(pinname)) {
		if(m_showmode==ADD|| (m_showmode == EDIT&&pinname != m_oldname)){
			QMessageBox::warning(this, "Warning", QString::fromLocal8Bit("已存在相同管脚名"),QMessageBox::Button::Ok);
			return;
		}
	}
	if (ui->x_name->text().size() == 0 || (ui->y_name->text().size() == 0 && ui->y_name->isVisible()) || (ui->a_name->text().size() == 0 && ui->a_name->isVisible())) {
		QMessageBox::warning(this,"Warning", QString::fromLocal8Bit("变量名不能为空"), QMessageBox::Button::Ok);
		return;
	}
	switch (m_showmode)
	{
	case EDIT:
		m_pinnames.replaceInStrings(m_oldname, pinname);
		break;
	case ADD:
		m_pinnames.append(pinname);
	default:
		break;
	}
	accept();
}

void PinsettingDialog::cancelclicked()
{
	reject();
}

void PinsettingDialog::rwtypechanged(int index)
{
}

void PinsettingDialog::datatypechanged(int index)
{
	switch (ui->var_type->currentIndex())
	{
	case twinCATParam::PinType::BOOL_TYPE:
	case twinCATParam::PinType::DOUBLE_TYPE:
	case twinCATParam::PinType::INT_TYPE:
	case twinCATParam::PinType::QSTRING_TYPE:
		ui->x_label->setVisible(false);
		ui->y_label->setVisible(false);
		ui->a_label->setVisible(false);
		ui->y_name->setVisible(false);
		ui->a_name->setVisible(false);
		break;
	case twinCATParam::PinType::XY_TYPE:
		ui->x_label->setVisible(true);
		ui->y_label->setVisible(true);
		ui->a_label->setVisible(false);
		ui->y_name->setVisible(true);
		ui->a_name->setVisible(false);
		break;
	case twinCATParam::PinType::XYA_TYPE:
		ui->x_label->setVisible(true);
		ui->y_label->setVisible(true);
		ui->a_label->setVisible(true);
		ui->y_name->setVisible(true);
		ui->a_name->setVisible(true);
		break;
	default:
		break;
	}
}
