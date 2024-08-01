/**
* @file GameEngine.h
*/

#ifndef GAMEENGINE_H_INCLUDE
#define GAMEENGINE_H_INCLUDE

#include "Primitive.h"
#include "Texture.h"
#include "Actor/Actor.h"
#include "Actor/UI/UI.h"
#include "Sampler.h"
#include "Camera.h"
#include "Shader/FramebufferObject.h"
#include "Actor/UI/Sprite.h"

#include <GLFW/glfw3.h>
#include <unordered_map>
#include <random>
#include <iostream>
#include <string>

using ActorList = std::vector<std::shared_ptr<Actor>>;
using TextureBuffer = std::unordered_map<std::string, std::shared_ptr<Texture>>;

class GameEngine
{
public:
	static bool Initialize();
	static void Finalize();
	static GameEngine& Get();

	ActorList& GetActors(Layer layer = Layer::Default);
	std::vector<std::shared_ptr<UI>>& GetUI();
	
	// �A�N�^�[�ǉ�
	void AddActors(std::shared_ptr<Actor> actor) { newActors.push_back(actor); }
	// UI�ǉ�
	void AddUI(std::shared_ptr<UI> ui) { this->ui.push_back(ui); }
	void UpdateActors(float deltaTime);
	void UpdateUI(float deltaTime);
	void UpdatePhysics(float deltaTime);
	void UpdateCamera();
	void NewFrame();
	void ActorsClear();
	void PostUpdateActors();
	void RemoveDeadActors();
	void RenderDefault();
	void RenderSprite();
	void RenderUI();
	void PostRender();

	PrimitiveBuffer& GetPrimitiveBuffer();
	const Primitive& GetPrimitive(int n);
	const Primitive& GetPrimitive(const char* filename)const;

	std::shared_ptr<Texture> LoadTexture(const char* filename);

	bool WindowShouldClose()const;

	bool GetKey(int key)const;

	int GetMouseButton(int key)const;

	glm::vec2 GetWindowSize()const;

	void SwapBuffers()const;
	double GetTime();

	glm::vec2 GetCursorPosition();


	void SetCursorType(int cursorType);

	void SetCursorPos(glm::vec2 cursorpos);

	void SetglClearColor(glm::vec3 bgcolor);

	void SetShowCollider(bool isShowCollider);

	void WindowClose();

	Camera& GetCamera();
	const Camera& GetCamera()const;

	std::shared_ptr<ProgramPipeline>& GetPipeline();
	std::shared_ptr<Sampler>& GetSampler();

private:
	GameEngine() = default;
	~GameEngine() = default;
	GameEngine(const GameEngine&) = delete;
	GameEngine& operator=(const GameEngine&) = delete;

	GLFWwindow* window = nullptr;
	// �E�B���h�E�T�C�Y
	glm::vec2 windowSize = glm::vec2(0);
	std::shared_ptr<ProgramPipeline> pipeline;
	std::shared_ptr<ProgramPipeline> pipelineUI;
	std::shared_ptr<Sampler> sampler;
	std::shared_ptr<Sampler> samplerUI;

	// �e�`��pFBO
	std::shared_ptr<FramebufferObject> fboShadow;

	// �A�N�^�[�z��
	ActorList actors[layerCount];
	// �ǉ�����A�N�^�[�̔z��
	ActorList newActors;
	// �v���~�e�B�u
	std::shared_ptr<PrimitiveBuffer> primitiveBuffer;
	// �e�N�X�`���[�z��
	TextureBuffer textureBuffer;
	// ���C���J����
	Camera mainCamera;

	// UI�z��
	std::vector<std::shared_ptr<UI>> ui;

	// �R���C�_�[�\���p(�f�o�b�O�p)

	// �R���C�_�[�\���t���O
	bool showCollider = true;
	std::shared_ptr<ProgramPipeline> pipelineCollider;
	std::shared_ptr<Texture> texCollider;

	// �X�v���C�g�`��p
	SpriteRenderer spriteRenderer;

	std::mt19937 rg;
	std::shared_ptr<FramebufferObject> fbo;
	std::shared_ptr<ProgramPipeline> pipelienShadow;
	std::shared_ptr<Sampler> samplerShadow;

	// �w�i�F
	glm::vec3 bgColor=glm::vec3(0);

};

#endif // GAMEENGINE_H_INCLUDE