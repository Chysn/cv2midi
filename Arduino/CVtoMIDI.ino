/* CV to MIDI
 * The Beige Maze Project
 * 
 * (c) 2017, Chysn
 *
 * This software may be freely shared, used, and distributed for any purpose, 
 * private or commercial, good or evil. See specific MIT License information
 * in LICENSE.
 *
 * Please see Documentation.txt for hardware build specifications. 
 *
 * When the gate (pin 7) goes high, the voltage at A0 is read, and quantized
 * to a MIDI note within a three-octave range (the octave below middle C
 * through two octaves above middle C), and a note on message is transmistted 
 * on MIDI channel 1.
 *
 * When the gate goes low again, a note off message is transmitted.
 *
 * NOTE: The Arduino's on-board pin 13 LED is activated when the gate 
 * is on. You may optionally connect an external LED on this pin.
 *
 * NOTE: Proofs-of-concept for handling velocity CV failed on my hardware.
 * The analog reads are multi-plexing and don't refresh fast enough, so
 * voltages measured at analog pins is usually affected by voltage
 * measured at the other pins. If there's a solution to this with the
 * on-board ADC, I couldn't find it.
 *
 */

int gate_state = 0;
int last_note = 0;

void setup() {
  Serial.begin(31250); /* MIDI 31250 baud */
  pinMode(13, OUTPUT);
}

void loop() {
  int gate = digitalRead(7);
  
  /* Turn the gate on if it's currently off */
  if (!gate_state && gate == HIGH) {
    gate_state = 1;

    /* Range is about 3 octaves from the octave below middle C */
    int cv1 = analogRead(A0);
    int note = (cv1 / 28) + 48; 
    last_note = note;

    MIDInote(note, 127);
    digitalWrite(13, HIGH // On-board LED on
  }
    
  /* Turn the gate off if it's on */
  if (gate_state && gate == LOW) {
    gate_state = 0;
    MIDInote(last_note, 0); /* Note off */
    digitalWrite(13, LOW); /* LED off */
  }
}

/* Send a MIDI note-on message on channel 1 */
void MIDInote(int note, int vel) {
  char status_msg = vel ? 0x90 : 0x80; /* Velocity = 0 sends Note Off */
  Serial.write(status_msg); /* Note on or off */
  Serial.write(note);
  Serial.write(vel);
}

