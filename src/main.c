#include <stdio.h>
#include "engine.h"
#include "huestack.h"

StackTile* testTile;
float angle = 0.0;
ALLEGRO_BITMAP *screen;
Engine* testEngine;
WorldObject* testObject;
WorldObject* testObject1;
WorldObject* testObject2;
WorldObject* testObject3;
World* testWorld;

void update(double dt) {
  angle += 0.01;
}

void draw() {
  al_set_target_bitmap(screen);
  al_clear_to_color(al_map_rgb(20, 20, 20));
  //stacktile_draw(testTile, 120, 120, angle);
  testWorld->rotation = angle;
  world_draw(testWorld);
  al_set_target_bitmap(al_get_backbuffer(testEngine->display));
  al_draw_scaled_bitmap(screen, 0, 0, 320, 240, 0, 0, 640, 480, 0);
}



int main(int argc, char* argv[]) {
  testEngine = newEngine(640, 480);
  testEngine->on_draw = draw;
  testEngine->on_update = update;
  testWorld = newWorld(32 * 32, 32 * 32, 320, 240);
  screen = al_create_bitmap(320, 240);
  testTile = newStackTile("assets/grass-");
  testObject = newWorldObject(16, 16, 0, 0, testTile);
  testObject1 = newWorldObject(-16, -16, 0, 0, testTile);
  testObject2 = newWorldObject(16, -16, 0, 0, testTile);
  testObject3 = newWorldObject(-16, 16, 0, 0, testTile);
  testWorld->camera_x = 40;
  testWorld->camera_y = 40;
  world_addObject(testWorld, testObject);
  world_addObject(testWorld, testObject1);
  world_addObject(testWorld, testObject2);
  world_addObject(testWorld, testObject3);
  engine_run(testEngine);
}
