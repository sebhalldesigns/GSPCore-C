
// GSPCore AG4 GLSL Shader
// Created by Seb Hall (June 2024) for the GSPCore Project
//
// AG4 (Abstract Graphics 4-Byte Representation) is a key component
// of the GSPCore framework that allows for efficient serialization
// of two-dimensional graphics into a stream of 4-byte units. This
// size is chosen as it is commonly used in graphics shaders and is
// the standard single-point float representation.

#define uint32_t uint

const uint32_t color = 0x0000FFFFu;

const uint32_t redMask   = 0xFF000000u;
const uint32_t greenMask = 0x00FF0000u;
const uint32_t blueMask  = 0x0000FF00u;
const uint32_t alphaMask = 0x000000FFu;

// COMMANDS

const uint32_t AG4_VERSION = 0x0000u;
const uint32_t AG4_BEGIN_ELEMENT = 0x0001u;
const uint32_t AG4_FRAME = 0x0002u;
const uint32_t AG4_CORNER_RADIUS = 0x0003u;
const uint32_t AG4_FILL_COLOR = 0x0004u;
const uint32_t AG4_END_ELEMENT = 0xFFFFu;


// BEGIN_SHAPE MODIFIERS
const uint32_t AG4_ELEMENT_RECTANGLE = 0x01u;
const uint32_t primitiveEllipse = 0x02u;
const uint32_t primitiveLine = 0x03u;
const uint32_t primitiveArc = 0x04u;
const uint32_t primitiveBezier = 0x05u;




struct AG4Rectangle {
    vec4 frame;
    float cornerRadius;
    vec4 fillColor;
};

const uint commands[] = uint[](
    //MMQQCCCC
    0x01000001u,// BEGIN_ELEMENT (RECTANGLE)
    0x00040002u, // FRAME (4 PARAMETERS)
        floatBitsToUint(0.5), 
        floatBitsToUint(0.5),
        floatBitsToUint(0.25),
        floatBitsToUint(0.25),
    0x00010004u, // FILL_COLOR (1 PARAMETER)
        0xFFFF00FFu,
        
    0x00010003u, // FRAME (4 PARAMETERS)
        floatBitsToUint(0.05), 
    
    0xFFFFFFFFu // END_ELEMENT
);

const uint numCommands = 11u;


vec4 AG4_GetColor(uint color) {
    return vec4(color&redMask, color&greenMask, color&blueMask, color&alphaMask);
}


struct AG4ElementResult {
    uint32_t nextIndex;
    vec4 color;
    bool success;
};

// possible unit types:
// Control Unit (packed into uint32)
// Color (packed into uint32)
// Float (standard single-precision float)

// command unit layout

// 2 bytes command index (max 65k commands, more than enough for now)
// 1 byte quantity of parameters (in uint units)
// 1 byte additional info (flags or modifier)

// 0x0001 BEGIN_ELEMENT - 0 UNITS - 1 BYTE PRIMITIVE TYPE
// 0xFFFF END_ELEMENT - 0 UNITS - NO MODIFIERS
// 0x0002 FRAME - 4 UNITS (X, Y, W, H) - NO MODIFIERS
// 0x0003 START_POINT - 2 UNITS (X, Y) - NO MODIFIERS
// 0x0003 FILL_COLOR - 1 UNIT (32BIT COLOR) - NO MODIFIERS
// 0x0004 IMAGE_ATTACHMENT - 1 UNIT ATTACHMENT INDEX UINT32 - NO MODIFIERS

float SdBox(vec2 x, vec2 p, vec2 b) {
    vec2 d = abs(p - x)-b;
    return length(max(d,0.0)) + min(max(d.x,d.y),0.0) - 0.1;
}


float AG4Rectangle_Distance(vec2 pixel, AG4Rectangle rectangle) {
    vec2 d = abs(rectangle.frame.xy - pixel) - (rectangle.frame.zw - vec2(rectangle.cornerRadius));
    return length(max(d,0.0)) + min(max(d.x,d.y),0.0) - rectangle.cornerRadius;
}

float SdCircle(vec2 pixel, vec2 position, float radius) {
    return length(pixel - position) - radius;
}



AG4ElementResult AG4_DrawElement(uint32_t startIndex, vec2 pixel) {
    
    //AG4ElementResult returnValue = AG4ElementResult(1u, vec4(1.0), true);
    
    uint32_t index = startIndex;
    
    switch ((commands[startIndex] >> 24) & 0xFFu) {
        case AG4_ELEMENT_RECTANGLE:
           AG4Rectangle rectangle;
        
           

           
           // jump to end of BEGIN_ELEMENT (should have 0 parameters but don't assume)
           index += 1u+(commands[startIndex] >> 16) & 0xFFu;

           
           while (index < numCommands) {
           
               switch (commands[index] & 0xFFFFu) {
                   case AG4_FRAME:
                       if ((commands[index] >> 16) == 4u) {
                           rectangle.frame.x = uintBitsToFloat(commands[index + 1u]);
                           rectangle.frame.y = uintBitsToFloat(commands[index + 2u]);
                           rectangle.frame.z = uintBitsToFloat(commands[index + 3u]);
                           rectangle.frame.w = uintBitsToFloat(commands[index + 4u]);
                       } else {
                           return AG4ElementResult(0u, vec4(0.0), false);
                       }
                       break;
                   case AG4_CORNER_RADIUS:
                       if ((commands[index] >> 16) == 1u) {
                           rectangle.cornerRadius = uintBitsToFloat(commands[index + 1u]);
                       } else {
                           return AG4ElementResult(0u, vec4(0.0), false);
                       }
                       break;
                  case AG4_FILL_COLOR:
                       if ((commands[index] >> 16) == 1u) {
                           rectangle.fillColor = AG4_GetColor(commands[index + 1u]);
                       } else {
                           return AG4ElementResult(0u, vec4(0.0), false);
                       }
                       break;
                   case
                   AG4_END_ELEMENT:
                       float dist = AG4Rectangle_Distance(pixel, rectangle);//SdBox(pixel, rectangle.frame.xy, rectangle.frame.zw);
                       if (dist < 0.0) {
                           return AG4ElementResult(index+1u, rectangle.fillColor, true);
                       } else {
                          return AG4ElementResult(index+1u, vec4(0.0), true);
                       }
                   default:
                       break;
               }
               
               
               index += 1u+(commands[index] >> 16) & 0xFFu;
           }
           


           // if the program gets to here, the shape wasn't ended properly, so output an error
           return AG4ElementResult(index+1u, vec4(0.0), false);
        default: 
           return AG4ElementResult(index+1u, vec4(1.0, 0.0, 1.0, 1.0), true);

            //return AG4ElementResult(0u, vec4(0.0), false);
    }
    
    
    //return AG4ElementResult(0u, vec4(0.0), false);
}


void mainImage( out vec4 fragColor, in vec2 fragCoord ) {
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord/iResolution.xy;

    vec4 color = vec4(0.0);
    uint index = 0u;
    
    
    while (index < numCommands) {
    
        uint commandUnit = commands[index];
        uint commandIndex = commandUnit & 0xFFFFu; // Extract the command index
        uint paramCount = (commandUnit >> 16) & 0xFFu; // Extract the parameter count
        uint additionalInfo = (commandUnit >> 24) & 0xFFu; // Extract additional info
        
        switch (commands[index] & 0xFFFFu) {
            case AG4_VERSION:
                break;
            case AG4_BEGIN_ELEMENT:
                
                //fragColor = vec4(0.0, 1.0, 0.0, 1.0);
                //return;
                
                AG4ElementResult elementResult = AG4_DrawElement(index, uv);
                
                if (!elementResult.success) {
                   fragColor = vec4(0.0, 0.0, 1.0, 1.0);
                   return;
                }
                
                color += elementResult.color;
                index = elementResult.nextIndex;
                break;
                
            default:
                fragColor = vec4(1.0, 0.0, 0.0, 1.0);
                return;

        }
    }

    // Output to screen
    fragColor = clamp(color, 0.0, 1.0);
}
          