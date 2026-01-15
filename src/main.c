
#include "image_view.h"
#include "render.h"
#include "model.h"
#include <GL/gl.h>
#include "microui.h"
#include "microui_renderer.h"
#include "atlas.inl"

static SDL_Renderer *renderer = NULL;

static int text_width(mu_Font font, const char *text, int len) {
    if (len == -1) { len = strlen(text); }
    return r_get_text_width(text, len);
}

static int text_height(mu_Font font) {
    return r_get_text_height();
}

static const char button_map[256] = {
  [ SDL_BUTTON_LEFT   & 0xff ] =  MU_MOUSE_LEFT,
  [ SDL_BUTTON_RIGHT  & 0xff ] =  MU_MOUSE_RIGHT,
  [ SDL_BUTTON_MIDDLE & 0xff ] =  MU_MOUSE_MIDDLE,
};

static const char key_map[256] = {
  [ SDLK_LSHIFT       & 0xff ] = MU_KEY_SHIFT,
  [ SDLK_RSHIFT       & 0xff ] = MU_KEY_SHIFT,
  [ SDLK_LCTRL        & 0xff ] = MU_KEY_CTRL,
  [ SDLK_RCTRL        & 0xff ] = MU_KEY_CTRL,
  [ SDLK_LALT         & 0xff ] = MU_KEY_ALT,
  [ SDLK_RALT         & 0xff ] = MU_KEY_ALT,
  [ SDLK_RETURN       & 0xff ] = MU_KEY_RETURN,
  [ SDLK_BACKSPACE    & 0xff ] = MU_KEY_BACKSPACE,
};



int main(int argc, char** argv) {
    //Initialize microui 
    mu_Context *ctx = malloc(sizeof(mu_Context));
    mu_init(ctx);
    SDL_SetAppMetadata("Corolla", "0.0.0", "com.ktjst.corolla");

    ctx->text_width = text_width;
    ctx->text_height = text_height;

    mu_begin(ctx);

    int SCR_WIDTH = 1000;
    int SCR_HEIGHT = 1000;

    int mouse_x = 0;
	int mouse_y = 0;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Surface * draw_surface = NULL; 

    bool success = SDL_Init(SDL_INIT_VIDEO);
    if( !success ) {
        SDL_Log("SDL initialization failed!: %s\n", SDL_GetError());
    }

    SDL_Window *gpu_win = NULL;
    if(!SDL_CreateWindowAndRenderer("Tiny UI", SCR_WIDTH, SCR_HEIGHT, 0, &gpu_win, &renderer)) {
        SDL_Log("gpu_win init failed");
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Tiny Renderer", SCR_WIDTH, SCR_HEIGHT, 0);

        //Surface for gui
    unsigned char pixels[128*128*4];
    
    int c = 0;
    for(int i = 0; i < 128 * 128; i++) {
        pixels[c] = atlas_texture[i];
        pixels[c+1] = atlas_texture[i];
        pixels[c+2] = atlas_texture[i];
        pixels[c+3] = 255;
        c+=4;
    }
    

    SDL_Surface *sur = SDL_CreateSurfaceFrom(128, 128, SDL_PIXELFORMAT_XRGB8888, pixels, 4 * 128);
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, sur);
    
        
    bool run = true;
    
    vector3f cam_pos = {0, 0, 6};
    vector3f cam_direction = {0, 0, 0};
    vector3f cam_up = {0, 1, 0};
    
    Shader *obj_model = malloc(sizeof(Shader));
    obj_model->model = read_model_lines("./src/models/african_head.obj");
    obj_model->model->color = (vector4f) {255.0f, 255.0f, 255.0f, 255.0f};
    obj_model->model->scale = 0.5f;

    obj_model->ModelView = lookat(cam_pos, cam_direction, cam_up);
    obj_model->Perspective = perspective(norm(subtract_vec3(cam_pos, cam_direction)));
    obj_model->Viewport = viewport(SCR_WIDTH/16.f, SCR_HEIGHT/16.f, SCR_WIDTH*7.f/8.f, SCR_HEIGHT*7.f/8.f);


    Shader *cube = malloc(sizeof(Shader));
    cube->model = malloc(sizeof(Model));
    cube->model->color = (vector4f) {75.0f, 255.0f, 75.0f, 255.0f};
    cube->model->scale = 0.2f;
    int vert_count = 36;
    double vertices[108] ={
        -0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,  

        -0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f,  

        -0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,  
        -0.5f, -0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  

         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f, -0.5f,  

        -0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f, -0.5f
    };
    cube->model->vertices = malloc(vert_count * sizeof(vector3f));
    int v_i = 0;
    for(int i = 0; i < 36; i++ ) {
        
        cube->model->vertices[i].x = vertices[v_i];
        v_i++;
        cube->model->vertices[i].y = vertices[v_i];
        v_i++;
        cube->model->vertices[i].z = vertices[v_i];
        v_i++;

    }


     cube->model->vertices_size = vert_count;
    cube->model->norm_size = vert_count;

    //vert indices
    cube->model->triangles = malloc(36 * sizeof(int));
    for(int i = 0; i < 36; i++) {
        cube->model->triangles[i] = i;
    }

    
    cube->model->triangles_size = 36;
    cube->model->normals = find_normals(cube->model->vertices, cube->model->vertices_size, cube->model->triangles, cube->model->triangles_size);

    cube->ModelView = lookat(cam_pos, cam_direction, cam_up);
    cube->Perspective = perspective(norm(subtract_vec3(cam_pos, cam_direction)));

    cube->Viewport = viewport(SCR_WIDTH/16.f, SCR_HEIGHT/16.f, SCR_WIDTH*7.f/8.f, SCR_HEIGHT*7.f/8.f);


    int zbuf_size = SCR_WIDTH * SCR_HEIGHT;
    double *zbuffer = malloc(sizeof(double) * zbuf_size);
    for (int z = 0; z < zbuf_size; z++) {
        zbuffer[z] = -DBL_MAX;
    }

    Uint64 current_time ;
    Uint64 last_time;
    double dt = 0;

    cube->model->angle =0;
    float move = 0.05f;


    while (run) {



        SDL_Event event;
        last_time = current_time;
        current_time = SDL_GetPerformanceCounter();
        
        dt = (double)((current_time - last_time)*25 / (double)SDL_GetPerformanceFrequency());

        while(SDL_PollEvent(&event)) {
            switch (event.type){

            case SDL_EVENT_WINDOW_RESIZED:
                if(draw_surface) {
                    SDL_DestroySurface(draw_surface);
                }
                draw_surface = NULL;

                SCR_WIDTH = event.window.data1;
                SCR_HEIGHT = event.window.data2;
                break;
        
            case SDL_EVENT_QUIT:
                run = false;
                break;
            case SDL_EVENT_MOUSE_MOTION:
                mouse_x = event.motion.x;
                mouse_y = event.motion.y;
                break;
            case SDL_EVENT_KEY_DOWN:
                switch(event.type) {
                case SDL_EVENT_KEY_DOWN: 
                    if(event.key.key == SDLK_ESCAPE)
                    {                       
                        run = false;
                        break;
                    }
                }
                break;
            
        }
        }

        if (!run)
            break;


        if (!draw_surface) {
            draw_surface = SDL_CreateSurface( SCR_WIDTH, SCR_HEIGHT, SDL_PIXELFORMAT_RGBA32);
            SDL_SetSurfaceBlendMode(draw_surface, SDL_BLENDMODE_NONE);
        }


      //Sets color buffer for screen
        image_view color_buffer;
        color_buffer.pixels = (color4ub *)draw_surface->pixels,
        color_buffer.width = SCR_WIDTH;
        color_buffer.height = SCR_HEIGHT;
        vector4f backgroundColor = {0.7f, 1.0f, 1.0f, 1.0f};

        //Set background color
        clear(&color_buffer, &backgroundColor);

		
        
        color_buffer.at = image_view_at;


        cube->model->angle += dt;

        //printf("%f \n", dt);
        if(move > 1.0f) 
            move = -1.0f;


        // render_faces(obj_model, zbuffer, &color_buffer, 0);
        // //Reset the zbuffer
        // for (int z = 0; z < zbuf_size; z++) {
        //     zbuffer[z] = -DBL_MAX;
        // }

        render_faces(cube, zbuffer, &color_buffer, true, 0);
        //Reset the zbuffer
        for (int z = 0; z < zbuf_size; z++) {
            zbuffer[z] = -DBL_MAX;
        }

        //render_wireframe(shader->model, &color_buffer);

     if (mu_begin_window(ctx, "My Window", mu_rect(10, 10, 140, 86))) {

            mu_end_window(ctx);
        }
        mu_end(ctx);

        SDL_FRect rect;
        // UI drawing
        mu_Command *cmd = NULL;

        SDL_FRect full;
        full.x = 0;
        full.y = 0;
        full.w = 128;
        full.h = 128;
        
        SDL_FRect full2;
        full2.x = 0;
        full2.y = 0;
        full2.w = 256;
        full2.h = 256;
        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
        SDL_RenderClear(renderer);
        

         while(mu_next_command(ctx, &cmd)) {
            if(cmd->type == MU_COMMAND_TEXT) {
                SDL_FRect dest;
                dest.x = cmd->text.pos.x;
                dest.y = cmd->text.pos.y;

                for(const char *p = cmd->text.str; *p; p++) {
                    if((*p & 0xc0) == 0x80) { continue; }
                    int chr = fmin((unsigned char) *p, 127);
                    mu_Rect src = atlas[ATLAS_FONT + chr];
                    dest.w = src.w;
                    dest.h = src.h;

                    SDL_FRect srcSDL;
                    srcSDL.h = src.h;
                    srcSDL.w = src.w;
                    srcSDL.x = src.x;
                    srcSDL.y = src.y;
                    
                    SDL_RenderTexture(renderer, tex, &srcSDL, &dest);
                    dest.x += dest.w;
                }
            }
            if(cmd->type == MU_COMMAND_RECT) {
                SDL_FRect rect;
                rect.h = cmd->rect.rect.h;
                rect.w = cmd->rect.rect.w;
                rect.x = cmd->rect.rect.x;
                rect.y = cmd->rect.rect.y;

                SDL_SetRenderDrawColor(renderer, cmd->rect.color.r, cmd->rect.color.g, cmd->rect.color.b, cmd->rect.color.a);
                SDL_RenderFillRect(renderer, &rect);
                
            }
            if(cmd->type == MU_COMMAND_ICON) {
                SDL_FRect rect;
                rect.h = cmd->icon.rect.h;
                rect.w = cmd->icon.rect.w;
                rect.x = cmd->icon.rect.x;
                rect.y = cmd->icon.rect.y;

                mu_Rect r = atlas[cmd->icon.id];
                SDL_FRect srect;
                srect.h = r.h;
                srect.w = r.w;
                srect.x = r.x;
                srect.y = r.y;
                
                SDL_RenderTexture(renderer, tex, &srect, &rect);
                
            }
            if(cmd->type == MU_COMMAND_CLIP) {
                SDL_Rect clip = {
                    cmd->clip.rect.x,
                    cmd->clip.rect.y,
                    cmd->clip.rect.w,
                    cmd->clip.rect.h
                };
    
                if (cmd->clip.rect.w > 0) {
                    SDL_SetRenderClipRect(renderer, &clip);
                } else {
                    SDL_SetRenderClipRect(renderer, NULL);
                }
            }
        }
        SDL_RenderPresent(renderer);
        
        SDL_Rect draw_rect;
        draw_rect.x = 0; 
        draw_rect.y = 0; 
        draw_rect.w = SCR_WIDTH; 
        draw_rect.h = SCR_HEIGHT;
        SDL_BlitSurface(draw_surface, &draw_rect,
            SDL_GetWindowSurface(window), &draw_rect);

        SDL_UpdateWindowSurface(window);

        
    }


    
    free(zbuffer);
    free(cube->model->triangles);
    free(cube->model->vertices);
    free(cube->model->normals);

    free(cube->model);
    free(cube);
    free(obj_model->model->triangles);
    free(obj_model->model->textures);
    free(obj_model->model->vertices);
    free(obj_model->model->normals);
    free(obj_model->model);
    free(obj_model);


    SDL_DestroySurface(draw_surface);
    //SDL_DestroyEvent(event);
    SDL_DestroyWindow(window);
    SDL_Quit();



    return 0;
}