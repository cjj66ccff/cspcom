#pragma once
#include <QString>
#include <QList>

class twinCATParam
{
public:
	explicit twinCATParam();
	~twinCATParam();
	virtual void readWriteParam(bool r, const QString& dirPath, int level);
	int port;
	int netid[6];
	enum RWType
	{
		READ_ONLY,
		WRITE_ONLY,
		RW
	};
	enum PinType
	{
		BOOL_TYPE,
		INT_TYPE,
		DOUBLE_TYPE,
		QSTRING_TYPE,
		XY_TYPE,
		XYA_TYPE
	};
	enum TriggerType
	{
		TriggerType_Raising = 0, TriggerType_Falling
	};
	struct Pininfo
	{
		Pininfo() :
			Pinname("newpin"), pintype(0), rwtype(0)
		{}
		QString Pinname;
		int pintype;
		int rwtype;
		QString varname[3];
		QString eventname[2];
	};
	struct Eventinfo
	{
		Eventinfo() :
			triggertype(0) 
		{};
		bool operator ==(Eventinfo info1){
			if (pinname == info1.pinname&&triggertype == info1.triggertype) {
				return true;
			}
			else
			{
				return false;
			}
		};
		QString pinname;
		QString eventname;
		int triggertype;
	};
	typedef struct _TriggerInfo
	{
		_TriggerInfo() :
			triggerType(0), triggerIndex(-1), triggerHalPinIndex(-1)
		{}
		int triggerType;
		int triggerIndex;
		int triggerHalPinIndex;
		QString triggerPinName;
		QString triggerEventName;
	}TriggerInfo;
	Pininfo **pininfos;
	TriggerInfo **triggerinfos;
	QList<Eventinfo> eventinfos;
	int pinsize;
	int triggerssize;
private:
	void iniData();
};

