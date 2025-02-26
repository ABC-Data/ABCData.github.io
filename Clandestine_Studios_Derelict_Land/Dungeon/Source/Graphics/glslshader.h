/* !
@file    glslshader.h
@author  louismineo.l@digipen.edu
@date    02/09/2022

This file contains definitions of member functions of class GLShader.
The member functions help to call for OpenGL's API to create the shader program
that will be invoked by the object. Theses functions range from compiling,
linking and validation of the shader program from its respective shader source,
logging of errors if the shader program does not work as intended,to the 
setting of unifrom varibles into the shader files and the use of the Shader
Program to draw certain meshes or data from VBO.


*//*__________________________________________________________________________*/

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GLSLSHADER_H
#define GLSLSHADER_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include <GL/glew.h> // for access to OpenGL API declarations 
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>

/*  _________________________________________________________________________ */
class GLSLShader
    /*! GLSLShader class.
    */
{
public:

    // default ctor required to initialize GLSLShader object to safe state
    GLSLShader() : pgm_handle(0), is_linked(GL_FALSE) {
    }

    /*!*****************************************************************************
    \brief
        CompileLinkValidate() compiles the the individual shader sources and also
        links the multiple shader objects into a executable shader program

        It opens the shader file via the full path to the file, and notes of its
        corresponding shader program type: Vertex, Fragment, Geometry or Tessellation

        The information is retrieved from the
        std::vector<std::pair<GLenum, std::string>> vec // FIRST PARAM

        For each shader file, CompileShaderFromFile() is called to link these shader
        files, then link() which will create the executable shader program.

        Lastly, it will call Validate() is ensure if the program is executable
        in the current OpenGL state.


    \param	vec
        a vector of pairs containing the <shader program type , file path to the shader >
    \return
        - GLFALSE if linking fails
        - GLFALSE if validation fails ( meaning the shader program is unable to
            execute)
        - GL True if shader program is executable
    *******************************************************************************/
    GLboolean CompileLinkValidate(std::vector<std::pair<GLenum, std::string>>);

    /*!*****************************************************************************
\brief
    CompileShaderFromFile() compiles the the individual shader files and also
    links the multiple shader objects into a executable shader program

    It loads the shader code from the file based from the file path, loads it
    into the shader_file ifstream

    the string buffer, contains the text from the shader file

    It then compiles the buffer using CompilerShaderFromString

    Compilation statuses and messages are logged into the log_string

    If compilation is successful, the shader object is then attached to the
    previously created shader program object


\param	shader_type
    the is the shader type of the shader file (VERT, FRAG etc)

\param	file_name
    the is the char* string to the shader file's path

\return
    - GLFALSE if shader file is not found
    - GLFALSE if shader file is unable to be opened
    - GLFALSE if shader program is unable to be created
    - GLTRUE or GLFALSE depending on the response of CompileShaderFromString()
*******************************************************************************/
    GLboolean CompileShaderFromFile(GLenum shader_type, std::string const& file_name);

/*!*****************************************************************************
\brief
    CompileShaderFromString() first checks if there is an existing shader
    program object, if not a new shader program is created using
    glCreateProgram()

    It loads the shader code from the 2ND PARAM, shader_src

    the type of GLSLShader depends on the shader_type:
    VERTEX SHADER, FRAGMENT SHADER, GEOMETRY SHADER, TESSELATION CONTROL
    or TESSELATION EVAULATION

    If an incorrect shader file type is parsed into the function, GLFALSE is
    returned and the function exits.

    The function then uses glShaderSource and glCompileShader to compile
    the shader code

    Compilation status of the shader source is also logged using log_string.
    GLFALSE is returned if compilation fails and the crash record is added into
    log_string which will be printed later.

    If compilation checks are all passed, the compiled shader code is attached
    to the newly-made or existing shader program and GLTRUE is returned.


\param	shader_type
    the is the shader type of the shader file (VERT, FRAG etc)

\param	shader_src
    this is the shader code which will be compiled.

\return
    - GLFALSE if shader program is unable to be created.
    - GLFALSE if shader type is incorrect
    - GLFALSE if shader code is unable to be compiled
    - GLTRUE if the compiled shader code is successfully attached to a shader
        program
*******************************************************************************/
    GLboolean CompileShaderFromString(GLenum shader_type, std::string const& shader_src);

/*!*****************************************************************************
\brief
    Link() will link the shader objects to the shader program by checking the
    pgm_handle

    if the pgm_handle is invalid, Link() will return GLFALSE;

    the shader program checked if it is linked using glGetProgramiv,
    if it returns 0, means that the shaderprogram is not successfully linked,
    reasons of linkage failed is logged into log_string and GL FALSE is RETURNED

    if linkage is successful, is_linked is flagged as TRUE and the function
    returns GLTRUE


\return
    - GLTRUE if shader program is already linked
    - GLFALSE if shader program's pgm_handle is invalid
    - GLFALSE if the program tries to link the existing shader code and still
        is invalid
    - GLTRUE if the program tries to link and is successful.
*******************************************************************************/
    GLboolean Link();

/*!*****************************************************************************
\brief
    Use() will check if shader program is valid and linked successfully, and
    then calls glUseProgram to execute the shader program
*******************************************************************************/
    void Use();

/*!*****************************************************************************
\brief
    UnUse() will stop the execution of the shader program and uninstalls it
    by replacing it with glUseProgram(0).
*******************************************************************************/
    void UnUse();

/*!*****************************************************************************
\brief
    Validate() will check if the current shader program can execute in the
    current OpenGL state.

    if shader program does not exist or is not linked
    GL_FALSE is returned.

    It utilises the openGL API's function, glValidateProgram to validate its
    executability.

    if validation fails, status is flagged false and its reasons are logged into
    log_string to be printed later.

    GL_TRUE is returned shader program passes validation.

return
    - GL_FALSE if shader program is not linked or non-existent
    - GL_FALSE if shader program fails the validation check
    - GL_TRUE if the shader program passes the validation check

*******************************************************************************/
    GLboolean Validate();

/*!*****************************************************************************
\brief
    GetHandle() returns the handle of the shader program
\return
    GLuint value , depending on the OpenGL's API
*******************************************************************************/
    GLuint GetHandle() const;

/*!*****************************************************************************
\brief
    GetHandle() is a checker function which return the value of is_linked
\return
    GLboolean value of is_linked
*******************************************************************************/
    GLboolean IsLinked() const;

/*!*****************************************************************************
\brief
    GetLOG() will return the string from log_string, which can be used to print
    the logging information from the compilation, validation and linking
    processes.
\return
    string of logging information from log_string
*******************************************************************************/
    std::string GetLog() const;

/*!*****************************************************************************
\brief
    BindAttribLocation() utilises the OpenGL's API function
    glBindAttribLocation() to dynamically associate a generic vertex attribute
    index with a named in attribute variable.
\param

\param index
    index of the vertex in location
\param name
    name to be binded to the index of the vertex
*******************************************************************************/
    void BindAttribLocation(GLuint index, GLchar const* name);

/*!*****************************************************************************
\brief
    BindFragDataLocation() utilises the OpenGL's API function
    glBindFragDataLocation() to dynamically associate a fragment shader index
    location to a user-definted varible to write out to.
\param color_number
    color of the fragment
\param name
    name of variable to be binded to the specific fragment
*******************************************************************************/
    void BindFragDataLocation(GLuint color_number, GLchar const* name);

/*!*****************************************************************************
\brief
    DeleteShaderProgram() deletes the shader_program object using
    glDeleteProgram()
*******************************************************************************/
    void DeleteShaderProgram();




    // OpenGL is C-based API and therefore doesn't understand function
    // overloading or templates
    // Therefore, we need a family of functions to specify values of uniform
    // variables of different types for the current program object
/*!*****************************************************************************
\brief
    this SetUniform() function parses the value of a boolean variable into
    into the shader program of the same variable name
\param name
    name of variable in shader program
\param val
    value of boolean variable
*******************************************************************************/
    void SetUniform(GLchar const* name, GLboolean val);
/*!*****************************************************************************
\brief
    this SetUniform() function parses the value of a integer variable into
    into the shader program of the same variable name
\param name
    name of variable in shader program
\param val
    value of integer variable
*******************************************************************************/
    void SetUniform(GLchar const* name, GLint val);
/*!*****************************************************************************
\brief
    this SetUniform() function parses the value of a float variable into
    into the shader program of the same variable name
\param name
    name of variable in shader program
\param val
    value of float variable
*******************************************************************************/
    void SetUniform(GLchar const* name, GLfloat val);
/*!*****************************************************************************
\brief
    this SetUniform() function parses the values of 2 float variable into
    into the shader program 's varible of name (1st param) and forms a vec2
    in the shader program
\param name
    name of variable in shader program
\param x
    value of the 1st element of the vec2
\param y
    value of the 2nd element of the vec2
*******************************************************************************/
    void SetUniform(GLchar const* name, GLfloat x, GLfloat y);
/*!*****************************************************************************
\brief
    this SetUniform() function parses the values of 2 float variable into
    into the shader program 's varible of name (1st param) and forms a vec2
    in the shader program
\param name
    name of variable in shader program
\param x
    value of the 1st element of the vec3
\param y
    value of the 2nd element of the vec3
\param z
    value of the 3rd element of the vec3
*******************************************************************************/
    void SetUniform(GLchar const* name, GLfloat x, GLfloat y, GLfloat z);
/*!*****************************************************************************
\brief
    this SetUniform() function parses the values of 2 float variable into
    into the shader program 's varible of name (1st param) and forms a vec2
    in the shader program
\param name
    name of variable in shader program
\param x
    value of the 1st element of the vec3
\param y
    value of the 2nd element of the vec3
\param z
    value of the 3rd element of the vec3
\param w
    value of the 4th element of the vec3
*******************************************************************************/
    void SetUniform(GLchar const* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
/*!*****************************************************************************
\brief
    this SetUniform() function parses the values of a glm vec2 into the shader
    program 's varible of name (1st param) and forms a vec2 in the shader
    program
\param name
    name of variable in shader program
\param val
    glm vec2 values to be parsed into
*******************************************************************************/
    void SetUniform(GLchar const* name, glm::vec2 const& val);
/*!*****************************************************************************
\brief
    this SetUniform() function parses the values of a glm vec3 into the shader
    program 's varible of name (1st param) and forms a vec3 in the shader
    program
\param name
    name of variable in shader program
\param val
    glm vec3 values to be parsed into
*******************************************************************************/
    void SetUniform(GLchar const* name, glm::vec3 const& val);
/*!*****************************************************************************
\brief
    this SetUniform() function parses the values of a glm vec4 into the shader
    program 's varible of name (1st param) and forms a vec4 in the shader
    program
\param name
    name of variable in shader program
\param val
    glm vec4 values to be parsed into
*******************************************************************************/
    void SetUniform(GLchar const* name, glm::vec4 const& val);
/*!*****************************************************************************
\brief
    this SetUniform() function parses the values of a glm mat3 into the shader
    program 's varible of name (1st param) and forms a mat3 in the shader
    program
\param name
    name of variable in shader program
\param val
    glm mat3 values to be parsed into
*******************************************************************************/
    void SetUniform(GLchar const* name, glm::mat3 const& val);
/*!*****************************************************************************
\brief
    this SetUniform() function parses the values of a glm mat4 into the shader
    program 's varible of name (1st param) and forms a mat4 in the shader
    program
\param name
    name of variable in shader program
\param val
    glm mat4 values to be parsed into
*******************************************************************************/
    void SetUniform(GLchar const* name, glm::mat4 const& val);


/*!*****************************************************************************
\brief
    PrintActiveAttribs() prints the list of active vertex attributes used by
    the vertex shader
*******************************************************************************/
    void PrintActiveAttribs() const;

/*!*****************************************************************************
\brief
    PrintActiveUniforms prints the active uniform variables
*******************************************************************************/
    void PrintActiveUniforms() const;

private:
    enum ShaderType {
        VERTEX_SHADER = GL_VERTEX_SHADER,
        FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
        GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
        TESS_CONTROL_SHADER = GL_TESS_CONTROL_SHADER,
        TESS_EVALUATION_SHADER = GL_TESS_EVALUATION_SHADER,
        // ignore compute shader for now because it is not connected to
        // the graphics pipe
        // COMPUTE_SHADER = GL_COMPUTE_SHADER
    };

    GLuint pgm_handle = 0;  // handle to linked shader program object
    GLboolean is_linked = GL_FALSE; // has the program successfully linked?
    std::string log_string; // log for OpenGL compiler and linker messages

private:
/*!*****************************************************************************
\brief
    GetUniformLocation() is a getter function that returns the location of the
    uniform variable
\param name
    name of the variable
\return
    intger value of the location of the variable in buffer
    - GL_INVALID_VALUE if the program is not a value generated by opengl
    - GL_INVALID_OPERATION if the program is not valid or not successfully linked.
*******************************************************************************/
    GLint GetUniformLocation(GLchar const* name);

/*!*****************************************************************************
\brief
    FileExists() checks if the file exists by checking if it is openable
\param file_name
    name of the file/ file path to the file
\return
    - GL_TRUE , if openable
    - GL_FALSE, if not able to be opened
*******************************************************************************/
    GLboolean FileExists(std::string const& file_name);
};

#endif /* GLSLSHADER_H */
