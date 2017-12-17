# The Creator 3D	

[![Build status](https://ci.appveyor.com/api/projects/status/2446x5ohw7wv0yy1?svg=true)](https://ci.appveyor.com/project/sergipa/the-creator-3d)

The Creator 3D is a engine made for the Engines subject of the Polytechnic University of Catalonia. This version is meant create a physics engine

that will be implemented for Project 3.

## Team

The Team consists of Rodrigo de Pedro: https://github.com/rodrigodpl and Daniel Garcia: https://github.com/viriato22

![Team](https://lh3.googleusercontent.com/GPbknJSuKsKD6CADqwi0ot7HHPjJTYEDeaXtC60MMp64_ZDFds1pmzKgmAUNxLV2-q7gf5qk1Yd3FfYFN_uhFq315Wg8tK141aqRh923aAAMrVvBpR6IrL0Fxb8ZJkTshDsoW4ahprdxlNhJnIzw64fo4cq5sVVeppC-iBzzodsYBXpAlFNFBMTO3JWfx0P_XjUOnm98e12-wMRVy6TGXUNuK7IYsbWGtubBTPATC--Ul9chxEobAi85L6UeID_qs2AZMWQh2nUH5cdwdWiTNkIIWtU1CfpI6oFYkQhLjVOzWPE4h9a_1f-nYXgHIxK0XEyKxy32ApO2zqb2WPXrWU796Vk-VtwUd0VfCzGsBu3CDfeoPMc9CyDKwhAFwjPJOUHZldlWHxt5TRbzlpMINqSlhUi9Vqb45TBpkb6ui1W4Mlhr_vVQETW601pNCM-Dxlxwn8D3pF98UW2869ux36j7AmnQDy8--BWKqtGkmaVkm-N5HXvBgd81hoC40JSQWgytN5hsEw4DsSaYW9kbPBWN92R0Q_WKRWS3gCA0v_hvr7KWJh92CqY0lOPZT1ClOF13OgA0Su69EcGE550rwxoYsNa67CDIuJ5b59h8lA=w780-h655-no)

The original creators of this engine were Sergi Pérez: https://github.com/sergipa and Adrià Martín: https://github.com/TinoTano

## Main core sub-system

 * Component: The component module handles the interaction between the modules and the UI
 * ModuleAudio: This module allows the use of audio inside the engine
 * ModuleCamera3D: This module is used to create and modify cameras.
 * ModuleEditor: This handles the UI and allows access to the different components
 * ModuleFileSystem: This allows the interaction between our engine and the system to create, read and modify files
 * ModuleInput: Allows us to use the keyboard and mouse to create certain functions needed for the engine such as using the arrow keys to move the camera
 * ModuleMaterialImporter: This module is used to load materials to use in the 3d models
 * ModulePrefabImporter: Allows to import models with their materials
 * ModuleRenderer3D: Draws the 3D components on a window
 * ModuleScene: Saves and loads the position of all the 3d objects with all of their components
 * ModuleTextureImporter: This module is used to import certain image formats to use in the 3d models
 * ModuleTime: Allows us to do certain actions acording to the time since opening the application
 * ModuleWindow: This is used to create a window where we will execute or application
 
## Last assignment sub-system

 * ModulePhysics: This module was created to allow the interaction between our application and the [Bullet](https://pybullet.org/wordpress/) library.
 * ComponentRigidBody: This component is used to create rigidbodies to use in our engine. This means that objects assigned this component will start behaving accorging to the laws of physics. 
 * ComponentCollider:  
 
## License

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/>
