#include "SceneManager.h"
#include "Editor.h"


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
#ifdef HAS_EDITOR
	Editor::instance()->SetSelectedEntity(nullptr);
#endif // HAS_EDITOR

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
