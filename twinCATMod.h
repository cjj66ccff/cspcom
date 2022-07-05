#pragma once
#include <moduleClass.h>
#include <imageModule.h>
#include <Windows.h>
#include "twinCATParam.h"


using namespace qzcv;
//typedef struct AmsAddr;
//typedef struct AdsNotificationAttrib;
class twinCATMod : public UnitModule
{
public:
	explicit twinCATMod();
	explicit twinCATMod(const QString&);
	~twinCATMod();
	int command(int cmdID, void* dataIn, void* dataOut);
	virtual int afterSetProperty(MetaProperty*);
	void delpin(twinCATParam::Pininfo &pininfo);
	void addpin(twinCATParam::Pininfo &pininfo);
protected:
	virtual void save(const QString& dirPath);
	virtual void load(const QString& dirPath, QvsParamLevel level);
	virtual int run(const QString& funName);
	virtual void viewResult(ImageView* iv, const QString& funName,int);
	virtual void textResult(ResultText* text, const QString& funName);
private:
	void iniData();
	void createPins();
	void renamePins();
	void addinputpin(twinCATParam::Pininfo &pininfo);
	void addoutputpin(twinCATParam::Pininfo &pininfo);
	void readallpins();
	void writeallpins();
	void write_var(QString varname,int varsize, void *pdata);
	void read_var(QString varname, int varsize, void *pdata);
	void update50ms();
	void connectplc();
	static UINT WINAPI _autoRunThreadFun0(void *arg);
	HANDLE autorunthread;
	UINT m_autoRunThread;
private:
	twinCATParam* m_param;
	friend class twinCATWdg;
private:
	UnitInputPin<float> m_input_float_value;
	//UnitInputPin<float> *m_input_float;
	UnitInputPin<int> m_input_int_value;
	UnitInputPin<bool> m_input_bool_value;
	UnitInputPin<XYA> m_input_xya_value;
	UnitInputPin<cv::Point2d> m_input_point2d_value;
	UnitInputPin<QString> m_write_var_name;
	UnitInputPin<QString> m_read_var_name;
	UnitOutputPin<float> m_output_float_value;
	UnitOutputPin<int> m_output_int_value;
	UnitOutputPin<bool> m_output_bool_value;
	UnitOutputPin<XYA> m_output_xya_value;
	UnitOutputPin<cv::Point2d> m_output_point2d_value;
	MetaInputPin *m_metainpins;
	QList<MetaPin*> m_allpins;
	QList<MetaInputPin*> m_allinputpins;
	QList<MetaOutputPin*> m_alloutputpins;
	QList<twinCATParam::Eventinfo> m_eventinfos;
	bool m_connectTwinCAT;
	long      nErr, nPort;	//定义端口变量
	//AmsAddr   *m_Addr;		//定义AMS地址变量
	void*  m_pAddr;// = &m_Addr;//定义端口地址变量
	void* adsNotificationAttrib;//为adsNotificationAttrib包含的所有属性定义一个通知

};

