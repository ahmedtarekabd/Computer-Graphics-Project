#version 330

// The texture holding the scene pixels
uniform sampler2D tex;

// Read "assets/shaders/fullscreen.vert" to know what "tex_coord" holds;
in vec2 tex_coord;

out vec4 frag_color;

// Vignette is a postprocessing effect that darkens the corners of the screen
// to grab the attention of the viewer towards the center of the screen

void main(){
    //TODO: Modify this shader to apply vignette
    // To apply vignette, divide the scene color
    // by 1 + the squared length of the 2D pixel location the NDC space
    // Hint: remember that the NDC space ranges from -1 to 1
    // while the texture coordinate space ranges from 0 to 1
    // We have the pixel's texture coordinate, how can we compute its location in the NDC space?
    
    // frag_color = texture(tex, tex_coord) / (tex_coord ** 2);
    frag_color = texture(tex, tex_coord);

    // // Sample the original color from the screen texture
    // vec4 originalColor = texture(tex, tex_coord);

    // // Apply vintage effect: Adjust contrast, desaturate, add sepia tone
    // vec3 sepiaColor = vec3(1.2, 1.0, 0.8); // Sepia color tone

    // vec3 resultColor = originalColor.rgb;
    // resultColor = mix(resultColor, sepiaColor, 0.3); // Mix in sepia tone
    // resultColor = mix(resultColor, vec3(1.0), 0.2); // Adjust contrast
    // resultColor = mix(resultColor, vec3(dot(resultColor, vec3(0.333))), 0.2); // Desaturate

    // // Combine the result color with the original alpha channel
    // FragColor = vec4(resultColor, originalColor.a);


}