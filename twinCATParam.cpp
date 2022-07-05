#include "twinCATParam.h"
#include <iniParam.h>


twinCATParam::twinCATParam()
{
	iniData();
}

twinCATParam::~twinCATParam()
{

}

void twinCATParam::readWriteParam(bool r, const QString& dirPath, int level)
{
	SETTING_INIT(dirPath + "/twinCAT.ini");
	RW_VALUE(r, pinsize);
	RW_VALUE(r, triggerssize);
	RW_VALUE(r, port);
	for (int i = 0; i < 6; i++)
	{
		RW_VALUE1(r, netid[i], i);
	}
	for (int i = 0; i < pinsize; i++)
	{
		if (r)
			pininfos[i] = new Pininfo;
		RW_VALUE1(r, pininfos[i]->Pinname, i);
		RW_VALUE1(r, pininfos[i]->pintype, i);
		RW_VALUE1(r, pininfos[i]->rwtype, i);
		RW_VALUE2(r, pininfos[i]->varname[0], i,0);
		switch (pininfos[i]->pintype)
		{
		case XY_TYPE:
			RW_VALUE2(r, pininfos[i]->varname[1], i, 1);
			break;
		case XYA_TYPE:
			RW_VALUE2(r, pininfos[i]->varname[1], i, 1);
			RW_VALUE2(r, pininfos[i]->varname[2], i, 2);
			break;
		default:
			break;
		}
		RW_VALUE2(r, pininfos[i]->eventname[0], i, 0);
		RW_VALUE2(r, pininfos[i]->eventname[1], i, 1);
	}
	if (r)
		eventinfos.clear();
	for (int i = 0; i < triggerssize; i++)
	{
		Eventinfo tmp;
		if (!r)
			tmp = eventinfos[i];
		RW_VALUE1(r, tmp.pinname, i);
		RW_VALUE1(r, tmp.triggertype, i);
		RW_VALUE1(r, tmp.eventname, i);
		if (r)
			eventinfos.append(tmp);
	}
}

void twinCATParam::iniData()
{
	pinsize = 0;
	port = 851;
	pininfos = new Pininfo*[512];
	triggerssize = 0;
	for (int i = 0; i < 6; i++)
	{
		netid[i]=i;
	}
}

