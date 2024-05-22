#include "GSPCore/GWindow.h"
#include "internal/def/GWindowDef.h"

#include "GSPCore/UI/GView.h"

#include <stdlib.h>
#include <stdio.h>

#include <emscripten.h>

void GWindow_SetRootView(GView view) {
    printf("SETTING ROOT VIEW\n");

    EM_ASM({

        const canvas = document.createElement('canvas');
        const context = canvas.getContext('2d');
        canvas.width = 10;
        canvas.height = 10;

        view = document.createElement('div');
        view.id = $0;
        view.style.top = "0";
        view.style.left = "0";
        view.style.width = "100vw";
        view.style.height = "100vh";
        view.style.margin = "0";
        view.style.position = "fixed";
        view.style.backgroundColor = "black";
        view.style.display = "flex";

        // example for wrap style
        view.style.flexWrap = "wrap";
        view.style.alignContent = "flex-start";
        view.style.justifyContent = "flex-start";

        document.body.appendChild(view);

        for (let iter = 0; iter < 100; iter++) { 
            var child = document.createElement('div');
            child.style.width = "10pt";
            child.style.height = "10pt";
            child.style.display = "block";

            var letters = '0123456789ABCDEF';
            var color = '#';
            for (var i = 0; i < 6; i++) {
                color += letters[Math.floor(Math.random() * 16)];
            }
            
            context.fillStyle = color;
            context.fillRect(0, 0, canvas.width, canvas.height);

            var dataURL = canvas.toDataURL();
            child.style.backgroundImage = `url(${dataURL})`;

            //child.style.backgroundColor = color;
            child.style.overfloat = "hidden";

            //child.style.flexGrow = "1";

            view.appendChild(child);
         }
    

        
    }, view);

}