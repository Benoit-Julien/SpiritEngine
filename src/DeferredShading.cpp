#include "DeferredShading.hpp"
#include "Shader/DefaultShader.hpp"

DeferredShading::DeferredShading() {
	this->_constant = 1.0f;
	this->_linear = 0.1f;
	this->_quadratic = 0.01f;

	Light::Constant = this->_constant;
	Light::Linear = this->_linear;
	Light::Quadratic = this->_quadratic;

	this->_shader = std::make_shared<DeferredShader>();
	this->setup();
}

DeferredShading::~DeferredShading() {
	glDeleteBuffers(1, &this->_vbo);
	glDeleteBuffers(1, &this->_vao);
}

void DeferredShading::WindowResize(const int &width, const int &height) {
	auto self = DeferredShading::getSingletonPtr();

	self->_width = width;
	self->_height = height;
	self->setupTexture();
}

void DeferredShading::Draw(const DrawInformation &info, const bool &doLightPass) {
	auto self = DeferredShading::getSingletonPtr();

	glBindFramebuffer(GL_FRAMEBUFFER, self->_gbufferFboManager->getFboId());
	glClearColor(.0f, .0f, .0f, .0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, self->_width, self->_height);
	glEnable(GL_DEPTH_TEST);

	Scene::Draw(info);

	if (doLightPass) {
		glBindFramebuffer(GL_FRAMEBUFFER, self->_deferredFboManager->getFboId());
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		glViewport(0, 0, self->_width, self->_height);

		self->_shader->use();
		self->setTexture("gPosition", self->_texManager->Get("gPosition"), 0);
		self->setTexture("gNormal", self->_texManager->Get("gNormal"), 1);
		self->setTexture("gAmbientAlbedo", self->_texManager->Get("gAmbientAlbedo"), 2);
		self->setTexture("gDiffuseAlbedo", self->_texManager->Get("gDiffuseAlbedo"), 3);
		self->setTexture("gSpecular", self->_texManager->Get("gSpecular"), 4);

		self->_shader->setUniform("cameraPosition", info.cameraPosition);
		self->_shader->setUniform("constant", self->_constant);
		self->_shader->setUniform("linear", self->_linear);
		self->_shader->setUniform("quadratic", self->_quadratic);

		self->_shader->setSubroutine("CalcAmbientColor", GL_FRAGMENT_SHADER);
		self->drawQuad();

		auto dirLight = Scene::GetDirectionLight();
		if (dirLight != nullptr) {
			self->_shader->setSubroutine("CalcDirectionalLight", GL_FRAGMENT_SHADER);

			glm::vec3 dir = dirLight->GetRotation() * glm::vec4(0, 0, 1, 1);

			self->_shader->setUniform("directionalLight.direction", dir);
			self->_shader->setUniform("directionalLight.ambient", dirLight->Ambient);
			self->_shader->setUniform("directionalLight.diffuse", dirLight->Diffuse);
			self->_shader->setUniform("directionalLight.specular", dirLight->Specular);
			self->_shader->setUniform("directionalLight.intensity", dirLight->Intensity);

			self->drawQuad();
		}

		for (auto &light : Scene::GetLights()) {
			glm::vec3 position = light.second->GetWorldModelMatrix() * glm::vec4(0, 0, 0, 1);

			auto spot = std::dynamic_pointer_cast<SpotLight>(light.second);
			if (spot != nullptr) {
				glm::vec3 dir = spot->GetRotation() * glm::vec4(0, 0, 1, 1);

				self->_shader->setSubroutine("CalcSpotLight", GL_FRAGMENT_SHADER);
				self->_shader->setUniform("spotLight.position", position);
				self->_shader->setUniform("spotLight.direction", dir);
				self->_shader->setUniform("spotLight.ambient", spot->Ambient);
				self->_shader->setUniform("spotLight.diffuse", spot->Diffuse);
				self->_shader->setUniform("spotLight.specular", spot->Specular);
				self->_shader->setUniform("spotLight.intensity", spot->Intensity);
				self->_shader->setUniform("spotLight.cutoff", spot->Cutoff);
				self->_shader->setUniform("spotLight.innerCutoff", spot->InnerCutoff);
				self->_shader->setUniform("spotLight.exponent", spot->Exponent);
			}
			else {
				self->_shader->setSubroutine("CalcPointLight", GL_FRAGMENT_SHADER);
				self->_shader->setUniform("pointLight.position", position);
				self->_shader->setUniform("pointLight.ambient", light.second->Ambient);
				self->_shader->setUniform("pointLight.diffuse", light.second->Diffuse);
				self->_shader->setUniform("pointLight.specular", light.second->Specular);
				self->_shader->setUniform("pointLight.intensity", light.second->Intensity);
				self->_shader->setUniform("pointLight.radius", light.second->GetRadius());
			}

			self->drawQuad();
		}
		self->_shader->disable();
	}
}

void DeferredShading::setupTexture() {
	this->_texManager = std::make_shared<TextureManager>();

	this->_texManager->CreateTexture("gPosition", this->_width, this->_height, GL_NEAREST, GL_RGB16F, GL_RGB, GL_FLOAT, false);
	this->_texManager->CreateTexture("gNormal", this->_width, this->_height, GL_NEAREST, GL_RGB16F, GL_RGB, GL_FLOAT, false);
	this->_texManager->CreateTexture("gAmbientAlbedo", this->_width, this->_height, GL_NEAREST, GL_RGB16F, GL_RGB, GL_UNSIGNED_BYTE, false);
	this->_texManager->CreateTexture("gDiffuseAlbedo", this->_width, this->_height, GL_NEAREST, GL_RGB16F, GL_RGB, GL_UNSIGNED_BYTE, false);
	this->_texManager->CreateTexture("gSpecular", this->_width, this->_height, GL_NEAREST, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, false);
	this->_texManager->CreateTexture("depth", this->_width, this->_height, GL_LINEAR, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT, true);

	this->_texManager->CreateTexture("deferredRender", this->_width, this->_height, GL_LINEAR, GL_RGB16F, GL_RGB, GL_FLOAT, false);

	this->_gbufferFboManager = std::make_shared<FboManager>();
	this->_gbufferFboManager->bindToFbo(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->_texManager->Get("gPosition"));
	this->_gbufferFboManager->bindToFbo(GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, this->_texManager->Get("gNormal"));
	this->_gbufferFboManager->bindToFbo(GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, this->_texManager->Get("gAmbientAlbedo"));
	this->_gbufferFboManager->bindToFbo(GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, this->_texManager->Get("gDiffuseAlbedo"));
	this->_gbufferFboManager->bindToFbo(GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, this->_texManager->Get("gSpecular"));
	this->_gbufferFboManager->bindToFbo(GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->_texManager->Get("depth"));
	this->_gbufferFboManager->setDrawBuffers();
	assert(this->_gbufferFboManager->checkFboStatus());

	this->_deferredFboManager = std::make_shared<FboManager>();
	this->_deferredFboManager->bindToFbo(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->_texManager->Get("deferredRender"));
	this->_deferredFboManager->setDrawBuffers();
	assert(this->_deferredFboManager->checkFboStatus());
}

void DeferredShading::setup() {
	//Quad verticles - omitted z coord, because it will always be 1
	float pos[] = {
					-1.0, 1.0,
					1.0, 1.0,
					-1.0, -1.0,
					1.0, -1.0
	};

	float uv[] = {
					0.0, 1.0,
					1.0, 1.0,
					0.0, 0.0,
					1.0, 0.0
	};

//Generate VAO
	glGenVertexArrays(1, &this->_vao);
	//Bind VAO
	glBindVertexArray(this->_vao);

	//Generate VBO
	glGenBuffers(1, &this->_vbo);
	//Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);
	//Alocate buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(pos) + sizeof(uv), NULL, GL_STATIC_DRAW);
	//Fill VBO
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), pos);
	glBufferSubData(GL_ARRAY_BUFFER, 8 * sizeof(float), sizeof(uv), uv);

	//Fill attributes and uniforms
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, (sizeof(float) * 2), (void *) nullptr);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, (sizeof(float) * 2), (GLvoid *) (sizeof(float) * 8));

	//unbind buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void DeferredShading::drawQuad() {
	//Bind VAO
	glBindVertexArray(this->_vao);
	//Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);

	//Draw
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	//Unbind texture for sure
	glBindTexture(GL_TEXTURE_2D, 0);

	//Unbind VBO - just for sure
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//Unbind VAO
	glBindVertexArray(0);
}

void DeferredShading::setTexture(const std::string &name, const GLuint &texture, const int &index) {
	//Bind texture
	this->_shader->setUniform(name, index);
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, texture);
}