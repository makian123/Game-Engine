#include <graphics/pointLight.h>
#include <ecs/gameobject.h>

namespace engine::graphics {
	void PointLight::Start() {
		currPos = gameobject->GetTransform().GetPos();

		shader.Use();
		GLuint lightCount;
		glGetUniformuiv(shader.GetProgram(), shader.GetUniform("lightCount"), &lightCount);

		glUniform1ui(shader.GetUniform("lightCount"), lightCount + 1);

		glUniform3f(shader.GetUniform("light.pos"), currPos.x, currPos.y, currPos.z);
	}
	void PointLight::Update() {
		lastPos = currPos;
		currPos = gameobject->GetTransform().GetPos();

		if (lastPos == currPos) return;

		shader.Use();
		glUniform3f(shader.GetUniform("light.pos"), currPos.x, currPos.y, currPos.z);
	}
}