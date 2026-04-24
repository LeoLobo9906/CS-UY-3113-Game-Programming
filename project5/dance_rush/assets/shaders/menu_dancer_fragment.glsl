#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform float time

out vec4 finalColor;

void main()
{
    vec4 color = texture(texture0, fragTexCoord) * colDiffuse * fragColor;

    // subtle pulsing glow
    float glow = 0.08 + 0.08 * sin(time * 3.0);

    // slight warm tint + glow
    vec3 warmTint = vec3(1.0, 0.92, 0.72);
    vec3 finalRGB = mix(color.rgb, color.rgb * warmTint + vec3(glow), 0.35);

    finalColor = vec4(finalRGB, color.a);
}