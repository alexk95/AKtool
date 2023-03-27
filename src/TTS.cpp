#include "TTS.h"
#include "AppBase.h"
#include "Settings.h"

#include <QtCore/qvector.h>
#include <QtTextToSpeech/qvoice.h>
#include <QtTextToSpeech/qtexttospeech.h>

TTS& TTS::instance() {
	static TTS g_instance;
	return g_instance;
}

void TTS::say(const QString& _text) {
	if (m_engine) m_engine->say(_text);
}

TTS::TTS() 
	: m_engine(nullptr)
{
	QStringList availableEngines = QTextToSpeech::availableEngines();
	if (availableEngines.empty()) return;

	AK_APP->appendOutput("#######################################\n");
	AK_APP->appendOutput("Setting up TTS...\n\n");

	AK_APP->appendOutput("Available TTS Engines:\n");
	for (auto eng : availableEngines) {
		AK_APP->appendOutput("\t" + eng + "\n");
	}
	m_engine = new QTextToSpeech;
	//m_engine->setVolume(0.5);

	AK_APP->appendOutput("\nAvailable TTS Voices:\n");
	for (auto v : m_engine->availableVoices()) {
		AK_APP->appendOutput("\t" + v.name() + "\n");
	}
	m_engine->setVoice(m_engine->availableVoices()[0]);

	AK_APP->appendOutput("#######################################\n");
	AK_APP->appendOutput("\nTTS Ready\n");
}

TTS::~TTS() {
	delete m_engine;
}