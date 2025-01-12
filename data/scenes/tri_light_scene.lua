clearColor = { 0.012, 0.02, 0.014, 1.0 }

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
            name = "lightMaterial",
            vertexShader = "basicVertexShader",
            fragmentShader = "lightFragmentShader",
            supportLight = true
        }
    },
    models = {
        {
            name = "monkey",
            model = "monkeyModel",
            pos = { 0.0, 0.0, 0.0 },
            sca = { 1.0, 1.0, 1.0 },
            rot = { 0.0, 0.0, 0.0 },
            material = "lightMaterial",
            uniforms = {
                colour = { 0.23, 0.21, 0.25, 1.0 }
            },
            behaviors = {
                {
                    name = "YRotatingBehavior",
                    rotatingAxis = { 0, 1, 0 },
                    rotationSpeed = 8
                }
            }
        }
    },
    lights = {
        {
            pos = { 0.0, 0.0, 2.0 },
            dir = { 0.0, 0.0, -1.0 },
            up = { 0.0, 1.0, 0.0 },
            colour = { 0.9, 0.01, 0.02, 1.0 },
            intensity = 250.0,
            specularPower = 128.0,
            directional = false
        },
        {
            pos = { 5.0, 0.0, 0.0 },
            dir = { -1.0, 0.0, 0.0 },
            up = { 0.0, 1.0, 0.0 },
            colour = { 0.01, 0.9, 0.02, 1.0 },
            intensity = 250.0,
            specularPower = 128.0,
            directional = false
        },
        {
            pos = {-5.0, 0.0, 0.0 },
            dir = { 1.0, 0.0, 0.0 },
            up = { 0.0, 1.0, 0.0 },
            colour = { 0.01, 0.02, 0.9, 1.0 },
            intensity = 250.0,
            specularPower = 128.0,
            directional = false
        }
    }
}

monkeyModel = {
    file = "data/models/nonormalmonkey.obj"
}

camera = {
    pos = { 0, 0, 6 },
    dir = { 0, 0, -1 },
    up = { 0, 1, 0 },
    right = { 1, 0, 0 },
    angle = { -90, 0 }
}
