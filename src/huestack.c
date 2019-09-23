#if INTERFACE
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <unistd.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define LAYERCOUNT 32

typedef struct {
  ALLEGRO_BITMAP *layers[LAYERCOUNT];
} StackTile;

typedef struct WorldObject {
  float x;
  float y;
  float z;
  float angle;
  float final_x;
  float final_y;
  float final_angle;
  float final_depth;
  StackTile* tile;
} WorldObject;

typedef struct {
  float width;
  float height;
  int screen_w;
  int screen_h;
  float camera_x;
  float camera_y;
  float rotation;
  WorldObject** objects;
  unsigned int object_count;
  unsigned int object_max;
} World;

#endif

#include "huestack.h"

void world_draw(World* world) {
  for (unsigned int i = 0; i < world->object_count; i++) {
    WorldObject* object = world->objects[i];
    float fixed_angle = object->angle + world->rotation;
    float relative_x = object->x - world->camera_x;
    float relative_y = object->y - world->camera_y;
    float new_x = relative_x * cos(world->rotation) - sin(world->rotation) * relative_y + world->camera_x;
    float new_y = relative_x * sin(world->rotation) + cos(world->rotation) * relative_y + world->camera_y;
    //printf("%f %f \n", new_x, new_y);
    float final_x = new_x + world->screen_w / 2 - world->camera_x;
    float final_y = new_y + world->screen_h / 2 - world->camera_y;
    //stacktile_draw(object->tile, final_x, final_y, fixed_angle);
    object->final_x = final_x;
    object->final_y = final_y;
    object->final_angle = fixed_angle;
    object->final_depth = final_y - object->z * 40;
  }
  unsigned char sorted = 0;
  while (sorted == 0) {
    sorted = 1;
    for (unsigned int i = 0; i < world->object_count - 1; i++) {
      if (world->objects[i]->final_depth > world->objects[i+1]->final_depth) {
        sorted = 0;
        WorldObject* temp = world->objects[i];
        world->objects[i] = world->objects[i+1];
        world->objects[i+1] = temp;
      }
    }
  }
  for (unsigned int i = 0; i < world->object_count; i++) {
    WorldObject* object = world->objects[i];
    stacktile_draw(object->tile, object->final_x, object->final_y, object->final_angle);
  }
}

void world_addObject(World* world, WorldObject* object) {
  if (world->object_count == world->object_max) {
    world->object_max += 32;
    WorldObject** newObjects = malloc(sizeof(WorldObject*) * world->object_max);
    if (world->objects != NULL) {
      for (unsigned int i = 0; i < world->object_count; i++) {
        newObjects[i] = world->objects[i];
      }
      free(world->objects);
    }
    world->objects = newObjects;
  }
  world->objects[world->object_count] = object;
  world->object_count++;
}

WorldObject *newWorldObject(float x, float y, float z, float angle, StackTile* tile) {
  WorldObject *object = malloc(sizeof(*object));
  object->x = x;
  object->y = y;
  object->z = z;
  object->angle = angle;
  object->tile = tile;
  return object;
}

World *newWorld(float width, float height, int screen_w, int screen_h) {
  World *world = malloc(sizeof(*world));
  world->width = width;
  world->height = height;
  world->screen_w = screen_w;
  world->screen_h = screen_h;
  world->object_count = 0;
  world->object_max = 0;
  world->objects = NULL;
  return world;
}

StackTile *newStackTile(char* name_base) {
  StackTile *result = malloc(sizeof(*result));
  unsigned char name_len = strlen(name_base) + 7;
  for (unsigned char i = 0; i < LAYERCOUNT; i++) {
    char* buffer = malloc(name_len*sizeof(char));
    snprintf(buffer, name_len, "%s%02d.png", name_base, i);
    printf("%s\n", buffer);
    if(access(buffer, F_OK) != -1) {
      result->layers[i] = al_load_bitmap(buffer);
      if(result->layers[i] == NULL) {
        printf("Failed to load %02d\n", i);
      }
    } else {
      free(buffer);
      break;
    }
    free(buffer);
  }
  return result;
}

void stacktile_draw(StackTile* target, float x, float y, float angle) {
  for (unsigned char i = 0; i < LAYERCOUNT; i++) {
    if (target->layers[i] != NULL) {
      //al_draw_scaled_rotated_bitmap(target->layers[i], 0, 0, x, y + 10, 10, 10, angle, 0);
      float w = al_get_bitmap_width(target->layers[i]);
      float h = al_get_bitmap_height(target->layers[i]);
      al_draw_scaled_rotated_bitmap(target->layers[i], w/2, h/2, x, y - (1 * i), 1, 1, angle, 0);
    }
    else {
      break;
    }
  }
}
