#include<SDL.h>
#include<SDL_image.h>
#include<iostream>
#include<string>

using namespace std;

    int frame = 0,state = 5,mx = 10,d_frame = 0,d_state = 13,cont = 0,d_mx = 161,hit = 0,hit1 = 0;

   const int window_w = 1200,window_h = 640,ground_lvl = 250,d_position = 500,max_reach = 410,jump_h = 100,max_hit = 3;

    Uint32 L_time = 0, timeA = 0, timeAttack2 = 0, timehurt = 0,deathtime = 0;

    bool running = true,pause = false,d_pause = false,di_rigth = true,action = false,d_action1 = false,d_action2 = false,d_hurt = false,
    walking = false,d_death = false,death = false,jumping = false,up = false,falling = false;

//These handle where will the texture will show
SDL_Rect dst = {0,250,120*3,80*3};
SDL_Rect d_dst = {1100,150,725,445};

// Function to load texture with error checking
SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* path) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        cout << "Error: Failed to load image file '" << path << "': " << IMG_GetError() <<endl;
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
       cout << "Error: Failed to create texture from surface: "<< path << "': " << SDL_GetError() <<endl;
        SDL_FreeSurface(surface);
        return nullptr;
    }
    SDL_FreeSurface(surface);
    return texture;
}

//Handling the movement button when it is kept pressed
void handleInput(SDL_Rect& rect) {
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

   if(!action){
    if (currentKeyStates[SDL_SCANCODE_D]) {
        if(rect.x < 440)rect.x += 15;
        if(!jumping)state = 6;mx = 10;
        walking = true;
        di_rigth = true;
    }
    if (currentKeyStates[SDL_SCANCODE_A]) {
        if(rect.x > -155)rect.x -= 15;
        if(!jumping)state = 6;mx = 10;
        walking = true;
        di_rigth = false;
    }
}
}

//This is the main function
int main(int argc, char* argv[]){

    SDL_Init(SDL_INIT_VIDEO);

    //Rendering Window
    SDL_Window* window = SDL_CreateWindow("Game",30,30,window_w,window_h,0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);


    //Rendering Texture
    SDL_Texture* idle_walk = loadTexture(renderer, "texture/idle&walk(140,161).png");
    SDL_Texture* death1 = loadTexture(renderer, "texture/death.png");
    SDL_Texture* tex = loadTexture(renderer, "texture/pic.png");
    SDL_Texture* attack2 = loadTexture(renderer, "texture/attack2.png");
    SDL_Texture* attack1 = loadTexture(renderer, "texture/attack1.png");
    SDL_Texture* entity = loadTexture(renderer, "texture/knight.png");
    SDL_Texture* hurt = loadTexture(renderer, "texture/hurt.png");

    //Running game loop
    while(running){
        SDL_Event e;

        //Running the event loop to handle mouse and  button event
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT)running = false;
              Uint32  C_time = SDL_GetTicks();
            if(e.type == SDL_MOUSEBUTTONDOWN && !action &&!jumping && !death && !d_action2 && !d_hurt){
                if(e.button.button == SDL_BUTTON_LEFT){
                    frame = 0;
                    state = 2;
                    mx = 6;
                    action = true;
                     if(dst.x > max_reach&& d_dst.x <= d_position && di_rigth && !d_death){
                        d_frame = 0;
                        d_state = 0;
                        d_mx = 62;
                        cont = 0;
                        d_hurt = true;
                        hit++;
                        hit1++;
                   }
                }
                if(e.button.button == SDL_BUTTON_RIGHT){
                    frame = 0;
                    state = 3;
                    mx = 10;
                    action = true;
                     if(dst.x > max_reach && d_dst.x <= d_position && di_rigth && d_state < 4 && !d_death){
                        d_frame = 0;
                        d_state = 0;
                        d_mx = 62;
                        cont = 0;
                        d_hurt = true;
                        hit++;
                        hit1++;
                   }
                }
                L_time = C_time;
            }
            if(e.type == SDL_KEYDOWN && dst.y == ground_lvl && !death){
                if(e.key.keysym.sym == SDLK_SPACE){
                    frame = 0;
                    state = 1;
                    jumping = true;
                    up = true;
                }
            }


        }

        Uint32  timeB = SDL_GetTicks();//This has the total time the game has been running for

        //This is for the PAUSE when got killed
        if(state == 4 && frame == 9)pause = true;
        if((timeB - timehurt)>= 1000)pause = false;

        //This handles the DEATH animation of the character
        if((dst.x >max_reach && d_action1 && !d_hurt && d_state == 5 && dst.y > (ground_lvl - 50) && !death && !d_death && d_dst.x == d_position )
           || (dst.x >max_reach && d_action2 && !d_hurt && d_state == 11 && dst.y > (ground_lvl - 50) && !death &&!d_death)){
                    frame = 0;
                    state = 4;
                    mx = 10;
                    death = true;
                    di_rigth = true;
                    timehurt = timeB;
                }

        //This handles the all the other animations for the character
        if((timeB -timeA) >= 70  && !pause){

            //This changes the frames every 70 milliseconds
            if(1){
                frame++;
                if(frame >= mx){
                    frame = 0;
                    state = 5;
                    mx = 10;
                    action = false;
                    walking = false;
                    death = false;
                }
                timeA = timeB;
        }

        if(!death)handleInput(dst);//Handling the movement input

        //handling the JUMP animation
        if(jumping){
           if(up){
                frame = 2;
                if(dst.y > jump_h)dst.y -= 30;
                else {up = false; falling = true;frame = 3;}
           }
           else if(falling && !action){
                frame = 4;
                if(dst.y < ground_lvl)dst.y += 30;
                else if(death){state = 4; frame = 9;}
                else{frame = 0; state = 5; falling = false;jumping = false;}
           }
        }
        }

        //This handles the ATTACK animation for the DRAGON every 4 seconds
        if((!d_hurt && d_dst.x<=d_position && !d_action2 && !d_death)
           && (((timeB-timeAttack2) >= 5000 && dst.x <= max_reach)||((timeB-timeAttack2) >= 2000 && dst.x > max_reach ))){
            d_action1 = true;
            d_frame = 0;
            d_state = 0;
            d_mx = 161;
            cont = 0;
            timeAttack2 = timeB;
        }

        //This handles the Death animation of the Dragon
        if(hit1 >= 6){
            hit1 = 0;
            d_death = true;
            d_frame = 0;
            d_state = 0;
            d_mx = 301;
            cont = 0;
        }

        //This handles all the other animations for the DRAGON without any delay
        if( d_death && (timeB - deathtime >= 9000)){d_pause = false; deathtime = timeB;}

        if(!d_pause){
                d_frame++;
                cont++;

                if(d_frame >= 11){
                    d_frame = 0;
                    d_state++;
                }

                if( d_death && cont == 300){d_pause = true;}

        if((cont >= d_mx) || (d_hurt && d_state == 3 && hit == max_hit && !d_death)){
                    d_frame = 0;
                    d_state = 0;
                    if(hit == max_hit && d_hurt){
                        d_mx = 158;
                        hit = 0;
                        d_state = 4;
                        d_action2 = true;
                    }
                    else if(d_dst.x > d_position){d_mx = 161;d_state = 13;}
                    else{d_mx = 140;}

                    if(d_action2 && cont >= d_mx){d_action2 = false;}
                    cont = 0;

                    d_death = false;
                    d_action1 = false;
                    d_hurt = false;
                    deathtime = timeB;
                }
        }

        //Moves the Dragon
        if(d_dst.x > d_position){
                d_dst.x--;
                if(d_dst.x <= d_position){d_frame = 0;d_state = 0;d_mx = 140;cont = 0;}
        }

        SDL_RenderClear(renderer);

        //Showing the Background texture
        SDL_RenderCopy(renderer,tex,NULL,NULL);

        //Showing the DRAGON texture
        SDL_Rect d_src = {d_frame * 725,d_state * 445,725,445};
        if(d_death){SDL_RenderCopy(renderer,death1,&d_src,&d_dst);}
        else if(d_action2){SDL_RenderCopy(renderer,attack2,&d_src,&d_dst);}
        else if(d_action1 && !d_hurt && !d_action2){SDL_RenderCopy(renderer,attack1,&d_src,&d_dst);}
        else if(d_hurt){SDL_RenderCopy(renderer,hurt,&d_src,&d_dst);}
        else {SDL_RenderCopy(renderer,idle_walk,&d_src,&d_dst);}

        //Showing the character texture
        SDL_Rect src = {frame * 120,state * 80,120,80};
        if(di_rigth)SDL_RenderCopy(renderer,entity,&src,&dst);
        else {SDL_RenderCopyEx(renderer,entity, &src, &dst, 0, NULL, SDL_FLIP_HORIZONTAL);}
        SDL_RenderPresent(renderer);
    }

    //Destroying the textures and closing the window
    SDL_DestroyTexture(tex);
    SDL_DestroyTexture(idle_walk);
    SDL_DestroyTexture(attack1);
    SDL_DestroyTexture(attack2);
    SDL_DestroyTexture(entity);
    SDL_DestroyTexture(hurt);
    SDL_DestroyTexture(death1);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
return 0;
}
