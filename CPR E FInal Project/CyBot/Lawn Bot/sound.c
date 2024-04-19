/*
 * sound.c
 *
 *  Created on: Apr 18, 2024
 *      Author: cihem
 */
#include "sound.h"
#include "open_interface.h"

void playZelda() {
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    int index = 0;
    int numNotes = 63;
    unsigned char notes[63] = {69, 69, 69, 69, 69, 67, 69, 0, 69, 69, 69, 69, 69, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 69, 64, 69, 69, 71, 73, 74,
    76, 0, 76, 76, 77, 79, 81, 0, 81, 81, 79, 77, 79, 77, 76, 76, 74, 74, 76, 77, 76, 74, 72, 72, 74, 76, 74, 72, 71, 71, 73, 75, 78};
    unsigned char duration[63] = {16, 16, 16, 16, 32, 16, 32, 32, 16, 16, 16, 16, 16, 8, 8, 16, 8, 8, 16, 8, 8, 16, 16, 32, 50, 16, 8, 8, 8, 8,
    64, 16, 16, 16, 16, 16, 64, 16, 16, 16, 16, 16, 24, 8, 64, 32, 16, 8, 8, 64, 16, 16, 16, 8, 8, 64, 16, 16, 16, 8, 8, 64, 32};
    oi_loadSong(index, numNotes, notes, duration);
    oi_play_song(index);
    free(sensor_data);
}

void template() { //copy and paste template to add your own sound
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    int index = 0;
    //******************************************
    //Only change these three lines
    int numNotes = 4; //Number of notes in your sound, MAX 64
    unsigned char notes[4] = {64, 64, 64, 64}; //The notes that are being played, 69 is an A
    unsigned char duration[4] = {32, 32, 32, 32}; //The length of the note that is being played, 32 is a regular note length
    //******************************************
    oi_loadSong(index, numNotes, notes, duration);
    oi_play_song(index);
    free(sensor_data);
}
