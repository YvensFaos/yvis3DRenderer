vertexShader = [[
    #version 400
    layout (location = 0) in vec3 vertex;
    layout (location = 1) in vec3 normal;
    layout (location = 2) in vec2 uv;

    out vectorOut {
        vec3 vposition;
        vec3 vnormal;
        vec2 vuv;
    } vectorOut;

    uniform mat4 model;
    uniform mat4 viewProjection;

    void main()
    {
        vectorOut.vposition = vec3(model * vec4(vertex, 1.0));
        vectorOut.vnormal = mat3(transpose(inverse(model))) * normal;
        vectorOut.vuv = uv;
        vec4 vecOut = vec4(vectorOut.vposition, 1.0);

        gl_Position = viewProjection * vecOut;
    }
]]

fragmentShader = [[
    #version 400

    struct Light {
        vec3 position;
        vec3 direction;
        vec4 color;
        float intensity;
        bool directional;
    };

    struct Fog {
        float maxDist;
        float minDist;
        vec4 color;
    };

    in vectorOut {
        vec3 vposition;
        vec3 vnormal;
        vec2 vuv;
    } vectorIn;

    float lightConstant = 1.0f;
    float lightLinear = 0.09f;
    float lightQuadratic = 0.032f;
    float maximumIntensity = 100.0f;

    uniform vec3 cameraPosition;
    uniform Light sceneLight;
    uniform Fog sceneFog;

    out vec4 frag_colour;

    void main()
    {
        vec3 norm = normalize(vectorIn.vnormal);
        float distance = length(sceneLight.position - vectorIn.vposition);

        float attenuationIntensity = sceneLight.intensity / maximumIntensity;
        lightConstant = 1.0f    / attenuationIntensity;
        lightLinear = 0.09f     / attenuationIntensity;
        lightQuadratic = 0.032f / attenuationIntensity;

        float attenuation = 1.0 / (lightConstant + lightLinear * distance + lightQuadratic * (distance * distance));
        float diff = max(dot(norm, sceneLight.direction), 0.0);
        vec3 diffuse =  attenuation * diff * sceneLight.color.xyz;

        distance = length(cameraPosition - vectorIn.vposition);
        float fogFactor = (sceneFog.maxDist - distance) / (sceneFog.maxDist - sceneFog.minDist);
        fogFactor = clamp(fogFactor, 0.0, 1.0);

        frag_colour = mix(sceneFog.color, vec4(diffuse, sceneLight.color.w), fogFactor);
    }
]]

monkey = {
    file = "data/models/monkey.fbx",
    pos = { 0.0, 0.5, -4.0},
    sca = {1.0, 1.0, 1.0},
    rot = {0.0,  0.0, 0.0}
}

cameraPosition = {
    pos   = {1.030, 0.303, 3.532},
    dir   = {-0.277, -0.042, -0.960},
    up    = {-0.012, 0.999, -0.040},
    right = {0.961, 0.000, -0.278},
    angle = {-106.120, -2.405}
}

lightIntensity = 100

light = {
    pos = { 0.0, 4.0, 0.0},
    dir = {0.0, 0.0, -1.0},
    up = {0.0, 1.0, 0.0},
    col = {227 / 255, 118 / 255, 252 / 255, 1.0},
    intensity = lightIntensity,
    specularPower = 256.0,
    directional = true
}

fog = {
    maxDist = 15.0,
    minDist =  2.0,
    color = {1.0, 0.2, 0.3, 1.0}
}