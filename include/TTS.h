#pragma once

// Qt header
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>

#define AK_TTS TTS::instance()

class QTextToSpeech;
class TTS {
public:
	static TTS& instance();

	void say(const QString& _text);

private:
	QTextToSpeech *	m_engine;

	TTS();
	virtual ~TTS();
};