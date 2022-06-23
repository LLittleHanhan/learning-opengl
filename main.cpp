#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <math.h>
#include <random>
#include "stb_image.h"
#include "camera.h"
#include "shader.h"
#include "model.h"




void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);
unsigned int loadCubemap(vector<std::string> faces);


void initSnow(Shader* snowShader, unsigned int* snowVAO, unsigned int* snowVBO, unsigned int* snowEBO);
void drawSnowing(Shader* snowShader, unsigned int* snowVAO, unsigned int* snowVBO, unsigned int* snowEBO, int snowNum);
void drawSingleSnow(Shader* snowShader, float speed, float pox, float poz, float scale, float r, float rx, float ry);
void genSnow(int num);

void initSkybox(Shader* skyboxShader, unsigned int* skyboxVAO, unsigned int* skyboxVBO, unsigned int* skyboxTexture, int index, vector<std::string>& faces);
void drawSkybox(Shader* skyboxShader, unsigned int* skyboxVAO, unsigned int* skyboxVBO, unsigned int* skyboxTexture0, unsigned int* skyboxTexture1);

void drawSnowman(Shader* snowmanShader, Model* snowmanModel);

void initCircle(Shader* circleShader, unsigned int* circleVAO, unsigned int* circleVBO, unsigned int* circleEBO);
void drawCircle(Shader* circleShader, unsigned int* circleVAO, unsigned int* circleVBO, unsigned int* circleEBO);
void getCircleVertices();

void drawTree(Shader* TreeShader, Model* TreeModel);




// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 1200;
// camera
Camera camera(glm::vec3(0.0f, 1.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
// timing
// time between current frame and last frame
float deltaTime = 0.0f;	
float lastFrame = 0.0f;


/****************************/
/*以下给出绘制图形所需的顶点坐标*/
//ground
float groundVertices[] = {
         1.0f,  0.0f,  1.0f,    1.0f,1.0f,
         1.0f,  0.0f, -1.0f,    1.0f,0.0f,
        -1.0f,  0.0f, -1.0f,    0.0f,0.0f,

        -1.0f,  0.0f, -1.0f,    0.0f,0.0f,
        -1.0f,  0.0f,  1.0f,    0.0f,1.0f,
         1.0f,  0.0f,  1.0f,    1.0f,1.0f
};
//snow
float snowVertices[] = {
    0.0f, 0.0f, 0.0f,               0.46f, 0.48f, 0.8f,
    0.05f,0.0288675f,0.0f,          0.46f, 0.48f, 0.8f,

    0.5f,0.0288675f,0.0f,           0.46f, 0.48f, 0.8f,
    0.7f,0.3f,0.0f,                 1.0f,1.0f,1.0f,
    0.75f,0.3f,0.0f,                1.0f,1.0f,1.0f,
    0.55f,0.0288675f,0.0f,          0.46f, 0.48f, 0.8f,

    0.75f,0.0288675f,0.0f,          0.46f, 0.48f, 0.8f,
    0.85f,0.2f,0.0f,                1.0f,1.0f,1.0f,
    0.9f,0.2f,0.0f,                 1.0f,1.0f,1.0f,
    0.8f,0.0288675f,0.0f,           0.46f, 0.48f, 0.8f,

    0.9f,0.0288675f,0.0f,           0.46f, 0.48f, 0.8f,
    1.0f, 0.0f, 0.0f,               0.46f, 0.48f, 0.8f,
};
unsigned int snowIndices[] = {
    0,1,10,
    0,10,11,
    2,3,4,
    2,4,5,
    6,7,8,
    6,8,9
};
float snow[200][7];
//skybox
float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};
//
vector<glm::vec3> circleVertices;
vector<unsigned int> circleIndices;
const int sectorCount = 108;
/****************************/



int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
   


    
  
    /****************************/
    /*以下为初始化函数*/
    getCircleVertices();
    Shader circleShader("circle.vs", "circle.fs");
    unsigned int circleVAO, circleVBO, circleEBO;
    initCircle(&circleShader, &circleVAO, &circleVBO, &circleEBO);

    //snow
    Shader snowShader("snowShader.vs", "snowShader.fs");
    unsigned int snowVAO, snowVBO, snowEBO;
    initSnow(&snowShader, &snowVAO, &snowVBO, &snowEBO);
    int snowNum = 200;
    genSnow(snowNum);
    
   //skybox
    Shader skyboxShader("skyboxShader.vs", "skyboxShader.fs");
    unsigned int skyboxVAO, skyboxVBO;
    unsigned int skyboxTexture0, skyboxTexture1;
    vector<std::string> faces0
    {
        ("./texture/skybox/0/right.png"),
        ("./texture/skybox/0/left.png"),
        ("./texture/skybox/0/top.png"),
        ("./texture/skybox/0/down.png"),
        ("./texture/skybox/0/front.png"),
        ("./texture/skybox/0/back.png")
    };
    vector<std::string> faces1
    {
        ("./texture/skybox/2/right.png"),
        ("./texture/skybox/2/left.png"),
        ("./texture/skybox/2/top.png"),
        ("./texture/skybox/2/down.png"),
        ("./texture/skybox/2/front.png"),
        ("./texture/skybox/2/back.png")
    };
    initSkybox(&skyboxShader, &skyboxVAO, &skyboxVBO, &skyboxTexture0, 0, faces0);
    initSkybox(&skyboxShader, &skyboxVAO, &skyboxVBO, &skyboxTexture1, 1, faces1);
    
    /*assimp*/
    //snowman
    Shader snowmanShader("snowmanShader.vs", "snowmanShader.fs");
    Model snowmanModel("./texture/snowman/snowman.obj");
    //tree
    Shader treeShader("tree.vs", "tree.fs");
    Model treeModel("./texture/tree/tree.obj");
    /****************************/




    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))
    {
       
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);
        // render
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        
        
        /****************************/
        /*以下为绘制函数*/
        drawSnowing(&snowShader, &snowVAO, &snowVBO, &snowEBO,snowNum);
        drawSkybox(&skyboxShader, &skyboxVAO, &skyboxVBO, &skyboxTexture0, &skyboxTexture1);
        drawSnowman(&snowmanShader, &snowmanModel);     
        drawTree(&treeShader, &treeModel);   
        drawCircle(&circleShader, &circleVAO, &circleVBO, &circleEBO);
        /****************************/
        
        
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &circleVAO);
    glDeleteBuffers(1, &circleVBO);  
    glDeleteBuffers(1, &circleEBO);
   
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);
   
    glDeleteVertexArrays(1, &snowVAO);
    glDeleteBuffers(1, &snowVBO);
    glDeleteBuffers(1, &snowEBO);
    
    glfwTerminate();
    return 0;
}



/*draw函数*/

void initSnow(Shader* snowShader, unsigned int* snowVAO, unsigned int* snowVBO, unsigned int* snowEBO) {
    glGenVertexArrays(1, snowVAO);
    glGenBuffers(1, snowVBO);
    glBindVertexArray(*snowVAO);
    glBindBuffer(GL_ARRAY_BUFFER, *snowVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(snowVertices), snowVertices, GL_STATIC_DRAW);

    glGenBuffers(1, snowEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *snowEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(snowIndices), snowIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void drawSnowing(Shader* snowShader, unsigned int* snowVAO, unsigned int* snowVBO, unsigned int* snowEBO,int snowNum) {
    // activate shader
    snowShader->use();

    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    snowShader->setMat4("projection", projection);

    // camera/view transformation
    glm::mat4 view = camera.GetViewMatrix();
    snowShader->setMat4("view", view);

    // render boxes
    glBindVertexArray(*snowVAO);

    //speed,x,z,scale,r,rx,ry
    for (int i = 0; i < snowNum; i++) {
        drawSingleSnow(snowShader, snow[i][0], snow[i][1], snow[i][2], snow[i][3], snow[i][4], snow[i][5], snow[i][6]);
    }
  
    
}

void drawSingleSnow(Shader* snowShader,float speed,float pox,float poz,float scale,float r,float rx,float ry) {
    float rorate[6]{ 0,60,120,180,240,300 };
    float ro = glfwGetTime();
    float poy = 10 - fmod(ro * speed, 10);

    for (int i = 0; i < 6; i++) {
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(pox, poy, poz));
        model = glm::scale(model, glm::vec3(scale, scale, scale));

        model = glm::rotate(model, glm::radians(r), glm::vec3(rx, ry, 0.0));
        model = glm::rotate(model, ro, glm::vec3(0.0, 0.0, 1.0));
        model = glm::rotate(model, glm::radians(rorate[i]), glm::vec3(0.0, 0.0, 1.0));

        snowShader->setMat4("model", model);

        snowShader->setInt("affine", 1);
        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
        snowShader->setInt("affine", -1);
        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
    }
}

void genSnow(int num) {
    std::random_device rd; 
    std::mt19937 gen(rd()); 
    std::uniform_real_distribution<> speed(1.0, 2.0);
    std::uniform_real_distribution<> scale(0.01, 0.05);
    std::uniform_real_distribution<> pox(-5.0, 5.0);
    std::uniform_real_distribution<> poz(-5.0, 5.0);
    std::uniform_real_distribution<> r(0.0, 90.0);
    std::uniform_real_distribution<> rx(0.0, 1.0);
    std::uniform_real_distribution<> ry(0.0, 1.0);
    for (int i = 0; i < num; i++) {
        snow[i][0] = speed(gen);
        snow[i][1] = pox(gen);
        snow[i][2] = poz(gen);
        snow[i][3] = scale(gen);
        snow[i][4] = r(gen);
        snow[i][5] = rx(gen);
        snow[i][6] = ry(gen);

    }
}

void initSkybox(Shader* skyboxShader, unsigned int* skyboxVAO, unsigned int* skyboxVBO, unsigned int* skyboxTexture,int index, vector<std::string>& faces) {
    glGenVertexArrays(1, skyboxVAO);
    glGenBuffers(1, skyboxVBO);
    glBindVertexArray(*skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, *skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    *skyboxTexture = loadCubemap(faces);
    
    skyboxShader->use();
    skyboxShader->setInt("skybox" + to_string(index), index);
}

void drawSkybox(Shader* skyboxShader, unsigned int* skyboxVAO, unsigned int* skyboxVBO, unsigned int* skyboxTexture0, unsigned int* skyboxTexture1){
    // draw skybox as last
    glDepthFunc(GL_LEQUAL);  
    skyboxShader->use();

    float time = fmod(glfwGetTime()*0.05, 2);
    if (time > 1) 
        time = 2 - time; 
    skyboxShader->setFloat("m", time);


    glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
   
    skyboxShader->setMat4("view", view);
    skyboxShader->setMat4("projection", projection);
    
    glBindVertexArray(*skyboxVAO);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, *skyboxTexture0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, *skyboxTexture1);
    
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); 
}

void drawSnowman(Shader* snowmanShader,Model* snowmanModel) {
    snowmanShader->use();
    snowmanShader->setVec3("light.direction", 0.0f, 1.0f, 0.0f);
    snowmanShader->setVec3("light.ambient", 1.0f, 1.0f, 1.0f);
    snowmanShader->setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
    snowmanShader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);


    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    snowmanShader->setMat4("projection", projection);
    snowmanShader->setMat4("view", view);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
    model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
    snowmanShader->setMat4("model", model);
    snowmanModel->Draw(*snowmanShader);
}

void drawTree(Shader* TreeShader, Model* TreeModel) {
    TreeShader->use();

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    TreeShader->setMat4("projection", projection);
    TreeShader->setMat4("view", view);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 2.0f));
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
    TreeShader->setMat4("model", model);
    TreeModel->Draw(*TreeShader);
}

void initCircle(Shader* circleShader, unsigned int* circleVAO, unsigned int* circleVBO, unsigned int* circleEBO) {
    //VAO,VBO,VEO
    glGenVertexArrays(1, circleVAO);
    glGenBuffers(1, circleVBO);

    glBindVertexArray(*circleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, *circleVBO);
    glBufferData(GL_ARRAY_BUFFER, circleVertices.size()*sizeof(glm::vec3), &circleVertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, circleEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *circleEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, circleIndices.size()*sizeof(unsigned int), &circleIndices[0], GL_STATIC_DRAW);
    
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3), (void*)(sizeof(glm::vec3)));
    glEnableVertexAttribArray(1);
}

void drawCircle(Shader* circleShader, unsigned int* circleVAO, unsigned int* circleVBO, unsigned int* circleEBO) {

    // activate shader
    circleShader->use();

    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    circleShader->setMat4("projection", projection);

    // camera/view transformation
    glm::mat4 view = camera.GetViewMatrix();
    circleShader->setMat4("view", view);

    // render boxes
    glBindVertexArray(*circleVAO);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(3.0f, 0.2f, 3.0f));

    circleShader->setMat4("model", model);
    glDrawElements(GL_TRIANGLES, sectorCount*12, GL_UNSIGNED_INT, 0);
}

void getCircleVertices() {

    const float pierRadius = 2.0f;

    const float PI = 3.1415926f;
    float sectorStep = 2 * PI / sectorCount;
    float sectorAngle = 0.0f;

    //上
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 color = glm::vec3(0.9f, 0.9f, 0.9f);
    circleVertices.push_back(position);
    circleVertices.push_back(color);
    
    color = glm::vec3(0.5f, 0.5f, 0.5f);
    for (int i = 0; i <= sectorCount; ++i)
    {
        sectorAngle = i * sectorStep;
        position.x = pierRadius * cos(sectorAngle);
        position.y = 0.0f;
        position.z = pierRadius * sin(sectorAngle);
        circleVertices.push_back(position);
        circleVertices.push_back(color);
    }
    //下

    position = glm::vec3(0.0f, -1.0f, 0.0f);
    color = glm::vec3(0.8f, 0.8f, 0.8f);
    circleVertices.push_back(position);
    circleVertices.push_back(color);
    
    for (int i = 0; i <= sectorCount; ++i)
    {
        sectorAngle = i * sectorStep;
        position.x = pierRadius * cos(sectorAngle);
        position.y = -1.0f;
        position.z = pierRadius * sin(sectorAngle);
        circleVertices.push_back(position);
        circleVertices.push_back(color);
    }


    for (int i = 1; i <= sectorCount; i++) {
        circleIndices.push_back(i);
        circleIndices.push_back(0);
        circleIndices.push_back(i + 1);
    }
    for (int i = 1; i <= sectorCount; i++) {
        circleIndices.push_back(i + sectorCount + 2);
        circleIndices.push_back(sectorCount + 2);
        circleIndices.push_back(i + sectorCount + 3);
    }
    for (int i = 1; i <= sectorCount; i++) {
        circleIndices.push_back(i);
        circleIndices.push_back(i + 1);
        circleIndices.push_back(i + sectorCount + 2);

        circleIndices.push_back(i + sectorCount + 2);
        circleIndices.push_back(i + sectorCount + 3);
        circleIndices.push_back(i + 1);
    }
}


/*input,output,camera*/
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}



/*texture*/
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

unsigned int loadCubemap(vector<std::string> faces){
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}


