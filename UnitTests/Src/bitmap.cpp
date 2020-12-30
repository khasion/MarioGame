#include "bitmap.hpp"

ALLEGRO al;

Bitmap dpyBuffer=nullptr;
bool dpyChanged=true;
Dim dpyX=0, dpyY=0;

// Display routines
bool Open (Dim rw, Dim rh, BitDepth depth) {
	/*al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
	al_set_new_display_flags(ALLEGRO_FRAMELESS);*/
	al_set_new_display_option(ALLEGRO_COLOR_SIZE, depth, ALLEGRO_SUGGEST);
	ALLEGRO_DISPLAY* disp = al_create_display(rw, rh);
	al.disp = disp;
	al.monitor = (ALLEGRO_MONITOR_INFO*) malloc(sizeof(ALLEGRO_MONITOR_INFO));
	al_get_monitor_info(0, al.monitor);
	return disp? true : false;
}

void Close (void) {
	return;
}

Dim GetResWidth (void) {
	return al_get_display_width(al.disp);
}

Dim GetResHeight (void) {
	return al_get_display_height(al.disp);
}

BitDepth GetDepth (void) {
	return (BitDepth) al_get_display_option(al.disp, ALLEGRO_COLOR_SIZE);
}
// Bitmap routines
Bitmap BitmapLoad (const std::string& path) {
	return al_load_bitmap(path.c_str());
}

Bitmap BitmapCreate (Dim w, Dim h) {
	return al_create_bitmap(w, h);
}

Bitmap BitmapCopy (Bitmap bmp) {
	return al_clone_bitmap((ALLEGRO_BITMAP*)bmp);
}

Bitmap BitmapClear (Bitmap bmp, RGB c) {
	Bitmap prev = al_get_target_bitmap();
	al_set_target_bitmap((ALLEGRO_BITMAP*)bmp);
	al_clear_to_color(al_map_rgb(c.r, c.g, c.b));
	al_set_target_bitmap((ALLEGRO_BITMAP*)prev);
	return bmp;
}

void BitmapDestroy (Bitmap bmp) {
	al_destroy_bitmap((ALLEGRO_BITMAP*) bmp);
}

Bitmap BitmapGetScreen (void) {
	return al_get_target_bitmap();
}

Dim BitmapGetWidth (Bitmap bmp) {
	return al_get_bitmap_width((ALLEGRO_BITMAP*)bmp);
}

Dim BitmapGetHeight (Bitmap bmp) {
	return al_get_bitmap_height((ALLEGRO_BITMAP*)bmp);
}

void BitmapBlit (Bitmap src, const Rect& from, Bitmap dest, const Point& to) {
	Bitmap prev = al_get_target_bitmap();
	al_set_target_bitmap((ALLEGRO_BITMAP*)dest);
	al_draw_bitmap_region((ALLEGRO_BITMAP*)src, from.x, from.y, from.w, from.h, to.x, to.y, 0);
	al_set_target_bitmap((ALLEGRO_BITMAP*)prev);
}

