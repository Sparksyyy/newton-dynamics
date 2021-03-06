// BindingTemplate.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <dTree.h>
#include "NewtonBinding.h"

#define targetPath			"./"


class ObjectiveCBinding: public NewtonBinding
{
	public:
	ObjectiveCBinding(const char* const outputPath)
		:NewtonBinding (outputPath)
		,m_tabsStack(0)
		,m_callbackMode(true)
	{
		char path[1024];
		strcpy (path, outputPath);
		strrchr (path, '/')[0] = 0;
		strcat (path, "/iosNewton.h");
		m_objectiveCHeader = fopen (path, "w");

//		strcpy (path, targetPath);
//		strrchr (path, '/')[0] = 0;
//		strcat (path, "/iNewton.mm");
//		iNewtonSource = fopen (path, "w");
//		fprintf (iNewtonSource, "%s", license);
//		fprintf (iNewtonHeader, "%s", license);
//		fprintf (iNewtonHeader, "#import <Foundation/Foundation.h>\n\n\n");

		Parse();
		fprintf (m_objectiveCHeader, "@end\n");
	}

	~ObjectiveCBinding()
	{
		fclose (m_objectiveCHeader);
	}

	void BlockEnd ()
	{
		fprintf (m_objectiveCHeader, "\n");
	}

	void EngineVersion (const string& mayor, const string& minor)
	{
		fprintf (m_objectiveCHeader, m_licence);
		fprintf (m_objectiveCHeader, "#import <Foundation/Foundation.h>\n");
		BlockEnd ();

		fprintf (m_objectiveCHeader, "//this objective C class was automatically generated by the newton binding tool\n");
		BlockEnd ();

		fprintf (m_objectiveCHeader, "#define NEWTON_MAJOR_VERSION %s\n", mayor.c_str());
		fprintf (m_objectiveCHeader, "#define NEWTON_MINOR_VERSION %s\n", minor.c_str());
	}

	void DeclareDataType (const string& singlePrecision, const string& doublePrecision)
	{
		fprintf (m_objectiveCHeader, "#define dFloat %s\n", singlePrecision.c_str());
		fprintf (m_objectiveCHeader, "#define dFloat64 %s\n", doublePrecision.c_str());
	}

	void ConstantDefinition (const string& constName, const string& value)
	{
		fprintf (m_objectiveCHeader, "#define %s %s\n", constName.c_str(), value.c_str());
	}

	void InternalEngineStruct (const string& structName)
	{
		m_symbolTable.Insert(structName + "*");
		fprintf (m_objectiveCHeader, "typedef struct ios%s{} ios%s;\n", structName.c_str(), structName.c_str());
	}

	void StructDeclareStart (const string& structName)
	{
		m_symbolTable.Insert(structName);
		fprintf (m_objectiveCHeader, "struct ios%s\n{\n", structName.c_str());
		m_tabs[m_tabsStack] = '\t';
		m_tabs[m_tabsStack + 1] = 0;
		m_tabsStack ++;
	}

	void StructNameLessUnion ()
	{
		fprintf (m_objectiveCHeader, "%s union\n", m_tabs);
		fprintf (m_objectiveCHeader, "%s {\n", m_tabs);
		m_tabs[m_tabsStack] = '\t';
		m_tabs[m_tabsStack + 1] = 0;
		m_tabsStack ++;
	}

	void StructAddDataType (const string& dataType, const string& dataName)
	{
		fprintf (m_objectiveCHeader, "%s %s %s;\n", m_tabs, dataType.c_str(), dataName.c_str());
	}

	void StructDeclareEnd ()
	{
		m_tabsStack --;
		m_tabs[m_tabsStack] = 0;
		fprintf (m_objectiveCHeader, "%s};\n", m_tabs);
	}

	void FunctionCallbackDeclaration (const string& returnType, const string& callbackName)
	{
		fprintf (m_objectiveCHeader, "typedef %s (*%s) (", returnType.c_str(), callbackName.c_str());
	}


	void FunctionDeclaration (const string& returnType, const string& callbackName)
	{
		if (m_callbackMode) {
			fprintf (m_objectiveCHeader, "@interface iosNewton: NSObject\n");
			fprintf (m_objectiveCHeader, "{\n");
			fprintf (m_objectiveCHeader, "\tvoid* m_world;\n");
			fprintf (m_objectiveCHeader, "}\n");
			fprintf (m_objectiveCHeader, "\n");
		}
		m_callbackMode = false;
		if (m_symbolTable.Find(returnType)) {
			fprintf (m_objectiveCHeader, "-(struct ios%s) %s", returnType.c_str(), callbackName.c_str());
		} else {
			fprintf (m_objectiveCHeader, "-(%s) %s", returnType.c_str(), callbackName.c_str());
		}
	}


	void FunctionArgumentSeparator ()
	{
		if (m_callbackMode) {
			fprintf (m_objectiveCHeader, ", ");
		}
	}

	void FunctionArgument (const string& argType, const string& name)
	{
		if (m_callbackMode) {
			fprintf (m_objectiveCHeader, "%s %s", argType.c_str(), name.c_str());
		} else {
			fprintf (m_objectiveCHeader, ": (%s) %s", argType.c_str(), name.c_str());
		}
	}

	void FunctionDeclarationEnd ()
	{
		if (m_callbackMode) {
			fprintf (m_objectiveCHeader, ");\n");
		} else {
			fprintf (m_objectiveCHeader, ";\n");
		}
	}

	bool m_callbackMode;
	int m_tabsStack;
	char m_tabs[32];
	FILE* m_objectiveCHeader;
	dTree<int, string> m_symbolTable;
//	FILE* m_newtonSource;
};


int _tmain(int argc, _TCHAR* argv[])
{
	ObjectiveCBinding binding (targetPath);
	return 0;
}

