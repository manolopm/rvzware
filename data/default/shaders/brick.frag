// Here are the uniforms that we set up access to from our application
// diffuseTexture is set up to get whatever texture is located in texture unit 0
uniform sampler2D diffuseTexture;
 
// We set the value of alpha in the application using the ExampleCallback
uniform float alpha;
 
void main(void)
{      
    // Sample the texture
    vec4 diffuse = texture2D(diffuseTexture, gl_TexCoord[0].xy * alpha);
 
    // Set the color using the texture and the alpha value
    gl_FragColor = vec4(diffuse.rgb, alpha);
}