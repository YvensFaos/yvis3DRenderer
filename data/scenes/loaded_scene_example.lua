clearColor = { 0.1, 0.2, 0.3, 1.0 }

basicVertexShader = [[
    #version 400
    layout (location = 0) in vec3 vertex;
    layout (location = 1) in vec3 normal;
    layout (location = 2) in vec2 uv;

    out vectorOut {
        vec3 vPosition;
        vec3 vNormal;
        vec2 vUv;
    } vectorOut;

    uniform mat4 model;
    uniform mat4 viewProjection;

    void main()
    {
        vectorOut.vPosition = vec3(model * vec4(vertex, 1.0));
        vectorOut.vNormal = mat3(transpose(inverse(model))) * normal;
        vectorOut.vUv = uv;
        vec4 vecOut = vec4(vectorOut.vPosition, 1.0);

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

normalFragmentShader = [[
    #version 400

    in vectorOut {
        vec3 vPosition;
        vec3 vNormal;
        vec2 vUv;
    } vectorIn;
    out vec4 frag_colour;

    void main()
    {
        frag_colour = vec4(vectorIn.vNormal, 1.0);
    }
]]


lightFragmentShader = [[
    #version 400

    struct Light {
        vec4 color;
        vec3 position;
        float intensity;
        vec3 direction;
        float specularPower;
        bool directional;
    };

    in vectorOut {
        vec3 vPosition;
        vec3 vNormal;
        vec2 vUv;
    } vectorIn;

    float lightConstant = 1.0f;
    float lightLinear = 0.09f;
    float lightQuadratic = 0.032f;
    float maximumIntensity = 100.0f;

    uniform int numberPointLights;
    uniform int numberDirectionLights;

    uniform Light pointLights[10];
    uniform Light directionalLights[10];
    uniform vec4 colour;

    out vec4 frag_colour;

    vec4 calculatePointLight(int index, const vec3 norm) {
        vec3 directionV = pointLights[index].position - vectorIn.vPosition;
        float distance = length(directionV);
        float attenuationIntensity = pointLights[index].intensity / maximumIntensity;
        lightConstant = 1.0f   / attenuationIntensity;
        lightLinear = 0.09f    / attenuationIntensity;
        lightQuadratic = 0.032f/ attenuationIntensity;
        float attenuation = 1.0 / (lightConstant + lightLinear * distance + lightQuadratic * (distance * distance));
        vec3 lightDir = normalize(directionV);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse =  vec3(colour) * attenuation * diff * pointLights[index].color.rgb;
        return vec4(diffuse, 1.0);
    }

    vec4 calculateDirectionalLight(int index, const vec3 norm) {
        float distance = length(directionalLights[index].position - vectorIn.vPosition);
        float attenuationIntensity = directionalLights[index].intensity / maximumIntensity;
        lightConstant = 1.0f    / attenuationIntensity;
        lightLinear = 0.09f     / attenuationIntensity;
        lightQuadratic = 0.032f / attenuationIntensity;
        float attenuation = 1.0 / (lightConstant + lightLinear * distance + lightQuadratic * (distance * distance));
        float diff = max(dot(norm, directionalLights[index].direction), 0.0);
        vec3 diffuse =  vec3(colour) * attenuation * diff * directionalLights[index].color.rgb;
        return vec4(diffuse, 1.0);
    }

    void main()
    {
        vec3 norm = normalize(vectorIn.vNormal);
        int clampedNumberOfDirectional = min(numberDirectionLights, 10);

        vec4 directionalLightResultant = vec4(0.0);
        for(int i = 0; i < clampedNumberOfDirectional; i++)
        {
            directionalLightResultant += calculateDirectionalLight(i, norm);
        }
        vec4 pointLightResultant = vec4(0.0);
        int clampedNumberOfPoint = min(numberPointLights, 10);
        for(int i = 0; i < clampedNumberOfPoint; i++)
        {
            pointLightResultant += calculatePointLight(i, norm);
        }

        vec4 resultantLight = directionalLightResultant + pointLightResultant;
        resultantLight.a = 1.0;

        frag_colour = resultantLight;
    }
]]

scene = {
    materials = {
        {
            name = "simpleColorMaterial",
            vertexShader = "basicVertexShader",
            fragmentShader = "colorFragmentShader",
            supportLight = false
        },
        {
            name = "lightMaterial",
            vertexShader = "basicVertexShader",
            fragmentShader = "lightFragmentShader",
            supportLight = true
        },
        {
            name = "normalMaterial",
            vertexShader = "basicVertexShader",
            fragmentShader = "normalFragmentShader",
            supportLight = false
        }
    },
    models = {
        {
            name = "greenMonkey",
            model = "monkeyModel",
            pos = { -2.0, 0.5, -4.0 },
            sca = { 1.0, 1.0, 1.0 },
            rot = { 20.0, 0.0, 0.0 },
            material = "simpleColorMaterial",
            uniforms = {
                colour = { 33.0 / 255, 217.0 / 255, 82.0 / 255, 1.0 }
            },
            behaviors = {
                {
                    name = "YRotatingBehavior",
                    rotatingAxis = { 0, 1, 0 },
                    rotationSpeed = 8.5
                }
            }
        }
    ,
        {
            name = "pinkMonkey",
            model = "monkeyModel",
            material = "lightMaterial",
            uniforms = {
                colour = { 227.0 / 255, 82.0 / 255, 210.0 / 255, 1.0 }
            },
            pos = { 2.0, 0.5, -4.0 },
            sca = { 1.0, 1.0, 1.0 },
            rot = { 0.0, 0.0, 0.0 },
            behaviors = {
                {
                    name = "YRotatingBehavior",
                    rotatingAxis = { 1, 0, 0.2 },
                    rotationSpeed = -15
                }
            }
        },
        {
            name = "purpleMonkey",
            model = "monkeyModel",
            material = "normalMaterial",
            uniforms = { },
            pos = { 0.0, 0.5, -2.0 },
            sca = { 1.0, 1.0, 1.0 },
            rot = { 0.0, 0.0, 0.0 },
            behaviors = {
                {
                    name = "YRotatingBehavior",
                    rotatingAxis = { 0.1, 1, 0 },
                    rotationSpeed = -10
                }
            }
        }
    },
    lights = {
        {
            pos = { 0.0, 0.0, 5.0 },
            dir = { 0.0, 0.0, -1.0 },
            up = { 0.0, 1.0, 0.0 },
            colour = { 1.0, 0.0, 0.0, 1.0 },
            intensity = 100.0,
            specularPower = 128.0,
            directional = true
        },
        {
            pos = { 0.25, 0.5, -2.0 },
            dir = { 0.0, 0.0, -1.0 },
            up = { 0.0, 1.0, 0.0 },
            colour = { 0.1, 0.4, 0.3, 1.0 },
            intensity = 400.0,
            specularPower = 32.0,
            directional = false
        },
        {
            pos = { 0.0, 2.0, 0.0 },
            dir = { 0.0, -1.0, 0.0 },
            up = { 0.0, 0.0, 1.0 },
            colour = { 0.9, 0.82, 0.0, 1.0 },
            intensity = 600.0,
            specularPower = 1024.0,
            directional = true
        }
    }
}

monkeyModel = {
    file = "data/models/monkey.fbx"
}

camera = {
    pos = { -9.998, 4.372, 16.406 },
    dir = { 0.384, -0.222, -0.896 },
    up = { 0.088, 0.975, -0.204 },
    right = { 0.919, -0.000, 0.394 },
    angle = { -66.793, -12.836 }
}
