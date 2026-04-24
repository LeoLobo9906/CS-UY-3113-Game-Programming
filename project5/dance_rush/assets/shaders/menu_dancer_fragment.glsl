#version 330

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D texture0;
uniform float time;

void main()
{
    vec4 tex = texture(texture0, fragTexCoord);
    float pulse = 0.1 * sin(time * 3.0);
    finalColor = vec4(tex.rgb + pulse, tex.a);
}