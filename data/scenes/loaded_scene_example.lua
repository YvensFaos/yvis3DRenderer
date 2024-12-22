clearColor = {0.1, 0.2, 0.3, 1.0}

basicVertexShader = [[
    #version 400
    layout (location = 0) in vec3 vertex;
    layout (location = 1) in vec3 normal;
    layout (location = 2) in vec2 uv;

    uniform mat4 model;
    uniform mat4 viewProjection;

    void main()
    {
        vec4 vecOut = vec4(vec3(model * vec4(vertex, 1.0)), 1.0);
        gl_Position = viewProjection * vecOut;
    }
]]

colorFragmentShader = [[
    #version 400

    uniform vec4 colour;
    out vec4 frag_colour;

    void main()
    {
        frag_colour = colour;
    }
]]

scene = {
    materials = {
        {
            name = "simpleColorMaterial",
            vertexShader = "basicVertexShader",
            fragmentShader = "colorFragmentShader"
        }
    },
    models = {
        {
            name = "greenMonkey",
            model = "monkeyModel",
            pos = {-2.0, 0.5, -4.0},
            sca = {1.0, 1.0, 1.0},
            rot = {0.0,  0.0, 0.0},
            material = "simpleColorMaterial",
            uniforms = {
                colour = {33.0 / 255, 217.0 / 255, 82.0 / 255, 1.0}
            }
        },
        {
            name = "pinkMonkey",
            model = "monkeyModel",
            material = "simpleColorMaterial",
            uniforms = {
                colour = {227.0 / 255, 82.0 / 255, 210.0 / 255, 1.0}
            },
            pos = {2.0, 0.5, -4.0},
            sca = {1.0, 1.0, 1.0},
            rot = {0.0,  0.0, 0.0},
        },
        {
            name = "purpleMonkey",
            model = "monkeyModel",
            material = "simpleColorMaterial",
            uniforms = {
                colour = {172.0 / 255, 21.0 / 255, 214.0 / 255, 1.0}
            },
            pos = {0.0, 0.5, -2.0},
            sca = {1.0, 1.0, 1.0},
            rot = {0.0,  0.0, 0.0},
        }
    }
}

monkeyModel = {
    file = "data/models/monkey.fbx"
}

camera = {
    pos   = {0.256, 0.304, 2.201},
    dir   = {-0.036, 0.013, -0.999},
    up    = {0.000, 1.000, 0.013},
    right = {0.999, 0.000, -0.036},
    angle = {-92.086, 0.725}
}