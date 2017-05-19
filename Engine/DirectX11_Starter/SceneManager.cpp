#include "SceneManager.h"
#include "Editor.h"
#include "GUI.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

Scene * SceneManager::CreateNewScene(string name)
{
	return new Scene();
}

Scene * SceneManager::LoadScene(string name)
{
	if (name.empty())
		return nullptr;

#ifdef HAS_EDITOR
	Editor::instance()->OnSceneLoad();
#endif // HAS_EDITOR
#ifdef HAS_GUI
	GUI::instance().OnSceneLoad();
#endif

	Scene* s = CreateNewScene(name);
	string file = "Assets/" + name + ".scene";
	FILE* fp = fopen(file.c_str(), "rb");
	char readBuffer[65536];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	Document d;
	d.ParseStream(is);	
	s->Build(d);
	fclose(fp);
	return s;
}

Scene * SceneManager::SaveScene(Scene * scene)
{
#ifdef HAS_EDITOR
	if (Editor::instance()->IsPlaying())
		return nullptr;
#endif
	StringBuffer sb;
	PrettyWriter<StringBuffer> writer(sb);
	scene->Serialize(writer);
	string str(sb.GetString(), sb.GetLength());
	ofstream outfile;
	string file = "Assets/" + scene->GetName() + ".scene";
	outfile.open(file.c_str());
	if (outfile.fail()) {
		return nullptr;
	}	
	outfile << str;
	outfile.close();	
	return nullptr;
}
