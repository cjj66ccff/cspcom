#pragma once
#include <moduleClass.h>
const char* const FactoryName = "twinCAT";
using namespace qzcv;

class twinCATFact : public ModuleFactory
{
public:
	explicit twinCATFact();
	virtual ~twinCATFact();
	virtual unsigned int classID()const { return 21293219; }
	virtual const QIcon* icon()const { return NULL; }
	virtual UnitModule* createModule();
	virtual UnitModule* createModule(const QString& dirPath);
	virtual QStringList getModSetWidgetStyles();
	virtual ModSetWidget* createModSetWidget();
	virtual ModSetWidget* createModSetWidget(const QString& style);
private:
	QStringList m_styleList;
};

