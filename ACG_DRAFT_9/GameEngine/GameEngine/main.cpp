#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw.h"
#include "imgui\imgui_impl_opengl3.h"

#include "Graphics\window.h"
#include "Camera\camera.h"
#include "Shaders\shader.h"
#include "Model Loading\mesh.h"
#include "Model Loading\texture.h"
#include "Model Loading\meshLoaderObj.h"
#include <stdio.h>

//music
#include <Windows.h>

void processKeyboardInput ();
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


Window window("Game Engine", 800, 800);
Camera camera;

glm::vec3 lightColor = glm::vec3(1.0f);
glm::vec3 lightPos = glm::vec3(-180.0f, 100.0f, -200.0f);


// Quest variables
bool mainQuestCompleted = false;
bool fireElementQuestCompleted = false;
bool windElementQuestCompleted = false;
bool earthElementQuestCompleted = false;
bool waterElementQuestCompleted = false;

//fire quest variables
bool hascoal = false;
bool flame1 = false;
bool flame2 = false;
bool flame3 = false;
bool flame4 = false;


//earth quest variables
bool hasore = false;

//water quest variables
float incrementwater = 0;
float incrementwater2 = 0;
glm::vec3 currentPositionofball;
glm::vec3 currentPositionofball2;
float haswater = false;

//wind quest variables 
bool haswindblessing = false;

//dragon variables
float timecrafted = 0;
float timefinish = 0;
bool dragonspwan = false;
bool hasObtainSword = false;
bool dragondead = false;
bool swordiscomplete = false;

//sounvariables
float lasttimeplayed = 0;
bool anothersoundhasplayed = true;
float soundlenght = 6;
bool hassound= true;

//handle audio
void play_sound(const std::string& sound_name) {
	if (hassound == true)
	{
		std::string sound_path = "Resources/Sounds/" + sound_name + ".wav";
		//stop the background music
		PlaySound(NULL, 0, 0);
		anothersoundhasplayed = true;
		//play the sound
		PlaySound(sound_path.c_str(), NULL, SND_ASYNC | SND_FILENAME);
		lasttimeplayed = glfwGetTime();
	}
	else
	{
		PlaySound(NULL, 0, 0);
	}
}

void RenderQuestUI()
{
    
	// Inside the game loop, after starting ImGui frame
	ImGui::Begin("Quest System");

	if (mainQuestCompleted == false) {
		// Main Quest
		if (swordiscomplete == false) {
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Main Quest:Power the Ancient Forge and Craft the ledendary Sword");
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Harness the Elemental Essence from the four Layers of the Ancient Dwarf Mine to Craft the Mighty Sword");
		}

		if (swordiscomplete == true && hasObtainSword == false)
		{
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Main Quest: Let the power of the four elements sip Into the blade!");
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Still thy hand, adventurer, and count to ten beats of a silent heart before claiming the mystic blade");
		}

		if (hasObtainSword == true)
		{
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Main Quest: The time of revenge has come! Defeat the dragon ");
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Unleash the power of the enchanted sword forged in the Ancient Forge. Face the dragon and secure the future for your people.");
		}

		// Sub-quests
		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "Sub-quests:");

		// Fire Element Quest
		ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "       X Fire Element Quest: %s", fireElementQuestCompleted ? "Completed" : "Not Completed");
		if (ImGui::IsItemHovered())
			if (flame1 && flame2 && flame3 && flame4)
				ImGui::SetTooltip("You can feel the heat radiating from the master forge. A testament to your success!");
			else
				ImGui::SetTooltip("Kindle the ancient forges in the timeworn basement. Your mind swirls with thoughts of fueling the dormant flames within...");

		// Water Element Quest
		ImGui::TextColored(ImVec4(0.0f, 0.0f, 1.0f, 1.0f), "       X Water Element Quest: %s", waterElementQuestCompleted ? "Completed" : "Not Completed");
		if (ImGui::IsItemHovered())
			if (haswater == false)
				ImGui::SetTooltip("To cool down a weapon you need water. The ancient well might still be working. What is that noise coming from it though?");
			else
				ImGui::SetTooltip("You heriocally faced the beasts of the sea! Your reward: Water to quench the masterpiece you're about to forge!");

		// Wind Element Quest
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "       X Wind Element Quest: %s", windElementQuestCompleted ? "Completed" : "Not Completed");
		if (ImGui::IsItemHovered())
			if (haswindblessing == false)
				ImGui::SetTooltip("The blessing of the wind is given by the goddess of the sky. Perhaps a prayer at her shrine... In her realm you somehow feel ligther");
			else
				ImGui::SetTooltip("The bellows blast once more with the force of a hurricane! The flames of the great forge are growing ever hotter!");

		// Earth Element Quest
		ImGui::TextColored(ImVec4(0.5f, 0.25f, 0.0f, 1.0f), "       X Earth Element Quest: %s", earthElementQuestCompleted ? "Completed" : "Not Completed");
		if (ImGui::IsItemHovered())
			if (hasore == false)
				ImGui::SetTooltip("A warm green light reveals the path, stirring memories of an age-old tale...a hero in search of a legendary weapon");
			else
				ImGui::SetTooltip("An ore worthy of your people! Unleash its power in the forge, craft a legendary weapon, and shape your destiny!");
		//make text bigger
		ImGui::SetWindowFontScale(1.2f);
		ImGui::SetWindowSize(ImVec2(1200, 350));
	}
	else
	{
		// Main Quest completion message
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Main Quest: Completed");

		// Congratulations and gratitude to the player
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "");
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Congratulations! By your valor, the dragon lies defeated.");
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Your courage has saved the dwarves from impending doom.");
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "As the sun sets over the dwarven realm, tales shall be told");
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "of your heroism, and the kinship of those who escaped the dragon's wrath.");
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "");
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Thank you for playing our game !");
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "");
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "A game concept by:");
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "						   Gradinariu Rares");
		ImGui::TextColored(ImVec4(0.0f, 0.0f, 1.0f, 1.0f), "				           Andrei Alexandru");
		ImGui::TextColored(ImVec4(1.0f, 0.686f, 0.686f, 1.0f),"   				        Pavaloiu Bianca");
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "				           Rizan Gabriel");
		ImGui::SetWindowFontScale(1.8f);
		//make windwow bigger
		ImGui::SetWindowSize(ImVec2(1920, 1080));
	}

	
	//creat button to quit the game
	if (ImGui::Button("Quit Game"))
	{
		glfwSetWindowShouldClose(window.getWindow(), true);
	}

	//creat button to mute background music (make a bool music false)
	if (ImGui::Button("Mute Music"))
	{
		if (hassound == false)
		{
			hassound = true;
			play_sound("background");
		}
		else
		{
			hassound = false;
			PlaySound(NULL, 0, 0);
		}
	}
	// End ImGui window
	ImGui::End();

}

int main()
{ 
	//sky colour
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	

	//building and compiling shader program
	Shader shader("Shaders/vertex_shader.glsl", "Shaders/fragment_shader.glsl");
	Shader sunShader("Shaders/sun_vertex_shader.glsl", "Shaders/sun_fragment_shader.glsl");

	//Textures

	GLuint tex = loadBMP("Resources/Textures/MazeWalls.bmp");
	GLuint tex2 = loadBMP("Resources/Textures/rock.bmp");
	GLuint tex3 = loadBMP("Resources/Textures/MazeTop.bmp");
	GLuint cristals = loadBMP("Resources/Textures/cristals.bmp");
	GLuint rock = loadBMP("Resources/Textures/rock.bmp");
	GLuint mazeportal = loadBMP("Resources/Textures/mazeportal.bmp");
	GLuint fireportaltext = loadBMP("Resources/Textures/fireportal.bmp");
	GLuint furnancetext = loadBMP("Resources/Textures/furnance.bmp");
	GLuint windportaltext = loadBMP("Resources/Textures/windportal.bmp");
	GLuint skytext = loadBMP("Resources/Textures/sky.bmp");
	GLuint cloudtext = loadBMP("Resources/Textures/cloud.bmp");
	GLuint anviltext = loadBMP("Resources/Textures/anvil.bmp");
	GLuint welltext = loadBMP("Resources/Textures/well.bmp");
	GLuint darkbrick = loadBMP("Resources/Textures/dark_brick.bmp");
	GLuint flametext = loadBMP("Resources/Textures/flame.bmp");
	GLuint carttext = loadBMP("Resources/Textures/cart.bmp");
	GLuint coaltext = loadBMP("Resources/Textures/coal.bmp");
	GLuint oretext = loadBMP("Resources/Textures/ore.bmp");
	GLuint dragonText = loadBMP("Resources/Textures/dragon.bmp");
	GLuint swordText = loadBMP("Resources/Textures/sword.bmp");
	GLuint anvilcrackedtext = loadBMP("Resources/Textures/anvil_cracked.bmp");
	GLuint balltext = loadBMP("Resources/Textures/balltext.bmp");
	GLuint crabtext = loadBMP("Resources/Textures/crabtext.bmp");
	GLuint crabtext2 = loadBMP("Resources/Textures/crabtext2.bmp");
	GLuint buckettext = loadBMP("Resources/Textures/buckets.bmp");	
	


	glEnable(GL_DEPTH_TEST);

	//Test custom mesh loading
	std::vector<Vertex> vert;
	vert.push_back(Vertex());
	vert[0].pos = glm::vec3(10.5f, 10.5f, 0.0f);
	vert[0].textureCoords = glm::vec2(3.0f, 3.0f); 

	vert.push_back(Vertex());
	vert[1].pos = glm::vec3(10.5f, -10.5f, 0.0f);
	vert[1].textureCoords = glm::vec2(3.0f, 0.0f);

	vert.push_back(Vertex());
	vert[2].pos = glm::vec3(-10.5f, -10.5f, 0.0f);
	vert[2].textureCoords = glm::vec2(0.0f, 0.0f);

	vert.push_back(Vertex());
	vert[3].pos = glm::vec3(-10.5f, 10.5f, 0.0f);
	vert[3].textureCoords = glm::vec2(0.0f, 3.0f); 

	vert[0].normals = glm::normalize(glm::cross(vert[1].pos - vert[0].pos, vert[3].pos - vert[0].pos));
	vert[1].normals = glm::normalize(glm::cross(vert[2].pos - vert[1].pos, vert[0].pos - vert[1].pos));
	vert[2].normals = glm::normalize(glm::cross(vert[3].pos - vert[2].pos, vert[1].pos - vert[2].pos));
	vert[3].normals = glm::normalize(glm::cross(vert[0].pos - vert[3].pos, vert[2].pos - vert[3].pos));

	std::vector<int> ind = { 0, 1, 3,   
		1, 2, 3 };

	std::vector<Texture> textures;
	textures.push_back(Texture());
	textures[0].id = tex;
	textures[0].type = "texture_diffuse";

	std::vector<Texture> textures2;
	textures2.push_back(Texture());
	textures2[0].id = tex2;
	textures2[0].type = "texture_diffuse";

	std::vector<Texture> textures3;
	textures3.push_back(Texture());
	textures3[0].id = tex3;
	textures3[0].type = "texture_diffuse";

	std::vector<Texture> textures4;
	textures4.push_back(Texture());
	textures4[0].id = cristals;
	textures4[0].type = "texture_diffuse";

	std::vector<Texture> textures5;
	textures5.push_back(Texture());
	textures5[0].id = rock;
	textures5[0].type = "texture_diffuse";

	std::vector<Texture> textures6;
	textures6.push_back(Texture());
	textures6[0].id = mazeportal;
	textures6[0].type = "texture_diffuse";

	std::vector<Texture> textures7;
	textures7.push_back(Texture());
	textures7[0].id = fireportaltext;
	textures7[0].type = "texture_diffuse";

	std::vector<Texture> textures8;
	textures8.push_back(Texture());
	textures8[0].id = furnancetext;
	textures8[0].type = "texture_diffuse";

	std::vector<Texture> textures9;
	textures9.push_back(Texture());
	textures9[0].id = windportaltext;
	textures9[0].type = "texture_diffuse";

	std::vector<Texture> textures10;
	textures10.push_back(Texture());
	textures10[0].id = skytext;
	textures10[0].type = "texture_diffuse";

	std::vector<Texture> textures11;
	textures11.push_back(Texture());
	textures11[0].id = cloudtext;
	textures11[0].type = "texture_diffuse";

	std::vector<Texture> textures12;
	textures12.push_back(Texture());
	textures12[0].id = anviltext;
	textures12[0].type = "texture_diffuse";

	std::vector<Texture> textures13;
	textures13.push_back(Texture());
	textures13[0].id = welltext;
	textures13[0].type = "texture_diffuse";

	std::vector<Texture> textures14;
	textures14.push_back(Texture());
	textures14[0].id = darkbrick;
	textures14[0].type = "texture_diffuse";

	std::vector<Texture> textures15;
	textures15.push_back(Texture());
	textures15[0].id = flametext;
	textures15[0].type = "texture_diffuse";

	std::vector<Texture> textures16;
	textures16.push_back(Texture());
	textures16[0].id = carttext;
	textures16[0].type = "texture_diffuse";

	std::vector<Texture> textures17;
	textures17.push_back(Texture());
	textures17[0].id = coaltext;
	textures17[0].type = "texture_diffuse";

	std::vector<Texture> textures18;
	textures18.push_back(Texture());
	textures18[0].id = oretext;
	textures18[0].type = "texture_diffuse";

	std::vector<Texture> textures19;
	textures19.push_back(Texture());
	textures19[0].id = dragonText;
	textures19[0].type = "texture_diffuse";

	std::vector<Texture> textures20;
	textures20.push_back(Texture());
	textures20[0].id = swordText;
	textures20[0].type = "texture_diffuse";

	std::vector<Texture> textures21;
	textures21.push_back(Texture());
	textures21[0].id = anvilcrackedtext;
	textures21[0].type = "texture_diffuse";

	std::vector<Texture> textures22;
	textures22.push_back(Texture());
	textures22[0].id = balltext;
	textures22[0].type = "texture_diffuse";

	std::vector<Texture> textures23;
	textures23.push_back(Texture());
	textures23[0].id = crabtext;
	textures23[0].type = "texture_diffuse";

	std::vector<Texture> textures24;
	textures24.push_back(Texture());
	textures24[0].id = crabtext2;
	textures24[0].type = "texture_diffuse";

	std::vector<Texture> textures25;
	textures25.push_back(Texture());
	textures25[0].id = buckettext;
	textures25[0].type = "texture_diffuse";






	Mesh mesh(vert, ind, textures3);

	// Create Obj files - easier :)
	// we can add here our textures :)
	MeshLoaderObj loader;
	Mesh sun = loader.loadObj("Resources/Models/sphere.obj");
	Mesh maze = loader.loadObj("Resources/Models/Maze.obj", textures);
	Mesh plane = loader.loadObj("Resources/Models/plane.obj", textures3);
	Mesh greenCrystal = loader.loadObj("Resources/Models/greencrystal.obj", textures4);
	Mesh hub = loader.loadObj("Resources/Models/Dungeon.obj",textures5);
	Mesh stoneportal = loader.loadObj("Resources/Models/portal.obj", textures6);
	Mesh fireportal = loader.loadObj("Resources/Models/portal.obj", textures7);
	Mesh furnance = loader.loadObj("Resources/Models/furnance.obj", textures8);
	Mesh windportal = loader.loadObj("Resources/Models/portal.obj", textures9);
	Mesh tophub = loader.loadObj("Resources/Models/plane.obj", textures5);
	Mesh skybox = loader.loadObj("Resources/Models/sky.obj", textures10);
	Mesh cloud = loader.loadObj("Resources/Models/cloud.obj", textures11); 
	Mesh owl = loader.loadObj("Resources/Models/owl.obj", textures5);
	Mesh anvil = loader.loadObj("Resources/Models/anvil.obj", textures12);
	Mesh well = loader.loadObj("Resources/Models/well.obj", textures13); 
	Mesh furnance2 = loader.loadObj("Resources/Models/furnance.obj", textures14);
	Mesh flame = loader.loadObj("Resources/Models/flame.obj", textures15);
	Mesh cart = loader.loadObj("Resources/Models/MineCart.obj", textures16);
	Mesh coal = loader.loadObj("Resources/Models/coal.obj", textures17);
	Mesh ore = loader.loadObj("Resources/Models/ore.obj", textures18);
	Mesh dragon = loader.loadObj("Resources/Models/dragon.obj", textures19);
	Mesh sword = loader.loadObj("Resources/Models/sword.obj", textures20);
	Mesh anvilcracked = loader.loadObj("Resources/Models/anvil.obj", textures21);
	Mesh ball = loader.loadObj("Resources/Models/ball.obj", textures22);
	Mesh crab = loader.loadObj("Resources/Models/crab.obj", textures23);
	Mesh crab2 = loader.loadObj("Resources/Models/crab.obj", textures24);
	Mesh ball2 = loader.loadObj("Resources/Models/ball.obj", textures22);
	Mesh bucket = loader.loadObj("Resources/Models/buckets.obj", textures25);


	// Initialize ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	const char* fontPath = "imgui/font.ttf";

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF(fontPath, 24);

	// Setup ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330");


	//check if we close the window or press the escape button
	while (!window.isPressed(GLFW_KEY_ESCAPE) &&
		glfwWindowShouldClose(window.getWindow()) == 0)
	{
		window.clear();

		//otherstuff
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processKeyboardInput();

		//test mouse input
		if (window.isMousePressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			//std::cout << "Pressing mouse button" << std::endl;
		}
		 //// Code for the light ////

		sunShader.use();

		glm::mat4 ProjectionMatrix = glm::perspective(90.0f, window.getWidth() * 1.0f / window.getHeight(), 0.1f, 10000.0f);
		glm::mat4 ViewMatrix = glm::lookAt(camera.getCameraPosition(), camera.getCameraPosition() + camera.getCameraViewDirection(), camera.getCameraUp());

		GLuint MatrixID = glGetUniformLocation(sunShader.getId(), "MVP");

		//Test for one Obj loading = light source

		glm::mat4 ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, lightPos);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		sun.draw(sunShader);

		//// End code for the light ////

		shader.use();

		///// Test Obj files for maze ////

		GLuint MatrixID2 = glGetUniformLocation(shader.getId(), "MVP");
		GLuint ModelMatrixID = glGetUniformLocation(shader.getId(), "model");

		ModelMatrix = glm::mat4(1.0);
	
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -30.0f, 0.0f));

		float scaleValue = 0.5f;  // Adjust this value as needed
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(scaleValue, 0.25, scaleValue));
	

		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniform3f(glGetUniformLocation(shader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(shader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(glGetUniformLocation(shader.getId(), "viewPos"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		maze.draw(shader);

		///// Test celing Obj file //////

		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 50.0f, 0.0f));

		//scale
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(10, 10, 10));

		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		plane.draw(shader);


		// 1st Green Crystal
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-252.0f, -30.0f, -187.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(10, 10, 10));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		greenCrystal.draw(shader);

		// 2nd Green Crystal
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-446.0f, -30.0f, -444.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(10, 10, 10));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		greenCrystal.draw(shader);

		// 3rd Green Crystal
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-297.0f, -30.0f, 190.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(10, 10, 10));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		greenCrystal.draw(shader);



		//Hub
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(259, -30.0f, -1280));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(10, 10, 10));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		hub.draw(shader);


		//stoneportalentrance
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(85, -80.0f, -1336.03));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.1, 0.1, 0.1));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		stoneportal.draw(shader);

		//stoneportalexit
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-247.106, -100, 509.32));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.25, 0.25, 0.25));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(5000.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		stoneportal.draw(shader);

		//stoneportalintolabyrinth
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(252.0f, -100, -500.187)); 
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.25, 0.25, 0.25)); 
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(5000.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix; 
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]); 
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]); 
		stoneportal.draw(shader); 

		//furnance
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(346, -30.0f, -1108));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.1, 0.1, 0.1));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(10250.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		furnance.draw(shader);

		//windportal
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(85.68, -80.0f, -1064.29));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.1, 0.1, 0.1));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		windportal.draw(shader);

	
		//hub_top
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(352.0f, 45.0f, -1249.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(10, 10, 10));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		tophub.draw(shader);

		//skybox
		ModelMatrix = glm::mat4(1.0); 
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-1385.0f, 0.0f, -1387.0f)); 
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(35, 35, 35)); 
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix; 
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]); 
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]); 
		skybox.draw(shader);

		//cloud
		ModelMatrix = glm::mat4(1.0); 
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-1385.0f, -160.0f, -1387.0f)); 
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1, 0.5, 0.5)); 
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix; 
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]); 
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]); 
		cloud.draw(shader); 

		//cloud2
		ModelMatrix = glm::mat4(1.0); 
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-1479.55, -190, -1170.74));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1, 0.5, 0.5)); 
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(5000.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix; 
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]); 
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]); 
		cloud.draw(shader);

		//cloud3
		ModelMatrix = glm::mat4(1.0); 
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-1285.0f, -160.0f, -1387.0f)); 
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1, 0.5, 0.5)); 
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix; 
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]); 
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]); 
		cloud.draw(shader); 

		//owl
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-1698.0f, -40.0f, -1555.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(100, 100, 100));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(10000.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		owl.draw(shader); 

		//forge1
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(630.0f, -30.0f, -1610.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.1, 0.1, 0.1));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		furnance2.draw(shader);

		//forge2
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(700.0f, -30.0f, -1610.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.1, 0.1, 0.1));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		furnance2.draw(shader);

		//forge3
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(770.0f, -30.0f, -1610.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.1, 0.1, 0.1));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		furnance2.draw(shader);

		//forge4
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(840.0f, -30.0f, -1610.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.1, 0.1, 0.1));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		furnance2.draw(shader);

		//anvil
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(350.0f, -35.0f, -1200.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.5, 0.5, 0.5));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(5000.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		if (swordiscomplete==false)
		  anvil.draw(shader);

		//well
		ModelMatrix = glm::mat4(1.0); 
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(140.396f, -30.f, -1547.35f)); 
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(20, 20, 20)); 

		//rotate
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(5000.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix; 
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]); 
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]); 
		well.draw(shader);


		//flame1
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(630.0f, -20.0f, -1593.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(100, 100, 100));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		if(flame1)
		 flame.draw(shader);

		//flame2
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(700.0f, -20.0f, -1593.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(100, 100, 100));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		if (flame2)
		 flame.draw(shader);


		//flame3
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(770.0f, -20.0f, -1593.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(100, 100, 100));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		if(flame3)
		 flame.draw(shader);


		//flame4
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(840.0f, -20.0f, -1593.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(100, 100, 100));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		if(flame4)
		 flame.draw(shader);


		//main forge flame
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(345.0f, -18.0f, -1123.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(100, 100, 100));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		if (flame1 && flame2 && flame3 && flame4)
			{
				flame.draw(shader);
			    fireElementQuestCompleted = true;

			}

		//Minecart
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(835.0f, -25.0f, -1263.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(7, 3, 5));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		cart.draw(shader);

		//coal
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(835.0f, -25.0f, -1263.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(13, 20, 13));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(5000.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		if(hascoal == false)
		 coal.draw(shader);

		//ore  
		ModelMatrix = glm::mat4(1.0); 
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-250.0f, -40.0f, 440.0f)); 
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(25, 25, 25)); 
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-5000.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix; 
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]); 
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]); 
		if(hasore == false)
		 ore.draw(shader);

		//dragon  
		ModelMatrix = glm::mat4(1.0); 
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(376.0f, -30.0f, -1380.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(5, 5, 5)); 
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix; 
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]); 
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]); 
		if (dragonspwan)
		  dragon.draw(shader);  

		//sword
		ModelMatrix = glm::mat4(1.0);
		//translate in front of the player
		ModelMatrix = glm::translate(ModelMatrix, camera.getCameraPosition() + camera.getCameraViewDirection() * 5.0f - glm::vec3(0, 5, 0));

		//rotate the sword
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-3000.0f)+ camera.getCameraViewDirection().z, glm::vec3(0.0f, 1.0f, 0.0f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-3000.0f) + camera.getCameraViewDirection().x, glm::vec3(1.0f, 1.0f, 0.0f));
		ModelMatrix = glm::rotate(ModelMatrix, -camera.getCameraViewDirection().y, glm::vec3(1.0f, 0.0f, 1.0f));

		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.5, 0.5, 0.5));;

		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		if(hasObtainSword)
		 sword.draw(shader);


		//sword on anvil
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(365.0f, -12.5f, -1200.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(4, 4, 4));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.5, 0.5, 0.5));;
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(5000.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-5150.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		if (hasObtainSword==false && swordiscomplete==false && earthElementQuestCompleted==true)
			sword.draw(shader);


		//sword spinning on anvil
		float rotationSpeed = 0.8;
		float currentTime = glfwGetTime();
		float angle = glm::radians(5000.0f) + sin(currentTime * rotationSpeed) * glm::radians(10400.0f);

		
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(350.0f, 25, -1200.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(4, 4, 4));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.5, 0.5, 0.5));;
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(5000.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(10300.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		ModelMatrix = glm::rotate(ModelMatrix, angle, glm::vec3(0.0f, 1.0f, 0.0f));
		


		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		if (hasObtainSword == false && swordiscomplete == true)
			sword.draw(shader);
		

		//anvilcracked
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(350.0f, -35.0f, -1200.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.5, 0.5, 0.5));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(5000.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		if (swordiscomplete == true)
			anvilcracked.draw(shader);
		
		//ball
		ModelMatrix = glm::mat4(1.0);
		glm::vec3 initialPosition(302, -15, -1660);
		glm::vec3 currentPositionofball;
		glm::vec3 finalPosition(302, -15, -1416);

		float ballSpeed = 0.3;
		incrementwater = incrementwater + ballSpeed;
		currentPositionofball = initialPosition + glm::vec3(0,0,incrementwater);

		//if player is close to the water projectile teleport back to entrance of water area
		float waterballpossitionThreshold = 30.0f;  // Adjust this distance as needed
		float waterballpossitiondistance = glm::length(camera.getCameraPosition() - currentPositionofball);
		if (waterballpossitiondistance < waterballpossitionThreshold && haswater==false)
		{
			camera.setCameraPosition(glm::vec3(390.0f, 0.0f, -1580.0f));
		}

		if (incrementwater > 244)
		{
			incrementwater = 0;
		}
		ModelMatrix = glm::translate(ModelMatrix, currentPositionofball);
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		if(haswater == false)
		  ball.draw(shader);
		

		//spwan crab1
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(302, -23, -1645));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.5, 0.5, 0.5));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-5000.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		crab.draw(shader);


		//spwan crab2
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(226.012f, -23.f, -1440.94f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.5, 0.5, 0.5));

		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-5000.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(-10000.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		crab2.draw(shader);

		//ball2
		ModelMatrix = glm::mat4(1.0);
		glm::vec3 initialPosition2(226, -15, -1440);
		glm::vec3 currentPositionofball2;
		glm::vec3 finalPosition2(226, -15, -1660);
		incrementwater2 = incrementwater2 + ballSpeed;
		currentPositionofball2 = initialPosition2 - glm::vec3(0, 0, incrementwater2);
		//if player is close to the water projectile teleport back to entrance of water area
		float waterballpossitionThreshold2 = 30.0f;  // Adjust this distance as needed
		float waterballpossitiondistance2 = glm::length(camera.getCameraPosition() - currentPositionofball2);
		if (waterballpossitiondistance2 < waterballpossitionThreshold2 && haswater==false)
		{
			camera.setCameraPosition(glm::vec3(390.0f, 0.0f, -1580.0f));
		}

		if (incrementwater2 > 244)
		{
			incrementwater2 = 0;
		}
		ModelMatrix = glm::translate(ModelMatrix, currentPositionofball2);
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		if(haswater==false)
			ball2.draw(shader);


		//buckets
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(270.776, -30, -1131.98));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(14, 14, 14));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		if (haswater == true)
			bucket.draw(shader);



		//resetbackground music
		if (anothersoundhasplayed == true && hassound==true) {
			float timeelapsed = glfwGetTime();
			if (timeelapsed - lasttimeplayed > soundlenght)
			{
				PlaySound(NULL, 0, 0);
				PlaySound(TEXT("Resources/Sounds/background.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
				anothersoundhasplayed = false;
			}
		}


		//gui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		
		// Inside the game loop, after starting ImGui frame
		RenderQuestUI();

         
		// Rendering ImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		window.update();

	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}



void processKeyboardInput()
{   
	float cameraSpeed = 50 * deltaTime;
	float originalCameraSpeed = 50 * deltaTime;
	

	// Sprinting
	if (window.isPressed(GLFW_KEY_LEFT_SHIFT))
		cameraSpeed = camera.sprint(true, cameraSpeed);
	else
		cameraSpeed = originalCameraSpeed;

	// Jumping
	glm::vec3 windareaPosition = glm::vec3(-1385.0f, 0.0f, -1387.0f);
	float windareadistanceThreshold = 500.0f;  // Adjust this distance as needed
	float windareadistance = glm::length(camera.getCameraPosition() - windareaPosition);  

	if (window.isPressed(GLFW_KEY_SPACE))
		if (windareadistance > windareadistanceThreshold) { 
			camera.jump();
		}
		else {
			camera.superjump();
		}

	//translation
	if (window.isPressed(GLFW_KEY_W))
		camera.keyboardMoveFront(cameraSpeed);
	if (window.isPressed(GLFW_KEY_S))
		camera.keyboardMoveBack(cameraSpeed);
	if (window.isPressed(GLFW_KEY_A))
		camera.keyboardMoveLeft(cameraSpeed);
	if (window.isPressed(GLFW_KEY_D))
		camera.keyboardMoveRight(cameraSpeed);
	if (window.isPressed(GLFW_KEY_R))
		camera.keyboardMoveUp(cameraSpeed);
	if (window.isPressed(GLFW_KEY_F))
		camera.keyboardMoveDown(cameraSpeed);

	//rotation
	if (window.isPressed(GLFW_KEY_LEFT))
		camera.rotateOy(cameraSpeed);
	if (window.isPressed(GLFW_KEY_RIGHT))
		camera.rotateOy(-cameraSpeed);
	if (window.isPressed(GLFW_KEY_UP))
		camera.rotateOx(cameraSpeed);
	if (window.isPressed(GLFW_KEY_DOWN))
		camera.rotateOx(-cameraSpeed);

	// Update camera to simulate gravity
	camera.update(deltaTime * 5);


	//interact
	if (window.isPressed(GLFW_KEY_E))
	{
		glm::vec3 coalPosition = glm::vec3(835.0f, -18.0f, -1263.0f);
		float coaldistanceThreshold = 30.0f;  // Adjust this distance as needed
		float coaldistance = glm::length(camera.getCameraPosition() - coalPosition);
		if (coaldistance < coaldistanceThreshold)
		{
			hascoal = true;
			play_sound("coal");
		}



		if (hascoal == true) {

			float flamedistanceThreshold = 30.0f;
			float distance;
			//flame1
			glm::vec3 flame1Position = glm::vec3(630.0f, -20.0f, -1593.0f);
			flamedistanceThreshold = 30.0f;  // Adjust this distance as needed
			distance = glm::length(camera.getCameraPosition() - flame1Position);
			if (distance < flamedistanceThreshold) {
				flame1 = true;
				play_sound("fire");
			}

			//flame2
			glm::vec3 flame2Position = glm::vec3(700.0f, -20.0f, -1593.0f);
			flamedistanceThreshold = 30.0f;  // Adjust this distance as needed
			distance = glm::length(camera.getCameraPosition() - flame2Position);
			if (distance < flamedistanceThreshold){
				flame2 = true;
				play_sound("fire");
			}

			//flame3
			glm::vec3 flame3Position = glm::vec3(770.0f, -20.0f, -1593.0f);
			flamedistanceThreshold = 30.0f;  // Adjust this distance as needed
			distance = glm::length(camera.getCameraPosition() - flame3Position);
			if (distance < flamedistanceThreshold){
				flame3 = true;
				play_sound("fire");
			}

			//flame4
			glm::vec3 flame4Position = glm::vec3(840.0f, -20.0f, -1593.0f);
			flamedistanceThreshold = 30.0f;  // Adjust this distance as needed
			distance = glm::length(camera.getCameraPosition() - flame4Position);
			if (distance < flamedistanceThreshold) {
				flame4 = true;
				play_sound("fire");
			}
		}
		//if close to owl statue = true
		glm::vec3 owlPosition = glm::vec3(-1698.0f, 0.0f, -1555.0f);
		float owldistanceThreshold = 50.0f;
		float owldistance = glm::length(camera.getCameraPosition() - owlPosition);
		if (owldistance < owldistanceThreshold)
		{
			haswindblessing = true;
		    play_sound("wind_godes"); 
			windElementQuestCompleted = true;
		}
		//if close to the well haswater = true
		glm::vec3 wellPosition = glm::vec3(140.396f, 0.f, -1547.35f);
		float welldistanceThreshold = 30.0f;  // Adjust this distance as needed
		float welldistance = glm::length(camera.getCameraPosition() - wellPosition);
		if (welldistance < welldistanceThreshold)
		{
			haswater = true;
			play_sound("water");
			waterElementQuestCompleted = true;
		}



		//mineore
		glm::vec3 orePosition = glm::vec3(-250.0f, 0.0f, 440.0f);
		float oredistanceThreshold = 30.0f;  // Adjust this distance as needed
		float oredistance = glm::length(camera.getCameraPosition() - orePosition);
		if (oredistance < oredistanceThreshold)
		{
			hasore = true;
			earthElementQuestCompleted = true;
			play_sound("ore");
		}


		//if all elements are collected craft sword if close to anvil
		glm::vec3 anvilPosition = glm::vec3(350.0f, 0, -1200.0f);
		float anvildistanceThreshold = 30.0f;  // Adjust this distance as needed
		float anvildistance = glm::length(camera.getCameraPosition() - anvilPosition);
		if (anvildistance < anvildistanceThreshold)
		{
			if (fireElementQuestCompleted ==true && earthElementQuestCompleted == true && windElementQuestCompleted == true && waterElementQuestCompleted == true && swordiscomplete == false)
			{
				//play sound
				soundlenght = 6;
				play_sound("craft");
				swordiscomplete = true;
				timecrafted = glfwGetTime();
				//std::cout << "sword is complete at : " << timecrafted << std::endl;
			}
		}

		//if sword is complete and close to the sword pick it up
		glm::vec3 swordPosition = glm::vec3(350.0f, 0, -1200.0f);
		float sworddistanceThreshold = 30.0f;  // Adjust this distance as needed
		float sworddistance = glm::length(camera.getCameraPosition() - swordPosition);
		if (sworddistance < sworddistanceThreshold)
		{   //check if atleast 10 seconds have passed since the sword was crafted
			float timenowsword = glfwGetTime();
			if (swordiscomplete == true && (timenowsword - timecrafted > 10))
			{
				hasObtainSword = true;
				//spawn dragon
				dragonspwan = true;
				play_sound("dragon");
			}
		}

		


		//kill dragon end game
		glm::vec3 dragonPosition = glm::vec3(376.0f, 0, -1380.0f);
		float dragondistanceThreshold = 80.0f;  // Adjust this distance as needed
		float dragondistance = glm::length(camera.getCameraPosition() - dragonPosition);
		if (dragondistance < dragondistanceThreshold && mainQuestCompleted==false)
		{
			soundlenght = 18;
			play_sound("dragon_end");
			dragonspwan = false;
			mainQuestCompleted = true;
			timefinish = glfwGetTime();
		}

	}

	//earth_teleport_to_maze
	if (!hasore) {
		glm::vec3 earthportalPosition = glm::vec3(85, 0.0f, -1336.03); 
		float earthportaldistanceThreshold = 20.0f;  // Adjust this distance as needed
		float earthportaldistance = glm::length(camera.getCameraPosition() - earthportalPosition);
		if (earthportaldistance < earthportaldistanceThreshold)
			camera.setCameraPosition(glm::vec3(250.316, 0, -443.551));
	}

	//earth_teleport_to_hub 
	if (hasore) {
		glm::vec3 earthportalPosition2 = glm::vec3(-247.106, 0, 509.32);
		float earthportaldistanceThreshold2 = 20.0f;  // Adjust this distance as needed
		float earthportaldistance2 = glm::length(camera.getCameraPosition() - earthportalPosition2);
		if (earthportaldistance2 < earthportaldistanceThreshold2)
			camera.setCameraPosition(glm::vec3(259, 0, -1280));
	}

	//wind_teleport_to_sky

	if (!haswindblessing) {
		glm::vec3 windportalPosition = glm::vec3(85.68, 0.0f, -1064.29);
		float windportaldistanceThreshold = 20.0f;
		float windportaldistance = glm::length(camera.getCameraPosition() - windportalPosition);
		if (windportaldistance < windportaldistanceThreshold)
		{
			camera.setCameraPosition(glm::vec3(-981.874, 0, -1380.92));
			play_sound("wind");
		}
	}

	//wind_teleport_to_hub
	if (haswindblessing && windareadistance < windareadistanceThreshold) {
		camera.setCameraPosition(glm::vec3(259, 0, -1280));
		soundlenght = 20;
		play_sound("wind_godes");
	}

	//if main quest is completed close the game after 45 seconds
	if (mainQuestCompleted == true)
	{
		float timenow = glfwGetTime();
		if (timenow - timefinish > 45)
		{
			exit(0);
		}
	}

}
