#include "engine.h"

#if INTERFACE
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <stdbool.h>
#include <stdio.h>
typedef struct {
  int screen_width;
  int screen_height;
  bool (*on_event)(ALLEGRO_EVENT event);
  void (*on_update)(double dt);
  void (*on_draw)();
  ALLEGRO_DISPLAY *display;
  ALLEGRO_EVENT_QUEUE *queue;
  ALLEGRO_TIMER *timer;
} Engine;
#endif

bool __engine_on_event(ALLEGRO_EVENT event) { return false; }
void __engine_on_update(double dt) {}
void __engine_on_draw() { al_clear_to_color(al_map_rgb(16, 128, 16)); }

Engine *newEngine(int screen_width, int screen_height) {
  Engine *engine = malloc(sizeof(*engine));
  engine->screen_width = screen_width;
  engine->screen_height = screen_height;
  engine->on_event = __engine_on_event;
  engine->on_update = __engine_on_update;
  engine->on_draw = __engine_on_draw;

  al_init();
  al_init_image_addon();
  al_init_primitives_addon();
  al_init_font_addon();
  al_init_ttf_addon();

  engine->display = al_create_display(engine->screen_width, engine->screen_height);
  engine->queue = al_create_event_queue();
  engine->timer = al_create_timer(1.0 / 60.0);

  al_install_keyboard();
  al_install_mouse();
  al_register_event_source(engine->queue, al_get_keyboard_event_source());
  al_register_event_source(engine->queue, al_get_display_event_source(engine->display));
  al_register_event_source(engine->queue, al_get_mouse_event_source());
  al_register_event_source(engine->queue, al_get_timer_event_source(engine->timer));

  al_start_timer(engine->timer);

  return engine;
}

int engine_run(Engine *engine) {


  double dt = 0;
  double old_time = al_get_time();

  ALLEGRO_EVENT event;
  bool running = true;
  while (running) {

    al_wait_for_event(engine->queue, &event);
    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
      running = false;
    }

    if (event.type == ALLEGRO_EVENT_TIMER) {

      engine->on_update(dt);

      al_clear_to_color(al_map_rgb(20, 20, 20));

      engine->on_draw();
      al_flip_display();
      double new_time = al_get_time();
      dt = new_time - old_time;
      double fps = 1/dt;
      old_time = new_time;
      printf("%f\n", fps);
    }
    engine->on_event(event);
  }

  al_stop_timer(engine->timer);
  al_destroy_timer(engine->timer);
  al_uninstall_keyboard();
  al_destroy_event_queue(engine->queue);
  al_destroy_display(engine->display);
  al_shutdown_primitives_addon();
  al_uninstall_system();

  return 0;
}
