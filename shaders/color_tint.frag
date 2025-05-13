uniform vec4 tintColor;    // Main shape color (outer)
uniform vec4 centerColor;  // Color at the center
uniform float radius;      // Start of blend (center radius)
uniform float edge;        // Width of blend edge (fade amount)

void main()
{
    vec2 localPos = gl_TexCoord[0].xy;
    float dist = length(localPos);

    // Full color from vertex + tint
    vec4 baseColor = gl_Color * tintColor;

    // Blend weight from center to edge
    float blend = 1.0 - smoothstep(radius, radius + edge, dist);

    // Mix centerColor into baseColor
    vec4 finalColor = mix(baseColor, centerColor, blend);

    gl_FragColor = finalColor;
}
