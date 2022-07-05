#include "twinCATFact.h"
#include "twinCATWdg.h"
#include "twinCATMod.h"

twinCATFact :: twinCATFact()
	: ModuleFactory(FactoryName, 0)
{

	getModSetWidgetStyles();
}

twinCATFact :: ~twinCATFact()
{

}

qzcv::UnitModule* twinCATFact::createModule()
{
	return new twinCATMod;
}

qzcv::UnitModule* twinCATFact::createModule(const QString& dirPath)
{
	return new twinCATMod(dirPath);
}

QStringList twinCATFact::getModSetWidgetStyles()
{
	m_styleList.clear();

	return m_styleList;
}

qzcv::ModSetWidget* twinCATFact::createModSetWidget()
{
	return new twinCATWdg;
}

qzcv::ModSetWidget* twinCATFact::createModSetWidget(const QString &style)
{
	for(int i=0;i<m_styleList.size();i++)
	{
		if(style==m_styleList.at(i))
			return new twinCATWdg(i);
	}
	return new twinCATWdg;
}

extern "C" Q_DECL_EXPORT int getFactoryCnt()
{
	return 1;
}

extern "C" Q_DECL_EXPORT void* createModuleFactory(int index)
{
	if(0==index)
	{
		return new twinCATFact;
	}
	else
		return NULL;
}

extern "C" Q_DECL_EXPORT int isDebug() {
#if defined(_DEBUG)||defined(QT_DEBUG)
	return 1;
#else
	return 0;
#endif
}

