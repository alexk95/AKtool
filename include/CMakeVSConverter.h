#pragma once

// Qt header
#include <qstring.h>
#include <qstringlist.h>

namespace CMakeVSConverter {

	class VS_CXX_Project {
	public:
		VS_CXX_Project();

		bool setFromProjectFile(const QByteArray& _fileContent);

	private:
		QString			m_projectName;
		QStringList		m_sourceFiles;

	};

}