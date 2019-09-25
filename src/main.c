#include <stdio.h>
#include "engine.h"
#include "huestack.h"

StackTile* testTile;
StackTile* tank;
float angle = 0.0;
Engine* testEngine;
WorldObject* tankObject;
WorldObject* testObject;
WorldObject* testObject1;
WorldObject* testObject2;
WorldObject* testObject3;
World* testWorld;

void update(double dt) {
  angle += 0.01;
}

void draw() {
  testWorld->rotation = angle;
  world_draw(testWorld);
}



int main(int argc, char* argv[]) {
  testEngine = newEngine(640, 480);
  testEngine->on_draw = draw;
  testEngine->on_update = update;
  testWorld = newWorld(640, 480);
  testTile = newStackTile("assets/grass-");
  tank = newStackTile("assets/bettertank-");
  testObject = newWorldObject(32, 32, 0, 0, testTile);
  tankObject = newWorldObject(0, 0, 32, 0, tank);
  testObject1 = newWorldObject(-32, -32, 0, 0, testTile);
  testObject2 = newWorldObject(32, -32, 0, 0, testTile);
  testObject3 = newWorldObject(-32, 32, 0, 0, testTile);
  testWorld->camera_x = 0;
  testWorld->camera_y = 100;
  testWorld->offset_x = 0;
  testWorld->offset_y = 100;
  world_addObject(testWorld, testObject);
  world_addObject(testWorld, testObject1);
  world_addObject(testWorld, testObject2);
  world_addObject(testWorld, testObject3);
  world_addObject(testWorld, tankObject);
  engine_run(testEngine);
}
