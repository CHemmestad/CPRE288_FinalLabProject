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
    int numNotes = 4; //Number of notes in your sound, MAX 64
    unsigned char notes[4] = {64, 64, 64, 64}; //The notes that being played, 64 is an A
    unsigned char duration[4] = {32, 32, 32, 32}; //The length the note is being played, 32 is a regular note length
    oi_loadSong(index, numNotes, notes, duration);
    oi_play_song(index);
    free(sensor_data);
}

void objectDetectedSound() {
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    int index = 0;
    int numNotes = 6; //Number of notes in your sound, MAX 64
    unsigned char notes[4] = {61, 61, 61, 61}; //The notes that being played, 64 is an A
    unsigned char duration[4] = {40, 40, 40, 40}; //The length the note is being played, 32 is a regular note length
    oi_loadSong(index, numNotes, notes, duration);
    oi_play_song(index);
    free(sensor_data);
}

void dogDetectedSound() {
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
int index = 0;
   int numNotes = 4; //Number of notes in your sound, MAX 64
   unsigned char notes[4] = {57, 57, 57, 57}; //The notes that being played, 64 is an A
   unsigned char duration[4] = {32, 47, 55, 57}; //The length the note is being played, 32 is a regular note length
   oi_loadSong(index, numNotes, notes, duration);
   oi_play_song(index);
   free(sensor_data);
}

void bumpSound() {
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    int index = 0;
    int numNotes = 10; //Number of notes in your sound, MAX 64
    unsigned char notes[10] = {71, 81, 77, 76, 81, 77, 76, 72, 80, 78}; //The notes that being played, 64 is an A
    unsigned char duration[10] = {8, 8, 8, 8, 16, 16, 16, 8, 8, 8}; //The length the note is being played, 32 is a regular note length
    oi_loadSong(index, numNotes, notes, duration);
    oi_play_song(index);
    free(sensor_data);
}

void holeSound() {
    oi_t *sensor_data = oi_alloc();
        oi_init(sensor_data);
        int index = 0;
        int numNotes = 6; //Number of notes in your sound, MAX 64
        unsigned char notes[6] = {71, 81, 61, 61, 67, 66}; //The notes that being played, 64 is an A
        unsigned char duration[6] = {8, 8, 4, 8, 16, 16}; //The length the note is being played, 32 is a regular note length
        oi_loadSong(index, numNotes, notes, duration);
        oi_play_song(index);
        free(sensor_data);
}

void finishSound() {
    oi_t *sensor_data = oi_alloc();
        oi_init(sensor_data);
        int index = 0;
        int numNotes = 10; //Number of notes in your sound, MAX 64
        unsigned char notes[12] = {74, 74, 81, 74, 74, 83, 74, 74, 81, 74, 74, 80}; //The notes that being played, 64 is an A
        unsigned char duration[12] = {8, 8, 16, 8, 8, 16, 8, 8, 16, 8, 8, 16}; //The length the note is being played, 32 is a regular note length
        oi_loadSong(index, numNotes, notes, duration);
        oi_play_song(index);
        free(sensor_data);
}

void startSound() {
    oi_t *sensor_data = oi_alloc();
        oi_init(sensor_data);
        int index = 0;
        int numNotes = 31; //Number of notes in your sound, MAX 64
        unsigned char notes[31] = {71, 83, 78, 76, 83, 77, 76, 74, 80, 78, 72, 77, 81, 77, 71, 81, 77, 76, 74, 76, 77, 77, 76, 76, 78, 80, 80, 80, 81, 81, 83}; //The notes that being played, 64 is an A
        unsigned char duration[31] = {8, 8, 8, 8, 4, 8, 16, 8, 8, 8, 8, 4, 8, 16, 8, 8, 8, 8, 4, 8, 16, 4, 8, 8, 4, 8, 8, 4, 8, 8, 16}; //The length the note is being played, 32 is a regular note length
        oi_loadSong(index, numNotes, notes, duration);
        oi_play_song(index);
        free(sensor_data);
}


void playMarioSoundEffect()
{
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);

    int index = 0;

    int numNotes = 9;
    unsigned char notes[] = {
        84, 83, 81, 79, 78, 76, 79, 78, 76};

    unsigned char durations[] = {8, 8, 16, 16, 16, 32, 16, 16, 32};

    oi_loadSong
}
