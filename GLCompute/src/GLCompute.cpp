#include "ImApp/Application.h"
#include "ImApp/EntryPoint.h"
#include "Utils/Resources/Resources.h"


class ExampleLayer : public ImApp::Layer
{
public:
	virtual void OnAttach() override
	{

		glGenTextures(1, &textureID);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RG16F, 512, 512);
		glBindImageTexture(0, textureID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RG16F);

		testCompute = Resources::LoadComputeShader("GLCompute/res/ComputeShaders/test.compute");
	
		glUniform1i(glGetUniformLocation(testCompute->programID, "destTex"), 0);


	}
	
	virtual void OnDetach() override
	{
		Resources::DeallocateMemory();
	}

	virtual void OnUIRender() override
	{
		glUseProgram(testCompute->programID);
		glUniform1f(glGetUniformLocation(testCompute->programID, "roll"), (float)0.05f);
		glDispatchCompute(512/16, 512/16, 1);

		ImGui::Begin("OpenGL Texture Text");
		ImGui::Text("pointer = %p", textureID);
		ImGui::Text("size = %d x %d", 512.0f, 512.0f);
		ImGui::Image((void*)(intptr_t)textureID, ImVec2(512.0f, 512.0f));
		ImGui::End();
	}


	ComputeShader* testCompute;

	unsigned int textureID;
};

ImApp::Application* ImApp::CreateApplication(int argc, char** argv)
{
	ImApp::ApplicationSpecification spec;
	spec.Name = "ImApp Example";

	ImApp::Application* app = new ImApp::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}