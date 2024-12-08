vertexShader = [[
    #version 400
    layout (location = 0) in vec3 vertex;
    layout (location = 1) in vec3 normal;
    layout (location = 2) in vec2 uv;
    uniform mat4 mvpMatrix;
    out vec4 nn;

    void main() {
      nn = vec4(normal, 1.0);
      gl_Position = mvpMatrix * vec4(vertex, 1.0);
    }
]]

fragmentShader = [[
    #version 400
    in vec4 nn;
    out vec4 frag_colour;
    void main() {
      frag_colour = vec4(nn.xyz, 1.0);
    }
]]

monkey = {file = "data/models/monkey.fbx",  pos = { 0.0, 0.5, -4.0}, sca = {1.0, 1.0, 1.0}, rot = {0.0,  0.0, 0.0}}
