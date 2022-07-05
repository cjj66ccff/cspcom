#include "twinCATMod.h"
#include <QObject>
#include <ImageView.h>
#include <resultText.h>
#include "TcAdsDef.h"
#include "TcAdsAPI.h"
#include <process.h>
#define WRITE_INT "write_int"
#define WRITE_BOOL "write_bool"
#define WRITE_FLOAT "write_float"
#define WRITE_POINT2D "write_point2d"
#define WRITE_XYA "write_xya"
#define READ_INT "read_int"
#define READ_BOOL "read_bool"
#define READ_FLOAT "read_float"
#define READ_POINT2D "read_point2d"
#define READ_XYA "read_xya"

twinCATMod::twinCATMod()
{
	iniData();
}

twinCATMod::twinCATMod(const QString& dirPath)
{
	iniData();
	load(dirPath, StructLevel);
	createPins();
	int a = 0;
	bool b = false;
	float c = 1.0;
	printf("%d,%d,%d\n", m_param->pinsize, sizeof(c), sizeof(b));
	autorunthread = (HANDLE)_beginthreadex(NULL, 0, _autoRunThreadFun0, this, 0, &m_autoRunThread);
}

twinCATMod::~twinCATMod()
{
	delete m_param;
}

int twinCATMod::command(int cmdID, void* dataIn, void* dataOut)
{
	return 0;
}

int twinCATMod::afterSetProperty(MetaProperty*)
{
	return 0;
}

void twinCATMod::save(const QString &dirPath)
{
	m_param->readWriteParam(false, dirPath, StructLevel);
}

void twinCATMod::load(const QString &dirPath, QvsParamLevel level)
{
	m_param->readWriteParam(true,dirPath,level);
}

int twinCATMod::run(const QString &funName)
{
	unsigned long lHdlVar;	//创建句柄
	char          String[] = { "MAIN.String1" };	//定义字符串
	char          szVar[] = { "MAIN.String1" };
	QString var_name= "MAIN.String1";
	bool boolsize = true;
	int intsize = 1;
	float floatsize = 1.0;
	if (!m_connectTwinCAT) {
		return -1;
	}
	if (funName.contains("write")) {
		QString pinname = funName.split('_')[1];
		for (int i = 0; i < m_param->pinsize; i++)
		{
			if (m_param->pininfos[i]->Pinname == pinname) {
				for (int j = 0; j < m_allinputpins.size(); j++)
				{
					if ( m_allinputpins[j]->getID() == "write_" + pinname) {
						if(m_param->pininfos[i]->pintype==twinCATParam::BOOL_TYPE){
							auto tmppin = dynamic_cast<UnitInputPin<bool>*>(m_allinputpins.at(j));
							bool writedata = false;
							tmppin->getData(writedata);
							write_var(m_param->pininfos[i]->varname[0], sizeof(writedata), &writedata);
						}
						else if (m_param->pininfos[i]->pintype == twinCATParam::INT_TYPE) {
								auto tmppin = dynamic_cast<UnitInputPin<int>*>(m_allinputpins.at(j));
								int writedata = 0;
								tmppin->getData(writedata);
								unsigned short plcdata = writedata;
								write_var(m_param->pininfos[i]->varname[0], sizeof(plcdata), &plcdata);
						}
						else if (m_param->pininfos[i]->pintype == twinCATParam::DOUBLE_TYPE) {
							auto tmppin = dynamic_cast<UnitInputPin<double>*>(m_allinputpins.at(j));
							double writedata = 0;
							tmppin->getData(writedata);
							float writedataf = writedata;
							write_var(m_param->pininfos[i]->varname[0], sizeof(writedataf), &writedataf);
						}
						else if (m_param->pininfos[i]->pintype == twinCATParam::QSTRING_TYPE) {
							auto tmppin = dynamic_cast<UnitInputPin<QString>*>(m_allinputpins.at(j));
							QString writedata = "";
							tmppin->getData(writedata);
							write_var(m_param->pininfos[i]->varname[0], writedata.size(), (void*)(writedata.toStdString().data()));
						}
						else if (m_param->pininfos[i]->pintype == twinCATParam::XY_TYPE) {
							auto tmppin = dynamic_cast<UnitInputPin<cv::Point2d>*>(m_allinputpins.at(j));
							cv::Point2d writedata;
							tmppin->getData(writedata);
							float x = writedata.x;
							float y = writedata.y;
							write_var(m_param->pininfos[i]->varname[0], sizeof(float), &x);
							write_var(m_param->pininfos[i]->varname[1], sizeof(float), &y);
						}
						else if (m_param->pininfos[i]->pintype == twinCATParam::QSTRING_TYPE) {
							auto tmppin = dynamic_cast<UnitInputPin<XYA>*>(m_allinputpins.at(j));
							XYA writedata;
							tmppin->getData(writedata);
							float x = writedata.x;
							float y = writedata.y;
							float a = writedata.a;
							write_var(m_param->pininfos[i]->varname[0], sizeof(float), &x);
							write_var(m_param->pininfos[i]->varname[1], sizeof(float), &y);
							write_var(m_param->pininfos[i]->varname[2], sizeof(float), &a);
						}
					}
				}
			}
		}
	}
	else if (funName.contains("read")) {
		QString pinname = funName.split('_')[1];
		for (int i = 0; i < m_param->pinsize; i++)
		{
			if (m_param->pininfos[i]->Pinname == pinname) {
				for (int j = 0; j < m_alloutputpins.size(); j++)
				{
					if (m_alloutputpins[j]->getID() == "read_" + pinname) {
						if (m_param->pininfos[i]->pintype == twinCATParam::BOOL_TYPE) {
							auto tmppin = dynamic_cast<UnitOutputPin<bool>*>(m_alloutputpins.at(j));
							bool writedata = false;
							read_var(m_param->pininfos[i]->varname[0], sizeof(writedata), &writedata);
							tmppin->setData(writedata);
							printf("%d\n", writedata);
						}
						else if (m_param->pininfos[i]->pintype == twinCATParam::INT_TYPE) {
							auto tmppin = dynamic_cast<UnitOutputPin<int>*>(m_alloutputpins.at(j));
							unsigned short writedata = 0;
							read_var(m_param->pininfos[i]->varname[0], sizeof(writedata), &writedata);
							tmppin->setData(writedata);
						}
						else if (m_param->pininfos[i]->pintype == twinCATParam::DOUBLE_TYPE) {
							auto tmppin = dynamic_cast<UnitOutputPin<double>*>(m_alloutputpins.at(j));
							float writedata = 0;
							read_var(m_param->pininfos[i]->varname[0], sizeof(writedata), &writedata);
							tmppin->setData(writedata);
						}
						else if (m_param->pininfos[i]->pintype == twinCATParam::QSTRING_TYPE) {
							auto tmppin = dynamic_cast<UnitOutputPin<QString>*>(m_alloutputpins.at(j));
							QString writedata = "";
							writedata.fill(0, 512);
							read_var(m_param->pininfos[i]->varname[0], writedata.size(), writedata.data());
							tmppin->setData(writedata);
						}
						else if (m_param->pininfos[i]->pintype == twinCATParam::XY_TYPE) {
							auto tmppin = dynamic_cast<UnitOutputPin<cv::Point2d>*>(m_alloutputpins.at(j));
							cv::Point2d writedata;
							float x = 0;
							float y = 0;
							read_var(m_param->pininfos[i]->varname[0], sizeof(float), &x);
							read_var(m_param->pininfos[i]->varname[1], sizeof(float), &y);
							writedata.x = x;
							writedata.y = y;
							tmppin->getData(writedata);
						}
						else if (m_param->pininfos[i]->pintype == twinCATParam::QSTRING_TYPE) {
							auto tmppin = dynamic_cast<UnitOutputPin<XYA>*>(m_alloutputpins.at(j));
							XYA writedata;
							float x = 0;
							float y = 0;
							float a = 0;
							read_var(m_param->pininfos[i]->varname[0], sizeof(float), &x);
							read_var(m_param->pininfos[i]->varname[1], sizeof(float), &y);
							read_var(m_param->pininfos[i]->varname[2], sizeof(float), &a);
							writedata.x = x;
							writedata.y = y;
							writedata.a = a;
							tmppin->setData(writedata);
						}
					}
				}
			}
		}
	}
	else if(funName =="updateall")
	{
		readallpins();
		writeallpins();
	}
	else if (funName == "updatein")
	{
		writeallpins();

	}
	else if(funName =="updateout")
	{
		readallpins();
	}
	return 0;
}

void twinCATMod::viewResult(ImageView *iv, const QString &funName, int)
{

}

void twinCATMod::textResult(ResultText *text, const QString &funName)
{

}

void twinCATMod::iniData()
{
	m_param = new twinCATParam;
	m_connectTwinCAT = false;
}

void twinCATMod::createPins()
{
	addPin(&m_output_bool_value, "status", "status");
	addFunction("updateall");
	addFunction("updatein");
	addFunction("updateout");
	for (int i = 0; i < m_param->pinsize; i++)
	{
		twinCATParam::Pininfo *pininfo = m_param->pininfos[i];
		switch (pininfo->rwtype)
		{
		case 0:
			addoutputpin(*pininfo);
			break;
		case 1:
			addinputpin(*pininfo);
			break;
		case 2:
			addoutputpin(*pininfo);
			addinputpin(*pininfo);
			break;
		default:
			break;
		}
	}
	for (int i = 0; i < m_param->triggerssize; i++)
	{
		twinCATParam::Eventinfo newpin = m_param->eventinfos[i];
		{
			if (!eventNames().contains(newpin.eventname))
				addEvent(newpin.eventname);
			m_eventinfos.append(newpin);
		}

	}
	connectplc();
	//m_pAddr->port = m_param->port;	//TC3通讯使用的为851端口
	//nPort = AdsPortOpen();	//打开ADS通讯端口
	//int nErr = AdsGetLocalAddress((PAmsAddr)m_pAddr);
	//for (int i = 0; i < 6; i++)
	//{
	//	m_pAddr->netId.b[i] = m_param->netid[i];  //手动填写目标设备的AMSNETID

	//}
	//if (nErr){
	//	m_connectTwinCAT = false;
	//	m_output_bool_value.setData(false);
	//}
	//else
	//{
	//	m_connectTwinCAT = true;
	//	m_output_bool_value.setData(true);
	//}
	//m_pAddr->netId.b[0] = 192;
	//m_pAddr->netId.b[1] = 168; //在例子中设备通讯AMSNETID为192.168.233.1.2.1
	//m_pAddr->netId.b[2] = 233;
	//m_pAddr->netId.b[3] = 1;
	//m_pAddr->netId.b[4] = 3;
	//m_pAddr->netId.b[5] = 1;
	nErr = AdsGetLastError();
}

void twinCATMod::renamePins()
{
}

void twinCATMod::addinputpin(twinCATParam::Pininfo &pininfo)
{
	void* tmp_pin = NULL;
	switch (pininfo.pintype)
	{
	case twinCATParam::BOOL_TYPE:
		tmp_pin = new UnitInputPin<bool>;
		break;
	case twinCATParam::INT_TYPE:
		tmp_pin = new UnitInputPin<int>;
		break;
	case twinCATParam::DOUBLE_TYPE:
		tmp_pin = new UnitInputPin<double>;
		break;
	case twinCATParam::QSTRING_TYPE:
		tmp_pin = new UnitInputPin<QString>;
		break;
	case twinCATParam::XY_TYPE:
		tmp_pin = new UnitInputPin<cv::Point2d>;
		break;
	case twinCATParam::XYA_TYPE:
		tmp_pin = new UnitInputPin<XYA>;
		break;

	default:
		break;
	}
	addFunction("write_" + pininfo.Pinname);
	//m_metainpins->setText(var_name);
	//m_metainpins->setID(var_name);
	insertPin(0, (MetaPin*)tmp_pin, "write_" + pininfo.Pinname);
	m_allinputpins.append((MetaInputPin*)tmp_pin);
}

void twinCATMod::addoutputpin(twinCATParam::Pininfo &pininfo)
{
	void* tmp_pin = NULL;
	switch (pininfo.pintype)
	{
	case twinCATParam::BOOL_TYPE:
		tmp_pin = new UnitOutputPin<bool>;
		break;
	case twinCATParam::INT_TYPE:
		tmp_pin = new UnitOutputPin<int>;
		break;
	case twinCATParam::DOUBLE_TYPE:
		tmp_pin = new UnitOutputPin<double>;
		break;
	case twinCATParam::QSTRING_TYPE:
		tmp_pin = new UnitOutputPin<QString>;
		break;
	case twinCATParam::XY_TYPE:
		tmp_pin = new UnitOutputPin<cv::Point2d>;
		break;
	case twinCATParam::XYA_TYPE:
		tmp_pin = new UnitOutputPin<XYA>;
		break;

	default:
		break;
	}
	addFunction("read_" + pininfo.Pinname);
	//m_metainpins->setText(var_name);
	//m_metainpins->setID(var_name);
	insertPin(0, (MetaPin*)tmp_pin, "read_" + pininfo.Pinname);
	m_alloutputpins.append((MetaOutputPin*)tmp_pin);
}

void twinCATMod::readallpins()
{
	for (int i = 0; i < m_param->pinsize; i++)
	{
		QString pinname = m_param->pininfos[i]->Pinname;
		{
			for (int j = 0; j < m_alloutputpins.size(); j++)
			{
				if (m_alloutputpins[j]->getID() == "read_" + pinname) {
					if (m_param->pininfos[i]->pintype == twinCATParam::BOOL_TYPE) {
						auto tmppin = dynamic_cast<UnitOutputPin<bool>*>(m_alloutputpins.at(j));
						bool writedata = false;
						read_var(m_param->pininfos[i]->varname[0], sizeof(writedata), &writedata);
						tmppin->setData(writedata);
					}
					else if (m_param->pininfos[i]->pintype == twinCATParam::INT_TYPE) {
						auto tmppin = dynamic_cast<UnitOutputPin<int>*>(m_alloutputpins.at(j));
						int writedata = 0;
						read_var(m_param->pininfos[i]->varname[0], sizeof(writedata), &writedata);
						tmppin->setData(writedata);
					}
					else if (m_param->pininfos[i]->pintype == twinCATParam::DOUBLE_TYPE) {
						auto tmppin = dynamic_cast<UnitOutputPin<double>*>(m_alloutputpins.at(j));
						float writedata = 0;
						read_var(m_param->pininfos[i]->varname[0], sizeof(writedata), &writedata);
						tmppin->setData(writedata);
					}
					else if (m_param->pininfos[i]->pintype == twinCATParam::QSTRING_TYPE) {
						auto tmppin = dynamic_cast<UnitOutputPin<QString>*>(m_alloutputpins.at(j));
						QString writedata = "";
						read_var(m_param->pininfos[i]->varname[0], sizeof(writedata), &writedata);
						tmppin->setData(writedata);
					}
					else if (m_param->pininfos[i]->pintype == twinCATParam::XY_TYPE) {
						auto tmppin = dynamic_cast<UnitOutputPin<cv::Point2d>*>(m_alloutputpins.at(j));
						cv::Point2d writedata;
						float x = 0;
						float y = 0;
						read_var(m_param->pininfos[i]->varname[0], sizeof(float), &x);
						read_var(m_param->pininfos[i]->varname[1], sizeof(float), &y);
						writedata.x = x;
						writedata.y = y;
						tmppin->getData(writedata);
					}
					else if (m_param->pininfos[i]->pintype == twinCATParam::QSTRING_TYPE) {
						auto tmppin = dynamic_cast<UnitOutputPin<XYA>*>(m_alloutputpins.at(j));
						XYA writedata;
						float x = 0;
						float y = 0;
						float a = 0;
						read_var(m_param->pininfos[i]->varname[0], sizeof(float), &x);
						read_var(m_param->pininfos[i]->varname[1], sizeof(float), &y);
						read_var(m_param->pininfos[i]->varname[2], sizeof(float), &a);
						writedata.x = x;
						writedata.y = y;
						writedata.a = a;
						tmppin->setData(writedata);
					}
				}
			}
		}
	}
}

void twinCATMod::writeallpins()
{
	for (int i = 0; i < m_param->pinsize; i++)
	{
		QString pinname = m_param->pininfos[i]->Pinname;
		{
			for (int j = 0; j < m_allinputpins.size(); j++)
			{
				if (m_allinputpins[j]->getID() == "write_" + pinname) {
					if (m_param->pininfos[i]->pintype == twinCATParam::BOOL_TYPE) {
						auto tmppin = dynamic_cast<UnitInputPin<bool>*>(m_allinputpins.at(j));
						bool writedata = false;
						tmppin->getData(writedata);
						write_var(m_param->pininfos[i]->varname[0], sizeof(writedata), &writedata);
					}
					else if (m_param->pininfos[i]->pintype == twinCATParam::INT_TYPE) {
						auto tmppin = dynamic_cast<UnitInputPin<int>*>(m_allinputpins.at(j));
						int writedata = 0;
						tmppin->getData(writedata);
						write_var(m_param->pininfos[i]->varname[0], sizeof(writedata), &writedata);
					}
					else if (m_param->pininfos[i]->pintype == twinCATParam::DOUBLE_TYPE) {
						auto tmppin = dynamic_cast<UnitInputPin<double>*>(m_allinputpins.at(j));
						double writedata = 0;
						tmppin->getData(writedata);
						float writedataf = writedata;
						write_var(m_param->pininfos[i]->varname[0], sizeof(writedataf), &writedataf);
					}
					else if (m_param->pininfos[i]->pintype == twinCATParam::QSTRING_TYPE) {
						auto tmppin = dynamic_cast<UnitInputPin<QString>*>(m_allinputpins.at(j));
						QString writedata = "";
						tmppin->getData(writedata);
						write_var(m_param->pininfos[i]->varname[0], writedata.size() + 1, (void*)(writedata.toStdString().data()));
					}
					else if (m_param->pininfos[i]->pintype == twinCATParam::XY_TYPE) {
						auto tmppin = dynamic_cast<UnitInputPin<cv::Point2d>*>(m_allinputpins.at(j));
						cv::Point2d writedata;
						tmppin->getData(writedata);
						float x = writedata.x;
						float y = writedata.y;
						write_var(m_param->pininfos[i]->varname[0], sizeof(float), &x);
						write_var(m_param->pininfos[i]->varname[1], sizeof(float), &y);
					}
					else if (m_param->pininfos[i]->pintype == twinCATParam::QSTRING_TYPE) {
						auto tmppin = dynamic_cast<UnitInputPin<XYA>*>(m_allinputpins.at(j));
						XYA writedata;
						tmppin->getData(writedata);
						float x = writedata.x;
						float y = writedata.y;
						float a = writedata.a;
						write_var(m_param->pininfos[i]->varname[0], sizeof(float), &x);
						write_var(m_param->pininfos[i]->varname[1], sizeof(float), &y);
						write_var(m_param->pininfos[i]->varname[2], sizeof(float), &a);
					}
				}
			}
		}
	}
}



void twinCATMod::write_var(QString varname, int varsize, void * pdata)
{

	unsigned long lHdlVar;
	nErr = AdsSyncReadWriteReq((PAmsAddr)m_pAddr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(lHdlVar), &lHdlVar, varname.size() + 1, (void*)(varname.toStdString().data()));
	if (nErr) {
		printf("gethdlfail,errorcode:%d\n", nErr);
	}
	nErr = AdsSyncWriteReq((PAmsAddr)m_pAddr, ADSIGRP_SYM_VALBYHND, lHdlVar, varsize, pdata);
	if (nErr) {
		printf("writefail,errorcode:%d,varsize:%d\n", nErr,varsize);
	}
}

void twinCATMod::read_var(QString varname, int varsize, void * pdata)
{
	unsigned long lHdlVar;
	nErr = AdsSyncReadWriteReq((PAmsAddr)m_pAddr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(lHdlVar), &lHdlVar, varname.size() + 1, (void*)(varname.toStdString().data()));
	if (nErr) {
		printf("get%shdlfail,errorcode:%d\n", varname.toStdString().c_str(),nErr);
	}
	nErr = AdsSyncReadReq((PAmsAddr)m_pAddr, ADSIGRP_SYM_VALBYHND, lHdlVar, varsize, pdata);
	if (nErr) {
		printf("readfail,errorcode:%d,varsize:%d\n", nErr, varsize);
	}
}

void twinCATMod::update50ms()
{
	while(1){
		if (m_connectTwinCAT) {
			for (int i = 0; i < m_param->pinsize; i++)
			{
				//printf("%d\n", i);
				if (m_param->pininfos[i]->pintype == twinCATParam::BOOL_TYPE) {
					if (m_param->pininfos[i]->eventname[0].size() > 0 || m_param->pininfos[i]->eventname[1].size() > 0) {
						QString pinname = m_param->pininfos[i]->Pinname;
						for (int j = 0; j < m_alloutputpins.size(); j++)
						{
							if (m_alloutputpins[j]->getID() == "read_" + pinname) {
								{
									auto tmppin = dynamic_cast<UnitOutputPin<bool>*>(m_alloutputpins.at(j));
									bool olddata = false;
									tmppin->getData(olddata);
									bool writedata = false;
									read_var(m_param->pininfos[i]->varname[0], sizeof(writedata), &writedata);
									if (olddata && (!writedata) && m_param->pininfos[i]->eventname[1].size() > 0) {
										triggerEvent(m_param->pininfos[i]->eventname[1]);
										printf("trigger %s fal\n", m_param->pininfos[i]->eventname[1].toStdString().c_str());
									}
									else if (writedata && (!olddata) && m_param->pininfos[i]->eventname[0].size() > 0) {
										triggerEvent(m_param->pininfos[i]->eventname[0]);
										printf("trigger %s raise\n", m_param->pininfos[i]->eventname[0].toStdString().c_str());
									}
									tmppin->setData(writedata);
								}
							}
						}

					}
				}

			}
		}
		Sleep(50);
	}
}

void twinCATMod::connectplc()
{
	m_pAddr = new AmsAddr();
	if (m_connectTwinCAT) {
		AdsPortClose();
	}
	((AmsAddr*)m_pAddr)->port = m_param->port;	//TC3通讯使用的为851端口
	int nErr = AdsGetLocalAddress((PAmsAddr)m_pAddr);
	nPort = AdsPortOpen();	//打开ADS通讯端口
	for (int i = 0; i < 6; i++)
	{
		((AmsAddr*)m_pAddr)->netId.b[i] = m_param->netid[i];  //手动填写目标设备的AMSNETID

	}
	USHORT   nAdsState;	//包含PLC的状态信息
	USHORT        nDeviceState;

	nErr = AdsSyncReadStateReq((PAmsAddr)m_pAddr, &nAdsState, &nDeviceState);
	if (!nErr){
		m_connectTwinCAT = true;
		m_output_bool_value.setData(true);
	}
	else{
		m_connectTwinCAT = false;
		m_output_bool_value.setData(false);
	}
}

UINT twinCATMod::_autoRunThreadFun0(void * arg)
{
	twinCATMod *p = (twinCATMod*)arg;
	p->update50ms();
	return 0;
}

void twinCATMod::delpin(twinCATParam::Pininfo &pininfo)
{
	for (int i = 0; i < m_allinputpins.size(); i++)
	{
		if (m_allinputpins[i]->getID() == "write_" + pininfo.Pinname) {
			m_allinputpins.removeAt(i);
		}
	}
	for (int i = 0; i < m_alloutputpins.size(); i++)
	{
		if (m_alloutputpins[i]->getID() == "read_" + pininfo.Pinname) {
			m_alloutputpins.removeAt(i);
		}
	}
	switch (pininfo.rwtype)
	{
	case 0:
		removePin("read_" + pininfo.Pinname, true);
		break;
	case 1:
		removePin("write_" + pininfo.Pinname, true);
		
		break;
	case 2:
		removePin("read_" + pininfo.Pinname, true);
		removePin("write_" + pininfo.Pinname, true);
		break;
	default:
		break;
	}
}

void twinCATMod::addpin(twinCATParam::Pininfo &pininfo)
{
	switch (pininfo.rwtype)
	{
	case 0:
		addoutputpin(pininfo);
		break;
	case 1:
		addinputpin(pininfo);
		break;
	case 2:
		addoutputpin(pininfo);
		addinputpin(pininfo);
		break;
	default:
		break;
	}
}

