# Development record

## Shaders and the Rendering pipeline
中文翻譯: 著色與渲染
- shaders (著色器)
- rendering (渲染)

## What is the rendering pipeline

- The rendering pipeline is a series of stages that take place in order to render an image to the screen
- Four stages are programmable via `shaders`
- Shaders are pieces of code written in GLSL(OpenGL shading Language), or HLSL(High-lebel shading language) if you’re using Direct 3D
- GLSL is based on C, HLSL is in different architecture

## The rendering pipeline stages

1. Vertex(頂點) specification
2. Vertex shader (programmable)
3. Tessellation (programmable)
    - tessellation: the [process](https://dictionary.cambridge.org/zht/%E8%A9%9E%E5%85%B8/%E8%8B%B1%E8%AA%9E-%E6%BC%A2%E8%AA%9E-%E7%B9%81%E9%AB%94/process) of [fitting](https://dictionary.cambridge.org/zht/%E8%A9%9E%E5%85%B8/%E8%8B%B1%E8%AA%9E-%E6%BC%A2%E8%AA%9E-%E7%B9%81%E9%AB%94/fitting) [shapes](https://dictionary.cambridge.org/zht/%E8%A9%9E%E5%85%B8/%E8%8B%B1%E8%AA%9E-%E6%BC%A2%E8%AA%9E-%E7%B9%81%E9%AB%94/shape) together in a [pattern](https://dictionary.cambridge.org/zht/%E8%A9%9E%E5%85%B8/%E8%8B%B1%E8%AA%9E-%E6%BC%A2%E8%AA%9E-%E7%B9%81%E9%AB%94/pattern) with no [spaces](https://dictionary.cambridge.org/zht/%E8%A9%9E%E5%85%B8/%E8%8B%B1%E8%AA%9E-%E6%BC%A2%E8%AA%9E-%E7%B9%81%E9%AB%94/space) in between, or an [arrangement](https://dictionary.cambridge.org/zht/%E8%A9%9E%E5%85%B8/%E8%8B%B1%E8%AA%9E-%E6%BC%A2%E8%AA%9E-%E7%B9%81%E9%AB%94/arrangement) of [shapes](https://dictionary.cambridge.org/zht/%E8%A9%9E%E5%85%B8/%E8%8B%B1%E8%AA%9E-%E6%BC%A2%E8%AA%9E-%E7%B9%81%E9%AB%94/shape) that has been made in this way
    - 中文翻譯：鑲嵌
4. Geometry shader (programmable)
5. Vertex post-processing
6. Primitive assembly
7. Rasterization
8. Fragment shader (programmable)
9. Pre-sample operations

## Vertex specification

- A **vertex** is a point in space, usually defined with x, y, z coordinates
- A **primitive** is a simple shape defined using one or more vertices
- Usually we use triangles, but we can also use points, lines and quads
- Vertex specification: Setting up the data of the vertices for the primitves we want to render
- Done in the application itself
- Using VAOs (Vertex Array Objects) and VBOs (Vertex Buffer Objects)
- VAO defined WHAT data a vertex has (position, colour, texture, normals, etc.)
- VBO defines the data itself
- Attribute pointers defined where and ow shaders can access vertex data

### Flow: Creating VAO / VBO

- Generate a VAO ID
- Bind the VAO with that ID
- Generate a VBO ID
- Bind the VBO with that ID (now you’re working on the chosen VBO attached to the chosen VAO)
- Attach the vertex data to that VBO
- Define the attribute pointer formatting
- Enable the attribute pointer
- Unbind the VAO and VBO ready for the next object to be bound

### Flow: Initiating Draw

- Activate shader program you want to use
- Bind the VAO of object you want to draw
- Call `glDrawArrays`, which initiates the reset of the pipeline

### Flow: vertex shader

- Handles vertices individually
- Not option
- must store something in `gl_Position` as it is used by later stages
- can specify additional outputs that can be picked up and used by user-defined shaders later in pipeline
- Inputs consist of the vertex data itself

Example

```cpp
#version 330

layout (location = 0) in vec3 pos;

void main() {

	gl_Psotition = vec4(pos, 1.0);

}
```

### Tessellation

- Allows you to divide up data in to smaller primitives
- Relatives new shader type, appeared in OpenGL4.0
- Can be used to add higher levels of detailed dynamically
- Won’t be used in this course

### Geometry Shader

- Vertex shader handles vertices, geometry shader handles primitives (group of vertices)
- Takes primitives then “emit” their vertices to create the given primitive, or even new primitives
- Can alter data given to it to modify given primitives, or even create new ones
- Can even alter the primitives type (points, lines, triangles etc.)
- Will use it once briefly in this course

### Vertex Post-Processing

- Transform Feedback (if enabled):
    - Result of Vertex and Geometry stages saved to buffers for later use
    - we won’t be using this though
- Clipping:
    - Primitives that won’t be visible are removed (don’t want to draw things we can’t see)
    - Positions converted from “clip-space” to “window space”

### Primitive Assembly

- Vertices are converted into a series of primitives
- So if rendering triangle… 6 vertices would become 2 triangles
- Face culling
    - Face culling is the removal of primitives that can’t be seen, or are facing “away” from the viewer. We don’t want to draw something if we cannot see it

### Rasterization

- Converts primitives into “Fragments”
- Fragments are pieces of data for each pixel, obtained from the rasterization process
- Fragment data will be interpolated based on its position relative to each vertex

### Fragment shader

- Handles data for each fragment
- Is optional but it’s rare to not use it. Exceptions are cases where only depth or stencil data is required (more on data later)
- Most important output is the colour of the pixel that the fragment covers
- Simplest opengl programs usually have a vertex shader and a fragment shader

```cpp
#version 330

out vec4 colour;

void main() {

	colour = vec4(1.0, 0.0, 0.0, 1.0);

}
```

## Pre-sample operation

- Series of tests run to see if the gragment should be drawn
- Most important test: depth test. Determines if something is in front of the point being drawn
- Colour blending: Using defined operations, fragment colours are “blended” together with overlapping fragments. Usually used to handle transparent objects
- Fragment data written to currently bound Framebuffer (usually the default buffer, more on this later)
- Lastly, in the application code the user usually defines a buffer swap here, putting the newly updated framebuffer to the front.
- The pipeline is complete

### On the origin of shaders

- Shaders programs are a group of shaders (vertex, tessellation, geometry, fragment…) associated with one another
- They are created in OpenGL via a series of functions

## Creating a shader program

1. Create empty program
2. Create empty shaders
3. Attach shader source code to shaders
4. Compile shaders
5. Attach shaders to program
6. Link program (creates executables from shaders and links them together)
7. Validate program (optional but highly advised because debugging shaders is a pain)

### Using a shader program

- When you create a shader, an ID is given (like with VAOs and VBOs)
- Simply call `glUseProgram` (shaderID)
- All draw calls from then on will use that shader, `glUseProgram` is used on a new shaderID, or on ‘0’ (meaning  ‘no shader’)

## Summary

- Rendering pipeline consists of several stages
- Four stages are programmable via shaders (Vertex Tessellation, Geometry, Fragment)
- Vertex shader is mandatory
- Vertices: User-defined points in space
- Primitives: Groups of vertices that make a simple shape (usually a triangle)
- Fragments: Per-pixel data created from primitives
- Vertex Array Object (VAO): what data a vertex has
- Vertex Buffer Object (VBO): the vertex data itself
- Shader programs are created with at least a vertex shader and then activated before use.