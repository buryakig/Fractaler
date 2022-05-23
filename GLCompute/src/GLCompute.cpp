#include "ImApp/Application.h"
#include "ImApp/EntryPoint.h"
#include "Utils/Resources/Resources.h"

void MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

class ExampleLayer : public ImApp::Layer
{
public:
	virtual void OnAttach() override
	{
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(MessageCallback, 0);


		glGenTextures(1, &textureID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA16F, 512, 512);
		glBindImageTexture(0, textureID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA16F);

		testCompute = Resources::LoadComputeShader("GLCompute/res/ComputeShaders/test.compute");
	
		glUniform1i(glGetUniformLocation(testCompute->programID, "destTex"), 0);
	}
	

	virtual void OnUIRender() override
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glUseProgram(testCompute->programID);
		glUniform1f(glGetUniformLocation(testCompute->programID, "roll"), (float)0.05f);
		glDispatchCompute(512/16, 512/16, 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		ImGui::Begin("OpenGL Texture Text");
		ImGui::Text("pointer = %p", textureID);
		ImGui::Text("size = %d x %d", 512, 512);
		ImGui::Image((void*)(intptr_t)textureID, ImVec2(512, 512));
		ImGui::End();
	}


	std::shared_ptr<ComputeShader> testCompute;

	GLuint textureID;
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