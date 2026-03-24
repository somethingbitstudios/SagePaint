#include "MainApp.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "./tools/ToolManager.h"
//InputManagerPtr inputManager = std::make_shared<InputManager>();
//CanvasManagerPtr canvasManager = std::make_shared<CanvasManager>();

CanvasObjectPtr go;
bool runApp = true;

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
		InputManager::Input(key, action, mods);
	//if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	//	glfwSetWindowShouldClose(window, GLFW_TRUE);
	

}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	//DLOG("[INPUT] Mouse: " << button << " Action: " << action)
		InputManager::Input(button, action, mods);
	//if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
	//	DLOG("what")
	//}
}
//I prefer calling it directly so it always updates even when staying still
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	//InputManager::SetCursorPos(xpos, ypos); TODO: USE AFTER IMPLEMENTING POSITIONBUFFER
}

float backgroundColor[4] = { 0.1f,0.05f,0.1f,1 };//TODO: move to some appstorage class

void MainApp() {
	IDLOG("----------------------------------<[DEBUG_MODE]>----------------------------------")
		IDLOG("IMPORTANT IDLOG MESSAGES ENABLED")
		DLOG("GRANULAR DLOG MESSAGES ENABLED")
		IDLOG("version: 0.06\n")



	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) {
		IDLOG("GLFW failed to init!")
		exit(EXIT_FAILURE);
	}
		
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow((int)Screen_width, (int)Screen_height, "Sage Paint Alpha",NULL, NULL);
	if (!window)
	{
		IDLOG("Window failed to init!")
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);

	glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	glfwMakeContextCurrent(window);
	glewInit();
	glfwSwapInterval(0);//when ==1, mouse movement caused small stutters

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init();



	InputManager::Init();
	//'start()'
	CanvasManager::obj= std::make_shared<CanvasObject>();
	

	go = CanvasManager::obj;
	//load some image, replace with CanvasManager call 
	ImagePtr images[] = {
		std::make_shared<Image>("C:\\temp\\gradient.png"),
		std::make_shared<Image>("C:\\temp\\miku.png")
	};

	go->LoadImageSync(images[0]);
	//put this into a call you'll do as part of canvasManager

	//TODO: put into a function
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	Screen_width = (float)width;
	Screen_height = (float)height;
	Screen_ratio = width / (float)height;

	CanvasManager::Init(); 
	

	

	int i = 0;
	float timestamp = 0;

	//static GLsync gpuFence = nullptr;

	double t1=0;
	while (runApp)
	{
		//frame init
		
		glfwGetFramebufferSize(window, &width, &height);
		Screen_width = (float)width;
		Screen_height = (float)height;
		Screen_ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		


		glfwPollEvents();
		InputManager::UpdateCursorPos(window);
		CanvasManager::UpdateRelativeCursorPos();
		InputManager::ProcessHeld();

		//'update' like portion

		{

			//go->scale.x = 1+0.5*sin((float)glfwGetTime());
			//go->scale.y = 1 + 0.5 * cos((float)glfwGetTime());
			//go->pos.x = sin((float)glfwGetTime());

			//go->rotation = 0.1f*(float)glfwGetTime();



			/*
			double xpos = inputManager->GetCursorX();
			double ypos = inputManager->GetCursorY();


			*/


			/*
			double xpos;
			double ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			*/
			//DLOG(xpos << "x")
			//DLOG(ypos << "y")
			/*
			go->pos.x = ((float)xpos)-Screen_width/2;
			go->pos.y = -((float)ypos)+Screen_height/2;
			if ((float)glfwGetTime() - timestamp > 5.0f) {
				timestamp = (float)glfwGetTime();
				i++;
				i %= 5;
				go->LoadImageSync(images[i]);
				go->scale.x = (float)images[i]->width;
				go->scale.y = (float)images[i]->height;

			}
			*/
			/* changing pixels
			int gg = (float)glfwGetTime() * 10;

			for (int o = 64000; o < images[0]->width * 2000; o++) {
				images[0]->texture[o] = (char)((gg + o * 10) % 256);
			}

			go->LoadImageSync(images[0]);
			*/


			//go->rotation = (float)glfwGetTime()/10.0f;
	}
		
		double t0 = glfwGetTime();
		//DLOG("time: " << t0 - t1)
		double frameTime = t0 - t1;
		t1 = t0;

		static double displayedFPS = 0.0;

		{
			static double frameTimes[10] = { 0 };
			static int frameIndex = 0;
			static int frameCount = 0;

			frameTimes[frameIndex] = frameTime;
			frameIndex = (frameIndex + 1) % 10;
			if (frameCount < 10) frameCount++;

			double sum = 0.0;
			for (int i = 0; i < frameCount; i++)
				sum += frameTimes[i];

			double avgFrameTime = sum / frameCount;
			double fps = avgFrameTime > 0.0 ? 1.0 / avgFrameTime : 0.0;


			static double lastUpdate = 0.0;

			if (t0 - lastUpdate >= 0.5)
			{
				displayedFPS = fps;
				lastUpdate = t0;
			}
		}

		//render objects
		go->Draw();

		//render UI
		
		//allows drawing directly
		
		


		ImGui::SetNextWindowPos(ImVec2(0, 18));
		ImGui::SetNextWindowSize(ImVec2(250, ImGui::GetIO().DisplaySize.y-18));

		ImGui::Begin("LeftMenu", nullptr,
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

		ImGui::Text("Tools:");
		if (ImGui::Button("Pencil"))
			ToolManager::SetTool(TOOL_PENCIL);
		if (ImGui::Button("Line"))
			ToolManager::SetTool(TOOL_LINE);
		if (ImGui::Button("Shape"))
			ToolManager::SetTool(TOOL_SHAPE);

		ImGui::End();


		ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x-250, 18));
		ImGui::SetNextWindowSize(ImVec2(250, ImGui::GetIO().DisplaySize.y-18));
		ImGui::Begin("RightMenu", nullptr,
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

		ImGui::Text("The right menu");
		ImGui::ColorEdit4("My Color", (float*)&CanvasManager::color);
		ImGui::ColorEdit4("Background", (float*)&backgroundColor);
		ImGui::Text(("X: " + std::to_string(go->pos.x)).c_str());
		ImGui::Text(("Y: " + std::to_string(go->pos.y)).c_str());

		if (ImGui::Button("Press"))
			DLOG("OK")
		ImGui::End();

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Shortcut")) {}
				if (ImGui::MenuItem("Open", "Shortcut")) {}
				if (ImGui::MenuItem("Save", "Shortcut")) {}
				if (ImGui::MenuItem("Exit")) {}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{

				ImGui::EndMenu();
			}


			ImGui::EndMainMenuBar();
		}

		ImDrawList* draw = ImGui::GetForegroundDrawList();
		draw->AddText(ImVec2(ImGui::GetIO().DisplaySize.x - 92, 3), IM_COL32(255, 255, 255, 255),
			("FPS: " + std::to_string(displayedFPS)).c_str());

		//ImGui::ShowDemoWindow();
		

		
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		glfwSwapBuffers(window);

		if (glfwWindowShouldClose(window)) {
			IDLOG("Exiting application...")

				//should it end(?) logic goes here
				//glfwSetWindowShouldClose(window, GLFW_FALSE); //use when cancelling to save modified file being opened etc.

				//it should:
			runApp = false;
			}

		
	}
	//destruction
	
	glfwDestroyWindow(window);

	glfwTerminate();
	//exit(EXIT_SUCCESS); //prevents destructors of classes in mainApp from running 


	
}

