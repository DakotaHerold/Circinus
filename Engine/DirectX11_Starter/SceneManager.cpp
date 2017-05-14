#include "SceneManager.h"



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

Scene * SceneManager::LoadScene(string path)
{

	return nullptr;
}

Scene * SceneManager::SaveScene(Scene * scene)
{
	Document d;
	string filepath = scene->GetName() + ".json";
	FILE* fp = fopen(filepath.c_str(), "wb");
	char writeBuffer[65536];
	FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	Writer<FileWriteStream> writer(os);
	d.Accept(writer);
	fclose(fp);
	return nullptr;
}
