#include "Editor.h"

#ifdef HAS_EDITOR

#include <string>
#include <Windows.h>
#include <ShlObj.h>

#ifdef UNICODE
typedef wchar_t char_t;
typedef std::wstring str_t;
#define STR(x) L##x
#else
typedef char char_t;
typedef std::string str_t;
#define STR(X) x
#endif

#include "RenderingSystem.h"
#include "InputManager.h"
#include "ComponentManager.h"
#include "Transform.h"
#include "Renderable.h"
#include "GizmoRenderer.h"
#include "Engine.h"
#include "Scene.h"

namespace
{

	const char_t engineExeName[] = STR("\\Engine.exe");
	const char_t dllsName[] = STR("\\*.dll");
	const char_t assetFolderName[] = STR("\\Assets");
	const char_t scriptFolderName[] = STR("\\Scripts");

	str_t SelectFolder()
	{
		char_t path[1024] = {};

		BROWSEINFO bi = {};
		bi.lpszTitle = STR("Output Folder");
		bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

		LPITEMIDLIST list = SHBrowseForFolder(&bi);
		if (nullptr != list)
		{
			SHGetPathFromIDList(list, path);

			IMalloc * imalloc = 0;
			if (SUCCEEDED(SHGetMalloc(&imalloc)))
			{
				imalloc->Free(list);
				imalloc->Release();
			}

			return path;
		}

		return STR("");
	}

	str_t GetWorkingDirectory()
	{
		char_t* cCwd = nullptr;
		uint32_t cwdLen = GetCurrentDirectory(0, nullptr);
		cCwd = new char_t[cwdLen];
		GetCurrentDirectory(cwdLen, cCwd);
		str_t cwd(cCwd);
		delete[] cCwd;

		return cwd;
	}

	void CopyFile(const str_t& from, const str_t& to)
	{
		str_t _from = from + STR('\0');
		str_t _to = to + STR('\0');
		SHFILEOPSTRUCT s = {};
		s.wFunc = FO_COPY;
		s.pFrom = _from.c_str();
		s.pTo = _to.c_str();
		s.fFlags = FOF_SILENT | FOF_NOCONFIRMMKDIR | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NO_UI;
		assert(0 == SHFileOperation(&s));
	}
}

Editor* Editor::_instance = nullptr;

void Editor::Init()
{
	assert(nullptr == _instance);
	_instance = this;

	cam.getViewMatrix();
	cam.setProjectionMatrix(800.0f / 600.0f);

	isPlaying = false;
}

void Editor::Update(float deltaTime, float totalTime)
{
	InputManager& input = InputManager::instance();

	// camera movement
	cam.update(deltaTime);

	if (!handle.IsMoving())
	{
		float x = input.GetMouseMoveX();
		if (input.GetMiddleMouseHeld())	// Left button is down
		{
			cam.moveSideways(input.GetMouseMoveX() *0.005f);
			cam.moveVertical(input.GetMouseMoveY() * 0.005f);
		}
		if (input.GetLeftMouseHeld())	// Right button is down
		{
			cam.setRotationY(input.GetMouseMoveX());
			cam.setRotationX(input.GetMouseMoveY());
		}

		cam.moveAlongDirection(input.GetMouseWheelDelta() * 0.01f);
	}

	{
		GizmoRenderer::opaque()->DrawGrid(
			DirectX::XMFLOAT3{ 10, 0, 0 },
			DirectX::XMFLOAT3{ 0, 0, 10 },
			DirectX::XMFLOAT3{ 0, 0, 0 },
			10, 10,
			DirectX::XMFLOAT3{ 0.3f, 0.3f, 0.3f }
		);
		GizmoRenderer::opaque()->DrawCoordinate(cam.getViewMatrix(), cam.getProjectionMatrix(), XMFLOAT3{ -0.8f, -0.8f, 0.5f }, 0.01f);
	}

	if (nullptr == Engine::instance()->GetCurScene())
		return;

	XMVECTOR mouseRayStart, mouseRayDirection, mouseWorldDelta;
	XMMATRIX matVP, matVP_Inv;

	// get mouse ray
	{
		const auto& frustum = cam.getFrustum();

		matVP = XMMatrixTranspose(XMLoadFloat4x4(&cam.getViewMatrix()));
		matVP = XMMatrixMultiply(
			matVP,
			XMMatrixTranspose(XMLoadFloat4x4(&cam.getProjectionMatrix()))
		);
		matVP_Inv = XMMatrixInverse(nullptr, matVP);

		float mouseClipSpaceX = input.GetMousePositionX() / RenderingSystem::instance()->GetWindowWidth() * 2.0f - 1.0f;
		float mouseClipSpaceY = -input.GetMousePositionY() / RenderingSystem::instance()->GetWindowHeight() * 2.0f + 1.0f;

		XMVECTOR mousePos = XMVectorSet(
			mouseClipSpaceX,
			mouseClipSpaceY,
			(frustum.Near == 0.0f ? 1.0f : frustum.Near),
			0
		);

		mousePos = XMVector3TransformCoord(mousePos, matVP_Inv)
			- XMVectorSet(0, 0, 0, 1);

		mouseRayStart = XMLoadFloat3(&cam.getPosition());
		mouseRayDirection = XMVector3Normalize(mousePos - mouseRayStart);

		mouseWorldDelta = XMVectorSet(
			input.GetMouseMoveX() * deltaTime,// / RenderingSystem::instance()->GetWindowWidth(),
			-input.GetMouseMoveY() * deltaTime,// / RenderingSystem::instance()->GetWindowHeight(),
			0,
			0
		);
	}

	// handle gizmos
	if (nullptr != selectedEntity)
	{
		auto r = selectedEntity->GetComponent<Renderable>();
		auto t = selectedEntity->GetComponent<Transform>();

		if (nullptr != r && nullptr != t)
		{
			XMMATRIX worldMat = XMMatrixTranspose(
				XMLoadFloat4x4(t->GetWorldMatrix())
			);

			BoundingOrientedBox box;
			BoundingOrientedBox::CreateFromBoundingBox(box, r->GetMesh()->GetBounds());
			box.Transform(box, worldMat);

			GizmoRenderer::opaque()->Draw(box, PrimitiveRenderer::green);
		}
	}

	handle.Update(selectedEntity, deltaTime, totalTime, mouseRayStart, mouseRayDirection, mouseWorldDelta, matVP);

	// mouse picking
	if (input.GetLeftMouseDown() && !handle.IsMoving())
	{
		float minDist = FLT_MAX;
		Renderable* selectedObj = nullptr;

		for(Renderable *i : ComponentManager::current->GetAllComponents<Renderable>()) {

			Transform* t = i->GetEntity()->GetComponent<Transform>();
			auto* m = t->GetWorldMatrix();

			XMMATRIX worldMat = XMMatrixTranspose(
				XMLoadFloat4x4(m)
			);

			BoundingOrientedBox bounds;
			BoundingOrientedBox::CreateFromBoundingBox(bounds, i->GetMesh()->GetBounds());
			bounds.Transform(bounds, worldMat);

			float dist = FLT_MAX;
			if (bounds.Intersects(mouseRayStart, mouseRayDirection, dist))
			{
				if (dist < minDist)
				{
					minDist = dist;
					selectedObj = i;
				}
			}
		}

		if (nullptr != selectedObj)
		{
			selectedEntity = selectedObj->GetEntity();
		}
		else
		{
			selectedEntity = nullptr;
		}
	}

	if (isPlaying && !isPaused)
	{
		Scene* s = Engine::instance()->GetCurScene();
		s->Tick(deltaTime, inGameTotalTime);

		inGameTotalTime += deltaTime;
	}
}

void Editor::CleanUp()
{
	assert(nullptr != _instance);
	delete _instance;
	_instance = nullptr;
}

void Editor::OnSceneLoad()
{
	inGameTotalTime = 0.0f;
	isPlaying = false;
	isPaused = false;
	selectedEntity = nullptr;
}

void Editor::Play()
{
	Scene* scene = Engine::instance()->GetCurScene();
	if (nullptr == scene) return;

	if (!isPlaying)
	{
		//Engine::instance()->SavaScene();
		isPlaying = true;
		inGameTotalTime = 0.0f;
	}
	isPaused = false;
}

void Editor::Pause()
{
	Scene* scene = Engine::instance()->GetCurScene();
	if (nullptr == scene) return;

	if (!isPlaying) return;

	isPaused = true;
}

void Editor::Stop()
{
	Scene* scene = Engine::instance()->GetCurScene();
	if (nullptr == scene) return;

	if (isPlaying)
	{
		isPlaying = false;
		inGameTotalTime = 0.0f;
		Engine::instance()->LoadScene(scene->GetName());
	}
}

void Editor::RunStandalone()
{
	//Engine::instance()->SavaScene();
	do
	{
		str_t cwd = GetWorkingDirectory();
		str_t path = cwd + engineExeName;
		
		STARTUPINFO si = {};
		PROCESS_INFORMATION pi = {};
		si.cb = sizeof(si);

		BOOL ret = CreateProcess(
			path.c_str(),
			nullptr,
			nullptr,
			nullptr,
			FALSE,
			0,
			nullptr,
			nullptr,
			&si,
			&pi);
			
		assert(ret);

		WaitForSingleObject(pi.hProcess, INFINITE);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

	} while (0);

}

void Editor::Build()
{
	//Engine::instance()->SavaScene();
	str_t cwd = GetWorkingDirectory();
	str_t path = SelectFolder();

	if (path.length() <= 0)
		return;
	
	{
		str_t from = cwd + assetFolderName + STR("\\*");
		str_t to = path + assetFolderName;
		CopyFile(from, to);
	}

	{
		str_t from = cwd + scriptFolderName + STR("\\*");
		str_t to = path + scriptFolderName;
		CopyFile(from, to);
	}

	{
		str_t from = cwd + dllsName;
		str_t to = path;
		CopyFile(from, to);
	}

	{
		str_t from = cwd + engineExeName;
		str_t to = path + engineExeName;
		CopyFile(from, to);
	}
}

#endif