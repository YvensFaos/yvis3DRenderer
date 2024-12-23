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
        simpleColorMaterial = {
            vertexShader = "basicVertexShader",
            fragmentShader = "colorFragmentShader"
        }
    },
    models = {
        greenMonkey = {
            model = "monkeyModel",
            pos = {-2.0, 0.5, -4.0},
            sca = {1.0, 1.0, 1.0},
            rot = {0.0,  0.0, 0.0},
            material = "simpleColorMaterial",
            uniforms = {
                colour = {
                    type = "vec4",
                    values = {33 / 255, 217 / 255, 82 / 255, 1.0}
                }
            }
        },
        pinkMonkey = {
            model = "monkeyModel",
            pos = {2.0, 0.5, -4.0},
            sca = {1.0, 1.0, 1.0},
            rot = {0.0,  0.0, 0.0},
            material = "simpleColorMaterial",
            uniforms = {
                colour = {
                    type = "vec4",
                    values = {227 / 255, 82 / 255, 210 / 255, 1.0}
                }
            }
        }
    }
}

monkeyModel = {
    file = "data/models/monkey.fbx"
}

camera = {
    pos   = {1.030, 0.303, 3.532},
    dir   = {-0.277, -0.042, -0.960},
    up    = {-0.012, 0.999, -0.040},
    right = {0.961, 0.000, -0.278},
    angle = {-106.120, -2.405}
}