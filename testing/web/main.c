// compiled with emcc .\main.c -o index.js -sEXPORTED_RUNTIME_METHODS=ccall

#include <stdio.h>
#include <emscripten.h>

void* buttonPtr;
int numPressed = 0;

EMSCRIPTEN_KEEPALIVE
void ButtonClickCallback() {
    //EM_ASM({
    //    alert("This is a C callback 🤯");
    //});

    numPressed++;

    EM_ASM({
        var button = document.getElementById($0);
        button.innerHTML = "BUTTON WAS PRESSED " + $1 + " TIMES";
        //alert("ALERT FROM C");
    }, buttonPtr, numPressed);

}

EMSCRIPTEN_KEEPALIVE
void MouseMove(double mouseX, double mouseY) {
    printf("%f %f\n", mouseX, mouseY);

    EM_ASM({
        var button = document.getElementById("GSPTITLE");
        button.innerHTML = "" + $0 + ", " + $1;
        //alert("ALERT FROM C");
    }, mouseX, mouseY);
}


int main() {


    printf("Hello, Emscripten!\n");

    // setup callbacks
    EM_ASM({
        addEventListener("mousemove", (event) => {
            Module.ccall("MouseMove", null, ["float", "float"], [event.clientX, event.clientY])
        });
    });

     EM_ASM({
        var button = document.getElementById($0);
        button.innerHTML = "BUTTON WAS PRESSED " + $1 + " TIMES";
        //alert("ALERT FROM C");
    }, buttonPtr, numPressed);


    buttonPtr = (void*)1234;
    
    // use the pointer as the id
    EM_ASM({
        var button = document.createElement("button");
        button.id = $0; // Assign ID to the button
        button.innerHTML = "Click me";
        button.onclick = function() {

            Module.ccall("ButtonClickCallback");
        };
        document.body.appendChild(button);
        //alert(button.id);
    }, buttonPtr);

    return 0;
}
